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
	int MappingCount()
	{
		return this->map1.size();
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
	bool Detail()
	{
		return detail;
	}
	void SetDetail(bool b)
	{

		this->detail = b;
	}

private:
	GameObject* obj;
	std::map<Component::Type, std::string> map1;
	bool detail = false;
	std::vector<Component::Type> _delete;

};
static bool testMode = false;




class Hierarchy : public ILayer
{
public:
	IFramework* framework = nullptr;
	IGameObjectManager* _manager;
	IResourceManager* ResM;
	Sprite* sp;
	std::vector<HierarchyObject*> gamelist;
	std::vector<HierarchyObject*> gameTestList;
	std::vector<Component*> componentlist;
	bool check1 = true;
	bool check2 = true;
	bool check3 = true;

	int component_selected = 0;
	int selected = 0;
	bool my_tool_active = true;
	bool active = false;
	bool show_warning = false;
	bool show_delete = false;
	bool component_active = false;
	bool show_render = false;
	char a[20]{};
	int g = 0;
	DirectX::XMFLOAT3 pos{};
	DirectX::XMFLOAT3 rot{};
	DirectX::XMFLOAT3 scale{ 1,1,1 };
	std::vector<std::string> spriteList;
	std::vector<std::string> tileList;
	std::vector<std::string> modelList;
	std::vector<std::string> testList;


