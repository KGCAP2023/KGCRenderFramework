#pragma once
#include "Gameobject.h"
#include "LuaManager.h"

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

	//업데이트문
	virtual void Update()
	{
		//루아 파일 실행 
		if (this->lua->CheckLua(L, luaL_dofile(L, filepath.c_str())))
		{
			//내부 로직 구현.... (pcall)




		}
	}

	virtual void Draw(const DirectX::XMMATRIX& viewProjectionMatrix) {}

private:

	//매핑 초기화
	void registerMappingFuncs(lua_State* L)
	{
		//Lua에 전역으로 C++ 함수를 등록합니다. 
		//lua_register(L, "Begin", LuaManager::lua_ImGuiBegin);
		//lua_register(L, "Text", LuaManager::lua_ImGuiText);
		//lua_register(L, "End", LuaManager::lua_ImGuiEnd);
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
	}

	lua_State* L = nullptr;
	std::string filepath; // = "..\\Lua\\gui.lua";
	LuaManager* lua;
};

