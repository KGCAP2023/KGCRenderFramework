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

	//��������Ʈ
	std::unordered_map<std::string, Sprite*> _spriteMap;
	std::unordered_map<std::string, TileMap*> _TileMap;

	//���ؽ� ���̴�
	std::unordered_map<std::string, VertexShader*> _vsMap;
	std::unordered_map<std::string, PixelShader*> _psMap;

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

};