#include "pch.h"
#include "SpriteRenderer.h"


void SpriteRenderer::Update()
{
	GameObject* owner = this->owner;
	owner->transform.worldMatrix = XMMatrixScaling(owner->transform.scale.x, owner->transform.scale.y, 1.0f) * XMMatrixRotationRollPitchYaw(owner->transform.rotation.x, owner->transform.rotation.y, owner->transform.rotation.z) * XMMatrixTranslation(owner->transform.position.x + owner->transform.scale.x / 2.0f, owner->transform.position.y + owner->transform.scale.y / 2.0f, owner->transform.position.z);
}
