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
	bool opt_enable_grid = true;
	bool isActiveWindow2 = false;

	//������ �κ�, ���� ������ ����
	//3�� 28�� Ÿ�ϸ� ���� + UI ���ο� �̹������� ���Կ� ���ҽ��Ŵ��� �߰� 
	LevelEditExample(IGameObjectManager* manager, IResourceManager* res, const std::string name) : ILayer(name) {
		this->ObjM = manager;
		this->ResM = res;
		image = ResM->FindSprite("test");
	}
	virtual ~LevelEditExample()
	{

	};

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

		static ImVec2 scrolling(0.0f, 0.0f);

		float width = image->GetWidth();
		float height = image->GetHeight();

		static int tilemap_size[2];
		static int tilemap_height, tilemap_width;

		//�̹��� ������ ũ�Ⱑ Grid ĭ ���� ������ 9ĭ, �Ʒ��� 12ĭ
		float x_unit = 1 / 9.0f;
		float y_unit = 1 / 12.0f;

		if (_isActive) {
			ImGui::Begin(u8"Level Editor", &_isActive, ImGuiWindowFlags_HorizontalScrollbar);
			ImGui::Checkbox("Grid", &opt_enable_grid);	//ĵ���� ������ grid ǥ�ÿ���
			
			ImGui::Text("Tile Map Size");
			ImGui::SameLine();
			ImGui::PushItemWidth(100);
			ImGui::InputInt2(" ", tilemap_size);
			tilemap_height = tilemap_size[0];
			tilemap_width = tilemap_size[1];
			static int mouse_cnt = 0;

			if (ImGui::Button("Open Tilemap")) {
				if (tilemap_height != 0 && tilemap_width != 0)
					isActiveWindow2 = true;
			}
			ImGui::PopItemWidth();

			//���� �迭 ����
			//4��12��) free() ���� ������ ƨ��� ��찡 �־� ������ �ʿ���.
			static float* mouse_x = (float*)malloc(sizeof(int) * (tilemap_height * tilemap_width));
			static float* mouse_y = (float*)malloc(sizeof(int) * (tilemap_height * tilemap_width));
			
			ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      
			ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   
			if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
			if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
			//ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);
			ImVec2 canvas_p1 = ImVec2(canvas_p0.x + width, canvas_p0.y + height);	//ĵ���� ũ�⸦ â ũ��� ����ȭ���� �ʰ� �̹��� ������� ����

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
			if (is_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left) &&isActiveWindow2) {
				points.push_back(mouse_pos_in_canvas);
				points.push_back(mouse_pos_in_canvas);

				//���� �迭�� ũ�⸦ �ʰ����� �ʴ� �������� ���콺 ��ǥ�� ����
				if(mouse_cnt <= tilemap_width * tilemap_height)	{
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

			//4��13��)imagebutton ���ο� �̹��� ������ ����, ƨ��� ��츦 if������ ���ܽ�Ŵ
			if (isActiveWindow2) {	//Added â�� ������ �̹��� ���� ���, â�� ũ��� Ÿ�ϸ��� ũ�⿡ ���߾� �ڵ����� ������
				ImGui::Begin(u8"Chosen Grid Block", &isActiveWindow2, ImGuiWindowFlags_AlwaysAutoResize);
				
				//���� ������ Ÿ���� Image �������� ���
				ImGui::Text("Current Tile");
				ImGui::Image((void*)image->Get(), ImVec2(125,125), 
					ImVec2((int)mouse_x[mouse_cnt - 1] * x_unit, (int)mouse_y[mouse_cnt - 1] * y_unit),
					ImVec2((int)mouse_x[mouse_cnt - 1] * x_unit + x_unit, (int)mouse_y[mouse_cnt - 1] * y_unit + y_unit));

				//��ü Ÿ�ϸ��� ImageButton �������� ���
				ImGui::Text("Tilemap");
				ImGui::Text("");
				for (int i = 0, k = 0; i < tilemap_height; i++) {
					for (int j = 0; j < tilemap_width; j++, k++) {
						ImGui::SameLine();
						ImGui::ImageButton((void*)image->Get(), ImVec2(50, 50),
							ImVec2((int)mouse_x[k] * x_unit, (int)mouse_y[k] * y_unit),
							ImVec2((int)mouse_x[k] * x_unit + x_unit, (int)mouse_y[k] * y_unit + y_unit));
					}
					ImGui::Text("");	//�ٹٲ��� ���� �� �ؽ�Ʈ ���
				}
				ImGui::End();
			}

			ImGui::End();
		}
	};
};