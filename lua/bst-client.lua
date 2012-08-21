-- -*- mode: lua; coding: utf-8 -*-

-- a control connection to the emacs side (emacs/bst-server.el). 'bst' is pretty much a random string, only to have something unique to search for.

json = require("dkjson")

bst = {}
bst.host = "37.99.56.117"
bst.port = 9999
bst.socket = nil
bst.mainThread = nil
bst.timeout = 30
bst.networkBuffer = ""
bst.messageQueue = {}
bst.connectedState = false
bst.eomMarker = "\n\n\n"

bst.isConnected = function ()
   if bst.connectedState then
      if bst.socket and platform.gprs.isSocketConnected(bst.socket) then
         return true
      else
         bst.connectedState = false
         bst.socket = nil
         bst.mainThread = nil
         return false
      end
   else
      return false
   end
end

bst.ensureConnected = function ()
   if not bst.isConnected() then
      bst.connect()
   end
end

bst.connect = function (timeout)
   if bst.isConnected() or isThreadAlive(bst.mainThread) then
      errorWithFormat("there's already a bst control thread, can't have multiple connections")
   end
   local timeout = timeout or bst.timeout
   platform.gprs.ensureGPRSConnected()
   bst.mainThread = newThread(
      function ()
         platform.display.printXY(1, 8, "BST connecting...")
         bst.socket = platform.gprs.connect(bst.host, bst.port, timeout)
         yield()
         bst.sendMessage(bst.socket,
                         {
                            request = "hello bst!",
                            serial = 1234,
                            foolabel = "foovalue"
                         },
                         timeout)
         platform.display.printXY(1, 8, "BST read message...")
         local response = bst.readMessage(timeout)
         if response.result == true then
            bst.connectedState = true
         else
            platform.gprs.disconnect(bst.socket)
            errorWithFormat("bst server returned '%s'?!", result)
         end
         platform.display.printXY(1, 8, "BST looping...")
         bst.messageLoop()
         platform.display.printXY(1, 8, "BST done...")
         bst.disconnect()
      end)
end

bst.disconnect = function ()
   if bst.isConnected() then
      platform.gprs.disconnect(bst.socket)
      bst.connectedState = false
      bst.socket = nil
      bst.mainThread = nil
   end
end

bst.sendMessage = function(socket, object, timeout)
   local timeout = timeout or bst.timeout
   platform.display.printXY(1, 8, "BST sending...")
   platform.gprs.send(bst.socket, json.encode(object) .. bst.eomMarker, timeout)
end

bst.messageLoop = function()
   while bst.isConnected() do
      local message = bst.readMessage(0)
      if message.request == "quit" then
         return "done"
      else
         newFullscreenThread(function ()
                                platform.display.clear()
                                displayMultilineText(tableToString(message))
                             end)
      end
   end
end

bst.receiveSomeMessages = function ()
   if bst.isConnected() then
      local chunk = platform.gprs.receive(bst.socket, nil, 0.1, false)
      bst.networkBuffer = bst.networkBuffer .. chunk
      while true do
         local messageEndPosition = string.find(bst.networkBuffer, bst.eomMarker, 1, true)
         if not messageEndPosition then
            break
         end
         local rawMessageText = string.sub(bst.networkBuffer, 1, messageEndPosition - 1)
         bst.networkBuffer = string.sub(bst.networkBuffer, messageEndPosition + #bst.eomMarker)
         local message, pos, err = json.decode(rawMessageText, 1, nil, nil)
         if err then
            displayMultilineText(string.format("Error while decoding BST message:\n%s", err))
         else
            table.insert(bst.messageQueue, message)
         end
      end
   end
   return bst.messageQueue
end

bst.readMessage = function (timeout)
   local timeout = timeout or bst.timeout
   local startTime = platform.getMonotonicTime()
   while bst.isConnected() do
      if timeout > 0 and platform.getMonotonicTime() - startTime > timeout then
         errorWithFormat("bst connection timed out")
      end
      local messages = bst.receiveSomeMessages()
      if #messages > 0 then
         platform.display.printXY(1, 8, "BST read a message...")
         return table.remove(messages, 1)
      else
         yield()
      end
   end
end
