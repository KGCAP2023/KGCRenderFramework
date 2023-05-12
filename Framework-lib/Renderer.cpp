#include "pch.h"
#include "Renderer.h"


void Renderer::Draw(const DirectX::XMMATRIX& viewProjectionMatrix)
{
	assert("해당 업데이트문은 Renderer 구현체가 필요합니다." && 0);
}

std::string Renderer::GetPath()
{
	assert("해당 업데이트문은 Renderer 구현체가 필요합니다." && 0);
	return "";
}

std::string Renderer::GetName()
{
	return std::string();
}

//const aiScene* Renderer::GetAiScene()
//{
//	assert("해당 업데이트문은 Renderer 구현체가 필요합니다." && 0);
//	return nullptr;
//}
