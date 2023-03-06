#pragma once
#include "pch.h"


class VertexLayout
{
private:
	std::vector<D3D11_INPUT_ELEMENT_DESC> layout;
public:

	VertexLayout()
	{
	}

	void add(std::string SemanticName, DXGI_FORMAT Format)
	{
		if(layout.empty())
			layout.push_back(D3D11_INPUT_ELEMENT_DESC{
			SemanticName.c_str(),
			0,
			Format ,
			0,
			0,
			D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,
			0 });
		else
			layout.push_back(D3D11_INPUT_ELEMENT_DESC{
			SemanticName.c_str(),
			0,
			Format ,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,
			0 });
	}

	UINT GetArraySize()
	{
		return layout.size(); 
	}

	D3D11_INPUT_ELEMENT_DESC* getArray()
	{
		return layout.data();
	}

};

