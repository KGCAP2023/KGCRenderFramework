#include "pch.h"
#include "Animation2D.h"
#include "Sprite.h"

void Animation2D::Draw(DirectX::SimpleMath::Vector2 pos, SpriteBatch* spriteBatch)
{
	spriteBatch->Begin();

	spriteBatch->Draw(sprite.Get(), pos, &frames[iCurFrame],
		Colors::White, 0.f, DirectX::SimpleMath::Vector2(0, 0), 1.f);

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
