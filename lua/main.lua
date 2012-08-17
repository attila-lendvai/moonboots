-- -*- mode: lua; coding: cp866 -*-

-- global variables
threads = {}
fullscreenThreads = {}
gprsConnectThread = nil
gprsStatusThread = nil
mainThread = nil

function newThread(trunk)
   local co = coroutine.create(function ()
                                  catch(trunk,
                                        {
                                           userMessage = function (condition)
                                              -- TODO FIXME localize, etc...
                                              displayMultilineText("Error:\n"..condition.message)
                                              return true
                                           end;

                                           default = function (condition)
                                              local prefix = tostring(condition)
                                              if type(condition) == "table" then
                                                 prefix = tableToString(condition)
                                              end
                                              error(debug.traceback(prefix))
                                           end
                                        })
                                  return "done"
                               end)
   table.insert(threads, co)
   return co
end

function newFullscreenThread(trunk)
   local co = newThread(trunk)
   table.insert(fullscreenThreads, 1, co)
   return co
end

function isFullscreenThread(thread)
   return table.find(fullscreenThreads, thread)
end

function isActiveFullscreenThread(thread)
   local index = isFullscreenThread(thread)
   return index and index == 1
end

function isThreadAlive(thread)
   return thread and coroutine.status(thread) ~= "dead"
end

function isToplevelThread()
   return select(2, coroutine.running())
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

--local type, moduleHw, moduleSw, board, driver = wls.GetVerInfo()
--platform.display.printXY(1, 4, string.format("type: '%s', hw: '%s', sw: '%s', board: '%s', drv: '%s'", type, moduleHw, moduleSw, board, driver))

function printGPRSStatusAt(x, y)
   --local status = wls.CheckNetLink()
   --platform.display.printXY(x, y, "%s %d.", status, platform.getMonotonicTime())
   local status = ctos.TCP_GPRSStatus()
   local bstStatus
   if bst.isConnected() then
      bstStatus = "C"
   else
      bstStatus = "D"
   end
   platform.display.printXY(x, y, "%s %d %s.", status, platform.getMonotonicTime(), bstStatus)
end

--[[
newThread(function ()
             local keyDownAt = nil
             while true do
                local key = ctos.KBDHit()
                if key == platform.keyboard.keyCancel then
                   if keyDownAt == nil then
                      keyDownAt = platform.getMonotonicTime()
                   elseif platform.getMonotonicTime() - keyDownAt > 5 then
                      os.exit()
                   end
                else
                   keyDownAt = nil
                end
                platform.waitForEvents()
             end
          end)
--]]

-- main thread
mainThread =
 newFullscreenThread(
   function ()
      -- make sure we have a thread alive printing the GPRS status
      if not isThreadAlive(gprsStatusThread) then
         gprsStatusThread = newThread(
            function ()
               while true do
                  printGPRSStatusAt(7, 7)
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
                         platform.display.printXY(1, 8, "GPRS connecting...")
                         platform.gprs.initiateGPRSConnection()
                         local elapsedTime = platform.gprs.waitUntilConnected()
                         platform.display.printXY(1, 8, "GPRS up in %d.", elapsedTime)
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
                platform.gprs.initiateGPRSDisconnection()
             end;
          },
          {
             title = "bst connect";
             keyCode = platform.keyboard.key3;
             trunk = function ()
                newThread(
                   function ()
                      bst.ensureConnected()
                   end)
             end;
          },
          {
             title = "bst disconnect";
             keyCode = platform.keyboard.key4;
             trunk = function ()
                newThread(
                   function ()
                      bst.disconnect()
                   end)
             end;
          },
          {
             title = "GSM reset";
             keyCode = platform.keyboard.key5;
             trunk = function ()
                ctos.GSMReset()
             end;
          }
          --[[
          {
             title = "http get";
             keyCode = platform.keyboard.key3;
             trunk = function ()
                newThread(
                   function ()
                      --wls.MTcpConnect(1, "176.9.81.202", 80)
                      --wls.MTcpSend(1, "GET /status HTTP/1.1\r\nHost: dwim.hu\r\n\r\n")
                      --platform.display.printXY(1, 6, "receiving...")
                      --local result = wls.MTcpRecv(1, 1024, 10)
                      --wls.MTcpClose(1)
                      --displayMultilineText("<start>\n" .. result .. "<end>")

                      local socket = ctos.TCP_GPRSConnectEx("176.9.81.202", 80, 5)
                      ctos.TCP_GPRSTx(socket, "GET /status HTTP/1.1\r\nHost: dwim.hu\r\n\r\n", 5)
                      platform.display.printXY(1, 6, "receiving...")
                      local result = ctos.TCP_GPRSRx(socket, 1024, 5)
                      ctos.TCP_GPRSDisconnect(socket, 5)
                      newFullscreenThread(function ()
                                             platform.display.clear()
                                             displayMultilineText("<start>\n" .. result .. "<end>")
                                          end)
                   end)
             end;
          },
          {
             title = "display multiline";
             keyCode = platform.keyboard.key4;
             trunk = function ()
                newFullscreenThread(function ()
                                       platform.display.clear()
                                       platform.display.printXY(1, 1, string.rep("x", 128/6 * 64/8))
                                       displayMultilineText("Русский язык! a234567890b234567890c234567890d234567890e234567890f234567890g234567890h234567890i234567890j234567890\n\n\nk234567890\nl234567890\nm234567890\nn234567890\no234567890\n",
                                                            nil, 2, 3, 4, 5)
                                    end)
             end;
          },
          {
             title = "error from here";
             keyCode = platform.keyboard.key5;
             trunk = function ()
                error("demo error")
             end;
          },
          {
             title = "error from thread";
             keyCode = platform.keyboard.key6;
             trunk = function ()
                newThread(
                   function ()
                      error("demo error from thread")
                   end)
             end;
          }
          --]]
         })
   end)

-- circle around the threads and run them one after the other
while
   # threads > 0 and
   coroutine.status(mainThread) ~= "dead"
   -- TODO be more cooperative with the other threads?
do
   -- get rid of dead fullscreen threads
   for index, thread in pairs(fullscreenThreads) do
      if not isThreadAlive(thread) then
         table.remove(fullscreenThreads, index)
      end
   end

   -- see if we have anyone to rune
   for index, thread in pairs(threads) do
      local shouldRun = not isFullscreenThread(thread) or isActiveFullscreenThread(thread)

      if shouldRun then
         local running, resultOrError = resume(thread)
         if resultOrError == "done" then
            table.remove(threads, index)
         elseif not running then
            table.remove(threads, index)
            displayMultilineText("Error reached toplevel:\n"..resultOrError)
         end
      end
   end

   platform.waitForEvents()
end


-- print(http.request("http://www.cs.princeton.edu/~diego/professional/luasocket"))
-- print(http.request("http://dwim.hu/status"))

-- ctos.LCDTPrintXY(1, 4, string.sub(ctos.GetSerialNumber(), 0, 16))
-- ctos.LCDTPrintXY(1, 5, string.sub(ctos.GetSerialNumber(), 16))
-- ctos.LCDTPrintXY(1, 5, ctos.GetFactorySN())

--ctos.LCDTPrintXY(1, 2, ctos.TickGet())
--key = ctos.KBDGet()


-- warning: the last character of this file will be overwritten with a zero, so keep this comment here...
