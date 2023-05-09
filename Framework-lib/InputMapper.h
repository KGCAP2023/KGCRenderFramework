#pragma once
#include "InputManager.h"

class InputMapper
{
public:

	static int KeyInput_W(lua_State* L)
	{
		auto kb = InputManager::GetKeyboardState();
		if (kb.W)
			lua_pushboolean(L, true);
		else
			lua_pushboolean(L, false);
		return 1;
	}

	static int KeyInput_A(lua_State* L)
	{
		auto kb = InputManager::GetKeyboardState();
		if (kb.A)
			lua_pushboolean(L, true);
		else
			lua_pushboolean(L, false);
		return 1;
	}

	static int KeyInput_S(lua_State* L)
	{
		auto kb = InputManager::GetKeyboardState();
		if (kb.S)
			lua_pushboolean(L, true);
		else
			lua_pushboolean(L, false);
		return 1;
	}

	static int KeyInput_D(lua_State* L)
	{
		auto kb = InputManager::GetKeyboardState();
		if (kb.D)
			lua_pushboolean(L, true);
		else
			lua_pushboolean(L, false);
		return 1;
	}

};

