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
#include <Framework/IFramework.h>

class DebugView : public ILayer
{
public:
    IGameObjectManager* ObjM;
    bool debug = true;

    DebugView(IGameObjectManager* manager, const std::string name) : ILayer(name)
    {
        this->ObjM = manager;
    }
    virtual ~DebugView()
    {

    };

    virtual void Init()
    {

    };

    virtual void Update()
    {

    };
    virtual void draw()
    {

    }
    virtual void Render()
    {
        auto keyboard = InputManager::GetKeyboardState();                                                                       //Ű���� ���� ��
        auto mouse = InputManager::GetMouseState();                                                                             //���콺 ���� ��
        ImGui::Begin(u8"Debug View", &debug, ImGuiWindowFlags_HorizontalScrollbar);

        ImGui::Text(u8"��� ������: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);    //������ �� ��ȯ ��� / ��ü
        ImGui::Text(u8"���콺 ��ǥ: X: %d, Y: %d", mouse.x, mouse.y);                                                            //���콺 ��ǥ �� x,y ��ȯ
        ImGui::Text(u8"�Է� Ű W: %d A: %d S: %d D: %d", keyboard.W, keyboard.A, keyboard.S, keyboard.D);                        //Ű���� W,A,S,D (���� ���� = 1),(�� ���� = 0) ��ȯ (�ٸ� ���� �߰�?)

        ImGui::End();
    };
};