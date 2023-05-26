#pragma once
#include "pch.h"
#include "LevelEditor.h"
#include <Framework/InputManager.h>
#include <Framework/ResourceManager.h>
#include <Framework/GameObjectManager.h>
#include <Framework/TileMapRenderer.h>

LevelEditExample::LevelEditExample(IGameObjectManager* manager, IResourceManager* res, const std::string name) : ILayer(name)
{
	this->ObjM = manager;
	this->ResM = res;
	Map_Img = ResM->FindSprite("test");
	Emp_Img = ResM->FindSprite("empty");
	width = Map_Img->GetWidth();
	height = Map_Img->GetHeight();
	Changed_Map_Height = 0;
	Changed_Map_Width = 0;

	//�⺻ ������ Sand.PNG �̹��� ������ ũ��� Grid ĭ ���� ������ 9ĭ, �Ʒ��� 12ĭ
	//Grid�� �̹��� ������ 32��Ʈ������ ���� ��
	x_grid_unit = 1 / (width / 32.0f);
	y_grid_unit = 1 / (height / 32.0f);
	mouse_cnt = 0;

	//Ÿ�ϸ��� ũ��� ����ڰ� InputInt�� �Է��ϴ� ���� 2���� ����
	tilemap_height = input_size[0];
	tilemap_width = input_size[1];
	tilemap_size = tilemap_height * tilemap_width;
}

LevelEditExample::~LevelEditExample()
{
	//���� �迭 �Ҵ�����
	delete[] mouse_x;
	delete[] mouse_y;
};

