#pragma once
#include "pch.h"
#include <WICTextureLoader.h>
#include "Logger.h"
#include "GameObject.h";
#include "Shaders.h"
#include "Timer.h"
#include "Camera.h"
#include "ModelRenderer.h"
#include "BoundingBoxRenderer.h"
#include "SkinnedMeshRenderer.h"
#include "SpriteRenderer.h"
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <windows.h>
#include <commdlg.h>
#include <shobjidl.h>
#include <Keyboard.h>
#include <Mouse.h>
#include <SimpleMath.h>
#include "GameObjectManager.h"


class Framework;
class ResourceManager;

class GraphicManager
{
public:

	bool Initialize(Framework* framework,HWND hwnd, int width, int height);
	void RenderFrame();

	//���� ������Ʈ
	//SimpleRenderableObject* obj;
	//RenderableObject* obj3;
	GameObject* camera;
	Camera3D* cameraComponent;

	Framework* framework;
	ResourceManager* res;

	//����̽�
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;

	//���� ���ٽ�
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;

	//������ ũ��
	int width;
	int height;

	//����� �ؽ�ó
	ID3D11Texture2D* refTex;
	ID3D11ShaderResourceView* refRes;

private:
	bool InitializeDirectX(HWND hwnd);
	bool InitializeShaders();
	bool InitializeScene();

	bool openFile();
	bool openFile2();

	ImGuiIO* io_;

	std::shared_ptr<DirectX::Keyboard> keyboard;

	std::string sSelectedFile;
	std::string sFilePath;

	std::string target;

	GameObject* CreateGameObject_1(const std::string& name, const std::string& path);
	GameObject* CreateGameObject_2(const std::string& name, const std::string& path);

	void SceneHierarchyWindow();
	bool doTreeNode(GameObject* obj, int index);
	Timer fps;



	//������ �ڵ�
	HWND hwnd;

	//�ؽ���
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> grassTexture;

	//�����Ͷ�����
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState_CullFront;

	//����
	Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;


	//������Ʈ �޴���
	GameObjectManager* gameObjectManager;

};

