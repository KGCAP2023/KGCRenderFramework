#include "pch.h"
#include "ResourceManager.h"
#include "Framework.h"

void ResourceManager::Init(Framework* framework)
{
	device = framework->graphics.device;
	deviceContext = framework->graphics.deviceContext;
	spriteBatch = std::make_unique<DirectX::SpriteBatch>(this->deviceContext.Get());
	m_states = std::make_unique<CommonStates>(this->device.Get());
}

void ResourceManager::InitFont(const std::wstring& path)
{
	std::cout << "[=] Starting Font Initialize....." << std::endl;
	spriteFont = std::make_unique<DirectX::SpriteFont>(this->device.Get(), path.c_str());
	std::cout << "[O] Successfully Completed Font Initialize!" << std::endl;
}

Sprite* ResourceManager::LoadSprite(const std::string& name, const std::string& spritePath)
{
	Sprite* sp = new Sprite();

	if (sp->Init(this->device.Get(), this->deviceContext.Get(), spritePath))
	{
		this->_spriteMap.insert(std::make_pair<>(name, sp));
		return sp;
	}
	else
	{
		return nullptr;
	}
};

bool ResourceManager::LoadVertexShader(const std::string& name, const std::wstring& path, D3D11_INPUT_ELEMENT_DESC* layout, UINT numElements2D)
{
	VertexShader* vs = new VertexShader();

	if (!vs->Initialize(path, "main", "vs_5_0", layout, numElements2D, device))
	{
		return false;
	}

	this->_vsMap.insert(std::make_pair<>(name, vs));
	return true;
};

bool ResourceManager::LoadPixelShader(const std::string& name, const std::wstring& path)
{
	PixelShader* ps = new PixelShader();

	if (!ps->Initialize(path, "main", "ps_4_0_level_9_3", device))
	{
		return false;
	}

	this->_psMap.insert(std::make_pair<>(name, ps));
	return true;
}
Sprite* ResourceManager::FindSprite(const std::string& key)
{
	if (this->_spriteMap.find(key) != this->_spriteMap.end()) {
		return this->_spriteMap[key];
	}
	return nullptr;
}
;

VertexShader* ResourceManager::FindVertexShader(const std::string& key)
{
	if (this->_vsMap.find(key) != this->_vsMap.end()) {
		return this->_vsMap[key];
	}
	return nullptr;
}

PixelShader* ResourceManager::FindPixelShader(const std::string& key)
{
	if (this->_psMap.find(key) != this->_psMap.end()) {
		return this->_psMap[key];
	}
	return nullptr;
}