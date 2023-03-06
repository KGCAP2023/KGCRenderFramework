#pragma once
#include "Component.h"
#include "Transform.h"
#include "Renderer.h"
using namespace DirectX;
//클래스 변수에  XMFLOAT를 넣자
//힙에서는 16바이트 정렬이 조금 까다로움

class GameObject
{
private:
	std::vector<Renderer*> render;
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

	virtual bool IsCollision(GameObject* model) { return false; };
	virtual void* GetCollider() { return NULL; }
	Component::Type GetColliderType() { return colliderType; }

	void SetParent(GameObject* pParent);
	GameObject* GetParent() { return parent; }
	void AddChild(GameObject* pParent);

	static std::unordered_map<std::string, GameObject*> gameObjects;

	//콜라이더 추가해야됨 
	Component::Type colliderType;
	GameObject* parent = nullptr;
	std::vector<GameObject*> child;
};

// 전역변수는 다른곳에서 가져다 쓸때 #include 없이 순수 extern 만 있어야됨
// 그래야 여러번 선언 오류가 안뜸
// std::unordered_map<std::string, GameObject*> gameObjects;




