#pragma once
#include "pch.h"
#include "GameObject.h"

class GameObjectManager {

public:

	GameObject* CreateGameObject(const std::string& _name);

	GameObject* FindGameObject(const std::string& _name);

	void DestroyGameObject(const std::string& _name);
	

private:
	std::unordered_map<std::string, GameObject*> gameObjects;

};