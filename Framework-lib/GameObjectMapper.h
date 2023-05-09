#pragma once
#include "pch.h"
#include "GameObject.h"

class Framework;

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

	//Mapping Transform
	static void RegisterMappingTransform(lua_State* lua, Transform* obj);
    static int lua_Transform_MoveUp(lua_State* lua);
    static int lua_Transform_MoveDown(lua_State* lua);
    static int lua_Transform_MoveRight(lua_State* lua);
    static int lua_Transform_MoveLeft(lua_State* lua);

    static void dumpstack(lua_State* L) {
        int top = lua_gettop(L);
        for (int i = 1; i <= top; i++) {
            printf("%d\t%s\t", i, luaL_typename(L, i));
            switch (lua_type(L, i)) {
            case LUA_TNUMBER:
                printf("%g\n", lua_tonumber(L, i));
                break;
            case LUA_TSTRING:
                printf("%s\n", lua_tostring(L, i));
                break;
            case LUA_TBOOLEAN:
                printf("%s\n", (lua_toboolean(L, i) ? "true" : "false"));
                break;
            case LUA_TNIL:
                printf("%s\n", "nil");
                break;
            default:
                printf("%p\n", lua_topointer(L, i));
                break;
            }
        }
    }

private: 
	Framework* framework = nullptr;
};

