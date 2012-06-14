-- -*- mode: lua; coding: utf-8 -*-

-- global variables
threads = {}
gprsConnectThread = nil
gprsStatusThread = nil
mainThread = nil

function newThread(trunk)
   local co = coroutine.create(function ()
                                  local errorMessage = nil
                                  xpcall(trunk,
                                         function (originalMessage)
                                            errorMessage = debug.traceback(tostring(originalMessage, 1))
                                            return originalMessage -- no clue what happens here, just return the original message
                                         end)
                                  if errorMessage then
                                     error(errorMessage)
                                  else
                                     return "done"
                                  end
                               end)
--   local co = coroutine.create(trunk)
   table.insert(threads, co)
   return co
end

function isThreadAlive(thread)
   return thread and coroutine.status(thread) ~= "dead"
end

function isToplevelThread()
   return select(2, coroutine.running())
end

function waitForEvents()
   -- ? platform.getMonotonicTime()
   if isToplevelThread() then
      platform.sleep(0.1)
   else
      yield()
   end
end

-- logging
log = {}
log.serialPort = 1
log.debug =
   function (message)
     if log.connected then
        platform.serial.send(log.serialPort, message)
     end
   end

log.initialize = function (serialPort)
   log.serialPort = serialPort or platform.serial.defaultPort
   local success, errorMessage = platform.serial.connect(log.serialPort)
   log.connected = success
   return success, errorMessage
end

platform.getDateString = function (dateTableGetter)
   local dateTable = (dateTableGetter or platform.getDate)()
   local dateString = string.format("%0.4d-%0.2d-%0.2d %0.2d:%0.2d:%0.2d",
                                    dateTable.year,
                                    dateTable.month,
                                    dateTable.day,
                                    dateTable.hour,
                                    dateTable.minute,
                                    dateTable.second)
   return dateString
end

--platform.display.clear()
--platform.display.printXY(1, 1, "Ру́сский язы́к from lua!")
--platform.keyboard.getKey()

--platform.display.printXY(1, 1, "clearing")
--platform.display.clear()

--local success, errorMessage = log.initialize()
--if not success then
--   displayMultilineText(string.format("log init error: %s", errorMessage))
--   platform.keyboard.getKey()
--end

--local type, moduleHw, moduleSw, board, driver = wls.GetVerInfo()
--platform.display.printXY(1, 4, string.format("type: '%s', hw: '%s', sw: '%s', board: '%s', drv: '%s'", type, moduleHw, moduleSw, board, driver))

function printGPRSStatusAt(x, y)
   local status = wls.CheckNetLink()
   platform.display.printXY(x, y, "%s %d.", status, platform.getMonotonicTime())
end

-- main thread
mainThread =
 newThread(
   function ()
      -- make sure we have a thread alive printing the GPRS status
      if not isThreadAlive(gprsStatusThread) then
         gprsStatusThread = newThread(
            function ()
               while true do
                  printGPRSStatusAt(10, 6)
                  yield()
               end
            end)
      end

      displayMenu(
         {{
             title = "Connect";
             keyCode = platform.keyboard.key1;
             trunk = function ()
                if not isThreadAlive(gprsConnectThread) then
                   gprsConnectThread = newThread(
                      function ()
                         platform.display.printXY(1, 3, "GPRS connecting...")
                         platform.gprs.initiateConnect()
                         local elapsedTime = platform.gprs.waitUntilConnected()
                         platform.display.printXY(1, 3, "GPRS up in %d.", elapsedTime)
                         gprsConnectThread = nil
                         return "done"
                      end)
                end
             end;
          },
          {
             title = "Disconnect";
             keyCode = platform.keyboard.key2;
             trunk = function ()
                platform.gprs.initiateDisconnect()
             end;
          },
          {
             title = "http get";
             keyCode = platform.keyboard.key3;
             trunk = function ()
                newThread(
                   function ()
                      wls.MTcpConnect(1, "176.9.81.202", 80)
                      wls.MTcpSend(1, "GET /status HTTP/1.1\r\nHost: dwim.hu\r\n\r\n")
                      platform.display.printXY(1, 6, "receiving...")
                      local result = wls.MTcpRecv(1, 1024, 10)
                      wls.MTcpClose(1)
                      displayMultilineText("<start>\n" .. result .. "<end>")
                   end)
             end;
          },
          {
             title = "display multiline";
             keyCode = platform.keyboard.key4;
             trunk = function ()
                platform.display.clear()
                platform.display.printXY(1, 1, string.rep("x", 128/6 * 64/8))

                displayMultilineText("a234567890b234567890c234567890d234567890e234567890f234567890g234567890h234567890i234567890j234567890\n\n\nk234567890\nl234567890\nm234567890\nn234567890\no234567890\n",
                                     nil, 2, 3, 4, 5)
             end;
          },
          {
             title = "clear test";
             keyCode = platform.keyboard.key5;
             trunk = function ()
                platform.display.clear()
                platform.display.printXY(1, 1, string.rep("x", 128/6 * 64/8))

                local fontWidth = platform.display.fontWidth
                local fontHeight = platform.display.fontHeight
                platform.display.clear((2 - 1) * fontWidth,
                                       (3 - 1) * fontHeight,
                                       4 * fontWidth,
                                       5 * fontHeight)
                platform.keyboard.getKey()
             end;
          },
          {
             title = "clear test2";
             keyCode = platform.keyboard.key6;
             trunk = function ()
                platform.display.clear()
                platform.display.printXY(1, 1, string.rep("x", 128/6 * 64/8))

                platform.display.clear(6, 16, 24, 40)
                platform.keyboard.getKey()
             end;
          }

         })
   end)

