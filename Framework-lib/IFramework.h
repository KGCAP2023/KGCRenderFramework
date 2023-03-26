#pragma once
#include "pch.h"
#include "IScript.h"
#include "ILayer.h"
#include "GameObjectManager.h"
#include "ResourceManager.h"
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

	/// <summary>
	/// gameObjectManger 인터페이스 반환
	/// </summary>
	/// <returns></returns>
	virtual IGameObjectManager* GetGameObjectManager() { return nullptr; };
	/// <summary>
	/// 리소스 매니저를 반환합니다.
	/// </summary>
	/// <returns></returns>
	virtual IResourceManager* GetResourceManager() { return nullptr; };

	/// <summary>
	/// 카메라 2d, 3d 모드 변환
	/// </summary>
	/// <param name="_type">[viewType::_2D] 또는 [viewType::_3D] 입력</param>
	virtual void ChangeCameraViewType(viewType);

};
