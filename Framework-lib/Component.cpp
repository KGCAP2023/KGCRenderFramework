#include "pch.h"
#include "Component.h"


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

Component::Type Component::GetDeleteType()
{
	return this->deleteType;
}
void Component::SetDeleteType(Component::Type type)
{
	this->deleteType = type;
}

GameObject* Component::GetOwner()
{
	return this->owner;
}

std::string Component::GetName()
{
	return this->name;
}