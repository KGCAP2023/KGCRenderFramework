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
		

		UIManager* uiMgr = new UIManager(rw);
		uiMgr->InitResource();
		uiMgr->Init();
		uiMgr->RegisterAllUI();
		rw->AddMenubar([&]() {

			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::MenuItem("LevelEditor view"))
				{
					rw->FindLayer("example2")->Activate();
				}
				if (ImGui::MenuItem("Hierarchy View"))
				{
					rw->FindLayer("hierarachy")->Activate();
				}
				if (ImGui::MenuItem("Debug View"))
				{
					rw->FindLayer("DebugView")->Activate();
				}
				if (ImGui::MenuItem("ResourceManager View"))
				{
					rw->FindLayer("RMV")->Activate();
				}
				if (ImGui::MenuItem("test"))
				{
					rw->SwitchObjectManager();
					uiMgr->ChangeHierachyObject();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Script"))
			{
				if (ImGui::MenuItem("Script Editor"))
				{
					rw->FindLayer("ScriptEditor")->Activate();
				}
					
				ImGui::EndMenu();
			}
			});
	}

	//Example* temp = new Example(objM, "example");

	rw->run();
	return 0;
}