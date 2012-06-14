#ifndef _BOOTSTRAP_H_
#define _BOOTSTRAP_H_ 1

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#ifdef pc1000
  #include "user-malloc.h"
#endif

int genericMain(void);
void outOfMemory(void);
void printErrorAndDie(const char *message);
struct luaL_Reg *copyLuaFunctionTable(const luaL_Reg *sourceEntries, const char *prefixToDrop);

// these functions should be defined in the platform specific main.c
char getKey(void);
void clearScreen(void);
void printXY(int x, int y, const char *message);
void platformLuaBindingSetupHook(lua_State *L);
void displayMultilineText(const char *text); // display the given text in a pagable manner, waiting returning when the cancel/esc button is pressed

// hack to turn the line number into a string
#define _STR(x) #x
#define _STR2(x) _STR(x)
#define S__LINE__ _STR2(__LINE__)

#define getLuaNumberArgument(narg, target, lowLimit, highLimit)         \
  {                                                                     \
    lua_Number num = luaL_checknumber(L, narg);                         \
    if (num < lowLimit || num > highLimit)                              \
        return luaL_error(L, "parameter '" #target "' is %f, which is not within limits [%f, %f] at " S__LINE__ "@"  __FILE__, (double)num, (double)lowLimit, (double)highLimit); \
    target = num;                                                       \
  }

static const inline int lua_checkBoolean(lua_State *L, int narg)
{
    luaL_checktype(L, narg, LUA_TBOOLEAN);
    return lua_toboolean(L, narg);
}

#endif
