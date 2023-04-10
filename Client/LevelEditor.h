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

		float width = image->GetWidth();
		float height = image->GetHeight();

		static float mouse_x, mouse_y;
		float Bitmap_size[2];
		
		//이미지 파일의 크기가 Grid 칸 기준 옆으로 9칸, 아래로 12칸
		float x_unit = 1 / 9.0f;
		float y_unit = 1 / 12.0f;


		if (_isActive) {
			ImGui::Begin(u8"Level Editor", &_isActive, ImGuiWindowFlags_HorizontalScrollbar);	//UI창을 내부 메뉴 선택과 창닫기가 가능하도록 MenuBar로 설정
			ImGui::Checkbox("Grid", &opt_enable_grid);								//캔버스 내부의 grid 표시여부
			
			ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList가 화면의 좌표를 가져와 사용, UI창 이동시 캔버스의 좌표도 같이 움직임
			ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   // 윈도우 창 사이즈에 맞추어 캔버스 사이즈를 조절
			if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
			if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
			ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

			ImGuiIO& io = ImGui::GetIO();
			ImDrawList* draw_list = ImGui::GetWindowDrawList();
			draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));	//캔버스 내부 색상과 테두리 설정
			draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

			//마우스가 캔버스 안에 들어와있을 때의 동작을 감지하는 부분
			ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
			const bool is_hovered = ImGui::IsItemHovered(); // 캔버스 내부에서 움직이는 상태인 경우
			const bool is_active = ImGui::IsItemActive();   // 캔버스 내부에서 클릭된 상태인 경우
			const ImVec2 origin(canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y);				// Lock scrolled origin
			const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);	//캔버스 내부의 마우스 좌표를 받아옴

			//캔버스 draw_list 창 내부에 이미지 삽입, 배경색과 배경테두리보다 나중에 호출되어 상단에 그려짐 
			//grid보다 먼저 호출되어 이미지 위에 grid가 그려질 수 있게함
			draw_list->AddImage((void*)image->Get(), origin, 
				ImVec2(origin.x + width, origin.y + height), 
				ImVec2(0, 0), ImVec2(1, 1));

			//마우스가 캔버스 내부에서 좌클릭된 경우
			if (is_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
			{
				points.push_back(mouse_pos_in_canvas);
				points.push_back(mouse_pos_in_canvas);

				//현재 마우스 좌표를 Grid 한 칸 크기인 32픽셀로 나누어 표현
				mouse_x = mouse_pos_in_canvas.x / 32;
				mouse_y = mouse_pos_in_canvas.y / 32;

				isActiveWindow2 = true;
			}

			//캔버스 내부에 Grid를 표시하는 부분, 체크박스 Grid가 해제된 경우 캔버스 내부에 grid를 그리지 않음
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

		if(isActiveWindow2) {	//Added 창에 선택한 이미지 범위 출력
			ImGui::Begin(u8"Chosen Grid Block");
			ImGui::Image((void*)image->Get(), ImVec2(125, 125),
				ImVec2((int)mouse_x * x_unit, (int)mouse_y * y_unit),
				ImVec2((int)mouse_x * x_unit + x_unit, (int)mouse_y * y_unit + y_unit));
			ImGui::End();
		}
	};
};