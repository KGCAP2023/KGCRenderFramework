// KGCRenderFramework.cpp : 애플리케이션에 대한 진입점을 정의합니다.
#include "pch.h"
#include <Framework/Framework.h>

#pragma comment(lib, "Framework-lib.lib")

int APIENTRY wWinMain( _In_ HINSTANCE hInstance,
                       _In_opt_ HINSTANCE hPrevInstance,
                       _In_ LPWSTR    lpCmdLine,
                       _In_ int       nCmdShow )
{
	Framework rw;
	rw.Initialize(hInstance, "Model Viewer 1.0", "Framework", 1600, 900);
	rw.run();
    return 0;
}



