
#pragma once
#include "pch.h"
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <Framework/IFrameworkFactory.h>
#include <Framework/Framework.h>
#include "hierarchy.h"
#include <Framework/ILayer.h>
#include <Framework/IFramework.h>
#include <imgui.h>
#include <cstring>

class HierarchyObject
{
public:
	HierarchyObject(GameObject* obj)
	{
		this->obj = obj;
	}

	GameObject* GetGameObject()
	{
		return obj;
	}

	void SetMappingValue(Component::Type type, std::string value)
	{
		this->map1.insert(std::make_pair<>(type, value));
	}
	void DeleteMappingValue(Component::Type type)
	{
		this->map1.erase(type);
	}
	std::string FindMappingValue(Component::Type name)
	{
		if (this->map1.find(name) != this->map1.end()) {
			return this->map1[name];
		}
		return std::string();
	}

	//삭제예정 컴포넌트 추가 
	void AddDeleteComponent(Component::Type type)
	{
		_delete.push_back(type);
	}

	//삭제예정 컴포넌트 일괄삭제 
	void DeleteComponent()
	{
		for (Component::Type a : _delete)
		{
			obj->RemoveComponent(a);
		}

		_delete.clear();
	}

private:
	GameObject* obj;
	std::map<Component::Type, std::string> map1;

	std::vector<Component::Type> _delete;

};

class Hierarchy : public ILayer
{
public:
	IFramework* framework = nullptr;
	IGameObjectManager* _manager;
	IResourceManager* ResM;
	Sprite* sp;
	std::vector<HierarchyObject*> gamelist;
	std::vector<Component*> componentlist;
	int selected = 0;
	bool check1 = true;
	bool check2 = true;
	bool check3 = true;
	int component_selected = 0;
	bool my_tool_active = true;
	bool active = false;
	bool component_active = false;
	char a[20]{};
	int g = 0;
	DirectX::XMFLOAT3 pos{};
	DirectX::XMFLOAT3 rot{};
	DirectX::XMFLOAT3 scale{};
	std::vector<std::string> spriteList;
	std::vector<std::string> tileList;
	std::vector<std::string> modelList;
	


