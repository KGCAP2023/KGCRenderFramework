﻿// KGCRenderFramework.cpp : 애플리케이션에 대한 진입점을 정의합니다.
#include "pch.h"
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <Framework/IFrameworkFactory.h>
#include <Framework/Framework.h>
#include <string>
#pragma comment(lib, "Framework-lib.lib")



static int selected = 0;

class Example : public ILayer
{
public:
	IGameObjectManager* _manager;

	bool my_tool_active = true;
	bool active = false;

	std::vector<GameObject*> gamelist;
	char a[20];

	Example(IGameObjectManager* manager, const std::string name) : ILayer(name)
	{
		this->_manager = manager;
		GameObject* obj = _manager->CreateGameObject("Object1");
		GameObject* obj1 = _manager->CreateGameObject("Object2");
		GameObject* obj2 = _manager->CreateGameObject("Object3");
		GameObject* obj3 = _manager->CreateGameObject("Object4");
		GameObject* obj4 = _manager->CreateGameObject("Object5");
		GameObject* obj5 = _manager->CreateGameObject("Object6");
		GameObject* obj6 = _manager->CreateGameObject("Object7");
		GameObject* obj7 = _manager->CreateGameObject("Object8");
		GameObject* obj8 = _manager->CreateGameObject("Object9");
		GameObject* obj9 = _manager->CreateGameObject("Object10");
		GameObject* obj10 = _manager->CreateGameObject("Object11");
		GameObject* obj11 = _manager->CreateGameObject("Object12");
		GameObject* obj12 = _manager->CreateGameObject("Object13");


		obj->transform.SetPosition(30, 20, 30);
		obj1->transform.SetPosition(10, 30, 15);
		obj2->transform.SetPosition(123, 20, 30);
		obj3->transform.SetPosition(13, 20, 30);
		obj4->transform.SetPosition(65, 20, 30);
		obj5->transform.SetPosition(23, 20, 30);
		obj6->transform.SetPosition(63, 20, 30);
		obj7->transform.SetPosition(71, 20, 30);
		obj8->transform.SetPosition(32, 20, 30);
		obj9->transform.SetPosition(10, 20, 30);
		obj10->transform.SetPosition(10, 20, 30);
		obj11->transform.SetPosition(10, 20, 30);
		obj12->transform.SetPosition(10, 20, 30);


		obj->transform.SetRotation(30, 20, 30);
		obj1->transform.SetRotation(10, 30, 15);
		obj2->transform.SetRotation(123, 20, 30);
		obj3->transform.SetRotation(13, 20, 30);
		obj4->transform.SetRotation(65, 20, 30);
		obj5->transform.SetRotation(23, 20, 30);
		obj6->transform.SetRotation(63, 20, 30);
		obj7->transform.SetRotation(71, 20, 30);
		obj8->transform.SetRotation(32, 20, 30);
		obj9->transform.SetRotation(10, 20, 30);
		obj10->transform.SetRotation(10, 20, 30);
		obj11->transform.SetRotation(10, 20, 30);
		obj12->transform.SetRotation(10, 20, 30);


		obj->transform.SetScale(30, 20, 30);
		obj1->transform.SetScale(10, 30, 15);
		obj2->transform.SetScale(123, 20, 30);
		obj3->transform.SetScale(13, 20, 30);
		obj4->transform.SetScale(65, 20, 30);
		obj5->transform.SetScale(23, 20, 30);
		obj6->transform.SetScale(63, 20, 30);
		obj7->transform.SetScale(71, 20, 30);
		obj8->transform.SetScale(32, 20, 30);
		obj9->transform.SetScale(10, 20, 30);
		obj10->transform.SetScale(10, 20, 30);
		obj11->transform.SetScale(10, 20, 30);
		obj12->transform.SetScale(10, 20, 30);

		gamelist.push_back(obj);
		gamelist.push_back(obj1);
		gamelist.push_back(obj2);
		gamelist.push_back(obj3);
		gamelist.push_back(obj4);
		gamelist.push_back(obj5);
		gamelist.push_back(obj6);
		gamelist.push_back(obj7);
		gamelist.push_back(obj8);
		gamelist.push_back(obj9);
		gamelist.push_back(obj10);
		gamelist.push_back(obj11);
		gamelist.push_back(obj12);

	}
	virtual ~Example() {};




