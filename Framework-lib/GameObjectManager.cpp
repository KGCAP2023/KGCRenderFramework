#include "pch.h"
#include "GameObjectManager.h"
#include "Framework.h"


GameObjectManager::GameObjectManager(Framework* framework)
{
		this->framework = framework;
		this->graphicManager = &framework->graphics;
		this->res = &framework->resourceManager;
}


GameObject* GameObjectManager::CreateGameObject(const std::string& _name)
{
	//�̸� ���� �� ����� ���� �ź�

	if (this->FindGameObject(_name) != nullptr) {

		
		return nullptr;
	}

	//������Ʈ�� �����մϴ�.
	GameObject* obj = new GameObject(_name);
	//���ӿ�����Ʈ�� ����մϴ�.
	this->gameObjects.insert(std::make_pair<>(_name, obj));

	obj->AddComponent(new BoundingBox3D(obj, res));

	return obj;
}

GameObject* GameObjectManager::CreateGameObject(const std::string& _name, const std::string& modelName)
{

	//�̸� ���� �� ����� ���� �ź�
	if (this->FindGameObject(_name) != nullptr) {
		return nullptr;
	}

	//������Ʈ�� �����մϴ�.
	GameObject* obj = CreateGameObject(_name);
	//�� �������� �����մϴ�.
	ModelRenderer* render = new ModelRenderer(obj);
	Model* model = res->FindModel(modelName);
	render->SetModel(model);
	//�� �������� ����մϴ�.
	obj->AddComponent(render);
	//�ٿ�� �ڽ� �������� ����&����մϴ�.
	obj->AddComponent(new BoundingBox3D(obj, res));

	//���ӿ�����Ʈ�� ����մϴ�.
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

	std::cout << "������Ʈ�� �������� ����" << std::endl;

	return nullptr;
}