	Hierarchy(IGameObjectManager* manager, IResourceManager* res, const std::string& name, IFramework* framework) : ILayer(name)
	{
		this->framework = framework;
		this->_manager = manager;
		this->ResM = res;



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



	

		gamelist.push_back(new HierarchyObject(obj));
		gamelist.push_back(new HierarchyObject(obj1));
		gamelist.push_back(new HierarchyObject(obj2));
		gamelist.push_back(new HierarchyObject(obj3));
		gamelist.push_back(new HierarchyObject(obj4));
		gamelist.push_back(new HierarchyObject(obj5));
		gamelist.push_back(new HierarchyObject(obj6));
		gamelist.push_back(new HierarchyObject(obj7));
		gamelist.push_back(new HierarchyObject(obj8));
	


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
	void component()
	{
		spriteList.clear();
		tileList.clear();

		auto map = ResM->GetSpriteMap();
		auto map1 = ResM->GetModelMap();
		auto map2 = ResM->GetTileMap();

		for (auto& pair : map)
		{
			Sprite* sp = pair.second;
			std::string name = sp->GetName();
			spriteList.push_back(name);

		}
		for (auto& pair1 : map1)
		{
			Model* md = pair1.second;
			std::string name1 = md->GetName();
			modelList.push_back(name1);

		}
		for (auto& pair2 : map2)
		{
			TileMap* tp = pair2.second;
			std::string name2 = tp->GetName();
			tileList.push_back(name2);

		}
		if (gamelist.at(selected)->GetGameObject()->GetComponentSize() != 0)
		{
			GameObject* obj = gamelist.at(selected)->GetGameObject();


			obj->ComponentForeach([&](Component* c) {
				int count = 0;
				std::string name = c->GetName();
				Component::Type type = c->GetType();

				switch (type)
				{
				case Component::Type::RENDERER_SPRITE:
				{
					SpriteRenderer* render4 = dynamic_cast<SpriteRenderer*>(c);
					componentlist.push_back(c);
					ImGui::Text(name.c_str());
					ImGui::SameLine();


					std::string f = gamelist.at(selected)->FindMappingValue(Component::Type::RENDERER_SPRITE);
					
					if (ImGui::BeginCombo("##combo", f.size()==0 ?"":f.c_str())) // The second parameter is the label previewed before opening the combo.
					{
						for (int n = 0; n < spriteList.size(); n++)
						{
							bool is_selected = (gamelist.at(selected)->FindMappingValue(Component::Type::RENDERER_SPRITE) == spriteList.at(n).c_str()); // You can store your selection however you want, outside or inside your objects
							if (ImGui::Selectable(spriteList[n].c_str(), is_selected))
							{
								gamelist.at(selected)->SetMappingValue(Component::Type::RENDERER_SPRITE,spriteList.at(n).c_str()); ;
								render4->AddSprite(ResM->FindSprite(spriteList.at(n).c_str()));
							}
							if (is_selected)
							{
								ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)

							}
						}
						ImGui::EndCombo();
					}
					ImGui::SameLine();

					if (ImGui::Button("Sprite Del"))
					{
						gamelist.at(selected)->AddDeleteComponent(Component::Type::RENDERER_SPRITE);
						gamelist.at(selected)->DeleteMappingValue(Component::Type::RENDERER_SPRITE);
					}



					ImGui::Separator();

					break;
				}

				case Component::Type::RENDERER_MODEL:
				{
					ModelRenderer* render5 = dynamic_cast<ModelRenderer*>(c);
					render5->SetModel(ResM->FindModel("test"));
					componentlist.push_back(c);
					ImGui::Text(name.c_str());
					ImGui::SameLine();

					std::string modelName = gamelist.at(selected)->FindMappingValue(Component::Type::RENDERER_MODEL);
					
					if (ImGui::BeginCombo("##model", modelName.size() == 0 ? "" : modelName.c_str())) // The second parameter is the label previewed before opening the combo.
					{
						for (int n = 0; n < spriteList.size(); n++)
						{
							bool is_selected = (gamelist.at(selected)->FindMappingValue(Component::Type::RENDERER_MODEL) == modelList.at(n).c_str()); // You can store your selection however you want, outside or inside your objects
							if (ImGui::Selectable(modelList[n].c_str(), is_selected))
							{
								gamelist.at(selected)->SetMappingValue(Component::Type::RENDERER_MODEL, modelList.at(n).c_str()); ;
								render5->SetModel(ResM->FindModel(modelList.at(n).c_str()));
							}
							if (is_selected)
							{
								ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)

							}
						}
						ImGui::EndCombo();
					}
					ImGui::SameLine();
					if (ImGui::Button("Model del"))
					{
						gamelist.at(selected)->AddDeleteComponent(Component::Type::RENDERER_MODEL);
						gamelist.at(selected)->DeleteMappingValue(Component::Type::RENDERER_MODEL);
					}

					ImGui::Separator();

					break;
				}

				case Component::Type::RENDERER_TILEMAP:
				{
					TileMapRenderer* render6 = dynamic_cast<TileMapRenderer*>(c);
					render6->AddTileMap(ResM->FindTileMap("test"));
					componentlist.push_back(c);
					ImGui::Text(name.c_str());
					ImGui::SameLine();
					std::string tileName = gamelist.at(selected)->FindMappingValue(Component::Type::RENDERER_TILEMAP);

					if (ImGui::BeginCombo("##tile", tileName.size() == 0 ? "" : tileName.c_str())) // The second parameter is the label previewed before opening the combo.
					{
						for (int n = 0; n < tileList.size(); n++)
						{
							bool is_selected = (gamelist.at(selected)->FindMappingValue(Component::Type::RENDERER_TILEMAP) == tileList.at(n).c_str()); // You can store your selection however you want, outside or inside your objects
							if (ImGui::Selectable(tileList[n].c_str(), is_selected))
							{
								gamelist.at(selected)->SetMappingValue(Component::Type::RENDERER_TILEMAP, tileList.at(n).c_str()); ;
								render6->AddTileMap(ResM->FindTileMap(tileList.at(n).c_str()));
							}
							if (is_selected)
							{
								ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)

							}
						}
						ImGui::EndCombo();
					}
					ImGui::SameLine();

					if (ImGui::Button("Tile del"))
					{
						gamelist.at(selected)->AddDeleteComponent(Component::Type::RENDERER_TILEMAP);
						gamelist.at(selected)->DeleteMappingValue(Component::Type::RENDERER_TILEMAP);
					}
					ImGui::Separator();

					break;
				}

				}




				});


