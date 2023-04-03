#pragma once
#include "pch.h"
#include <Keyboard.h>
#include <Mouse.h>

class Framework;

class InputManager
{
public:

	/// <summary>
	/// 키보드의 상태값을 가져옵니다.
	/// </summary>
	/// <returns></returns>
	static DirectX::Keyboard::State GetKeyboardState() { return keyboard->GetState();}
	/// <summary>
	/// 마우스의 상태값을 가져옵니다.
	/// </summary>
	/// <returns></returns>
	static DirectX::Mouse::State GetMouseState() {

		DirectX::Mouse::State state = mouse->GetState();

		if (isDockingSpace)
		{
			state.x = (int)viewX;
			state.y = (int)viewY;
		}

		return state;
	}
	/// <summary>
	/// GameView에 의해 정규화되지않은 값을 가져옵니다.
	/// 프론트에서 사용금지 
	/// </summary>
	/// <returns></returns>
	static DirectX::Keyboard* GetRawKeyboard() { return keyboard; }
	/// <summary>
	/// GameView에 의해 정규화되지않은 값을 가져옵니다.
	/// 프론트에서 사용금지 
	/// </summary>
	/// <returns></returns>
	static DirectX::Mouse* GetRawMouse() { return mouse; }

public:
	void Init(Framework* framework);
	
	static std::queue<int>& GetXPoseRelative() {return xPosRelative;}
	static std::queue<int>& GetYPoseRelative() {return yPosRelative;}

	void PushRawInputData(LONG x,LONG y);

	static bool isDockingSpace;
	static float viewX;
	static float viewY;

private:

	static DirectX::Keyboard* keyboard;
	static DirectX::Mouse* mouse;
	static std::queue<int> xPosRelative;
	static std::queue<int> yPosRelative;

	Framework* framework;
};