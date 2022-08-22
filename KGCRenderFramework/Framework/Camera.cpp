#include "Camera.h"


Camera3D::Camera3D()
{
	this->position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->positionVector = XMLoadFloat3(&this->position);
	this->rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->rotationVector = XMLoadFloat3(&this->rotation);
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
	XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYaw(this->rotation.x, this->rotation.y, this->rotation.z) ;
	//Calculate unit vector of cam target based off Camera3D forward value transformed by cam rotation matrix
	XMVECTOR camTarget = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, camRotationMatrix);
	//Adjust cam target to be offset by the Camera3D's current position
	camTarget += this->positionVector;
	//Calculate up direction based on current rotation
	XMVECTOR upDir = XMVector3TransformCoord(this->DEFAULT_UP_VECTOR, camRotationMatrix);
	//Rebuild view matrix
	this->viewMatrix = XMMatrixLookAtLH(this->positionVector, camTarget, upDir);

	this->UpdateDirection();

}
