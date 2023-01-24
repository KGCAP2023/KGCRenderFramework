#pragma once
#include <DirectXMath.h>

struct Vertex2D
{
	Vertex2D() {}
	Vertex2D(float x, float y, float z, float u, float v)
		: pos(x, y, z), texCoord(u, v) {}

	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 texCoord;
};

struct Vertex3D
{
	Vertex3D() {}
	Vertex3D(float x, float y, float z, float u, float v, float nx, float ny, float nz)
		: pos(x, y, z), texCoord(u, v), normal(nx, ny, nz) {}

	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 texCoord;
	DirectX::XMFLOAT3 normal;
};

struct Vertex3Dw
{
	Vertex3Dw() {}
	Vertex3Dw(float x, float y, float z, float u, float v, float nx, float ny, float nz)
		: pos(x, y, z), texCoord(u, v), normal(nx, ny, nz) {}
	Vertex3Dw(float x, float y, float z, float u, float v, float nx, float ny, float nz,
		float id1, float id2, float id3, float id4, float w1, float w2, float w3,float w4)
		: pos(x, y, z), texCoord(u, v), normal(nx, ny, nz) , blendBoneIDs(id1,id2,id3,id4), blendWeights(w1,w2,w3,w4) {}

	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 texCoord;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT4 blendBoneIDs;
	DirectX::XMFLOAT4 blendWeights;
};

struct SimpleVertex
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT4 Color;
};

