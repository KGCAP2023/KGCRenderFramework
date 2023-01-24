#include "SkinnedMesh.h"

SkinnedMesh::~SkinnedMesh()
{
	Clear();
}

void SkinnedMesh::Clear()
{

}

bool SkinnedMesh::LoadMesh(
	const std::string& Filename,
	ID3D11Device* device,
	ID3D11DeviceContext* deviceContext,
	ConstantBuffer<CB_VS_2>& cb_vs_vertexshader,
	ConstantBuffer<CB_VS_3>& cb_vs_vertexshader2,
	VertexShader* vertexShader,
	PixelShader* pixelShader)
{
	Clear(); //클리어 

		//상수 버퍼
	this->constantBuffer = &cb_vs_vertexshader;
	this->constantBuffer2 = &cb_vs_vertexshader2;

	this->vertexShader = vertexShader;
	this->pixelShader = pixelShader;

	//컨텍스트
	this->deviceContext = deviceContext;
	this->device = device;

	this->filePath = Filename;

	bool Ret = false;

	pScene = Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate |
	aiProcess_ConvertToLeftHanded | aiProcess_OptimizeGraph);

	if (pScene) {
		aiMatrix4x4 temp = pScene->mRootNode->mTransformation;
		temp = temp.Inverse();

		m_GlobalInverseTransform = DirectX::XMMatrixTranspose(XMMATRIX(&temp.a1));
		Ret = InitFromScene(pScene, Filename);
	}
	else {
		printf("Error parsing '%s': '%s'\n", Filename.c_str(), Importer.GetErrorString());
	}

	return Ret;
}

bool SkinnedMesh::InitFromScene(const aiScene* pScene, const std::string& Filename)
{

	this->directory = Filename.substr(0, Filename.find_last_of("/\\"));

	m_Meshes.resize(pScene->mNumMeshes);

	unsigned int NumVertices = 0;
	unsigned int NumIndices = 0;

	CountVerticesAndIndices(pScene, NumVertices, NumIndices);

	ReserveSpace(NumVertices, NumIndices);

	InitAllMeshes(pScene);

	InitAnimations(pScene);

	std::cout <<"Bone size: " << m_BoneInfo.size() << std::endl;

	//if (!InitMaterials(pScene, Filename)) {
	//	return false;
	//}
	InitMaterials(pScene);

	PopulateBuffers();

	return true;
}

void SkinnedMesh::CountVerticesAndIndices(const aiScene* pScene, unsigned int& NumVertices, unsigned int& NumIndices)
{
	for (unsigned int i = 0; i < m_Meshes.size(); i++) {
		m_Meshes[i].MaterialIndex = pScene->mMeshes[i]->mMaterialIndex;
		m_Meshes[i].NumIndices = pScene->mMeshes[i]->mNumFaces * 3;
		m_Meshes[i].BaseVertex = NumVertices;
		m_Meshes[i].BaseIndex = NumIndices;

		NumVertices += pScene->mMeshes[i]->mNumVertices;
		NumIndices += m_Meshes[i].NumIndices;
	}
}

void SkinnedMesh::ReserveSpace(unsigned int NumVertices, unsigned int NumIndices)
{
	m_vertices.reserve(NumVertices);
	m_indices.reserve(NumIndices);
	m_Bones.resize(NumVertices);
}

void SkinnedMesh::InitAnimations(const aiScene* pScene)
{
	if (pScene->HasAnimations())
	{
		for (UINT i = 0; i < pScene->mNumAnimations; i++)
		{
			this->animations.push_back(pScene->mAnimations[i]);
		}
	}
}


void SkinnedMesh::InitAllMeshes(const aiScene* pScene)
{
	for (unsigned int i = 0; i < m_Meshes.size(); i++) {
		const aiMesh* paiMesh = pScene->mMeshes[i];
		InitSingleMesh(i, paiMesh);
	}

	for (unsigned int i = 0; i < m_Bones.size(); i++) {
		VertexBoneData data = m_Bones[i];
		m_vertices[i].blendBoneIDs = XMFLOAT4(data.BoneIDs[0], data.BoneIDs[1], data.BoneIDs[2], data.BoneIDs[3]);
		m_vertices[i].blendWeights = XMFLOAT4(data.Weights[0], data.Weights[1], data.Weights[2], data.Weights[3]);
		//std::cout << "가중치" << data.Weights[0] << "/" << data.Weights[1] << "/" << data.Weights[2] << "/" << data.Weights[3] << std::endl;
	}
}

