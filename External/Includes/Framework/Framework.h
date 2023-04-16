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
* ������ ��ũ �Դϴ�.
*/

class Framework : public IFramework
{
//API IMPLEMENT
public:

	//��ũ��Ʈ 
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
	/// ����Ʈ ��� ������ �������̽� ��ȯ
	/// </summary>
	/// <returns></returns>
	virtual IGameObjectManager* GetGameObjectManager() override;
	/// <summary>
	/// ���ҽ� �Ŵ����� ��ȯ�մϴ�. 
	/// </summary>
	/// <returns></returns>
	virtual IResourceManager* GetResourceManager() override;
	/// <summary>
	/// ����� �Ŵ����� ��ȯ�մϴ�.
	/// </summary>
	/// <returns></returns>
	virtual IAudioManager* GetAudioManager() override;
	/// <summary>
	/// �鿣�� ���߿� �ν��Ͻ� ��ȯ
	/// </summary>
	/// <returns></returns>
	GameObjectManager* GetGameObjectManagerInstance();
	/// <summary>
	/// ī�޶� ��� ��ȯ
	/// </summary>
	/// <param name="">viewType::_2D �Ǵ� viewType::_3D ����</param>
	virtual void ChangeCameraViewType(Camera3D::ViewType viewType);

//INTERNAL
public:

	static const float getDeltaTime() { return dt;}
	
	void RegisterWindow(HINSTANCE hInstance); // WindowClassEX�� �ʱ�ȭ �Ѵ�.
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
	std::wstring window_title_wide = L""; // 2����Ʈ �����ڵ�
	std::string window_class = "";
	std::wstring window_class_wide = L""; // 2����Ʈ �����ڵ�

	int width = 0; //�ʺ�
	int height = 0; //����

	HWND handle = NULL; // ������ �ڵ�
	HINSTANCE hInstance = NULL; // ���ø����̼� �ν��Ͻ�

private:

	static float dt;
	Timer timer;
	
};

static Framework* framework;