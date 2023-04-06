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
				it.second->Draw(viewProjectionMatrix);
			}
		}

		if (bbox != nullptr && bbox->isActive())
			bbox->Draw(viewProjectionMatrix);

		if (!this->child.empty())
		{
			for (auto it : this->child)
			{
				it->Draw(viewProjectionMatrix);
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

	if (bbox != nullptr)
		bbox->Update();

	if (!this->child.empty())
	{
		for (auto it : this->child)
		{
			it->Update();
		}
	}

	//행렬 업데이트
	transform.worldMatrix = DirectX::XMMatrixScaling(transform.scale.x, transform.scale.y, transform.scale.z) * XMMatrixRotationRollPitchYaw(transform.rotation.x, transform.rotation.y, transform.rotation.z) * XMMatrixTranslation(transform.position.x, transform.position.y, transform.position.z);
	transform.UpdateDirection();
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
	if (this->bbox != nullptr) {

		this->game->ReleaseFocus();
		this->bbox->ChangeColor(255, 0, 0);
		this->game = this;
		
	}
	else {
		assert("bounding box가 존재하지 않는 객체입니다.");
	}
}

void GameObject::ReleaseFocus()
{
	if (this->bbox != nullptr && this->game == this) {
		this->bbox->ChangeColor(0, 0, 0);
		this->game = nullptr;
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