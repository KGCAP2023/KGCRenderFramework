#pragma once
#include "Gameobject.h"
#include "Shaders.h"
#include "Sprite.h"
#include "ResourceManager.h"

class TestRenderer : public Component
{
public:

	TestRenderer(GameObject *owner) : Component(owner)
	{
		this->type = Component::Type::RENDERER_TEST;
		this->name = "TestRenderer";
	}

	~TestRenderer()
	{
		res = nullptr;
		device = nullptr;
		deviceContext = nullptr;
		vertices.clear();
		sprite = nullptr;
		vertexShader = nullptr;
		pixelShader = nullptr;
	}

	TestRenderer(const TestRenderer& rhs, GameObject* owner) : Component(owner)
	{
		std::cout << "[=] TestRenderer CLONE Process - Copy constructor called" << std::endl;
		res = rhs.res;

		device = rhs.device;
		deviceContext = rhs.deviceContext;

		vertices = rhs.vertices;
		constantBuffer = rhs.constantBuffer;

		vertexbuffer = rhs.vertexbuffer;
		indexbuffer = rhs.indexbuffer;

		sprite = rhs.sprite;

		vertexShader = rhs.vertexShader;
		pixelShader = rhs.pixelShader;

	}

	virtual Component* Copy(GameObject* owner)
	{
		Component* compo = new TestRenderer(*this, owner);
		return compo;
	};

	bool Init(ResourceManager* res, Sprite* sprite);

	virtual void Draw(const XMMATRIX& viewProjectionMatrix) override;
	virtual void Update() override;

private:

	ResourceManager* res;

	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* deviceContext = nullptr;

	std::vector<Vertex3D> vertices;
	ConstantBuffer<CB_VS_2> constantBuffer;

	VertexBuffer<Vertex3D> vertexbuffer;
	//ÀÎµ¦½º ¹öÆÛ
	IndexBuffer indexbuffer;

	Sprite* sprite = nullptr;

	VertexShader* vertexShader = nullptr;
	PixelShader* pixelShader = nullptr;
};