#include <iostream>
#include <lua.hpp>
using namespace std;

//extern "C" {
//	#include "lua.h"
//	#include "lualib.h"
//	#include "lauxlib.h"
//}


lua_State *L;

static int average(lua_State *L) {
	// Get args
	int n = lua_gettop(L);
	double sum = 0;
	int i;

	// Loop and summate
	for (i = 1; i <= n; i++)
		sum += lua_tonumber(L, i);
	
	// push average and sum onto the stack
	lua_pushnumber(L, sum / n);
	lua_pushnumber(L, sum);

	// return the number of results
	return 2;
}

int main(int argc, char *argv[]) {
	// Open & init lua
	L = luaL_newstate();
	luaL_openlibs(L);

	// register our funciton
	lua_register(L, "average", average);

	// run the script
	luaL_dofile(L, "avg.lua");

	// cleanup
	lua_close(L);

	cin.get();

	return 0;
}

