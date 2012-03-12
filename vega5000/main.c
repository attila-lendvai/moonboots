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
logg(const char* msg)
{
    CTOS_LCDTPrintXY(1, 1, (char*)msg);
}

void
printErrorAndDie(const char *message)
{
    CTOS_LCDTPrintXY(1, 3, (char*)"Error:");
    CTOS_LCDTPrintXY(1, 4, (char*)message);

    getkey();
    exit(1);
}

void
outOfMemory(void)
{
    printErrorAndDie("out of memory");
}

void *
my_realloc(void *ptr, size_t size)
{
    void *result = realloc(ptr, size);
    if (! result)
        outOfMemory();
}

void
displayMultilineText(const char *text)
{
    int screenColumns = 16;
    int screenLines = 8;
    const char *beginningOfLine = text;
    int linesSize = 32;
    int linesIndex = -1;
    const char **lines = malloc(linesSize);

    for (int index = 0, currentColumn = 0; 1; ++index, ++currentColumn)
    {
        if ((index > 0) && (currentColumn % screenColumns) == 0 ||
            text[index] == 0 ||
            text[index] == 0x0A)
        {
            currentColumn = 0;
            ++linesIndex;
            if (linesIndex >= linesSize)
            {
                linesSize *= 2;
                lines = my_realloc(lines, linesSize);
            }
            lines[linesIndex] = beginningOfLine;

            if (text[index] == 0)
                break;

            beginningOfLine = text + index;

            if (*beginningOfLine == 0x0A)
                ++beginningOfLine;
        }
    }

    {
        int lineCount = linesIndex + 1;
        int currentLineOffset = 0;
        while (1)
        {
            for (int currentLine = currentLineOffset, screenLine = 0;
                 screenLine < screenLines;
                 ++currentLine, ++screenLine)
            {
                const char *line = lines[currentLine];
                int column = 0;

                CTOS_LCDTGotoXY(1, screenLine + 1);
                if (currentLine < lineCount)
                {
                    while (column < screenColumns &&
                           line[column] != 0 &&
                           line[column] != 0x0A)
                    {
                        const char ch = line[column];
                        if (ch >= 0x20 && ch < 127)
                            CTOS_LCDTPutch(ch);
                        else
                            CTOS_LCDTPutch('_'); // just some random stuff to mark gibberish chars
                        ++column;
                    }
                }
                CTOS_LCDTClear2EOL();
            }
            switch (getkey())
            {
              case d_KBD_DOWN:
                if (currentLineOffset < (lineCount + screenLines))
                    ++currentLineOffset;
                break;

              case d_KBD_UP:
                if (currentLineOffset > 0)
                    --currentLineOffset;
                break;

              case d_KBD_CANCEL:
                goto ret;
                break;
            }
        }
    }
ret:
    free(lines);
    CTOS_LCDTClearDisplay();
}

void
printLuaErrorAndDie(lua_State *L, const char *errorContext)
{
    //lineBuffer[lineBufferIndex++] = "Lua error:";
    //if (errorContext)
    //    lineBuffer[lineBufferIndex++] = errorContext;

    displayMultilineText(lua_tostring(L, -1));

    exit(1);
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

    {
        // register the lua modules
        lua_getglobal(L, "package");
        lua_getfield(L, -1, "preload");

        lua_pushcfunction(L, luaopen_ctosapi);
        lua_setfield(L, -2, "ctosapi");

        lua_pushcfunction(L, luaopen_socket_core);
        lua_setfield(L, -2, "socket.core");

        lua_pushcfunction(L, luaopen_mime_core);
        lua_setfield(L, -2, "mime.core");

        lua_pop(L, 2);
    }

    {
        // set lua load path
        lua_getglobal(L, "package");
        lua_pushstring(L, "./lua/?.lua;?/init.lua");
        lua_setfield(L, -2, "path");
    }

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