				gamelist.at(selected)->DeleteComponent();



		}
		ImGui::Separator();

		ImGui::Separator();
		if (ImGui::Button("Add Component"))
		{
			component_active = true;
		}
	}

	
	virtual void Render()
	{
		if (active)
		{
			ImGui::Begin("Add Object", &active, ImGuiWindowFlags_MenuBar);


			ImGui::InputText("name", a, IM_ARRAYSIZE(a));
			ImGui::SliderFloat3(u8"pos     X : Y : Z", &pos.x, -100, 100);
			ImGui::SliderFloat3(u8"rot     X : Y : Z", &rot.x, -1.58, 1.58);
			ImGui::SliderFloat3(u8"scale     X : Y : Z", &scale.x, 1, 3);
			if (ImGui::Button("save"))
			{
				GameObject* ob = _manager->CreateGameObject(a);
				ob->transform.SetPosition(pos.x, pos.y, pos.z);
				ob->transform.SetRotation(rot.x, rot.y, rot.z);
				ob->transform.SetScale(scale.x, scale.y, scale.z);

				gamelist.push_back(new HierarchyObject(ob));
				gamelist.at(gamelist.size() - 1)->GetGameObject();

			}

			ImGui::End();
		}
		if (my_tool_active)
		{
			ImGui::Begin(u8"Hierarchy View", &my_tool_active, ImGuiWindowFlags_MenuBar);
			if (ImGui::Button("add"))
			{
				ImGui::SetNextWindowSize(ImVec2(550, 350), ImGuiCond_FirstUseEver);
				active = true;


			}
			ImGui::SameLine();
			if (ImGui::Button("delete"))
			{
				//selected=0~12 size 13
				if (selected > 0) 
				{
					if (selected == gamelist.size() - 1)
					{
						gamelist.at(selected)->GetGameObject()->Destroy();
						gamelist.pop_back();
						selected--;
					}
					else
					{
						gamelist.at(selected)->GetGameObject()->Destroy();
						gamelist.erase(gamelist.begin() + selected);
						
					}
				}
				else 
				{
					gamelist.at(selected)->GetGameObject()->Destroy();
					gamelist.pop_back();
				}


			}




			if (gamelist.size() != 0)
			{
				ImGui::BeginChild("Scrolling", ImVec2(150, 0));
				for (int i = 0; i < gamelist.size(); i++)
				{
					if (ImGui::Selectable((gamelist.at(i)->GetGameObject()->ObjectName.c_str()), selected == i))
						selected = i;
				}
				ImGui::EndChild();
				ImGui::SameLine();
				ImGui::BeginGroup();

				ImGui::Text(const_cast<char*>(gamelist.at(selected)->GetGameObject()->ObjectName.c_str()));
				ImGui::Separator();
				ImGui::Text("TRANSFORM");


				ImGui::SliderFloat3(u8"pos     X : Y : Z", &gamelist.at(selected)->GetGameObject()->transform.position.x, -100, 100);
				ImGui::SliderFloat3(u8"roation X : Y : Z", &gamelist.at(selected)->GetGameObject()->transform.rotation.x, -1.58, 1.58);
				ImGui::SliderFloat3(u8"scale :  X : Y : Z", &gamelist.at(selected)->GetGameObject()->transform.scale.x, 1, 3);

				ImGui::Separator();

				ImGui::Text("Other Component");
				ImGui::Separator();

				component();



				ImGui::SameLine();
				ImGui::EndGroup();
				ImGui::SameLine();

				
			}
			ImGui::End();
		}

				if (component_active)
				{
					ImGui::Begin("Add Component", &component_active, ImGuiWindowFlags_MenuBar);

					if (ImGui::Button("SpriteRenderer"))
					{
						GameObject* temp = gamelist.at(selected)->GetGameObject();
						SpriteRenderer* render1 = new SpriteRenderer(temp);
						gamelist.at(selected)->GetGameObject()->AddComponent(render1);

					}
					if (ImGui::Button("ModelRenderer"))
					{
						GameObject* temp = gamelist.at(selected)->GetGameObject();
						ModelRenderer* render2 = new ModelRenderer(temp);
						//render->Init();
						gamelist.at(selected)->GetGameObject()->AddComponent(render2);


					}
					if (ImGui::Button("TileMapRender"))
					{
						GameObject* temp1 = gamelist.at(selected)->GetGameObject();
						TileMapRenderer* render3 = new TileMapRenderer(temp1);
						//render->Init();
						gamelist.at(selected)->GetGameObject()->AddComponent(render3);
					}
					ImGui::End();
				}
			


		

	}
};
