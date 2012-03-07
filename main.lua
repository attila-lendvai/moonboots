local ctos = require "ctosapi"

ctos.LCDTPrintXY(1, 3, "hello from lua")
ctos.LCDTPrintXY(1, 4, string.sub(ctos.GetSerialNumber(), 0, 16))
ctos.LCDTPrintXY(1, 5, string.sub(ctos.GetSerialNumber(), 16))
-- ctos.LCDTPrintXY(1, 5, ctos.GetFactorySN())
repeat
   key = ctos.KBDGet()
   ctos.LCDTPrintXY(1, 6, key)
until key == ctos.KBD_CANCEL

-- warning: the last character of this file will be overwritten, so keep this comment here...
