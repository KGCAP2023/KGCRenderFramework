#pragma once
#include "pch.h"
#include "Transform.h"
#include <SimpleMath.h>
#include <SpriteBatch.h>

class Sprite;
class Animation2D
{
public:
	Animation2D(int x, int y, int width, int height, int count,
				Sprite& sprite, float holdTime, XMVECTORF32 chroma)
		:
		sprite(sprite),
		holdTime(holdTime),
		chroma(chroma)
	{
		for (int i = 0; i < count; i++)
		{
			//frames.push_back({x + i * width, x + (i+1) * width, y, y + height});
			frames.push_back({ x + i * width, y, x + (i + 1) * width, y + height });
		}
	}

	//Animation2D();
	//Animation2D(const Animation2D& rhs) = delete;
	//Animation2D& operator=(const Animation2D& ref) = delete;

	void Draw(Transform& transform, SpriteBatch* spriteBatch);
	void Update(float dt);
private:
	void Advance();

	XMVECTORF32 chroma = DirectX::Colors::Magenta;
	Sprite& sprite;
	std::vector<RECT> frames;
	int iCurFrame = 0;
	float holdTime;
	float curFrameTime = 0.0f;

};

