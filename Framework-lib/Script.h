#pragma once
#include "Gameobject.h"
#include "LuaManager.h"
#include "InputMapper.h"

class framework;

//GrahpicManager.cpp 616번줄 참고
//Script 생성자 init() 안에 Lua_Begin();
//Script 소멸자 clear() 안에 Lua_End();

class Script : public Component
{
public:
	//생성자
	Script(GameObject* owner, Framework* framework);

	Script(const Script& rhs, GameObject* owner) : Component(owner)
	{
		std::cout << "[=] Script CLONE Process - Copy constructor called" << std::endl;
		this->type = Component::Type::SCRIPT;
		this->name = "Script";
		L = nullptr;
		filepath = rhs.filepath;
		lua = rhs.lua;
	}

	virtual Component* Copy(GameObject* owner)
	{
		Component* compo = new Script(*this, owner);
		return compo;
	};

	//소멸자 
	~Script()
	{
		std::cout << "[=] Script destructor called" << std::endl;
		if(this->isLoaded) this->clear();
	}

	//루아 파일경로 설정
	void SetLuaFilePath(std::string filepath)
	{
		this->filepath = filepath;
		
	}

	bool LoadScript()
	{
		this->init();

		if (this->lua->CheckLua(L, luaL_dofile(L, filepath.c_str())))
		{
			lua_getglobal(L, "Start");
			if (lua_isfunction(L, -1))
			{
				this->lua->CheckLua(L, lua_pcall(L, 0, 0, 0));
			}
			isLoaded = true;
			return true;
		}
		else
		{
			isLoaded = false;
			return false;
		}
			
	}

	//업데이트문
	virtual void Update()
	{
		if (isLoaded == false) return;

		//루아 파일 실행 
		if (L != nullptr)
		{
			lua_getglobal(L, "Update");
			if (lua_isfunction(L, -1))
			{
				this->lua->CheckLua(L, lua_pcall(L, 0, 0, 0));
			}

		}

	}

	virtual void Draw(const DirectX::XMMATRIX& viewProjectionMatrix) {}

private:

	//매핑 초기화
	void registerMappingFuncs(lua_State* L)
	{
		//Lua에 전역으로 C++ 함수를 등록합니다. 
		lua_register(L, "KeyInput_W", InputMapper::KeyInput_W);
		lua_register(L, "KeyInput_A", InputMapper::KeyInput_A);
		lua_register(L, "KeyInput_S", InputMapper::KeyInput_S);
		lua_register(L, "KeyInput_D", InputMapper::KeyInput_D);
		lua_register(L, "GetDeltaTime", LuaManager::lua_GetDeltaTime);

		this->RegisterMappingScript(L);
		this->lua->GetGameObjectMapper()->RegisterMappingGameObjectManager(L);
		this->lua->registerAudioManager(L);
		this->lua->registerCamera(L);

		//etc....
	}

	void RegisterMappingScript(lua_State* lua)
	{
		Script* objManager = this;
		Script** pmPtr = (Script**)lua_newuserdata(lua, sizeof(Script*));
		*pmPtr = objManager;

		luaL_newmetatable(lua, "ScriptMetaTable");

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -2, "__index");

		luaL_Reg personManagerFunctions[] = {
		   "GetThisObject", lua_Script_GetThisObject,
			nullptr, nullptr
		};

		luaL_setfuncs(lua, personManagerFunctions, 0);

		lua_setmetatable(lua, -2);
		lua_setglobal(lua, "Script");
	}
	static int lua_Script_GetThisObject(lua_State* lua)
	{
		Script** pptr = (Script**)luaL_checkudata(lua, 1, "ScriptMetaTable");
		GameObject* obj = (*pptr)->GetOwner();

		if (obj)
			GameObjectMapper::RegisterMappingGameObject(lua, obj);
		else
			lua_pushnil(lua);
		return 1;
	}

	//초기화
	void init()
	{
		this->L = this->lua->Lua_Begin();
		this->registerMappingFuncs(L);
	}

	void clear()
	{
		this->lua->Lua_End(L);
		L = nullptr;
	}

	lua_State* L = nullptr;
	std::string filepath; // = "..\\Lua\\gui.lua";
	LuaManager* lua;
	bool isLoaded = false;
};

