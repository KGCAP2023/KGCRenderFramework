#pragma once
#include "pch.h"
#include "ConstantBuffer.h"
#include "ConstantBufferType.h"
#include "Sprite.h"
#include "TileMap.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <CommonStates.h>
#include "Shaders.h"
#include "Model.h"
#include "SkinnedMesh.h"
class Framework;

class IResourceManager
{
public:
	/// <summary>
	/// 스프라이트를 리소스 매니저에 로드합니다.
	/// 실패시 nullptr를 반환합니다.
	/// </summary>
	/// <param name="name">등록할 이름</param>
	/// <param name="spritePath">파일 경로</param>
	/// <returns>스프라이트</returns>
	virtual Sprite* LoadSprite(const std::string& name, const std::string& spritePath) { return nullptr; };
	/// <summary>
	/// 스프라이트를 찾습니다.
	/// 존재하지 않을시 nullptr를 반환합니다.
	/// </summary>
	/// <param name="name">찾을 이름</param>
	/// <returns>스프트라이트</returns>
	virtual Sprite* FindSprite(const std::string& name) { return nullptr; };
	/// <summary>
	/// 버텍스 쉐이더를 로드합니다.
	/// </summary>
	/// <param name="name">이름</param>
	/// <param name="path">경로</param>
	/// <param name="layout">버텍스 레이아웃</param>
	/// <param name="numElements2D">버텍스 레이아웃 크기</param>
	/// <returns>성공여부</returns>
	virtual bool LoadVertexShader(const std::string& name, const std::wstring& path, D3D11_INPUT_ELEMENT_DESC* layout, UINT numElements2D) { return false; };
	/// <summary>
	/// 픽셀쉐이더를 로드합니다.
	/// </summary>
	/// <param name="name">이름</param>
	/// <param name="path">경로</param>
	/// <returns>성공여부</returns>
	virtual bool LoadPixelShader(const std::string& name, const std::wstring& path) { return false; };
	/// <summary>
	/// 버텍스 쉐이더를 찾습니다.
	/// 존재하지
	/// </summary>
	/// <param name="key">찾을 이름</param>
	/// <returns>버텍스쉐이더</returns>
	virtual VertexShader* FindVertexShader(const std::string& key) { return nullptr; };
	/// <summary>
	/// 픽셀 쉐이더를 찾습니다.
	/// 존재하지 않을시 nullptr를 반환합니다.
	/// </summary>
	/// <param name="key">찾을 이름</param>
	/// <returns>버텍스쉐이더</returns>
	virtual PixelShader* FindPixelShader(const std::string& key) { return nullptr; };
	/// <summary>
	/// 타일맵을 리소스매니저에 등록합니다.
	/// </summary>
	/// <param name="name">찾을 이름</param>
	/// <param name="tileMap">타일맵</param>
	virtual void RegisterTileMap(const std::string& name, TileMap* tileMap) {};
	/// <summary>
	/// 타일맵을 찾습니다.
	/// </summary>
	/// <param name="name">찾을 이름</param>
	/// <returns>타일맵</returns>
	virtual TileMap* FindTileMap(const std::string& name) { return nullptr; };
	/// <summary>
	/// 일반 모델링 파일을 로드합니다. 
	/// </summary>
	/// <param name="modelName"></param>
	/// <param name="filePath"></param>
	/// <returns></returns>
	virtual bool LoadModel(const std::string& modelName, const std::string& filePath) { return false; };
	/// <summary>
	/// 스키닝 모델(애니메이션이 있는 모델)을 로드합니다.
	/// </summary>
	/// <param name="modelName"></param>
	/// <param name="filePath"></param>
	/// <returns></returns>
	virtual bool LoadSkinnedModel(const std::string& modelName, const std::string& filePath) { return false; };
	/// <summary>
	/// 스키닝 모델을 가져옵니다. (Clone)
	/// </summary>
	/// <param name="modelName"></param>
	/// <returns></returns>
	virtual SkinnedMesh* FindSkinnedModel(const std::string& modelName) { return nullptr; };
	/// <summary>
	/// 일반 모델을 가져옵니다. (Clone)
	/// </summary>
	/// <param name="modelName"></param>
	/// <returns></returns>
	virtual Model* FindModel(const std::string& modelName) { return nullptr; };
	/// <summary>
	/// 모델에 캐싱되어있는 텍스쳐를 가져옵니다.
	/// </summary>
	/// <param name="modelName"></param>
	/// <returns></returns>
	virtual std::vector<Texture>* GetCachedTexture(const std::string& modelName) { return nullptr; };
	/// <summary>
	/// 스프라이트 맵을 순회합니다.
	/// </summary>
	/// <param name="callback"></param>
	virtual void SpriteForeach(std::function<void(Sprite*)> callback) {};
	/// <summary>
	/// 스프라이트 맵을 가져옵니다.
	/// </summary>
	/// <returns></returns>
	virtual std::unordered_map<std::string, Sprite*> GetSpriteMap() { return std::unordered_map<std::string, Sprite*>(); };
	/// <summary>
	/// 타일 맵을 순회합니다.
	/// </summary>
	/// <param name="callback"></param>
	virtual void TileMapForeach(std::function<void(TileMap*)> callback) {};
	/// <summary>
	/// 타일 맵을 가져옵니다.
	/// </summary>
	/// <returns></returns>
	virtual std::unordered_map<std::string, TileMap*> GetTileMap() { return std::unordered_map<std::string, TileMap*>(); };
};


