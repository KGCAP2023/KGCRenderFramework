#pragma once
#include "..\pch.h"
#include "GraphicManager.h"
#include "Timer.h"
#include <Keyboard.h>
#include <Mouse.h>


/*
* ������ ��ũ �Դϴ�.
*/
class Framework
{
public:

	bool Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height);\
	void RegisterWindow(HINSTANCE hInstance); // WindowClassEX�� �ʱ�ȭ �Ѵ�.
	LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool ProcessMsg();
	void run();
	void Update();
	void RenderFrame();

	~Framework();

	std::string window_title = "";
	std::wstring window_title_wide = L""; // 2����Ʈ �����ڵ�
	std::string window_class = "";
	std::wstring window_class_wide = L""; // 2����Ʈ �����ڵ�

	int width = 0; //�ʺ�
	int height = 0; //����


private:

	HWND handle = NULL; // ������ �ڵ�
	HINSTANCE hInstance = NULL; // ���ø����̼� �ν��Ͻ�
	//std::unique_ptr<DirectX::Keyboard> keyboard;
	std::shared_ptr<DirectX::Keyboard> keyboard;
	std::unique_ptr<DirectX::Mouse> mouse;

	std::queue<int> xPosRelative;
	std::queue<int> yPosRelative;

	GraphicManager graphics;
	Timer timer;
	
};

static Framework* framework = nullptr;