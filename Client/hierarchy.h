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

	void SetScriptName(char name)
	{
		script_name[20] = name;
	}

	char* GetScriptName()
	{

		return script_name;
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

	void Copy(HierarchyObject* h)
	{
		this->map1 = h->map1;
		strcpy_s(this->script_name, 20 * sizeof(char), h->script_name);
		this->detail = h->detail;
	}

private:
	GameObject* obj;
	std::map<Component::Type, std::string> map1;
	char script_name[20]{};
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
	LuaManager* lua;


	std::vector<HierarchyObject*> gamelist;
	std::vector<HierarchyObject*> gameTestList;
	std::vector<HierarchyObject*>* current = &gamelist;

	GameObject* obj;          //GameObject 함수를 쓰려고 하는 obj = current->at(selected)->getGameObject(); 로 표현됨
	HierarchyObject* Hobj;    //HierarachyObject를 관리 하기 위해  씀 obj = current->at(selected); 로 표현
	bool check1 = true;
	bool check2 = true;
	bool check3 = true;

	int test_check = 1;

	int component_selected = 0;
	int selected = 0;
	//창 끄고 키는 bool 변수들
	bool my_tool_active = true;
	bool active = false;
	bool component_active = false;

	// 팝업창을 위한 bool 변수들
	bool show_error_script = false;
	bool show_script = false;
	bool show_warning = false;
	bool Exceed = false;
	bool show_delete = false;
	bool show_render = false;
	bool delete_sprite = false;
	bool can_delete = true;
	bool delete_script = false;
	bool show_not_exist = false;
	char name[20]{};

	int g = 0;

	DirectX::XMFLOAT3 pos{};
	DirectX::XMFLOAT3 rot{};
	DirectX::XMFLOAT3 scale{ 1,1,1 };
	/// <summary>
	/// map 함수를 이용하여 담기 위한 vector 
	/// </summary>
	std::vector<std::string> spriteList;
	std::vector<std::string> tileList;
	std::vector<std::string> modelList;
	std::vector<std::string> testList;
	std::vector<Component::Type> componentList;

	HierarchyObject* FindHierarchyObjectInGamelist(std::string name)
	{
		for (HierarchyObject* h : gamelist)
		{
			std::string objName = h->GetGameObject()->GetName();
			if (objName.compare(name) == 0)
				return h;
		}
		return nullptr;
	}


	Hierarchy(IGameObjectManager* manager, IResourceManager* res, const std::string& name, IFramework* framework) : ILayer(name)
	{
		this->framework = framework;
		this->_manager = manager;
		this->ResM = res;
		this->lua = framework->GetLuaManager();

		if (framework->GetCurrentGameObjectManager()->GetMode() != SceneMode::PLAY) {
			this->_manager->AddFocusedObjectListener([&](GameObject* selectedObject) {

				for (int k = 0; k < current->size(); k++)
				{
					if (selectedObject->GetName() == current->at(k)->GetGameObject()->GetName())
					{
						selected = k;
					}

				}

				});

		}









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

	void change()
	{
		if (framework->GetCurrentGameObjectManager()->GetMode() == SceneMode::PLAY)
		{
			//gamelist -> gametestlist 로 복사
			for (auto& pair : framework->GetCurrentGameObjectManager()->GetObejctMap())
			{
				GameObject* obj = pair.second;
				HierarchyObject* play_obj = new HierarchyObject(obj);
				HierarchyObject* dev_obj = FindHierarchyObjectInGamelist(obj->GetName());
				play_obj->Copy(dev_obj);

				gameTestList.push_back(play_obj);

			}

			current = &gameTestList;

		}
		else
		{
			std::cout << 'z';
			current = &gamelist;
			// 날려주기
			gameTestList.clear();

		}

	}
	/// <summary>
	/// Object의 컴포넌트 기능 코드를 모아둔 함수
	/// </summary>
	void component()
	{
		//obj 은 gameObject쪽 함수, Hobj는 HierarcyObject쪽 함수
		obj = current->at(selected)->GetGameObject();
		Hobj = current->at(selected);

		//render로 돌아기 떄문에 다시 돌아 올때 비워줌
		spriteList.clear();
		modelList.clear();
		tileList.clear();

		// sprite , model , tile 를 map 에 저장하여 꺼내 쓸 코드
		auto spriteMap = ResM->GetSpriteMap();
		auto modelMap = ResM->GetModelMap();
		auto tileMap = ResM->GetTileMap();


		for (auto& spritePair : spriteMap)
		{
			Sprite* sp = spritePair.second;
			std::string spriteName = sp->GetName();
			spriteList.push_back(spriteName);

		}
		for (auto& modelPair : modelMap)
		{
			Model* md = modelPair.second;
			std::string modelName = md->GetName();
			modelList.push_back(modelName);

		}
		for (auto& tilePair : tileMap)
		{
			TileMap* tp = tilePair.second;
			std::string tileName = tp->GetName();
			tileList.push_back(tileName);

		}
		// 컴포넌트가 추가 되면 forEach 문으로 탐색하면서 switch 문으로 각 컴포넌트 타입 마다 생기는 칸 생성 하는 코드
		if (obj->GetComponentSize() != 0)
		{


			obj->ComponentForeach([&](Component* c) {
				int count = 0;
				std::string name = c->GetName();
				Component::Type type = c->GetType();

				switch (type)
				{
				case Component::Type::RENDERER_SPRITE:
				{
					SpriteRenderer* render4 = dynamic_cast<SpriteRenderer*>(c);
					ImGui::Text(name.c_str());
					ImGui::SameLine();

					std::string f = Hobj->FindMappingValue(Component::Type::RENDERER_SPRITE);
					ImGui::PushItemWidth(100);
					if (ImGui::BeginCombo("##combo", f.size() == 0 ? "" : f.c_str())) // The second parameter is the label previewed before opening the combo.
					{
						for (int n = 0; n < spriteList.size(); n++)
						{
							bool is_selected = (Hobj->FindMappingValue(Component::Type::RENDERER_SPRITE) == spriteList.at(n).c_str()); // You can store your selection however you want, outside or inside your objects
							if (ImGui::Selectable(spriteList[n].c_str(), is_selected))
							{
								Hobj->DeleteMappingValue(Component::Type::RENDERER_SPRITE);
								Hobj->SetMappingValue(Component::Type::RENDERER_SPRITE, spriteList.at(n).c_str()); ;
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
						obj->ComponentForeach([&](Component* c) {
							Component::Type type = c->GetType();
							switch (type) {
							case Component::Type::SCRIPT:
							{
								if (framework->GetCurrentGameObjectManager()->GetMode() == SceneMode::PLAY)
								{
									delete_sprite = true;
								}
								else {
									if (obj->GetComponentSize() == 1)
										can_delete = false;
									if (obj->GetComponentSize() == 2)
										can_delete = true;
								}
								break;
							}
							case Component::Type::RENDERER_SPRITE:
							{
								if (framework->GetCurrentGameObjectManager()->GetMode() != SceneMode::PLAY)
									can_delete = true;
								else if (obj->GetComponentSize() == 1 && framework->GetCurrentGameObjectManager()->GetMode() == SceneMode::PLAY)
									can_delete = true;
								else
									can_delete = false;
								break;
							}
							}
							});
						if (can_delete)
						{
							Hobj->AddDeleteComponent(Component::Type::RENDERER_SPRITE);
							Hobj->DeleteMappingValue(Component::Type::RENDERER_SPRITE);
							Logger::AddLog("Delete Component : Sprite");

						}

					}
					ImGui::PushItemWidth(100);
					float spriteLayer = render4->GetLayerDepth();
					ImGui::Text("Layer");
					ImGui::SameLine();
					ImGui::InputFloat("##layer", &spriteLayer, 0.2);
					if (spriteLayer < 0)
						spriteLayer = 0;
					if (spriteLayer > 1)
						spriteLayer = 1;
					render4->SetLayerDepth(spriteLayer);
					ImGui::Separator();

					break;
				}

				case Component::Type::RENDERER_MODEL:
				{
					ModelRenderer* render5 = dynamic_cast<ModelRenderer*>(c);
					ImGui::Text(name.c_str());
					ImGui::SameLine();

					std::string modelName = Hobj->FindMappingValue(Component::Type::RENDERER_MODEL);
					ImGui::PushItemWidth(100);
					if (ImGui::BeginCombo("##model", modelName.size() == 0 ? "" : modelName.c_str())) // The second parameter is the label previewed before opening the combo.
					{
						for (int n = 0; n < modelList.size(); n++)
						{
							bool is_selected = (Hobj->FindMappingValue(Component::Type::RENDERER_MODEL) == modelList.at(n).c_str()); // You can store your selection however you want, outside or inside your objects
							if (ImGui::Selectable(modelList[n].c_str(), is_selected))
							{
								Hobj->DeleteMappingValue(Component::Type::RENDERER_MODEL);
								Hobj->SetMappingValue(Component::Type::RENDERER_MODEL, modelList.at(n).c_str()); ;
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
						Hobj->AddDeleteComponent(Component::Type::RENDERER_MODEL);
						Hobj->DeleteMappingValue(Component::Type::RENDERER_MODEL);
						Logger::AddLog("Delete Component : Model");

					}



					ImGui::Separator();

					break;
				}

				case Component::Type::RENDERER_TILEMAP:
				{
					TileMapRenderer* render6 = dynamic_cast<TileMapRenderer*>(c);

					ImGui::Text(name.c_str());
					ImGui::SameLine();
					std::string tileName = Hobj->FindMappingValue(Component::Type::RENDERER_TILEMAP);
					ImGui::PushItemWidth(100);
					if (ImGui::BeginCombo("##tile", tileName.size() == 0 ? "" : tileName.c_str())) // The second parameter is the label previewed before opening the combo.
					{
						for (int n = 0; n < tileList.size(); n++)
						{
							bool is_selected = (current->at(selected)->FindMappingValue(Component::Type::RENDERER_TILEMAP) == tileList.at(n).c_str()); // You can store your selection however you want, outside or inside your objects
							if (ImGui::Selectable(tileList[n].c_str(), is_selected))
							{
								Hobj->DeleteMappingValue(Component::Type::RENDERER_TILEMAP);
								Hobj->SetMappingValue(Component::Type::RENDERER_TILEMAP, tileList.at(n).c_str()); ;
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
						Hobj->AddDeleteComponent(Component::Type::RENDERER_TILEMAP);
						Hobj->DeleteMappingValue(Component::Type::RENDERER_TILEMAP);
						Logger::AddLog("Delete Component : TileMap");

					}
					ImGui::PushItemWidth(100);
					float TileLayer = render6->GetLayerDepth();
					ImGui::Text("Layer");
					ImGui::SameLine();
					ImGui::InputFloat("##layer", &TileLayer, 0.2);
					if (TileLayer < 0)
						TileLayer = 0;
					if (TileLayer > 1)
						TileLayer = 1;
					render6->SetLayerDepth(TileLayer);
					ImGui::Separator();

					break;
				}
				case Component::Type::SCRIPT:
				{
					Script* script = dynamic_cast<Script*>(c);


					ImGui::Text(name.c_str());
					ImGui::PushItemWidth(130);



					char* script_name = Hobj->GetScriptName();

					ImGui::InputText("name", script_name, IM_ARRAYSIZE(script_name));
					ImGui::SameLine();

					if (ImGui::Button("input")&&script_name[0]!='\0')
					{
						
						std::string s(script_name);
						std::string path = "..\\Lua\\";
						std::string path2 = ".lua";
						path = path + script_name + path2;
						std::cout << path;
						if (_access(path.c_str(), 0) == 0)
						{
							script->SetLuaFilePath(path);
							Logger::AddLog("Set Lua FilePath > " + path);
						}
						else
							show_not_exist = true;
					}
					ImGui::SameLine();
					if (ImGui::Button("Script Del"))
					{
						if (framework->GetCurrentGameObjectManager()->GetMode() == SceneMode::PLAY)
						{
							delete_script = true;

						}
						else {
							Hobj->AddDeleteComponent(Component::Type::SCRIPT);
							Hobj->DeleteMappingValue(Component::Type::SCRIPT);
							Logger::AddLog("Delete Component : Script");

						}
					}


					ImGui::Separator();

					break;
				}
				}




				});



			Hobj->DeleteComponent();



		}
		ImGui::Text("                        ");
		ImGui::SameLine();
		if (ImGui::Button("Add Component"))
		{
			component_active = true;

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
		if (delete_sprite) {
			ImGui::OpenPopup("Script Delete");
		}

		if (ImGui::BeginPopupModal("Script Delete", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text(u8"실행모드일때 스크립트가 포함된 오브젝트는 컴포넌트를 삭제할수 없습니다.");
			if (ImGui::Button("OK") || ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter))) {
				ImGui::CloseCurrentPopup();
				delete_sprite = false;
			}
			ImGui::EndPopup();
		}

		if (delete_script) {
			ImGui::OpenPopup("Play Mode");
		}

		if (ImGui::BeginPopupModal("Play Mode", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text(u8"실행 모드일때 스크립트 삭제가 안됩니다.");
			if (ImGui::Button("OK") || ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter))) {
				ImGui::CloseCurrentPopup();
				delete_script = false;
			}
			ImGui::EndPopup();
		}

		if (show_render) {
			ImGui::OpenPopup("Caution");
		}

		if (ImGui::BeginPopupModal("Caution", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text(u8"렌더러는 1개 가능합니다");
			if (ImGui::Button("OK") || ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter))) {
				ImGui::CloseCurrentPopup();
				show_render = false;
			}
			ImGui::EndPopup();
		}

		if (show_script) {
			ImGui::OpenPopup("caution");
		}

		if (ImGui::BeginPopupModal("caution", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text(u8"스크립트는 1개 가능합니다");
			if (ImGui::Button("OK") || ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter))) {
				ImGui::CloseCurrentPopup();
				show_script = false;
			}
			ImGui::EndPopup();
		}

		if (Exceed) {
			ImGui::OpenPopup("Exceed");
		}

		if (ImGui::BeginPopupModal("Exceed", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text(u8"더이상 컴포넌트 추가가 불가능합니다");
			if (ImGui::Button("OK") || ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter))) {
				ImGui::CloseCurrentPopup();
				Exceed = false;
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
		if (show_not_exist) {
			ImGui::OpenPopup(u8"존재하지 않는 파일");
		}

		if (ImGui::BeginPopupModal(u8"존재하지 않는 파일", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text(u8"파일이 존재하지 않습니다.");
			if (ImGui::Button(u8"확인") || ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)))
			{

				ImGui::CloseCurrentPopup();
				show_not_exist = false;

			}
			ImGui::EndPopup();
		}

	}
	/// <summary>
	/// inputfloat 할떄 최대값 최소값 넘어가면 최대값 최소값으로 설정하게 만들려고 만든 함수 입니다.
	/// </summary>
	void CheckSpriteTransform()
	{
		obj = current->at(selected)->GetGameObject();
		Hobj = current->at(selected);

		float posX = obj->transform.position.x;
		float posY = obj->transform.position.y;
		float rotZ = obj->transform.rotation.z;


		float minPosX = -1000.0f;
		float maxPosX = 1600.0f;

		float minPosY = -1000.0f;
		float maxPosY = 1600.0f;


		float minRotValue = -3.16f;
		float maxRotValue = 3.16f;


		float minScaleValue = 0.01f;
		float maxScaleValue = 10.0f;

		if (posX < minPosX) {
			posX = minPosX;
			obj->transform.position.x = posX;
		}
		else if (posX > maxPosX) {
			posX = maxPosX;
			obj->transform.position.x = posX;
		}

		if (posY < minPosY) {
			posY = minPosY;
			obj->transform.position.y = posY;
		}
		else if (posY > maxPosY) {
			posY = maxPosY;
			obj->transform.position.y = posY;
		}

		if (rotZ < minRotValue) {
			rotZ = minRotValue;
			obj->transform.rotation.z = rotZ;
		}
		else if (rotZ > maxRotValue) {
			rotZ = maxRotValue;
			obj->transform.rotation.z = rotZ;
		}

		float scaleX = current->at(selected)->GetGameObject()->transform.scale.x;

		if (scaleX < minScaleValue) {
			scaleX = minScaleValue;
			obj->transform.scale.x = scaleX;
		}
		else if (scaleX > maxScaleValue) {
			scaleX = maxScaleValue;
			obj->transform.scale.x = scaleX;
		}


	}
	/// <summary>
	/// inputfloat 할떄 최대값 최소값 넘어가면 최대값 최소값으로 설정하게 만들려고 만든 함수 입니다.
	/// </summary>
	void CheckTransform()
	{
		obj = current->at(selected)->GetGameObject();
		Hobj = current->at(selected);

		float posX = obj->transform.position.x;
		float posY = obj->transform.position.y;
		float posZ = obj->transform.position.z;

		float rotX = obj->transform.rotation.x;
		float rotY = obj->transform.rotation.y;
		float rotZ = obj->transform.rotation.z;

		float scaleX = obj->transform.scale.x;
		float scaleY = obj->transform.scale.y;
		float scaleZ = obj->transform.scale.z;


		float minPosValue = -1000.0f;
		float maxPosValue = 1600.0f;

		float minRotValue = -3.16f;
		float maxRotValue = 3.16f;

		float minScaleValue = 0.01f;
		float maxScaleValue = 10.0f;

		/// <summary>
		/// Pos x , y ,z 에 대한 값이 -100 보다 작거나 100보다 크면 -100 ,100 값으로 치환 시켜주는 곳
		/// </summary>
		if (posX < minPosValue) {
			posX = minPosValue;
			obj->transform.position.x = posX;
		}
		else if (posX > maxPosValue) {
			posX = maxPosValue;
			obj->transform.position.x = posX;
		}

		if (posY < minPosValue) {
			posY = minPosValue;
			obj->transform.position.y = posY;
		}
		else if (posY > maxPosValue) {
			posY = maxPosValue;
			obj->transform.position.y = posY;
		}

		if (posZ < minPosValue) {
			posZ = minPosValue;
			obj->transform.position.z = posZ;
		}
		else if (posZ > maxPosValue) {
			posZ = maxPosValue;
			obj->transform.position.z = posZ;
		}
		/// <summary>
		/// Rot x , y ,z 에 대한 값이 -1.58 보다 작거나 1.58보다 크면 -1.58 ,1.58 값으로 치환 시켜주는 곳
		/// </summary>
		if (rotX < minRotValue) {
			rotX = minRotValue;
			obj->transform.rotation.x = rotX;
		}
		else if (rotX > maxRotValue) {
			rotX = maxRotValue;
			obj->transform.rotation.x = rotX;
		}

		if (rotY < minRotValue) {
			rotY = minRotValue;
			obj->transform.rotation.y = rotY;
		}
		else if (rotY > maxRotValue) {
			rotY = maxRotValue;
			obj->transform.rotation.y = rotY;
		}

		if (rotZ < minRotValue) {
			rotZ = minRotValue;
			obj->transform.rotation.z = rotZ;
		}
		else if (rotZ > maxRotValue) {
			rotZ = maxRotValue;
			obj->transform.rotation.z = rotZ;
		}
		/// <summary>
		/// Scale x , y ,z 에 대한 값이 1 보다 작거나 3보다 크면 1 ,3 값으로 치환 시켜주는 곳
		/// </summary>
		if (scaleX < minScaleValue) {
			scaleX = minScaleValue;
			obj->transform.scale.x = scaleX;
		}
		else if (scaleX > maxScaleValue) {
			scaleX = maxScaleValue;
			obj->transform.scale.x = scaleX;
		}

		if (scaleY < minScaleValue) {
			scaleY = minScaleValue;
			obj->transform.scale.y = scaleY;
		}
		else if (scaleY > maxScaleValue) {
			scaleY = maxScaleValue;
			obj->transform.scale.y = scaleY;
		}

		if (scaleZ < minScaleValue) {
			scaleZ = minScaleValue;
			obj->transform.scale.z = scaleZ;
		}
		else if (scaleZ > maxScaleValue) {
			scaleZ = maxScaleValue;
			obj->transform.scale.z = scaleZ;
		}

	}




	virtual void Render()
	{

		/// <summary>
		/// 정리 해둔 팝업 창 뛰우는 코드 모아둔 함수 추가
		/// </summary>
		Warning();

		/// 오브젝트 추가하는 창 
		if (active)
		{
			ImGui::SetNextWindowSize(ImVec2(700, 600));
			ImGui::Begin("Add Object", &active);

			ImGui::PushItemWidth(130);

			ImGui::InputText("Object Name", name, IM_ARRAYSIZE(name));

			ImGui::Separator();
			ImGui::Text("POS");
			ImGui::Text("X:");
			ImGui::SameLine();
			ImGui::SliderFloat(u8"##pos", &pos.x, -1000, 1600);
			ImGui::SameLine();
			ImGui::Text("Y:");
			ImGui::SameLine();
			ImGui::SliderFloat(u8"##pos1", &pos.y, -1000, 1600);
			ImGui::SameLine();
			ImGui::Text("Z:");
			ImGui::SameLine();
			ImGui::SliderFloat(u8"##pos2", &pos.z, -1000, 1600);


			ImGui::Separator();
			ImGui::Text("ROTATION");
			ImGui::Text("X:");
			ImGui::SameLine();
			ImGui::SliderFloat(u8"##rot", &rot.x, -3.16, 3.16);
			ImGui::SameLine();
			ImGui::Text("Y:");
			ImGui::SameLine();
			ImGui::SliderFloat(u8"##rot1", &rot.y, -3.16, 3.16);
			ImGui::SameLine();
			ImGui::Text("Z:");
			ImGui::SameLine();
			ImGui::SliderFloat(u8"##rot2", &rot.z, -3.16, 3.16);
			scale.x = 1;
			scale.y = 1;
			scale.z = 1;
			ImGui::Separator();
			ImGui::Text("SCALE");
			ImGui::Text("X:");
			ImGui::SameLine();
			ImGui::SliderFloat(u8"##scale", &scale.x, 0.01, 10);
			ImGui::SameLine();
			ImGui::Text("Y:");
			ImGui::SameLine();
			ImGui::SliderFloat(u8"##scale1", &scale.y, 0.01, 10);
			ImGui::SameLine();
			ImGui::Text("Z:");
			ImGui::SameLine();
			ImGui::SliderFloat(u8"##scale2", &scale.z, 0.01, 10);
			ImGui::PopItemWidth();

			ImGui::Separator();
			if (ImGui::Button("create"))
			{

				GameObject* ob = framework->GetCurrentGameObjectManager()->CreateGameObject(name);
				if (ob != nullptr) {
					ob->transform.SetPosition(pos.x, pos.y, pos.z);
					ob->transform.SetRotation(rot.x, rot.y, rot.z);
					ob->transform.SetScale(scale.x, scale.y, scale.z);

					current->push_back(new HierarchyObject(ob));
				}
				else
				{
					show_warning = true;
				}

				std::string Log = name;
				Logger::AddLog("Add Object : " + Log);

				std::memset(name, 0, IM_ARRAYSIZE(name));
				active = false;

			}

			ImGui::End();
		}


		// 계층뷰 창 
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
				if (current->size() != 0)
				{
					//selected=0~12 size 13
					if (selected > 0)
					{
						if (selected == current->size() - 1)
						{
							Logger::AddLog("Delete Object : " + current->at(selected)->GetGameObject()->GetName());

							_manager->DestroyGameObject(current->at(selected)->GetGameObject()->GetName());
							current->pop_back();
							selected--;
							show_delete = true;
						}
						else
						{
							Logger::AddLog("Delete Object : " + current->at(selected)->GetGameObject()->GetName());

							_manager->DestroyGameObject(current->at(selected)->GetGameObject()->GetName());
							current->erase(current->begin() + selected);
							show_delete = true;
						}
					}
					else
					{
						Logger::AddLog("Delete Object : " + current->at(selected)->GetGameObject()->GetName());
						_manager->DestroyGameObject(current->at(selected)->GetGameObject()->GetName());
						current->erase(current->begin() + selected);
						show_delete = true;
					}

				}



			}



			//object 리스트 보여주는 곳
			// SetFocus()기능도 존재

			if (current->size() != 0)
			{
				ImGui::BeginChild("Scrolling", ImVec2(150, 0), true);

				for (int i = 0; i < current->size(); i++)
				{
					if (ImGui::Selectable((current->at(i)->GetGameObject()->ObjectName.c_str()), selected == i))
					{
						selected = i;
						current->at(i)->GetGameObject()->SetFocus();

					}


				}




				ImGui::EndChild();
				ImGui::SetScrollX(100.0f);

				ImGui::SameLine();
				ImGui::BeginGroup();

				ImGui::Text(const_cast<char*>(current->at(selected)->GetGameObject()->ObjectName.c_str()));
				ImGui::Separator();

				ImGui::Text("TRANSFORM");
				// 좌표 기능 코드 들어있는 함수
				Transform();
				ImGui::Text("Other Component");
				if (current->at(selected)->GetGameObject()->GetComponentSize() != 0)
					ImGui::Separator();
				// 컴포넌트 추가 버튼 등 코드 들어있는 함수
				component();


				ImGui::Separator();



				ImGui::SameLine();
				ImGui::EndGroup();
				ImGui::SameLine();

			}
			ImGui::End();
		}

		// 컴포넌트 함수 안에 있는 component_active = true 되면 열리는 컴포넌트 추가하는 창 
		if (component_active)
		{
			ImGui::Begin("Add Component", &component_active);

			GameObject* obj = current->at(selected)->GetGameObject();
			componentList.clear();
			auto map = obj->GetComponentMap();
			// coponentList에 obj 컴포넌트 정보 맵으로 받기
			for (auto& pair : map)
			{
				Component::Type type = pair.first;
				componentList.push_back(type);

			}
			// sprite 버튼 눌렀을때 for문으로 List 탐색해서 존재 여부 탐색
			if (ImGui::Button("SpriteRenderer"))
			{

				if (obj->GetComponentSize() == 1)
				{
					for (int k = 0; k < componentList.size(); k++)
					{
						switch (componentList.at(k))
						{
						case Component::Type::RENDERER_SPRITE:
						{
							component_active = false;
							show_render = true;
							break;

						}
						case Component::Type::RENDERER_MODEL:
						{
							component_active = false;
							show_render = true;
							break;

						}
						case Component::Type::RENDERER_TILEMAP:
						{
							component_active = false;
							show_render = true;
							break;

						}
						case Component::Type::SCRIPT:
						{
							GameObject* temp = current->at(selected)->GetGameObject();
							SpriteRenderer* render1 = new SpriteRenderer(temp, (ResourceManager*)this->ResM);
							current->at(selected)->GetGameObject()->AddComponent(render1);
							component_active = false;
							Logger::AddLog("Add Sprite");

							break;

						}
						}

					}
				}
				else if (obj->GetComponentSize() == 0)
				{
					GameObject* temp = current->at(selected)->GetGameObject();
					SpriteRenderer* render1 = new SpriteRenderer(temp, (ResourceManager*)this->ResM);
					current->at(selected)->GetGameObject()->AddComponent(render1);
					component_active = false;
					Logger::AddLog("Add Sprite");

				}
				else
				{
					Exceed = true;
				}

			}
			// model 버튼 눌렀을때 for문으로 List 탐색해서 존재 여부 탐색

			if (ImGui::Button("ModelRenderer"))
			{

				if (obj->GetComponentSize() == 1)
				{
					for (int k = 0; k < componentList.size(); k++)
					{
						switch (componentList.at(k))
						{
						case Component::Type::RENDERER_SPRITE:
						{
							component_active = false;
							show_render = true;
							break;

						}
						case Component::Type::RENDERER_MODEL:
						{
							component_active = false;
							show_render = true;
							break;

						}
						case Component::Type::RENDERER_TILEMAP:
						{
							component_active = false;
							show_render = true;
							break;

						}
						case Component::Type::SCRIPT:
						{
							GameObject* temp1 = current->at(selected)->GetGameObject();
							ModelRenderer* render2 = new ModelRenderer(temp1, (ResourceManager*)this->ResM);
							//render->Init();
							current->at(selected)->GetGameObject()->AddComponent(render2);
							component_active = false;
							Logger::AddLog("Add Model");

							break;

						}
						}

					}
				}
				else if (obj->GetComponentSize() == 0)
				{
					GameObject* temp1 = current->at(selected)->GetGameObject();
					ModelRenderer* render2 = new ModelRenderer(temp1, (ResourceManager*)this->ResM);
					//render->Init();
					current->at(selected)->GetGameObject()->AddComponent(render2);
					component_active = false;
					Logger::AddLog("Add Model");


				}
				else
				{
					Exceed = true;
				}



			}
			// TileMapRender 버튼 눌렀을때 for문으로 List 탐색해서 존재 여부 탐색

			if (ImGui::Button("TileMapRender"))
			{
				if (obj->GetComponentSize() == 1)
				{
					for (int k = 0; k < componentList.size(); k++)
					{
						switch (componentList.at(k))
						{
						case Component::Type::RENDERER_SPRITE:
						{
							component_active = false;
							show_render = true;
							break;

						}
						case Component::Type::RENDERER_MODEL:
						{
							component_active = false;
							show_render = true;
							break;

						}
						case Component::Type::RENDERER_TILEMAP:
						{
							component_active = false;
							show_render = true;
							break;

						}
						case Component::Type::SCRIPT:
						{
							GameObject* temp2 = current->at(selected)->GetGameObject();
							TileMapRenderer* render3 = new TileMapRenderer(temp2, (ResourceManager*)this->ResM);
							//render->Init();
							current->at(selected)->GetGameObject()->AddComponent(render3);
							component_active = false;
							Logger::AddLog("Add TileMap");

							break;

						}
						}

					}
				}
				else if (obj->GetComponentSize() == 0)
				{
					GameObject* temp2 = current->at(selected)->GetGameObject();
					TileMapRenderer* render3 = new TileMapRenderer(temp2, (ResourceManager*)this->ResM);
					//render->Init();
					current->at(selected)->GetGameObject()->AddComponent(render3);
					component_active = false;

					Logger::AddLog("Add TileMap");


				}
				else
				{
					Exceed = true;
				}
			}
			// Script 버튼 눌렀을때 for문으로 List 탐색해서 존재 여부 탐색

			if (ImGui::Button("Script"))
			{

				if (obj->GetComponentSize() == 1)
				{
					for (int k = 0; k < componentList.size(); k++)
					{
						switch (componentList.at(k))
						{
						case Component::Type::SCRIPT:
						{
							component_active = false;
							show_script = true;
							break;

						}
						default:
						{
							GameObject* scriptObj = current->at(selected)->GetGameObject();
							Script* script = new Script(scriptObj, (Framework*)framework);
							scriptObj->AddComponent(script);
							component_active = false;
							Logger::AddLog("Add Script");

							break;
						}
						}

					}
				}
				else if (obj->GetComponentSize() == 0)
				{
					GameObject* scriptObj = current->at(selected)->GetGameObject();
					Script* script = new Script(scriptObj, (Framework*)framework);
					scriptObj->AddComponent(script);
					component_active = false;
					Logger::AddLog("Add Script");



				}
				else
				{
					Exceed = true;
				}
			}
			ImGui::End();
		}





	}
	/// <summary>
	/// Object의 pos,rot,scale 크기 조절하는 곳 
	/// </summary>
	void Transform()
	{
		obj = current->at(selected)->GetGameObject();
		Hobj = current->at(selected);
		if (obj->GetComponentSize() == 0)
		{
			ImGui::PushItemWidth(130);

			if (Hobj->Detail() == false)
			{

				ImGui::Separator();
				ImGui::Text("POS");
				ImGui::Text("X:");
				ImGui::SameLine();
				ImGui::SliderFloat(u8"##pos", &obj->transform.position.x, -1000, 1600);
				ImGui::SameLine();
				ImGui::Text("Y:");
				ImGui::SameLine();
				ImGui::SliderFloat(u8"##pos1", &obj->transform.position.y, -1000, 1600);
				ImGui::SameLine();
				ImGui::Text("Z:");
				ImGui::SameLine();
				ImGui::SliderFloat(u8"##pos2", &obj->transform.position.z, -1000, 1600);


				ImGui::Separator();
				ImGui::Text("ROTATION");
				ImGui::Text("X:");
				ImGui::SameLine();
				ImGui::SliderFloat(u8"##rot", &obj->transform.rotation.x, -3.16, 3.16);
				ImGui::SameLine();
				ImGui::Text("Y:");
				ImGui::SameLine();
				ImGui::SliderFloat(u8"##rot1", &obj->transform.rotation.y, -3.16, 3.16);
				ImGui::SameLine();
				ImGui::Text("Z:");
				ImGui::SameLine();
				ImGui::SliderFloat(u8"##rot2", &obj->transform.rotation.z, -3.16, 3.16);

				ImGui::Separator();
				ImGui::Text("SCALE");
				ImGui::Text("X:");
				ImGui::SameLine();
				ImGui::SliderFloat(u8"##scale", &obj->transform.scale.x, 0.01, 10);
				ImGui::SameLine();
				ImGui::Text("Y:");
				ImGui::SameLine();
				ImGui::SliderFloat(u8"##scale1", &obj->transform.scale.y, 0.01, 10);
				ImGui::SameLine();
				ImGui::Text("Z:");
				ImGui::SameLine();
				ImGui::SliderFloat(u8"##scale2", &obj->transform.scale.z, 0.01, 10);
				ImGui::PopItemWidth();


			}
			else
			{
				CheckTransform();

				ImGui::Separator();
				ImGui::Text("POS");
				ImGui::Text("X:");
				ImGui::SameLine();
				ImGui::InputFloat(u8"##pos3", &obj->transform.position.x);
				ImGui::SameLine();
				ImGui::Text("Y:");
				ImGui::SameLine();
				ImGui::InputFloat(u8"##pos4", &obj->transform.position.y);
				ImGui::SameLine();
				ImGui::Text("Z:");
				ImGui::SameLine();
				ImGui::InputFloat(u8"##pos5", &obj->transform.position.z);

				ImGui::Separator();
				ImGui::Text("ROTATION");
				ImGui::Text("X:");
				ImGui::SameLine();
				ImGui::InputFloat(u8"##rot3", &obj->transform.rotation.x);
				ImGui::SameLine();
				ImGui::Text("Y:");
				ImGui::SameLine();
				ImGui::InputFloat(u8"##rot4", &obj->transform.rotation.y);
				ImGui::SameLine();
				ImGui::Text("Z:");
				ImGui::SameLine();
				ImGui::InputFloat(u8"##rot5", &obj->transform.rotation.z);

				ImGui::Separator();
				ImGui::Text("SCALE");
				ImGui::Text("X:");
				ImGui::SameLine();
				ImGui::InputFloat(u8"##scale3", &obj->transform.scale.x, 0.1, 10);
				ImGui::SameLine();
				ImGui::Text("Y:");
				ImGui::SameLine();
				ImGui::InputFloat(u8"##scale4", &obj->transform.scale.y, 0.1, 10);
				ImGui::SameLine();
				ImGui::Text("Z:");
				ImGui::SameLine();
				ImGui::InputFloat(u8"##scale5", &obj->transform.scale.z, 0.1, 10);
				ImGui::PopItemWidth();



			}
			ImGui::Separator();
			bool check_detail = Hobj->Detail();
			ImGui::Checkbox("Detail", &check_detail);
			Hobj->SetDetail(check_detail);

			ImGui::Separator();

		}
		else if (obj->GetComponentSize() == 1)
		{
			obj->ComponentForeach([&](Component* c) {
				int count = 0;
				std::string name = c->GetName();
				Component::Type type = c->GetType();

				switch (type)
				{
				case Component::Type::RENDERER_SPRITE:
				{
					obj->transform.position.z = 0;
					obj->transform.rotation.x = 0;
					obj->transform.rotation.y = 0;
					obj->transform.scale.y = 0;
					obj->transform.scale.z = 0;

					if (current->at(selected)->Detail() == false)
					{
						ImGui::PushItemWidth(130);

						ImGui::Separator();
						ImGui::Text("POS");
						ImGui::Text("X:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##pos", &obj->transform.position.x, -1000, 1600);
						ImGui::SameLine();
						ImGui::Text("Y:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##pos1", &obj->transform.position.y, -1000, 1600);

						ImGui::Separator();
						ImGui::Text("ROTATION");
						ImGui::Text("Z:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##rot2", &obj->transform.rotation.z, -3.16, 3.16);

						ImGui::Separator();
						ImGui::Text("SCALE");
						ImGui::Text(u8"크기:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##scale", &obj->transform.scale.x, 0.01, 10);
						ImGui::PopItemWidth();



					}
					else
					{
						CheckSpriteTransform();

						ImGui::PushItemWidth(130);

						ImGui::Separator();
						ImGui::Text("POS");
						ImGui::Text("X:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##pos3", &obj->transform.position.x);
						ImGui::SameLine();
						ImGui::Text("Y:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##pos4", &obj->transform.position.y);


						ImGui::Separator();
						ImGui::Text("ROTATION");
						ImGui::Text("Z:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##rot5", &obj->transform.rotation.z);

						ImGui::Separator();
						ImGui::Text("SCALE");
						ImGui::Text(u8"크기:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##scale3", &obj->transform.scale.x, 0.1, 10);
						ImGui::PopItemWidth();


					}
					ImGui::Separator();
					bool check_detail = Hobj->Detail();
					ImGui::Checkbox("Detail", &check_detail);
					Hobj->SetDetail(check_detail);

					ImGui::Separator();
					break;
				}

				case Component::Type::RENDERER_MODEL:
				{
					if (Hobj->Detail() == false)
					{
						ImGui::PushItemWidth(130);

						ImGui::Separator();
						ImGui::Text("POS");
						ImGui::Text("X:");

						ImGui::SameLine();
						ImGui::SliderFloat(u8"##pos", &obj->transform.position.x, -1000, 1600);
						ImGui::SameLine();
						ImGui::Text("Y:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##pos1", &obj->transform.position.y, -1000, 1600);
						ImGui::SameLine();
						ImGui::Text("Z:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##pos2", &obj->transform.position.z, -1000, 1600);


						ImGui::Separator();
						ImGui::Text("ROTATION");
						ImGui::Text("X:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##rot", &obj->transform.rotation.x, -3.16, 3.16);
						ImGui::SameLine();
						ImGui::Text("Y:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##rot1", &obj->transform.rotation.y, -3.16, 3.16);
						ImGui::SameLine();
						ImGui::Text("Z:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##rot2", &obj->transform.rotation.z, -3.16, 3.16);

						ImGui::Separator();
						ImGui::Text("SCALE");
						ImGui::Text("X:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##scale", &obj->transform.scale.x, 0.01, 10);
						ImGui::SameLine();
						ImGui::Text("Y:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##scale1", &obj->transform.scale.y, 0.01, 10);
						ImGui::SameLine();
						ImGui::Text("Z:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##scale2", &obj->transform.scale.z, 0.01, 10);
						ImGui::PopItemWidth();


					}
					else
					{
						CheckTransform();

						ImGui::PushItemWidth(130);

						ImGui::Separator();
						ImGui::Text("POS");
						ImGui::Text("X:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##pos3", &obj->transform.position.x);
						ImGui::SameLine();
						ImGui::Text("Y:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##pos4", &obj->transform.position.y);
						ImGui::SameLine();
						ImGui::Text("Z:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##pos5", &obj->transform.position.z);


						ImGui::Separator();
						ImGui::Text("ROTATION");
						ImGui::Text("X:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##rot3", &obj->transform.rotation.x);
						ImGui::SameLine();
						ImGui::Text("Y:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##rot4", &obj->transform.rotation.y);
						ImGui::SameLine();
						ImGui::Text("Z:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##rot5", &obj->transform.rotation.z);

						ImGui::Separator();
						ImGui::Text("SCALE");
						ImGui::Text("X:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##scale3", &obj->transform.scale.x, 0.1, 10);
						ImGui::SameLine();
						ImGui::Text("Y:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##scale4", &obj->transform.scale.y, 0.1, 10);
						ImGui::SameLine();
						ImGui::Text("Z:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##scale5", &obj->transform.scale.z, 0.1, 10);
						ImGui::PopItemWidth();



					}
					ImGui::Separator();
					bool check_detail = Hobj->Detail();
					ImGui::Checkbox("Detail", &check_detail);
					Hobj->SetDetail(check_detail);

					ImGui::Separator();
					break;
				}

				case Component::Type::RENDERER_TILEMAP:
				{
					if (current->at(selected)->Detail() == false)
					{
						obj->transform.position.z = 0;
						obj->transform.rotation.x = 0;
						obj->transform.rotation.y = 0;
						obj->transform.rotation.y = 0;
						obj->transform.scale.y = 0;
						obj->transform.scale.z = 0;
						ImGui::PushItemWidth(130);

						ImGui::Separator();
						ImGui::Text("POS");
						ImGui::Text("X:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##pos", &obj->transform.position.x, -1000, 1600);
						ImGui::SameLine();
						ImGui::Text("Y:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##pos1", &obj->transform.position.y, -1000, 1600);



						ImGui::Separator();
						ImGui::Text("SCALE");
						ImGui::Text(u8"크기:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##scale", &obj->transform.scale.x, 0.01, 10);
						ImGui::PopItemWidth();



					}
					else
					{
						CheckTransform();

						ImGui::PushItemWidth(130);

						ImGui::Separator();
						ImGui::Text("POS");
						ImGui::Text("X:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##pos3", &obj->transform.position.x);
						ImGui::SameLine();
						ImGui::Text("Y:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##pos4", &obj->transform.position.y);


						ImGui::Separator();
						ImGui::Text("SCALE");
						ImGui::Text(u8"크기:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##scale3", &obj->transform.scale.x, 0.1, 10);
						ImGui::PopItemWidth();


					}
					ImGui::Separator();
					bool check_detail = Hobj->Detail();
					ImGui::Checkbox("Detail", &check_detail);
					Hobj->SetDetail(check_detail);

					ImGui::Separator();
					break;

				}

				case Component::Type::SCRIPT:
				{
					ImGui::PushItemWidth(130);

					if (Hobj->Detail() == false)
					{

						ImGui::Separator();
						ImGui::Text("POS");
						ImGui::Text("X:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##pos", &obj->transform.position.x, -1000, 1600);
						ImGui::SameLine();
						ImGui::Text("Y:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##pos1", &obj->transform.position.y, -1000, 1600);
						ImGui::SameLine();
						ImGui::Text("Z:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##pos2", &obj->transform.position.z, -1000, 1600);


						ImGui::Separator();
						ImGui::Text("ROTATION");
						ImGui::Text("X:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##rot", &obj->transform.rotation.x, -3.16, 3.16);
						ImGui::SameLine();
						ImGui::Text("Y:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##rot1", &obj->transform.rotation.y, -3.16, 3.16);
						ImGui::SameLine();
						ImGui::Text("Z:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##rot2", &obj->transform.rotation.z, -3.16, 3.16);

						ImGui::Separator();
						ImGui::Text("SCALE");
						ImGui::Text("X:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##scale", &obj->transform.scale.x, 0.01, 10);
						ImGui::SameLine();
						ImGui::Text("Y:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##scale1", &obj->transform.scale.y, 0.01, 10);
						ImGui::SameLine();
						ImGui::Text("Z:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##scale2", &obj->transform.scale.z, 0.01, 10);
						ImGui::PopItemWidth();


					}
					else
					{
						CheckTransform();

						ImGui::Separator();
						ImGui::Text("POS");
						ImGui::Text("X:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##pos3", &obj->transform.position.x);
						ImGui::SameLine();
						ImGui::Text("Y:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##pos4", &obj->transform.position.y);
						ImGui::SameLine();
						ImGui::Text("Z:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##pos5", &obj->transform.position.z);

						ImGui::Separator();
						ImGui::Text("ROTATION");
						ImGui::Text("X:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##rot3", &obj->transform.rotation.x);
						ImGui::SameLine();
						ImGui::Text("Y:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##rot4", &obj->transform.rotation.y);
						ImGui::SameLine();
						ImGui::Text("Z:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##rot5", &obj->transform.rotation.z);

						ImGui::Separator();
						ImGui::Text("SCALE");
						ImGui::Text("X:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##scale3", &obj->transform.scale.x, 0.1, 10);
						ImGui::SameLine();
						ImGui::Text("Y:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##scale4", &obj->transform.scale.y, 001, 10);
						ImGui::SameLine();
						ImGui::Text("Z:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##scale5", &obj->transform.scale.z, 0.1, 10);
						ImGui::PopItemWidth();



					}
					ImGui::Separator();
					bool check_detail = Hobj->Detail();
					ImGui::Checkbox("Detail", &check_detail);
					Hobj->SetDetail(check_detail);

					ImGui::Separator();
					break;

				}


				}

				});
		}
		else
		{
			obj->ComponentForeach([&](Component* c) {
				int count = 0;
				std::string name = c->GetName();
				Component::Type type = c->GetType();

				switch (type)
				{
				case Component::Type::RENDERER_SPRITE:
				{
					obj->transform.position.z = 0;
					obj->transform.rotation.x = 0;
					obj->transform.rotation.y = 0;
					obj->transform.scale.y = 0;
					obj->transform.scale.z = 0;
					if (Hobj->Detail() == false)
					{
						ImGui::PushItemWidth(130);

						ImGui::Separator();
						ImGui::Text("POS");
						ImGui::Text("X:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##pos", &obj->transform.position.x, 0, 1600);
						ImGui::SameLine();
						ImGui::Text("Y:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##pos1", &obj->transform.position.y, 0, 1600);

						ImGui::Separator();
						ImGui::Text("ROTATION");
						ImGui::Text("Z:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##rot2", &obj->transform.rotation.z, -3.16, 3.16);

						ImGui::Separator();
						ImGui::Text("SCALE");
						ImGui::Text(u8"크기:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##scale", &obj->transform.scale.x, 0.01, 10);
						ImGui::PopItemWidth();



					}
					else
					{
						CheckSpriteTransform();

						ImGui::PushItemWidth(130);

						ImGui::Separator();
						ImGui::Text("POS");
						ImGui::Text("X:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##pos3", &obj->transform.position.x);
						ImGui::SameLine();
						ImGui::Text("Y:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##pos4", &obj->transform.position.y);


						ImGui::Separator();
						ImGui::Text("ROTATION");
						ImGui::Text("Z:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##rot5", &obj->transform.rotation.z, -3.16f, 3.16f);

						ImGui::Separator();
						ImGui::Text("SCALE");
						ImGui::Text(u8"크기:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##scale3", &obj->transform.scale.x, 0.01, 10);
						ImGui::PopItemWidth();


					}
					ImGui::Separator();
					bool check_detail = Hobj->Detail();
					ImGui::Checkbox("Detail", &check_detail);
					Hobj->SetDetail(check_detail);

					ImGui::Separator();
					break;
				}

				case Component::Type::RENDERER_MODEL:
				{
					if (current->at(selected)->Detail() == false)
					{
						ImGui::PushItemWidth(130);

						ImGui::Separator();
						ImGui::Text("POS");
						ImGui::Text("X:");

						ImGui::SameLine();
						ImGui::SliderFloat(u8"##pos", &obj->transform.position.x, -100, 100);
						ImGui::SameLine();
						ImGui::Text("Y:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##pos1", &obj->transform.position.y, -100, 100);
						ImGui::SameLine();
						ImGui::Text("Z:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##pos2", &obj->transform.position.z, -100, 100);


						ImGui::Separator();
						ImGui::Text("ROTATION");
						ImGui::Text("X:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##rot", &obj->transform.rotation.x, -3.16, 3.16);
						ImGui::SameLine();
						ImGui::Text("Y:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##rot1", &obj->transform.rotation.y, -3.16, 3.16);
						ImGui::SameLine();
						ImGui::Text("Z:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##rot2", &obj->transform.rotation.z, -3.16, 3.16);

						ImGui::Separator();
						ImGui::Text("SCALE");
						ImGui::Text("X:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##scale", &obj->transform.scale.x, 0.01, 10);
						ImGui::SameLine();
						ImGui::Text("Y:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##scale1", &obj->transform.scale.y, 0.01, 10);
						ImGui::SameLine();
						ImGui::Text("Z:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##scale2", &obj->transform.scale.z, 0.01, 10);
						ImGui::PopItemWidth();


					}
					else
					{
						CheckTransform();

						ImGui::PushItemWidth(130);

						ImGui::Separator();
						ImGui::Text("POS");
						ImGui::Text("X:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##pos3", &obj->transform.position.x);
						ImGui::SameLine();
						ImGui::Text("Y:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##pos4", &obj->transform.position.y);
						ImGui::SameLine();
						ImGui::Text("Z:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##pos5", &obj->transform.position.z);


						ImGui::Separator();
						ImGui::Text("ROTATION");
						ImGui::Text("X:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##rot3", &obj->transform.rotation.x, -3.16f, 3.16f);
						ImGui::SameLine();
						ImGui::Text("Y:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##rot4", &obj->transform.rotation.y, -3.16f, 3.16f);
						ImGui::SameLine();
						ImGui::Text("Z:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##rot5", &obj->transform.rotation.z, -3.16f, 3.16f);

						ImGui::Separator();
						ImGui::Text("SCALE");
						ImGui::Text("X:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##scale3", &obj->transform.scale.x, 0.01, 10);
						ImGui::SameLine();
						ImGui::Text("Y:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##scale4", &obj->transform.scale.y, 0.01, 10);
						ImGui::SameLine();
						ImGui::Text("Z:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##scale5", &obj->transform.scale.z, 0.01, 10);
						ImGui::PopItemWidth();



					}
					ImGui::Separator();
					bool check_detail = current->at(selected)->Detail();
					ImGui::Checkbox("Detail", &check_detail);
					current->at(selected)->SetDetail(check_detail);

					ImGui::Separator();
					break;
				}

				case Component::Type::RENDERER_TILEMAP:
				{
					if (current->at(selected)->Detail() == false)
					{
						ImGui::PushItemWidth(130);

						obj->transform.position.z = 0;
						obj->transform.rotation.x = 0;
						obj->transform.rotation.y = 0;
						obj->transform.rotation.z = 0;
						obj->transform.scale.y = 0;
						obj->transform.scale.z = 0;

						ImGui::Separator();
						ImGui::Text("POS");
						ImGui::Text("X:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##pos", &obj->transform.position.x, -100, 100);
						ImGui::SameLine();
						ImGui::Text("Y:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##pos1", &obj->transform.position.y, -100, 100);



						ImGui::Separator();
						ImGui::Text("SCALE");
						ImGui::Text(u8"크기:");
						ImGui::SameLine();
						ImGui::SliderFloat(u8"##scale", &obj->transform.scale.x, 0.01, 10);
						ImGui::PopItemWidth();



					}
					else
					{
						CheckTransform();

						ImGui::PushItemWidth(130);

						ImGui::Separator();
						ImGui::Text("POS");
						ImGui::Text("X:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##pos3", &obj->transform.position.x);
						ImGui::SameLine();
						ImGui::Text("Y:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##pos4", &obj->transform.position.y);


						ImGui::Separator();
						ImGui::Text("SCALE");
						ImGui::Text(u8"크기:");
						ImGui::SameLine();
						ImGui::InputFloat(u8"##scale3", &obj->transform.scale.x, 0.1, 10);
						ImGui::PopItemWidth();


					}
					ImGui::Separator();
					bool check_detail = Hobj->Detail();
					ImGui::Checkbox("Detail", &check_detail);
					Hobj->SetDetail(check_detail);

					ImGui::Separator();
					break;

				}


				}

				});



		}


	}

};