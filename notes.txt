lua binding policy:
 - every measure is in its natural unit, e.g. time in seconds
 - functions and variables are never renamed (not even DelayMS, even though it accepts seconds). obvious prefixes are removed.
 - functions rise lua errors, unless return codes are part of normal usage (e.g. Wls_CheckNetLink)


broken:
 - on pc1000 string format using %f




--[[

pc1000 stress test

function foo(level)
   if level > 1000 then
      return 42
   else
      local result = foo(level + 1)
      return result
   end
end

foo(0)

while not jos.KbCheck() or jos.KbGetCh() ~= jos.KEYCANCEL do
   local t = {}
   for i = 1, math.random() * 100, 1 do
      local t2 = {}
      for j = 1, math.random() * 100, 1 do
         table.insert(t2, function () return t end)
      end
      table.insert(t, t2)
   end
   local k, b = collectgarbage("count")
   --jos.LcdPrintxy(0, 0, 0, string.format("%d kb, %d.", k, platform.getMonotonicTime()))
   platform.display.printXY(1, 1, "%d kb, %d.", k, platform.getMonotonicTime())
end

os.exit()
--]]
