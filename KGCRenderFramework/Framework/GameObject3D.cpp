#include "GameObject3D.h"

void GameObject3D::SetLookAtPos(XMFLOAT3 lookAtPos)
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

const XMVECTOR& GameObject3D::GetForward()
{
	return this->forward;
}

const XMVECTOR& GameObject3D::GetRight()
{
	return this->right;
}

const XMVECTOR& GameObject3D::GetBackward()
{
	return this->backward;
}

const XMVECTOR& GameObject3D::GetLeft()
{
	return this->left;
}

void GameObject3D::Draw(const XMMATRIX& viewProjectionMatrix)
{
	assert("해당 업데이트문은 Draw 구현체가 필요합니다." && 0);
}

void GameObject3D::Update()
{
	assert("해당 업데이트문은 Update 구현체가 필요합니다." && 0);
}

void GameObject3D::UpdateDirection()
{
	XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	forward = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
	backward = XMVector3TransformCoord(DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
	left = XMVector3TransformCoord(DEFAULT_LEFT_VECTOR, vecRotationMatrix);
	right = XMVector3TransformCoord(DEFAULT_RIGHT_VECTOR, vecRotationMatrix);
}
