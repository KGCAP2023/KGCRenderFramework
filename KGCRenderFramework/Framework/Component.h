#pragma once
#include "..\pch.h"
class GameObject;

class Component
{
public:
	static enum class Type
	{
		RENDERER_MESH_,
		RENDERER_MODEL,
		RENDERER_SPRITE,
		CAMERA,
		COLLIDER_RECT,
		COLLIDER_SPHERE
	};

	Component(GameObject* owner) : owner(owner) {}

	virtual void Update();



	static struct ComponentHash
	{
		template <typename T>
		std::size_t operator()(T t) const
		{
			return static_cast<std::size_t>(t);
		}
	};

	Component::Type GetType();

protected:
	Component::Type type;
	GameObject* owner = nullptr;
	std::string name;
};

