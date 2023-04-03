#include "pch.h"
#include "SpriteRenderer.h"
#include "Framework.h"

void SpriteRenderer::Update()
{
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
