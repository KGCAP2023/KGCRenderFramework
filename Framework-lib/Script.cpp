#include "pch.h"
#include "Script.h"
#include "Framework.h"

Script::Script(GameObject* owner, Framework* framework) : Component(owner)
{
	this->lua = framework->GetLuaManager();
	this->init();
}
