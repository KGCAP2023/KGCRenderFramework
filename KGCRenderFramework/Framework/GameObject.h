#pragma once
#include "..\pch.h"
#include "Component.h"
using namespace DirectX;
//Ŭ���� ������  XMFLOAT�� ����
//�������� 16����Ʈ ������ ���� ��ٷο�

class GameObject
{
private:
	std::unordered_map<Component::Type, Component *, Component::ComponentHash> components;
public:
	virtual void Draw(const XMMATRIX& viewProjectionMatrix);

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

	const DirectX::XMVECTOR& GetPositionXMVector() const;
	const DirectX::XMFLOAT3& GetPositionXMFloat3() const;
	const DirectX::XMVECTOR& GetRotationXMVector() const;
	const DirectX::XMFLOAT3& GetRotationXMFloat3() const;

	//������ 
	void SetScale(float xScale, float yScale, float zScale = 1.0f);

	//������
	void SetPosition(const DirectX::XMVECTOR& pos);
	void SetPosition(const DirectX::XMFLOAT3& pos);
	void SetPosition(float x, float y, float z);

	void Translate(const DirectX::XMVECTOR& pos);
	void Translate(const DirectX::XMFLOAT3& pos);
	void Translate(float x, float y, float z);

	//�����̼�
	void SetRotation(const DirectX::XMVECTOR& rot);
	void SetRotation(const DirectX::XMFLOAT3& rot);
	void SetRotation(float x, float y, float z);

	void Rotate(const DirectX::XMVECTOR& rot);
	void Rotate(const DirectX::XMFLOAT3& rot);
	void Rotate(float x, float y, float z);

	static std::unordered_map<std::string, GameObject*> gameObjects;

protected:
	/*
		����� ������Ʈ
	*/
	
	virtual void Update();

	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 scale;

	DirectX::XMVECTOR positionVector;
	DirectX::XMVECTOR rotationVector;

	//�ݶ��̴� �߰��ؾߵ� 
	Component::Type colliderType;

	bool isDestroy;
	bool isActive;

	std::wstring ObjectName;

	//GameObject* parent;
};

// ���������� �ٸ������� ������ ���� #include ���� ���� extern �� �־�ߵ�
// �׷��� ������ ���� ������ �ȶ�
// std::unordered_map<std::string, GameObject*> gameObjects;




