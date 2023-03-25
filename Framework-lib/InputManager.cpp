#include "pch.h"
#include "InputManager.h"
#include "Framework.h"

DirectX::Keyboard* InputManager::keyboard = nullptr;
DirectX::Mouse* InputManager::mouse = nullptr;
std::queue<int> InputManager::xPosRelative;
std::queue<int> InputManager::yPosRelative;

void InputManager::Init(Framework* framework)
{
	this->keyboard = new Keyboard();
	this->mouse = new Mouse();
	this->mouse->SetWindow(framework->handle);
}

void InputManager::PushRawInputData(LONG x, LONG y)
{
	this->xPosRelative.push(x);
	this->yPosRelative.push(y);
}
