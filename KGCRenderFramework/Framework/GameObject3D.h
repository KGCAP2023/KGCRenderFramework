#pragma once
#include "..\pch.h"
#include "GameObject.h"

class GameObject3D : public GameObject
{
public:
	virtual void Draw(const XMMATRIX& viewProjectionMatrix);
	void SetLookAtPos(XMFLOAT3 lookAtPos);
	const XMVECTOR& GetForward();
	const XMVECTOR& GetRight();
	const XMVECTOR& GetBackward();
	const XMVECTOR& GetLeft();

protected:
	
	virtual void Update();
	void UpdateDirection();

	//���� ����
	const XMVECTOR DEFAULT_FORWARD_VECTOR = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	//�� ����
	const XMVECTOR DEFAULT_UP_VECTOR = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	//�Ĺ� ���� 
	const XMVECTOR DEFAULT_BACKWARD_VECTOR = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	//���� ���� 
	const XMVECTOR DEFAULT_LEFT_VECTOR = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
	//������ ����
	const XMVECTOR DEFAULT_RIGHT_VECTOR = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	XMVECTOR forward;
	XMVECTOR left;
	XMVECTOR right;
	XMVECTOR backward;

};

