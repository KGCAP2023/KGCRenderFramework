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

int LuaManager::lua_GameObjectManager_getGameObject(lua_State* lua)
{
	//Remember that first arbument should be userdata with your PersonManager 
	//instance, as in Lua you would call PersonManager:getPerson("Stack Overflower");
	//Normally I would first check, if first parameter is userdata with metatable 
	//called PersonManagerMetaTable, for safety reasons

	GameObjectManager** pmPtr = (GameObjectManager**)luaL_checkudata(
		lua, 1, "GameObjectManagerMetaTable");
	
	std::string personName = luaL_checkstring(lua, 2);

	GameObject* person = (*pmPtr)->CreateGameObject("hello");
	if (person == nullptr) person = (*pmPtr)->FindGameObject("hello");

	if (person)
		registerGameObject(lua, person);    //Function that registers person. After 
				//the function is called, the newly created instance of Person 
				//object is on top of the stack
	else
		lua_pushnil(lua);

	return 1;
}


bool LuaManager::ExecuteExample1()
{
	std::cout << "[=] Load LuaScripts... - Example1 " << std::endl;

	struct Player
	{
		std::string title;
		std::string name;
		std::string family;
		int level;
	} player;

	lua_State* L = this->Lua_Begin();

	//Lua에 전역으로 C++ 함수를 등록합니다. 
	lua_register(L, "HostFunction", LuaManager::lua_HostFunction);


	//Lua파일이 유효한지 확인한후 로드합니다.
	if (CheckLua(L, luaL_dofile(L, "..\\Lua\\EmbeddingLua1.lua")))
	{
		// Stage 1: Just read simple variables
		// -1 <- 제일 위에있는게 -1
		// -2
		// -3

		// lua_pop(L, 1); <- 이거는 1번 스택 pop한다.
		// lua_pcall(L, 3, 1, 0) <- 인자 3개 반환 1개  0은 디폴트
	
		std::cout << "[CPP] Stage 1 - Read Simple Variables" << std::endl;
		lua_getglobal(L, "a");
		if (lua_isnumber(L, -1)) std::cout << "[CPP S1] a = " << (int)lua_tointeger(L, -1) << std::endl;
		lua_getglobal(L, "b");
		if (lua_isnumber(L, -1)) std::cout << "[CPP S1] b = " << (int)lua_tointeger(L, -1) << std::endl;
		lua_getglobal(L, "c");
		if (lua_isnumber(L, -1)) std::cout << "[CPP S1] c = " << (int)lua_tointeger(L, -1) << std::endl;
		lua_getglobal(L, "d");
		if (lua_isstring(L, -1)) std::cout << "[CPP S1] d = " << lua_tostring(L, -1) << std::endl << std::endl;

		// Stage 2: Read Table Object
		std::cout << "[CPP] Stage 2 - Read Table (Key/Value pairs)" << std::endl;
		lua_getglobal(L, "player");
		if (lua_istable(L, -1))
		{
			lua_pushstring(L, "Name");
			lua_gettable(L, -2);
			player.name = lua_tostring(L, -1);
			lua_pop(L, 1);

			lua_pushstring(L, "Family");
			lua_gettable(L, -2);
			player.family = lua_tostring(L, -1);
			lua_pop(L, 1);

			lua_pushstring(L, "Title");
			lua_gettable(L, -2);
			player.title = lua_tostring(L, -1);
			lua_pop(L, 1);

			lua_pushstring(L, "Level");
			lua_gettable(L, -2);
			player.level = (int)lua_tointeger(L, -1);
			lua_pop(L, 1);
		}
		std::cout << "[CPP S2] " << player.title << " " << player.name << " of " << player.family << " [Lvl: " << player.level << "]" << std::endl << std::endl;

		// Stage 3: Call Lua Function
		std::cout << "[CPP] Stage 3 - Call Lua Function" << std::endl;
		lua_getglobal(L, "CalledFromCPP1");
		if (lua_isfunction(L, -1))
		{
			lua_pushnumber(L, 5.0f);
			lua_pushnumber(L, 6.0f);
			lua_pushstring(L, "Bwa ha ha!");
			std::cout << "[CPP S3] Calling 'CalledFromCPP1' in lua script" << std::endl;
			if (CheckLua(L, lua_pcall(L, 3, 1, 0)))
			{
				std::cout << "[CPP S3] 'CalledFromCPP1' returned " << (float)lua_tonumber(L, -1) << std::endl << std::endl;
			}
		}

		// Stage 4: Call Lua Function, which calls C++ Function
		std::cout << "[CPP] Stage 4 - Call Lua Function, whcih in turn calls C++ Function" << std::endl;
		lua_getglobal(L, "CalledFromCPP2");
		if (lua_isfunction(L, -1))
		{
			lua_pushnumber(L, 5.0f);
			lua_pushnumber(L, 6.0f);
			std::cout << "[CPP S4] Calling 'CalledFromCPP2' in lua script" << std::endl;
			if (CheckLua(L, lua_pcall(L, 2, 1, 0)))
			{
				std::cout << "[CPP S4] 'CalledFromCPP2' returned " << (float)lua_tonumber(L, -1) << std::endl << std::endl;
			}
		}
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
