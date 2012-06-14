-- -*- mode: lua; coding: utf-8 -*-

function localizedError(message, ...)
   -- TODO translate the message
   if select('#', ...) > 0 then
      message = string.format(message, ...)
   end
   error(message, 1)
end

