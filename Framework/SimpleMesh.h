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

	//���ؽ� ����
	VertexBuffer<SimpleVertex> vertexbuffer;
	//�ε��� ����
	IndexBuffer indexbuffer;
	//���ؽ�Ʈ
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;

	//�ؽ���
	std::vector<Texture> textures;

};


