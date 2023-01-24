#pragma once
#include "pch.h"
#include "Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "ConstantBufferType.h"
#include "Texture.h"

class SimpleMesh
{
public:
	SimpleMesh(ID3D11Device* device,
		ID3D11DeviceContext* deviceContext,
		std::vector<SimpleVertex>& vertices,
		std::vector<DWORD>& indices);
	SimpleMesh(const SimpleMesh& mesh);
	SimpleMesh() {}
	void Draw();

private:

	//버텍스 버퍼
	VertexBuffer<SimpleVertex> vertexbuffer;
	//인덱스 버퍼
	IndexBuffer indexbuffer;
	//컨텍스트
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;

	//텍스쳐
	std::vector<Texture> textures;

};


