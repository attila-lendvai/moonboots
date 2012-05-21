-- -*- mode: lua; coding: utf-8 -*-

jos = require("justtide.josapi")
wls = require("justtide.wlsapi")

platform.getDate = jos.GetDateTime -- always returns a table

platform._timerOffset = 0
platform.getMonotonicTime =
   -- the time between two calls to this function must be less than (65536 / 10) secs, otherwise the counter overflows
   function ()
     platform._timerOffset = 6553 - jos.CheckTimer(4) + platform._timerOffset
     jos.SetTimer(4, 6553)
     return platform._timerOffset
   end

jos.SetTimer(4, 6553)

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
platform.display.clear      = jos.LcdCls
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

-- less general
platform.keyboard.keyUp        = jos.KEYUP
platform.keyboard.keyDown      = jos.KEYDOWN
platform.keyboard.keyMenu      = jos.KEYMENU
platform.keyboard.keyDot       = jos.KEYDOT
platform.keyboard.keyOk        = jos.KEYOK

--
-- gprs
--
platform.gprs = {}
platform.gprs.connectionTimeout = 60
platform.gprs.initiateConnection =
   function ()
     wls.Init(wls.GPRS_G610)
     wls.Reset(wls.GPRS_G610)
     local simStatus = wls.CheckSim()
     if simStatus ~= wls.OK then
        error(string.format("wls.CheckSim: problem with sim card: %s", simStatus), 0)
     end
     --wls.InputUidPwd()
     wls.Dial("internet.vodafone.net")
   end

-- TODO "After Wls_NetClose() returns WLS_OK, it is possible the netlink does not close really, you should invoke Wls_CheckNetLink to check the netlink."
platform.gprs.disconnect = wls.NetClose

platform.gprs.waitUntilConnected =
   function (timeout)
     local timeout = timeout or platform.gprs.connectionTimeout
     local startTime = platform.getMonotonicTime()
     while true do
        local status = wls.CheckNetLink()
        local elapsedTime = platform.getMonotonicTime() - startTime
        if status == wls.OK then
           return elapsedTime
        elseif elapsedTime > timeout then
           error("platform.gprs.waitUntilConnected: timed out", 0)
        elseif status == wls.LINKOPENING then
           yield(status, elapsedTime)
        else
           error(string.format("platform.gprs.waitUntilConnected: unknown error: %d", status), 0)
        end
     end
   end


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
-- initialize the libs as the API mandates
--
