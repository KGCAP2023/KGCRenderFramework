#include "pch.h"
#include "Camera.h"


Camera3D::Camera3D(GameObject* owner) : Component(owner)
{
	this->type = Component::Type::CAMERA;
	this->owner->transform.position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->owner->transform.positionVector = XMLoadFloat3(&this->owner->transform.position);
	this->owner->transform.rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->owner->transform.rotationVector = XMLoadFloat3(&this->owner->transform.rotation);
	this->Update();
}

void Camera3D::SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
	float fovRadians = (fovDegrees / 360.0f) * XM_2PI;
	this->projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fovRadians, aspectRatio, nearZ, farZ);
}

const XMMATRIX& Camera3D::GetViewMatrix() const
{
	return this->viewMatrix;
}

const XMMATRIX& Camera3D::GetProjectionMatrix() const
{
	return this->projectionMatrix;
}

void Camera3D::Update()
{
	//Calculate Camera3D rotation matrix
	XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYaw(this->owner->transform.rotation.x, this->owner->transform.rotation.y, this->owner->transform.rotation.z) ;
	//Calculate unit vector of cam target based off Camera3D forward value transformed by cam rotation matrix
	XMVECTOR camTarget = XMVector3TransformCoord(this->owner->transform.DEFAULT_FORWARD_VECTOR, camRotationMatrix);
	//Adjust cam target to be offset by the Camera3D's current position
	camTarget += this->owner->transform.positionVector;
	//Calculate up direction based on current rotation
	XMVECTOR upDir = XMVector3TransformCoord(this->owner->transform.DEFAULT_UP_VECTOR, camRotationMatrix);
	//Rebuild view matrix
	this->viewMatrix = XMMatrixLookAtLH(this->owner->transform.positionVector, camTarget, upDir);
	this->owner->transform.UpdateDirection();
}
