#pragma once
#include "pch.h"
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <Framework/IFrameworkFactory.h>
#include <Framework/Framework.h>
#include <Framework/GameObjectManager.h>
#include <Framework/TileMapRenderer.h>
#include <Framework/TileMap.h>
#include "LevelEditor.h"
#include "hierarchy.h"

class UIManager 
{

public:
	UIManager(IFramework* framework)
	{
		this->framework = framework;
		this->objMgr = framework->GetGameObjectManager();	//3/31 리소스 매니저 추가 후 매니저 명 혼동을 방지하기 위해 manager에서 objM로 이름 변경
		this->resMgr = framework->GetResourceManager();
	}

	void Init() //UI 클래스 초기화
	{
		LevelEditExample* temp2 = new LevelEditExample(this->objMgr, resMgr, "example2");
		uiList.push_back(temp2);
		Hierarchy* hierarachy = new Hierarchy(this->objMgr, "hierarachy", this->framework);
		uiList.push_back(hierarachy);
	}

	void InitResource()
	{
		this->resMgr->LoadSprite("test", "..\\Resource\\Textures\\seamless_grass.JPG");

	}

	void RegisterAllUI()
	{
		for (ILayer* ui : uiList)
		{
			framework->RegisterLayer(ui->GetName(), ui);
		}
	}

	std::vector<ILayer*> uiList;
	IResourceManager* resMgr = nullptr;
	IGameObjectManager* objMgr = nullptr;
	IFramework* framework = nullptr;
};