bool SkinnedMesh::InitMaterials(const aiScene* pScene)
{
	std::string Dir = this->directory;

	bool Ret = true;

	printf("Num materials: %d\n", pScene->mNumMaterials);

	m_textures.reserve(pScene->mNumMaterials);

	// Initialize the materials
	for (unsigned int i = 0; i < pScene->mNumMaterials; i++) {
		aiMaterial* pMaterial = pScene->mMaterials[i];
		std::vector<Texture> diffuseMaps = this->loadMaterialTextures(pMaterial, aiTextureType_DIFFUSE, "texture_diffuse", pScene);

		//m_textures.insert(m_textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::cout << i << "texture 사이즈" << diffuseMaps.size() << std::endl;

		if (diffuseMaps.size() == 0)
		{
			Texture tex{};
			m_textures.emplace_back(tex);
		}
		else
			m_textures.emplace_back(diffuseMaps[0]);

	}

	return Ret;
}


void SkinnedMesh::InitSingleMesh(UINT MeshIndex, const aiMesh* paiMesh)
{

	// Populate the vertex attribute vectors
	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {

		Vertex3Dw vertex;

		vertex.pos.x = paiMesh->mVertices[i].x;
		vertex.pos.y = paiMesh->mVertices[i].y;
		vertex.pos.z = paiMesh->mVertices[i].z;

		if (paiMesh->HasNormals())
		{
			vertex.normal.x = paiMesh->mNormals[i].x;
			vertex.normal.y = paiMesh->mNormals[i].y;
			vertex.normal.z = paiMesh->mNormals[i].z;
		}

		if (paiMesh->mTextureCoords[0]) {
			vertex.texCoord.x = (float)paiMesh->mTextureCoords[0][i].x;
			vertex.texCoord.y = (float)paiMesh->mTextureCoords[0][i].y;
		}

		m_vertices.push_back(vertex);
	}

	LoadMeshBones(MeshIndex, paiMesh);

	// Populate the index buffer
	for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) {
		const aiFace& Face = paiMesh->mFaces[i];
		//        printf("num indices %d\n", Face.mNumIndices);
		//        assert(Face.mNumIndices == 3);
		m_indices.push_back(Face.mIndices[0]);
		m_indices.push_back(Face.mIndices[1]);
		m_indices.push_back(Face.mIndices[2]);
	}

}

void SkinnedMesh::LoadMeshBones(UINT MeshIndex, const aiMesh* paiMesh)
{
	for (UINT i = 0; i < paiMesh->mNumBones; i++) {
		LoadSingleBone(MeshIndex, paiMesh->mBones[i]);
	}
}

void SkinnedMesh::LoadSingleBone(UINT MeshIndex, const aiBone* pBone)
{
	int BoneId = GetBoneId(pBone);

	if (BoneId == m_BoneInfo.size()) {
		BoneInfo bi(XMMatrixTranspose(XMMATRIX(&pBone->mOffsetMatrix.a1)));
		m_BoneInfo.push_back(bi);
	}

	for (UINT i = 0; i < pBone->mNumWeights; i++) {
		const aiVertexWeight& vw = pBone->mWeights[i];
		UINT GlobalVertexID = m_Meshes[MeshIndex].BaseVertex + pBone->mWeights[i].mVertexId;
		m_Bones[GlobalVertexID].AddBoneData(BoneId, vw.mWeight);
	}
}

int SkinnedMesh::GetBoneId(const aiBone* pBone)
{
	int BoneIndex = 0;
	std::string BoneName(pBone->mName.C_Str());

	if (m_BoneNameToIndexMap.find(BoneName) == m_BoneNameToIndexMap.end()) {
		// Allocate an index for a new bone
		BoneIndex = (int)m_BoneNameToIndexMap.size();
		m_BoneNameToIndexMap[BoneName] = BoneIndex;
	}
	else {
		BoneIndex = m_BoneNameToIndexMap[BoneName];
	}

	return BoneIndex;
}

