#include <limits.h> // bug in gcc config/build, needs to be included before stdlib.h, maybe others, too
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "luasocket.h"
#include "mime.h"

#include "ctosapi.h"

LUAMOD_API int luaopen_ctosapi(lua_State* L);
int genericMain(void);

char getKey(void)
{
    BYTE key;
    CTOS_KBDGet(&key);
    return key;
}

void clearScreen(void)
{
    CTOS_LCDTClearDisplay();
}

void printXY(int x, int y, const char* msg)
{
    CTOS_LCDTPrintXY(x + 1, y + 1, (char*)msg);
}

void apiBindingSetupHook(lua_State *L)
{
    lua_pushcfunction(L, luaopen_ctosapi);
    lua_setfield(L, -2, "ctosapi");
}

int
main(void)
{
    return genericMain();
}
