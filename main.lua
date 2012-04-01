local ctos -- the API for the Castles Vega5000 device
local jos  -- the API for the Justtide PC1000 device

if not platform then
   error("running on unsupported platform", 0)
elseif platform.model == "pc1000" then
   jos = require("josapi")

   platform.getKey      = jos.KbGetCh
   platform.clearScreen = jos.LcdCls
   platform.printXY =
      function (x, y, message, ...)
        jos.LcdPrintxy(x, y, 0, message:format(...))
      end
elseif platform.model == "vega5000" then
   ctos = require("ctosapi")

   platform.screenWidth  = ctos.LCD_WINDOW_X_SIZE
   platform.screenHeight = ctos.LCD_WINDOW_Y_SIZE
   platform.clearScreen  = ctos.LCDTClearDisplay -- or LCDGClearCanvas?

   platform.getKey       = ctos.KBDGet
   platform.printXY =
      function (x, y, message, ...)
        -- TODO track font size, divide accordingly
        ctos.LCDTPrintXY(x / 8, y / 8, message:format(...))
      end
else
   error(string.format("running on unsupported model: '%s'", platform.model), 0)
end

platform.clearScreen()
platform.printXY(0, 0, "hello from lua!")
platform.getKey()


local socket = require("socket")
local http   = require("socket.http")

-- first some feedback that we are alive, then some initialization
ctos.LCDSelectMode(ctos.LCD_GRAPHIC_MODE)
ctos.LCDGClearCanvas()
ctos.LCDGTextOut(0, 0, "Lua running...")
-- ctos.LCDGTextOut(0, 0, "Lua running...", ctos.FONT_8x8, true)

ctos.TCP_GPRSInit()

-- aliases
local yield = coroutine.yield
local resume = coroutine.resume

-- global variables
local threads = {}
local gprsConnectThread = nil
local gprsStatusThread = nil
local mainThread = nil

function newThread(fn)
   local co = coroutine.create(fn)
   table.insert(threads, co)
   return co
end

function isThreadAlive(thread)
   return thread and coroutine.status(thread) ~= "dead"
end

-- paging

function stringToLines(str, maxColumns, indent, indent1)
  local indent = indent or ""
  local indent1 = indent1 or indent
  local maxColumns = maxColumns or 16
  local lines = {}
  local currentLine = indent1

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
                      table.insert(lines, currentLine)
                      currentLine = indent..word
                      while #currentLine > maxColumns do
                         table.insert(lines, currentLine:sub(0, maxColumns))
                         currentLine = currentLine:sub(maxColumns + 1)
                      end
                      -- TODO deal with words longer than maxColumns
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
   local atRow = atRow or 0
   local atColumn = atColumn or 0
   local windowColumns = windowColumns or 16
   local windowRows = windowRows or 8

   local lines = stringToLines(text, windowColumns)

   local currentLineOffset = 0
   while true do
      local currentLineIndex = currentLineOffset
      local currentScreenRow = atRow
      while currentScreenRow < windowRows do
         ctos.LCDTGotoXY(atColumn + 1, currentScreenRow + 1)
         if currentLineIndex < #lines then
            local line = lines[currentLineIndex + 1]
            ctos.LCDTPrint(line)
            ctos.LCDTClear2EOL()
         else
            ctos.LCDTClear2EOL()
         end
         currentScreenRow = currentScreenRow + 1
         currentLineIndex = currentLineIndex + 1
      end

      local key = ctos.KBDGet()
      if key == ctos.KBD_DOWN then
         if currentLineOffset < #lines - windowRows then
            currentLineOffset = currentLineOffset + 1
         end
      elseif key == ctos.KBD_UP then
         if currentLineOffset > 0 then
            currentLineOffset = currentLineOffset - 1
         end
      elseif key == ctos.KBD_CANCEL then
         break
      end
   end
end

function printGPRSStatusAt(x, y)
   local status, returnValue = ctos.TCP_GPRSStatus()

   ctos.LCDTGotoXY(x, y)
   ctos.LCDTPrint("S: ")
   ctos.LCDTPrint(status)
   ctos.LCDTPrint(", R: ")
   ctos.LCDTPrint(returnValue)
   ctos.LCDTClear2EOL()
end

function waitUntilGPRSIsConnected(timeout)
   local startTime = ctos.TickGet()
   while true do
      local status, returnValue = ctos.TCP_GPRSStatus()
      local elapsedTime = ctos.TickGet() - startTime
      if elapsedTime > timeout then
         error("waitUntilGPRSIsConnected: timed out", 0)
      elseif status == 0 then
         error("waitUntilGPRSIsConnected: not connecting", 0)
      elseif returnValue == ctos.TCP_IO_PROCESSING or
             status ~= ctos.TCP_GPRS_STATE_ESTABLISHED
      then
         yield(status, elapsedTime)
      else
         return status, elapsedTime
      end
   end
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

         ctos.LCDTPrintXY(1, 4, ctos.TickGet())
         ctos.LCDTClear2EOL()

         if ctos.KBDInKey() then
            local key = ctos.KBDGet()
            if key == ctos.KBD_1 then
               if not isThreadAlive(gprsConnectThread) then
                  gprsConnectThread = newThread(
                     function ()
                        ctos.LCDTPrintXY(1, 3, "GPRS connecting...")
                        ctos.TCP_GPRSOpen("", "internet", "", "")
                        local status, elapsedTime = waitUntilGPRSIsConnected(60)
                        ctos.LCDTPrintXY(1, 3, "GPRS: ")
                        ctos.LCDTPrint(ctos.TCP_GPRSGetIP())
                        ctos.LCDTPrint(", ")
                        ctos.LCDTPrint(elapsedTime)
                        ctos.LCDTClear2EOL()
                        gprsConnectThread = nil
                        return "done"
                     end)
               end
            elseif key == ctos.KBD_2 then
               ctos.TCP_GPRSClose_A()
            elseif key == ctos.KBD_3 then
               local body, code, headers, status = http.request("http://dwim.hu/status?from-vega5000")
               ctos.LCDTPrintXY(1, 6, status)
               ctos.LCDTPrint(body)
            elseif key == ctos.KBD_CANCEL then
               -- TODO make it more cooperative with the other threads
               return "done"
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

   ctos.SystemWait(0.3, ctos.EVENT_KBD + ctos.EVENT_SC + ctos.EVENT_MSR + ctos.EVENT_MODEM + ctos.EVENT_COM1 + ctos.EVENT_COM2)
end


-- print(http.request("http://www.cs.princeton.edu/~diego/professional/luasocket"))
-- print(http.request("http://dwim.hu/status"))

-- ctos.LCDTPrintXY(1, 4, string.sub(ctos.GetSerialNumber(), 0, 16))
-- ctos.LCDTPrintXY(1, 5, string.sub(ctos.GetSerialNumber(), 16))
-- ctos.LCDTPrintXY(1, 5, ctos.GetFactorySN())

--ctos.LCDTPrintXY(1, 2, ctos.TickGet())
--key = ctos.KBDGet()

-- warning: the last character of this file will be overwritten with a zero, so keep this comment here...