void SkinnedMesh::PopulateBuffers()
{
	for (unsigned int i = 0; i < m_Meshes.size(); i++) {

		Vertex3Dw *v = m_vertices.data();
		VertexBuffer<Vertex3Dw> vertices;
		vertices.Init(v + m_Meshes[i].BaseVertex , pScene->mMeshes[i]->mNumVertices , this->device.Get());
		this->vertexbuffer.push_back(vertices);

		DWORD* u = m_indices.data();
		IndexBuffer indices;
		indices.Init( u + m_Meshes[i].BaseIndex, m_Meshes[i].NumIndices, this->device.Get());
		this->indexbuffer.push_back(indices);

	}

}

void SkinnedMesh::Draw(const XMMATRIX& worldMatrix, const XMMATRIX& viewProjectionMatrix)
{
	this->deviceContext->VSSetShader(this->vertexShader->GetShader(), NULL, 0);
	this->deviceContext->PSSetShader(this->pixelShader->GetShader(), NULL, 0);

	this->deviceContext->VSSetConstantBuffers(0, 1, this->constantBuffer->GetAddressOf());
	this->deviceContext->VSSetConstantBuffers(1, 1, this->constantBuffer2->GetAddressOf());

	//상수버퍼1 WVP
	this->constantBuffer->data.WVP = DirectX::XMMatrixTranspose(worldMatrix * viewProjectionMatrix); //Calculate World-View-Projection Matrix
	this->constantBuffer->data.W = DirectX::XMMatrixTranspose(worldMatrix); //Calculate World
	this->constantBuffer->ApplyChanges();

	//상수버퍼2 g_bones
	for (int k = 0; k < m_BoneInfo.size(); k++)
	{
		this->constantBuffer2->data.g_bones[k] = m_BoneInfo[k].FinalTransformation;
	}
	this->constantBuffer2->ApplyChanges();

	for (unsigned int i = 0; i < m_Meshes.size(); i++) {

		//텍스쳐
		unsigned int MaterialIndex = m_Meshes[i].MaterialIndex;
		UINT offset = 0;

		this->deviceContext->PSSetShaderResources(0, 1, m_textures[MaterialIndex].GetTextureResourceViewAddress());
		
		//버텍스 인덱스 버퍼 탑재
		this->deviceContext->IASetVertexBuffers(0, 1, this->vertexbuffer[i].GetAddressOf(), this->vertexbuffer[i].StridePtr(), &offset);
		this->deviceContext->IASetIndexBuffer(indexbuffer[i].Get(), DXGI_FORMAT_R32_UINT, 0);
		this->deviceContext->DrawIndexed(this->indexbuffer[i].IndexCount(), 0, 0);

	}
	


}


UINT SkinnedMesh::FindPosition(float AnimationTimeTicks, const aiNodeAnim* pNodeAnim)
{
	for (UINT i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) {
		float t = (float)pNodeAnim->mPositionKeys[i + 1].mTime;
		if (AnimationTimeTicks < t) {
			return i;
		}
	}

	return 0;
}


void SkinnedMesh::CalcInterpolatedPosition(aiVector3D& Out, float AnimationTimeTicks, const aiNodeAnim* pNodeAnim)
{
	// we need at least two values to interpolate...
	if (pNodeAnim->mNumPositionKeys == 1) {
		Out = pNodeAnim->mPositionKeys[0].mValue;
		return;
	}

	UINT PositionIndex = FindPosition(AnimationTimeTicks, pNodeAnim);
	UINT NextPositionIndex = PositionIndex + 1;
	assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
	float t1 = (float)pNodeAnim->mPositionKeys[PositionIndex].mTime;
	float t2 = (float)pNodeAnim->mPositionKeys[NextPositionIndex].mTime;
	float DeltaTime = t2 - t1;
	float Factor = (AnimationTimeTicks - t1) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
	const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
	aiVector3D Delta = End - Start;
	Out = Start + Factor * Delta;
}


UINT SkinnedMesh::FindRotation(float AnimationTimeTicks, const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumRotationKeys > 0);

	for (UINT i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) {
		float t = (float)pNodeAnim->mRotationKeys[i + 1].mTime;
		if (AnimationTimeTicks < t) {
			return i;
		}
	}

	return 0;
}


