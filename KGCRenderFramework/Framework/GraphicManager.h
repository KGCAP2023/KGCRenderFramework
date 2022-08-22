#pragma once
#include "..\pch.h"
#include <WICTextureLoader.h>
#include "Logger.h"
#include "SimpleRenderableObject.h";
#include "RenderableObject.h";
#include "Shaders.h"
#include "Timer.h"
#include "Camera.h"
#include "ConstantBuffer.h"
#include "ConstantBufferType.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>


class GraphicManager
{
public:

	bool Initialize(HWND hwnd, int width, int height);
	void RenderFrame();


	//���� ������Ʈ
	SimpleRenderableObject* obj;
	RenderableObject* obj3;
	Camera3D camera;
	


private:
	bool InitializeDirectX(HWND hwnd);
	bool InitializeShaders();
	bool InitializeScene();

	Timer fps;

	//������ ũ��
	int width;
	int height;

	//���̴�
	VertexShader vs_1;
	PixelShader ps_1;

	VertexShader vs_2;
	PixelShader ps_2;


	//�������
	ConstantBuffer<CB_VS_1> cb1;
	ConstantBuffer<CB_VS_2> cb2;
	ConstantBuffer<CB_PS_light> cb_light;

	//�ؽ���
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> grassTexture;


	//����̽�
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;

	//���� ���ٽ�
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;

	//�����Ͷ�����
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState_CullFront;


	std::unique_ptr<SpriteBatch> spriteBatch;
	std::unique_ptr<SpriteFont> spriteFont;

};

