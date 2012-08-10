-- -*- mode: lua; coding: utf-8 -*-

-- a control connection to the emacs side (etc/emacs-bst-server.el). bst is only a random string to have something unique to search for.

bst = {}
bst.host = "87.247.56.157"
bst.port = 9999
bst.socket = 1
bst.timeout = 10
bst.networkBuffer = ""
bst.messageBuffer = {}
bst.isConnected = false

bst.isConnected = function ()
   local state = wls.CheckMTcpLink(bst.socket)
   return state == wls.OK and bst.isConnected
end

bst.ensureConnected = function ()
   if not bst.isConnected() then
      bst.connect()
   end
end

bst.connect = function ()
   platform.gprs.ensureConnected()
   wls.MTcpConnect(bst.socket, bst.host, bst.port)
   yield()
   wls.MTcpSend(bst.socket, string.format("hello bst!\nserial:1234\nfoolabel: foovalue\n\n"))
   local response = bst.readMessage()
   if response == "done" then
      bst.isConnected = true
   else
      wls.MTcpClose(bst.socket)
      throwUserMessage("bst server returned '%s'?!", result)
   end
end

bst.disconnect = function ()
   if bst.isConnected() then
      wls.MTcpClose(bst.socket)
      bst.isConnected = false
   end
end

bst.readSomeMessages = function ()
   local chunk = wls.MTcpRecv(bst.socket, 512, 0.1)
   bst.networkBuffer = bst.networkBuffer..chunk
   while true do
      local messageEndPosition = string.find(bst.networkBuffer, "\n\n", 1, true)
      if not messageEndPosition then
         break
      end
      local message = string.sub(bst.networkBuffer, 1, messageEndPosition - 1)
      bst.networkBuffer = string.sub(bst.networkBuffer, messageEndPosition + 2)
      table.insert(bst.messageBuffer, message)
   end
   return bst.messageBuffer
end

bst.readMessage = function (timeout)
   local timeout = timeout or bst.timeout
   local startTime = platform.getMonotonicTime()
   while true do
      if platform.getMonotonicTime() - startTime > timeout then
         error("bst connection timed out")
      end
      bst.readSomeMessages()
      if #bst.messageBuffer > 0 then
         return table.remove(bst.messageBuffer, 1)
      else
         yield()
      end
   end
end
