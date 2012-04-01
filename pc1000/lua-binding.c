#include <string.h>
#include <stdlib.h>

#include <lua.h>
#include <lauxlib.h>

#include "josapi.h"
#include "wlsapi.h"

typedef struct luaL_Cst {
	const char* name;
	int value;
} luaL_Cst;

static luaL_Cst josapi_constants[] = {
#define BIND(name) {#name, name},
    #include "lua-binding-constants.inc"
#undef BIND
    {0, 0},
};

#include "lua-binding-functions.inc"

LUAMOD_API int luaopen_josapi(lua_State* L)
{
    // functions
    {
        char* _copy_storage[sizeof(josapi_functions)];
        memset(_copy_storage, 0, sizeof(_copy_storage));

        const luaL_Reg* original = josapi_functions;
        luaL_Reg* copy = (luaL_Reg*)_copy_storage;

        while (original -> name)
        {
            const char* name = original -> name;
            const char* prefix = "Lib_";
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
