#include "GameObject.h"
using namespace DirectX;
//XMLoad XMVECTOR로 탑재
//XMSTORE XMFLOAT로 만듬

std::unordered_map<std::string, GameObject*> GameObject::gameObjects;

const DirectX::XMVECTOR& GameObject::GetPositionXMVector() const
{
	return this->positionVector;
}

const DirectX::XMFLOAT3& GameObject::GetPositionXMFloat3() const
{
	return this->position;
}

const DirectX::XMVECTOR& GameObject::GetRotationXMVector() const
{
	return this->rotationVector;
}

const DirectX::XMFLOAT3& GameObject::GetRotationXMFloat3() const
{
	return this->rotation;
}

void GameObject::SetScale(float xScale, float yScale, float zScale)
{
	this->scale = XMFLOAT3(xScale, yScale, zScale);
	this->Update();
}

void GameObject::SetPosition(const DirectX::XMVECTOR& pos)
{
	DirectX::XMStoreFloat3(&this->position,pos);
	this->positionVector = pos;
	this->Update();
}

void GameObject::SetPosition(const DirectX::XMFLOAT3& pos)
{
	this->position = pos;
	this->positionVector = DirectX::XMLoadFloat3(&this->position);
	this->Update();
}

void GameObject::SetPosition(float x, float y, float z)
{
	this->position = DirectX::XMFLOAT3(x,y,z);
	this->positionVector = DirectX::XMLoadFloat3(&this->position);
	this->Update();
}

void GameObject::Translate(const DirectX::XMVECTOR& pos)
{
	this->positionVector += pos;
	DirectX::XMStoreFloat3(&this->position, this->positionVector);
	this->Update();
}

void GameObject::Translate(const DirectX::XMFLOAT3& pos)
{
	this->position.x += pos.x;
	this->position.y += pos.y;
	this->position.z += pos.z;
	this->positionVector = DirectX::XMLoadFloat3(&this->position);
	this->Update();
}

void GameObject::Translate(float x, float y, float z)
{
	this->position.x += x;
	this->position.y += y;
	this->position.z += z;
	this->positionVector = DirectX::XMLoadFloat3(&this->position);
	this->Update();
}

void GameObject::SetRotation(const DirectX::XMVECTOR& rot)
{
	this->rotationVector = rot;
	DirectX::XMStoreFloat3(&this->rotation, this->rotationVector);
	this->Update();
}

void GameObject::SetRotation(const DirectX::XMFLOAT3& rot)
{
	this->rotation = rot;
	this->rotationVector = DirectX::XMLoadFloat3(&this->rotation);
	this->Update();
}

void GameObject::SetRotation(float x, float y, float z)
{
	this->rotation = XMFLOAT3(x,y,z);
	this->rotationVector = DirectX::XMLoadFloat3(&this->rotation);
	this->Update();
}

void GameObject::Rotate(const DirectX::XMVECTOR& rot)
{
	this->rotationVector += rot;
	DirectX::XMStoreFloat3(&this->rotation,this->rotationVector);
	this->Update();
}

void GameObject::Rotate(const DirectX::XMFLOAT3& rot)
{
	this->rotation.x += rot.x;
	this->rotation.y += rot.y;
	this->rotation.z += rot.z;
	this->rotationVector = DirectX::XMLoadFloat3(&this->rotation);
	this->Update();
}

void GameObject::Rotate(float x, float y, float z)
{
	this->rotation.x += x;
	this->rotation.y += y;
	this->rotation.z += z;
	this->rotationVector = DirectX::XMLoadFloat3(&this->rotation);
	this->Update();
}

void GameObject::Update()
{
	assert("해당 업데이트문은 Update 구현체가 필요합니다." && 0);
}

void GameObject::Draw(const XMMATRIX& viewProjectionMatrix)
{
	assert("해당 업데이트문은 Draw 구현체가 필요합니다." && 0);
}
