#pragma once
#include "..\pch.h"

class VertexShader
{
public:
	bool Initialize(std::wstring filename, //������ġ 
		std::string EntryPoint,	//���̴� ������ �̸�
		std::string ShaderModel,//���̴� �� 
		D3D11_INPUT_ELEMENT_DESC* layoutDesc, //���̴��� ������ ��ǲ ���̾ƿ� ���� 
		UINT numElements, //��ǲ ���̾ƿ� ũ�� 
		Microsoft::WRL::ComPtr<ID3D11Device>& device
		);

	ID3D11VertexShader* GetShader();
	ID3D10Blob* GetBuffer();
	ID3D11InputLayout* GetInputLayout();
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> shader;
	Microsoft::WRL::ComPtr<ID3D10Blob> shader_buffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;

};
 

class PixelShader
{
public:
	bool Initialize(std::wstring filename,
		std::string EntryPoint,
		std::string ShaderModel,
		Microsoft::WRL::ComPtr<ID3D11Device>& device
	);
	ID3D11PixelShader* GetShader();
	ID3D10Blob* GetBuffer();
private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> shader;
	Microsoft::WRL::ComPtr<ID3D10Blob> shader_buffer;
};
