#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <malloc.h>

#include <lua.h>
#include <lauxlib.h>

#include "bootstrap.h"
#include "ctosapi.h"

/// TODO: comments marked with three /// means that it's not converted yet

/// TODO: convert all existing bindings to use this
/// TODO: check if %x works fine. on the pc1000 it doesn't, and i need to use %d instead (also search for this, it's used at other places also)
#define CTOS_CALL(expr)                                                 \
    {                                                                   \
        USHORT ___ctos_result = (CTOS_##expr);                          \
        if (___ctos_result != d_OK)                                     \
            return luaL_error(L, "error from CTOS call '" #expr "', code %d", (int)___ctos_result); \
    }

#define CTOS_CALL_2(expr, errorCode2)                                   \
    {                                                                   \
        USHORT ___ctos_result = (CTOS_##expr);                          \
        if (___ctos_result != d_OK && ___ctos_result != errorCode2)     \
            return luaL_error(L, "error from CTOS call '" #expr "', code %d", (int)___ctos_result); \
    }

static const char lua_checkChar(lua_State *L, int narg)
{
    const char *charstr = luaL_checkstring(L, narg);
    if (strlen(charstr) != 1)
        luaL_argerror(L, 1, "char args must be strings of length 1");
    return *charstr;
}

char digit_to_hexa_char(unsigned char digit)
{
    if (digit > 15)
        return '?';
    else if (digit < 10)
        return '0' + digit;
    else
        return 'a' + (digit - 10);
}

void
bytes_to_hexa_string(unsigned char *bytes, unsigned int count, char *result)
{
    for (int i = 0; i < count; i++)
    {
        unsigned char byte = bytes[i];
        result[i * 2] = digit_to_hexa_char((byte >> 4) & 0xf);
        result[i * 2 + 1] = digit_to_hexa_char(byte & 0xf);
    }
    result[(2 * count)] = 0;
}

typedef struct luaL_Cst {
	const char* name;
	int value;
} luaL_Cst;

static luaL_Cst ctosapi_constants[] = {
#define BIND(name) {#name, name},
    #include "lua-binding-constants.inc"
    #include "lua-binding-errors.inc"
#undef BIND
	{0, 0},
};

#include "lua-binding-functions.inc"

LUAMOD_API int luaopen_ctosapi(lua_State* L)
{
    // functions
    {
        const luaL_Reg* copy = copyLuaFunctionTable(ctosapi_functions, "CTOS_");
        luaL_newlib(L, (luaL_Reg*)copy);
        free((void *)copy);
    }

    // constants
    {
	for (luaL_Cst* it = ctosapi_constants; it -> name; ++it)
        {
            const char* name = it -> name;
            if (strncmp(name, "d_", 2) == 0)
                name = name + 2;

            lua_pushnumber(L, it->value);
            lua_setfield(L, -2, name);
        }

        //lua_pushnumber(L, d_EVENT_KBD | d_EVENT_SC | d_EVENT_MSR | d_EVENT_MODEM | d_EVENT_ETHERNET | d_EVENT_COM1 | d_EVENT_COM2);
        //lua_setfield(L, -2, "EVENT_ANY");
    }
    return 1;
}
