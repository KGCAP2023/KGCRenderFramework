#pragma once
#include "pch.h"
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include "GameObject.h"
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



	void registerGameObjectManager(lua_State* lua);

	
	static int lua_GameObjectManager_getGameObject(lua_State* lua);

	static int lua_GameObject_getName(lua_State* lua)
	{
		GameObject** pptr = (GameObject**)luaL_checkudata(lua, 1, "GameObjectMetaTable");
		lua_pushstring(lua, (*pptr)->GetName().c_str());
		//lua_pushnumber(lua, 123);
		return 1;
	}

	static void registerGameObject(lua_State* lua, GameObject* obj)
	{
		//We assume that the person is a valid pointer
		GameObject** pptr = (GameObject**)lua_newuserdata(lua, sizeof(GameObject*));
		*pptr = obj; //Store the pointer in userdata. You must take care to ensure 
						//the pointer is valid entire time Lua has access to it.

		if (luaL_newmetatable(lua, "GameObjectMetaTable")) { //This is important. Since you 
			//may invoke it many times, you should check, whether the table is newly 
			//created or it already exists

			//The table is newly created, so we register its functions
			lua_pushvalue(lua, -1);
			lua_setfield(lua, -2, "__index");

			luaL_Reg GameObjectFunctions[] = {
				"GetName", lua_GameObject_getName,
				nullptr, nullptr
			};

			luaL_setfuncs(lua, GameObjectFunctions, 0);

			//luaL_newlib(lua, GameObjectFunctions);
		}

		lua_setmetatable(lua, -2);
		
	}

	std::vector<std::string>& GetLogBuffer() { return this->logBuffer; };

private:
	Framework* framework = nullptr;
	ResourceManager* res = nullptr;
	GameObjectManager* objManager = nullptr;
	std::vector<std::string> logBuffer;
};