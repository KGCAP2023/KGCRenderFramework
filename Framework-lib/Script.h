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

	//�Ҹ��� 
	~Script()
	{
		this->clear();
	}

	//��� ���ϰ�� ����
	void SetLuaFilePath(std::string filepath)
	{
		this->filepath = filepath;
		
	}

	bool LoadScript()
	{
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

		this->lua->GetGameObjectMapper()->RegisterMappingGameObjectManager(L);
		//etc....
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