void SkinnedMesh::CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTimeTicks, const aiNodeAnim* pNodeAnim)
{
	// we need at least two values to interpolate...
	if (pNodeAnim->mNumRotationKeys == 1) {
		Out = pNodeAnim->mRotationKeys[0].mValue;
		return;
	}

	UINT RotationIndex = FindRotation(AnimationTimeTicks, pNodeAnim);
	UINT NextRotationIndex = RotationIndex + 1;
	assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
	float t1 = (float)pNodeAnim->mRotationKeys[RotationIndex].mTime;
	float t2 = (float)pNodeAnim->mRotationKeys[NextRotationIndex].mTime;
	float DeltaTime = t2 - t1;
	float Factor = (AnimationTimeTicks - t1) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
	const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
	aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
	Out.Normalize();
}


UINT SkinnedMesh::FindScaling(float AnimationTimeTicks, const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumScalingKeys > 0);

	for (UINT i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++) {
		float t = (float)pNodeAnim->mScalingKeys[i + 1].mTime;
		if (AnimationTimeTicks < t) {
			return i;
		}
	}

	return 0;
}


void SkinnedMesh::CalcInterpolatedScaling(aiVector3D& Out, float AnimationTimeTicks, const aiNodeAnim* pNodeAnim)
{
	// we need at least two values to interpolate...
	if (pNodeAnim->mNumScalingKeys == 1) {
		Out = pNodeAnim->mScalingKeys[0].mValue;
		return;
	}

	UINT ScalingIndex = FindScaling(AnimationTimeTicks, pNodeAnim);
	UINT NextScalingIndex = ScalingIndex + 1;
	assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
	float t1 = (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime;
	float t2 = (float)pNodeAnim->mScalingKeys[NextScalingIndex].mTime;
	float DeltaTime = t2 - t1;
	float Factor = (AnimationTimeTicks - (float)t1) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
	const aiVector3D& End = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
	aiVector3D Delta = End - Start;
	Out = Start + Factor * Delta;
}


void SkinnedMesh::GetBoneTransforms(float TimeInSeconds, std::vector<XMMATRIX>& Transforms)
{
	Transforms.resize(m_BoneInfo.size());
	XMMATRIX Identity = XMMatrixIdentity();

	if (!this->animations.empty())
	{
		float TicksPerSecond = (float)(pScene->mAnimations[0]->mTicksPerSecond != 0 ? pScene->mAnimations[0]->mTicksPerSecond : 25.0f);
		float TimeInTicks = TimeInSeconds * TicksPerSecond;
		float AnimationTimeTicks = fmod(TimeInTicks, (float)pScene->mAnimations[0]->mDuration);
		ReadNodeHierarchy(AnimationTimeTicks, pScene->mRootNode, Identity, pScene->mAnimations[0]);
	}
	else
	{
		ReadNodeHierarchy(0.0f, pScene->mRootNode, Identity, nullptr);
	}

	Transforms.resize(m_BoneInfo.size());

	for (UINT i = 0; i < m_BoneInfo.size(); i++) {
		Transforms[i] = m_BoneInfo[i].FinalTransformation;
	}
}

void SkinnedMesh::SetBoneTransforms(UINT index, XMMATRIX transform)
{
	m_BoneInfo[index].FinalTransformation = transform;
}

std::string SkinnedMesh::GetPath()
{
	return this->filePath;
}

const aiScene* SkinnedMesh::GetAiScene()
{
	return this->pScene;
}


void SkinnedMesh::ReadNodeHierarchy(float AnimationTimeTicks, const aiNode* pNode, const XMMATRIX& ParentTransform, const aiAnimation* pAnimation)
{
	std::string NodeName(pNode->mName.data);
	XMMATRIX NodeTransformation(XMMatrixTranspose(XMMATRIX(&pNode->mTransformation.a1)));
	XMMATRIX GlobalTransformation;

	if (pAnimation != nullptr)
	{
		const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);

		if (pNodeAnim)
		{
			// Interpolate scaling and generate scaling transformation matrix
			aiVector3D Scaling;
			CalcInterpolatedScaling(Scaling, AnimationTimeTicks, pNodeAnim);
			aiMatrix4x4 ScalingM;
			ScalingM.Scaling(Scaling, ScalingM);
			//ScalingM.InitScaleTransform(Scaling.x, Scaling.y, Scaling.z);

			// Interpolate rotation and generate rotation transformation matrix
			aiQuaternion RotationQ;
			CalcInterpolatedRotation(RotationQ, AnimationTimeTicks, pNodeAnim);
			aiMatrix4x4 RotationM(RotationQ.GetMatrix());
			//Matrix4f RotationM = Matrix4f(RotationQ.GetMatrix());

			// Interpolate translation and generate translation transformation matrix
			aiVector3D Translation;
			CalcInterpolatedPosition(Translation, AnimationTimeTicks, pNodeAnim);
			aiMatrix4x4 TranslationM;
			TranslationM.Translation(Translation, TranslationM);
			//TranslationM.InitTranslationTransform(Translation.x, Translation.y, Translation.z);
			// Combine the above transformations
			aiMatrix4x4 temp = TranslationM * RotationM * ScalingM;
			NodeTransformation = XMMatrixTranspose(XMMATRIX(&temp.a1));
		}
	}

	GlobalTransformation = NodeTransformation * ParentTransform;

	if (m_BoneNameToIndexMap.find(NodeName) != m_BoneNameToIndexMap.end()) {

		UINT BoneIndex = m_BoneNameToIndexMap[NodeName];

		//특정 본만 회전하고싶을시
		//if(BoneIndex == 16)
		//	GlobalTransformation = XMMatrixRotationRollPitchYaw(0, 0.5, 0) * GlobalTransformation;
		m_BoneInfo[BoneIndex].FinalTransformation = XMMatrixTranspose(m_BoneInfo[BoneIndex].OffsetMatrix * GlobalTransformation * m_GlobalInverseTransform);
	}
	

	for (UINT i = 0; i < pNode->mNumChildren; i++) {
		ReadNodeHierarchy(AnimationTimeTicks,pNode->mChildren[i], GlobalTransformation, pAnimation);
	}

}

