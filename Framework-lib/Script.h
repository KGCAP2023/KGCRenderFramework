#pragma once
#include "Gameobject.h"
#include "LuaManager.h"
#include "InputMapper.h"

class framework;

//GrahpicManager.cpp 616���� ����
//Script ������ init() �ȿ� Lua_Begin();
//Script �Ҹ��� clear() �ȿ� Lua_End();

class Script : public Component
{
public:
	//������
	Script(GameObject* owner, Framework* framework);

	Script(const Script& rhs, GameObject* owner) : Component(owner)
	{
		std::cout << "[=] Script CLONE Process - Copy constructor called" << std::endl;
		L = nullptr;
		filepath = rhs.filepath;
		lua = rhs.lua;
	}

	virtual Component* Copy(GameObject* owner)
	{
		Component* compo = new Script(*this, owner);
		return compo;
	};

	//�Ҹ��� 
	~Script()
	{
		std::cout << "[=] Script destructor called" << std::endl;
		this->clear();
	}

	//��� ���ϰ�� ����
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
			return true;
		}
		else
			return false;
	}

	//������Ʈ��
	virtual void Update()
	{

		//��� ���� ���� 
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

	//���� �ʱ�ȭ
	void registerMappingFuncs(lua_State* L)
	{
		//Lua�� �������� C++ �Լ��� ����մϴ�. 
		lua_register(L, "KeyInput_W", InputMapper::KeyInput_W);
		lua_register(L, "KeyInput_A", InputMapper::KeyInput_A);
		lua_register(L, "KeyInput_S", InputMapper::KeyInput_S);
		lua_register(L, "KeyInput_D", InputMapper::KeyInput_D);

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

	//�ʱ�ȭ
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

};

