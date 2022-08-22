#pragma once
#include "..\pch.h"


class IndexBuffer
{
private:
	IndexBuffer(const IndexBuffer& rhs);
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	UINT indexCount;
public:
	IndexBuffer() {}

	UINT IndexCount() const
	{
		return this->indexCount;
	}

	ID3D11Buffer* Get() const
	{

		return buffer.Get();
	}

	ID3D11Buffer** GetAddressOf()
	{
		return buffer.GetAddressOf();
	}

	HRESULT Init(DWORD* indices, UINT count ,ID3D11Device* device)
	{

		indexCount = count;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(DWORD) * count;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = indices;

		HRESULT hr = device->CreateBuffer(&bd, &InitData, buffer.GetAddressOf());

		return hr;
	}


};

