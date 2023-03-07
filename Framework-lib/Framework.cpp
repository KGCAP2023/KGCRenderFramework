#include "pch.h"
#include "Framework.h"



void Framework::run()
{

	while (ProcessMsg() == true)
	{
		Update();
		RenderFrame();
	}

}

void Framework::Update()
{

	float dt = timer.getDeltaTime();
	timer.ReStart();

	//std::async([&]
	//{
	//	std::cout << dt << std::endl;
	//});

	//콘솔출력 시스템콜 -> 성능저하
	//std::cout << dt << std::endl;

	float speed = 0.006f;

	auto kb = this->keyboard->GetState();
	auto mouse = this->mouse->GetState();

	//GameObject* walk = GameObject::gameObjects["walk"];

	//this->graphics.animation->Update(dt);

	if (mouse.rightButton)
	{
		std::cout << mouse.x << " " << mouse.y << std::endl;

		while (!this->yPosRelative.empty())
		{
			
			int yPosRelative = this->yPosRelative.front();
			int xPosRelative = this->xPosRelative.front();
			this->yPosRelative.pop();
			this->xPosRelative.pop();
			//std::cout << "Pos: " << xPosRelative <<" " << yPosRelative << std::endl;
			this->graphics.camera->transform.Rotate((float)yPosRelative * 0.001f, (float)xPosRelative * 0.001f, 0);
		}

	}


	while (!this->yPosRelative.empty())
	{
		this->yPosRelative.pop();
		this->xPosRelative.pop();
	}


	//if(mouse.leftButton)
	//{
	//	this->graphics.camera->transform.SetLookAtPos(XMFLOAT3(0.0f, 0.0f, 0.0f));
	//}
		
	

	if (kb.Back) // Backspace key is down
	{
		std::cout << "Back" << std::endl;
	} 

	if (kb.LeftShift) // Backspace key is down
	{
		speed = 0.01f;
	}

	if (kb.C) // Backspace key is down
	{
		this->graphics.camera->transform.Translate(0, -speed * dt,0);
	}

	if (kb.Space) // Backspace key is down
	{
		this->graphics.camera->transform.Translate(0, +speed * dt, 0);
	}

	if (kb.W) // W key is down
	{
		//std::cout << "w" << std::endl;
		//walk->transform.Translate(walk->transform.GetForward() * speed * dt);
		this->graphics.camera->transform.Translate(this->graphics.camera->transform.GetForward() * speed * dt);
	}
		
			

	if (kb.A) // A key is down
	{
		//std::cout << "a" << std::endl;
		this->graphics.camera->transform.Translate(this->graphics.camera->transform.GetLeft() * speed * dt);
	}

	if (kb.S) // S key is down
	{
		//std::cout << "s" << std::endl;
		//walk->transform.Translate(walk->transform.GetBackward() * speed * dt);
		this->graphics.camera->transform.Translate(this->graphics.camera->transform.GetBackward() * speed * dt);
	}

	if (kb.D) // D key is down
	{
		//std::cout << "d" << std::endl;
		this->graphics.camera->transform.Translate(this->graphics.camera->transform.GetRight() * speed * dt);
	}
		
	//if (kb.LeftShift)
			// Left shift key is down

	//if (kb.RightShift)
			// Right shift key is down

	if (kb.IsKeyDown(Keyboard::Keys::Enter) == true) // Keyboard::Keys
			std::cout << "Enter" << std::endl;




	
	
	//this->graphics.obj->Translate(0, 0, 0.008 * dt);
	
	//this->graphics.obj3->SetPosition(0,-13,-3);
	
	//this->graphics.camera.Translate(0,0,0.008*dt);

	

	bool change = true;

	

	//for (const auto& kv : GameObject::gameObjects) {

	//	if (change)
	//	{
	//		kv.second->SetPosition(10, 0, 10);
	//		kv.second->Rotate(speed * dt,0, 0);
	//		change = false;
	//	}
	//	else
	//	{
	//		kv.second->SetPosition(5, 0, 5);
	//		kv.second->Rotate(speed * dt, 0, speed * dt);
	//	}
	//}

	//this->graphics.camera.Rotate(0, speed * dt,0);

}

void Framework::RenderFrame()
{
	graphics.RenderFrame();
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
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, //Windows style - See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms632600(v=vs.85).aspx
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

	//this->keyboard = std::make_unique<Keyboard>();
	this->keyboard = std::make_shared<Keyboard>();
	this->mouse = std::make_unique<Mouse>();
	this->mouse->SetWindow(this->handle);

	

	std::cout << "[O] Successfully Completed KeyBoard/Mouse Initialize!" << std::endl;



	//DIRECTX 그래픽스 초기화
	if (!this->graphics.Initialize(this->handle, this->width, this->height,this->keyboard))
	{
		std::cout << "[X] FAILED Graphics Manager Initialize!" << std::endl;
		return false;
	}



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
				this->xPosRelative.push(raw->data.mouse.lLastX);
				this->yPosRelative.push(raw->data.mouse.lLastY);
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


