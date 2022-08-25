#include "Component.h"

void Component::Update()
{
	assert("해당 업데이트문은 Component 구현체가 필요합니다." && 0);
}

Component::Type Component::GetType()
{
	return this->type;
}
