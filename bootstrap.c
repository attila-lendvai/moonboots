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

#include "bootstrap.h"

#include "build/main.lua.dump"

void startupLog(const char* msg)
{
    printXY(0, 0, msg);
}

void printErrorAndDie(const char *message)
{
    // TODO use paging

    printXY(0, 0, message);

    getKey();
    exit(1);
}

void printLuaErrorAndDie(lua_State *L, const char *errorContext)
{
    // TODO errorContext?
    printErrorAndDie(lua_tostring(L, -1));
}

void outOfMemory(void)
{
    printErrorAndDie("out of memory");
}

static int luaPanicHook(lua_State *L)
{
    // TODO use paging
    printXY(0, 3, "Lua panic:");
    printXY(0, 4, (char*)lua_tostring(L, -1));

    getKey();
    return 0;  // return to Lua to abort
}

int genericMain(void)
{
    int status;
    lua_State *L = NULL;

    clearScreen();
    startupLog("Initializing...");

    L = luaL_newstate();
    lua_atpanic(L, &luaPanicHook);

    luaL_openlibs(L);

    // register the lua modules provided from the C side
    platformLuaBindingSetupHook(L);

    {
        // set lua load path
        lua_getglobal(L, "package");
        lua_pushstring(L, "./lua/?.lua;?/init.lua");
        lua_setfield(L, -2, "path");
    }

    startupLog("Running script...");

    main_lua[main_lua_len - 1] = 0;
    status = luaL_dostring(L, main_lua);
    if (status)
        printLuaErrorAndDie(L, "running main.lua");

    clearScreen();
    startupLog("Cleaning up...");

    lua_pop(L, 1);  // Take the returned value out of the stack
    lua_close(L);   // Cya, Lua

    startupLog("Exiting...");
    return 0;
}
