#include "pch.h"
#include "TestRenderer.h"

bool TestRenderer::Init(ResourceManager* res, Sprite *sprite)
{
    //기본셋팅 
    this->device = res->device.Get();
    this->deviceContext = res->deviceContext.Get();
    this->constantBuffer = res->cb2;
    this->pixelShader = res->FindPixelShader("ps_2");
    this->vertexShader = res->FindVertexShader("vs_2");
    this->sprite = sprite;

    //정점 좌표 그리기
	vertices.push_back(Vertex3D(-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f));
	vertices.push_back(Vertex3D(0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f));
	vertices.push_back(Vertex3D(-0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f));
	vertices.push_back(Vertex3D(0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f));
    
    //반드시 시계방향으로 그릴것!!!!
    std::vector<DWORD> arr =
    {
        0, 2, 1,
        2, 3, 1
    };

    //이거는 반시계방향
    //반시계방향으로 그리면 정면에서 안보이고 뒤쪽에서 바라보면 보입니다.
    //std::vector<DWORD> arr =
    //{
    //    0, 1, 2,
    //    2, 1, 3
    //};

	std::vector<DWORD> indices(std::begin(arr), std::end(arr));

	//버텍스 버퍼와 인덱스 버퍼에 등록합니다.
	HRESULT hr = this->vertexbuffer.Init(vertices.data(), vertices.size(), device);
	hr = this->indexbuffer.Init(indices.data(), indices.size(), device);



    //<필수> 초기화 작업 꼭 해줄것 안하면 scale 기본값 0 0 이라 안보임

    this->owner->transform.SetPosition(0.0f, 0.0f, 0.0f);
    this->owner->transform.SetRotation(0.0f, 0.0f, 0.0f);
    this->owner->transform.SetScale(1.f, 1.f);

	return true;
}

void TestRenderer::Draw(const XMMATRIX& viewProjectionMatrix)
{
    // 버텍스 쉐이더에 들어가는 구조체는 버텍스 레이아웃과 동일해야합니다.
    // 버텍스 레이아웃은 GraphicManager.cpp의 InitializeShaders()를 참조하십시요
    this->deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    this->deviceContext->IASetInputLayout(this->vertexShader->GetInputLayout());

    //버텍스 쉐이더를 셋팅합니다.
    this->deviceContext->VSSetShader(this->vertexShader->GetShader(), NULL, 0);
    //픽셀 쉐이더를 셋팅합니다.
    this->deviceContext->PSSetShader(this->pixelShader->GetShader(), NULL, 0);

    XMMATRIX worldMatrix = this->owner->transform.worldMatrix;

    //상수버퍼 - 쉐이더에서 WVP값을 가져다 쓸수있게 값을 셋팅해줍니다. 
    {
        //상수버퍼를 셋팅합니다.
        this->deviceContext->VSSetConstantBuffers(0, 1, this->constantBuffer.GetAddressOf());
        //상수버퍼에 들어가는 값을 셋팅합니다.
        this->constantBuffer.data.WVP = XMMatrixTranspose(worldMatrix * viewProjectionMatrix); //Calculate World-View-Projection Matrix
        this->constantBuffer.data.W = XMMatrixTranspose(worldMatrix);
        //해당 값을 적용시킵니다.
        this->constantBuffer.ApplyChanges();
    }

    UINT offset = 0;
    
    if(this->sprite != nullptr)
    this->deviceContext->PSSetShaderResources(0,1,this->sprite->GetTextureResourceViewAddress());

    //버텍스 버퍼를 셋팅합니다.
    this->deviceContext->IASetVertexBuffers(0, 1, this->vertexbuffer.GetAddressOf(), this->vertexbuffer.StridePtr(), &offset);
    //인덱스 버퍼를 셋팅합니다.
    this->deviceContext->IASetIndexBuffer(indexbuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

    //파이프라인에 최종적으로 등록합니다.
    this->deviceContext->DrawIndexed(this->indexbuffer.IndexCount(), 0, 0);

}

void TestRenderer::Update()
{
    //행렬 업데이트
    //Scale * Rotate * Translation
	GameObject* owner = this->owner;
	owner->transform.worldMatrix = DirectX::XMMatrixScaling(owner->transform.scale.x, owner->transform.scale.y, owner->transform.scale.z) * XMMatrixRotationRollPitchYaw(owner->transform.rotation.x, owner->transform.rotation.y, owner->transform.rotation.z) * XMMatrixTranslation(owner->transform.position.x, owner->transform.position.y, owner->transform.position.z);
	owner->transform.UpdateDirection();
}
