#pragma once
#include "pch.h"
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <Framework/IFrameworkFactory.h>
#include <Framework/Framework.h>
#include <Framework/GameObjectManager.h>
#include <Framework/TileMapRenderer.h>
#include <Framework/TileMap.h>

class LevelEditExample : public ILayer
{
public:
	IGameObjectManager* ObjM;
	IResourceManager* ResM;
	ImVector<ImVec2> points;
	Sprite* image;
	Sprite* image2;
	bool opt_enable_grid = true;
	bool Resizable = true;
	bool TilemapWindow = false;
	bool SavemapWindow = false;
	bool SaveAlert = false;
	bool ResetAlert = false;
	bool AlreadyClosedAlert = false;

	int tilemap_size[2] = {};
	int tilemap_height, tilemap_width, mouse_cnt, size;
	char tilemap_name[20] = {};
	float* mouse_x = nullptr;
	float* mouse_y = nullptr;
	float** mouse_col = nullptr;
	
	float x_unit, y_unit, width, height;

	//3�� 28�� Ÿ�ϸ� ���� + UI ���ο� �̹������� ���Կ� ���ҽ��Ŵ��� �߰� 
	LevelEditExample(IGameObjectManager* manager, IResourceManager* res, const std::string name) : ILayer(name) {
		this->ObjM = manager;
		this->ResM = res;
		image = ResM->FindSprite("test");
		image2 = ResM->FindSprite("empty");
		width = image->GetWidth();
		height = image->GetHeight();

		//�̹��� ������ ũ�Ⱑ Grid ĭ ���� ������ 9ĭ, �Ʒ��� 12ĭ
		x_unit = 1 / 9.0f;
		y_unit = 1 / 12.0f;
		mouse_cnt = 0;

		tilemap_height = tilemap_size[0];
		tilemap_width = tilemap_size[1];
		size = tilemap_height * tilemap_width;
	}
	virtual ~LevelEditExample()
	{
		//���� �迭 �Ҵ�����
		delete[] mouse_x;
		delete[] mouse_y;
	};

	virtual void Init()
	{

	};
	virtual void Update()
	{

	};

	/// <summary>
	/// ��Ȳ�� ���â ���
	/// </summary>
	void Alert() {
		if (SaveAlert)
			ImGui::OpenPopup("Save Map");

		if (ImGui::BeginPopupModal("Save Map", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text(u8"Map Has Been Saved");
			if (ImGui::Button("OK") || ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter))){
				ImGui::CloseCurrentPopup();
				SaveAlert = false;
			}
			ImGui::EndPopup();
		}

		if (ResetAlert)
			ImGui::OpenPopup("Reset");

