#include "pch.h"
#include "Matrix.h"


void Matrix::print(const DirectX::XMMATRIX& matrix)
{
	std::cout << "==================================" << std::endl;
	for (int i = 0; i < 4; i++)
	{
		DirectX::XMFLOAT4 v;
		DirectX::XMStoreFloat4(&v, matrix.r[i]);
		std::cout << " " << v.x << " | " << v.y << " | " << v.z << " | " << v.w << std::endl;
	}
	std::cout << "==================================" << std::endl;
}
