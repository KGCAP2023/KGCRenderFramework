#pragma once
#include "pch.h"
#include "IScript.h"
#include "ILayer.h"
#include "GameObjectManager.h"
#include "Camera.h"

class IFramework
{
public:
	//스크립트 
	//virtual void RegisterScript(const std::string& key, IScript* layer);
	//virtual void DeleteScript(const std::string& key);
	//virtual IScript* FindScript(const std::string& key);
	
	//GUI 
	virtual void RegisterLayer(const std::string& key, ILayer* layer);
	virtual void DeleteLayer(const std::string& key);
	virtual ILayer* FindLayer(const std::string& key);

	//RUN

	virtual bool Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height);
	virtual void run();

	virtual IGameObjectManager* GetGameObjectManager() { return nullptr; };
	virtual void ChangeCameraViewType(viewType);

};
