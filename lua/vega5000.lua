-- -*- mode: lua; coding: utf-8 -*-

ctos = require("castles.ctosapi")

platform.getDate = os.date
platform.getMonotonicTime = ctos.TickGet

--
-- display
--
platform.display = {}
platform.display.fontWidth  = 8
platform.display.fontHeight = 8
platform.display.width   = ctos.LCD_WINDOW_X_SIZE
platform.display.height  = ctos.LCD_WINDOW_Y_SIZE
platform.display.clear   = ctos.LCDTClearDisplay -- or LCDGClearCanvas?
platform.display.printXY =
   function (x, y, message, ...)
     if #{...} > 3 then
        message = string.format(message, ...)
     elseif type(message) ~= "string" then
        message = tostring(message)
     end

     -- TODO track font size, divide accordingly? or gfx mode?
     ctos.LCDTPrintXY(x, y, message)
   end

   -- TODO follow jos
platform.getKey       = ctos.KBDGet
platform.cancelKey    = ctos.KBD_CANCEL
platform.enterKey     = ctos.KBD_ENTER
platform.backspaceKey = ctos.KBD_CLEAR

--
-- serial
--
platform.serial = {}
platform.serial.defaultPort = 0
platform.serial.connect =
   function (port)
     local success, errorMessage = pcall(function ()
                                            ctos.RS232FlushRxBuffer(port)
                                            ctos.RS232FlushTxBuffer(port)
                                            ctos.RS232Open(port, 115200, "E", 8, 1)
                                         end)
     return success, errorMessage
   end

platform.serial.send =
   function (port, data)
       ctos.RS232TxData(port, tostring(data))
   end

--
-- initialize the libs as the API mandates
--
ctos.TCP_GPRSInit()

