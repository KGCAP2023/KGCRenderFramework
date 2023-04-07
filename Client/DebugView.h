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
        auto keyboard = InputManager::GetKeyboardState();                                                                       //키보드 상태 값
        auto mouse = InputManager::GetMouseState();                                                                             //마우스 상태 값
        ImGui::Begin(u8"Debug View", &debug, ImGuiWindowFlags_HorizontalScrollbar);

        ImGui::Text(u8"평균 프레임: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);    //프레임 값 반환 평균 / 전체
        ImGui::Text(u8"마우스 좌표: X: %d, Y: %d", mouse.x, mouse.y);                                                            //마우스 좌표 값 x,y 반환
        ImGui::Text(u8"입력 키 W: %d A: %d S: %d D: %d", keyboard.W, keyboard.A, keyboard.S, keyboard.D);                        //키보드 W,A,S,D (누른 상태 = 1),(뗀 상태 = 0) 반환 (다른 자판 추가?)

        ImGui::End();
    };
};