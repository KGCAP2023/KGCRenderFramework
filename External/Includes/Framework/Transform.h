#pragma once

class GameObject;
using namespace DirectX;

class Transform
{
public:
	GameObject* owner;

	Transform(GameObject* owner) : owner(owner) {}
	
	void Copy(const Transform& rhs)
	{
		worldMatrix = rhs.worldMatrix;

		position = rhs.position;
		rotation = rhs.rotation;
		scale = rhs.scale;

		positionVector = rhs.positionVector;
		rotationVector = rhs.rotationVector;
		forward = rhs.forward;
		left = rhs.left;
		right = rhs.right;
		backward = rhs.backward;
	}

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

	void SetLookAtPos(XMFLOAT3 lookAtPos);
	const XMVECTOR& GetForward();
	const XMVECTOR& GetRight();
	const XMVECTOR& GetBackward();
	const XMVECTOR& GetLeft();

	void UpdateDirection();
	void UpdateMatrix();

	DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();

	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 scale;

	DirectX::XMVECTOR positionVector;
	DirectX::XMVECTOR rotationVector;

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

	bool isPerspective;

	DirectX::XMVECTOR forward;
	DirectX::XMVECTOR left;
	DirectX::XMVECTOR right;
	DirectX::XMVECTOR backward;

};

