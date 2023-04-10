#include "pch.h"
#include "Animation2D.h"
#include "Sprite.h"

void Animation2D::Draw(Transform& transform, SpriteBatch* spriteBatch)
{
	XMFLOAT2 pos(transform.position.x, transform.position.y);
	float rot = transform.rotation.z;
	float scale = transform.scale.x;

	spriteBatch->Begin();

	spriteBatch->Draw(sprite.Get(), pos, &frames[iCurFrame],
		Colors::White, rot, DirectX::SimpleMath::Vector2(0, 0), scale);

	spriteBatch->End();
}

void Animation2D::Update(float dt)
{
	curFrameTime += dt;
	while (curFrameTime >= holdTime)
	{
		Advance();
		curFrameTime -= holdTime;
	}
}

void Animation2D::Advance()
{
	if (++iCurFrame >= frames.size())
	{
		iCurFrame = 0;
	}
}
