-- -*- mode: lua; coding: utf-8 -*-

-- a control connection to the emacs side (emacs/bst-server.el, uses json), through which you can evaluate lua code on the device or redefine lua functions dynamically. 
-- for now 'bst' is pretty much a random string, only to have something unique to search for throughout the codebase.

json = require("dkjson")

bst = {}

bst.theConnection = nil -- should be a connectionList, but let's keep stuff simple for now
bst.eomMarker = "\nQoOqmr5+ThM=\n" -- random string that needs to match the elisp side

--
-- Connection
--

bst.Connection = {}

function bst.Connection:new ()
   if bst.theConnection and bst.theConnection:isConnected() then
      errorWithFormat("there's already a bst connection, can't have multiple connections")
   end
   o = {}
   setmetatable(o, self)
   self.__index = self
   o.messageQueue = {}
   o.networkBuffer = ""
   return o
end

function bst.Connection:disconnect ()
   bst.theConnection = nil
end

function bst.Connection:receiveSomeMessages ()
   local chunk = self:receiveBytes(nil)
   self.networkBuffer = (self.networkBuffer or "") .. chunk
   while true do
      local messageEndPosition = string.find(self.networkBuffer, bst.eomMarker, 1, true)
      if not messageEndPosition then
         break
      end
      local rawMessageText = string.sub(self.networkBuffer, 1, messageEndPosition - 1)
      self.networkBuffer = string.sub(self.networkBuffer, messageEndPosition + #bst.eomMarker)
      local message, pos, err = json.decode(rawMessageText, 1, nil, nil)
      if err then
         displayMultilineText(string.format("Error while decoding BST message:\n%s", err))
      else
         table.insert(self.messageQueue, message)
      end
   end
   return self.messageQueue
end

function bst.Connection:sendMessage (object)
   platform.display.printXY(1, 8, "BST sending...")
   self:sendBytes(json.encode(object) .. bst.eomMarker)
end

function bst.Connection:mainThread ()
   platform.display.printXY(1, 8, "BST connecting...")
   self:sendBytes(bst.eomMarker) -- we start with an EOM marker to sync with the server
   self:sendMessage({
                       type = "bst connect request",
                       serial = 1234, -- TODO delme
                       foolabel = "foovalue"
                    })
   platform.display.printXY(1, 8, "BST read message...")
   local response = self:readMessage()
   if response.type == "bst connect response" then
      self.connectedState = true
   else
      self:disconnect()
      errorWithFormat("bst server returned '%s'?!", response)
   end
   platform.display.printXY(1, 8, "BST looping...")
   local isSuccess, error = pcall(self.messageLoop, self)
   if isSuccess == false then
      displayMessage("BST error:\n" .. error)
   end
   self:disconnect()
end

bst.messageHandlers = {}

bst.messageHandlers["eval request"] = function (message)
   local form = message.form
   local response = { type = "eval response" }
   local isLoaded, fn = pcall(load, form)
   if isLoaded then
      local isRun, returnValue = pcall(fn)
      if isRun then
         response["result"] = true
         response["return-value"] = returnValue
      else
         response["result"] = false
         response["error"] = returnValue
      end
   else
      response["result"] = false
      response["error"] = fn
   end
   return response
end

bst.messageHandlers["ping"] = function (message)
   return { type = "pong" }
end

function bst.Connection:messageLoop ()
   while self:isConnected() do
      local message = self:readMessage(0) -- pop me a message if there's one read already
      local handler = bst.messageHandlers[message.type]

      if message.type == "disconnect" then
         self.connectedState = false
         return "done"
      elseif handler then
         local response = handler(message)
         self:sendMessage(response)
      else
         displayMessage("Unknown BST msg:\n" .. tableToString(message))
      end
      yield()
   end
end

function bst.Connection:readMessage (timeout)
   local timeout = timeout or self.timeout
   local startTime = platform.getMonotonicTime()
   while true do
      if timeout > 0 and platform.getMonotonicTime() - startTime > timeout then
         errorWithFormat("bst readMessage timed out")
      end
      local messages = self:receiveSomeMessages()
      if #messages > 0 then
         platform.display.printXY(1, 8, "BST has read a message...")
         return table.remove(messages, 1)
      else
         yield()
         platform.sleep(0.2) -- FIXME ideally the receive functions should be able to yield if there's no data
      end
   end
end

--
-- IpConnection
--

bst.IpConnection = bst.Connection:new ()

function bst.IpConnection:new (host, port, timeout)
   o = bst.Connection.new(self)
   o.host = host
   o.port = port
   o.timeout = timeout
   platform.gprs.ensureGPRSConnected()
   yield()
   o.socket = platform.gprs.connect(host, port, timeout)
   o.mainThreadObject = newThread(
      function ()
         o:mainThread()
      end)
   bst.theConnection = o
   return o
end

function bst.IpConnection:disconnect ()
   platform.gprs.disconnect(self.socket)
   bst.Connection.disconnect(self)
end

function bst.IpConnection:isConnected ()
   return self.socket and platform.gprs.isSocketConnected(self.socket)
end

function bst.IpConnection:sendBytes (data, timeout)
   return platform.gprs.send(self.socket, data, timeout or self.timeout)
end

function bst.IpConnection:receiveBytes (length)
   return platform.gprs.receive(self.socket, length, 0.1, false)
end

--
-- SerialConnection
--

bst.SerialConnection = bst.Connection:new ()

function bst.SerialConnection:new (port, timeout)
   port = port or platform.serial.defaultPort
   o = bst.Connection.new(self)
   o.port = port
   o.timeout = timeout or 5
   local success, errorMessage = platform.serial.connect(port)
   if not success then
      error("Serial connect failed: " .. errorMessage)
   end
   o.mainThreadObject = newThread(
      function ()
         o:mainThread()
      end)
   bst.theConnection = o
   return o
end

function bst.SerialConnection:disconnect ()
   platform.serial.disconnect(self.port)
   bst.Connection.disconnect(self)
end

function bst.SerialConnection:isConnected ()
   return true
end

function bst.SerialConnection:sendBytes (data)
   return platform.serial.send(self.port, data)
end

function bst.SerialConnection:receiveBytes (length)
   return platform.serial.receive(self.port, length)
end
