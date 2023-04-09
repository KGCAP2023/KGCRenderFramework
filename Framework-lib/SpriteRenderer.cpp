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
		Transform& t = this->owner->transform;
		XMFLOAT2 pos(t.position.x, t.position.y);
		float rot = t.rotation.z;
		float scale = t.scale.x;


		if (selectedAnimation != nullptr)
		{
			selectedAnimation->Draw(t, spriteBatch);
		}
		else
		{
			spriteBatch->Begin();

			spriteBatch->Draw(this->sprite->Get(), pos, nullptr,
				Colors::White, rot, DirectX::SimpleMath::Vector2(0, 0), scale);

			spriteBatch->End();
		}
	}
}
