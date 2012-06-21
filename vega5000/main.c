#include <stdlib.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "ctosapi.h"
#include "bootstrap.h"

LUAMOD_API int luaopen_ctosapi(lua_State* L);

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
    CTOS_LCDTClear2EOL();
}

void platformLuaBindingSetupHook(lua_State *L)
{
    {
        // register the lua modules
        lua_getglobal(L, "package");
        lua_getfield(L, -1, "preload");

        lua_pushcfunction(L, luaopen_ctosapi);
        lua_setfield(L, -2, "castles.ctosapi");

        lua_pop(L, 2);
    }

    {
        // define "platform" global table
        lua_getglobal(L, "_G");
        lua_newtable(L);
        lua_setfield(L, -2, "platform");

        lua_getfield(L, -1, "platform");

        lua_pushstring(L, "castles");
        lua_setfield(L, -2, "kind");

        lua_pushstring(L, "vega5000");
        lua_setfield(L, -2, "model");

        lua_pop(L, 2);
    }

}

void *myRealloc(void *ptr, size_t size)
{
    void *result = realloc(ptr, size);
    if (! result)
        outOfMemory();
    return result;
}

void displayMultilineText(const char *text)
{
    int screenColumns = 16;
    int screenRows = 8;
    const char *beginningOfLine = text;
    int linesSize = 32;
    int linesIndex = -1;
    const char **lines = myRealloc(NULL, linesSize);

    for (int index = 0, currentColumn = 0; 1; ++index, ++currentColumn)
    {
        if ((index > 0 && (currentColumn % screenColumns) == 0) ||
            text[index] == 0 ||
            text[index] == 0x0A)
        {
            currentColumn = 0;
            ++linesIndex;
            if (linesIndex >= linesSize)
            {
                linesSize *= 2;
                lines = myRealloc(lines, linesSize);
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
                 screenLine < screenRows;
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
            switch (getKey())
            {
              case d_KBD_DOWN:
                if (currentLineOffset < (lineCount + screenRows))
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

int main(void)
{
    return genericMain();
}
