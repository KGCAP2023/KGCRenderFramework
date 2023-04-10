#include "pch.h"
#include "GameObject.h"

using namespace DirectX;
//XMLoad XMVECTOR로 탑재
//XMSTORE XMFLOAT로 만듬

void GameObject::Draw(const XMMATRIX& viewProjectionMatrix)
{

		if (isActive)
		{
			for (auto it : this->components)
			{
				if (it.second->IsSprite()) continue;
				it.second->Draw(viewProjectionMatrix);
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

void GameObject::DrawSprite(const XMMATRIX& viewProjectionMatrix)
{
	if (isActive)
	{
		for (auto it : this->components)
		{
			if (!it.second->IsSprite()) continue;
			it.second->Draw(viewProjectionMatrix);
		}
	}

	if (!this->child.empty())
	{
		for (auto it : this->child)
		{
			it->DrawSprite(viewProjectionMatrix);
		}
	}
}

GameObject* GameObject::GetFocusedObject()
{
	return game;
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

	//행렬 업데이트
	this->transform.worldMatrix = DirectX::XMMatrixScaling(transform.scale.x, transform.scale.y, transform.scale.z) * XMMatrixRotationRollPitchYaw(transform.rotation.x, transform.rotation.y, transform.rotation.z) * XMMatrixTranslation(transform.position.x, transform.position.y, transform.position.z);
	this->transform.UpdateDirection();
}

void GameObject::AddComponent(Component* pComponent)
{
	Component::Type type = pComponent->GetType();

	if (type == Component::Type::BOUNDING_BOX)
	{
		return;
	}

	components.insert(std::make_pair(type, pComponent));
}

void GameObject::RemoveComponent(const Component::Type componentID)
{
	//Component* c = this->GetComponent(componentID);
	//if (c != nullptr)
	//	delete c;
	components.erase(componentID);
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

std::unordered_map<Component::Type, Component*, Component::ComponentHash> GameObject::GetComponentMap()
{
	return this->components;
}

int GameObject::GetComponentSize()
{
	return components.size();
}

void GameObject::ComponentForeach(std::function<void(Component*)> callback)
{
	for (auto& a : components)
	{
		callback(a.second);
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

void GameObject::SetFocus()
{
	this->game->ReleaseFocus();
	if (this->bbox != nullptr)
	{
		this->bbox->ChangeColor(255, 0, 0);
		this->game = this;
	}
}

void GameObject::ReleaseFocus()
{
	if (GameObject::game == nullptr) 
		return;
	else
	{
		GameObject::game->bbox->ChangeColor(0, 0, 0);
		GameObject::game = nullptr;
	}
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

GameObject* GameObject::game = nullptr;