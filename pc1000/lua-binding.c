#include <string.h>
#include <stdlib.h>

#include <lua.h>
#include <lauxlib.h>

#include "bootstrap.h"
#include "josapi.h"
#include "wlsapi.h"

typedef struct luaL_Cst {
	const char* name;
	int value;
} luaL_Cst;

#define BIND(name) {#name, name},
static luaL_Cst josapi_constants[] = {
    #include "lua-binding-jos-constants.inc"
    {0, 0},
};

static luaL_Cst wlsapi_constants[] = {
    #include "lua-binding-wls-constants.inc"
    {0, 0},
};
#undef BIND

inline unsigned char bcdToHexa(unsigned char bcd)
{
    return (((bcd >> 4) & 0x0f) * 10 + (bcd & 0x0f));
}

int pushBCDDateAsLuaTable(lua_State* L, unsigned char *bcdDate)
{
    void setfield(const char *key, int value)
    {
        lua_pushinteger(L, value);
        lua_setfield(L, -2, key);
    }

    int year, month, day, hour, minute, second;

    Lib_GetDateTime(bcdDate);

    year = 1900 + bcdToHexa(bcdDate[0]);
    if (bcdDate[0] <= 0x49)
        year += 100;
    month  = bcdToHexa(bcdDate[1]);
    day    = bcdToHexa(bcdDate[2]);
    hour   = bcdToHexa(bcdDate[3]);
    minute = bcdToHexa(bcdDate[4]);
    second = bcdToHexa(bcdDate[5]);

    lua_createtable(L, 0, 6);
    setfield("second", second);
    setfield("minute", minute);
    setfield("hour",   hour);
    setfield("day",    day);
    setfield("month",  month);
    setfield("year",   year);

    return 1;
}

#include "lua-binding-jos-functions.inc"
#include "lua-binding-wls-functions.inc"

LUAMOD_API int luaopen_josapi(lua_State* L)
{
    // functions
    {
        luaL_Reg* copy = copyLuaFunctionTable(josapi_functions, "Lib_");
        luaL_newlib(L, (luaL_Reg*)copy);
        free(copy);
    }

    // constants
    {
	for (luaL_Cst* it = josapi_constants; it -> name; ++it)
        {
            const char* name = it -> name;
            //if (strncmp(name, "d_", 2) == 0)
            //    name = name + 2;

            lua_pushnumber(L, it->value);
            lua_setfield(L, -2, name);
        }
    }
    return 1;
}

LUAMOD_API int luaopen_wlsapi(lua_State* L)
{
    // functions
    {
        luaL_Reg* copy = copyLuaFunctionTable(wlsapi_functions, "Wls_");
        luaL_newlib(L, copy);
        free(copy);
    }

    // constants
    {
	for (luaL_Cst* it = wlsapi_constants; it -> name; ++it)
        {
            const char* name = it -> name;
            if (strncmp(name, "WLS_", 4) == 0)
                name = name + 4;

            lua_pushnumber(L, it->value);
            lua_setfield(L, -2, name);
        }
    }
    return 1;
}
