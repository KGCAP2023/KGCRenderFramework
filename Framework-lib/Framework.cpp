#include "pch.h"
#include "Framework.h"

float Framework::dt;

void Framework::run()
{
	/*
	* FRAMEWORK GAME LOOP
	* 
	* ProcessMsg() : Win32창에대한 이벤트를 감지합니다. 
	* 마우스, 키보드에 대한 입력이 후킹되어있습니다.
	* 해상도 변경에 대한 처리로직이 등록 되어있습니다.
	* 
	* Update() : 업데이트 함수입니다. 
	* 해당 함수는 게임루프에 돌아가는 최상위 함수로
	* 이 함수내에서 매니저들이 다른 인터페이스의 Update()문을 
	* 옵저버 패턴등을 통해 전파(Propagate)합니다. 
	* 
	* RenderFrame() : 렌더링 작업을 하는 함수입니다.
	* 1프레임당 화면에 그릴 로직(DrawCall)이 포함됩니다.
	* 게임오브젝트의 Draw()가 호출됩니다.
	* 
	*/
	while (ProcessMsg() == true)
	{
		Update(); //업데이트 문입니다.
		RenderFrame(); //프레임을 렌더링 합니다. 
	}
	
}

IGameObjectManager* Framework::GetGameObjectManager()
{
	if (this->gameObjManager == nullptr)
	{
		gameObjManager = new GameObjectManager(this, SceneMode::DEV);
		
		testgameObjManager = new GameObjectManager(this, SceneMode::PLAY);

		currentgameObjManager = gameObjManager;
	}
	return currentgameObjManager;
}

GameObjectManager* Framework::GetGameObjectManagerInstance()
{
	if (this->gameObjManager == nullptr)
	{
		gameObjManager = new GameObjectManager(this, SceneMode::DEV);

		testgameObjManager = new GameObjectManager(this, SceneMode::PLAY);

		currentgameObjManager = gameObjManager;
	}
	return currentgameObjManager;
}

void Framework::SwitchObjectManager()
{
	if (currentgameObjManager == gameObjManager)
	{
		std::unordered_map<std::string, GameObject*> newGameObjects;
		for (const auto& gameObject : gameObjManager->gameObjects) {
			newGameObjects[gameObject.first] = new GameObject(*gameObject.second);
		}
		testgameObjManager->gameObjects = std::move(newGameObjects);
		currentgameObjManager = testgameObjManager;
		std::cout << SceneModeMap[(int)currentgameObjManager->GetMode()] << std::endl;
		this->layerManager.SetDockingSpace(false);
	}
	else
	{
		testgameObjManager->gameObjects.clear();
		currentgameObjManager = gameObjManager;
		std::cout << SceneModeMap[(int)currentgameObjManager->GetMode()] << std::endl;

	}
}

IGameObjectManager* Framework::GetCurrentGameObjectManager()
{
	return this->currentgameObjManager;
}

IResourceManager* Framework::GetResourceManager()
{
	return &this->resourceManager;
}

IAudioManager* Framework::GetAudioManager()
{
	return &this->audioManager;
}

LuaManager* Framework::GetLuaManager()
{
	return &this->luaManager;
}

void Framework::ChangeCameraViewType(Camera3D::ViewType _viewType)
{

	if(this->graphics.cameraComponent != nullptr) this->graphics.cameraComponent->ChangeProjectionValues(_viewType);

}

