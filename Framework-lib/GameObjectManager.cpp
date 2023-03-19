#include "pch.h"
#include "GameObjectManager.h"


GameObject* GameObjectManager::CreateGameObject(const std::string& _name)
{
    if (this->FindGameObject(_name) != nullptr) {
        //std::cout << "object already exists" << std::endl;
        return nullptr; 
    }

    GameObject* obj = new GameObject(_name);


    this->gameObjects.insert(std::make_pair<>(_name, obj));

    //std::cout << "add success" << std::endl;

    return obj;
}

GameObject* GameObjectManager::FindGameObject(const std::string& _name)
{

    GameObject* obj = nullptr;

    if (this->gameObjects.find(_name) != this->gameObjects.end()) {
        obj = this->gameObjects[_name];
        //std::cout << "found success" << std::endl;
    }

   //std::cout << "found process end" << std::endl;

    return obj;
}

void GameObjectManager::DestroyGameObject(const std::string& _name)
{
    this->gameObjects.erase(_name);
    //std::cout << "delete process called" << std::endl;
}

std::string GameObjectManager::GetObjectNames()
{
    std::string name;

    for (auto& _g : gameObjects) {
        name = name + _g.first + "\n";
    }

    return name;
}
