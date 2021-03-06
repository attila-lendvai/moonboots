-- -*- mode: lua; coding: utf-8 -*-

function errorWithFormat(object, ...)
   if type(object) == "string" and select('#', ...) > 0 then
      error(string.format(object, ...), 2)
   else
      error(object, 2)
   end
end

function printIndexedTable(table)
   for index, value in ipairs(table) do
      io.write(index)
      io.write(": ")
      print(value)
   end
end

function indexedTableToString(table)
   local result = ""
   for index, value in ipairs(table) do
      result = result .. index .. ": " .. value .. "\n"
   end
   return result
end

function printTable(table)
   for key, value in pairs(table) do
      io.write(key)
      io.write(": ")
      print(value)
   end
end

function tableToString(table)
   local result = ""
   for key, value in pairs(table) do
      result = result .. key .. ": " .. value .. "\n"
   end
   return result
end

function table.find(table, valueToFind)
   for index, value in ipairs(table) do
      if value == valueToFind then
         return index, value
      end
   end

   return nil
end

function stringToLines(str, maxColumns, indent, indent1)
  local indent = indent or ""
  local indent1 = indent1 or indent
  local maxColumns = maxColumns or (platform.display.width / platform.display.fontWidth)
  local lines = {}
  local currentLine = indent1
  local firstLine = true

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
                      if not firstLine then
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
                   firstLine = false
                end)
     table.insert(lines, currentLine)
     currentLine = indent
  end

  return lines
end

function displayMultilineText(text, keyMap, atColumn, atRow, windowColumns, windowRows)
   local atRow = atRow or 1
   local atColumn = atColumn or 1
   local fontWidth = platform.display.fontWidth
   local fontHeight = platform.display.fontHeight
   local windowColumns = windowColumns or (platform.display.width / fontWidth)
   local windowRows = windowRows or (platform.display.height / fontHeight)
   local keyMap = keyMap or {}
   local needsRedraw = true

   local lines = stringToLines(text, windowColumns)

   local currentLineOffset = 0
   while true do
      if needsRedraw then
         platform.display.clear((atColumn - 1) * fontWidth,
                                (atRow - 1) * fontHeight,
                                windowColumns * fontWidth,
                                windowRows * fontHeight)
         local currentLineIndex = currentLineOffset
         local currentScreenRow = atRow
         while currentScreenRow - atRow < windowRows do
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
      end
      needsRedraw = true
      if platform.keyboard.hasKeyInBuffer() then
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
         elseif keyMap[key] then
            keyMap[key]()
         else
            needsRedraw = false
         end
      else
         needsRedraw = false
      end

      platform.waitForEvents()
   end
end

function displayMessage(message)
   -- TODO this is somewhat kludgey
   newFullscreenThread(function ()
                          platform.display.clear()
                          displayMultilineText(message)
                       end)
end

function displayMenu(entries)
   local keyMap = {}
   local text = nil
   for index, entry in ipairs(entries) do
      local keyCode = entry.keyCode
      local trunk = entry.trunk
      local title = entry.title
      if not keyCode or not trunk or not title then
         error(string.format("displayMenu called with broken entry: %s, %s, %s", title, keyCode, trunk))
      end
      keyMap[keyCode] = trunk
      if text then
         text = text.."\n"..title
      else
         text = title
      end
   end

   displayMultilineText(text, keyMap)
end

-- condition handling

function throw(type, message, ...)
   local condition = {}
   condition.type = type
   condition.message = message
   condition.extraArguments = {...}
   error(condition)
end

function catch(body, handlers)
   local condition
   local run, result = xpcall(body,
                              function (object)
                                 -- xpcall calls the handler body in the scope of the handler. so we simply store the condition and quit
                                 -- to avoid re-entering the handler in case of an error, and/or to allow re-signalling from the handlers...
                                 condition = object
                              end)
   if condition then
      local handler = handlers["default"]
      if type(condition) == "table" and
         type(condition.type) == "string" and
         handlers[condition.type]
      then
         handler = handlers[condition.type]
      end

      if handler then
         local handlerResult = handler(condition)
         if handlerResult then
            -- return whatever the handler returned (usually true)
            return false, handlerResult
         end
      end

      -- wasn't handled by any of the handlers, so let's resignal it
      error(condition)
   else
      return true, result
   end
end

function throwUserMessage(message, ...)
   throw("userMessage", string.format(message, ...))
end
