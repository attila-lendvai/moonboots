-- -*- mode: lua; coding: utf-8 -*-

jos = require("justtide.josapi")
wls = require("justtide.wlsapi")

jos.KbMute(true)

platform.getDate = jos.GetDateTime -- always returns a table

platform._monotonicTimerOffset = 0
platform._monotonicTimerId = 0
platform.getMonotonicTime =
   -- the time between two calls to this function must be less than (65536 / 10) secs, otherwise the counter overflows
   function ()
     platform._monotonicTimerOffset = 6553 - jos.CheckTimer(platform._monotonicTimerId) + platform._monotonicTimerOffset
     jos.SetTimer(platform._monotonicTimerId, 6553)
     return platform._monotonicTimerOffset
   end

jos.SetTimer(platform._monotonicTimerId, 6553)

platform.sleep = jos.DelayMs

--
-- display
--
platform.display = {}
-- Small font is 6x8, Middle font is 6x12, Big font is 8x16
platform.display.fontWidth  = 6
platform.display.fontHeight = 8
platform.display.width      = 128
platform.display.height     = 64
platform.display.clear =
   function (x, y, width, height)
     local allParametersProvided = x and y and width and height
     if x and y and width and height then
        for line = y, y + height, 1 do
           jos.LcdDrawLine(x, line, x + width - 1, line, 0)
        end
     elseif x or y or width or height then
        error(string.format("platform.display.clear called with %s, %s, %s, %s.", x, y, width, height))
     else
        jos.LcdCls()
     end
   end

platform.display.print =
   function (message, ...)
     if select('#', ...) > 0 then
        message = string.format(message, ...)
     elseif type(message) ~= "string" then
        message = tostring(message)
     end

     jos.Lcdprintf(message)
   end

platform.display.printXY =
   function (x, y, message, ...)
     if select('#', ...) > 0 then
        message = string.format(message, ...)
     elseif type(message) ~= "string" then
        message = tostring(message)
     end

     jos.LcdPrintxy((x - 1) * platform.display.fontWidth,
                    (y - 1) * platform.display.fontHeight,
                    0, message)
   end

platform.display.gotoXY =
   function (x, y)
     jos.LcdGotoxy((x - 1) * platform.display.fontWidth,
                   (y - 1) * platform.display.fontHeight)
   end

--
-- keyboard
--
platform.keyboard = {}
platform.keyboard.getKey         = jos.KbGetCh
platform.keyboard.hasKeyInBuffer = jos.KbCheck

platform.keyboard.keyCancel    = jos.KEYCANCEL
platform.keyboard.keyEnter     = jos.KEYENTER
platform.keyboard.keyBackspace = jos.KEYBACKSPACE

platform.keyboard.key0 = jos.KEY0
platform.keyboard.key1 = jos.KEY1
platform.keyboard.key2 = jos.KEY2
platform.keyboard.key3 = jos.KEY3
platform.keyboard.key4 = jos.KEY4
platform.keyboard.key5 = jos.KEY5
platform.keyboard.key6 = jos.KEY6
platform.keyboard.key7 = jos.KEY7
platform.keyboard.key8 = jos.KEY8
platform.keyboard.key9 = jos.KEY9

platform.keyboard.keyDot       = jos.KEYDOT

platform.keyboard.keyUp        = jos.KEYUP
platform.keyboard.keyDown      = jos.KEYDOWN

-- less general
platform.keyboard.keyMenu      = jos.KEYMENU
platform.keyboard.keyOk        = jos.KEYOK

--
-- serial
--
platform.serial = {}
platform.serial.defaultPort = 1
platform.serial.connect =
   function (port)
     local success, errorMessage = pcall(function ()
                                            jos.ComClose(port)
                                            jos.ComOpen(port, "115200,8,n,1")
                                            jos.DelayMs(0.020)
                                            -- jos.ComReset(port)
                                         end)
     return success, errorMessage
   end

platform.serial.send =
   function (port, data)
     jos.ComSend(port, tostring(data))
   end

--
-- gprs
--
platform.gprs = {}
platform.gprs.connectionTimeout = 60
platform.gprs.currentSimSlot = nil

platform.gprs.isConnected =
   function ()
      local state = wls.CheckNetLink()
      if state == wls.OK or state == wls.LINKOPENING then
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
     wls.SelectSim(simSlot - 1)
     wls.Init(wls.GPRS_G610)
     -- the reset call takes several seconds, but we don't yield before it to keep it all as an atomic operation.
     wls.Reset(wls.GPRS_G610)
     platform.gprs.currentSimSlot = simSlot
   end

platform.gprs.initiateConnect =
   function ()
     if not platform.gprs.currentSimSlot then
        platform.gprs.selectSimSlot(1)
     end
     yield()
     local simStatus = wls.CheckSim()
     if simStatus == wls.NOSIM then
        localizedError("No sim card in slot: %s", platform.gprs.currentSimSlot)
     elseif simStatus ~= wls.OK then
        localizedError("Unexpected error with sim card: %s, in card slot: %s", simStatus, platform.gprs.currentSimSlot)
     end
     yield()
     --wls.InputUidPwd()
     wls.Dial("internet")
     yield()
   end

-- TODO "After Wls_NetClose() returns WLS_OK, it is possible the netlink does not close really, you should invoke Wls_CheckNetLink to check the netlink."
platform.gprs.initiateDisconnect =
   function ()
     wls.NetClose()
     wls.Reset(wls.GPRS_G610)
   end

platform.gprs.waitUntilConnected =
   function (timeout)
     local timeout = timeout or platform.gprs.connectionTimeout
     local startTime = platform.getMonotonicTime()
     while true do
        local status = wls.CheckNetLink()
        local elapsedTime = platform.getMonotonicTime() - startTime
        platform.display.printXY(1, 4, "GPRS elapsed: %s", elapsedTime)
        if status == wls.OK then
           return elapsedTime
        elseif elapsedTime > timeout then
           localizedError("GPRS connection timed out after %s seconds.", timeout)
        elseif status == wls.LINKOPENING then
           yield(status, elapsedTime)
        else
           error(string.format("platform.gprs.waitUntilConnected: unknown error: %d", status), 0)
        end
     end
   end

platform.gprs.ensureConnected =
   function (timeout)
     local state = platform.gprs.isConnected()
     local timeout = timeout or platform.gprs.connectionTimeout
     if not state then
        platform.gprs.initiateConnect()
        platform.gprs.waitUntilConnected(timeout)
     elseif state == wls.LINKOPENING then
        platform.gprs.waitUntilConnected(timeout)
     end
   end

--
-- initialize the libs as the API mandates
--
