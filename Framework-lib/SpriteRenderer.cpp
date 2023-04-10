#include "pch.h"
#include "SpriteRenderer.h"
#include "Framework.h"
#include "ResourceManager.h"
#include "BoundingBox.h"

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

		this->bbox->Draw(viewProjectionMatrix);
	}
}

void SpriteRenderer::InitBoundingBox()
{
	this->bbox = new BoundingBox2D(this->owner,this,res);
	this->owner->bbox = this->bbox;
}

BoundingBoxRenderer* SpriteRenderer::GetBoundingBox()
{
	return this->bbox;
}
