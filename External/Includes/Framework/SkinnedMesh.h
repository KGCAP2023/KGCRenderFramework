#pragma once
#include "Texture.h"
#include "Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shaders.h"
#include "ConstantBuffer.h"
#include "ConstantBufferType.h"
#include "Matrix.h"
#include "Animation3D.h"
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>
#include <dtex\DirectXTex.h>


using namespace DirectX;
class ResourceManager;

#define MAX_NUM_BONES_PER_VERTEX 4

class SkinnedMesh
{
public:
    SkinnedMesh() {}

    ~SkinnedMesh();

    bool LoadMesh(const std::string& modelName,
        const std::string& Filename,
        ResourceManager* res);

    bool AddAnimation(std::string filepath)
    {
        const aiScene* scene = Importer.ReadFile(filepath.c_str(), aiProcess_Triangulate |
            aiProcess_ConvertToLeftHanded | aiProcess_OptimizeGraph);

        if (scene) {
            this->InitAnimations(scene);
            return true;
        }
        else {
            std::cout << "[x] Animation Load Failed - Error parsing " << filepath.c_str() << " : " << Importer.GetErrorString() << std::endl;
            return false;
        }
    }

    SkinnedMesh(const SkinnedMesh& rhs)
    {
        std::cout << "[=] SkinnedMesh CLONE Process - Copy constructor called" << std::endl;

        this->modelName = rhs.modelName;
        this->pScene = rhs.pScene;
        this->_isDefaultPose = rhs._isDefaultPose;
        this->animations = rhs.animations;
        this->world = XMMatrixIdentity();
        this->m_GlobalInverseTransform = rhs.m_GlobalInverseTransform;
        this->directory = rhs.directory;
        this->filePath = rhs.filePath;

        //쉐이더
        this->vertexShader = rhs.vertexShader;
        this->pixelShader = rhs.pixelShader;

        //메시 기본정보 (대입연산)
        this->m_Meshes = rhs.m_Meshes;

        //인덱스 버퍼 (대입연산)
        this->vertexbuffer = rhs.vertexbuffer;
        this->indexbuffer = rhs.indexbuffer;

        this->constantBuffer = rhs.constantBuffer;
        this->constantBuffer2 = rhs.constantBuffer2;

        this->device = rhs.device;
        this->deviceContext = rhs.deviceContext;

        this->m_vertices = rhs.m_vertices;
        this->m_indices = rhs.m_indices;

        this->m_textures = rhs.m_textures;
        this->textures_loaded_ = rhs.textures_loaded_;
        this->m_BoneNameToIndexMap = rhs.m_BoneNameToIndexMap;

        this->m_BoneInfo = rhs.m_BoneInfo;

        for (UINT i = 0; i < this->m_BoneInfo.size(); i++) {
            m_BoneInfo[i].FinalTransformation = XMMatrixIdentity();
        }

    }

    void Draw(const XMMATRIX& worldMatrix, const XMMATRIX& viewProjectionMatrix);

    UINT NumBones() const
    {
        return (UINT)m_BoneNameToIndexMap.size();
    }

    XMMATRIX& GetWorldTransform() { return world; }

    void GetBoneTransforms(float TimeInSeconds, std::vector<XMMATRIX>& Transforms, Animation3D* animation);
    void SetBoneTransforms(UINT index, XMMATRIX transform);
    //const Material& GetMaterial();

    std::string GetPath();
    const aiScene* GetAiScene();
    std::string GetName();

    void InitDefaultPose();
    bool isDefaultPose() { return _isDefaultPose; }
    void setDefaultPose(bool value) { _isDefaultPose = value; }

    std::vector<Animation3D*>* GetAnimationList() { return animations; }


private:
    Assimp::Importer Importer;
    const aiScene* pScene = nullptr;

    bool _isDefaultPose = true;
    //std::vector<aiAnimation*> animations;
    std::vector<Animation3D*>* animations;

    XMMATRIX world = XMMatrixIdentity();
    XMMATRIX m_GlobalInverseTransform;

    std::string modelName;
    std::string directory;
    std::string filePath;

    VertexShader* vertexShader;
    PixelShader* pixelShader;

    void Clear();

    bool InitFromScene(const aiScene* pScene, const std::string& Filename);

    void CountVerticesAndIndices(const aiScene* pScene, unsigned int& NumVertices, unsigned int& NumIndices);

    void ReserveSpace(unsigned int NumVertices, unsigned int NumIndices);

    void InitAllMeshes(const aiScene* pScene);

    void InitSingleMesh(UINT MeshIndex, const aiMesh* paiMesh);

    bool InitMaterials(const aiScene* pScene);

