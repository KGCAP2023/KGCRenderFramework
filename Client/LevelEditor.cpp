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

	//기본 설정된 Sand.PNG 이미지 파일의 크기는 Grid 칸 기준 옆으로 9칸, 아래로 12칸
	//Grid는 이미지 파일을 32비트씩으로 나눈 것
	x_grid_unit = 1 / (width / 32.0f);
	y_grid_unit = 1 / (height / 32.0f);
	mouse_cnt = 0;

	//타일맵의 크기는 사용자가 InputInt에 입력하는 숫자 2개로 결정
	tilemap_height = input_size[0];
	tilemap_width = input_size[1];
	tilemap_size = tilemap_height * tilemap_width;
}

LevelEditExample::~LevelEditExample()
{
	//동적 배열 할당해제
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
		ImGui::Checkbox("Grid", &opt_enable_grid);	//캔버스 내부의 grid 표시여부

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

		//Resizable이 비활성화되면 인풋에 입력된 숫자를 무시함
		if (ImGui::InputInt2(" ", input_size) && Resizable) {
			tilemap_height = input_size[0];
			tilemap_width = input_size[1];
			tilemap_size = tilemap_height * tilemap_width;
		}

		if (ImGui::Button("Open Tilemap")) {
			//new[]로 동적 배열 할당 및 초기화
			mouse_x = new float[tilemap_size] {};
			mouse_y = new float[tilemap_size] {};

			//창을 x버튼으로 껐을 땐 완전히 초기화 후 다시 키도록 유도
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


			//타일맵 선택 창이 켜진 이후에 배열 크기가 변경되지 못하게 막음
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
		draw_list->AddImage((void*)Map_Img->Get(), origin,
			ImVec2(origin.x + width, origin.y + height),
			ImVec2(0, 0), ImVec2(1, 1));

		//마우스가 캔버스 내부에서 좌클릭된 경우 + 타일맵 창이 켜졌을 경우
		if (is_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && TilemapWindow) {
			points.push_back(mouse_pos_in_canvas);
			points.push_back(mouse_pos_in_canvas);

			//동적 배열의 크기를 초과하지 않는 범위에서 마우스 좌표를 저장
			if (mouse_cnt < tilemap_size) {
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
}

void LevelEditExample::Save()
{
	if (SavemapWindow) {
		ImGui::Begin(u8"Save Tilemap", &SavemapWindow);
		ImGui::PushItemWidth(100);
		TileMap* map = new TileMap();
		
		//InputText를 통해 사용자로부터 이름을 길이 20의 tilemap_name 배열로 받아옴
		ImGui::InputText("Tilemap Name", tilemap_name, 20);
		std::string tilemap_string(tilemap_name);
		if (ImGui::Button("Save This Name")) {
			if (tilemap_string.compare("") != 0) {
				//Tilemap* 형식의 map에 이름, 너비, 높이, 원본 이미지를 지정
				map->Init(tilemap_name, Map_Img, tilemap_width, tilemap_height);

				//사용자가 선택한 이미지 타일들을 map에 차례대로 지정
				for (int k = 0; k < tilemap_size; k++) {
					//gridX, gridY가 k / tilemap_width와 k % tilemap_width로 입력
					int a = (int)mouse_x[k];
					int b = (int)mouse_y[k];
					int c = k / tilemap_width;
					int d = k % tilemap_width;
					map->SelectTile(a, b, d, c);
				}

				//지정된 이미지 타일들을 이름이 tilemap_name인 map으로 새롭게 등록 
				this->ResM->RegisterTileMap(tilemap_name, map);
				SaveAlert = true;

				//Save시 맵은 저장되고, 창이 꺼진 뒤 새로운 입력을 위해 기존 입력 내역을 모두 초기화
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
	if (TilemapWindow) {	//창의 크기는 타일맵의 크기에 맞추어 자동으로 조절됨
		ImGui::Begin(u8"Chosen Grid Block", &TilemapWindow, ImGuiWindowFlags_AlwaysAutoResize);

		//현재 선택한 타일을 125,125 크기의 Image 형식으로 출력
		ImGui::Text("Current Tile");
		ImGui::Image((void*)Map_Img->Get(), ImVec2(125, 125),
			ImVec2((int)mouse_x[mouse_cnt - 1] * x_grid_unit, (int)mouse_y[mouse_cnt - 1] * y_grid_unit),
			ImVec2((int)mouse_x[mouse_cnt - 1] * x_grid_unit + x_grid_unit, (int)mouse_y[mouse_cnt - 1] * y_grid_unit + y_grid_unit));

		//전체 타일맵을 Image 형식으로 출력
		ImGui::Text("Tilemap");
		ImGui::Text("");

		for (int i = 0, k = 0; i < tilemap_height; i++) {
			for (int j = 0; j < tilemap_width; j++, k++) {
				ImGui::SameLine();
				//아직 값이 지정되지 않은 부분은 흰색의 empty 이미지로 출력
				if (mouse_cnt - 1 < 0 || k > mouse_cnt - 1) {
					ImGui::Image((void*)Emp_Img->Get(), ImVec2(50, 50), ImVec2(0, 0), ImVec2(10, 10));
				}
				//사용자가 이미지를 클릭해 정규화된 좌표가 저장된 부분은 해당 좌표가 포함된 이미지를 출력
				else {
					ImGui::Image((void*)Map_Img->Get(), ImVec2(50, 50),
						ImVec2((int)mouse_x[k] * x_grid_unit, (int)mouse_y[k] * y_grid_unit),
						ImVec2((int)mouse_x[k] * x_grid_unit + x_grid_unit, (int)mouse_y[k] * y_grid_unit + y_grid_unit));
				}
			}
			ImGui::Text("");	//줄바꿈을 위해 빈 텍스트 출력
		}
		//Save 버튼 클릭 시 savemap 창이 새롭게 열림
		if (ImGui::Button("Save"))
			SavemapWindow = true;
		ImGui::SameLine();

		//배열에 가장 마지막에 입력된 좌표값을 삭제, 클릭된 횟수 또한 한번 줄임
		if (ImGui::Button("Undo")) {
			if (mouse_cnt > 0) {
				mouse_cnt--;
				mouse_x[mouse_cnt] = 0;
				mouse_y[mouse_cnt] = 0;
			}
		}
		ImGui::SameLine();

		//동적 배열에 저장된 값과 배열 크기, 카운터를 모두 초기화
		//5월 8일 초기화 시 창이 아예 닫히도록 변경
		//5월 15일 창의 x버튼으로 창을 껐을 때도 Reset 버튼을 눌렀을 때와 동일하게 창이 종료되도록 변경
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
	//저장완료 알림
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

	//이름 입력안됨 오류 알림
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

	//리셋 실행 알림
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

	//타일맵 창의 비정상적 종료 후 재오픈 시 알림
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
