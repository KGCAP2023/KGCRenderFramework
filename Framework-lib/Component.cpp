#include "pch.h"
#include "Component.h"


Component* Component::Copy(GameObject* owner)
{
	assert("�ش� COPY�� Component ����ü�� �ʿ��մϴ�." && 0);
	return nullptr;
}

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