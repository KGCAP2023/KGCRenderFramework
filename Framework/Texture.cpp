#include "pch.h"
#include "Texture.h"


Texture::Texture(ID3D11Device* device, const std::string& filePath)
{

}

Texture::Texture()
{
}

Texture::~Texture()
{
}

ID3D11ShaderResourceView** Texture::GetTextureResourceViewAddress()
{
    return this->texture.GetAddressOf();
}
