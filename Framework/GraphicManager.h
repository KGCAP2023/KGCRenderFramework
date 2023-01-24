#pragma once
#include "pch.h"
#include <WICTextureLoader.h>
#include "Logger.h"
#include "GameObject.h";
#include "Shaders.h"
#include "Timer.h"
#include "Camera.h"
#include "ConstantBuffer.h"
#include "ConstantBufferType.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include "ModelRenderer.h"
#include "BoundingBoxRenderer.h"
#include "SkinnedMeshRenderer.h"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui_impl_dx11.h>
#include <windows.h>
#include <commdlg.h>
#include <shobjidl.h>
#include <Keyboard.h>
#include <Mouse.h>

class GraphicManager
{
public:

	bool Initialize(HWND hwnd, int width, int height, std::shared_ptr<DirectX::Keyboard> keyboard);
	void RenderFrame();


	//게임 오브젝트
	//SimpleRenderableObject* obj;
	//RenderableObject* obj3;
	GameObject* camera;
	Camera3D* cameraComponent;
	
	

private:
	bool InitializeDirectX(HWND hwnd);
	bool InitializeShaders();
	bool InitializeScene();

	bool openFile();
	bool openFile2();

	std::shared_ptr<DirectX::Keyboard> keyboard;

	std::string sSelectedFile;
	std::string sFilePath;

	std::string target;

	GameObject* CreateGameObject_1(const std::string& name, const std::string& path);
	GameObject* CreateGameObject_2(const std::string& name, const std::string& path);

	void SceneHierarchyWindow();
	bool doTreeNode(GameObject* obj, int index);
	Timer fps;

	//윈도우 크기
	int width;
	int height;

	//윈도우 핸들
	HWND hwnd;

	//쉐이더
	VertexShader vs_1;
	PixelShader ps_1;

	VertexShader vs_2;
	PixelShader ps_2;

	VertexShader vs_3;

	//상수버퍼
	ConstantBuffer<CB_VS_1> cb1;
	ConstantBuffer<CB_VS_2> cb2;

	ConstantBuffer<CB_VS_2> cb_skinning_1;
	ConstantBuffer<CB_VS_3> cb_skinning_2;

	ConstantBuffer<CB_PS_light> cb_light;

	//텍스쳐
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> grassTexture;


	//디바이스
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;

	//뎁스 스텐실
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;

	//레스터라이저
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState_CullFront;


	std::unique_ptr<SpriteBatch> spriteBatch;
	std::unique_ptr<SpriteFont> spriteFont;

};