		if (ImGui::BeginPopupModal("Reset", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text(u8"Map Reset Complete");
			ImGui::Text(u8"Please Retype Size");
			if (ImGui::Button("OK") || ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter))) {
				ImGui::CloseCurrentPopup();
				ResetAlert = false;
			}
			ImGui::EndPopup();
		}

		if (AlreadyClosedAlert)
			ImGui::OpenPopup("Select Window Closed");

		if (ImGui::BeginPopupModal("Select Window Closed", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text(u8"Please Reopen Tilemap");
			if (ImGui::Button("OK") || ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter))) {
				ImGui::CloseCurrentPopup();
				AlreadyClosedAlert = false;
			}
			ImGui::EndPopup();
		}
	}

	/// <summary>
	/// ������ Ÿ�ϸ��� �����ϰ� �� �̸��� �Է¹޴� â
	/// </summary>
	void Save() {
		if(SavemapWindow) {
			ImGui::Begin(u8"Save Tilemap", &SavemapWindow);
			ImGui::PushItemWidth(100);
			TileMap* map = new TileMap();

			ImGui::InputText("Tilemap Name", tilemap_name, 20);
			if(ImGui::Button("Save This Name") && tilemap_name) {
				map->Init(tilemap_name, image, tilemap_width, tilemap_height);
			
				for (int k = 0; k < tilemap_height * tilemap_width; k++) {
					//gridX, gridY�� k / tilemap_width�� k % tilemap_width�� �Է�
					map->SelectTile((int)mouse_x[k], (int)mouse_y[k], k / tilemap_width, k % tilemap_width);
				}

				this->ResM->RegisterTileMap(tilemap_name, map);
				SaveAlert = true;

				//Save�� ���� ����ǰ�, â�� ���� �� ���ο� �Է��� ���� ���� ���� ������ �ʱ�ȭ
				SavemapWindow = false;
				TilemapWindow = false;
				for (int i = 0; i < tilemap_width * tilemap_height; i++) {
					mouse_x[i] = { 0 };
					mouse_y[i] = { 0 };
				}
				mouse_cnt = 0;
				tilemap_height = 0;
				tilemap_width = 0;
				Resizable = true;
			}
			
			ImGui::End();
		}
	}

	/// <summary>
	/// ������ �̹����� �Էµ� �迭�� ȭ�鿡 ����ϴ� â
	/// </summary> 
	void SelectTile() {
		if (TilemapWindow) {	//â�� ũ��� Ÿ�ϸ��� ũ�⿡ ���߾� �ڵ����� ������
			ImGui::Begin(u8"Chosen Grid Block", &TilemapWindow, ImGuiWindowFlags_AlwaysAutoResize);

			//���� ������ Ÿ���� Image �������� ���
			ImGui::Text("Current Tile");
			ImGui::Image((void*)image->Get(), ImVec2(125, 125),
				ImVec2((int)mouse_x[mouse_cnt - 1] * x_unit, (int)mouse_y[mouse_cnt - 1] * y_unit),
				ImVec2((int)mouse_x[mouse_cnt - 1] * x_unit + x_unit, (int)mouse_y[mouse_cnt - 1] * y_unit + y_unit));
		
			//��ü Ÿ�ϸ��� Image �������� ���
			ImGui::Text("Tilemap");
			ImGui::Text("");
			
			for (int i = 0, k = 0; i < tilemap_height; i++) {
				for (int j = 0; j < tilemap_width; j++, k++) {
					ImGui::SameLine();
					if (mouse_cnt - 1 < 0 || k > mouse_cnt - 1) {
						ImGui::Image((void*)image2->Get(), ImVec2(50, 50), ImVec2(0,0), ImVec2(10,10));
					}
					else{
						ImGui::Image((void*)image->Get(), ImVec2(50, 50),
							ImVec2((int)mouse_x[k] * x_unit, (int)mouse_y[k] * y_unit),
							ImVec2((int)mouse_x[k] * x_unit + x_unit, (int)mouse_y[k] * y_unit + y_unit));
					}
				}
				ImGui::Text("");	//�ٹٲ��� ���� �� �ؽ�Ʈ ���
			}
			if (ImGui::Button("Save"))
				SavemapWindow = true;
			ImGui::SameLine();

			//���� �迭�� ����� ���� �迭 ũ�⸦ ��� �ʱ�ȭ, �ٽ� ų �� ���Ӱ� ���� �ޱ� ���� ī���͵� �Բ� �ʱ�ȭ
			//5�� 8�� �ʱ�ȭ �� â�� �ƿ� �������� ����
			//5�� 15�� â�� x��ư���� â�� ���� ���� Reset ��ư�� ������ ���� �����ϰ� â�� ����ǵ��� ����
			if (ImGui::Button("Reset Tile")) {
				ResetAlert = true;
				ImGui::End();
				
				for (int i = 0; i < tilemap_width * tilemap_height; i++) {
					mouse_x[i] = { 0 };
					mouse_y[i] = { 0 };
				}
				mouse_cnt = 0;
				tilemap_height = 0;
				tilemap_width = 0;
				Resizable = true;
				TilemapWindow = false;
			}
			else
				ImGui::End();
		}
	}

	/// <summary>
	/// �� ������ �⺻â
	/// </summary>
	virtual void Render()
	{
		static ImVec2 scrolling(0.0f, 0.0f);

		auto keyboard = InputManager::GetKeyboardState();
		auto mouse = InputManager::GetMouseState();

		Alert();
		SelectTile();
		Save();
		if (_isActive) {
			ImGui::Begin(u8"Level Editor", &_isActive, ImGuiWindowFlags_HorizontalScrollbar);
			ImGui::Checkbox("Grid", &opt_enable_grid);	//ĵ���� ������ grid ǥ�ÿ���
			
			ImGui::Text("Tile Map Size");
			ImGui::SameLine();
			ImGui::PushItemWidth(100);

			//Resizable�� ��Ȱ��ȭ�Ǹ� ��ǲ�� �Էµ� ���ڸ� ������
			//������ ũ���� �迭�� ���븸 �ʱ�ȭ�ϰ� ���� �� ���� �� 
			//�ٸ� ũ��� �迭�� ������ ���� ��, �ѹ� �� ������ ����
			//�ٽ� ���� ���ϴ� ũ���� �迭�� �����ؾ� �ϴ� ������ ����
			if (ImGui::InputInt2(" ", tilemap_size) && Resizable) {
				tilemap_height = tilemap_size[0];
				tilemap_width = tilemap_size[1];
			}

			if (ImGui::Button("Open Tilemap")) {
				//new[]�� ���� �迭 �Ҵ� �� �ʱ�ȭ
				mouse_x = new float[tilemap_height * tilemap_width] { };
				mouse_y = new float[tilemap_height * tilemap_width] { };
				
				if (!Resizable && !TilemapWindow) {
					AlreadyClosedAlert = true;

					for (int i = 0; i < tilemap_width * tilemap_height; i++) {
						mouse_x[i] = { 0 };
						mouse_y[i] = { 0 };
					}
					mouse_cnt = 0;
					tilemap_height = 0;
					tilemap_width = 0;
					Resizable = true;
				}


				//Ÿ�ϸ� ���� â�� ���� ���Ŀ� �迭 ũ�Ⱑ ������� ���ϰ� ���Ƶ�
				if (tilemap_height != 0 && tilemap_width != 0){
					Resizable = false;
					TilemapWindow = true;
				}
			}
			ImGui::PopItemWidth();
			
			ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      
			ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   
			if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
			if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
			ImVec2 canvas_p1 = ImVec2(canvas_p0.x + width, canvas_p0.y + height);	
			//ĵ���� ũ�⸦ â ũ��� ����ȭ���� �ʰ� �̹��� ������� ����

			ImGuiIO& io = ImGui::GetIO();
			ImDrawList* draw_list = ImGui::GetWindowDrawList();
			draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));	
			draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

			//���콺�� ĵ���� �ȿ� �������� ���� ������ �����ϴ� �κ�
			ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
			const bool is_hovered = ImGui::IsItemHovered(); 
			const bool is_active = ImGui::IsItemActive();   
			const ImVec2 origin(canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y);				
			const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);	//ĵ���� ������ ���콺 ��ǥ

			//ĵ���� draw_list â ���ο� �̹��� ����, ������ ����׵θ����� ���߿� ȣ��Ǿ� ��ܿ� �׷��� 
			//grid���� ���� ȣ��Ǿ� �̹��� ���� grid�� �׷��� �� �ְ���
			draw_list->AddImage((void*)image->Get(), origin, 
				ImVec2(origin.x + width, origin.y + height), 
				ImVec2(0, 0), ImVec2(1, 1));

			//���콺�� ĵ���� ���ο��� ��Ŭ���� ��� + Ÿ�ϸ� â�� ������ ���
			if (is_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left) &&TilemapWindow) {
				points.push_back(mouse_pos_in_canvas);
				points.push_back(mouse_pos_in_canvas);

				//���� �迭�� ũ�⸦ �ʰ����� �ʴ� �������� ���콺 ��ǥ�� ����
				if(mouse_cnt < tilemap_width * tilemap_height)	{
					//���� ���콺 ��ǥ�� Grid �� ĭ ũ���� 32�ȼ��� ������ ǥ��
					mouse_x[mouse_cnt] = mouse_pos_in_canvas.x / 32;
					mouse_y[mouse_cnt] = mouse_pos_in_canvas.y / 32;

					mouse_cnt++;
				}
			}

			//ĵ���� ���ο� Grid�� ǥ���ϴ� �κ�, üũ�ڽ� Grid�� ������ ��� ĵ���� ���ο� grid�� �׸��� ����
			draw_list->PushClipRect(canvas_p0, canvas_p1, true);
			if (opt_enable_grid) {
				const float GRID_STEP = 32.0f;
				for (float x = fmodf(scrolling.x, GRID_STEP); x < canvas_sz.x; x += GRID_STEP)
					draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), IM_COL32(200, 200, 200, 250));
				for (float y = fmodf(scrolling.y, GRID_STEP); y < canvas_sz.y; y += GRID_STEP)
					draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), IM_COL32(200, 200, 200, 250));
			}
			for (int n = 0; n < points.Size; n += 2)
				draw_list->AddLine(ImVec2(origin.x + points[n].x, origin.y + points[n].y), ImVec2(origin.x + points[n + 1].x, origin.y + points[n + 1].y), IM_COL32(255, 255, 0, 255), 2.0f);
			draw_list->PopClipRect();

			ImGui::End();
		}
	};
};