const aiNodeAnim* SkinnedMesh::FindNodeAnim(const aiAnimation* pAnimation, const std::string& NodeName)
{
	for (UINT i = 0; i < pAnimation->mNumChannels; i++) {
		const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

		if (std::string(pNodeAnim->mNodeName.data) == NodeName) {
			return pNodeAnim;
		}
	}

	return NULL;
}

std::wstring SkinnedMesh::Utf8ToWide(const char* in) {
	int size = MultiByteToWideChar(CP_UTF8, 0, in, -1, nullptr, 0);
	// size includes terminating null; std::wstring adds null automatically
	std::wstring out(static_cast<size_t>(size) - 1, L'\0');
	MultiByteToWideChar(CP_UTF8, 0, in, -1, &out[0], size);
	return out;
}

std::wstring SkinnedMesh::getFileExtension(std::wstring& path)
{
	size_t index = path.rfind(L".");

	if (index != std::wstring::npos) {

		return path.substr(index + 1, path.length() - index);
	}

	return std::wstring();
}

//텍스쳐를 로드 합니다.
std::vector<Texture> SkinnedMesh::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const aiScene* scene)
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
				std::wstring filename = std::wstring(directory.begin(), directory.end()) + L'/' + Utf8ToWide(str.C_Str());

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
				{
					std::cout << "Texture couldn't be loaded" << std::endl;
					Logger::Log(hr, L"Texture couldn't be loaded");
				}	
				else
				{
					hr = CreateShaderResourceView(device.Get(),
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
ID3D11ShaderResourceView* SkinnedMesh::loadEmbeddedTexture(const aiTexture* embeddedTexture) {
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
			Logger::Log(hr, L"CreateTexture2D failed!");

		hr = this->device->CreateShaderResourceView(texture2D, nullptr, &texture);


		if (FAILED(hr))
			Logger::Log(hr, L"CreateShaderResourceView failed!");

		return texture;
	}

	// mHeight is 0, so try to load a compressed texture of mWidth bytes
	const size_t size = embeddedTexture->mWidth;

	hr = CreateWICTextureFromMemory(this->device.Get(), this->deviceContext.Get(), reinterpret_cast<const unsigned char*>(embeddedTexture->pcData), size, nullptr, &texture);

	if (FAILED(hr))
		Logger::Log(hr, L"Texture couldn't be created from memory!");

	return texture;
}