void Framework::Update()
{

	this->dt = timer.getDeltaTime();
	timer.ReStart();

	float speed = 0.006f;
	
	auto kb = this->InputManager.GetKeyboardState();
	auto mouse = this->InputManager.GetMouseState();
	std::queue<int>& xPosRelative = this->InputManager.GetXPoseRelative();
	std::queue<int>& yPosRelative = this->InputManager.GetYPoseRelative();

	this->layerManager.Update();

	for (auto& kv : this->gameObjManager->gameObjects) {
		kv.second->Update();
	}

	this->graphics.camera->Update();

	static bool F5_BUTTON_PRESSED = false;

	if (kb.F5)
	{
		if (!F5_BUTTON_PRESSED)
		{
			if (this->currentgameObjManager->GetMode() == SceneMode::PLAY)
			{
				this->SwitchObjectManager();
				this->layerManager.SetDockingSpace(true);
			}
		}
		F5_BUTTON_PRESSED = true;
	}
	else
	{
		F5_BUTTON_PRESSED = false;
	}

	static bool F2_BUTTON_PRESSED = false;
	static Camera3D::ViewType cameraType = Camera3D::ViewType::_3D;

	if (kb.F2)
	{
		if (!F2_BUTTON_PRESSED)
		{
			if (cameraType == Camera3D::ViewType::_2D)
			{
				this->graphics.cameraComponent->ChangeProjectionValues(Camera3D::ViewType::_3D);
				cameraType = Camera3D::ViewType::_3D;
				this->ray->SetOrthoGrahpicProjection(false);
			}
			else
			{
				this->graphics.cameraComponent->ChangeProjectionValues(Camera3D::ViewType::_2D);
				cameraType = Camera3D::ViewType::_2D;
				this->ray->SetOrthoGrahpicProjection(true);
			}
				
		}
		F2_BUTTON_PRESSED = true;
	}
	else
	{
		F2_BUTTON_PRESSED = false;
	}

	GameObjectManager* objMgr = this->GetGameObjectManagerInstance();
	static bool MOUSE_LEFT_BUTTON_PRESSED = false;

	if (mouse.leftButton && this->layerManager.isGameViewFocus())
	{
		if (!MOUSE_LEFT_BUTTON_PRESSED)
		{
			if ((0 <= mouse.x && mouse.x <= this->width) && (0 <= mouse.y && mouse.y <= this->height))
			{
				std::cout << "클릭된좌표:  " << mouse.x << "/" << mouse.y << std::endl;

				GameObject* selectedObject = nullptr;
				float min_dist = 10000;

				this->ray->CalculatePicking(mouse.x, mouse.y);
				for (auto& kv : objMgr->gameObjects) {
					kv.second->ComponentForeach([&](Component* c) {
						switch (c->GetType())
						{
							case Component::Type::RENDERER_MODEL:
							case Component::Type::RENDERER_SKINNED_MODEL:
							{
								BoundingBox3D* bbox = dynamic_cast<BoundingBox3D*>(dynamic_cast<Renderer*>(c)->GetBoundingBox());
								if (bbox != nullptr)
								{
									float dist = this->ray->isPicked(bbox);

									if (dist != -1 && dist < min_dist)
									{
										min_dist = dist;
										selectedObject = kv.second;
									}
								}
							}
							break;
						}
					});
				}

				if (selectedObject == nullptr)
				{
					for (auto& kv : objMgr->gameObjects) {
						kv.second->ComponentForeach([&](Component* c) {
							switch (c->GetType())
							{
								case Component::Type::RENDERER_SPRITE:
								case Component::Type::RENDERER_TILEMAP:
								{
									BoundingBox2D* bbox = dynamic_cast<BoundingBox2D*>(dynamic_cast<Renderer*>(c)->GetBoundingBox());
									int a = bbox->CalculatePointInBoundingBox(mouse.x, mouse.y);

									if (a == 1)
									{
										selectedObject = kv.second;
									}									
								}
								break;
							}
						});
					}

				}

				std::cout << "distance >" << min_dist << std::endl;

				if (selectedObject != nullptr)
				{
					std::cout <<"오브젝트: " << selectedObject->GetName() << "가 선택되었습니다." << std::endl;
					objMgr->notifyFousedObject(selectedObject);
					selectedObject->SetFocus();
				}
				else
				{
					//implement Release Focus
					GameObject::ReleaseFocus();
				}
			}
		}
		MOUSE_LEFT_BUTTON_PRESSED = true;
	}
	else
	{
		MOUSE_LEFT_BUTTON_PRESSED = false;
	}

	//=========================================

	if (mouse.rightButton && this->layerManager.isGameViewFocus())
	{
		while (!yPosRelative.empty())
		{
			int yPosRelative2 = yPosRelative.front();
			int xPosRelative2 = xPosRelative.front();
			yPosRelative.pop();
			xPosRelative.pop();
			this->graphics.camera->transform.Rotate((float)yPosRelative2 * 0.001f, (float)xPosRelative2 * 0.001f, 0);
		}

	}

	while (!yPosRelative.empty())
	{
		yPosRelative.pop();
		xPosRelative.pop();
	}

	//auto& io = ImGui::GetIO();
	//if (io.WantCaptureMouse || io.WantCaptureKeyboard) {
	//	return;
	//}
		
	if (kb.Back) // Backspace key is down
	{
		std::cout << "Back" << std::endl;
	} 

	if (kb.LeftShift && this->layerManager.isGameViewFocus()) // Backspace key is down
	{
		speed = 0.01f;
	}

	if (kb.C && this->layerManager.isGameViewFocus()) // Backspace key is down
	{
		this->graphics.camera->transform.Translate(0, -speed * dt,0);
	}

	if (kb.Space && this->layerManager.isGameViewFocus()) // Backspace key is down
	{
		this->graphics.camera->transform.Translate(0, +speed * dt, 0);
	}

	if (kb.W && this->layerManager.isGameViewFocus()) // W key is down
	{

		if (cameraType != Camera3D::ViewType::_2D)
		{
			this->graphics.camera->transform.Translate(this->graphics.camera->transform.GetForward() * speed * dt);
		}

	}

	if (kb.A && this->layerManager.isGameViewFocus()) // A key is down
	{
		this->graphics.camera->transform.Translate(this->graphics.camera->transform.GetLeft() * speed * dt);
	}

	if (kb.S && this->layerManager.isGameViewFocus()) // S key is down
	{
		if (cameraType != Camera3D::ViewType::_2D)
		{
			this->graphics.camera->transform.Translate(this->graphics.camera->transform.GetBackward() * speed * dt);
		}
	}

	if (kb.D && this->layerManager.isGameViewFocus()) // D key is down
	{
		this->graphics.camera->transform.Translate(this->graphics.camera->transform.GetRight() * speed * dt);
	}
		

}

