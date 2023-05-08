#pragma once
#include "pch.h"
#include "ILayer.h"
#include "GameObjectManager.h"
#include "ResourceManager.h"
#include "AudioManager.h"
#include "Camera.h"

class IFramework
{
public:
	//��ũ��Ʈ 
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
	/// gameObjectManger ��ȯ.
	/// </summary>
	/// <returns></returns>
	virtual IGameObjectManager* GetGameObjectManager() { return nullptr; };

	/// <summary>
	/// gameObjectManger ��ü.
	/// </summary>
	/// <returns></returns>
	virtual void SwitchObjectManager() {};

	virtual IGameObjectManager* GetCurrentGameObjectManager() { return nullptr; };


	/// <summary>
	/// ���ҽ� �Ŵ����� ��ȯ�մϴ�.
	/// </summary>
	/// <returns></returns>
	virtual IResourceManager* GetResourceManager() { return nullptr; };

	/// <summary>
	/// ����� �Ŵ����� ��ȯ�մϴ�.
	/// </summary>
	/// <returns></returns>
	virtual IAudioManager* GetAudioManager() { return nullptr; };

	/// <summary>
	/// ī�޶� 2d, 3d ��� ��ȯ
	/// </summary>
	/// <param name="_type">[viewType::_2D] �Ǵ� [viewType::_3D] �Է�</param>
	virtual void ChangeCameraViewType(Camera3D::ViewType viewType);

};
