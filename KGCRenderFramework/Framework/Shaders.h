#pragma once
#include "..\pch.h"

class VertexShader
{
public:
	bool Initialize(std::wstring filename, //파일위치 
		std::string EntryPoint,	//쉐이더 진입점 이름
		std::string ShaderModel,//쉐이더 모델 
		D3D11_INPUT_ELEMENT_DESC* layoutDesc, //쉐이더에 전달할 인풋 레이아웃 정의 
		UINT numElements, //인풋 레이아웃 크기 
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
