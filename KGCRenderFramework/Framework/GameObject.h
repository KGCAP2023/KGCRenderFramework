#pragma once
#include "..\pch.h"
#include "Component.h"
#include "Transform.h"
#include "Model.h"
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
	GameObject() : transform(Transform(this)) { }
	~GameObject();

	bool isDestroy;
	bool isActive;

	Transform transform;
	std::wstring ObjectName;
	
	void Draw(const XMMATRIX& viewProjectionMatrix);
	void Update();

	void AddComponent(Component* pComponent);
	Component* GetComponent(const std::wstring componentID);
	void CleanUpComponent();

	void Destroy() { this->isDestroy = true; }
	bool IsDestroy() { return this->isDestroy; }

	void SetActive(bool bActive);
	bool IsActive() { return this->isActive; }

	virtual bool IsCollision(GameObject* model) { return false; };
	virtual void* GetCollider() { return NULL; }
	Component::Type GetColliderType() { return colliderType; }

	//void SetParent(GameObject* pParent);
	//GameObject* GetParent() { return parent; }
	//void AddChild(GameObject* pParent);

	static std::unordered_map<std::string, GameObject*> gameObjects;

	//콜라이더 추가해야됨 
	Component::Type colliderType;



	//GameObject* parent;
};

// 전역변수는 다른곳에서 가져다 쓸때 #include 없이 순수 extern 만 있어야됨
// 그래야 여러번 선언 오류가 안뜸
// std::unordered_map<std::string, GameObject*> gameObjects;




