#include "pch.h"
#include "GameObjectMapper.h"
#include "Framework.h"
#include "SkinnedMeshRenderer.h"
#include "SpriteRenderer.h"

ResourceManager* GameObjectMapper::res = nullptr;

GameObjectMapper::GameObjectMapper(Framework* framework)
{
    this->framework = framework;
    this->res = &framework->resourceManager;
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
           "LoadAnimPreset",lua_GameObject_LoadAnimation,
           "PlayAnim2D",lua_GameObject_PlayAnim2D,
           "AddAnim2D",lua_GameObject_AddAnimation2D,
           "IsActive",lua_GameObject_IsActive,
           "SetActive",lua_GameObject_SetActive,
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

int GameObjectMapper::lua_GameObject_LoadAnimation(lua_State* lua)
{
    GameObject** pptr = (GameObject**)luaL_checkudata(lua, 1, "GameObjectMetaTable");
    GameObject* obj = (*pptr);
   
    std::string animationPreset = luaL_checkstring(lua, 2);

    if (SpriteRenderer* render = dynamic_cast<SpriteRenderer*>(obj->GetComponent(Component::Type::RENDERER_SPRITE)))
    {
        Sprite* sp = render->GetSprite();

        if (sp != nullptr && sp->GetName().compare(animationPreset) == 0)
        {
            render->AddAnimation2D("run", 0, 0, 172, 188, 5, 80);
            render->AddAnimation2D("idle", 0, 0, 172, 188, 1, 20);
        }
    }
    else if (SkinnedMeshRenderer* render = dynamic_cast<SkinnedMeshRenderer*>(obj->GetComponent(Component::Type::RENDERER_SPRITE)))
    {
        std::string name = render->GetName();
        if (name.compare(animationPreset) == 0)
        {
            render->AddAnimation("..\\Resource\\Objects\\walking\\Idle.fbx");
            render->AddAnimation("..\\Resource\\Objects\\walking\\walk.fbx");
        }
    }

    return 0;
}

int GameObjectMapper::lua_GameObject_AddAnimation2D(lua_State* lua)
{
    GameObject** pptr = (GameObject**)luaL_checkudata(lua, 1, "GameObjectMetaTable");
    GameObject* obj = (*pptr);

    std::string animationName = luaL_checkstring(lua, 2);
    int x = luaL_checknumber(lua, 3);
    int y = luaL_checknumber(lua, 4);
    int width = luaL_checknumber(lua, 5);
    int height = luaL_checknumber(lua, 6);
    int count = luaL_checknumber(lua, 7);
    float holdtime = luaL_checknumber(lua, 8);

    if (SpriteRenderer* render = dynamic_cast<SpriteRenderer*>(obj->GetComponent(Component::Type::RENDERER_SPRITE)))
    {
        Sprite* sp = render->GetSprite();

        if (sp != nullptr)
        {
            render->AddAnimation2D(animationName, x, y, width, height, count, holdtime);
        }
    }
    return 0;
}

int GameObjectMapper::lua_GameObject_PlayAnim2D(lua_State* lua)
{
    GameObject** pptr = (GameObject**)luaL_checkudata(lua, 1, "GameObjectMetaTable");
    GameObject* obj = (*pptr);

    std::string animationName = luaL_checkstring(lua, 2);
    int flip = luaL_checkinteger(lua, 3);

    SpriteRenderer* render = dynamic_cast<SpriteRenderer*>(obj->GetComponent(Component::Type::RENDERER_SPRITE));
    if (render != nullptr)
    {
        if (flip)
            render->SetImageFlip(true);
        else
            render->SetImageFlip(false);
        render->SelectAnimation(animationName);
    }
    return 0;
}

int GameObjectMapper::lua_GameObject_IsActive(lua_State* lua)
{
    GameObject** pptr = (GameObject**)luaL_checkudata(lua, 1, "GameObjectMetaTable");
    GameObject* obj = (*pptr);
    if (obj->IsActive())
        lua_pushboolean(lua, true);
    else
        lua_pushboolean(lua, false);
    return 1;
}

int GameObjectMapper::lua_GameObject_SetActive(lua_State* lua)
{
    GameObject** pptr = (GameObject**)luaL_checkudata(lua, 1, "GameObjectMetaTable");
    GameObject* obj = (*pptr);

    bool value = lua_toboolean(lua, 2);
    obj->SetActive(value);
    return 0;
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
       "Translate",lua_Transform_Translate,
       "SetPosition",lua_Transform_SetPosition,
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

int GameObjectMapper::lua_Transform_Translate(lua_State* lua)
{
    Transform** pptr = (Transform**)luaL_checkudata(lua, 1, "TransformMetaTable");
    float x = luaL_checknumber(lua, 2);
    float y = luaL_checknumber(lua, 3);
    float z = luaL_checknumber(lua, 4);
    (*pptr)->Translate(x, y, z);
    return 0;
}

int GameObjectMapper::lua_Transform_SetPosition(lua_State* lua)
{
    Transform** pptr = (Transform**)luaL_checkudata(lua, 1, "TransformMetaTable");
    float x = luaL_checknumber(lua, 2);
    float y = luaL_checknumber(lua, 3);
    float z = luaL_checknumber(lua, 4);
    (*pptr)->SetPosition(x, y, z);
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


