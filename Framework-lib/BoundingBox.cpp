#include "pch.h"
#include "BoundingBox.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "ModelRenderer.h"
#include "TileMapRenderer.h"
#include "SkinnedMeshRenderer.h"

#pragma region 그리기_순서_목록_벡터

	//큐브
	DWORD cubeArr[] =
	{
		0, 1,
		1, 2,
		2, 3,
		3, 0,
		4, 5,
		5, 6,
		6, 7,
		7, 4,
		0, 4,
		1, 5,
		2, 6,
		3, 7
	};
	std::vector<DWORD> cube(std::begin(cubeArr), std::end(cubeArr));

	//모래시계
	DWORD hourGlassArr[] =
	{
		0, 1,
		1, 2,
		2, 3,
		3, 0,
		4, 5,
		5, 6,
		6, 7,
		7, 4,
		0,5,
		1,4,
		3,6,
		2,7,
	};
	std::vector<DWORD> hourGlass(std::begin(hourGlassArr), std::end(hourGlassArr));

#pragma endregion




BoundingBox3D::BoundingBox3D(GameObject* owner, Component* component, ResourceManager* res) : BoundingBoxRenderer(owner)
{
	this->type = Component::Type::BOUNDING_BOX;
	this->deviceContext = res->deviceContext;
	this->device = res->device;
	this->ps = res->FindPixelShader("ps_1");
	this->vs = res->FindVertexShader("vs_1");
	this->constantBuffer = &res->cb1;

	Renderer* render = nullptr;

	switch (component->GetType())
	{
		case Component::Type::RENDERER_MODEL:
		case Component::Type::RENDERER_SKINNED_MODEL:
		{
			render = dynamic_cast<Renderer*>(component);
		}
		break;
	}

	min = { 0,0,0 };
	max = { 0,0,0 };

	XMMATRIX worldMatrix = owner->transform.worldMatrix;

	if (render != nullptr)
	{
		std::string name = render->GetName();
		
		std::vector<SimpleVertex>* v = res->GetCachedBBOXVertices(name);

		if (v != nullptr)
		{
			this->vertices = v;
		}
		else
		{
			Assimp::Importer importer;
			const aiScene* pScene = importer.ReadFile(render->GetPath(),
				aiProcess_Triangulate |
				aiProcess_ConvertToLeftHanded);

			processNode(pScene->mRootNode, pScene, DirectX::XMMatrixIdentity());
		}
	}

	if (this->vertices == nullptr)
	{
		this->vertices = new std::vector<SimpleVertex>();
		vertices->push_back(SimpleVertex{ XMFLOAT3(min.x, max.y, min.z), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) });
		vertices->push_back(SimpleVertex{ XMFLOAT3(max.x, max.y, min.z), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) });
		vertices->push_back(SimpleVertex{ XMFLOAT3(max.x, max.y, max.z), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) });
		vertices->push_back(SimpleVertex{ XMFLOAT3(min.x, max.y, max.z), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) });
		vertices->push_back(SimpleVertex{ XMFLOAT3(min.x, min.y, min.z), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) });
		vertices->push_back(SimpleVertex{ XMFLOAT3(max.x, min.y, min.z), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) });
		vertices->push_back(SimpleVertex{ XMFLOAT3(max.x, min.y, max.z), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) });
		vertices->push_back(SimpleVertex{ XMFLOAT3(min.x, min.y, max.z), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) });
	}

	//초기설정은 cube
	indices = cube;

	//정점에대한 인덱스를 정의합니다. 
	DrawSetting();
	
}

BoundingBox3D::BoundingBox3D(Model* model) : BoundingBoxRenderer(nullptr)
{
	Assimp::Importer importer;
	const aiScene* pScene = importer.ReadFile(model->GetPath(),
		aiProcess_Triangulate |
		aiProcess_ConvertToLeftHanded);

	processNode(pScene->mRootNode, pScene, DirectX::XMMatrixIdentity());

	this->vertices = new std::vector<SimpleVertex>();
	vertices->push_back(SimpleVertex{ XMFLOAT3(min.x, max.y, min.z), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) });
	vertices->push_back(SimpleVertex{ XMFLOAT3(max.x, max.y, min.z), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) });
	vertices->push_back(SimpleVertex{ XMFLOAT3(max.x, max.y, max.z), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) });
	vertices->push_back(SimpleVertex{ XMFLOAT3(min.x, max.y, max.z), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) });
	vertices->push_back(SimpleVertex{ XMFLOAT3(min.x, min.y, min.z), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) });
	vertices->push_back(SimpleVertex{ XMFLOAT3(max.x, min.y, min.z), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) });
	vertices->push_back(SimpleVertex{ XMFLOAT3(max.x, min.y, max.z), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) });
	vertices->push_back(SimpleVertex{ XMFLOAT3(min.x, min.y, max.z), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) });
}

