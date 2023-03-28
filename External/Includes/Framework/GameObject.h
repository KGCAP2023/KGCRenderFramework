#pragma once
#include "Component.h"
#include "Transform.h"
#include "BoundingBox.h"
using namespace DirectX;
//클래스 변수에  XMFLOAT를 넣자
//힙에서는 16바이트 정렬이 조금 까다로움

class GameObject
{
private:
	std::unordered_map<Component::Type, Component *, Component::ComponentHash> components;
public:

	static enum class ObjectType
	{
		OBJECT_DEFAULT,
		OBJECT_2D,
		OBJECT_3D
	};

	GameObject(const std::string& name) : transform(Transform(this)) 
	{
		ObjectName = name;
		isActive = true;
		isDestroy = false;
		this->objectType = GameObject::ObjectType::OBJECT_DEFAULT;
	}

	~GameObject();

	bool isDestroy;
	bool isActive;
	std::string ObjectName;

	Transform transform;
	GameObject::ObjectType objectType = GameObject::ObjectType::OBJECT_DEFAULT;
	BoundingBoxRenderer* bbox = nullptr;

	std::string GetName() { return ObjectName; };
	void SetObjectType(GameObject::ObjectType type);

	/// <summary>
	/// Component를 추가합니다.
	/// <주의> 한오브젝트에서 렌더러를 2D렌더러/3D렌더러를 같이 사용하지 마십시요
	/// </summary>
	/// <param name="pComponent"></param>
	void AddComponent(Component* pComponent);
	Component* GetComponent(const std::string componentID);
	Component* GetComponent(const Component::Type componentID);
	void CleanUpComponent();

	void Draw(const XMMATRIX& viewProjectionMatrix, GameObject::ObjectType type);
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


	//콜라이더 추가해야됨 
	
	Component::Type colliderType;
	GameObject* parent = nullptr;
	std::vector<GameObject*> child;
};