class ResourceManager : public IResourceManager
{
public:
	ResourceManager() {};

	void Init(Framework *framework);

	//Font
	void InitFont(const std::wstring& path);

	//Sprite
	Sprite* LoadSprite(const std::string& name, const std::string& spritePath) override;
	Sprite* FindSprite(const std::string& name) override;

	//Sprite
	void RegisterTileMap(const std::string& name, TileMap* tileMap) override;
	TileMap* FindTileMap(const std::string& name) override;

	//Shader
	bool LoadVertexShader(const std::string& name, const std::wstring& path, D3D11_INPUT_ELEMENT_DESC* layout, UINT numElements2D) override;
	bool LoadPixelShader(const std::string& name, const std::wstring& path) override;

	VertexShader* FindVertexShader(const std::string& key) override;
	PixelShader* FindPixelShader(const std::string& key) override;

	bool LoadSkinnedModel(const std::string& modelName, const std::string& filePath) override;
	bool LoadModel(const std::string& modelName, const std::string& filePath) override;

	SkinnedMesh* FindSkinnedModel(const std::string& modelName) override;
	Model* FindModel(const std::string& modelName) override;

	std::vector<Texture>* GetCachedTexture(const std::string& modelName) override;

	void SpriteForeach(std::function<void(Sprite*)> callback) override;
	std::unordered_map<std::string, Sprite*> GetSpriteMap() override;

	void TileMapForeach(std::function<void(TileMap*)> callback) override;
	std::unordered_map<std::string, TileMap*> GetTileMap() override;

	//스프라이트
	std::unordered_map<std::string, Sprite*> _spriteMap;
	std::unordered_map<std::string, TileMap*> _TileMap;

	//모델
	std::unordered_map<std::string, Model*> _modelMap;
	//스키닝 모델
	std::unordered_map<std::string, SkinnedMesh*> _skinnedMap;
	//텍스쳐
	std::unordered_map<std::string, std::vector<Texture>*> _textures_loaded;

	//버텍스 쉐이더
	std::unordered_map<std::string, VertexShader*> _vsMap;
	std::unordered_map<std::string, PixelShader*> _psMap;

	//상수버퍼
	ConstantBuffer<CB_VS_1> cb1;
	ConstantBuffer<CB_VS_2> cb2;
	ConstantBuffer<CB_VS_2> cb_skinning_1;
	ConstantBuffer<CB_VS_3> cb_skinning_2;
	ConstantBuffer<CB_PS_light> cb_light;

	std::unique_ptr<DirectX::CommonStates> m_states;
	std::shared_ptr<SpriteBatch> spriteBatch;
	std::unique_ptr<SpriteFont> spriteFont;

	Microsoft::WRL::ComPtr<ID3D11Device> device = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext = nullptr;

};