void BoundingBox3D::DrawSetting()
{
	//버텍스 버퍼와 인덱스 버퍼에 등록합니다.
	HRESULT hr = this->vertexbuffer.Init(vertices->data(), vertices->size(), this->device.Get());
	hr = this->indexbuffer.Init(indices.data(), indices.size(), this->device.Get());
}

void BoundingBox3D::ChangeDrawShape(ShapeType _type)
{

	switch (_type)
	{
	case BoundingBox3D::ShapeType::CUBE:
		indices = cube;
		break;
	case BoundingBox3D::ShapeType::HOUR_GLASS:
		indices = hourGlass;
		break;
	default:
		break;
	}

	DrawSetting();

}


void BoundingBox3D::Draw(const XMMATRIX& viewProjectionMatrix)
{

    if (isActiveBoundingBox)
    {
        // 버텍스 쉐이더에 들어가는 구조체는 버텍스 레이아웃과 동일해야합니다.
        // 버텍스 레이아웃은 GraphicManager.cpp의 InitializeShaders()를 참조하십시요

		this->deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
        deviceContext->IASetInputLayout(this->vs->GetInputLayout());

        //버텍스 쉐이더를 셋팅합니다.
        this->deviceContext->VSSetShader(this->vs->GetShader(), NULL, 0);
        //픽셀 쉐이더를 셋팅합니다.
        this->deviceContext->PSSetShader(this->ps->GetShader(), NULL, 0);

        //상수버퍼 - 쉐이더에서 WVP값을 가져다 쓸수있게 값을 셋팅해줍니다. 
        {
            //상수버퍼를 셋팅합니다.
            this->deviceContext->VSSetConstantBuffers(0, 1, this->constantBuffer->GetAddressOf());
            //상수버퍼에 들어가는 값을 셋팅합니다.
            this->constantBuffer->data.WVP = XMMatrixTranspose(this->owner->transform.worldMatrix * viewProjectionMatrix); //Calculate World-View-Projection Matrix
            //해당 값을 적용시킵니다.
            this->constantBuffer->ApplyChanges();
        }

       

        UINT offset = 0; 
        //정점을 그리는 방식을 결정합니다.
        //기본은 D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST를 사용합니다.
        //현재코드는 경계값을 그리기위해 D3D_PRIMITIVE_TOPOLOGY_LINELIST를 사용하며
        //D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST 사용시 정점 입력과 인덱스 입력을 삼각형 그리는 방향으로 정의 하십시요

        

        //버텍스 버퍼를 셋팅합니다.
        this->deviceContext->IASetVertexBuffers(0, 1, this->vertexbuffer.GetAddressOf(), this->vertexbuffer.StridePtr(), &offset);
        //인덱스 버퍼를 셋팅합니다.
        this->deviceContext->IASetIndexBuffer(indexbuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

        //파이프라인에 최종적으로 등록합니다.
        this->deviceContext->DrawIndexed(this->indexbuffer.IndexCount(), 0, 0);
  
    }
}

std::vector<DWORD>* BoundingBox3D::GetIndices()
{
	return &this->indices;
}

std::vector<SimpleVertex>* BoundingBox3D::getVertices()
{
	return this->vertices;
}

void BoundingBox3D::ChangeColor(float r, float g, float b, float alpha)
{
	//색깔을 교체합니다.
	for (SimpleVertex& v : *this->vertices)
	{
		v.Color = XMFLOAT4(r, g, b, alpha);
	}

	D3D11_MAPPED_SUBRESOURCE resource;
	ZeroMemory(&resource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	//해당 인덱스 버퍼에 락을 겁니다.
	this->deviceContext->Map(this->vertexbuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	//새로운 값 메모리 카피
	memcpy(resource.pData, vertices->data(), sizeof(SimpleVertex) * vertices->size());
	//락 해제
	this->deviceContext->Unmap(this->vertexbuffer.Get(), 0);
}

void BoundingBox3D::Update()
{

}

BoundingBox2D::BoundingBox2D(GameObject* owner, Component* component, ResourceManager* res) : BoundingBoxRenderer(owner)
{
	this->type = Component::Type::BOUNDING_BOX;
	this->deviceContext = res->deviceContext;
	this->device = res->device;
	this->spriteBatch = res->spriteBatch.get();
	this->color = new Texture();
	this->color->Initialize1x1ColorTexture(this->device.Get(), RGBColor(0, 0, 0));

	switch (component->GetType())
	{
		case Component::Type::RENDERER_SPRITE:
		{
			SpriteRenderer* render = dynamic_cast<SpriteRenderer*>(component);
			Sprite* sp = render->GetSprite();
			width = sp->GetWidth();
			height = sp->GetHeight();
		}
		break;
		case Component::Type::RENDERER_TILEMAP:
		{
			TileMapRenderer* render = dynamic_cast<TileMapRenderer*>(component);
			TileMap* tilemap = render->GetTileMap();
			width = tilemap->GetTileMapWidth();
			height = tilemap->GetTileMapHeight();
		}
		break;
	}
}

XMFLOAT2 BoundingBox2D::CalculateRotation(LONG x, LONG y, XMMATRIX& rotationMatrix)
{
	XMVECTOR rotatedVector = XMVector2Transform(XMVectorSet(x,y,0.0f,1.0f), rotationMatrix); // 회전된 좌표 계산
	XMFLOAT2 result;
	XMStoreFloat2(&result,rotatedVector);
	return result;
}

int BoundingBox2D::CalculatePointInBoundingBox(float pointX, float pointY)
{
	Transform& t = this->owner->transform;
	XMFLOAT3 pos = t.position;
	float scale = t.scale.x;
	float rot = t.rotation.z;
	RECT rectangle = RECT{ 0 , 0 , (LONG)(this->width * scale) , (LONG)(this->height * scale) };

	XMMATRIX rotationMatrix = XMMatrixRotationZ(rot);
	XMFLOAT2 result = CalculateRotation(rectangle.right, 0, rotationMatrix);
	XMFLOAT2 result2 = CalculateRotation(rectangle.right, rectangle.bottom, rotationMatrix);
	XMFLOAT2 result3 = CalculateRotation(0, rectangle.bottom, rotationMatrix);
	
	float x[4] = 
	{
		pos.x,
		pos.x + result.x,
		pos.x + result2.x,
		pos.x + result3.x
	};

	float y[4] = 
	{
		pos.y,
		pos.y + result.y,
		pos.y + result2.y,
		pos.y + result3.y
	};

	return pnpoly(4,x,y, pointX, pointY);
}

int BoundingBox2D::pnpoly(int nvert, float* vertx, float* verty, float testx, float testy)
{
	int i, j, c = 0;
	for (i = 0, j = nvert - 1; i < nvert; j = i++) {
		if (((verty[i] > testy) != (verty[j] > testy)) &&
			(testx < (vertx[j] - vertx[i]) * (testy - verty[i]) / (verty[j] - verty[i]) + vertx[i]))
			c = !c;
	}
	return c;
}

void BoundingBox2D::SetSize(int width, int height)
{
	this->width = width;
	this->height = height;
}

void BoundingBox2D::Draw(const XMMATRIX& viewProjectionMatrix)
{
	if (isActiveBoundingBox)
	{
		Transform& t = this->owner->transform;
		float scale = t.scale.x;
		float rot = t.rotation.z;
		LONG x = t.position.x;
		LONG y = t.position.y;

		RECT rectangle = RECT{ 0 , 0 , (LONG)(this->width * scale) , (LONG)(this->height * scale) };

		RECT a1 = RECT
		{ 
			x + rectangle.left,
			y + rectangle.top,
			x + rectangle.left + lineWidth,
			y + rectangle.top + rectangle.bottom
		};

		RECT a2 = RECT
		{ 
			x + rectangle.left,
			y + rectangle.top,
			x + rectangle.left + rectangle.right,
			y + rectangle.top + lineWidth
		};

		XMMATRIX rotationMatrix = XMMatrixRotationZ(rot);
		XMFLOAT2 result = CalculateRotation(rectangle.right, 0, rotationMatrix);
		XMFLOAT2 result2 = CalculateRotation(0, rectangle.bottom, rotationMatrix);

		RECT a3;

		a3 = RECT
		{
			x + (LONG)result.x + rectangle.left ,
			y + (LONG)result.y + rectangle.top,
			x + (LONG)result.x + rectangle.left + lineWidth,
			y + (LONG)result.y + rectangle.bottom + rectangle.top + lineWidth
		};

		RECT a4 = RECT
		{ 
			x + (LONG)result2.x + rectangle.left,
			y + (LONG)result2.y + rectangle.top,
			x + (LONG)result2.x + rectangle.left + rectangle.right + lineWidth,
			y + (LONG)result2.y + rectangle.top  + lineWidth
		};

		{
			this->spriteBatch->Draw(this->color->Get(), a1, nullptr, Colors::White, rot, XMFLOAT2(0, 0), SpriteEffects_None, this->layerDepth);
			this->spriteBatch->Draw(this->color->Get(), a2, nullptr, Colors::White, rot, XMFLOAT2(0, 0), SpriteEffects_None, this->layerDepth);
			this->spriteBatch->Draw(this->color->Get(), a3, nullptr, Colors::White, rot, XMFLOAT2(0, 0), SpriteEffects_None, this->layerDepth);
			this->spriteBatch->Draw(this->color->Get(), a4, nullptr, Colors::White, rot, XMFLOAT2(0, 0), SpriteEffects_None, this->layerDepth);
		}
	}
}

std::vector<DWORD>* BoundingBox2D::GetIndices()
{
	return &this->indices;
}

void BoundingBox2D::ChangeColor(float r, float g, float b, float alpha)
{
	if (this->color != nullptr) delete this->color;
	this->color = new Texture();
	this->color->Initialize1x1ColorTexture(this->device.Get(), RGBColor(r, g, b));
}


