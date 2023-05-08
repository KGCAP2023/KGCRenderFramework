#pragma once
#include "Gameobject.h"
#include "LuaManager.h"

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
		this->lua->CheckLua(L, luaL_dofile(L, filepath.c_str()));
		return true;
	}

	//������Ʈ��
	virtual void Update()
	{
		//��� ���� ���� 
		if (L != nullptr)
		{
			//���� ���� ����.... (pcall)
			lua_getglobal(L, "Update");

			if (lua_isfunction(L, -1))
			{
				lua_pushnumber(L, 5.0f);
				lua_pushnumber(L, 6.0f);

				if (this->lua->CheckLua(L, lua_pcall(L, 2, 1, 0)))
				{
					//std::cout << "5+6 =" << (float)lua_tonumber(L, -1) << std::endl << std::endl;
				}
			}

		}
	}

	virtual void Draw(const DirectX::XMMATRIX& viewProjectionMatrix) {}

private:

	//���� �ʱ�ȭ
	void registerMappingFuncs(lua_State* L)
	{
		//Lua�� �������� C++ �Լ��� ����մϴ�. 
		//lua_register(L, "Begin", LuaManager::lua_ImGuiBegin);
		//lua_register(L, "Text", LuaManager::lua_ImGuiText);
		//lua_register(L, "End", LuaManager::lua_ImGuiEnd);
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