    void InitAnimations(const aiScene* pScene);

    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
        std::string typeName, const aiScene* scene);

    ID3D11ShaderResourceView* loadEmbeddedTexture(const aiTexture* embeddedTexture);

    struct VertexBoneData
    {
        UINT BoneIDs[MAX_NUM_BONES_PER_VERTEX] = { 0 };
        float Weights[MAX_NUM_BONES_PER_VERTEX] = { 0.0f };
        int index = 0;

        VertexBoneData()
        {
        }

        void AddBoneData(UINT BoneID, float Weight)
        {
            for (int i = 0; i < index; i++)
            {
                if (BoneIDs[i] == BoneID) {
                    return;
                }
            }

            //zero weights 일시 오버플로우 버그 해결
            if (Weight == 0.0f)
            {
                return;
            }

            if (index == MAX_NUM_BONES_PER_VERTEX)
            {
                printf("Max number of bones per vertex (%d) exceeded! \n", MAX_NUM_BONES_PER_VERTEX);
                assert(0);
            }

            BoneIDs[index] = BoneID;
            Weights[index] = Weight;

            index++;

            //for (UINT i = 0; i < ARRAYSIZE(BoneIDs); i++) {
            //    if (Weights[i] == 0.0) {
            //        BoneIDs[i] = BoneID;
            //        Weights[i] = Weight;
            //        //printf("Adding bone %d weight %f at index %i\n", BoneID, Weight, i);
            //        return;
            //    }
            //}
            //// should never get here - more bones than we have space for
            //assert(0);
        }
    };

    void LoadMeshBones(UINT MeshIndex, const aiMesh* paiMesh);
    void LoadSingleBone(UINT MeshIndex, const aiBone* pBone);
    int GetBoneId(const aiBone* pBone);
    void ReadNodeHierarchy(float AnimationTimeTicks, const aiNode* pNode, const XMMATRIX& ParentTransform, const aiAnimation* pAnimation, Animation3D* selectedAnimation);
   
    void PopulateBuffers();

    //Animation



    const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const std::string& NodeName);
    void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTimeTicks, const aiNodeAnim* pNodeAnim);
    UINT FindScaling(float AnimationTimeTicks, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTimeTicks, const aiNodeAnim* pNodeAnim);
    UINT FindRotation(float AnimationTimeTicks, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTimeTicks, const aiNodeAnim* pNodeAnim);
    UINT FindPosition(float AnimationTimeTicks, const aiNodeAnim* pNodeAnim);

    std::wstring Utf8ToWide(const char* in);
    std::wstring getFileExtension(std::wstring& path);

    struct BasicMeshEntry {
        BasicMeshEntry()
        {
            NumIndices = 0;
            BaseVertex = 0;
            BaseIndex = 0;
            MaterialIndex = -1;
        }

        unsigned int NumIndices;
        unsigned int BaseVertex;
        unsigned int BaseIndex;
        unsigned int MaterialIndex;
    };

    std::vector<BasicMeshEntry> m_Meshes;

    //버텍스 버퍼
    std::vector<VertexBuffer<Vertex3Dw>> vertexbuffer;
    //인덱스 버퍼
    std::vector<IndexBuffer> indexbuffer;
    //상수 버퍼
    ConstantBuffer<CB_VS_2>* constantBuffer = nullptr;
    ConstantBuffer<CB_VS_3>* constantBuffer2 = nullptr;

    //컨텍스트
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
    Microsoft::WRL::ComPtr<ID3D11Device> device;

    //정점
    std::vector<Vertex3Dw> m_vertices;
    //인덱스
    std::vector<DWORD> m_indices;

    //텍스쳐
    std::vector<Texture> m_textures;
    std::vector<Texture>* textures_loaded_;

    //BONE
    std::vector<VertexBoneData> m_Bones;
    std::map<std::string, UINT> m_BoneNameToIndexMap;


    struct BoneInfo
    {
        XMMATRIX OffsetMatrix;
        XMMATRIX FinalTransformation;

        BoneInfo(const XMMATRIX& Offset)
        {
            OffsetMatrix = Offset;
            XMMATRIX ZERO (0, 0, 0, 0,
                        0, 0, 0, 0,
                        0, 0, 0, 0,
                        0, 0, 0, 0);
            FinalTransformation = ZERO;
        }

        BoneInfo(const aiMatrix4x4 m)
        {
            XMMATRIX OFFSET
            (m.a1, m.a2, m.a3, m.a4,
                m.b1, m.b2, m.b3, m.b4,
                m.c1, m.c2, m.c3, m.c4,
                m.d1, m.d2, m.d3, m.d4);
            OffsetMatrix = OFFSET;

            XMMATRIX ZERO(0, 0, 0, 0,
                0, 0, 0, 0,
                0, 0, 0, 0,
                0, 0, 0, 0);
            FinalTransformation = ZERO;
        }

    };

    std::vector<BoneInfo> m_BoneInfo;

};

