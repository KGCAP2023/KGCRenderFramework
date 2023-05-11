#pragma once
#include "pch.h"
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include "GameObject.h"
#include "GameObjectMapper.h"

class Framework;
class ResourceManager;
class GameObjectManager;

class LuaManager
{
public:
	LuaManager()
	{
		this->logBuffer.reserve(500);
	};

	bool Initialize(Framework* framework);
	bool ExecuteExample1();
	bool ExecuteGUITest();

	GameObjectMapper* GetGameObjectMapper();

	lua_State* Lua_Begin();
	void Lua_End(lua_State* L);

	// Little error checking utility function
	bool CheckLua(lua_State* L, int r)
	{
		if (r != LUA_OK)
		{
			std::string errormsg = lua_tostring(L, -1);
			std::cout << errormsg << std::endl;
			this->logBuffer.push_back(errormsg);
			return false;
		}
		return true;
	}

	static int lua_HostFunction(lua_State* L)
	{
		float a = (float)lua_tonumber(L, 1);
		float b = (float)lua_tonumber(L, 2);
		std::cout << "[CPP S4] HostFunction(" << a << ", " << b << ") called from Lua" << std::endl;
		float c = a * b;
		lua_pushnumber(L, c);
		return 1;
	}

	static int lua_ImGuiBegin(lua_State* L)
	{
		std::string name = lua_tostring(L, 1);
		ImGui::Begin(name.c_str());
		return 0;
	}

	static int lua_ImGuiText(lua_State* L)
	{
		std::string name = lua_tostring(L, 1);
		ImGui::Text(name.c_str());
		return 0;
	}

	static int lua_ImGuiEnd(lua_State* L)
	{
		ImGui::End();
		return 0;
	}


	static int lua_LoadAudio(lua_State* lua);
	static int lua_PlayAudio(lua_State* lua);
	static int lua_DeleteAudio(lua_State* lua);
	static int lua_SetAudioVolume(lua_State* lua);
	static int lua_PauseAudio(lua_State* lua);
	static int lua_ResumeAudio(lua_State* lua);
	static int lua_StopAudio(lua_State* lua);

	void registerAudioManager(lua_State* lua);

	void registerCamera(lua_State* lua);
	static int lua_Camera_TraceObject(lua_State* lua);

	static void dumpstack(lua_State* L) {
		int top = lua_gettop(L);
		for (int i = 1; i <= top; i++) {
			printf("%d\t%s\t", i, luaL_typename(L, i));
			switch (lua_type(L, i)) {
			case LUA_TNUMBER:
				printf("%g\n", lua_tonumber(L, i));
				break;
			case LUA_TSTRING:
				printf("%s\n", lua_tostring(L, i));
				break;
			case LUA_TBOOLEAN:
				printf("%s\n", (lua_toboolean(L, i) ? "true" : "false"));
				break;
			case LUA_TNIL:
				printf("%s\n", "nil");
				break;
			default:
				printf("%p\n", lua_topointer(L, i));
				break;
			}
		}
	}
	std::vector<std::string>& GetLogBuffer() { return this->logBuffer; };

private:
	Framework* framework = nullptr;
	ResourceManager* res = nullptr;
	GameObjectManager* objManager = nullptr;
	GameObjectMapper* gameMapper = nullptr;
	std::vector<std::string> logBuffer;
};