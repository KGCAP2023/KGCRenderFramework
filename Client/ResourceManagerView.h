#pragma once
#include "pch.h"
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <Framework/IFrameworkFactory.h>
#include <Framework/Framework.h>
#include <Framework/GameObjectManager.h>
#include <Framework/ResourceManager.h>
#include <Framework/ILayer.h>
#include <Framework/TileMapRenderer.h>
#include <Framework/TileMap.h>
class ResourceManagerView : public ILayer
{
    IGameObjectManager* ObjM;
    IResourceManager* ResM;
    Sprite* image;
    bool RMV = true;

public:
    ResourceManagerView(IGameObjectManager* manager, IResourceManager* res, const std::string name) : ILayer(name)
    {
        this->ObjM = manager;
        this->ResM = res;
        image = ResM->FindSprite("test");
    }

    virtual ~ResourceManagerView()
    {

    };

    void Init()
    {

    }

    void update()
    {

    }

    void Render()
    {
        auto keyboard = InputManager::GetKeyboardState();
        auto mouse = InputManager::GetMouseState();
        auto map = ResM->GetSpriteMap();

        ImGui::Begin(u8"Resource Manager View", &RMV, ImGuiWindowFlags_HorizontalScrollbar);
        ImGui::Button("add");                                   //add ��ư ������ �ϴ� �߽��ϴ�
        ImGui::SameLine();
        ImGui::Button("delete");                                //delete ��ư ������ �ϴ� �߽��ϴ�

        for (auto& pair : map) {
            Sprite* sp = pair.second;
            sp->GetName();
            ImGui::Image((void*)sp->Get(), ImVec2(32, 32));     //�̰� ũ��� �ϴ� 32*32�� �����߽��ϴ�
        }
        ImGui::End();
    }
};