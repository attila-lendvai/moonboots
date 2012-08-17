-- -*- mode: lua; coding: utf-8 -*-

ctos = require("castles.ctosapi")

ctos.KBDSetSound(false)

platform.getDate = os.date
platform.getMonotonicTime = ctos.TickGet
platform.sleep = ctos.Delay

platform.waitForEvents =
   function()
     if isToplevelThread() then
        ctos.SystemWait(0.3, ctos.EVENT_KBD + ctos.EVENT_SC + ctos.EVENT_MSR + ctos.EVENT_MODEM + ctos.EVENT_COM1 + ctos.EVENT_COM2)
     else
        yield()
     end
   end

--
-- display
--
platform.display = {}
platform.display.fontWidth  = 8
platform.display.fontHeight = 8
platform.display.width   = ctos.LCD_WINDOW_X_SIZE
platform.display.height  = ctos.LCD_WINDOW_Y_SIZE

platform.display.clear =
   function (x, y, width, height)
     local allParametersProvided = x and y and width and height
     if x and y and width and height then
        ctos.LCDGSetBox(x, y, width, height, 0)
     elseif x or y or width or height then
        error(string.format("platform.display.clear called with %s, %s, %s, %s.", x, y, width, height))
     else
        ctos.LCDGClearWindow()
     end
   end

platform.display.print =
   function (message, ...)
     if select('#', ...) > 0 then
        message = string.format(message, ...)
     elseif type(message) ~= "string" then
        message = tostring(message)
     end

     ctos.LCDTPrint(message)
   end

platform.display.printXY =
   function (x, y, message, ...)
     --if type(message) ~= "string" or type(x) ~= "number" or type(y) ~= "number" then
     --   error(string.format("printXY called with %s, %s, %s, extra args: %s", x, y, message, select('#', ...)))
     --end
     if select('#', ...) > 0 then
        message = string.format(message, ...)
     elseif type(message) ~= "string" then
        message = tostring(message)
     end

     ctos.LCDTPrintXY(x, y, message)
   end

platform.display.gotoXY = ctos.LCDTGotoXY

--
-- keyboard
--
platform.keyboard = {}
platform.keyboard.getKey         = ctos.KBDGet
platform.keyboard.hasKeyInBuffer = ctos.KBDInKey

platform.keyboard.keyCancel    = ctos.KBD_CANCEL
platform.keyboard.keyEnter     = ctos.KBD_ENTER
platform.keyboard.keyBackspace = ctos.KBD_CLEAR

platform.keyboard.key0 = ctos.KBD_0
platform.keyboard.key1 = ctos.KBD_1
platform.keyboard.key2 = ctos.KBD_2
platform.keyboard.key3 = ctos.KBD_3
platform.keyboard.key4 = ctos.KBD_4
platform.keyboard.key5 = ctos.KBD_5
platform.keyboard.key6 = ctos.KBD_6
platform.keyboard.key7 = ctos.KBD_7
platform.keyboard.key8 = ctos.KBD_8
platform.keyboard.key9 = ctos.KBD_9

platform.keyboard.keyDot       = ctos.KBD_DOT

platform.keyboard.keyUp        = ctos.KBD_UP
platform.keyboard.keyDown      = ctos.KBD_DOWN

-- less general
platform.keyboard.function1    = ctos.KBD_F1
platform.keyboard.function2    = ctos.KBD_F2
platform.keyboard.function3    = ctos.KBD_F3
platform.keyboard.function4    = ctos.KBD_F4

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
-- gprs
--
platform.gprs = {}
platform.gprs.gprsConnectionTimeout = 80
platform.gprs.connectTimeout = 10
platform.gprs.transferTimeout = 10
platform.gprs.currentSimSlot = nil
platform.gprs.selectSimTimeout = 3

