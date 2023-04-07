#include "pch.h"
#include "ResourceManager.h"
#include "Framework.h"

void ResourceManager::Init(Framework* framework)
{
	this->framework = framework;
	device = framework->graphics.device;
	deviceContext = framework->graphics.deviceContext;
	depthStencilView = framework->graphics.depthStencilView;
	depthStencilBuffer = framework->graphics.depthStencilBuffer;
	depthStencilState = framework->graphics.depthStencilState;
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

	if (sp->Init(name,this->device.Get(), this->deviceContext.Get(), spritePath,this->spriteBatch.get()))
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

void ResourceManager::RegisterTileMap(const std::string& name, TileMap* tileMap)
{
	if(tileMap != nullptr)
		this->_TileMap.insert(std::make_pair<>(name, tileMap));
}

TileMap* ResourceManager::FindTileMap(const std::string& name)
{
	if (this->_TileMap.find(name) != this->_TileMap.end()) {
		return this->_TileMap[name];
	}
	return nullptr;
}

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

bool ResourceManager::LoadSkinnedModel(const std::string& modelName, const std::string& filePath)
{
	if (this->_skinnedMap.find(modelName) != this->_skinnedMap.end()) {
		std::cout << "[x] 이미 <"<< modelName <<"> 모델이 존재합니다!" << std::endl;
		return false; 
	}
	else
	{
		SkinnedMesh* model = new SkinnedMesh();
		if (!model->LoadMesh(modelName, filePath, this))
		{
			std::cout << "[x] 모델 <" << modelName <<">를 로드하는데 실패하였습니다."<< std::endl;
			return false;
		}
		else
		{
			this->_skinnedMap.insert(std::make_pair<>(modelName, model));
			return true;
		}	
	}
}

bool ResourceManager::LoadModel(const std::string& modelName, const std::string& filePath)
{
	if (this->_modelMap.find(modelName) != this->_modelMap.end()) {
		std::cout << "[x] 이미 <" << modelName << "> 모델이 존재합니다!" << std::endl;
		return false;
	}
	else
	{
		Model* model = new Model();
		if (!model->Init(modelName, filePath, this))
		{
			std::cout << "[x] 모델 <" << modelName << ">를 로드하는데 실패하였습니다." << std::endl;
			return false;
		}
		else
		{
			this->_modelMap.insert(std::make_pair<>(modelName, model));
			return true;
		}
			
	}
}

SkinnedMesh* ResourceManager::FindSkinnedModel(const std::string& modelName)
{
	if (this->_skinnedMap.find(modelName) != this->_skinnedMap.end()) {
		SkinnedMesh* skinned = new SkinnedMesh(*(this->_skinnedMap[modelName]));
		return skinned;
	}
	else return nullptr;
}

Model* ResourceManager::FindModel(const std::string& modelName)
{
	if (this->_modelMap.find(modelName) != this->_modelMap.end()) {
		Model* model = new Model(*(this->_modelMap[modelName]));
		return model;
	}
	else return nullptr;
}

std::vector<Texture>* ResourceManager::GetCachedTexture(const std::string& modelName)
{
	if (this->_textures_loaded.find(modelName) != this->_textures_loaded.end()) {
		return this->_textures_loaded[modelName];
	}
	else
	{
		std::vector<Texture>* tex = new std::vector<Texture>();
		this->_textures_loaded.insert(std::make_pair<>(modelName, tex));
		return tex;
	}
}

void ResourceManager::ModelForeach(std::function<void(Model*)> callback)
{
	for (auto& pair : this->_modelMap)
	{
		Model* sp = pair.second;
		callback(sp);
	}
}

std::unordered_map<std::string, Model*> ResourceManager::GetModelMap()
{
	return this->_modelMap;
}

void ResourceManager::SpriteForeach(std::function<void(Sprite*)> callback)
{
	for (auto& pair : this->_spriteMap)
	{
		Sprite* sp = pair.second;
		callback(sp);
	}
}

std::unordered_map<std::string, Sprite*> ResourceManager::GetSpriteMap()
{
	return this->_spriteMap;
}

void ResourceManager::TileMapForeach(std::function<void(TileMap*)> callback)
{
	for (auto& pair : this->_TileMap)
	{
		TileMap* tile = pair.second;
		callback(tile);
	}
}

std::unordered_map<std::string, TileMap*> ResourceManager::GetTileMap()
{
	return this->_TileMap;
}

void ResourceManager::LoadAudio(const char* audioName, const char* audioFilePath)
{
	framework->audioManager.LoadAudio(audioName, audioFilePath);
}

void ResourceManager::DeleteAudio(const char* audioName)
{
	framework->audioManager.DeleteAudio(audioName);
}



