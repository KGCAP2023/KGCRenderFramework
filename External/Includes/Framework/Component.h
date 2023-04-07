#pragma once

class GameObject;

class Component
{
public:

	Component(GameObject* owner) : owner(owner) {}
	virtual ~Component() {};

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
		CAMERA,
		COLLIDER_RECT,
		COLLIDER_SPHERE
	};
	virtual void SetDeleteType(Component::Type type);

	static struct ComponentHash
	{
		template <typename T>
		std::size_t operator()(T t) const
		{
			return static_cast<std::size_t>(t);
		}
	};

	Component::Type GetType();
	Component::Type GetDeleteType();

	GameObject* GetOwner();
	std::string GetName();

protected:
	Component::Type type;
	Component::Type deleteType;
	GameObject* owner = nullptr;
	std::string name;
};