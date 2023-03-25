#include "pch.h"
#include "GameObjectManager.h"
#include "Framework.h"


GameObject* GameObjectManager::CreateGameObject(const std::string& _name, const std::string& path)
{

	//���ҽ��Ŵ������� ���̴��� �����ɴϴ�.
	PixelShader* ps_1 = res->FindPixelShader("ps_1");
	VertexShader* vs_1 = res->FindVertexShader("vs_1");

	PixelShader* ps_2 = res->FindPixelShader("ps_2");
	VertexShader* vs_2 = res->FindVertexShader("vs_2");
	Sprite* sp = res->FindSprite("ani");

	//������Ʈ�� �����մϴ�.
	GameObject* obj = new GameObject(_name);

	//�� �������� �����մϴ�.
	ModelRenderer* render1 = new ModelRenderer(obj);
	render1->Init(path, graphicManager->device.Get(), graphicManager->deviceContext.Get(), graphicManager->res->cb2, vs_2, ps_2);

	//�� �������� ����մϴ�.
	obj->AddComponent(render1);

	//�ٿ�� �ڽ� �������� ����&����մϴ�.
	obj->AddComponent(new BoundingBox3D(obj, graphicManager->device.Get(), graphicManager->deviceContext.Get(), ps_1, vs_1, res->cb1));

	//SpriteRenderer* render2 = new SpriteRenderer(obj);
	//render2->AddSprite(sp);
	//render2->AddAnimation2D("motion1", 200, 200, 100, 100, 4, 1000.f, DirectX::Colors::Magenta);
	//render2->SelectAnimation("motion1");
	//obj->AddComponent(render2);

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