-- GPRS, not just a socket...
platform.gprs.isConnected =
   function ()
      local state = ctos.TCP_GPRSStatus()
      if state == ctos.TCP_GPRS_STATE_ESTABLISHED or
         state == ctos.TCP_GPRS_STATE_ESTABLISHING
      then
         return state
      else
         return false
      end
   end

platform.gprs.selectSimSlot =
   function (simSlot)
     if simSlot < 1 or simSlot > 2 then
        error("Sim slot number must be 1 <= sim-slot <= 2")
     end
     ctos.GSMSelectSIM(simSlot - 1)
     platform.gprs.currentSimSlot = simSlot
     -- wait for ctos.SIMCheckReady returning OK
     local startTime = platform.getMonotonicTime()
     while true do
        local elapsedTime = platform.getMonotonicTime() - startTime
        if elapsedTime > platform.gprs.selectSimTimeout then
           localizedError("Problem with sim card in slot: %s", platform.gprs.currentSimSlot)
        end
        local simStatus = ctos.SIMCheckReady()
        if simStatus then
           return true
        end
        yield()
     end
   end

platform.gprs.initiateGPRSConnection =
   function ()
     if not platform.gprs.currentSimSlot then
        platform.gprs.selectSimSlot(1)
     end
     yield()
     ctos.TCP_GPRSOpen(nil, "internet", "", "")
     yield()
   end

platform.gprs.initiateGPRSDisconnection =
   function ()
     ctos.TCP_GPRSClose_A()
   end

platform.gprs.waitUntilConnected =
   function (timeout)
     local timeout = timeout or platform.gprs.gprsConnectionTimeout
     local startTime = platform.getMonotonicTime()
     while true do
        local status, returnValue = ctos.TCP_GPRSStatus()
        local elapsedTime = platform.getMonotonicTime() - startTime
        platform.display.printXY(1, 8, "GPRS elapsed: %s", elapsedTime) -- TODO delme
        if bit32.band(status, ctos.TCP_GPRS_STATE_ESTABLISHED) ~= 0 then
           return elapsedTime
        elseif elapsedTime > timeout then
           localizedError("GPRS connection timed out after %s seconds.", timeout)
        elseif returnValue == ctos.TCP_IO_PROCESSING or bit32.band(status, ctos.TCP_GPRS_STATE_ESTABLISHING) ~= 0 then
           yield(status, elapsedTime)
        else
           error(string.format("platform.gprs.waitUntilConnected: unknown error: %d, status: %d", returnValue, status), 0)
        end
     end
   end

platform.gprs.ensureGPRSConnected =
   function (timeout)
     local state = platform.gprs.isConnected()
     local timeout = timeout or platform.gprs.gprsConnectionTimeout
     if not state then
        platform.gprs.initiateGPRSConnection()
        platform.gprs.waitUntilConnected(timeout)
     elseif bit32.band(state, ctos.TCP_GPRS_STATE_ESTABLISHING) ~= 0 then
        platform.gprs.waitUntilConnected(timeout)
     end
   end

platform.gprs.connect =
   function (ipAddress, port, timeout)
     local socket = ctos.TCP_GPRSConnectEx(ipAddress, port, timeout or platform.gprs.connectTimeout)
     return socket
   end

platform.gprs.disconnect =
   function (socket)
      ctos.TCP_GPRSDisconnect(socket)
   end

platform.gprs.isSocketConnected =
   function (socket)
      -- TODO FIXME but how?
      return true
   end

platform.gprs.send =
   function (socket, data, timeout)
      ctos.TCP_GPRSTx(socket, data, timeout or platform.gprs.transferTimeout)
   end

platform.gprs.receive =
   function (socket, length, timeout)
      local result, didTimedOut = ctos.TCP_GPRSRx(socket, length, timeout or platform.gprs.transferTimeout)
      return result, didTimedOut
   end



--
-- initialize the libs as the API mandates
--
ctos.TCP_GPRSInit()
ctos.LanguageConfig(ctos.FONT_RUSSIAN, ctos.FONT_8x8, 0, false)
