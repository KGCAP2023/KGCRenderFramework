#pragma once
#include "Component.h"
#include "Transform.h"
#include "BoundingBox.h"
using namespace DirectX;
//Ŭ���� ������  XMFLOAT�� ����
//�������� 16����Ʈ ������ ���� ��ٷο�

class GameObject
{
private:
	std::unordered_map<Component::Type, Component *, Component::ComponentHash> components;
public:
	GameObject(const std::string& name) : transform(Transform(this)) 
	{
		ObjectName = name;
		isActive = true;
		isDestroy = false;
	}

	~GameObject();

	Transform transform;

	std::string GetName() { return ObjectName; };
	void AddComponent(Component* pComponent);
	Component* GetComponent(const std::wstring componentID);
	Component* GetComponent(const Component::Type componentID);
	void CleanUpComponent();

	bool isDestroy;
	bool isActive;

	std::string ObjectName;
	
	void Draw(const XMMATRIX& viewProjectionMatrix);
	void Update();

	void Destroy() { this->isDestroy = true; }
	bool IsDestroy() { return this->isDestroy; }

	void SetActive(bool bActive);
	bool IsActive() { return this->isActive; }

	BoundingBoxRenderer* GetBoundingBox() { return bbox; }

	virtual bool IsCollision(GameObject* model) { return false; };
	virtual void* GetCollider() { return NULL; }
	Component::Type GetColliderType() { return colliderType; }

	void SetParent(GameObject* pParent);
	GameObject* GetParent() { return parent; }
	void AddChild(GameObject* pParent);


	BoundingBoxRenderer* bbox;
	//�ݶ��̴� �߰��ؾߵ� 
	Component::Type colliderType;
	GameObject* parent = nullptr;
	std::vector<GameObject*> child;
};





