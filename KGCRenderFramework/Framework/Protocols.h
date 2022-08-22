#pragma once
#include "..\pch.h"
#include <unordered_map>

struct ComponentHash
{
	template <typename T>
	std::size_t operator()(T t) const
	{
		return static_cast<std::size_t>(t);
	}
};

enum class ComponentProtocol
{
	SPHERE,
	CUBE,
	

};

std::unordered_map<ComponentProtocol, int, ComponentHash> myMap;
