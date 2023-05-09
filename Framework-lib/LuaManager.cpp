#include "pch.h"
#include "LuaManager.h"
#include "Framework.h"


bool LuaManager::Initialize(Framework* framework)
{
	if (framework == nullptr) return false;
	else
	{
		this->framework = framework;
		this->res = &framework->resourceManager;
		this->gameMapper = new GameObjectMapper(framework);
		this->objManager = framework->gameObjManager;
	}
	return true;
}

lua_State* LuaManager::Lua_Begin()
{
	//lua_State 생성
	lua_State* L = luaL_newstate();

	//표준 라이브러리를 루아 가성머신에 추가
	luaL_openlibs(L);

	return L;
}

void LuaManager::Lua_End(lua_State* L)
{
	lua_close(L);
}

void LuaManager::registerAudioManager(lua_State* lua)
{

	AudioManager* audioManager = &framework->audioManager;
	AudioManager** amPtr = (AudioManager**)lua_newuserdata(lua, sizeof(AudioManager*));
	*amPtr = audioManager;

	luaL_newmetatable(lua, "AudioManagerMetaTable");

	lua_pushvalue(lua, -1);
	lua_setfield(lua, -2, "__index");

	luaL_Reg audioManagerFunctions[] = {
		"LoadAudio",lua_LoadAudio,//저장할 이름 (str), 경로(str)
		"PlayAudio",lua_PlayAudio,//오디오 이름(str)
		"StopAudio",lua_StopAudio,//오디오 이름(str)
		"DeleteAudio",lua_DeleteAudio,//오디오 이름(str)
		"SetAudioVolume",lua_SetAudioVolume,//오디오 이름(str), 설정 볼륨(int [0~10])
		"PauseAudio",lua_PauseAudio,//오디오 이름(str)
		"ResumeAudio",lua_ResumeAudio,//오디오 이름(str)
		nullptr, nullptr
	};
	luaL_setfuncs(lua, audioManagerFunctions, 0);

	lua_setmetatable(lua, -2);

	lua_setglobal(lua, "AudioManager");

	// 사용법 => AudioManager:PlayAudio("audioName")
}

#pragma region 오디오메니저_관련_래핑_함수들

int LuaManager::lua_LoadAudio(lua_State* lua) {
	
	AudioManager** pmPtr = (AudioManager**)luaL_checkudata(
		lua, 1, "AudioManagerMetaTable");

	std::string audioName = luaL_checkstring(lua, 2);
	std::string path = luaL_checkstring(lua, 3);

	(*pmPtr)->LoadAudio(audioName.c_str(), path.c_str());

	return 1;
}

int LuaManager::lua_PlayAudio(lua_State* lua) {

	AudioManager** pmPtr = (AudioManager**)luaL_checkudata(
		lua, 1, "AudioManagerMetaTable");

	std::string audioName = luaL_checkstring(lua, 2);

	(*pmPtr)->PlayAudio(audioName.c_str());

	return 1;
}

int LuaManager::lua_StopAudio(lua_State* lua) {
	
	AudioManager** pmPtr = (AudioManager**)luaL_checkudata(
		lua, 1, "AudioManagerMetaTable");

	std::string audioName = luaL_checkstring(lua, 2);

	(*pmPtr)->StopAudio(audioName.c_str());

	return 1;

}

int LuaManager::lua_DeleteAudio(lua_State* lua) {
	
	AudioManager** pmPtr = (AudioManager**)luaL_checkudata(
		lua, 1, "AudioManagerMetaTable");

	std::string audioName = luaL_checkstring(lua, 2);

	(*pmPtr)->DeleteAudio(audioName.c_str());

	return 1;

}

int LuaManager::lua_PauseAudio(lua_State* lua) {

	AudioManager** pmPtr = (AudioManager**)luaL_checkudata(
		lua, 1, "AudioManagerMetaTable");

	std::string audioName = luaL_checkstring(lua, 2);

	(*pmPtr)->PauseAudio(audioName.c_str());

	return 1;
}

int LuaManager::lua_ResumeAudio(lua_State* lua) {
	
	AudioManager** pmPtr = (AudioManager**)luaL_checkudata(
		lua, 1, "AudioManagerMetaTable");

	std::string audioName = luaL_checkstring(lua, 2);

	(*pmPtr)->ResumeAudio(audioName.c_str());

	return 1;

}

int LuaManager::lua_SetAudioVolume(lua_State* lua) {

	AudioManager** pmPtr = (AudioManager**)luaL_checkudata(
		lua, 1, "AudioManagerMetaTable");

	std::string audioName = luaL_checkstring(lua, 2);
	int volume = luaL_checkinteger(lua, 3);

	if (volume < 0 || volume > 10) {
		std::cout << "volume is between 0 and 10" << std::endl;
		return 0;
	}

	(*pmPtr)->SetVolume(audioName.c_str(), ((float)volume) / 10);

	return 1;

}

#pragma endregion


bool LuaManager::ExecuteExample1()
{
	std::cout << "[=] Load LuaScripts... - Example1 " << std::endl;


	lua_State* L = this->Lua_Begin();


	//Lua파일이 유효한지 확인한후 로드합니다.
	if (CheckLua(L, luaL_dofile(L, "..\\Lua\\lt.lua")))
	{

	}

	this->Lua_End(L);
	
	return true;
}

bool LuaManager::ExecuteGUITest()
{
	lua_State* L = this->Lua_Begin();

	//Lua에 전역으로 C++ 함수를 등록합니다. 
	lua_register(L, "Begin", LuaManager::lua_ImGuiBegin);
	lua_register(L, "Text", LuaManager::lua_ImGuiText);
	lua_register(L, "End", LuaManager::lua_ImGuiEnd);

	if (CheckLua(L, luaL_dofile(L, "..\\Lua\\gui.lua")))
	{
		lua_getglobal(L,"render");

		if (lua_isfunction(L, -1))
		{
			lua_pushnumber(L, 5.0f);
			lua_pushnumber(L, 6.0f);

			if (CheckLua(L, lua_pcall(L, 2, 1, 0)))
			{
				std::cout << "5+6 =" << (float)lua_tonumber(L, -1) << std::endl << std::endl;
			}
		}
	}

	return true;
}

GameObjectMapper* LuaManager::GetGameObjectMapper()
{
	return this->gameMapper;
}