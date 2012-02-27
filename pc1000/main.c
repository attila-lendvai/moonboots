#include <limits.h> // bug in gcc config/build, needs to be included before stdlib.h, maybe others, too
#include <stdlib.h>
#include <stdio.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <josapi.h>

const APP_MSG App_Msg=
{
  "bootstrap", "1.0", 0, 0, 0, "iSoft", "descr", "", 0, ""
};

unsigned char *main_lua;
LUAMOD_API int luaopen_josapi(lua_State* L);

void
printLuaErrorAndDie(lua_State *L, const char *errorContext)
{
    Lib_LcdGotoxy(0, 2);
    Lib_Lcdprintf("E: ");
    if (errorContext)
        Lib_Lcdprintf((char *)errorContext);
    Lib_LcdGotoxy(0, 3);
    Lib_Lcdprintf((char *)lua_tostring(L, -1));

    Lib_KbGetCh();
    exit(1);
}

int
log(char* msg)
{
    Lib_LcdGotoxy(1, 8);
    Lib_Lcdprintf(msg);

    Lib_KbGetCh();
}

static int
lua_panic_hook(lua_State *L)
{
    Lib_LcdGotoxy(1, 8);
    Lib_Lcdprintf("Fatal: %s", lua_tostring(L, -1));
    Lib_KbGetCh();
    return 0;  /* return to Lua to abort */
}

static void*
lua_alloc_hook(void *ud, void *ptr, size_t osize, size_t nsize) {
    (void)ud; (void)osize;  /* not used */
    if (nsize == 0)
    {
        free(ptr);
        return NULL;
    }
    else if (ptr == NULL)
    {
        return malloc(nsize);
    }
    else
    {
        void *new = malloc(nsize);
        memcpy(new, ptr, nsize);
        free(ptr);
        return new;
    }
}

int
main(void)
{
    int status, result, i;
    double sum;
    lua_State *L = NULL;

    Lib_AppInit();

    log("malloc");

    malloc(42);

    log("lua newstate");

    L = lua_newstate(lua_alloc_hook, NULL);
    lua_atpanic(L, &lua_panic_hook);

    log("lua openlibs");
    luaL_openlibs(L);

    log("lua register");

    // register josapi
    lua_getglobal(L, "package");
    lua_getfield(L, -1, "preload");
    lua_pushcfunction(L, luaopen_josapi);
    lua_setfield(L, -2, "josapi");
    lua_pop(L, 2);

    /*
      to make it unconditionally available, do this:
      lua_pushcfunction(L, luaopen_mylibraryname);
      lua_pushstring(L, "mylibraryname");
      lua_call(L, 1, 0);
    */
    log("lua dostring");

    status = luaL_dostring(L, main_lua);
    if (status)
    {
        printLuaErrorAndDie(L, "running main.lua");
    }

    log("lua closing");
    lua_pop(L, 1);  /* Take the returned value out of the stack */
    lua_close(L);   /* Cya, Lua */

    Lib_LcdGotoxy(0, 2);
    Lib_Lcdprintf("Finished.");

    Lib_KbGetCh();
    return 0;
}
