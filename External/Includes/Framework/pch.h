// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

#pragma once

#include <SDKDDKVer.h>

#ifdef _DEBUG
#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif
#endif

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
#include <wrl/client.h>

// C 런타임 헤더 파일 선언
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <atlstr.h>

// C++ 헤더파일 선언
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <future>
#include <queue>
#include <map>
#include <chrono>
#include <thread>

// Directx 11
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

extern "C"
{
	#include <Lua/lua.h>
	#include <Lua/lauxlib.h>
	#include <Lua/lualib.h>
}

#include <Fmod/fmod.hpp>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"DirectXTK.lib")
#pragma comment(lib,"DirectXTex.lib")
#pragma comment(lib,"assimp-vc143-mtd.lib")
#pragma comment(lib,"fmod_vc.lib")
#pragma comment(lib,"liblua54.a")

#include "Logger.h"

using namespace DirectX;