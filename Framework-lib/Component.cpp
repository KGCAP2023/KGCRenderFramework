#include "pch.h"
#include "Component.h"


Component* Component::Copy(GameObject* owner)
{
	assert("해당 COPY는 Component 구현체가 필요합니다." && 0);
	return nullptr;
}

void Component::Update()
{
	assert("해당 업데이트문은 Component 구현체가 필요합니다." && 0);
}

void Component::Draw(const DirectX::XMMATRIX& viewProjectionMatrix)
{
	assert("해당 업데이트문은 Renderer 구현체가 필요합니다." && 0);
}

Component::Type Component::GetType()
{
	return this->type;
}

GameObject* Component::GetOwner()
{
	return this->owner;
}

std::string Component::GetName()
{
	return this->name;
}