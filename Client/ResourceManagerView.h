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
        ImGui::Button("add");                                   //add 버튼 구현만 일단 했습니다
        ImGui::SameLine();
        ImGui::Button("delete");                                //delete 버튼 구현만 일단 했습니다

        for (auto& pair : map) {
            Sprite* sp = pair.second;
            sp->GetName();
            ImGui::Image((void*)sp->Get(), ImVec2(32, 32));     //이거 크기는 일단 32*32로 설정했습니다
        }
        ImGui::End();
    }
};