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

	// table, cheeseburger
	lua_pushstring(L, "cheeseburger");
	lua_gettable(L, -2);
	// table, cheeseburger, len
	int result = lua_tonumber(L, -1);

	lua_pop(L, 1);

	lua_pushstring(L, "hamburger");
	lua_gettable(L, -2);
	int result2 = lua_tonumber(L, -1);


	// push to table
	lua_pop(L, 1);
	lua_pushstring(L, "test");
	lua_pushstring(L, "hello");
	lua_settable(L, -3);

	lua_pushstring(L, "test");
	lua_gettable(L, -2);

	const char *hello = lua_tostring(L, -1);


	printf("result is %i, %i\n", result, result2);
	printf("test=%s\n", hello);



	return 0;
}
