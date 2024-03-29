#pragma once

class GameObject;

class Component
{
public:

	Component(GameObject* owner) : owner(owner) {}
	virtual ~Component() {};
	virtual Component* Copy(GameObject* owner);
	virtual void Update();
	virtual void Draw(const DirectX::XMMATRIX& viewProjectionMatrix);

	static enum class Type
	{
		UNKNOWN,
		RENDERER_MESH_,
		RENDERER_MODEL,
		RENDERER_SKINNED_MODEL,
		RENDERER_TEST,
		RENDERER_SPRITE,
		RENDERER_TILEMAP,
		BOUNDING_BOX,
		SCRIPT,
		CAMERA,
		COLLIDER_RECT,
		COLLIDER_SPHERE
	};

	static struct ComponentHash
	{
		template <typename T>
		std::size_t operator()(T t) const
		{
			return static_cast<std::size_t>(t);
		}
	};

	Component::Type GetType();
	GameObject* GetOwner();
	std::string GetName();

	bool IsSprite() { return isSpriteRender; }

protected:
	bool isSpriteRender = false;
	Component::Type type;
	GameObject* owner = nullptr;
	std::string name;
};