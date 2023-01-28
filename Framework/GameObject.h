#pragma once
#include "Component.h"
#include "Transform.h"
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

	//�ݶ��̴� �߰��ؾߵ� 
	Component::Type colliderType;
	GameObject* parent = nullptr;
	std::vector<GameObject*> child;
};

// ���������� �ٸ������� ������ ���� #include ���� ���� extern �� �־�ߵ�
// �׷��� ������ ���� ������ �ȶ�
// std::unordered_map<std::string, GameObject*> gameObjects;