	Hierarchy(IGameObjectManager* manager, IResourceManager* res, const std::string& name, IFramework* framework) : ILayer(name)
	{
		this->framework = framework;
		this->_manager = manager;
		this->ResM = res;

		this->_manager->AddFocusedObjectListener([&](GameObject* selectedObject) {

			for (int k = 0; k < gamelist.size(); k++)
			{
				if (selectedObject->GetName() == gamelist.at(k)->GetGameObject()->GetName())
				{
					selected = k;
				}

			}

			});









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
	void testMode()
	{
		for (int a = 0; a < gamelist.size(); a++)
		{
			gameTestList.push_back(new HierarchyObject(gamelist.at(a)->GetGameObject()));
		}

	}
	void component()
	{
		spriteList.clear();
		tileList.clear();

		testList.clear();

		auto map = ResM->GetSpriteMap();
		auto map1 = ResM->GetModelMap();
		auto map2 = ResM->GetTileMap();
		auto map3 = _manager->GetObejctMap();

		for (auto& pair : map3)
		{
			GameObject* obj = pair.second;
			std::string name3 = obj->GetName();
			testList.push_back(name3);
		}
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
					ImGui::PushItemWidth(100);
					if (ImGui::BeginCombo("##combo", f.size() == 0 ? "" : f.c_str())) // The second parameter is the label previewed before opening the combo.
					{
						for (int n = 0; n < spriteList.size(); n++)
						{
							bool is_selected = (gamelist.at(selected)->FindMappingValue(Component::Type::RENDERER_SPRITE) == spriteList.at(n).c_str()); // You can store your selection however you want, outside or inside your objects
							if (ImGui::Selectable(spriteList[n].c_str(), is_selected))
							{
								gamelist.at(selected)->SetMappingValue(Component::Type::RENDERER_SPRITE, spriteList.at(n).c_str()); ;
								render4->SetSprite(ResM->FindSprite(spriteList.at(n).c_str()));
							}
							if (is_selected)
							{
								ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)

							}
						}
						ImGui::EndCombo();
					}
					ImGui::PopItemWidth();
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
					componentlist.push_back(c);
					ImGui::Text(name.c_str());
					ImGui::SameLine();

					std::string modelName = gamelist.at(selected)->FindMappingValue(Component::Type::RENDERER_MODEL);
					ImGui::PushItemWidth(100);
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
					ImGui::PopItemWidth();
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

					componentlist.push_back(c);
					ImGui::Text(name.c_str());
					ImGui::SameLine();
					std::string tileName = gamelist.at(selected)->FindMappingValue(Component::Type::RENDERER_TILEMAP);
					ImGui::PushItemWidth(100);
					if (ImGui::BeginCombo("##tile", tileName.size() == 0 ? "" : tileName.c_str())) // The second parameter is the label previewed before opening the combo.
					{
						for (int n = 0; n < tileList.size(); n++)
						{
							bool is_selected = (gamelist.at(selected)->FindMappingValue(Component::Type::RENDERER_TILEMAP) == tileList.at(n).c_str()); // You can store your selection however you want, outside or inside your objects
							if (ImGui::Selectable(tileList[n].c_str(), is_selected))
							{
								gamelist.at(selected)->SetMappingValue(Component::Type::RENDERER_TILEMAP, tileList.at(n).c_str()); ;
								render6->SetTileMap(ResM->FindTileMap(tileList.at(n).c_str()));
							}
							if (is_selected)
							{
								ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)

							}
						}
						ImGui::EndCombo();
					}
					ImGui::PopItemWidth();

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
		ImGui::Text("                        ");
		ImGui::SameLine();
		if (ImGui::Button("Add Component"))
		{
			if (gamelist.at(selected)->GetGameObject()->GetComponentSize() == 0)
			{
				component_active = true;
			}
			else
				show_render = true;
		}
	}


	/// <summary>
	/// 팝업 기능을 이용해서 경고창 생성하는 코드 모아둔 함수 입니다.
	/// </summary>
	void Warning()
	{
		if (show_warning) {
			ImGui::OpenPopup("Warning");
		}

		if (ImGui::BeginPopupModal("Warning", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text(u8"중복 오브젝트 생성");
			if (ImGui::Button("OK") || ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter))) {
				ImGui::CloseCurrentPopup();
				show_warning = false;
			}
			ImGui::EndPopup();
		}
		if (show_render) {
			ImGui::OpenPopup("Error");
		}

		if (ImGui::BeginPopupModal("Error", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text(u8"렌더러는 1개만 가능합니다");
			if (ImGui::Button("OK") || ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter))) {
				ImGui::CloseCurrentPopup();
				show_render = false;
			}
			ImGui::EndPopup();
		}
		if (show_delete) {
			ImGui::OpenPopup("Delete");
		}

		if (ImGui::BeginPopupModal("Delete", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text(u8"삭제 되었습니다.");
			if (ImGui::Button(u8"확인") || ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)))
			{

				ImGui::CloseCurrentPopup();
				show_delete = false;

			}
			ImGui::EndPopup();
		}

	}
	/// <summary>
	/// inputfloat 할떄 최대값 최소값 넘어가면 최대값 최소값으로 설정하게 만들려고 만든 함수 입니다.
	/// </summary>
	void CheckSpriteTransform()
	{
		float posX = gamelist.at(selected)->GetGameObject()->transform.position.x;
		float posY = gamelist.at(selected)->GetGameObject()->transform.position.y;
		float rotZ = gamelist.at(selected)->GetGameObject()->transform.rotation.z;


		float minPosX = 0.0f;
		float maxPosX = 1400.0f;

		float minPosY = 000.0f;
		float maxPosY = 1600.0f;


		float minRotValue = -1.58f;
		float maxRotValue = 1.58f;


		float minScaleValue = 1.0f;
		float maxScaleValue = 3.0f;

		if (posX < minPosX) {
			posX = minPosX;
			gamelist.at(selected)->GetGameObject()->transform.position.x = posX;
		}
		else if (posX > maxPosX) {
			posX = maxPosX;
			gamelist.at(selected)->GetGameObject()->transform.position.x = posX;
		}

		if (posY < minPosY) {
			posY = minPosY;
			gamelist.at(selected)->GetGameObject()->transform.position.y = posY;
		}
		else if (posY > maxPosY) {
			posY = maxPosY;
			gamelist.at(selected)->GetGameObject()->transform.position.y = posY;
		}

		if (rotZ < minRotValue) {
			rotZ = minRotValue;
			gamelist.at(selected)->GetGameObject()->transform.rotation.z = rotZ;
		}
		else if (rotZ > maxRotValue) {
			rotZ = maxRotValue;
			gamelist.at(selected)->GetGameObject()->transform.rotation.z = rotZ;
		}

		float scaleX = gamelist.at(selected)->GetGameObject()->transform.scale.x;

		if (scaleX < minScaleValue) {
			scaleX = minScaleValue;
			gamelist.at(selected)->GetGameObject()->transform.position.x = scaleX;
		}
		else if (scaleX > maxScaleValue) {
			scaleX = maxScaleValue;
			gamelist.at(selected)->GetGameObject()->transform.position.x = scaleX;
		}


	}

	void CheckTransform()
	{
		float posX = gamelist.at(selected)->GetGameObject()->transform.position.x;
		float posY = gamelist.at(selected)->GetGameObject()->transform.position.y;
		float posZ = gamelist.at(selected)->GetGameObject()->transform.position.z;

		float rotX = gamelist.at(selected)->GetGameObject()->transform.rotation.x;
		float rotY = gamelist.at(selected)->GetGameObject()->transform.rotation.y;
		float rotZ = gamelist.at(selected)->GetGameObject()->transform.rotation.z;

		float scaleX = gamelist.at(selected)->GetGameObject()->transform.scale.x;
		float scaleY = gamelist.at(selected)->GetGameObject()->transform.scale.y;
		float scaleZ = gamelist.at(selected)->GetGameObject()->transform.scale.z;


		float minPosValue = -100.0f;
		float maxPosValue = 100.0f;

		float minRotValue = -1.58f;
		float maxRotValue = 1.58f;

		float minScaleValue = 1.0f;
		float maxScaleValue = 3.0f;

		/// <summary>
		/// Pos x , y ,z 에 대한 값이 -100 보다 작거나 100보다 크면 -100 ,100 값으로 치환 시켜주는 곳
		/// </summary>
		if (posX < minPosValue) {
			posX = minPosValue;
			gamelist.at(selected)->GetGameObject()->transform.position.x = posX;
		}
		else if (posX > maxPosValue) {
			posX = maxPosValue;
			gamelist.at(selected)->GetGameObject()->transform.position.x = posX;
		}

		if (posY < minPosValue) {
			posY = minPosValue;
			gamelist.at(selected)->GetGameObject()->transform.position.y = posY;
		}
		else if (posY > maxPosValue) {
			posY = maxPosValue;
			gamelist.at(selected)->GetGameObject()->transform.position.y = posY;
		}

		if (posZ < minPosValue) {
			posZ = minPosValue;
			gamelist.at(selected)->GetGameObject()->transform.position.z = posZ;
		}
		else if (posZ > maxPosValue) {
			posZ = maxPosValue;
			gamelist.at(selected)->GetGameObject()->transform.position.z = posZ;
		}
		/// <summary>
		/// Rot x , y ,z 에 대한 값이 -1.58 보다 작거나 1.58보다 크면 -1.58 ,1.58 값으로 치환 시켜주는 곳
		/// </summary>
		if (rotX < minRotValue) {
			rotX = minRotValue;
			gamelist.at(selected)->GetGameObject()->transform.rotation.x = rotX;
		}
		else if (rotX > maxRotValue) {
			rotX = maxRotValue;
			gamelist.at(selected)->GetGameObject()->transform.rotation.x = rotX;
		}

		if (rotY < minRotValue) {
			rotY = minRotValue;
			gamelist.at(selected)->GetGameObject()->transform.rotation.y = rotY;
		}
		else if (rotY > maxRotValue) {
			rotY = maxRotValue;
			gamelist.at(selected)->GetGameObject()->transform.rotation.y = rotY;
		}

		if (rotZ < minRotValue) {
			rotZ = minRotValue;
			gamelist.at(selected)->GetGameObject()->transform.rotation.z = rotZ;
		}
		else if (rotZ > maxRotValue) {
			rotZ = maxRotValue;
			gamelist.at(selected)->GetGameObject()->transform.rotation.z = rotZ;
		}
		/// <summary>
		/// Scale x , y ,z 에 대한 값이 1 보다 작거나 3보다 크면 1 ,3 값으로 치환 시켜주는 곳
		/// </summary>
		if (scaleX < minScaleValue) {
			scaleX = minScaleValue;
			gamelist.at(selected)->GetGameObject()->transform.position.x = scaleX;
		}
		else if (scaleX > maxScaleValue) {
			scaleX = maxScaleValue;
			gamelist.at(selected)->GetGameObject()->transform.position.x = scaleX;
		}

		if (scaleY < minScaleValue) {
			scaleY = minScaleValue;
			gamelist.at(selected)->GetGameObject()->transform.position.y = scaleY;
		}
		else if (scaleY > maxScaleValue) {
			scaleY = maxScaleValue;
			gamelist.at(selected)->GetGameObject()->transform.position.y = scaleY;
		}

		if (scaleZ < minScaleValue) {
			scaleZ = minScaleValue;
			gamelist.at(selected)->GetGameObject()->transform.position.z = scaleZ;
		}
		else if (scaleZ > maxScaleValue) {
			scaleZ = maxScaleValue;
			gamelist.at(selected)->GetGameObject()->transform.position.z = scaleZ;
		}

	}
	/// <summary>
	/// Object의 pos,rot,scale 크기 조절하는 곳 
	/// </summary>
	void Transform()
	{

		if (gamelist.at(selected)->GetGameObject()->GetComponentSize() == 0)
		{
			ImGui::PushItemWidth(90);

			if (gamelist.at(selected)->Detail() == false)
			{

				ImGui::Separator();
				ImGui::Text("POS");
				ImGui::Text("X:");
				ImGui::SameLine();
				ImGui::SliderFloat(u8"##pos", &gamelist.at(selected)->GetGameObject()->transform.position.x, -100, 100);
				ImGui::SameLine();
				ImGui::Text("Y:");
				ImGui::SameLine();
				ImGui::SliderFloat(u8"##pos1", &gamelist.at(selected)->GetGameObject()->transform.position.y, -100, 100);
				ImGui::SameLine();
				ImGui::Text("Z:");
				ImGui::SameLine();
				ImGui::SliderFloat(u8"##pos2", &gamelist.at(selected)->GetGameObject()->transform.position.z, -100, 100);


				ImGui::Separator();
				ImGui::Text("ROTATION");
				ImGui::Text("X:");
				ImGui::SameLine();
				ImGui::SliderFloat(u8"##rot", &gamelist.at(selected)->GetGameObject()->transform.rotation.x, -1.58, 1.58);
				ImGui::SameLine();
				ImGui::Text("Y:");
				ImGui::SameLine();
				ImGui::SliderFloat(u8"##rot1", &gamelist.at(selected)->GetGameObject()->transform.rotation.y, -1.58, 1.58);
				ImGui::SameLine();
				ImGui::Text("Z:");
				ImGui::SameLine();
				ImGui::SliderFloat(u8"##rot2", &gamelist.at(selected)->GetGameObject()->transform.rotation.z, -1.58, 1.58);

				ImGui::Separator();
				ImGui::Text("SCALE");
				ImGui::Text("X:");
				ImGui::SameLine();
				ImGui::SliderFloat(u8"##scale", &gamelist.at(selected)->GetGameObject()->transform.scale.x, 1, 3);
				ImGui::SameLine();
				ImGui::Text("Y:");
				ImGui::SameLine();
				ImGui::SliderFloat(u8"##scale1", &gamelist.at(selected)->GetGameObject()->transform.scale.y, 1, 3);
				ImGui::SameLine();
				ImGui::Text("Z:");
				ImGui::SameLine();
				ImGui::SliderFloat(u8"##scale2", &gamelist.at(selected)->GetGameObject()->transform.scale.z, 1, 3);
				ImGui::PopItemWidth();


			}
			else
			{
				CheckTransform();

				ImGui::Separator();
				ImGui::Text("POS");
				ImGui::Text("X:");
				ImGui::SameLine();
				ImGui::InputFloat(u8"##pos3", &gamelist.at(selected)->GetGameObject()->transform.position.x);
				ImGui::SameLine();
				ImGui::Text("Y:");
				ImGui::SameLine();
				ImGui::InputFloat(u8"##pos4", &gamelist.at(selected)->GetGameObject()->transform.position.y);
				ImGui::SameLine();
				ImGui::Text("Z:");
				ImGui::SameLine();
				ImGui::InputFloat(u8"##pos5", &gamelist.at(selected)->GetGameObject()->transform.position.z);

				ImGui::Separator();
				ImGui::Text("ROTATION");
				ImGui::Text("X:");
				ImGui::SameLine();
				ImGui::InputFloat(u8"##rot3", &gamelist.at(selected)->GetGameObject()->transform.rotation.x, -1.58f, 1.58f);
				ImGui::SameLine();
				ImGui::Text("Y:");
				ImGui::SameLine();
				ImGui::InputFloat(u8"##rot4", &gamelist.at(selected)->GetGameObject()->transform.rotation.y, -1.58f, 1.58f);
				ImGui::SameLine();
				ImGui::Text("Z:");
				ImGui::SameLine();
				ImGui::InputFloat(u8"##rot5", &gamelist.at(selected)->GetGameObject()->transform.rotation.z, -1.58f, 1.58f);

				ImGui::Separator();
				ImGui::Text("SCALE");
				ImGui::Text("X:");
				ImGui::SameLine();
				ImGui::InputFloat(u8"##scale3", &gamelist.at(selected)->GetGameObject()->transform.scale.x);
				ImGui::SameLine();
				ImGui::Text("Y:");
				ImGui::SameLine();
				ImGui::InputFloat(u8"##scale4", &gamelist.at(selected)->GetGameObject()->transform.scale.y);
				ImGui::SameLine();
				ImGui::Text("Z:");
				ImGui::SameLine();
				ImGui::InputFloat(u8"##scale5", &gamelist.at(selected)->GetGameObject()->transform.scale.z);
				ImGui::PopItemWidth();



			}
			ImGui::Separator();
			bool check_detail = gamelist.at(selected)->Detail();
			ImGui::Checkbox("Detail", &check_detail);
			gamelist.at(selected)->SetDetail(check_detail);

			ImGui::Separator();

		}
		else
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

					if (gamelist.at(selected)->Detail() == false)
					{
						ImGui::PushItemWidth(90);

						ImGui::Separator();
						ImGui::Text("POS");
						ImGui::Text("X:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##pos", &gamelist.at(selected)->GetGameObject()->transform.position.x, 0, 1400);
						ImGui::SameLine();
						ImGui::Text("Y:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##pos1", &gamelist.at(selected)->GetGameObject()->transform.position.y, 0, 1600);

						ImGui::Separator();
						ImGui::Text("ROTATION");
						ImGui::Text("Z:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##rot2", &gamelist.at(selected)->GetGameObject()->transform.rotation.z, -1.58, 1.58);

						ImGui::Separator();
						ImGui::Text("SCALE");
						ImGui::Text("X:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##scale", &gamelist.at(selected)->GetGameObject()->transform.scale.x, 1, 3);
						ImGui::PopItemWidth();



					}
					else
					{
						CheckSpriteTransform();

						ImGui::PushItemWidth(90);

						ImGui::Separator();
						ImGui::Text("POS");
						ImGui::Text("X:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##pos3", &gamelist.at(selected)->GetGameObject()->transform.position.x);
						ImGui::SameLine();
						ImGui::Text("Y:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##pos4", &gamelist.at(selected)->GetGameObject()->transform.position.y);


						ImGui::Separator();
						ImGui::Text("ROTATION");
						ImGui::Text("Z:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##rot5", &gamelist.at(selected)->GetGameObject()->transform.rotation.z, -1.58f, 1.58f);

						ImGui::Separator();
						ImGui::Text("SCALE");
						ImGui::Text("X:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##scale3", &gamelist.at(selected)->GetGameObject()->transform.scale.x);
						ImGui::PopItemWidth();


					}
					ImGui::Separator();
					bool check_detail = gamelist.at(selected)->Detail();
					ImGui::Checkbox("Detail", &check_detail);
					gamelist.at(selected)->SetDetail(check_detail);

					ImGui::Separator();
					break;
				}

				case Component::Type::RENDERER_MODEL:
				{
					if (gamelist.at(selected)->Detail() == false)
					{
						ImGui::PushItemWidth(90);

						ImGui::Separator();
						ImGui::Text("POS");
						ImGui::Text("X:");

						ImGui::SameLine();
						ImGui::SliderFloat(u8"##pos", &gamelist.at(selected)->GetGameObject()->transform.position.x, -100, 100);
						ImGui::SameLine();
						ImGui::Text("Y:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##pos1", &gamelist.at(selected)->GetGameObject()->transform.position.y, -100, 100);
						ImGui::SameLine();
						ImGui::Text("Z:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##pos2", &gamelist.at(selected)->GetGameObject()->transform.position.z, -100, 100);


						ImGui::Separator();
						ImGui::Text("ROTATION");
						ImGui::Text("X:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##rot", &gamelist.at(selected)->GetGameObject()->transform.rotation.x, -1.58, 1.58);
						ImGui::SameLine();
						ImGui::Text("Y:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##rot1", &gamelist.at(selected)->GetGameObject()->transform.rotation.y, -1.58, 1.58);
						ImGui::SameLine();
						ImGui::Text("Z:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##rot2", &gamelist.at(selected)->GetGameObject()->transform.rotation.z, -1.58, 1.58);

						ImGui::Separator();
						ImGui::Text("SCALE");
						ImGui::Text("X:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##scale", &gamelist.at(selected)->GetGameObject()->transform.scale.x, 1, 3);
						ImGui::SameLine();
						ImGui::Text("Y:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##scale1", &gamelist.at(selected)->GetGameObject()->transform.scale.y, 1, 3);
						ImGui::SameLine();
						ImGui::Text("Z:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##scale2", &gamelist.at(selected)->GetGameObject()->transform.scale.z, 1, 3);
						ImGui::PopItemWidth();


					}
					else
					{
						CheckTransform();

						ImGui::PushItemWidth(90);

						ImGui::Separator();
						ImGui::Text("POS");
						ImGui::Text("X:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##pos3", &gamelist.at(selected)->GetGameObject()->transform.position.x);
						ImGui::SameLine();
						ImGui::Text("Y:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##pos4", &gamelist.at(selected)->GetGameObject()->transform.position.y);
						ImGui::SameLine();
						ImGui::Text("Z:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##pos5", &gamelist.at(selected)->GetGameObject()->transform.position.z);


						ImGui::Separator();
						ImGui::Text("ROTATION");
						ImGui::Text("X:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##rot3", &gamelist.at(selected)->GetGameObject()->transform.rotation.x, -1.58f, 1.58f);
						ImGui::SameLine();
						ImGui::Text("Y:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##rot4", &gamelist.at(selected)->GetGameObject()->transform.rotation.y, -1.58f, 1.58f);
						ImGui::SameLine();
						ImGui::Text("Z:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##rot5", &gamelist.at(selected)->GetGameObject()->transform.rotation.z, -1.58f, 1.58f);

						ImGui::Separator();
						ImGui::Text("SCALE");
						ImGui::Text("X:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##scale3", &gamelist.at(selected)->GetGameObject()->transform.scale.x);
						ImGui::SameLine();
						ImGui::Text("Y:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##scale4", &gamelist.at(selected)->GetGameObject()->transform.scale.y);
						ImGui::SameLine();
						ImGui::Text("Z:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##scale5", &gamelist.at(selected)->GetGameObject()->transform.scale.z);
						ImGui::PopItemWidth();



					}
					ImGui::Separator();
					bool check_detail = gamelist.at(selected)->Detail();
					ImGui::Checkbox("Detail", &check_detail);
					gamelist.at(selected)->SetDetail(check_detail);

					ImGui::Separator();
					break;
				}

				case Component::Type::RENDERER_TILEMAP:
				{
					if (gamelist.at(selected)->Detail() == false)
					{
						ImGui::PushItemWidth(90);

						ImGui::Separator();
						ImGui::Text("POS");
						ImGui::Text("X:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##pos", &gamelist.at(selected)->GetGameObject()->transform.position.x, -100, 100);
						ImGui::SameLine();
						ImGui::Text("Y:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##pos1", &gamelist.at(selected)->GetGameObject()->transform.position.y, -100, 100);

						ImGui::Separator();
						ImGui::Text("ROTATION");
						ImGui::Text("Z:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##rot2", &gamelist.at(selected)->GetGameObject()->transform.rotation.z, -1.58, 1.58);

						ImGui::Separator();
						ImGui::Text("SCALE");
						ImGui::Text("X:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##scale", &gamelist.at(selected)->GetGameObject()->transform.scale.x, 1, 3);
						ImGui::PopItemWidth();



					}
					else
					{
						CheckTransform();

						ImGui::PushItemWidth(90);

						ImGui::Separator();
						ImGui::Text("POS");
						ImGui::Text("X:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##pos3", &gamelist.at(selected)->GetGameObject()->transform.position.x);
						ImGui::SameLine();
						ImGui::Text("Y:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##pos4", &gamelist.at(selected)->GetGameObject()->transform.position.y);


						ImGui::Separator();
						ImGui::Text("ROTATION");
						ImGui::Text("Z:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##rot5", &gamelist.at(selected)->GetGameObject()->transform.rotation.z, -1.58f, 1.58f);

						ImGui::Separator();
						ImGui::Text("SCALE");
						ImGui::Text("X:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##scale3", &gamelist.at(selected)->GetGameObject()->transform.scale.x);
						ImGui::PopItemWidth();


					}
					ImGui::Separator();
					bool check_detail = gamelist.at(selected)->Detail();
					ImGui::Checkbox("Detail", &check_detail);
					gamelist.at(selected)->SetDetail(check_detail);

					ImGui::Separator();
					break;

				}


				}

				});



		}


	}

	virtual void Render()
	{

		if (framework->GetCurrentGameObjectManager()->GetMode() == SceneMode::PLAY)
			testMode();
		Warning();
		if (active)
		{
			ImGui::SetNextWindowSize(ImVec2(700, 600));
			ImGui::Begin("Add Object", &active, ImGuiWindowFlags_MenuBar);

			ImGui::PushItemWidth(90);

			ImGui::InputText("name", a, IM_ARRAYSIZE(a));

			ImGui::Separator();
			ImGui::Text("POS");
			ImGui::Text("X:");
			ImGui::SameLine();
			ImGui::SliderFloat(u8"##pos", &pos.x, -100, 100);
			ImGui::SameLine();
			ImGui::Text("Y:");
			ImGui::SameLine();
			ImGui::SliderFloat(u8"##pos1", &pos.y, -100, 100);
			ImGui::SameLine();
			ImGui::Text("Z:");
			ImGui::SameLine();
			ImGui::SliderFloat(u8"##pos2", &pos.z, -100, 100);


			ImGui::Separator();
			ImGui::Text("ROTATION");
			ImGui::Text("X:");
			ImGui::SameLine();
			ImGui::SliderFloat(u8"##rot", &rot.x, -1.58, 1.58);
			ImGui::SameLine();
			ImGui::Text("Y:");
			ImGui::SameLine();
			ImGui::SliderFloat(u8"##rot1", &rot.y, -1.58, 1.58);
			ImGui::SameLine();
			ImGui::Text("Z:");
			ImGui::SameLine();
			ImGui::SliderFloat(u8"##rot2", &rot.z, -1.58, 1.58);
			scale.x = 1;
			scale.y = 1;
			scale.z = 1;
			ImGui::Separator();
			ImGui::Text("SCALE");
			ImGui::Text("X:");
			ImGui::SameLine();
			ImGui::SliderFloat(u8"##scale", &scale.x, 1, 3);
			ImGui::SameLine();
			ImGui::Text("Y:");
			ImGui::SameLine();
			ImGui::SliderFloat(u8"##scale1", &scale.y, 1, 3);
			ImGui::SameLine();
			ImGui::Text("Z:");
			ImGui::SameLine();
			ImGui::SliderFloat(u8"##scale2", &scale.z, 1, 3);
			ImGui::PopItemWidth();

			ImGui::Separator();
			if (ImGui::Button("create"))
			{

				GameObject* ob = _manager->CreateGameObject(a);
				if (ob != nullptr) {
					ob->transform.SetPosition(pos.x, pos.y, pos.z);
					ob->transform.SetRotation(rot.x, rot.y, rot.z);
					ob->transform.SetScale(scale.x, scale.y, scale.z);

					gamelist.push_back(new HierarchyObject(ob));
				}
				else
				{
					show_warning = true;
				}



				std::memset(a, 0, IM_ARRAYSIZE(a));
				active = false;

			}

			ImGui::End();
		}



		if (_isActive)
		{
			ImGui::Begin(u8"Hierarchy View", &_isActive, ImGuiWindowFlags_HorizontalScrollbar);
			if (ImGui::Button("add"))
			{
				ImGui::SetNextWindowSize(ImVec2(600, 600), ImGuiCond_FirstUseEver);
				active = true;


			}
			ImGui::SameLine();
			if (ImGui::Button("delete"))
			{
				if (gamelist.size() != 0)
				{
					//selected=0~12 size 13
					if (selected > 0)
					{
						if (selected == gamelist.size() - 1)
						{
							_manager->DestroyGameObject(gamelist.at(selected)->GetGameObject()->GetName());
							gamelist.pop_back();
							selected--;
							show_delete = true;
						}
						else
						{
							_manager->DestroyGameObject(gamelist.at(selected)->GetGameObject()->GetName());
							gamelist.erase(gamelist.begin() + selected);
							show_delete = true;
						}
					}
					else
					{
						_manager->DestroyGameObject(gamelist.at(selected)->GetGameObject()->GetName());
						gamelist.erase(gamelist.begin() + selected);
						show_delete = true;
					}
				}


			}



			//object 리스트 보여주는 곳
			// SetFocus()기능도 존재

			if (gamelist.size() != 0)
			{
				ImGui::BeginChild("Scrolling", ImVec2(150, 0), true);
				if (framework->GetCurrentGameObjectManager()->GetMode() != SceneMode::PLAY)
				{
					for (int i = 0; i < gamelist.size(); i++)
					{
						if (ImGui::Selectable((gamelist.at(i)->GetGameObject()->ObjectName.c_str()), selected == i))
						{
							selected = i;
							gamelist.at(i)->GetGameObject()->SetFocus();

						}


					}
				}
				if (framework->GetCurrentGameObjectManager()->GetMode() != SceneMode::PLAY)
				{
					for (int i = 0; i < gameTestList.size(); i++)
					{
						if (ImGui::Selectable((gameTestList.at(i)->GetGameObject()->ObjectName.c_str()), selected == i))
						{
							selected = i;
							gameTestList.at(i)->GetGameObject()->SetFocus();

						}


					}
				}
				ImGui::EndChild();
				ImGui::SetScrollX(100.0f);

				ImGui::SameLine();
				ImGui::BeginGroup();

				ImGui::Text(const_cast<char*>(gamelist.at(selected)->GetGameObject()->ObjectName.c_str()));
				ImGui::Separator();
				ImGui::Text("TRANSFORM");
				Transform();
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
				SpriteRenderer* render1 = new SpriteRenderer(temp, (ResourceManager*)this->ResM);
				gamelist.at(selected)->GetGameObject()->AddComponent(render1);
				component_active = false;

			}
			if (ImGui::Button("ModelRenderer"))
			{

				GameObject* temp1 = gamelist.at(selected)->GetGameObject();
				ModelRenderer* render2 = new ModelRenderer(temp1, (ResourceManager*)this->ResM);
				//render->Init();
				gamelist.at(selected)->GetGameObject()->AddComponent(render2);
				component_active = false;


			}
			if (ImGui::Button("TileMapRender"))
			{
				GameObject* temp2 = gamelist.at(selected)->GetGameObject();
				TileMapRenderer* render3 = new TileMapRenderer(temp2, (ResourceManager*)this->ResM);
				//render->Init();
				gamelist.at(selected)->GetGameObject()->AddComponent(render3);
				component_active = false;

			}
			ImGui::End();
		}





	}
};