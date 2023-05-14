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
	/// ��������Ʈ�� ���ҽ� �Ŵ����� �ε��մϴ�.
	/// ���н� nullptr�� ��ȯ�մϴ�.
	/// </summary>
	/// <param name="name">����� �̸�</param>
	/// <param name="spritePath">���� ���</param>
	/// <returns>��������Ʈ</returns>
	virtual Sprite* LoadSprite(const std::string& name, const std::string& spritePath) { return nullptr; };
	/// <summary>
	/// ��������Ʈ�� ã���ϴ�.
	/// �������� ������ nullptr�� ��ȯ�մϴ�.
	/// </summary>
	/// <param name="name">ã�� �̸�</param>
	/// <returns>����Ʈ����Ʈ</returns>
	virtual Sprite* FindSprite(const std::string& name) { return nullptr; };
	/// <summary>
	/// ���ؽ� ���̴��� �ε��մϴ�.
	/// </summary>
	/// <param name="name">�̸�</param>
	/// <param name="path">���</param>
	/// <param name="layout">���ؽ� ���̾ƿ�</param>
	/// <param name="numElements2D">���ؽ� ���̾ƿ� ũ��</param>
	/// <returns>��������</returns>
	virtual bool LoadVertexShader(const std::string& name, const std::wstring& path, D3D11_INPUT_ELEMENT_DESC* layout, UINT numElements2D) { return false; };
	/// <summary>
	/// �ȼ����̴��� �ε��մϴ�.
	/// </summary>
	/// <param name="name">�̸�</param>
	/// <param name="path">���</param>
	/// <returns>��������</returns>
	virtual bool LoadPixelShader(const std::string& name, const std::wstring& path) { return false; };
	/// <summary>
	/// ���ؽ� ���̴��� ã���ϴ�.
	/// ��������
	/// </summary>
	/// <param name="key">ã�� �̸�</param>
	/// <returns>���ؽ����̴�</returns>
	virtual VertexShader* FindVertexShader(const std::string& key) { return nullptr; };
	/// <summary>
	/// �ȼ� ���̴��� ã���ϴ�.
	/// �������� ������ nullptr�� ��ȯ�մϴ�.
	/// </summary>
	/// <param name="key">ã�� �̸�</param>
	/// <returns>���ؽ����̴�</returns>
	virtual PixelShader* FindPixelShader(const std::string& key) { return nullptr; };
	/// <summary>
	/// Ÿ�ϸ��� ���ҽ��Ŵ����� ����մϴ�.
	/// </summary>
	/// <param name="name">ã�� �̸�</param>
	/// <param name="tileMap">Ÿ�ϸ�</param>
	virtual void RegisterTileMap(const std::string& name, TileMap* tileMap) {};
	/// <summary>
	/// Ÿ�ϸ��� ã���ϴ�.
	/// </summary>
	/// <param name="name">ã�� �̸�</param>
	/// <returns>Ÿ�ϸ�</returns>
	virtual TileMap* FindTileMap(const std::string& name) { return nullptr; };
	/// <summary>
	/// �Ϲ� �𵨸� ������ �ε��մϴ�. 
	/// </summary>
	/// <param name="modelName"></param>
	/// <param name="filePath"></param>
	/// <returns></returns>
	virtual bool LoadModel(const std::string& modelName, const std::string& filePath) { return false; };
	/// <summary>
	/// ��Ű�� ��(�ִϸ��̼��� �ִ� ��)�� �ε��մϴ�.
	/// </summary>
	/// <param name="modelName"></param>
	/// <param name="filePath"></param>
	/// <returns></returns>
	virtual bool LoadSkinnedModel(const std::string& modelName, const std::string& filePath) { return false; };
	/// <summary>
	/// ��Ű�� ���� �����ɴϴ�. (Clone)
	/// </summary>
	/// <param name="modelName"></param>
	/// <returns></returns>
	virtual SkinnedMesh* FindSkinnedModel(const std::string& modelName) { return nullptr; };
	/// <summary>
	/// �Ϲ� ���� �����ɴϴ�. (Clone)
	/// </summary>
	/// <param name="modelName"></param>
	/// <returns></returns>
	virtual Model* FindModel(const std::string& modelName) { return nullptr; };
	/// <summary>
	/// �𵨿� ĳ�̵Ǿ��ִ� �ؽ��ĸ� �����ɴϴ�.
	/// </summary>
	/// <param name="modelName"></param>
	/// <returns></returns>
	virtual std::vector<Texture>* GetCachedTexture(const std::string& modelName) { return nullptr; };
	/// <summary>
	/// �𵨿� ĳ�̵Ǿ��ִ� ��� ������ �����ɴϴ�.
	/// </summary>
	/// <param name="modelName"></param>
	/// <returns></returns>
	virtual std::vector<SimpleVertex>* GetCachedBBOXVertices(const std::string& modelName) { return nullptr; };
	/// <summary>
	/// ��������Ʈ ���� ��ȸ�մϴ�.
	/// </summary>
	/// <param name="callback"></param>
	virtual void SpriteForeach(std::function<void(Sprite*)> callback) {};
	/// <summary>
	/// ��������Ʈ ���� �����ɴϴ�.
	/// </summary>
	/// <returns></returns>
	virtual std::unordered_map<std::string, Sprite*> GetSpriteMap() { return std::unordered_map<std::string, Sprite*>(); };
	/// <summary>
	/// Ÿ�� ���� ��ȸ�մϴ�.
	/// </summary>
	/// <param name="callback"></param>
	virtual void TileMapForeach(std::function<void(TileMap*)> callback) {};
	/// <summary>
	/// Ÿ�� ���� �����ɴϴ�.
	/// </summary>
	/// <returns></returns>
	virtual std::unordered_map<std::string, TileMap*> GetTileMap() { return std::unordered_map<std::string, TileMap*>(); };
	/// <summary>
	/// ����� ������ �ҷ��� map�� �����մϴ�.
	/// </summary>
	/// <param name="audioName">map�� ����� �̸��Դϴ�.</param>
	/// <param name="audioFilePath">����� ������ ����Դϴ�.</param>
	virtual void LoadAudio(const char* audioName, const char* audioFilePath) {};
	/// <summary>
	/// �ҷ��� ������� �����մϴ�.
	/// </summary>
	/// <param name="audioName">������ ����� �̸��Դϴ�.</param>
	virtual void DeleteAudio(const char* audioName) {};
	/// <summary>
	/// ����� ���� �����ɴϴ�.
	/// </summary>
	/// <returns></returns>
	virtual std::unordered_map<std::string, FMOD::Sound*> GetAudioMap() { return std::unordered_map<std::string, FMOD::Sound*>(); }
	/// <summary>
	/// ���� ��ȸ�մϴ�.
	/// </summary>
	/// <param name="callback"></param>
	virtual void ModelForeach(std::function<void(Model*)> callback) {};
	/// <summary>
	/// �𵨸��� ��ȯ�մϴ�.
	/// </summary>
	/// <returns></returns>
	virtual std::unordered_map<std::string, Model*> GetModelMap() { return std::unordered_map<std::string, Model*>(); };
	/// <summary>
	/// �ҷ��� ��������Ʈ�� �����մϴ�.
	/// </summary>
	/// <param name="audioName">������ ��������Ʈ �̸��Դϴ�.</param>
	virtual void DeleteSprite(const char* Name) {};
	/// <summary>
	/// �ҷ��� Ÿ���� �����մϴ�.
	/// </summary>
	/// <param name="audioName">������ Ÿ�� �̸��Դϴ�.</param>
	virtual void DeleteTile(const char* Name) {};
	/// <summary>
	/// �ҷ��� �𵨸� �����մϴ�.
	/// </summary>
	/// <param name="audioName">������ �� �̸��Դϴ�.</param>
	virtual void DeleteModel(const char* Name) {};
	/// <summary>
	/// �ҷ��� ��Ű�� ���� �����մϴ�.
	/// </summary>
	/// <param name="audioName">������ ��Ű�� �� �̸��Դϴ�.</param>
	virtual void DeleteSkinnedModel(const char* Name) {};
	/// <summary>
	/// �ҷ��� �ؽ��ĸ� �����մϴ�.
	/// </summary>
	/// <param name="audioName">������ �ؽ��� �̸��Դϴ�.</param>
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

	

	//��������Ʈ
	std::unordered_map<std::string, Sprite*> _spriteMap;
	std::unordered_map<std::string, TileMap*> _TileMap;

	//��
	std::unordered_map<std::string, Model*> _modelMap;
	//��Ű�� ��
	std::unordered_map<std::string, SkinnedMesh*> _skinnedMap;
	//�ؽ���
	std::unordered_map<std::string, std::vector<Texture>*> _textures_loaded;
	std::unordered_map<std::string, std::vector<SimpleVertex>*> _bbox_vertices;

	//���ؽ� ���̴�
	std::unordered_map<std::string, VertexShader*> _vsMap;
	std::unordered_map<std::string, PixelShader*> _psMap;

	//�����
	std::unordered_map<std::string, FMOD::Sound*> m_sound;


	//�������
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