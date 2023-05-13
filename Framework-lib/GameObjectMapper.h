#pragma once
#include "pch.h"
#include "GameObject.h"

class Framework;
class ResourceManager;

class GameObjectMapper
{
public:
	GameObjectMapper(Framework* framework);

	//Mapping GameObjectManager
	void RegisterMappingGameObjectManager(lua_State* lua);
	static int lua_GameObjectManager_GetGameObject(lua_State* lua);

	//Mapping GameObject
	static void RegisterMappingGameObject(lua_State* lua, GameObject* obj);
	static int lua_GameObject_GetName(lua_State* lua);
    static int lua_GameObject_GetTransform(lua_State* lua);
	static int lua_GameObject_LoadAnimation(lua_State* lua);
	static int lua_GameObject_PlayAnim2D(lua_State* lua);

	//Mapping Transform
	static void RegisterMappingTransform(lua_State* lua, Transform* obj);
    static int lua_Transform_MoveUp(lua_State* lua);
    static int lua_Transform_MoveDown(lua_State* lua);
    static int lua_Transform_MoveRight(lua_State* lua);
    static int lua_Transform_MoveLeft(lua_State* lua);


private: 
	static ResourceManager* res;
	Framework* framework = nullptr;
};