-- circle around the threads and run them one after the other
while
   # threads > 0 and
   coroutine.status(mainThread) ~= "dead"
   -- TODO be more cooperative with the other threads?
do
   for index, thread in pairs(threads) do
      local running, error = resume(thread)
      if error == "done" then
         table.remove(threads, index)
      elseif not running then
         table.remove(threads, index)
         displayMultilineText("Error in a thread:\n"..error)
      end
   end

   waitForEvents()
end


-- print(http.request("http://www.cs.princeton.edu/~diego/professional/luasocket"))
-- print(http.request("http://dwim.hu/status"))

-- ctos.LCDTPrintXY(1, 4, string.sub(ctos.GetSerialNumber(), 0, 16))
-- ctos.LCDTPrintXY(1, 5, string.sub(ctos.GetSerialNumber(), 16))
-- ctos.LCDTPrintXY(1, 5, ctos.GetFactorySN())

--ctos.LCDTPrintXY(1, 2, ctos.TickGet())
--key = ctos.KBDGet()



         --platform.display.clear()
         --platform.display.printXY(1, 1, "Tick: %d.", platform.getMonotonicTime())

         --[[
         if platform.keyboard.hasKeyInBuffer() then
            local key = platform.keyboard.getKey()
            if key == platform.keyboard.key1 then
               if not isThreadAlive(gprsConnectThread) then
                  gprsConnectThread = newThread(
                     function ()
                        platform.display.printXY(1, 3, "GPRS connecting...")
                        platform.gprs.initiateConnect()
                        local elapsedTime = platform.gprs.waitUntilConnected()
                        platform.display.printXY(1, 3, "GPRS up in %d.", elapsedTime)
                        gprsConnectThread = nil
                        return "done"
                     end)
               end
            elseif key == platform.keyboard.key2 then
               platform.gprs.initiateDisconnect()
            elseif key == platform.keyboard.key3 then
               newThread(
                  function ()
                     wls.MTcpConnect(1, "176.9.81.202", 80)
                     wls.MTcpSend(1, "GET /status HTTP/1.1\r\nHost: dwim.hu\r\n\r\n")
                     platform.display.printXY(1, 6, "receiving...")
                     local result = wls.MTcpRecv(1, 1024, 10)
                     wls.MTcpClose(1)
                     displayMultilineText("<start>\n" .. result .. "<end>")
                  end)
            elseif key == platform.keyboard.key9 then
               displayMultilineText("a234567890b234567890c234567890d234567890e234567890f234567890g234567890h234567890i234567890j234567890\n\n\nk234567890\nl234567890\nm234567890\nn234567890\no234567890\n")
            elseif key == platform.keyboard.key8 then
               wls.Reset(wls.GPRS_G610)
            elseif key == platform.keyboard.keyCancel then
               -- TODO make it more cooperative with the other threads?
               return "done"
            else
               platform.display.printXY(1, 4, "Key: %d.", key)
            end
         end
            --]]


-- warning: the last character of this file will be overwritten with a zero, so keep this comment here...



