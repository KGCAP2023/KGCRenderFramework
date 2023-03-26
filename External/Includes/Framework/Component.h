#pragma once
class GameObject;

class Component
{
public:

	Component(GameObject* owner) : owner(owner) {}

	virtual void Update();
	virtual void Draw(const DirectX::XMMATRIX& viewProjectionMatrix);

	static enum class Type
	{
		RENDERER_MESH_,
		RENDERER_MODEL,
		RENDERER_SKINNED_MODEL,
		RENDERER_SPRITE,
		BOUNDING_BOX,
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

protected:
	Component::Type type;
	GameObject* owner = nullptr;
	std::string name;
};

