#pragma once
#include "IFramework.h"
#include "GraphicManager.h"
#include "AudioManager.h"
#include "Timer.h"
#include "InputManager.h"
#include "LayerManager.h"
#include "GameObjectManager.h"
#include "ResourceManager.h"
#include "LuaManager.h"
#include "Ray.h"

/*
* 프레임 워크 입니다.
*/

class Framework : public IFramework
{
//API IMPLEMENT
public:

	//스크립트 
	//virtual void RegisterScript(const std::string& key, IScript* layer) override;
	//virtual void DeleteScript(const std::string& key) override;
	//virtual IScript* FindScript(const std::string& key) override;

	//GUI 
	virtual void RegisterLayer(const std::string& key, ILayer* layer) override;
	virtual void DeleteLayer(const std::string& key) override;
	virtual ILayer* FindLayer(const std::string& key) override;
	virtual void AddMenubar(std::function<void()> callback) override;
	virtual void SetImGuiDemo(bool value) override;

	virtual bool Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height) override;
	virtual void run() override;

	/// <summary>
	/// 프론트 기능 제공용 인터페이스 반환
	/// </summary>
	/// <returns></returns>
	virtual IGameObjectManager* GetGameObjectManager() override;
	/// <summary>
	/// 리소스 매니저를 반환합니다. 
	/// </summary>
	/// <returns></returns>
	virtual IResourceManager* GetResourceManager() override;
	/// <summary>
	/// 오디오 매니저를 반환합니다.
	/// </summary>
	/// <returns></returns>
	virtual IAudioManager* GetAudioManager() override;
	/// <summary>
	/// 백엔드 개발용 인스턴스 반환
	/// </summary>
	/// <returns></returns>
	GameObjectManager* GetGameObjectManagerInstance();
	/// <summary>
	/// 카메라 모드 전환
	/// </summary>
	/// <param name="">viewType::_2D 또는 viewType::_3D 전달</param>
	virtual void ChangeCameraViewType(Camera3D::ViewType viewType);

//INTERNAL
public:

	static const float getDeltaTime() { return dt;}
	
	void RegisterWindow(HINSTANCE hInstance); // WindowClassEX를 초기화 한다.
	LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool ProcessMsg();
	
	void Update();
	void RenderFrame();

	InputManager InputManager;
	LayerManager layerManager;
	GraphicManager graphics;
	ResourceManager resourceManager;
	AudioManager audioManager;
	LuaManager luaManager;
	GameObjectManager* gameObjManager = nullptr;
	Ray* ray;

	~Framework();

	std::string window_title = "";
	std::wstring window_title_wide = L""; // 2바이트 유니코드
	std::string window_class = "";
	std::wstring window_class_wide = L""; // 2바이트 유니코드

	int width = 0; //너비
	int height = 0; //높이

	HWND handle = NULL; // 윈도우 핸들
	HINSTANCE hInstance = NULL; // 어플리케이션 인스턴스

private:

	static float dt;
	Timer timer;
	
};

static Framework* framework;