	virtual void Init()
	{



	};

	virtual void Update()
	{

	};

	virtual void Render()
	{
		auto keyboard = InputManager::GetKeyboardState();
		auto mouse = InputManager::GetMouseState();


		if (my_tool_active) {
			ImGui::Begin(u8"Hierarchy View", &my_tool_active, ImGuiWindowFlags_MenuBar);

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::Button("add"))
				{

					if (active)
					{
						ImGui::SetNextWindowSize(ImVec2(520, 600));
						if (!ImGui::BeginPopupContextWindow("AddObject", ImGuiWindowFlags_Popup))
						{
							std::cout << active << std::endl;

							if (ImGui::MenuItem("Close")) { active = false; }
							ImGui::EndPopup();
						}
						else
							ImGui::Text("z");
						ImGui::EndPopup();
					}



				}
				if (ImGui::Button("delete"))
				{

					gamelist.at(selected)->Destroy();
					gamelist.erase(gamelist.begin() + selected);

				}

				ImGui::EndMenuBar();
			}

			ImGui::BeginChild("Scrolling", ImVec2(150, 0));
			for (int i = 0; i < gamelist.size(); ++i) {
				if (ImGui::Selectable(const_cast<char*>(gamelist.at(i)->ObjectName.c_str()), selected == i))
					selected = i;
			}

			static bool show = true;
			ImGui::ShowDemoWindow(&show);
			ImGui::EndChild();
			ImGui::SameLine();

			ImGui::BeginGroup();

			ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
			ImGui::Text(const_cast<char*>(gamelist.at(selected)->ObjectName.c_str()));
			ImGui::Separator();
			ImGui::Text("TRANSFORM");
			ImGui::SliderFloat3(u8"pos     X : Y : Z", &gamelist.at(selected)->transform.position.x, 0, 1600);
			ImGui::SliderFloat3(u8"roation X : Y : Z", &gamelist.at(selected)->transform.rotation.x, 0, 1600);
			ImGui::SliderFloat3(u8"scale :  X : Y : Z", &gamelist.at(selected)->transform.scale.x, 0, 1600);
			ImGui::Separator();
			ImGui::Text("Other Component");
			ImGui::Separator();
			ImGui::Button("Add Component");
			ImGui::EndChild();
			ImGui::SameLine();
			ImGui::EndGroup();
			ImGui::SameLine();

			ImGui::End();
		}
		ImGui::Begin(u8"AddObject");
		ImGui::InputText("name", a, IM_ARRAYSIZE(a));
		if (ImGui::Button("save")) {
			GameObject* ob = _manager->CreateGameObject(a);
			gamelist.push_back(ob);

		}

		ImGui::End();
		ImGui::SetNextWindowSize(ImVec2(520, 600));
		ImGui::Begin(u8"Debug View");
		ImGui::Text(u8"평균 프레임: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text(u8"마우스 좌표: X: %d, Y: %d", mouse.x, mouse.y);
		ImGui::Text(u8"입력 키 W: %d A: %d S: %d D: %d", keyboard.W, keyboard.A, keyboard.S, keyboard.D);
		ImGui::End();
	};
};


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	IFramework* rw = IFrameworkFactory::createFramework();

	rw->Initialize(hInstance, "KYONGGI CAPSTONE", "Framework", 1600, 900);

	IGameObjectManager* manager = rw->GetGameObjectManager();

	Example* temp = new Example(manager, "example");
	rw->RegisterLayer("example", temp);

	rw->run();
	return 0;
}