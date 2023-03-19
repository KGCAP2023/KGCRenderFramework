#pragma once
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "ConstantBufferType.h"
#include "Texture.h"
#include "GameObject.h"
#include "Animation2D.h"
#include <SimpleMath.h>
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>
#include <SpriteBatch.h>

class Sprite
{
public:
	bool Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::string spritePath);
	float GetWidth();
	float GetHeight();
	DirectX::SimpleMath::Vector2 origin;
	ID3D11ShaderResourceView* Get();
	ID3D11ShaderResourceView** GetTextureResourceViewAddress();

	void Draw(const DirectX::SimpleMath::Vector2& pos, SpriteBatch* spriteBatch)
	{
		if (selectedAnimation != nullptr)
		{
			selectedAnimation->Draw(pos,spriteBatch);
		}
		else
		{
			spriteBatch->Begin();
			spriteBatch->Draw(this->view.Get(),pos);
			spriteBatch->End();
		}
	}

	void Update(const float& dt)
	{
		if (selectedAnimation != nullptr)
		{
			selectedAnimation->Update(dt);
		}
	}

	void AddAnimation2D(const std::string& name, int x, int y, int width, int height, int count,
		 float holdTime, XMVECTORF32 chroma)
	{
		Animation2D* animation = new Animation2D(x, y, width, height, count,
			*this, holdTime, chroma);
		this->_AnimationMap.insert(std::make_pair<>(name, animation));
	};

	void RemoveAnimation2D(const std::string& name)
	{
		this->_AnimationMap.erase(name);
	};

	Animation2D* FindAnimation2D(const std::string& key)
	{
		if (this->_AnimationMap.find(key) != this->_AnimationMap.end()) {
			return this->_AnimationMap[key];
		}
		return nullptr;
	}

	bool SelectAnimation(const std::string& name)
	{
		Animation2D* selected = FindAnimation2D(name);

		if (selected != nullptr)
		{
			selectedAnimation = selected;
			return true;
		}
		else
			return false;
	}

	Animation2D* GetSelectedAnim() {return selectedAnimation;}

private:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2d = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> view = nullptr;
	ID3D11DeviceContext* deviceContext = nullptr;
	Animation2D* selectedAnimation = nullptr;
	std::unordered_map<std::string, Animation2D*> _AnimationMap;
};

