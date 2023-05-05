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
		this->objMgr = framework->GetGameObjectManager();	//3/31 ���ҽ� �Ŵ��� �߰� �� �Ŵ��� �� ȥ���� �����ϱ� ���� manager���� objM�� �̸� ����
		this->resMgr = framework->GetResourceManager();
		this->audMgr = framework->GetAudioManager();
	}

	void Init() //UI Ŭ���� �ʱ�ȭ
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

		//Ÿ�ϻ����� ������
		tileMap->SetScale(2.f);

		//Ÿ�ϼ��� 
		tileMap->SelectTile(0, 0, 0, 0);
		tileMap->SelectTile(1, 1, 1, 1);
		tileMap->SelectTile(2, 2, 2, 2);

		//Ÿ�ϸʵ��
		resMgr->RegisterTileMap("tilemap", tileMap);

		//���ӿ�����Ʈ ����
		GameObject* gg = objMgr->CreateGameObject("eee");
		gg->transform.SetPosition(50, 40, 0);

		//Ÿ�ϸ� ������ ������Ʈ ����
		TileMapRenderer* tileRender = new TileMapRenderer(gg,(ResourceManager*)resMgr);

		//Ÿ�ϸ� ������ ������Ʈ�� Ÿ�ϸ��� ����		Init ��� AddTileMap ���
		tileRender->SetTileMap(tileMap);

		//���ӿ�����Ʈ�� Ÿ�ϸ� ������ ������Ʈ ����
		gg->AddComponent(tileRender);
	}

	std::vector<ILayer*> uiList;
	IResourceManager* resMgr = nullptr;
	IGameObjectManager* objMgr = nullptr;
	IAudioManager* audMgr = nullptr;
	IFramework* framework = nullptr;
	Sprite* sp = nullptr;
};
