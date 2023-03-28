#include "pch.h"
#include "GameObject.h"

using namespace DirectX;
//XMLoad XMVECTOR�� ž��
//XMSTORE XMFLOAT�� ����

void GameObject::Draw(const XMMATRIX& viewProjectionMatrix, GameObject::ObjectType objectType)
{
	if (this->objectType == objectType)
	{
		if (isActive)
		{
			for (auto it : this->components)
			{
				it.second->Draw(viewProjectionMatrix);
			}
		}

		if (bbox != nullptr && bbox->isActive())
			bbox->Draw(viewProjectionMatrix);

		if (!this->child.empty())
		{
			for (auto it : this->child)
			{
				it->Draw(viewProjectionMatrix, objectType);
			}
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

void GameObject::SetObjectType(GameObject::ObjectType type)
{
	this->objectType = type;
}

void GameObject::AddComponent(Component* pComponent)
{
	Component::Type type = pComponent->GetType();

	if (type == Component::Type::BOUNDING_BOX)
	{
		this->bbox = dynamic_cast<BoundingBoxRenderer*>(pComponent);
		return;
	}

	switch (type)
	{
		case Component::Type::RENDERER_MODEL:
		case Component::Type::RENDERER_SKINNED_MODEL:
			this->objectType = GameObject::ObjectType::OBJECT_3D;
			break;
		case Component::Type::RENDERER_SPRITE:
		case Component::Type::RENDERER_TILEMAP:
			this->objectType = GameObject::ObjectType::OBJECT_2D;
			break;
	}

	components.insert(std::make_pair(type, pComponent));
}

Component* GameObject::GetComponent(const std::string componentID)
{
	for (auto& a : components)
	{
		Component* c = a.second; 
		if (c->GetName().compare(componentID) == 0)
		{
			return c;
		}
	}
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
	for (auto& a : components)
	{
		Component* c = a.second;
		//delete c;
	}
	this->components.clear();
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
