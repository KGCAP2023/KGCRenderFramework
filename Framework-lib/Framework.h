#pragma once
#include "IFramework.h"
#include "GraphicManager.h"
#include "Timer.h"
#include "InputManager.h"
#include "LayerManager.h"
/*
* 프레임 워크 입니다.
*/

class Framework : public IFramework
{
//API IMPLEMENT
public:

	//스크립트 
	//virtual void RegisterScript(const std::string& key, IScript* layer) override;
	//virtual void DeleteScript(const std::string& key) override;
	//virtual IScript* FindScript(const std::string& key) override;

	//GUI 
	virtual void RegisterLayer(const std::string& key, ILayer* layer) override;
	virtual void DeleteLayer(const std::string& key) override;
	virtual ILayer* FindLayer(const std::string& key) override;

	virtual bool Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height) override;
	virtual void run() override;

//INTERNAL
public:

	static const float& getDeltaTime() { return dt;}
	
	void RegisterWindow(HINSTANCE hInstance); // WindowClassEX를 초기화 한다.
	LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool ProcessMsg();
	
	void Update();
	void RenderFrame();

	InputManager InputManager;
	LayerManager layerManager;
	GraphicManager graphics;

	~Framework();

	std::string window_title = "";
	std::wstring window_title_wide = L""; // 2바이트 유니코드
	std::string window_class = "";
	std::wstring window_class_wide = L""; // 2바이트 유니코드

	int width = 0; //너비
	int height = 0; //높이

	HWND handle = NULL; // 윈도우 핸들
	HINSTANCE hInstance = NULL; // 어플리케이션 인스턴스

private:

	static float dt;
	Timer timer;
	
	static bool isMouseRightDown;
};

static Framework* framework;