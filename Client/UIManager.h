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
#include "DebugView.h"
#include "ResourceManagerView.h"
#include "ScriptEditor.h"

class UIManager 
{

public:
	UIManager(IFramework* framework)
	{
		this->framework = framework;
		this->objMgr = framework->GetGameObjectManager();	//3/31 리소스 매니저 추가 후 매니저 명 혼동을 방지하기 위해 manager에서 objM로 이름 변경
		this->resMgr = framework->GetResourceManager();
		this->audMgr = framework->GetAudioManager();
	}

	void Init() //UI 클래스 초기화
	{
		LevelEditExample* temp2 = new LevelEditExample(this->objMgr, resMgr, "example2");
		uiList.push_back(temp2);
		Hierarchy* hierarachy = new Hierarchy(this->objMgr,this->resMgr, "hierarachy", this->framework);
		uiList.push_back(hierarachy);
		DebugView* debug = new DebugView(this->objMgr, "DebugView");
		uiList.push_back(debug);
		ResourceManagerView* RMV = new ResourceManagerView(this->objMgr, resMgr, audMgr, "RMV");
		uiList.push_back(RMV);
		ScriptEditor* SE = new ScriptEditor(this->objMgr, "ScriptEditor");
		uiList.push_back(SE);
	}

	void InitResource()
	{
		sp = this->resMgr->LoadSprite("test", "..//Resource//Textures//sand.PNG");
		//this->TileMapping();
	}

	void RegisterAllUI()
	{
		for (ILayer* ui : uiList)
		{
			framework->RegisterLayer(ui->GetName(), ui);
		}
	}

	void TileMapping() 
	{
		TileMap* tileMap = new TileMap();
		tileMap->Init("tilemap",sp, 5, 5);

		//타일사이즈 스케일
		tileMap->SetScale(2.f);

		//타일선택 
		tileMap->SelectTile(0, 0, 0, 0);
		tileMap->SelectTile(1, 1, 1, 1);
		tileMap->SelectTile(2, 2, 2, 2);

		//타일맵등록
		resMgr->RegisterTileMap("tilemap", tileMap);

		//게임오브젝트 생성
		GameObject* gg = objMgr->CreateGameObject("eee");
		gg->transform.SetPosition(50, 40, 0);

		//타일맵 렌더러 컴포넌트 생성
		TileMapRenderer* tileRender = new TileMapRenderer(gg,(ResourceManager*)resMgr);

		//타일맵 렌더러 컴포넌트에 타일맵을 장착		Init 대신 AddTileMap 사용
		tileRender->SetTileMap(tileMap);

		//게임오브젝트에 타일맵 렌더러 컴포넌트 장착
		gg->AddComponent(tileRender);
	}

	std::vector<ILayer*> uiList;
	IResourceManager* resMgr = nullptr;
	IGameObjectManager* objMgr = nullptr;
	IAudioManager* audMgr = nullptr;
	IFramework* framework = nullptr;
	Sprite* sp = nullptr;
};
