// KGCRenderFramework.cpp : 애플리케이션에 대한 진입점을 정의합니다.
#include "pch.h"
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <Framework/IFrameworkFactory.h>
#include <Framework/Framework.h>
#include <Framework/GameObjectManager.h>
#include <Framework/TileMapRenderer.h>
#include <Framework/TileMap.h>
#include "UIManager.h"
#pragma comment(lib, "Framework-lib.lib")

class Example : public ILayer
{
public:
	IGameObjectManager* _manager;

	Example(IGameObjectManager* manager, const std::string name) : ILayer(name)
	{
		this->_manager = manager;
	}
	virtual ~Example() 
	{
	
	};
	virtual void Init()
	{

	};
	virtual void Update()
	{

	};
	virtual void draw()
	{

	}
	virtual void Render()
	{
		
	};
};

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{	
	IFramework* rw = IFrameworkFactory::createFramework();

	rw->Initialize(hInstance, "KYONGGI CAPSTONE", "Framework", 1600, 900);

	{
		rw->AddMenubar([]() {

			if (ImGui::BeginMenu("File2222"))
			{
				if (ImGui::MenuItem("Open", "Ctrl+O")) { /* 파일 열기 */ }
				if (ImGui::MenuItem("Save", "Ctrl+S")) { /* 파일 저장 */ }
				ImGui::EndMenu();
			}
			
		});

		UIManager* uiMgr = new UIManager(rw);
		uiMgr->InitResource();
		uiMgr->Init();
		uiMgr->RegisterAllUI();
	}

	//Example* temp = new Example(objM, "example");

	rw->run();
	return 0;
}