void Framework::RenderFrame()
{
	graphics.RenderFrame();
}

void Framework::RegisterLayer(const std::string& key, ILayer* layer)
{
	this->layerManager.RegisterLayer(key,layer);
}

void Framework::DeleteLayer(const std::string& key)
{
	this->layerManager.DeleteLayer(key);
}

void Framework::AddMenubar(std::function<void()> callback)
{
	this->layerManager.AddMenubar(callback);
}

void Framework::SetImGuiDemo(bool value)
{
	this->layerManager.SetImGuiDemo(value);
}

ILayer* Framework::FindLayer(const std::string& key)
{
	return this->layerManager.FindLayer(key);
}

bool Framework::Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height)
{

	std::cout << "[=] Starting KDFramework....." << std::endl;

	this->timer.Start();

	this->hInstance = hInstance;
	this->width = width;
	this->height = height;
	this->window_title = window_title;
	this->window_title_wide = StringUtil::StringToWide(window_title);
	this->window_class = window_class;
	this->window_class_wide = StringUtil::StringToWide(window_class); //wide string representation of class string (used for registering class and creating window)

	this->RegisterWindow(hInstance);

	framework = this;

	int centerScreenX = GetSystemMetrics(SM_CXSCREEN) / 2 - this->width / 2;
	int centerScreenY = GetSystemMetrics(SM_CYSCREEN) / 2 - this->height / 2;

	//윈도우 바를 포함한 크기가 아니라 순수 창크기로 설정
	RECT rect; //Widow Rectangle
	rect.left = centerScreenX;
	rect.top = centerScreenY;
	rect.right = rect.left + this->width;
	rect.bottom = rect.top + this->height;

	AdjustWindowRect(&rect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

	this->handle = CreateWindowEx( 0 , //Extended Windows style - we are using the default. For other options, see: https://msdn.microsoft.com/en-us/library/windows/desktop/ff700543(v=vs.85).aspx
		this->window_class_wide.c_str(), //Window class name
		this->window_title_wide.c_str(), //Window Title
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_SIZEBOX, //Windows style - See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms632600(v=vs.85).aspx
		rect.left, //Window X Position
		rect.top, //Window Y Position
		rect.right - rect.left, //Window Width
		rect.bottom - rect.top, //Window Height
		NULL, //Handle to parent of this window. Since this is the first window, it has no parent window.
		NULL, //Handle to menu or child window identifier. Can be set to NULL and use menu in WindowClassEx if a menu is desired to be used.
		this->hInstance, //Handle to the instance of module to be used with this window
		nullptr); //Param to create window

	if (this->handle == NULL)
	{
		Logger::Log(GetLastError(), "CreateWindowEX Failed for window: " + this->window_title);
		return false;
	}

	ShowWindow(this->handle, SW_SHOW);
	SetForegroundWindow(this->handle);
	SetFocus(this->handle);

	std::cout << "[O] Successfully Completed Window Initialize!" << std::endl;

	/*
		<매니저 클래스는 여기서 초기화 해주세요> 
	*/

	//인풋 매니저 초기화
	this->InputManager.Init(this);

	//레이어 매니저 초기화
	this->layerManager.Init(this);

	//루아 스크립트 매니저 초기화
	if (!this->luaManager.Initialize(framework))
	{
		std::cout << "[X] FAILED LUA Manager Initialize!" << std::endl;
		return false;
	}

	//그래픽스 매니저 초기화
	if (!this->graphics.Initialize(framework,this->handle, this->width, this->height))
	{
		std::cout << "[X] FAILED Graphics Manager Initialize!" << std::endl;
		return false;
	}

	//오디오 매니저 초기화
	if (!this->audioManager.Initialize(framework))
	{
		std::cout << "[X] FAILED Audio Manager Initialize!" << std::endl;
		return false;
	}




	ray = new Ray(this);





	std::cout << "[O] Successfully Completed Manager Initialize!" << std::endl;

	return true;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
//extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT Framework::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
		return true;

	switch (uMsg)
	{
	case WM_ACTIVATE:
	case WM_ACTIVATEAPP:
		DirectX::Keyboard::ProcessMessage(uMsg, wParam, lParam);
		DirectX::Mouse::ProcessMessage(uMsg, wParam, lParam);
		break;
	case WM_INPUT:
	{
		UINT dwSize = sizeof(RAWINPUT);
		static BYTE lpb[sizeof(RAWINPUT)];

		GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));

		if (dwSize > 0)
		{
			RAWINPUT* raw = (RAWINPUT*)lpb;

			if (raw->header.dwType == RIM_TYPEMOUSE)
			{
				this->InputManager.PushRawInputData(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
			}
		}
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
		DirectX::Mouse::ProcessMessage(uMsg, wParam, lParam);
		break;
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		DirectX::Keyboard::ProcessMessage(uMsg, wParam, lParam);
		//unsigned char keycode = static_cast<unsigned char>(wParam);
		//std::cout << "key up" << keycode << std::endl;
		break;
	case WM_SYSKEYDOWN:
		DirectX::Keyboard::ProcessMessage(uMsg, wParam, lParam);
		if (wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000)
		{
			// This is where you'd implement the classic ALT+ENTER hotkey for fullscreen toggle
		}
		break;
	case WM_DPICHANGED:
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
		{
			//const int dpi = HIWORD(wParam);
			//printf("WM_DPICHANGED to %d (%.0f%%)\n", dpi, (float)dpi / 96.0f * 100.0f);
			const RECT* suggested_rect = (RECT*)lParam;
			::SetWindowPos(hwnd, NULL, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
		}
		break;
	case WM_SIZE:
		if (this->graphics.swapchain.Get() != nullptr)
		{
			INT nWidth = LOWORD(lParam);
			INT nHeight = HIWORD(lParam);

			this->width = nWidth;
			this->height = nHeight;

			this->graphics.ResizeWindow(nWidth, nHeight);

		}
		return 1;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

}

bool Framework::ProcessMsg()
{
	// Handle the windows messages.
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG)); // Initialize the message structure.


	while (PeekMessage(&msg, //Where to store message (if one exists) See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms644943(v=vs.85).aspx
		this->handle, //Handle to window we are checking messages for
		0,    //Minimum Filter Msg Value - We are not filtering for specific messages, but the min/max could be used to filter only mouse messages for example.
		0,    //Maximum Filter Msg Value
		PM_REMOVE))//Remove message after capturing it via PeekMessage. For more argument options, see: https://msdn.microsoft.com/en-us/library/windows/desktop/ms644943(v=vs.85).aspx
	{
		TranslateMessage(&msg); //Translate message from virtual key messages into character messages so we can dispatch the message. See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms644955(v=vs.85).aspx
		DispatchMessage(&msg); //Dispatch message to our Window Proc for this window. See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms644934(v=vs.85).aspx
	}

	// Check if the window was closed
	if (msg.message == WM_NULL)
	{
		if (!IsWindow(this->handle))
		{
			this->handle = NULL; //Message processing loop takes care of destroying this window
			UnregisterClass(this->window_class_wide.c_str(), this->hInstance);
			return false;
		}
	}

	return true;
}

Framework::~Framework()
{
	if (this->handle != NULL)
	{
		std::cout << "FRAMEWORK IS SHUTDOWN" << std::endl;
		UnregisterClass(this->window_class_wide.c_str(), this->hInstance);
		framework = nullptr;
		DestroyWindow(handle);
	}
}

LRESULT CALLBACK HandleWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_CLOSE:
		{
			DestroyWindow(hwnd);
			return 0;
		}
		default:
		{
			return framework->WindowProc(hwnd,uMsg,wParam,lParam);
		}
	}
}

