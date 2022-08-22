#pragma once
#include "..\pch.h"
//#include "GameObject.h"

class Component
{
//private:
//	GameObject* owner;
//	std::string name;

public:
	static enum class Type
	{
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

};

