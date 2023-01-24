#pragma once
#include "pch.h"
#include "Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "ConstantBufferType.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "Texture.h"
     

class Mesh
{
public:
	Mesh(ID3D11Device* device,
		ID3D11DeviceContext* deviceContext,
		std::vector<Vertex3D>& vertices,
		std::vector<DWORD>& indices,
		std::vector<Texture>& textures,
		const DirectX::XMMATRIX& transformMatrix);
	Mesh(const Mesh& mesh);
	void Draw();
	const DirectX::XMMATRIX& GetTransformMatrix();

	void SetTransformMatrix(const DirectX::XMMATRIX& m);
	
private:

	//버텍스 버퍼
	VertexBuffer<Vertex3D> vertexbuffer;
	//인덱스 버퍼
	IndexBuffer indexbuffer;
	//컨텍스트
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;

	//텍스쳐
	std::vector<Texture> textures;

	DirectX::XMMATRIX transformMatrix;

};

