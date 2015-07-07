#include <stdio.h>
#include <string.h>
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"


static int l_bar(lua_State *L) {
	const char *str = lua_tostring(L, -1);

	int length;
	while (1) {
		char c = str[length];
		if (c == '\0')
			break;
		length++;
	}

	lua_pushstring(L, str);
	lua_pushnumber(L, length);

	return 2;
}



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

	lua_pushcfunction(L, l_bar);
	lua_setglobal(L, "bar");

	lua_getglobal(L, "foo");
	lua_pushstring(L, "cheeseburger");
	lua_pcall(L, 1, 1, 0);
	// foo, result
	int result = lua_tonumber(L, -1);




	printf("result is %i\n", result);



	return 0;
}
