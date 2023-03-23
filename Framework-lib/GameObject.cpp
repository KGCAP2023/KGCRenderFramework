#include "pch.h"
#include "GameObject.h"

using namespace DirectX;
//XMLoad XMVECTOR�� ž��
//XMSTORE XMFLOAT�� ����

void GameObject::Draw(const XMMATRIX& viewProjectionMatrix)
{
	if (isActive)
	{
		for (auto it : this->render)
		{
			it->Draw(viewProjectionMatrix);
		}
	}

	if (!this->child.empty())
	{
		for (auto it : this->child)
		{
			it->Draw(viewProjectionMatrix);
		}
	}
}

void GameObject::Update()
{
	for (auto& it : this->components)
	{
		it.second->Update();
	}

	if (!this->child.empty())
	{
		for (auto it : this->child)
		{
			it->Update();
		}
	}
}

void GameObject::AddComponent(Component* pComponent)
{
	Component::Type type = pComponent->GetType();
	if (type == Component::Type::RENDERER_MODEL || type == Component::Type::BOUNDING_BOX)
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

Component* GameObject::GetComponent(const Component::Type componentID)
{
	auto item = components.find(componentID);

	if (item != components.end())
	{
		return item->second;
	}
	else
	{
		return nullptr;
	}
}

void GameObject::CleanUpComponent()
{
}

void GameObject::SetActive(bool bActive)
{
	this->isActive = bActive;
}

void GameObject::SetParent(GameObject* pParent)
{
	this->parent = pParent;
}

void GameObject::AddChild(GameObject* pChild)
{
	pChild->transform.Rotate(this->transform.rotationVector);
	pChild->transform.Translate(this->transform.positionVector);
	this->child.push_back(pChild);
}
