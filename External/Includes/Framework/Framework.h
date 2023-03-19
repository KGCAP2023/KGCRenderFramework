#pragma once
#include "IFramework.h"
#include "GraphicManager.h"
#include "Timer.h"
#include "InputManager.h"
#include "LayerManager.h"
/*
* ������ ��ũ �Դϴ�.
*/

class Framework : public IFramework
{
//API IMPLEMENT
public:

	//��ũ��Ʈ 
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
	
	void RegisterWindow(HINSTANCE hInstance); // WindowClassEX�� �ʱ�ȭ �Ѵ�.
	LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool ProcessMsg();
	
	void Update();
	void RenderFrame();

	InputManager InputManager;
	LayerManager layerManager;
	GraphicManager graphics;

	~Framework();

	std::string window_title = "";
	std::wstring window_title_wide = L""; // 2����Ʈ �����ڵ�
	std::string window_class = "";
	std::wstring window_class_wide = L""; // 2����Ʈ �����ڵ�

	int width = 0; //�ʺ�
	int height = 0; //����

	HWND handle = NULL; // ������ �ڵ�
	HINSTANCE hInstance = NULL; // ���ø����̼� �ν��Ͻ�

private:

	static float dt;
	Timer timer;
	
	static bool isMouseRightDown;
};

static Framework* framework;