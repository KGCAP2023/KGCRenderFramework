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

	//생성자 부분, 추후 변경할 예정
	//3월 28일 타일맵 예제 + UI 내부에 이미지파일 삽입용 리소스매니저 추가 
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

		float x = 32 / image->GetWidth();
		float y = 32 / image->GetHeight();

		if (level_edit) {
			ImGui::SetNextWindowSize(ImVec2(450, 500));
			ImGui::Begin(u8"Level Editor", &level_edit, ImGuiWindowFlags_MenuBar);	//UI창을 내부 메뉴 선택과 창닫기가 가능하도록 MenuBar로 설정

			ImGui::Image((void*)image->Get(), ImVec2(512, 512), ImVec2(1, 1), ImVec2(x, y));	//이미지 파일 띄우기 (크기는 512, 512로 고정)

			if (ImGui::Button("add")) {		//새 창 띄우기
				isActiveWindow2 = true;
			}
			if (isActiveWindow2) {
				ImGui::Begin(u8"Info2");
				ImGui::End();
			}

			ImGui::Checkbox("Grid", &opt_enable_grid);								//캔버스 내부의 grid 표시여부
			ImGui::Checkbox("Context Menu", &opt_enable_context_menu);				//컨텍스트 메뉴 표시여부
			ImGui::Text("Mouse Left: drag to add lines,\nMouse Right: drag to scroll, click for context menu.");

			ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList가 화면의 좌표를 가져와 사용, UI창 이동시 캔버스의 좌표도 같이 움직임
			ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   // 윈도우 창 사이즈에 맞추어 캔버스 사이즈를 조절
			if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
			if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
			ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

			ImGuiIO& io = ImGui::GetIO();
			ImDrawList* draw_list = ImGui::GetWindowDrawList();
			draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
			draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

			//마우스가 캔버스 안에 들어와있을 때의 동작을 감지하는 부분
			ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
			const bool is_hovered = ImGui::IsItemHovered(); // 캔버스 내부에서 움직이는 상태인 경우
			const bool is_active = ImGui::IsItemActive();   // 캔버스 내부에서 클릭된 상태인 경우
			const ImVec2 origin(canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y);				// Lock scrolled origin
			const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);	//캔버스 내부의 마우스 좌표를 받아옴

			//마우스가 캔버스 내부에 들어와있는 상태에서 좌클릭된 경우
			if (is_hovered && !adding_line && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
			{
				points.push_back(mouse_pos_in_canvas);
				points.push_back(mouse_pos_in_canvas);
				adding_line = true;
			}
			//마우스가 캔버스 내부에서 좌클릭되어(드래그 시작시점) 그리기가 활성화된 경우
			if (adding_line)
			{
				points.back() = mouse_pos_in_canvas;
				if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))	//마우스 클릭이 해제된 경우(드래그 종료시점)
					adding_line = false;
			}

			//마우스가 캔버스 내부에서 우클릭되어 드래깅 중인 경우 캔버스 내부의 좌표를 변경
			const float mouse_threshold_for_pan = opt_enable_context_menu ? -1.0f : 0.0f;
			if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, mouse_threshold_for_pan))
			{
				scrolling.x += io.MouseDelta.x;
				scrolling.y += io.MouseDelta.y;
			}

			//마우스가 캔버스 내부에서 드래깅 없이 우클릭된 경우 캔버스 내부에서 컨텍스트 메뉴 창을 띄움
			//상단의 체크박스 context menu가 체크 해제된 경우 켜지지 않음
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
			//캔버스 내부에 Grid를 표시하는 부분, 체크박스 Grid가 해제된 경우 캔버스 내부에 grid를 그리지 않음
			draw_list->PushClipRect(canvas_p0, canvas_p1, true);
			if (opt_enable_grid)
			{
				const float GRID_STEP = 64.0f;
				for (float x = fmodf(scrolling.x, GRID_STEP); x < canvas_sz.x; x += GRID_STEP)
					draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), IM_COL32(200, 200, 200, 40));
				for (float y = fmodf(scrolling.y, GRID_STEP); y < canvas_sz.y; y += GRID_STEP)
					draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), IM_COL32(200, 200, 200, 40));
			}
			for (int n = 0; n < points.Size; n += 2)
				draw_list->AddLine(ImVec2(origin.x + points[n].x, origin.y + points[n].y), ImVec2(origin.x + points[n + 1].x, origin.y + points[n + 1].y), IM_COL32(255, 255, 0, 255), 2.0f);
			draw_list->PopClipRect();

			ImGui::End();
		}
	};
};