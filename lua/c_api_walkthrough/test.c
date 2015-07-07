#include <stdio.h>
#include <string.h>
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

int main() {
	// Init state
	lua_State *L = luaL_newstate();

	// load libs
	luaL_openlibs(L);

	//  Do something
	luaL_dofile(L, "test.lua");

	// Variables
//	lua_getglobal(L, "foo");
//	if (lua_isnil(L, -1))
//		printf("Is nil!\n");
//	else {
//		int i = lua_tonumber(L, -1);
//		printf("Th num is %i\n", i);
//	}

	// Functions
	lua_getglobal(L, "foo");		
	// foo
	lua_pushnumber(L, 4);
	// foo, 2
	lua_pcall(L, 1, 1, 0);
	// foo, result
	int result = lua_tonumber(L, -1);

	printf("result is %i\n", result);

	// Stack
	lua_getglobal(L, "x");			// foo, x

	int x = lua_tonumber(L, -1);
	printf("x=%i\n", x);

	return 0;
}
