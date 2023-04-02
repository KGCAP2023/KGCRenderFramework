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



void Camera3D::ChangeProjectionValues(ViewType _type) {

	switch (_type)
	{
	case ViewType::_2D:
		changeMode(false);
		this->GetOwner()->transform.SetRotation(0, 0, 0);
		break;

	case ViewType::_3D:
		changeMode(true);
		break;
	default:
		break;
	}

}


void Camera3D::initViewMatrix(float fovDegrees, float aspectRatio, float nearZ, float farZ, float _width, float _hight, float _nearZ2, float _farZ2)
{
	float fovRadians = (fovDegrees / 360.0f) * XM_2PI;
	this->view3DMatrix = DirectX::XMMatrixPerspectiveFovLH(fovRadians, aspectRatio, nearZ, farZ);

	this->view2DMatrix = DirectX::XMMatrixOrthographicLH(_width, _hight, _nearZ2, _farZ2);

	//todo : 설정에 따라서 초기 뷰 모드 메트릭스로 설정, 현재는 3d 가 디폴트
	this->projectionMatrix = view3DMatrix;


}

const XMMATRIX& Camera3D::GetViewMatrix() const
{
	return this->viewMatrix;
}



void Camera3D::changeMode(bool _bool) {
	if (curType == _bool) return;
	projectionMatrix = _bool ? view2DMatrix : view3DMatrix;
	curType = _bool;
}

void Camera3D::cameraLerp()
{
	if(curType)
		this->projectionMatrix = this->projectionMatrix + view3DMatrix/8000;
	else
		this->projectionMatrix = this->projectionMatrix + view2DMatrix / 2.8f;

}

const XMMATRIX& Camera3D::GetProjectionMatrix()
{

	cameraLerp();
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
