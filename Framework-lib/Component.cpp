#include "pch.h"
#include "Component.h"


void Component::Update()
{
	assert("�ش� ������Ʈ���� Component ����ü�� �ʿ��մϴ�." && 0);
}

void Component::Draw(const DirectX::XMMATRIX& viewProjectionMatrix)
{
	assert("�ش� ������Ʈ���� Renderer ����ü�� �ʿ��մϴ�." && 0);
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