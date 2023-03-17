#include "pch.h"
#include "GameObjectManager.h"

GameObject* GameObjectManager::CreateGameObject(const std::string& _name)
{
    if (this->FindGameObject(_name) != nullptr) {
        return nullptr; //실패(중복)
    }

    //임시
    GameObject* obj = new GameObject(_name);

    this->gameObjects.insert(std::make_pair<>(_name, obj));

    return obj;
}

GameObject* GameObjectManager::FindGameObject(const std::string& _name)
{

    GameObject* obj = nullptr;

    if (this->gameObjects.find(_name) != this->gameObjects.end()) {
        obj = this->gameObjects[_name];
    }

    return obj;
}

void GameObjectManager::DestroyGameObject(const std::string& _name)
{
    this->gameObjects.erase(_name);
}
