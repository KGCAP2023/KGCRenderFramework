// KGCRenderFramework.cpp : 애플리케이션에 대한 진입점을 정의합니다.
#include "pch.h"
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <Framework/IFrameworkFactory.h>
#include <Framework/Framework.h>
#include <string>
#pragma comment(lib, "Framework-lib.lib")

class LevelEditExample : public ILayer 
{
public:
	IGameObjectManager* _manager;
	ImVector<ImVec2> points;
	bool opt_enable_grid = true;
	bool opt_enable_context_menu = true;
	bool adding_line = false;
	bool level_edit = true;

	//생성자 부분, 추후 변경할 예정
	LevelEditExample(IGameObjectManager* manager, const std::string name) : ILayer(name) {
		this->_manager = manager;
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

		if (level_edit) {
			ImGui::SetNextWindowSize(ImVec2(350, 500));
			ImGui::Begin(u8"Level Editor", &level_edit, ImGuiWindowFlags_MenuBar);	//UI창을 내부 메뉴 선택과 창닫기가 가능하도록 MenuBar로 설정
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

class Example : public ILayer
{
public:
	IGameObjectManager* _manager;

	bool my_tool_active = true;
	bool active = false;

	std::vector<GameObject*> gamelist;

	Example(IGameObjectManager* manager, const std::string name) : ILayer(name)
	{
		this->_manager = manager;
		GameObject* obj = _manager->CreateGameObject("Object1");
		GameObject* obj1 = _manager->CreateGameObject("Object2");
		GameObject* obj2 = _manager->CreateGameObject("Object3");
		GameObject* obj3 = _manager->CreateGameObject("Object4");
		GameObject* obj4 = _manager->CreateGameObject("Object5");
		GameObject* obj5 = _manager->CreateGameObject("Object6");
		GameObject* obj6 = _manager->CreateGameObject("Object7");
		GameObject* obj7 = _manager->CreateGameObject("Object8");
		GameObject* obj8 = _manager->CreateGameObject("Object9");
		GameObject* obj9 = _manager->CreateGameObject("Object10");
		GameObject* obj10 = _manager->CreateGameObject("Object11");
		GameObject* obj11 = _manager->CreateGameObject("Object12");
		GameObject* obj12 = _manager->CreateGameObject("Object13");

		obj->transform.SetPosition(30, 20, 30);
		obj1->transform.SetPosition(10, 30, 15);
		obj2->transform.SetPosition(123, 20, 30);
		obj3->transform.SetPosition(13, 20, 30);
		obj4->transform.SetPosition(65, 20, 30);
		obj5->transform.SetPosition(23, 20, 30);
		obj6->transform.SetPosition(63, 20, 30);
		obj7->transform.SetPosition(71, 20, 30);
		obj8->transform.SetPosition(32, 20, 30);
		obj9->transform.SetPosition(10, 20, 30);
		obj10->transform.SetPosition(10, 20, 30);
		obj11->transform.SetPosition(10, 20, 30);
		obj12->transform.SetPosition(10, 20, 30);


		obj->transform.SetRotation(30, 20, 30);
		obj1->transform.SetRotation(10, 30, 15);
		obj2->transform.SetRotation(123, 20, 30);
		obj3->transform.SetRotation(13, 20, 30);
		obj4->transform.SetRotation(65, 20, 30);
		obj5->transform.SetRotation(23, 20, 30);
		obj6->transform.SetRotation(63, 20, 30);
		obj7->transform.SetRotation(71, 20, 30);
		obj8->transform.SetRotation(32, 20, 30);
		obj9->transform.SetRotation(10, 20, 30);
		obj10->transform.SetRotation(10, 20, 30);
		obj11->transform.SetRotation(10, 20, 30);
		obj12->transform.SetRotation(10, 20, 30);


		obj->transform.SetScale(30, 20, 30);
		obj1->transform.SetScale(10, 30, 15);
		obj2->transform.SetScale(123, 20, 30);
		obj3->transform.SetScale(13, 20, 30);
		obj4->transform.SetScale(65, 20, 30);
		obj5->transform.SetScale(23, 20, 30);
		obj6->transform.SetScale(63, 20, 30);
		obj7->transform.SetScale(71, 20, 30);
		obj8->transform.SetScale(32, 20, 30);
		obj9->transform.SetScale(10, 20, 30);
		obj10->transform.SetScale(10, 20, 30);
		obj11->transform.SetScale(10, 20, 30);
		obj12->transform.SetScale(10, 20, 30);

		gamelist.push_back(obj);
		gamelist.push_back(obj1);
		gamelist.push_back(obj2);
		gamelist.push_back(obj3);
		gamelist.push_back(obj4);
		gamelist.push_back(obj5);
		gamelist.push_back(obj6);
		gamelist.push_back(obj7);
		gamelist.push_back(obj8);
		gamelist.push_back(obj9);
		gamelist.push_back(obj10);
		gamelist.push_back(obj11);
		gamelist.push_back(obj12);
	}
	virtual ~Example() {};




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


		if (my_tool_active) {
			ImGui::Begin(u8"Hierarchy View", &my_tool_active, ImGuiWindowFlags_MenuBar);

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::Button("add"))
				{

					if (active)
					{
						ImGui::SetNextWindowSize(ImVec2(520, 600));
						if (!ImGui::BeginPopupContextWindow("AddObject", ImGuiWindowFlags_Popup))
						{
							std::cout << active << std::endl;

							if (ImGui::MenuItem("Close")) { active = false; }
							ImGui::EndPopup();
						}
						else
							ImGui::Text("z");
						ImGui::EndPopup();
					}



				}
				if (ImGui::Button("delete"))
				{

					active = true;
					if (active)
					{
						ImGui::BeginChild("add");

						if (ImGui::MenuItem("Close")) { active = false; }
						ImGui::EndChild();
					}





				}

				ImGui::EndMenuBar();
			}
			static int selected = 0;

			ImGui::BeginChild("Scrolling", ImVec2(150, 0));
			for (int i = 0; i < gamelist.size(); ++i) {
				if (ImGui::Selectable(const_cast<char*>(gamelist.at(i)->ObjectName.c_str()), selected == i))
					selected = i;
			}

			ImGui::EndChild();
			ImGui::SameLine();

			ImGui::BeginGroup();

			ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
			ImGui::Text(const_cast<char*>(gamelist.at(selected)->ObjectName.c_str()));
			ImGui::Separator();
			ImGui::Text("TRANSFORM");
			ImGui::Text(u8"pos : X:%.3f\tY:%.3f\tZ:%.3f", gamelist.at(selected)->transform.position.x, gamelist.at(selected)->transform.position.y, gamelist.at(selected)->transform.position.z);
			ImGui::Text(u8"rotation :X:%.3f\tY:%.3f\tZ:%.3f", gamelist.at(selected)->transform.rotation.x, gamelist.at(selected)->transform.rotation.y, gamelist.at(selected)->transform.rotation.z);
			ImGui::Text(u8"scale :X:%.3f\tY:%.3f\tZ:%.3f", gamelist.at(selected)->transform.scale.x, gamelist.at(selected)->transform.scale.y, gamelist.at(selected)->transform.scale.z);

			ImGui::EndChild();
			ImGui::SameLine();
			ImGui::EndGroup();
			ImGui::SameLine();

			ImGui::End();
		}
		ImGui::SetNextWindowSize(ImVec2(520, 600));
		ImGui::Begin(u8"Debug View");
		ImGui::Text(u8"평균 프레임: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text(u8"마우스 좌표: X: %d, Y: %d", mouse.x, mouse.y);
		ImGui::Text(u8"입력 키 W: %d A: %d S: %d D: %d", keyboard.W, keyboard.A, keyboard.S, keyboard.D);
		ImGui::End();
	};
};

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	IFramework* rw = IFrameworkFactory::createFramework();

	rw->Initialize(hInstance, "KYONGGI CAPSTONE", "Framework", 1600, 900);

	IGameObjectManager* manager = rw->GetGameObjectManager();

	Example* temp = new Example(manager, "example");
	rw->RegisterLayer("example", temp);

	LevelEditExample* temp2 = new LevelEditExample(manager, "example2");
	rw->RegisterLayer("example2", temp2);

	rw->run();
	return 0;
}