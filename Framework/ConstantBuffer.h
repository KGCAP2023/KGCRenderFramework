#pragma once
#include "pch.h"
#include "Logger.h"

template<class T>
class ConstantBuffer 
{
private:
	ConstantBuffer(const ConstantBuffer<T>& rhs);

	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	ID3D11DeviceContext* deviceContext = nullptr;

public:
	ConstantBuffer() {}
	T data;

	ID3D11Buffer* Get()
	{
		
		return buffer.Get();
	}

	ID3D11Buffer** GetAddressOf()
	{
		return buffer.GetAddressOf();
	}

	HRESULT Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	{
		
		this->deviceContext = deviceContext;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd,sizeof(bd));
		//bd.Usage = D3D11_USAGE_DEFAULT;
		//bd.CPUAccessFlags = 0;

		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		//bd.ByteWidth = sizeof(T);
		bd.ByteWidth = static_cast<UINT>(sizeof(T) + (16 - (sizeof(T) % 16)));
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		
		HRESULT hr = device->CreateBuffer(&bd, NULL, this->buffer.GetAddressOf());

		


		return hr;
	}


	bool ApplyChanges()
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		ZeroMemory(&mappedResource,sizeof(D3D11_MAPPED_SUBRESOURCE));

		HRESULT hr = this->deviceContext->Map(this->buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(hr))
		{
			Logger::Log(hr, "Failed to map constant buffer.");
			return false;
		}
		CopyMemory(mappedResource.pData, &data, sizeof(T));
		this->deviceContext->Unmap(buffer.Get(), 0);
		return true;
	}

};
