#pragma once
#include "pch.h"
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <Framework/IFrameworkFactory.h>
#include <Framework/Framework.h>
#include <Framework/ILayer.h>
#include <Framework/IFramework.h>
#include <imgui.h>

static int selected = 0;

class Hierarchy : public ILayer 
{
public:

	IFramework* framework = nullptr;
	IGameObjectManager* _manager;
	std::vector<GameObject*> gamelist;
	int selected = 0;
	bool my_tool_active = true;
	bool active = false;
	char a[20]{};
	DirectX::XMFLOAT3 pos{};
	DirectX::XMFLOAT3 rot{};
	DirectX::XMFLOAT3 scale{};

	Hierarchy(IGameObjectManager* manager, const std::string& name, IFramework* framework) : ILayer(name)
	{
		this->framework = framework;
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
	
	virtual ~Hierarchy()
	{

	}

	virtual void Init()
	{

	}
	virtual void Update()
	{

	}

	virtual void Render()
	{
		if (my_tool_active) {
			ImGui::Begin(u8"Hierarchy View", &my_tool_active, ImGuiWindowFlags_MenuBar);
			if (ImGui::Button("add"))
			{
				active = true;


			}
			ImGui::SameLine();
			if (ImGui::Button("delete"))
			{
				//selected=0~12 size 13
				if (selected >= 0) {
					if (selected == gamelist.size() - 1)
					{
						gamelist.at(selected)->Destroy();
						gamelist.pop_back();
						selected--;
					}
					else
					{
						gamelist.at(selected)->Destroy();
						gamelist.erase(gamelist.begin() + selected);
					}
				}


			}



			ImGui::BeginChild("Scrolling", ImVec2(150, 0));
			if (gamelist.size() != 0)
			{
				for (int i = 0; i < gamelist.size(); i++) {
					if (ImGui::Selectable(const_cast<char*>(gamelist.at(i)->ObjectName.c_str()), selected == i))
						selected = i;
				}
			}
			static bool show = true;
			ImGui::ShowDemoWindow(&show);
			ImGui::EndChild();
			ImGui::SameLine();

			ImGui::BeginGroup();

			ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
			if (gamelist.size() != 0)
			{
				ImGui::Text(const_cast<char*>(gamelist.at(selected)->ObjectName.c_str()));
				ImGui::Separator();
				ImGui::Text("TRANSFORM");

				ImGui::SliderFloat3(u8"pos     X : Y : Z", &gamelist.at(selected)->transform.position.x, 0, 1600);
				ImGui::SliderFloat3(u8"roation X : Y : Z", &gamelist.at(selected)->transform.rotation.x, 0, 1600);
				ImGui::SliderFloat3(u8"scale :  X : Y : Z", &gamelist.at(selected)->transform.scale.x, 0, 1600);
			}

			ImGui::Separator();

			ImGui::Text("Other Component");

			ImGui::Separator();

			ImGui::Button("Add Component");

			ImGui::EndChild();
			ImGui::SameLine();
			ImGui::EndGroup();
			ImGui::SameLine();

			ImGui::End();
			if (active) {
				ImGui::Begin("add2", &active, ImGuiWindowFlags_MenuBar);


				ImGui::InputText("name", a, IM_ARRAYSIZE(a));
				ImGui::SliderFloat3(u8"pos     X : Y : Z", &pos.x, 0, 1600);
				ImGui::SliderFloat3(u8"rot     X : Y : Z", &rot.x, 0, 1600);
				ImGui::SliderFloat3(u8"scale     X : Y : Z", &scale.x, 0, 1600);
				if (ImGui::Button("save")) {
					GameObject* ob = _manager->CreateGameObject(a);
					ob->transform.SetPosition(pos.x, pos.y, pos.z);
					ob->transform.SetRotation(rot.x, rot.y, rot.z);
					ob->transform.SetScale(scale.x, scale.y, scale.z);

					gamelist.push_back(ob);

				}

				ImGui::End();
			}
		}


	}


};