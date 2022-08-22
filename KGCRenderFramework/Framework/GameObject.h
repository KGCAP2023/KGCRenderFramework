#pragma once
#include "..\pch.h"
#include "Component.h"
using namespace DirectX;
//클래스 변수에  XMFLOAT를 넣자
//힙에서는 16바이트 정렬이 조금 까다로움

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

	//스케일 
	void SetScale(float xScale, float yScale, float zScale = 1.0f);

	//포지션
	void SetPosition(const DirectX::XMVECTOR& pos);
	void SetPosition(const DirectX::XMFLOAT3& pos);
	void SetPosition(float x, float y, float z);

	void Translate(const DirectX::XMVECTOR& pos);
	void Translate(const DirectX::XMFLOAT3& pos);
	void Translate(float x, float y, float z);

	//로테이션
	void SetRotation(const DirectX::XMVECTOR& rot);
	void SetRotation(const DirectX::XMFLOAT3& rot);
	void SetRotation(float x, float y, float z);

	void Rotate(const DirectX::XMVECTOR& rot);
	void Rotate(const DirectX::XMFLOAT3& rot);
	void Rotate(float x, float y, float z);

	static std::unordered_map<std::string, GameObject*> gameObjects;

protected:
	/*
		행렬을 업데이트
	*/
	
	virtual void Update();

	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 scale;

	DirectX::XMVECTOR positionVector;
	DirectX::XMVECTOR rotationVector;

	//콜라이더 추가해야됨 
	Component::Type colliderType;

	bool isDestroy;
	bool isActive;

	std::wstring ObjectName;

	//GameObject* parent;
};

// 전역변수는 다른곳에서 가져다 쓸때 #include 없이 순수 extern 만 있어야됨
// 그래야 여러번 선언 오류가 안뜸
// std::unordered_map<std::string, GameObject*> gameObjects;




