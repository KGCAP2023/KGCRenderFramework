#include "pch.h"
#include "SpriteRenderer.h"
#include "Framework.h"

void SpriteRenderer::Update()
{
	GameObject* owner = this->owner;
	owner->transform.worldMatrix = XMMatrixScaling(owner->transform.scale.x, owner->transform.scale.y, 1.0f) * XMMatrixRotationRollPitchYaw(owner->transform.rotation.x, owner->transform.rotation.y, owner->transform.rotation.z) * XMMatrixTranslation(owner->transform.position.x + owner->transform.scale.x / 2.0f, owner->transform.position.y + owner->transform.scale.y / 2.0f, owner->transform.position.z);
	owner->transform.UpdateDirection();
	const float& dt = Framework::getDeltaTime();

	if (selectedAnimation != nullptr)
	{
		selectedAnimation->Update(dt);
	}
}

void SpriteRenderer::Draw(const DirectX::XMMATRIX& viewProjectionMatrix)
{
	if (sprite != nullptr)
	{
		XMFLOAT3 pos = this->owner->transform.position;

		if (selectedAnimation != nullptr)
		{
			selectedAnimation->Draw(SimpleMath::Vector2(pos.x, pos.y), spriteBatch);
		}
		else
		{
			spriteBatch->Begin();
			spriteBatch->Draw(this->sprite->Get(), SimpleMath::Vector2(pos.x, pos.y));
			spriteBatch->End();
		}
	}
}
