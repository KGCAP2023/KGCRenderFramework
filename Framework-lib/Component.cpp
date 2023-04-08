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

GameObject* Component::GetOwner()
{
	return this->owner;
}

std::string Component::GetName()
{
	return this->name;
}