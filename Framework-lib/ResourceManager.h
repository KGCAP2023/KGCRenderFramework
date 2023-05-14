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
	/// 모델에 캐싱되어있는 경계 정점을 가져옵니다.
	/// </summary>
	/// <param name="modelName"></param>
	/// <returns></returns>
	virtual std::vector<SimpleVertex>* GetCachedBBOXVertices(const std::string& modelName) { return nullptr; };
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
	/// <summary>
	/// 오디오 파일을 불러와 map에 저장합니다.
	/// </summary>
	/// <param name="audioName">map에 저장될 이름입니다.</param>
	/// <param name="audioFilePath">오디오 파일의 경로입니다.</param>
	virtual void LoadAudio(const char* audioName, const char* audioFilePath) {};
	/// <summary>
	/// 불러온 오디오를 삭제합니다.
	/// </summary>
	/// <param name="audioName">삭제할 오디오 이름입니다.</param>
	virtual void DeleteAudio(const char* audioName) {};
	/// <summary>
	/// 오디오 맵을 가져옵니다.
	/// </summary>
	/// <returns></returns>
	virtual std::unordered_map<std::string, FMOD::Sound*> GetAudioMap() { return std::unordered_map<std::string, FMOD::Sound*>(); }
	/// <summary>
	/// 모델을 순회합니다.
	/// </summary>
	/// <param name="callback"></param>
	virtual void ModelForeach(std::function<void(Model*)> callback) {};
	/// <summary>
	/// 모델맵을 반환합니다.
	/// </summary>
	/// <returns></returns>
	virtual std::unordered_map<std::string, Model*> GetModelMap() { return std::unordered_map<std::string, Model*>(); };
	/// <summary>
	/// 불러온 스프라이트를 삭제합니다.
	/// </summary>
	/// <param name="audioName">삭제할 스프라이트 이름입니다.</param>
	virtual void DeleteSprite(const char* Name) {};
	/// <summary>
	/// 불러온 타일을 삭제합니다.
	/// </summary>
	/// <param name="audioName">삭제할 타일 이름입니다.</param>
	virtual void DeleteTile(const char* Name) {};
	/// <summary>
	/// 불러온 모델를 삭제합니다.
	/// </summary>
	/// <param name="audioName">삭제할 모델 이름입니다.</param>
	virtual void DeleteModel(const char* Name) {};
	/// <summary>
	/// 불러온 스키닝 모델을 삭제합니다.
	/// </summary>
	/// <param name="audioName">삭제할 스키닝 모델 이름입니다.</param>
	virtual void DeleteSkinnedModel(const char* Name) {};
	/// <summary>
	/// 불러온 텍스쳐를 삭제합니다.
	/// </summary>
	/// <param name="audioName">삭제할 텍스쳐 이름입니다.</param>
	virtual void DeleteTextures(const char* Name) {};
};


class ResourceManager : public IResourceManager
{
public:
	Framework* framework;
	ResourceManager() {};

	void Init(Framework *framework);

	//Font
	void InitFont(const std::wstring& path);

	//Sprite
	Sprite* LoadSprite(const std::string& name, const std::string& spritePath) override;
	Sprite* FindSprite(const std::string& name) override;
	void DeleteSprite(const char* Name) override;

	//Sprite
	void RegisterTileMap(const std::string& name, TileMap* tileMap) override;
	TileMap* FindTileMap(const std::string& name) override;
	void DeleteTile(const char* Name) override;

	//Shader
	bool LoadVertexShader(const std::string& name, const std::wstring& path, D3D11_INPUT_ELEMENT_DESC* layout, UINT numElements2D) override;
	bool LoadPixelShader(const std::string& name, const std::wstring& path) override;

	VertexShader* FindVertexShader(const std::string& key) override;
	PixelShader* FindPixelShader(const std::string& key) override;

	bool LoadSkinnedModel(const std::string& modelName, const std::string& filePath) override;
	bool LoadModel(const std::string& modelName, const std::string& filePath) override;

	SkinnedMesh* FindSkinnedModel(const std::string& modelName) override;
	Model* FindModel(const std::string& modelName) override;
	void DeleteModel(const char* Name) override;
	void DeleteSkinnedModel(const char* Name) override;

	std::vector<Texture>* GetCachedTexture(const std::string& modelName) override;
	std::vector<SimpleVertex>* GetCachedBBOXVertices(const std::string& modelName) override;
	void DeleteTextures(const char* Name) override;

	void ModelForeach(std::function<void(Model*)> callback) override;
	std::unordered_map<std::string, Model*> GetModelMap() override;

	void SpriteForeach(std::function<void(Sprite*)> callback) override;
	std::unordered_map<std::string, Sprite*> GetSpriteMap() override;

	void TileMapForeach(std::function<void(TileMap*)> callback) override;
	std::unordered_map<std::string, TileMap*> GetTileMap() override;

	//Audio
	void LoadAudio(const char* audioName, const char* audioFilePath) override;
	void DeleteAudio(const char* audioName) override;
	std::unordered_map<std::string, FMOD::Sound*> GetAudioMap() override;

	

	//스프라이트
	std::unordered_map<std::string, Sprite*> _spriteMap;
	std::unordered_map<std::string, TileMap*> _TileMap;

	//모델
	std::unordered_map<std::string, Model*> _modelMap;
	//스키닝 모델
	std::unordered_map<std::string, SkinnedMesh*> _skinnedMap;
	//텍스쳐
	std::unordered_map<std::string, std::vector<Texture>*> _textures_loaded;
	std::unordered_map<std::string, std::vector<SimpleVertex>*> _bbox_vertices;

	//버텍스 쉐이더
	std::unordered_map<std::string, VertexShader*> _vsMap;
	std::unordered_map<std::string, PixelShader*> _psMap;

	//오디오
	std::unordered_map<std::string, FMOD::Sound*> m_sound;


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

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState = nullptr;

};