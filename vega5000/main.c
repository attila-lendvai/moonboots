#include <limits.h> // bug in gcc config/build, needs to be included before stdlib.h, maybe others, too
#include <stdlib.h>
#include <stdio.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <ctosapi.h>

#include "../build/main.lua.dump"

LUAMOD_API int luaopen_ctosapi(lua_State* L);

BYTE
getkey(void)
{
    BYTE key;
    CTOS_KBDGet(&key);
    return key;
}

void
printLuaErrorAndDie(lua_State *L, const char *errorContext)
{
    CTOS_LCDTPrintXY(1, 3, (char*)"Lua error:");
    if (errorContext)
        CTOS_LCDTPrintXY(1, 4, (char*)errorContext);
    CTOS_LCDTPrintXY(1, 5, (char*)lua_tostring(L, -1));

    getkey();
    exit(1);
}

void
logg(const char* msg)
{
    CTOS_LCDTPrintXY(1, 1, (char*)msg);
}

static int
lua_panic_hook(lua_State *L)
{
    CTOS_LCDTPrintXY(0, 3, "Lua panic:");
    CTOS_LCDTPrintXY(0, 4, (char*)lua_tostring(L, -1));

    getkey();
    return 0;  // return to Lua to abort
}

static void*
lua_alloc_hook(void *ud, void *ptr, size_t osize, size_t nsize) {
    (void)ud; (void)osize;  // not used
    if (nsize == 0) {
        free(ptr);
        return NULL;
    }
    else
    {
        return realloc(ptr, nsize);
    }
}

int
main(void)
{
    int status, result, i;
    double sum;
    lua_State *L = NULL;

    CTOS_LCDTClearDisplay();
    logg("Initializing...");

    L = lua_newstate(lua_alloc_hook, NULL);
    lua_atpanic(L, &lua_panic_hook);

    luaL_openlibs(L);

    // register ctosapi
    lua_getglobal(L, "package");
    lua_getfield(L, -1, "preload");
    lua_pushcfunction(L, luaopen_ctosapi);
    lua_setfield(L, -2, "ctosapi");
    lua_pop(L, 2);

    // to make it unconditionally available, do this:
    //lua_pushcfunction(L, luaopen_ctosapi);
    //lua_pushstring(L, "ctosapi");
    //lua_call(L, 1, 0);

    logg("Running script...");
    main_lua[main_lua_len - 1] = 0;
    status = luaL_dostring(L, main_lua);
    if (status)
    {
        printLuaErrorAndDie(L, "running main.lua");
    }

    CTOS_LCDTClearDisplay();

    logg("Cleaning up...");
    lua_pop(L, 1);  // Take the returned value out of the stack
    lua_close(L);   // Cya, Lua

    logg("Exiting...");
    return 0;
}
