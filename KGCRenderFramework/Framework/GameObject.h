#pragma once
#include "..\pch.h"
#include "Component.h"
#include "Transform.h"
#include "Model.h"
#include "Renderer.h"
using namespace DirectX;
//Ŭ���� ������  XMFLOAT�� ����
//�������� 16����Ʈ ������ ���� ��ٷο�

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

	//�ݶ��̴� �߰��ؾߵ� 
	Component::Type colliderType;



	//GameObject* parent;
};

// ���������� �ٸ������� ������ ���� #include ���� ���� extern �� �־�ߵ�
// �׷��� ������ ���� ������ �ȶ�
// std::unordered_map<std::string, GameObject*> gameObjects;




