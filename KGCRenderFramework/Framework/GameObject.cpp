#include "GameObject.h"
using namespace DirectX;
//XMLoad XMVECTOR로 탑재
//XMSTORE XMFLOAT로 만듬

std::unordered_map<std::string, GameObject*> GameObject::gameObjects;

void GameObject::Draw(const XMMATRIX& viewProjectionMatrix)
{
	for (auto it : this->render)
	{
		it->Draw(viewProjectionMatrix);
	}
}

void GameObject::Update()
{
	for (auto& it : this->components)
	{
		it.second->Update();
	}
}

void GameObject::AddComponent(Component* pComponent)
{
	Component::Type type = pComponent->GetType();
	if (type == Component::Type::RENDERER_MODEL)
	{
		this->render.push_back(dynamic_cast<Renderer*>(pComponent));
		//this->render.push_back((Renderer*)pComponent);
	}
	components.insert(std::make_pair(type, pComponent));
}

Component* GameObject::GetComponent(const std::wstring componentID)
{

	return nullptr;
}

void GameObject::CleanUpComponent()
{
}

void GameObject::SetActive(bool bActive)
{
}
