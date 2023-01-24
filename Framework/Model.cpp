#include "Model.h"

bool Model::Init(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_2>& constantBuffer, VertexShader* vertexShader, PixelShader* pixelShader)
{
	this->device = device;
	this->deviceContext = deviceContext;
	this->constantBuffer = &constantBuffer;
	this->filePath = filePath;
	this->vertexShader = vertexShader;
	this->pixelShader = pixelShader;

	try
	{
		if (!this->loadModel(filePath))
			return false;
	}
	catch (...)
	{
		Logger::Log(L"로드 실패");
		return false;
	}

	return true;
}

void Model::Draw(const XMMATRIX& worldMatrix, const XMMATRIX& viewProjectionMatrix)
{

	this->deviceContext->VSSetShader(this->vertexShader->GetShader(), NULL, 0);
	this->deviceContext->PSSetShader(this->pixelShader->GetShader(), NULL, 0);

	this->deviceContext->VSSetConstantBuffers(0, 1, this->constantBuffer->GetAddressOf());
	for (size_t i = 0; i < meshes.size(); ++i) {
		this->constantBuffer->data.WVP = XMMatrixTranspose(meshes[i].GetTransformMatrix() * worldMatrix * viewProjectionMatrix); //Calculate World-View-Projection Matrix
		this->constantBuffer->data.W = XMMatrixTranspose(meshes[i].GetTransformMatrix() * worldMatrix); //Calculate World
		this->constantBuffer->ApplyChanges();
		meshes[i].Draw();
	}
}

std::vector<Mesh>& Model::GetMeshes()
{
	return this->meshes;
}

const aiScene* Model::getAiScene()
{
	return this->pScene;
}

std::string Model::GetPath()
{
	return this->filePath;
}

bool Model::loadModel(const std::string & path)
{
	pScene = Importer.ReadFile(path,
		aiProcess_Triangulate |
		aiProcess_ConvertToLeftHanded);

	if (pScene == nullptr)
		return false;

	this->directory = path.substr(0, path.find_last_of("/\\"));

	processNode(pScene->mRootNode, pScene, DirectX::XMMatrixIdentity());

	return true;
}

