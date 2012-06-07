-- -*- mode: lua; coding: utf-8 -*-

-- aliases
yield = coroutine.yield
resume = coroutine.resume

-- global variables
threads = {}
gprsConnectThread = nil
gprsStatusThread = nil
mainThread = nil

function newThread(fn)
   local co = coroutine.create(fn)
   table.insert(threads, co)
   return co
end

function isThreadAlive(thread)
   return thread and coroutine.status(thread) ~= "dead"
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

-- displaying long text

function printTable(table)
   for index, value in ipairs(table) do
      io.write(index)
      io.write(": ")
      print(value)
   end
end

function stringToLines(str, maxColumns, indent, indent1)
  local indent = indent or ""
  local indent1 = indent1 or indent
  local maxColumns = maxColumns or (platform.display.width / platform.display.fontWidth)
  local lines = {}
  local currentLine = {}

  local newlineChunks = {}

  str:gsub("([^\n]+)(\n*)",
           function (chunk, newlines)
              table.insert(newlineChunks, chunk)
              for i = 1, #newlines - 1, 1 do
                 table.insert(newlineChunks, "")
              end
           end)

  for index, chunk in pairs(newlineChunks) do
     chunk:gsub("(%s*)()(%S+)()",
                function (space, start, word, endd)
                   if #currentLine + #space + #word > maxColumns then
                      if #currentLine > 0 then
                         table.insert(lines, currentLine)
                      end
                      currentLine = indent..word
                      while #currentLine > maxColumns do
                         table.insert(lines, currentLine:sub(0, maxColumns))
                         currentLine = currentLine:sub(maxColumns + 1)
                      end
                    else
                       currentLine = currentLine..space..word
                   end
                end)
     table.insert(lines, currentLine)
     currentLine = indent
  end

  return lines
end

function displayMultilineText(text, atRow, atColumn, windowRows, windowColumns)
   local atRow = atRow or 1
   local atColumn = atColumn or 1
   local windowColumns = windowColumns or (platform.display.width / platform.display.fontWidth)
   local windowRows = windowRows or (platform.display.height / platform.display.fontHeight)

   local lines = stringToLines(text, windowColumns)

   local currentLineOffset = 0
   while true do
      platform.display.clear()
      local currentLineIndex = currentLineOffset
      local currentScreenRow = atRow
      while currentScreenRow <= windowRows do
         if currentLineIndex < #lines then
            local line = lines[currentLineIndex + 1]
            platform.display.printXY(atColumn, currentScreenRow, line)
            --platform.display.print(string.rep(' ', windowColumns - #line))
         else
            --platform.display.printXY(atColumn, currentScreenRow, string.rep(' ', windowColumns))
         end
         currentScreenRow = currentScreenRow + 1
         currentLineIndex = currentLineIndex + 1
      end

      local key = platform.keyboard.getKey()
      if key == platform.keyboard.keyDown then
         if currentLineOffset < #lines - windowRows then
            currentLineOffset = currentLineOffset + 1
         end
      elseif key == platform.keyboard.keyUp then
         if currentLineOffset > 0 then
            currentLineOffset = currentLineOffset - 1
         end
      elseif key == platform.keyboard.keyCancel then
         break
      end
   end
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
   local status = wls.CheckTcpLink()
   platform.display.printXY(x, y, "Status: %d.", status)
end

-- main thread
mainThread =
 newThread(
   function ()
      while true do

         -- make sure we have a thread alive printing the GPRS status
         if not isThreadAlive(gprsStatusThread) then
            gprsStatusThread = newThread(
               function ()
                  while true do
                     printGPRSStatusAt(1, 2)
                     yield()
                  end
               end)
         end

         platform.display.clear()
         platform.display.printXY(1, 1, "Tick: %d.", platform.getMonotonicTime())

         if platform.keyboard.hasKeyInBuffer() then
            local key = platform.keyboard.getKey()
            if key == platform.keyboard.key1 then
               if not isThreadAlive(gprsConnectThread) then
                  gprsConnectThread = newThread(
                     function ()
                        platform.display.printXY(1, 3, "GPRS connecting...")
                        platform.gprs.initiateConnection()
                        local status, elapsedTime = platform.gprs.waitUntilConnected()
                        platform.display.printXY(1, 3, "GPRS up in %d.", elapsedTime)
                        gprsConnectThread = nil
                        return "done"
                     end)
               end
            elseif key == platform.keyboard.key2 then
               platform.gprs.disconnect()
            --elseif key == platform.keyboard.key3 then
            --   local body, code, headers, status = http.request("http://dwim.hu/status?from-vega5000")
            --   ctos.LCDTPrintXY(1, 6, status)
            --   ctos.LCDTPrint(body)
            elseif key == platform.keyboard.key9 then
               displayMultilineText("a234567890b234567890c234567890d234567890e234567890f234567890g234567890h234567890i234567890j234567890\nk234567890\nl234567890\nm234567890\nn234567890\no234567890\n")
            elseif key == platform.keyboard.key8 then
               platform.display.clear()
               platform.display.printXY(1, 1, "a234567890b234567890c2")
               platform.display.printXY(1, 4, "a234567890b234567890c")
               platform.keyboard.getKey()
            elseif key == platform.keyboard.keyCancel then
               -- TODO make it more cooperative with the other threads?
               return "done"
            else
               platform.display.printXY(1, 4, "Key: %d.", key)
            end
         end

         yield()
      end
   end)

-- circle around the threads and run them one after the other
while
   # threads > 0 and
   coroutine.status(mainThread) ~= "dead"
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

   platform.sleep(0.1)
end


-- print(http.request("http://www.cs.princeton.edu/~diego/professional/luasocket"))
-- print(http.request("http://dwim.hu/status"))

-- ctos.LCDTPrintXY(1, 4, string.sub(ctos.GetSerialNumber(), 0, 16))
-- ctos.LCDTPrintXY(1, 5, string.sub(ctos.GetSerialNumber(), 16))
-- ctos.LCDTPrintXY(1, 5, ctos.GetFactorySN())

--ctos.LCDTPrintXY(1, 2, ctos.TickGet())
--key = ctos.KBDGet()

-- warning: the last character of this file will be overwritten with a zero, so keep this comment here...
