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
		this->objManager = framework->gameObjManager;
	}
	return true;
}

lua_State* LuaManager::Lua_Begin()
{
	//lua_State ����
	lua_State* L = luaL_newstate();

	//ǥ�� ���̺귯���� ��� �����ӽſ� �߰�
	luaL_openlibs(L);

	return L;
}

void LuaManager::Lua_End(lua_State* L)
{
	lua_close(L);
}

void LuaManager::registerGameObjectManager(lua_State* lua)
{
	//���� lua ���������̺� GameObjectManager �߰�
	GameObjectManager** pmPtr = (GameObjectManager**)lua_newuserdata(
		lua, sizeof(GameObjectManager*));
	*pmPtr = this->objManager;

	//gameobjectManager ��Ÿ���̺� �߰� -> ������ ���� ���� ����
	luaL_newmetatable(lua, "GameObjectManagerMetaTable");

	//���� ���� ������ �����ϰ� ���� �ϳ��� index�� ����
	lua_pushvalue(lua, -1);
	lua_setfield(lua, -2, "__index");

	//�ش� ��Ÿ���̺� ���ǵ� �Լ��� ����
	luaL_Reg gameObjectManagerFunctions[] = {
		"GetObject", lua_GameObjectManager_getGameObject,
		nullptr, nullptr
	};

	//���ε� ����ü ����
	luaL_setfuncs(lua, gameObjectManagerFunctions, 0);

	//�ε���ȭ ��Ų�� ��Ÿ���̺�ȭ
	lua_setmetatable(lua, -2);

	//�Լ�ȣ�� �� �ۼ��� �̸� ���ÿ� ���� ex) GameObjectManager:GetObject("name")
	lua_setglobal(lua, "GameObjectManager");


	AudioManager* audioManager = &framework->audioManager;
	AudioManager** amPtr = (AudioManager**)lua_newuserdata(lua, sizeof(AudioManager*));
	*amPtr = audioManager;

	luaL_newmetatable(lua, "AudioManagerMetaTable");

	lua_pushvalue(lua, -1);
	lua_setfield(lua, -2, "__index");

	luaL_Reg audioManagerFunctions[] = {
		"LoadAudio",lua_LoadAudio,//������ �̸� (str), ���(str)
		"PlayAudio",lua_PlayAudio,//����� �̸�(str)
		"StopAudio",lua_StopAudio,//����� �̸�(str)
		"DeleteAudio",lua_DeleteAudio,//����� �̸�(str)
		"SetAudioVolume",lua_SetAudioVolume,//����� �̸�(str), ���� ����(int [0~10])
		"PauseAudio",lua_PauseAudio,//����� �̸�(str)
		"ResumeAudio",lua_ResumeAudio,//����� �̸�(str)
		nullptr, nullptr
	};
	luaL_setfuncs(lua, audioManagerFunctions, 0);

	lua_setmetatable(lua, -2);

	lua_setglobal(lua, "AudioManager");

	// ���� => AudioManager:PlayAudio("audioName")
}


int LuaManager::lua_GameObjectManager_getGameObject(lua_State* lua)
{
	//Remember that first arbument should be userdata with your PersonManager 
	//instance, as in Lua you would call PersonManager:getPerson("Stack Overflower");
	//Normally I would first check, if first parameter is userdata with metatable 
	//called PersonManagerMetaTable, for safety reasons

	GameObjectManager** pmPtr = (GameObjectManager**)luaL_checkudata(
		lua, 1, "GameObjectManagerMetaTable");
	
	std::string personName = luaL_checkstring(lua, 2);

	GameObject* person = (*pmPtr)->FindGameObject(personName);
	if (person == nullptr) person = (*pmPtr)->CreateGameObject(personName);

	if (person)
		registerGameObject(lua, person);    //Function that registers person. After 
				//the function is called, the newly created instance of Person 
				//object is on top of the stack
	else
		lua_pushnil(lua);

	return 1;
}


#pragma region ������޴���_����_����_�Լ���

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
	LuaManager::registerGameObjectManager(L);


	//Lua������ ��ȿ���� Ȯ������ �ε��մϴ�.
	if (CheckLua(L, luaL_dofile(L, "..\\Lua\\lt.lua")))
	{

	}

	this->Lua_End(L);
	
	return true;
}

bool LuaManager::ExecuteGUITest()
{
	lua_State* L = this->Lua_Begin();

	//Lua�� �������� C++ �Լ��� ����մϴ�. 
	lua_register(L, "Begin", LuaManager::lua_ImGuiBegin);
	lua_register(L, "Text", LuaManager::lua_ImGuiText);
	lua_register(L, "End", LuaManager::lua_ImGuiEnd);

	LuaManager::registerGameObjectManager(L);

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
