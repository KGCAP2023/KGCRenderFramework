#pragma once
#include "pch.h"
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <Framework/ILayer.h>

class IGameObjectManager;
class IResourceManager;
class Sprite;

class LevelEditExample : public ILayer
{
public:
	IGameObjectManager* ObjM;
	IResourceManager* ResM;
	ImVector<ImVec2> points;
	Sprite* Map_Img;
	Sprite* Emp_Img;
	bool opt_enable_grid = true;
	bool Resizable = true;
	bool TilemapWindow = false;
	bool SavemapWindow = false;
	bool SaveAlert = false;
	bool NullName = false;
	bool ResetAlert = false;
	bool AlreadyClosedAlert = false;

	int input_size[2] = {};
	int tilemap_height, tilemap_width, tilemap_size, mouse_cnt;
	char Original_Img[20] = {};
	char tilemap_name[20] = {};
	float* mouse_x = nullptr;
	float* mouse_y = nullptr;	
	
	float x_grid_unit, y_grid_unit, width, height;

	//3월 28일 타일맵 예제 + UI 내부에 이미지파일 삽입용 리소스매니저 추가
	//맵 선택용 스프라이트 이미지는 test로, 빈자리 표현용 스프라이트 이미지는 empty로 불러옴
	LevelEditExample(IGameObjectManager* manager, IResourceManager* res, const std::string name);
	virtual ~LevelEditExample();

	virtual void Init()
	{

	};
	virtual void Update()
	{

	};

	/// <summary>
	/// 상황별 경고창 출력
	/// </summary>
	void Alert();

	/// <summary>
	/// 선택한 타일맵을 저장하고 맵 이름을 입력받는 창
	/// </summary>
	void Save();

	/// <summary>
	/// 선택한 이미지가 입력된 배열을 화면에 출력하는 창
	/// </summary> 
	void SelectTile();

	/// <summary>
	/// 맵 에디터 기본창
	/// </summary>
	virtual void Render();
};