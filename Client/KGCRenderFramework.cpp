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

class Example1 : public ILayer
{
public:
	Example1(const std::string name) : ILayer(name) {}
	virtual ~Example1() {};

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
		ImGui::Text(u8"TEST1");
		ImGui::End();
	};


};


int APIENTRY wWinMain( _In_ HINSTANCE hInstance,
                       _In_opt_ HINSTANCE hPrevInstance,
                       _In_ LPWSTR    lpCmdLine,
                       _In_ int       nCmdShow )
{
	IFramework* rw = IFrameworkFactory::createFramework();
	IFramework* rw1 = IFrameworkFactory::createFramework();

	rw->Initialize(hInstance, "KYONGGI CAPSTONE", "Framework", 1600, 900);

	Example* temp = new Example("example");
    rw->RegisterLayer("example", temp);

	Example1* temp1 = new Example1("example1");
	rw1->RegisterLayer("example1", temp1);




	rw->run();
    return 0;
}