void LevelEditExample::Render()
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

		ImGui::PushItemWidth(100);
		ImGui::Text("Change Map");
		ImGui::SameLine();

		ImGui::InputText(" ", Original_Img, 20);
		ImGui::SameLine();
		if (ImGui::Button("Change")) {
			if (ResM->FindSprite(Original_Img) != nullptr) {
				Map_Img = ResM->FindSprite(Original_Img);

				width = Map_Img->GetWidth();
				height = Map_Img->GetHeight();
				x_grid_unit = 1 / (width / 32.0f);
				y_grid_unit = 1 / (height / 32.0f);
			}
		}

		ImGui::Text("Tile Map Size");
		ImGui::SameLine();

		//Resizable�� ��Ȱ��ȭ�Ǹ� ��ǲ�� �Էµ� ���ڸ� ������
		if (ImGui::InputInt2(" ", input_size) && Resizable) {
			tilemap_height = input_size[0];
			tilemap_width = input_size[1];
			tilemap_size = tilemap_height * tilemap_width;
		}

		if (ImGui::Button("Open Tilemap")) {
			//new[]�� ���� �迭 �Ҵ� �� �ʱ�ȭ
			mouse_x = new float[tilemap_size] {};
			mouse_y = new float[tilemap_size] {};

			//â�� x��ư���� ���� �� ������ �ʱ�ȭ �� �ٽ� Ű���� ����
			if (!Resizable && !TilemapWindow) {
				AlreadyClosedAlert = true;

				for (int i = 0; i < tilemap_size; i++) {
					mouse_x[i] = { 0 };
					mouse_y[i] = { 0 };
				}
				mouse_cnt = 0;
				tilemap_height = 0;
				tilemap_width = 0;
				tilemap_size = 0;
				Resizable = true;
			}


			//Ÿ�ϸ� ���� â�� ���� ���Ŀ� �迭 ũ�Ⱑ ������� ���ϰ� ����
			if (tilemap_height != 0 && tilemap_width != 0) {
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
		draw_list->AddImage((void*)Map_Img->Get(), origin,
			ImVec2(origin.x + width, origin.y + height),
			ImVec2(0, 0), ImVec2(1, 1));

		//���콺�� ĵ���� ���ο��� ��Ŭ���� ��� + Ÿ�ϸ� â�� ������ ���
		if (is_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && TilemapWindow) {
			points.push_back(mouse_pos_in_canvas);
			points.push_back(mouse_pos_in_canvas);

			//���� �迭�� ũ�⸦ �ʰ����� �ʴ� �������� ���콺 ��ǥ�� ����
			if (mouse_cnt < tilemap_size) {
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
}

void LevelEditExample::Save()
{
	if (SavemapWindow) {
		ImGui::Begin(u8"Save Tilemap", &SavemapWindow);
		ImGui::PushItemWidth(100);
		TileMap* map = new TileMap();
		
		//InputText�� ���� ����ڷκ��� �̸��� ���� 20�� tilemap_name �迭�� �޾ƿ�
		ImGui::InputText("Tilemap Name", tilemap_name, 20);
		std::string tilemap_string(tilemap_name);
		if (ImGui::Button("Save This Name")) {
			if (tilemap_string.compare("") != 0) {
				//Tilemap* ������ map�� �̸�, �ʺ�, ����, ���� �̹����� ����
				map->Init(tilemap_name, Map_Img, tilemap_width, tilemap_height);

				//����ڰ� ������ �̹��� Ÿ�ϵ��� map�� ���ʴ�� ����
				for (int k = 0; k < tilemap_size; k++) {
					//gridX, gridY�� k / tilemap_width�� k % tilemap_width�� �Է�
					int a = (int)mouse_x[k];
					int b = (int)mouse_y[k];
					int c = k / tilemap_width;
					int d = k % tilemap_width;
					map->SelectTile(a, b, d, c);
				}

				//������ �̹��� Ÿ�ϵ��� �̸��� tilemap_name�� map���� ���Ӱ� ��� 
				this->ResM->RegisterTileMap(tilemap_name, map);
				SaveAlert = true;

				//Save�� ���� ����ǰ�, â�� ���� �� ���ο� �Է��� ���� ���� �Է� ������ ��� �ʱ�ȭ
				SavemapWindow = false;
				TilemapWindow = false;
				for (int i = 0; i < tilemap_size; i++) {
					mouse_x[i] = { 0 };
					mouse_y[i] = { 0 };
				}
				mouse_cnt = 0;
				tilemap_height = 0;
				tilemap_width = 0;
				tilemap_size = 0;
				Resizable = true;
			}
			else {
				NullName = true;
			}
		}

		ImGui::End();
	}
}

void LevelEditExample::SelectTile()
{
	if (TilemapWindow) {	//â�� ũ��� Ÿ�ϸ��� ũ�⿡ ���߾� �ڵ����� ������
		ImGui::Begin(u8"Chosen Grid Block", &TilemapWindow, ImGuiWindowFlags_AlwaysAutoResize);

		//���� ������ Ÿ���� 125,125 ũ���� Image �������� ���
		ImGui::Text("Current Tile");
		ImGui::Image((void*)Map_Img->Get(), ImVec2(125, 125),
			ImVec2((int)mouse_x[mouse_cnt - 1] * x_grid_unit, (int)mouse_y[mouse_cnt - 1] * y_grid_unit),
			ImVec2((int)mouse_x[mouse_cnt - 1] * x_grid_unit + x_grid_unit, (int)mouse_y[mouse_cnt - 1] * y_grid_unit + y_grid_unit));

		//��ü Ÿ�ϸ��� Image �������� ���
		ImGui::Text("Tilemap");
		ImGui::Text("");

		for (int i = 0, k = 0; i < tilemap_height; i++) {
			for (int j = 0; j < tilemap_width; j++, k++) {
				ImGui::SameLine();
				//���� ���� �������� ���� �κ��� ����� empty �̹����� ���
				if (mouse_cnt - 1 < 0 || k > mouse_cnt - 1) {
					ImGui::Image((void*)Emp_Img->Get(), ImVec2(50, 50), ImVec2(0, 0), ImVec2(10, 10));
				}
				//����ڰ� �̹����� Ŭ���� ����ȭ�� ��ǥ�� ����� �κ��� �ش� ��ǥ�� ���Ե� �̹����� ���
				else {
					ImGui::Image((void*)Map_Img->Get(), ImVec2(50, 50),
						ImVec2((int)mouse_x[k] * x_grid_unit, (int)mouse_y[k] * y_grid_unit),
						ImVec2((int)mouse_x[k] * x_grid_unit + x_grid_unit, (int)mouse_y[k] * y_grid_unit + y_grid_unit));
				}
			}
			ImGui::Text("");	//�ٹٲ��� ���� �� �ؽ�Ʈ ���
		}
		//Save ��ư Ŭ�� �� savemap â�� ���Ӱ� ����
		if (ImGui::Button("Save"))
			SavemapWindow = true;
		ImGui::SameLine();

		//�迭�� ���� �������� �Էµ� ��ǥ���� ����, Ŭ���� Ƚ�� ���� �ѹ� ����
		if (ImGui::Button("Undo")) {
			if (mouse_cnt > 0) {
				mouse_cnt--;
				mouse_x[mouse_cnt] = 0;
				mouse_y[mouse_cnt] = 0;
			}
		}
		ImGui::SameLine();

		//���� �迭�� ����� ���� �迭 ũ��, ī���͸� ��� �ʱ�ȭ
		//5�� 8�� �ʱ�ȭ �� â�� �ƿ� �������� ����
		//5�� 15�� â�� x��ư���� â�� ���� ���� Reset ��ư�� ������ ���� �����ϰ� â�� ����ǵ��� ����
		if (ImGui::Button("Reset Every Tile")) {
			ResetAlert = true;
			ImGui::End();

			for (int i = 0; i < tilemap_size; i++) {
				mouse_x[i] = { 0 };
				mouse_y[i] = { 0 };
			}
			mouse_cnt = 0;
			tilemap_height = 0;
			tilemap_width = 0;
			tilemap_size = 0;
			Resizable = true;
			TilemapWindow = false;
		}
		else
			ImGui::End();
	}
}

void LevelEditExample::Alert()
{
	//����Ϸ� �˸�
	if (SaveAlert)
		ImGui::OpenPopup("Save Map");

	if (ImGui::BeginPopupModal("Save Map", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text(u8"Map Has Been Saved");
		if (ImGui::Button("OK") || ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter))) {
			ImGui::CloseCurrentPopup();
			SaveAlert = false;
		}
		ImGui::EndPopup();
	}

	//�̸� �Է¾ȵ� ���� �˸�
	if (NullName)
		ImGui::OpenPopup("Name Alert");

	if (ImGui::BeginPopupModal("Name Alert", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text(u8"Please Type Name");
		if (ImGui::Button("OK") || ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter))) {
			ImGui::CloseCurrentPopup();
			NullName = false;
		}
		ImGui::EndPopup();
	}

	//���� ���� �˸�
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

	//Ÿ�ϸ� â�� �������� ���� �� ����� �� �˸�
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
