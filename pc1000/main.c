#include <limits.h> // bug in gcc config/build, needs to be included before stdlib.h, maybe others, too
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <setjmp.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "josapi.h"
#include "bootstrap.h"

// malloc is broken in their system, so we set up ourselves a heap and shadow theirs at link time for our codebase
mspace heap;
extern char malloc_heap_start_address[]; // defined in the ldscript

extern void *__wrap_malloc(size_t size)
{
    void *result = mspace_malloc(heap, size);
    if (! result)
        outOfMemory();
    return result;
}

extern void __wrap_free(void *ptr)
{
    mspace_free(heap, ptr);
}

extern void *__wrap_calloc(size_t count, size_t size)
{
    void *result = mspace_calloc(heap, count, size);
    if (! result)
        outOfMemory();
    return result;
}

extern void *__wrap_realloc(void *ptr, size_t size)
{
    void *result = mspace_realloc(heap, ptr, size);
    if (size != 0 && ! result)
        outOfMemory();
    return result;
}

// application data structure
const APP_MSG App_Msg=
{
  "bootstrap", "1.0", 0, 0, 0, "iSoft", "descr", "", 0, ""
};

LUAMOD_API int luaopen_josapi(lua_State* L);
LUAMOD_API int luaopen_wlsapi(lua_State* L);

char getKey(void)
{
    return Lib_KbGetCh();
}

void clearScreen(void)
{
    Lib_LcdCls();
}

void printXY(int x, int y, const char* msg)
{
    Lib_LcdPrintxy(x * 8, y * 8, 0, (char*)msg);
}

void platformLuaBindingSetupHook(lua_State *L)
{
    {
        // register the lua modules
        lua_getglobal(L, "package");
        lua_getfield(L, -1, "preload");

        lua_pushcfunction(L, luaopen_josapi);
        lua_setfield(L, -2, "justtide.josapi");

        lua_pushcfunction(L, luaopen_wlsapi);
        lua_setfield(L, -2, "justtide.wlsapi");

        lua_pop(L, 2);
    }

    {
        // define "platform" global table
        lua_getglobal(L, "_G");
        lua_newtable(L);
        lua_setfield(L, -2, "platform");

        lua_getfield(L, -1, "platform");

        lua_pushstring(L, "justtide");
        lua_setfield(L, -2, "kind");

        lua_pushstring(L, "pc1000");
        lua_setfield(L, -2, "model");

        lua_pop(L, 2);
    }
}

void displayMultilineText(const char *text)
{
    // TODO this is only a kludge
    Lib_LcdSetFont(8, 16, 0);
    Lib_LcdPrintxy(0, 0, 0, (char*)text);
    getKey();
    Lib_LcdCls();
}


jmp_buf exitJmpEnv;
void __wrap_exit(int status)
{
    longjmp(exitJmpEnv, status);
}

int
main(void)
{
    Lib_AppInit();
    Lib_LcdCls();

    // initialize the heap space of our malloc/free replacement
    void *heap_start = malloc_heap_start_address;

    Lib_LcdPrintxy(0, 0, 0, "Heap is at: 0x%x", heap_start);

    heap_start += 32 * 1024; // TODO FIXME this is needed to keep things stable. maybe because the stack is at the end of the link space?
    unsigned long heap_size = 0x20700000 - (unsigned long)heap_start;
    memset(heap_start, 0, heap_size);
    heap = create_mspace_with_base(heap_start, heap_size, 0);

    // exit() is broken in their API, so we need to set up a 
    int result;
    if ((result = setjmp(exitJmpEnv)))
    {
        return result;
    }
    else
    {
        Lib_LcdPrintxy(0, 8, 0, "Lowlevel init done", heap_start);
        return genericMain();
    }
}
