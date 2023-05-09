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

	//소멸자 
	~Script()
	{
		this->clear();
	}

	//루아 파일경로 설정
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

	//업데이트문
	virtual void Update()
	{

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

		this->lua->GetGameObjectMapper()->RegisterMappingGameObjectManager(L);
		//etc....
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

};

