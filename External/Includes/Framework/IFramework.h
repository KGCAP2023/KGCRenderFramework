#pragma once
#include "pch.h"
#include "ILayer.h"
#include "GameObjectManager.h"
#include "ResourceManager.h"
#include "AudioManager.h"
#include "LuaManager.h"
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
	virtual void AddMenubar(std::function<void()> callback);
	virtual void SetImGuiDemo(bool value);

	//RUN

	virtual bool Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height);
	virtual void run();

	/// <summary>
	/// gameObjectManger 반환.
	/// </summary>
	/// <returns></returns>
	virtual IGameObjectManager* GetGameObjectManager() { return nullptr; };

	/// <summary>
	/// gameObjectManger 교체.
	/// </summary>
	/// <returns></returns>
	virtual void SwitchObjectManager() {};
	/// <summary>
	/// 스위치 리스너를 등록합니다.
	/// 내부에서 모드가 바뀐 후에 호출됩니다.
	/// </summary>
	/// <param name="callback"></param>
	virtual void AddSwitchEventListener(std::function<void(SceneMode mode)> callback) {};
	/// <summary>
	/// 스위치 리스너 이벤트를 호출합니다.
	/// </summary>
	/// <param name="mode"></param>
	virtual void CallSwitchEvent(SceneMode mode) {};
	virtual IGameObjectManager* GetCurrentGameObjectManager() { return nullptr; };

	/// <summary>
	/// 리소스 매니저를 반환합니다.
	/// </summary>
	/// <returns></returns>
	virtual IResourceManager* GetResourceManager() { return nullptr; };

	/// <summary>
	/// 오디오 매니저를 반환합니다.
	/// </summary>
	/// <returns></returns>
	virtual IAudioManager* GetAudioManager() { return nullptr; };

	/// <summary>
	/// 루아 매니저를 반환합니다.
	/// </summary>
	/// <returns></returns>
	virtual LuaManager* GetLuaManager() { return nullptr; }

	/// <summary>
	/// 카메라 2d, 3d 모드 변환
	/// </summary>
	/// <param name="_type">[viewType::_2D] 또는 [viewType::_3D] 입력</param>
	virtual void ChangeCameraViewType(Camera3D::ViewType viewType);

};
