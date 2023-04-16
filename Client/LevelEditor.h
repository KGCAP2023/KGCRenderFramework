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
	//bool isTypedOnce = true;

	int tilemap_size[2] = {};
	int tilemap_height, tilemap_width, mouse_cnt, size;
	char tilemap_name[20] = {};
	float* mouse_x = nullptr;
	float* mouse_y = nullptr;
	float** mouse_col = nullptr;
	
	float x_unit, y_unit, width, height;

	//3월 28일 타일맵 예제 + UI 내부에 이미지파일 삽입용 리소스매니저 추가 
	LevelEditExample(IGameObjectManager* manager, IResourceManager* res, const std::string name) : ILayer(name) {
		this->ObjM = manager;
		this->ResM = res;
		image = ResM->FindSprite("test");
		width = image->GetWidth();
		height = image->GetHeight();

		//이미지 파일의 크기가 Grid 칸 기준 옆으로 9칸, 아래로 12칸
		x_unit = 1 / 9.0f;
		y_unit = 1 / 12.0f;
		mouse_cnt = 0;

		tilemap_height = tilemap_size[0];
		tilemap_width = tilemap_size[1];
		size = tilemap_height * tilemap_width;
	}
	virtual ~LevelEditExample()
	{
		//동적 배열 할당해제
		delete[] mouse_x;
		delete[] mouse_y;
	};

	virtual void Init()
	{

	};
	virtual void Update()
	{

	};

	//선택한 이미지가 입력된 배열을 화면에 출력
	void Popup() {
		if (isActiveWindow2) {	//창의 크기는 타일맵의 크기에 맞추어 자동으로 조절됨
			ImGui::Begin(u8"Chosen Grid Block", &isActiveWindow2, ImGuiWindowFlags_AlwaysAutoResize);

			//현재 선택한 타일을 Image 형식으로 출력
			ImGui::Text("Current Tile");	
			ImGui::Image((void*)image->Get(), ImVec2(125, 125),
				ImVec2((int)mouse_x[mouse_cnt - 1] * x_unit, (int)mouse_y[mouse_cnt - 1] * y_unit),
				ImVec2((int)mouse_x[mouse_cnt - 1] * x_unit + x_unit, (int)mouse_y[mouse_cnt - 1] * y_unit + y_unit));
		
			//전체 타일맵을 ImageButton 형식으로 출력
			ImGui::Text("Tilemap");
			ImGui::Text("");
			for (int i = 0, k = 0; i < tilemap_height; i++) {
				for (int j = 0; j < tilemap_width; j++, k++) {
					ImGui::SameLine();
					ImGui::ImageButton((void*)image->Get(), ImVec2(50, 50),
						ImVec2((int)mouse_x[k] * x_unit, (int)mouse_y[k] * y_unit),
						ImVec2((int)mouse_x[k] * x_unit + x_unit, (int)mouse_y[k] * y_unit + y_unit));
				}
				ImGui::Text("");	//줄바꿈을 위해 빈 텍스트 출력
			}
			if (ImGui::Button("Save"))
			{
				TileMap* map = new TileMap();

				ImGui::InputText("Tilemap Name", tilemap_name, 20);
				map->Init(tilemap_name, image, tilemap_width, tilemap_height, width * height);

				for (int i = 0, k = 0; i < tilemap_height; i++) {
					for (int j = 0; j < tilemap_width; j++, k++) {
						map->SelectTile(0, 0, i, j);
					}
				}

				this->ResM->RegisterTileMap("name", map);
			}
			ImGui::End();

			/*if (!isActiveWindow2)
				isTypedOnce = true;*/
		}
	}

	virtual void Render()
	{
		static ImVec2 scrolling(0.0f, 0.0f);

		auto keyboard = InputManager::GetKeyboardState();
		auto mouse = InputManager::GetMouseState();

		Popup();
		if (_isActive) {
			ImGui::Begin(u8"Level Editor", &_isActive, ImGuiWindowFlags_HorizontalScrollbar);
			ImGui::Checkbox("Grid", &opt_enable_grid);	//캔버스 내부의 grid 표시여부
			
			ImGui::Text("Tile Map Size");
			ImGui::SameLine();
			ImGui::PushItemWidth(100);
			
			//최초 입력 이후 숫자를 바꿀 때 동적 배열 할당에 문제가 발생
			//한번 입력한 뒤엔 "창을 끌때까지" 더이상 입력할 수 없게 막아야함.
			ImGui::InputInt2(" ", tilemap_size);

			//if (ImGui::InputInt2(" ", tilemap_size) && isTypedOnce) {
			tilemap_height = tilemap_size[0];
			tilemap_width = tilemap_size[1];
			//}

			if (ImGui::Button("Open Tilemap")) {
				//new[]로 동적 배열 할당 및 초기화
				mouse_x = new float[tilemap_height * tilemap_width] {};
				mouse_y = new float[tilemap_height * tilemap_width] {};
				
				if (tilemap_height != 0 && tilemap_width != 0){
					//isTypedOnce 사용시 타일맵은 가장 처음 입력받은 height, width 이후로 변경이 불가능함
					//isTypedOnce = false;
					isActiveWindow2 = true;
				}
			}
			ImGui::PopItemWidth();
			
			ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      
			ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   
			if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
			if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
			ImVec2 canvas_p1 = ImVec2(canvas_p0.x + width, canvas_p0.y + height);	
			//캔버스 크기를 창 크기와 동기화하지 않고 이미지 사이즈로 고정

			ImGuiIO& io = ImGui::GetIO();
			ImDrawList* draw_list = ImGui::GetWindowDrawList();
			draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));	
			draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

			//마우스가 캔버스 안에 들어와있을 때의 동작을 감지하는 부분
			ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
			const bool is_hovered = ImGui::IsItemHovered(); 
			const bool is_active = ImGui::IsItemActive();   
			const ImVec2 origin(canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y);				
			const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);	//캔버스 내부의 마우스 좌표

			//캔버스 draw_list 창 내부에 이미지 삽입, 배경색과 배경테두리보다 나중에 호출되어 상단에 그려짐 
			//grid보다 먼저 호출되어 이미지 위에 grid가 그려질 수 있게함
			draw_list->AddImage((void*)image->Get(), origin, 
				ImVec2(origin.x + width, origin.y + height), 
				ImVec2(0, 0), ImVec2(1, 1));

			//마우스가 캔버스 내부에서 좌클릭된 경우 + 타일맵 창이 켜졌을 경우
			if (is_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left) &&isActiveWindow2) {
				points.push_back(mouse_pos_in_canvas);
				points.push_back(mouse_pos_in_canvas);

				//동적 배열의 크기를 초과하지 않는 범위에서 마우스 좌표를 저장
				if(mouse_cnt <= tilemap_width * tilemap_height)	{
					//현재 마우스 좌표를 Grid 한 칸 크기인 32픽셀로 나누어 표현
					mouse_x[mouse_cnt] = mouse_pos_in_canvas.x / 32;
					mouse_y[mouse_cnt] = mouse_pos_in_canvas.y / 32;

					mouse_cnt++;
				}
			}

			//캔버스 내부에 Grid를 표시하는 부분, 체크박스 Grid가 해제된 경우 캔버스 내부에 grid를 그리지 않음
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