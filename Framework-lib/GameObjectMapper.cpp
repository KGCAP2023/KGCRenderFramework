#include "pch.h"
#include "GameObjectMapper.h"
#include "Framework.h"

GameObjectMapper::GameObjectMapper(Framework* framework)
{
    this->framework = framework;
}

void GameObjectMapper::RegisterMappingGameObjectManager(lua_State* lua)
{
    GameObjectManager* objManager = framework->GetGameObjectManagerInstance();
    GameObjectManager** pmPtr = (GameObjectManager**)lua_newuserdata(lua, sizeof(GameObjectManager*));
    *pmPtr = objManager;

    luaL_newmetatable(lua, "GameObjectManagerMetaTable");

    lua_pushvalue(lua, -1);
    lua_setfield(lua, -2, "__index");

    luaL_Reg personManagerFunctions[] = {
       "GetObject", lua_GameObjectManager_GetGameObject,
        nullptr, nullptr
    };

    luaL_setfuncs(lua, personManagerFunctions, 0);

    lua_setmetatable(lua, -2);
    lua_setglobal(lua, "GameObjectManager");
}

void GameObjectMapper::RegisterMappingGameObject(lua_State* lua, GameObject* obj)
{
    GameObject** pptr = (GameObject**)lua_newuserdata(lua, sizeof(GameObject*));
    *pptr = obj;

    if (luaL_newmetatable(lua, "GameObjectMetaTable"))
    {

        lua_pushvalue(lua, -1);
        lua_setfield(lua, -2, "__index");

        luaL_Reg GameObjectFunctions[] = {
           "GetName", lua_GameObject_GetName,
           "GetTransform",lua_GameObject_GetTransform,
           nullptr, nullptr
        };

        luaL_setfuncs(lua, GameObjectFunctions, 0);
    }

    lua_setmetatable(lua, -2);

}

int GameObjectMapper::lua_GameObject_GetName(lua_State* lua)
{
    GameObject** pptr = (GameObject**)luaL_checkudata(lua, 1, "GameObjectMetaTable");
    lua_pushstring(lua, (*pptr)->GetName().c_str());
    return 1;
}

int GameObjectMapper::lua_GameObject_GetTransform(lua_State* lua)
{
    GameObject** pptr = (GameObject**)luaL_checkudata(lua, 1, "GameObjectMetaTable");
    Transform* trans = &((*pptr)->transform);
    RegisterMappingTransform(lua, trans);
    return 1;
}

void GameObjectMapper::RegisterMappingTransform(lua_State* lua, Transform* obj)
{
    Transform** pptr = (Transform**)lua_newuserdata(lua, sizeof(Transform*));
    *pptr = obj;

    luaL_newmetatable(lua, "TransformMetaTable");

    lua_pushvalue(lua, -1);
    lua_setfield(lua, -2, "__index");

    luaL_Reg TransformFunctions[] = {
       "MoveUp", lua_Transform_MoveUp,
       "MoveDown",lua_Transform_MoveDown,
       "MoveRight",lua_Transform_MoveRight,
       "MoveLeft",lua_Transform_MoveLeft,
       nullptr, nullptr
    };

    luaL_setfuncs(lua, TransformFunctions, 0);


    lua_setmetatable(lua, -2);
}

int GameObjectMapper::lua_Transform_MoveUp(lua_State* lua)
{
    float speed = 0.1f;
    float dt = Framework::getDeltaTime();
    Transform** pptr = (Transform**)luaL_checkudata(lua, 1, "TransformMetaTable");
    (*pptr)->Translate(0, -speed * dt, 0);
    return 0;
}

int GameObjectMapper::lua_Transform_MoveDown(lua_State* lua)
{
    float speed = 0.1f;
    float dt = Framework::getDeltaTime();
    Transform** pptr = (Transform**)luaL_checkudata(lua, 1, "TransformMetaTable");
    (*pptr)->Translate(0, speed * dt, 0);
    return 0;
}

int GameObjectMapper::lua_Transform_MoveRight(lua_State* lua)
{
    float speed = 0.1f;
    float dt = Framework::getDeltaTime();
    Transform** pptr = (Transform**)luaL_checkudata(lua, 1, "TransformMetaTable");
    (*pptr)->Translate(speed * dt, 0, 0);
    return 0;
}

int GameObjectMapper::lua_Transform_MoveLeft(lua_State* lua)
{
    float speed = 0.1f;
    float dt = Framework::getDeltaTime();
    Transform** pptr = (Transform**)luaL_checkudata(lua, 1, "TransformMetaTable");
    (*pptr)->Translate(-speed * dt, 0, 0);
    return 0;
}

int GameObjectMapper::lua_GameObjectManager_GetGameObject(lua_State* lua)
{
    GameObjectManager** pmPtr = (GameObjectManager**)luaL_checkudata(
        lua, 1, "GameObjectManagerMetaTable");

    std::string objName = luaL_checkstring(lua, 2);

    GameObject* obj = (*pmPtr)->FindGameObject(objName);
    if (obj == nullptr) obj = (*pmPtr)->CreateGameObject(objName);

    if (obj)
        RegisterMappingGameObject(lua, obj);
    else
        lua_pushnil(lua);

    return 1;
}


