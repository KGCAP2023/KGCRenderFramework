#include "pch.h"
#include "Transform.h"
#include "GameObject.h"


void Transform::SetLookAtPos(XMFLOAT3 lookAtPos)
{
	if (lookAtPos.x == position.x && lookAtPos.y == position.y && lookAtPos.z == position.z)
		return;

	lookAtPos.x = position.x - lookAtPos.x;
	lookAtPos.y = position.y - lookAtPos.y;
	lookAtPos.z = position.z - lookAtPos.z;

	float pitch = 0.0f;
	if (lookAtPos.y != 0.0f)
	{
		const float distance = sqrt(lookAtPos.x * lookAtPos.x + lookAtPos.z * lookAtPos.z);
		pitch = atan(lookAtPos.y / distance);
	}

	float yaw = 0.0f;
	if (lookAtPos.x != 0.0f)
	{
		yaw = atan(lookAtPos.x / lookAtPos.z);
	}
	if (lookAtPos.z > 0)
		yaw += XM_PI;

	std::cout << "pitch :" << pitch << " yaw:" << yaw << std::endl;

	this->SetRotation(pitch, yaw, 0.0f);

}

const XMVECTOR& Transform::GetForward()
{
	return this->forward;
}

const XMVECTOR& Transform::GetRight()
{
	return this->right;
}

const XMVECTOR& Transform::GetBackward()
{
	return this->backward;
}

const XMVECTOR& Transform::GetLeft()
{
	return this->left;
}

const DirectX::XMVECTOR& Transform::GetPositionXMVector() const
{
	return this->positionVector;
}

const DirectX::XMFLOAT3& Transform::GetPositionXMFloat3() const
{
	return this->position;
}

const DirectX::XMVECTOR& Transform::GetRotationXMVector() const
{
	return this->rotationVector;
}

const DirectX::XMFLOAT3& Transform::GetRotationXMFloat3() const
{
	return this->rotation;
}

void Transform::SetScale(float xScale, float yScale, float zScale)
{
	this->scale = XMFLOAT3(xScale, yScale, zScale);
	UpdateMatrix();
}

void Transform::SetPosition(const DirectX::XMVECTOR& pos)
{
	DirectX::XMStoreFloat3(&this->position, pos);
	this->positionVector = pos;
	UpdateMatrix();
}

void Transform::SetPosition(const DirectX::XMFLOAT3& pos)
{
	this->position = pos;
	this->positionVector = DirectX::XMLoadFloat3(&this->position);
	UpdateMatrix();
}

void Transform::SetPosition(float x, float y, float z)
{
	this->position = DirectX::XMFLOAT3(x, y, z);
	this->positionVector = DirectX::XMLoadFloat3(&this->position);
	UpdateMatrix();
}

void Transform::Translate(const DirectX::XMVECTOR& pos)
{
	this->positionVector += pos;
	DirectX::XMStoreFloat3(&this->position, this->positionVector);
	UpdateMatrix();
}

void Transform::Translate(const DirectX::XMFLOAT3& pos)
{
	this->position.x += pos.x;
	this->position.y += pos.y;
	this->position.z += pos.z;
	this->positionVector = DirectX::XMLoadFloat3(&this->position);
	UpdateMatrix();
}

void Transform::Translate(float x, float y, float z)
{
	this->position.x += x;
	this->position.y += y;
	this->position.z += z;
	this->positionVector = DirectX::XMLoadFloat3(&this->position);
	UpdateMatrix();
}

void Transform::SetRotation(const DirectX::XMVECTOR& rot)
{
	this->rotationVector = rot;
	DirectX::XMStoreFloat3(&this->rotation, this->rotationVector);
	UpdateMatrix();
}

void Transform::SetRotation(const DirectX::XMFLOAT3& rot)
{
	this->rotation = rot;
	this->rotationVector = DirectX::XMLoadFloat3(&this->rotation);
	UpdateMatrix();
}

void Transform::SetRotation(float x, float y, float z)
{
	this->rotation = XMFLOAT3(x, y, z);
	this->rotationVector = DirectX::XMLoadFloat3(&this->rotation);
	UpdateMatrix();
}

void Transform::Rotate(const DirectX::XMVECTOR& rot)
{
	this->rotationVector += rot;
	DirectX::XMStoreFloat3(&this->rotation, this->rotationVector);
	UpdateMatrix();
}

void Transform::Rotate(const DirectX::XMFLOAT3& rot)
{
	this->rotation.x += rot.x;
	this->rotation.y += rot.y;
	this->rotation.z += rot.z;
	this->rotationVector = DirectX::XMLoadFloat3(&this->rotation);
	UpdateMatrix();
}

void Transform::Rotate(float x, float y, float z)
{
	this->rotation.x += x;
	this->rotation.y += y;
	this->rotation.z += z;
	this->rotationVector = DirectX::XMLoadFloat3(&this->rotation);
	UpdateMatrix();
}

void Transform::UpdateDirection()
{
	XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	forward = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
	backward = XMVector3TransformCoord(DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
	left = XMVector3TransformCoord(DEFAULT_LEFT_VECTOR, vecRotationMatrix);
	right = XMVector3TransformCoord(DEFAULT_RIGHT_VECTOR, vecRotationMatrix);
}

void Transform::UpdateMatrix()
{
	//행렬 업데이트
	this->worldMatrix = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) * XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z) * XMMatrixTranslation(position.x, position.y, position.z);
	this->UpdateDirection();
}
