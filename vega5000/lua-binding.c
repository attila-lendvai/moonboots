#include <stdio.h>
#include <string.h>
#include <strings.h>

#include <lua.h>
#include <lauxlib.h>

#include "ctosapi.h"

/// TODO: comments marked with three /// means that it's not converted yet

/// TODO: convert all existing bindings to use this
#define CTOS_CALL(expr)                                                 \
    {                                                                   \
        USHORT ___ctos_result = (CTOS_##expr);                          \
        if (___ctos_result != d_OK)                                     \
            return luaL_error(L, "error from CTOS call '" #expr "', 0x%x", (unsigned int)___ctos_result); \
    }

static const char lua_checkChar(lua_State *L, int narg)
{
    const char *charstr = luaL_checkstring(L, narg);
    if (strlen(charstr) != 1)
        luaL_argerror(L, 1, "char args must be strings of length 1");
    return *charstr;
}

static const int lua_checkBoolean(lua_State *L, int narg)
{
    luaL_checktype(L, narg, LUA_TBOOLEAN);
    return lua_toboolean(L, narg);
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
        char* _copy_storage[sizeof(ctosapi_functions)];
        bzero(_copy_storage, sizeof(_copy_storage));

        const luaL_Reg* original = ctosapi_functions;
        luaL_Reg* copy = (luaL_Reg*)_copy_storage;

        while (original -> name)
        {
            const char* name = original -> name;
            const char* prefix = "CTOS_";
            if (strncmp(name, prefix, sizeof(prefix)) == 0)
                name = name + sizeof(prefix);

            copy -> name = name;
            copy -> func = original -> func;
            ++original;
            ++copy;
        }
        luaL_newlib(L, (luaL_Reg*)_copy_storage);
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
