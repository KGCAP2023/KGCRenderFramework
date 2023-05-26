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

	//�������� ������Ʈ �߰� 
	void AddDeleteComponent(Component::Type type)
	{
		_delete.push_back(type);
	}

	//�������� ������Ʈ �ϰ����� 
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

	GameObject* obj;          //GameObject �Լ��� ������ �ϴ� obj = current->at(selected)->getGameObject(); �� ǥ����
	HierarchyObject* Hobj;    //HierarachyObject�� ���� �ϱ� ����  �� obj = current->at(selected); �� ǥ��
	bool check1 = true;
	bool check2 = true;
	bool check3 = true;

	int test_check = 1;

	int component_selected = 0;
	int selected = 0;
	//â ���� Ű�� bool ������
	bool my_tool_active = true;
	bool active = false;
	bool component_active = false;

	// �˾�â�� ���� bool ������
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
	/// map �Լ��� �̿��Ͽ� ��� ���� vector 
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
			//gamelist -> gametestlist �� ����
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
			// �����ֱ�
			gameTestList.clear();

		}

	}
	/// <summary>
	/// Object�� ������Ʈ ��� �ڵ带 ��Ƶ� �Լ�
	/// </summary>
	void component()
	{
		//obj �� gameObject�� �Լ�, Hobj�� HierarcyObject�� �Լ�
		obj = current->at(selected)->GetGameObject();
		Hobj = current->at(selected);

		//render�� ���Ʊ� ������ �ٽ� ���� �ö� �����
		spriteList.clear();
		modelList.clear();
		tileList.clear();

		// sprite , model , tile �� map �� �����Ͽ� ���� �� �ڵ�
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
		// ������Ʈ�� �߰� �Ǹ� forEach ������ Ž���ϸ鼭 switch ������ �� ������Ʈ Ÿ�� ���� ����� ĭ ���� �ϴ� �ڵ�
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
	/// �˾� ����� �̿��ؼ� ���â �����ϴ� �ڵ� ��Ƶ� �Լ� �Դϴ�.
	/// </summary>
	void Warning()
	{
		if (show_warning) {
			ImGui::OpenPopup("Warning");
		}

		if (ImGui::BeginPopupModal("Warning", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text(u8"�ߺ� ������Ʈ ����");
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
			ImGui::Text(u8"�������϶� ��ũ��Ʈ�� ���Ե� ������Ʈ�� ������Ʈ�� �����Ҽ� �����ϴ�.");
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
			ImGui::Text(u8"���� ����϶� ��ũ��Ʈ ������ �ȵ˴ϴ�.");
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
			ImGui::Text(u8"�������� 1�� �����մϴ�");
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
			ImGui::Text(u8"��ũ��Ʈ�� 1�� �����մϴ�");
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
			ImGui::Text(u8"���̻� ������Ʈ �߰��� �Ұ����մϴ�");
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
			ImGui::Text(u8"���� �Ǿ����ϴ�.");
			if (ImGui::Button(u8"Ȯ��") || ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)))
			{

				ImGui::CloseCurrentPopup();
				show_delete = false;

			}
			ImGui::EndPopup();
		}
		if (show_not_exist) {
			ImGui::OpenPopup(u8"�������� �ʴ� ����");
		}

		if (ImGui::BeginPopupModal(u8"�������� �ʴ� ����", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text(u8"������ �������� �ʽ��ϴ�.");
			if (ImGui::Button(u8"Ȯ��") || ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)))
			{

				ImGui::CloseCurrentPopup();
				show_not_exist = false;

			}
			ImGui::EndPopup();
		}

	}
	/// <summary>
	/// inputfloat �ҋ� �ִ밪 �ּҰ� �Ѿ�� �ִ밪 �ּҰ����� �����ϰ� ������� ���� �Լ� �Դϴ�.
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
	/// inputfloat �ҋ� �ִ밪 �ּҰ� �Ѿ�� �ִ밪 �ּҰ����� �����ϰ� ������� ���� �Լ� �Դϴ�.
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
		/// Pos x , y ,z �� ���� ���� -100 ���� �۰ų� 100���� ũ�� -100 ,100 ������ ġȯ �����ִ� ��
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
		/// Rot x , y ,z �� ���� ���� -1.58 ���� �۰ų� 1.58���� ũ�� -1.58 ,1.58 ������ ġȯ �����ִ� ��
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
		/// Scale x , y ,z �� ���� ���� 1 ���� �۰ų� 3���� ũ�� 1 ,3 ������ ġȯ �����ִ� ��
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
		/// ���� �ص� �˾� â �ٿ�� �ڵ� ��Ƶ� �Լ� �߰�
		/// </summary>
		Warning();

		/// ������Ʈ �߰��ϴ� â 
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


		// ������ â 
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



			//object ����Ʈ �����ִ� ��
			// SetFocus()��ɵ� ����

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
				// ��ǥ ��� �ڵ� ����ִ� �Լ�
				Transform();
				ImGui::Text("Other Component");
				if (current->at(selected)->GetGameObject()->GetComponentSize() != 0)
					ImGui::Separator();
				// ������Ʈ �߰� ��ư �� �ڵ� ����ִ� �Լ�
				component();


				ImGui::Separator();



				ImGui::SameLine();
				ImGui::EndGroup();
				ImGui::SameLine();

			}
			ImGui::End();
		}

		// ������Ʈ �Լ� �ȿ� �ִ� component_active = true �Ǹ� ������ ������Ʈ �߰��ϴ� â 
		if (component_active)
		{
			ImGui::Begin("Add Component", &component_active);

			GameObject* obj = current->at(selected)->GetGameObject();
			componentList.clear();
			auto map = obj->GetComponentMap();
			// coponentList�� obj ������Ʈ ���� ������ �ޱ�
			for (auto& pair : map)
			{
				Component::Type type = pair.first;
				componentList.push_back(type);

			}
			// sprite ��ư �������� for������ List Ž���ؼ� ���� ���� Ž��
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
			// model ��ư �������� for������ List Ž���ؼ� ���� ���� Ž��

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
			// TileMapRender ��ư �������� for������ List Ž���ؼ� ���� ���� Ž��

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
			// Script ��ư �������� for������ List Ž���ؼ� ���� ���� Ž��

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
	/// Object�� pos,rot,scale ũ�� �����ϴ� �� 
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
						ImGui::Text(u8"ũ��:");
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
						ImGui::Text(u8"ũ��:");
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
						ImGui::Text(u8"ũ��:");
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
						ImGui::Text(u8"ũ��:");
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
						ImGui::Text(u8"ũ��:");
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
						ImGui::Text(u8"ũ��:");
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
						ImGui::Text(u8"ũ��:");
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
						ImGui::Text(u8"ũ��:");
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