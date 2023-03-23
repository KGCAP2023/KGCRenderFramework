#include "pch.h"
#include "LayerManager.h"
#include "InputManager.h"
#include "Framework.h"
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

void LayerManager::Init(Framework* framework)
{
	this->framework = framework;
}

void LayerManager::RegisterLayer(const std::string& key, ILayer* layer)
{
	this->_layerMap.insert(std::make_pair<>(key,layer));
}

void LayerManager::DeleteLayer(const std::string& key)
{
	this->_layerMap.erase(key);
}

ILayer* LayerManager::FindLayer(const std::string& key)
{
	if (this->_layerMap.find(key) != this->_layerMap.end()) {
		return this->_layerMap[key];
	}
	return nullptr;
}

void LayerManager::Update()
{
	for (auto& layer : this->_layerMap)
	{
		layer.second->Update();
	}
}

void LayerManager::Render()
{
	if (_isActiveDockingSpace)
		this->DockingSpace();

	if(_isActiveDemo)
		ImGui::ShowDemoWindow();

	for (auto& layer : this->_layerMap)
	{
		layer.second->Render();
	}
}

void LayerManager::SetImGuiDemo(bool value)
{
	this->_isActiveDemo = value;
}

void LayerManager::SetDockingSpace(bool value)
{
	this->_isActiveDockingSpace = value;
}

void LayerManager::DockingSpace()
{
	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	static bool is_open = true;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	if (!opt_padding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &is_open, window_flags);
	if (!opt_padding)		ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// Submit the DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}
	ImGui::End();

	//===========================================================================

	static bool zzz = true;
	ImGui::Begin(u8"GameView", &zzz, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysUseWindowPadding);

	ImVec2 windowSize = ImGui::GetWindowSize();
	windowSize.x -= 10;
	windowSize.y -= 32;

	ImVec2 vMin = ImGui::GetWindowContentRegionMin();  // 8/8
	ImVec2 vMax;

	vMax.x = vMin.x + windowSize.x;
	vMax.y = vMin.y + windowSize.y;

	//·ÎÄÃ ÁÂÇ¥
	vMin.x += ImGui::GetWindowPos().x;
	vMin.y += ImGui::GetWindowPos().y;
	vMax.x += ImGui::GetWindowPos().x;
	vMax.y += ImGui::GetWindowPos().y;

	float view_x = vMax.x - vMin.x;
	float view_y = vMax.y - vMin.y;

	const auto& mouse = InputManager::GetMouse()->GetState();

	ImVec2 temp;
	temp.x = mouse.x - vMin.x;
	temp.y = mouse.y - vMin.y;

	float normalX = temp.x * (this->framework->width / view_x);
	float normalY = temp.y * (this->framework->height / view_y);

	//std::cout <<"ÁÂÇ¥:  " << normalX << "/" << normalY << std::endl;

	ImGui::Image((void*)framework->graphics.refRes, windowSize);
	ImGui::End();
}
