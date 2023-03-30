#include "pch.h"
#include "Shaders.h"


/*
예시: CompileShaderFromFile( L"Tutorial04.fx", "VS", "vs_4_0", &pVSBlob );
CompileShaderFromFile( L"BasicHLSL11_VS.hlsl", "VSMain", "vs_4_0_level_9_1", &vsBlob );
*/
HRESULT CompileShaderFromFile(std::wstring FileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	UINT dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob = nullptr ;
	hr = D3DCompileFromFile(FileName.c_str(), NULL, NULL, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, ppBlobOut, &pErrorBlob);

	if (FAILED(hr))
	{
		if (pErrorBlob != nullptr)
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		if (pErrorBlob) pErrorBlob->Release();
		return hr;
	}

	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
}


/*
쉐이더 컴파일 -> 인풋어셈블리 등록
*/
bool VertexShader::Initialize(std::wstring filename,
	std::string EntryPoint,
	std::string ShaderModel,
	D3D11_INPUT_ELEMENT_DESC* layoutDesc,
	UINT numElements,
	Microsoft::WRL::ComPtr<ID3D11Device>& device
	)
{
	//HRESULT hr = D3DReadFileToBlob(shaderpath.c_str(), this->shader_buffer.GetAddressOf());
	HRESULT hr = CompileShaderFromFile(filename, EntryPoint.c_str(), ShaderModel.c_str(), this->shader_buffer.GetAddressOf());

	if (FAILED(hr))
	{
		std::wstring errorMsg = L"[X] 버텍스 쉐이더를 컴파일 하는데 실패하였습니다 :-( ";
		errorMsg += filename;
		Logger::Log(hr, errorMsg);
		return false;
	}

	hr = device->CreateVertexShader(this->shader_buffer->GetBufferPointer(), this->shader_buffer->GetBufferSize(), NULL, this->shader.GetAddressOf());
	if (FAILED(hr))
	{
		std::wstring errorMsg = L"[X] 버텍스 쉐이더를 생성 하는데 실패하였습니다 :-( ";
		errorMsg += filename;
		Logger::Log(hr, errorMsg.c_str());
		return false;
	}

	hr = device->CreateInputLayout(layoutDesc, numElements, this->shader_buffer->GetBufferPointer(), this->shader_buffer->GetBufferSize(), this->inputLayout.GetAddressOf());
	if (FAILED(hr))
	{
		Logger::Log(hr, "[X] 버텍스 쉐이더의 인풋 레이아웃을 생성 하는데 실패하였습니다 :-( ");
		return false;
	}

	return true;
}

ID3D11VertexShader* VertexShader::GetShader()
{
	return this->shader.Get();
}

ID3D10Blob* VertexShader::GetBuffer()
{
	return this->shader_buffer.Get();
}

ID3D11InputLayout* VertexShader::GetInputLayout()
{
	return this->inputLayout.Get();
}

bool PixelShader::Initialize(std::wstring filename,
	std::string EntryPoint,
	std::string ShaderModel,
	Microsoft::WRL::ComPtr<ID3D11Device>& device
	)
{
	//HRESULT hr = D3DReadFileToBlob(shaderpath.c_str(), this->shader_buffer.GetAddressOf());
	HRESULT hr = CompileShaderFromFile(filename, EntryPoint.c_str(), ShaderModel.c_str(), this->shader_buffer.GetAddressOf());

	if (FAILED(hr))
	{
		std::wstring errorMsg = L"[X] 픽셀 쉐이더를 컴파일 하는데 실패하였습니다 :-( ";
		errorMsg += filename;
		Logger::Log(hr, errorMsg);
		return false;
	}

	hr = device->CreatePixelShader(this->shader_buffer.Get()->GetBufferPointer(), this->shader_buffer.Get()->GetBufferSize(), NULL, this->shader.GetAddressOf());
	if (FAILED(hr))
	{
		std::wstring errorMsg = L"[X] 픽셀 쉐이더를 생성 하는데 실패하였습니다 :-( ";
		errorMsg += filename;
		Logger::Log(hr, errorMsg);
		return false;
	}

	return true;
}

ID3D11PixelShader* PixelShader::GetShader()
{
	return this->shader.Get();
}

ID3D10Blob* PixelShader::GetBuffer()
{
	return this->shader_buffer.Get();
}

