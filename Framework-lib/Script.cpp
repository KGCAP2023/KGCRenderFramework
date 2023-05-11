#include "pch.h"
#include "Script.h"
#include "Framework.h"

Script::Script(GameObject* owner, Framework* framework) : Component(owner)
{
	this->type = Component::Type::SCRIPT;
	this->name = "Script";
	this->lua = framework->GetLuaManager();
}