// KGCRenderFramework.cpp : 애플리케이션에 대한 진입점을 정의합니다.
#include "pch.h"
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <Framework/IFrameworkFactory.h>
#pragma comment(lib, "Framework-lib.lib")

class Example : public ILayer
{
public:
	Example(const std::string name) : ILayer(name) {}
	virtual ~Example() {};

	virtual void Init() 
	{


	};

	virtual void Update() 
	{
		std::cout << "test" << std::endl;

	};

	virtual void Render() 
	{
		ImGui::Begin(u8"정보창");
		ImGui::Text(u8"TEST");
		ImGui::End();
	};

};



int APIENTRY wWinMain( _In_ HINSTANCE hInstance,
                       _In_opt_ HINSTANCE hPrevInstance,
                       _In_ LPWSTR    lpCmdLine,
                       _In_ int       nCmdShow )
{
	IFramework* rw = IFrameworkFactory::createFramework();

	rw->Initialize(hInstance, "KYONGGI CAPSTONE", "Framework", 1600, 900);

	Example* temp = new Example("example");
    rw->RegisterLayer("example", temp);

	rw->run();
    return 0;
}