void Framework::RegisterWindow(HINSTANCE hInstance)
{
	//윈도우 클래스
	WNDCLASSEX wc;	//Our Window Class (This has to be filled before our window can be created) See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms633577(v=vs.85).aspx
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	//Flags [Redraw on width/height change from resize/movement] See: https://msdn.microsoft.com/en-us/library/windows/desktop/ff729176(v=vs.85).aspx
	//wc.lpfnWndProc = DefWindowProc;
	wc.lpfnWndProc = HandleWindowProc; //Pointer to Window Proc function for handling messages from this window
	wc.cbClsExtra = 0; //# of extra bytes to allocate following the window-class structure. We are not currently using this.
	wc.cbWndExtra = 0; //# of extra bytes to allocate following the window instance. We are not currently using this.
	wc.hInstance = hInstance; //Handle to the instance that contains the Window Procedure
	wc.hIcon = NULL;   //Handle to the class icon. Must be a handle to an icon resource. We are not currently assigning an icon, so this is null.
	wc.hIconSm = NULL; //Handle to small icon for this class. We are not currently assigning an icon, so this is null.
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); //Default Cursor - If we leave this null, we have to explicitly set the cursor's shape each time it enters the window.
	wc.hbrBackground = NULL; //Handle to the class background brush for the window's background color - we will leave this blank for now and later set this to black. For stock brushes, see: https://msdn.microsoft.com/en-us/library/windows/desktop/dd144925(v=vs.85).aspx
	wc.lpszMenuName = NULL; //Pointer to a null terminated character string for the menu. We are not using a menu yet, so this will be NULL.
	wc.lpszClassName = this->window_class_wide.c_str(); //Pointer to null terminated string of our class name for this window.
	wc.cbSize = sizeof(WNDCLASSEX); //Need to fill in the size of our struct for cbSize
	RegisterClassEx(&wc); // Register the class so that it is usable.

}


