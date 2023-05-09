#include "pch.h"
#include "GameObjectManager.h"
#include "Framework.h"


GameObjectManager::GameObjectManager(Framework* framework, SceneMode type)
{
		this->framework = framework;
		this->graphicManager = &framework->graphics;
		this->res = &framework->resourceManager;
		this->type = type;
}

SceneMode GameObjectManager::GetMode()
{
	return this->type;
}


GameObject* GameObjectManager::CreateGameObject(const std::string& _name)
{
	//이름이 없을 경우 생성 거부
	if (_name.size() == 0) {
		return nullptr;
	}

	//이름 같은 거 존재시 생성 거부
	if (this->FindGameObject(_name) != nullptr) {
		return nullptr;
	}

	//오브젝트를 생성합니다.
	GameObject* obj = new GameObject(_name);
	//게임오브젝트를 등록합니다.
	this->gameObjects.insert(std::make_pair<>(_name, obj));

	return obj;
}

GameObject* GameObjectManager::CreateGameObject(const std::string& _name, const std::string& modelName)
{
	//이름이 없을 경우 생성 거부
	if (_name.size() == 0) {
		return nullptr;
	}

	//이름 같은 거 존재시 생성 거부
	if (this->FindGameObject(_name) != nullptr) {
		return nullptr;
	}

	//오브젝트를 생성합니다.
	GameObject* obj = CreateGameObject(_name);
	//모델 렌더러를 생성합니다.
	ModelRenderer* render = new ModelRenderer(obj,this->res);
	Model* model = res->FindModel(modelName);
	render->SetModel(model);
	//모델 렌더러를 등록합니다.
	obj->AddComponent(render);

	//게임오브젝트를 등록합니다.
	this->gameObjects.insert(std::make_pair<>(_name, obj));

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

GameObject* GameObjectManager::GetFocusedObject()
{

	if (gameObjects.size() > 0) {

		return GameObject::GetFocusedObject();
	}

	std::cout << "오브젝트가 존재하지 않음" << std::endl;

	return nullptr;
}

void GameObjectManager::AddFocusedObjectListener(std::function<void(GameObject*)> callback)
{
	_focusedObjectCallback.push_back(callback);
}

void GameObjectManager::notifyFousedObject(GameObject* object)
{
	for (auto func : _focusedObjectCallback)
	{
		func(object);
	}
}

std::unordered_map<std::string, GameObject*> GameObjectManager::GetObejctMap()
{
	return this->gameObjects;
}
