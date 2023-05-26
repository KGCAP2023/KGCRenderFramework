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

	//3�� 28�� Ÿ�ϸ� ���� + UI ���ο� �̹������� ���Կ� ���ҽ��Ŵ��� �߰�
	//�� ���ÿ� ��������Ʈ �̹����� test��, ���ڸ� ǥ���� ��������Ʈ �̹����� empty�� �ҷ���
	LevelEditExample(IGameObjectManager* manager, IResourceManager* res, const std::string name);
	virtual ~LevelEditExample();

	virtual void Init()
	{

	};
	virtual void Update()
	{

	};

	/// <summary>
	/// ��Ȳ�� ���â ���
	/// </summary>
	void Alert();

	/// <summary>
	/// ������ Ÿ�ϸ��� �����ϰ� �� �̸��� �Է¹޴� â
	/// </summary>
	void Save();

	/// <summary>
	/// ������ �̹����� �Էµ� �迭�� ȭ�鿡 ����ϴ� â
	/// </summary> 
	void SelectTile();

	/// <summary>
	/// �� ������ �⺻â
	/// </summary>
	virtual void Render();
};