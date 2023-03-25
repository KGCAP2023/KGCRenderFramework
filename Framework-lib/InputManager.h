#pragma once
#include "pch.h"
#include <Keyboard.h>
#include <Mouse.h>

class Framework;

class InputManager
{
public:
	void Init(Framework* framework);
	
	static DirectX::Keyboard* GetKeyboard() { return keyboard; }
	static DirectX::Mouse* GetMouse() { return mouse; }
	static DirectX::Keyboard::State GetKeyboardState() { return keyboard->GetState();}
	static DirectX::Mouse::State GetMouseState() { return mouse->GetState();}
	
	static std::queue<int>& GetXPoseRelative() {return xPosRelative;}
	static std::queue<int>& GetYPoseRelative() {return yPosRelative;}

	void PushRawInputData(LONG x,LONG y);

private:

	static DirectX::Keyboard* keyboard;
	static DirectX::Mouse* mouse;
	static std::queue<int> xPosRelative;
	static std::queue<int> yPosRelative;

	Framework* framework;
};