void Model::processNode(aiNode* node, const aiScene* scene, const XMMATRIX& parentTransformMatrix)
{
	//부모 노드 기준 좌표계로 변환
	XMMATRIX nodeTransformMatrix = XMMatrixTranspose(XMMATRIX(&node->mTransformation.a1)) * parentTransformMatrix;
	
	//해당 노드의 메시를 순회 
	for (UINT i = 0; i < node->mNumMeshes; i++) {
		
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		std::cout << node->mName.C_Str() << std::endl;
		
		//Matrix::print(nodeTransformMatrix);
		meshes.push_back(this->processMesh(mesh, scene, nodeTransformMatrix));
	}

	for (UINT i = 0; i < node->mNumChildren; i++) {
		this->processNode(node->mChildren[i], scene, nodeTransformMatrix);
	}
}


Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene, const XMMATRIX& transformMatrix)
{
	
	// Data to fill
	std::vector<Vertex3D> vertices;
	std::vector<DWORD> indices;
	std::vector<Texture> textures;

	// Walk through each of the mesh's vertices
	for (UINT i = 0; i < mesh->mNumVertices; i++) {
		Vertex3D vertex;

		vertex.pos.x = mesh->mVertices[i].x;
		vertex.pos.y = mesh->mVertices[i].y;
		vertex.pos.z = mesh->mVertices[i].z;
		
		vertex.normal.x = mesh->mNormals[i].x;
		vertex.normal.y = mesh->mNormals[i].y;
		vertex.normal.z = mesh->mNormals[i].z;

		if (mesh->mTextureCoords[0]) {
			vertex.texCoord.x = (float)mesh->mTextureCoords[0][i].x;
			vertex.texCoord.y = (float)mesh->mTextureCoords[0][i].y;
		}

		vertices.push_back(vertex);
	}
	 
	for (UINT i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse",scene);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	}

	return Mesh(this->device, this->deviceContext, vertices, indices, textures, transformMatrix);
}

static std::wstring Utf8ToWide(const char* in) {
	int size = MultiByteToWideChar(CP_UTF8, 0, in, -1, nullptr, 0);
	// size includes terminating null; std::wstring adds null automatically
	std::wstring out(static_cast<size_t>(size) - 1, L'\0');
	MultiByteToWideChar(CP_UTF8, 0, in, -1, &out[0], size);
	return out;
}

std::wstring getFileExtension(std::wstring& path)
{
	size_t index = path.rfind(L".");

	if (index != std::wstring::npos) {

		return path.substr(index + 1, path.length() - index);
	}

	return std::wstring();
}


//텍스쳐를 로드 합니다.
std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const aiScene* scene)
{
	std::vector<Texture> textures;

	for (UINT i = 0; i < mat->GetTextureCount(type); i++) {

		aiString str;
		mat->GetTexture(type, i, &str);
		// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;

		for (UINT j = 0; j < textures_loaded_.size(); j++) {
			if (std::strcmp(textures_loaded_[j].path.c_str(), str.C_Str()) == 0) {
				textures.push_back(textures_loaded_[j]);
				skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip) {   // If texture hasn't been loaded already, load it
			HRESULT hr;
			Texture texture;

			const aiTexture* embeddedTexture = scene->GetEmbeddedTexture(str.C_Str());
			if (embeddedTexture != nullptr) {
				texture.texture = loadEmbeddedTexture(embeddedTexture);
			}
			else {

				std::cout << "Texture: " << directory + '/' + std::string(str.C_Str()) << std::endl;
				std::wstring filename = std::wstring (directory.begin(), directory.end()) + L'/' + Utf8ToWide(str.C_Str());

				std::wstring ext = getFileExtension(filename);
				//std::wcout << ext << std::endl;
				DirectX::ScratchImage image;
				if (ext._Equal(L"dds") || ext._Equal(L"DDS"))
				{
					hr = DirectX::LoadFromDDSFile(filename.c_str(), DirectX::DDS_FLAGS_NONE, NULL, image);
					
				}
				else if (ext._Equal(L"tga") || ext._Equal(L"TGA"))
				{
					hr = DirectX::LoadFromTGAFile(filename.c_str(), NULL, image);
				}
				else
				{
					hr = DirectX::LoadFromWICFile(filename.c_str(), WIC_FLAGS_NONE, nullptr, image);
					//hr = DirectX::CreateWICTextureFromFile(this->device, this->deviceContext, filename.c_str(), nullptr, &texture.texture);
				}

				if (FAILED(hr))
					Logger::Log(hr, L"Texture couldn't be loaded");
				else
				{
					hr = CreateShaderResourceView(device,
						image.GetImages(), image.GetImageCount(),
						image.GetMetadata(), &texture.texture);
				}
				
			}
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			this->textures_loaded_.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}
	return textures;
}



//자체 내장된 텍스쳐를 로드 한다. 
ID3D11ShaderResourceView* Model::loadEmbeddedTexture(const aiTexture* embeddedTexture) {
	HRESULT hr;
	ID3D11ShaderResourceView* texture = nullptr;

	if (embeddedTexture->mHeight != 0) {
		// Load an uncompressed ARGB8888 embedded texture
		D3D11_TEXTURE2D_DESC desc;
		desc.Width = embeddedTexture->mWidth;
		desc.Height = embeddedTexture->mHeight;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA subresourceData;
		subresourceData.pSysMem = embeddedTexture->pcData;
		subresourceData.SysMemPitch = embeddedTexture->mWidth * 4;
		subresourceData.SysMemSlicePitch = embeddedTexture->mWidth * embeddedTexture->mHeight * 4;

		ID3D11Texture2D* texture2D = nullptr;
		hr = this->device->CreateTexture2D(&desc, &subresourceData, &texture2D);


		if (FAILED(hr))
			Logger::Log(hr,L"CreateTexture2D failed!");

		hr = this->device->CreateShaderResourceView(texture2D, nullptr, &texture);


		if (FAILED(hr))
			Logger::Log(hr, L"CreateShaderResourceView failed!");

		return texture;
	}

	// mHeight is 0, so try to load a compressed texture of mWidth bytes
	const size_t size = embeddedTexture->mWidth;

	hr = CreateWICTextureFromMemory(this->device, this->deviceContext, reinterpret_cast<const unsigned char*>(embeddedTexture->pcData), size, nullptr, &texture);

	if (FAILED(hr))
		Logger::Log(hr, L"Texture couldn't be created from memory!");

	return texture;
}