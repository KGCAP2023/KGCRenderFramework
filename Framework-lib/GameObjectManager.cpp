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

	//리소스매니저에서 쉐이더를 가져옵니다.
	PixelShader* ps_1 = res->FindPixelShader("ps_1");
	VertexShader* vs_1 = res->FindVertexShader("vs_1");


	//오브젝트를 생성합니다.
	GameObject* obj = new GameObject(_name);


	//바운딩 박스 렌더러를 생성&등록합니다.
	obj->AddComponent(new BoundingBox3D(obj, graphicManager->device.Get(), graphicManager->deviceContext.Get(), ps_1, vs_1, res->cb1));


	//게임오브젝트를 등록합니다.
	this->gameObjects.insert(std::make_pair<>(_name, obj));

	return obj;
}

GameObject* GameObjectManager::CreateGameObject(const std::string& _name, const std::string& path)
{

	//리소스매니저에서 쉐이더를 가져옵니다.
	PixelShader* ps_1 = res->FindPixelShader("ps_1");
	VertexShader* vs_1 = res->FindVertexShader("vs_1");

	PixelShader* ps_2 = res->FindPixelShader("ps_2");
	VertexShader* vs_2 = res->FindVertexShader("vs_2");


	//오브젝트를 생성합니다.
	GameObject* obj = new GameObject(_name);


	//모델 렌더러를 생성합니다.
	ModelRenderer* render1 = new ModelRenderer(obj);
	render1->Init(path, graphicManager->device.Get(), graphicManager->deviceContext.Get(), graphicManager->res->cb2, vs_2, ps_2);


	//모델 렌더러를 등록합니다.
	obj->AddComponent(render1);


	//바운딩 박스 렌더러를 생성&등록합니다.
	obj->AddComponent(new BoundingBox3D(obj, graphicManager->device.Get(), graphicManager->deviceContext.Get(), ps_1, vs_1, res->cb1));


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
