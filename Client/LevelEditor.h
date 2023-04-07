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
	bool opt_enable_context_menu = true;
	bool adding_line = false;
	bool level_edit = true;
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

		//float x = 32 / image->GetWidth();
		//float y = 32 / image->GetHeight();

		float width = image->GetWidth();
		float height = image->GetHeight();

		static float mouse_x;
		static float mouse_y;

		if (level_edit) {
			//ImGui::SetNextWindowSize(ImVec2(450, 500));	//������ ����
			ImGui::Begin(u8"Level Editor", &level_edit, ImGuiWindowFlags_HorizontalScrollbar);	//UIâ�� ���� �޴� ���ð� â�ݱⰡ �����ϵ��� MenuBar�� ����

			if (ImGui::Button("add")) {		//�� â ����, �̹������� ������ �κ�(ũ�� 32,32)�� ����ϵ��� ����
				isActiveWindow2 = true;
			}

			ImGui::Checkbox("Grid", &opt_enable_grid);								//ĵ���� ������ grid ǥ�ÿ���
			ImGui::Checkbox("Context Menu", &opt_enable_context_menu);				//���ؽ�Ʈ �޴� ǥ�ÿ���
			ImGui::Text("Mouse Left: drag to add lines,\nMouse Right: drag to scroll, click for context menu.");

			ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList�� ȭ���� ��ǥ�� ������ ���, UIâ �̵��� ĵ������ ��ǥ�� ���� ������
			ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   // ������ â ����� ���߾� ĵ���� ����� ����
			if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
			if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
			ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

			ImGuiIO& io = ImGui::GetIO();
			ImDrawList* draw_list = ImGui::GetWindowDrawList();
			draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));	//ĵ���� ���� ����� �׵θ� ����
			draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

			//���콺�� ĵ���� �ȿ� �������� ���� ������ �����ϴ� �κ�
			ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
			const bool is_hovered = ImGui::IsItemHovered(); // ĵ���� ���ο��� �����̴� ������ ���
			const bool is_active = ImGui::IsItemActive();   // ĵ���� ���ο��� Ŭ���� ������ ���
			const ImVec2 origin(canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y);				// Lock scrolled origin
			const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);	//ĵ���� ������ ���콺 ��ǥ�� �޾ƿ�

			//ĵ���� draw_list â ���ο� �̹��� ����, ������ ����׵θ����� ���߿� ȣ��Ǿ� ��ܿ� �׷��� 
			//grid���� ���� ȣ��Ǿ� �̹��� ���� grid�� �׷��� �� �ְ���
			//ĵ���� ������ ����� ��쿡�� �̹����� �������� �ʴ� ������ ���� ������ �ʿ���
			draw_list->AddImage((void*)image->Get(), origin, ImVec2(origin.x + width, origin.y + height), ImVec2(0, 0), ImVec2(1, 1));

			//���콺�� ĵ���� ���ο� �����ִ� ���¿��� ��Ŭ���� ���
			if (is_hovered && !adding_line && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
			{
				points.push_back(mouse_pos_in_canvas);
				points.push_back(mouse_pos_in_canvas);

				mouse_x = mouse_pos_in_canvas.x / 32;
				mouse_y = mouse_pos_in_canvas.y / 32;
				
				//unit size = 32px
				std::cout << width << std::endl;
				std::cout << height << std::endl;

				std::cout << (int)mouse_x << std::endl;
				std::cout << (int)mouse_y << std::endl;
				isActiveWindow2 = true;
			}

			ImGui::Begin(u8"Added");

			float x_unit = 1 / 9.0f;
			float y_unit = 1 / 12.0f;

			ImGui::Image((void*)image->Get(), ImVec2(125, 125), 
				ImVec2((int)mouse_x * x_unit, (int)mouse_y * y_unit), 
				ImVec2((int)mouse_x * x_unit + x_unit, (int)mouse_y * y_unit + y_unit));
			if (ImGui::Button("end")) {
				isActiveWindow2 = false;
			}
			ImGui::End();

			//���콺�� ĵ���� ���ο��� �巡�� ���� ��Ŭ���� ��� ĵ���� ���ο��� ���ؽ�Ʈ �޴� â�� ���
			//����� üũ�ڽ� context menu�� üũ ������ ��� ������ ����
			ImVec2 drag_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
			if (opt_enable_context_menu && drag_delta.x == 0.0f && drag_delta.y == 0.0f)
				ImGui::OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);
			if (ImGui::BeginPopup("context"))
			{
				if (adding_line)
					points.resize(points.size() - 2);
				adding_line = false;
				if (ImGui::MenuItem("Remove Last One", NULL, false, points.Size > 0)) { points.resize(points.size() - 2); }
				if (ImGui::MenuItem("Remove All", NULL, false, points.Size > 0)) { points.clear(); }
				ImGui::EndPopup();
			}
			//ĵ���� ���ο� Grid�� ǥ���ϴ� �κ�, üũ�ڽ� Grid�� ������ ��� ĵ���� ���ο� grid�� �׸��� ����
			draw_list->PushClipRect(canvas_p0, canvas_p1, true);
			if (opt_enable_grid)
			{
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