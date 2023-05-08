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
#include <Framework/LuaManager.h>
struct ExampleAppLog                                                                            //Log View 출력 부분(DEMO 버전)
{
    ImGuiTextBuffer     Buf;
    ImGuiTextFilter     Filter;
    ImVector<int>       LineOffsets; 
    bool                AutoScroll; 

    ExampleAppLog()
    {
        AutoScroll = true;
        Clear();
    }

    void    Clear()
    {
        Buf.clear();
        LineOffsets.clear();
        LineOffsets.push_back(0);
    }

    void    AddLog(const char* fmt, ...) IM_FMTARGS(2)
    {
        int old_size = Buf.size();
        va_list args;
        va_start(args, fmt);
        Buf.appendfv(fmt, args);
        va_end(args);
        for (int new_size = Buf.size(); old_size < new_size; old_size++)
            if (Buf[old_size] == '\n')
                LineOffsets.push_back(old_size + 1);
    }

    void    Draw(const char* title, bool* p_open = NULL)
    {
        if (!ImGui::Begin(title, p_open))
        {
            ImGui::End();
            return;
        }

        // Options menu
        if (ImGui::BeginPopup("Options"))
        {
            ImGui::Checkbox("Auto-scroll", &AutoScroll);
            ImGui::EndPopup();
        }

        // Main window
        if (ImGui::Button("Options"))
            ImGui::OpenPopup("Options");
        ImGui::SameLine();
        bool clear = ImGui::Button("Clear");
        ImGui::SameLine();
        bool copy = ImGui::Button("Copy");
        ImGui::SameLine();
        Filter.Draw("Filter", -100.0f);

        ImGui::Separator();

        if (ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar))
        {
            if (clear)
                Clear();
            if (copy)
                ImGui::LogToClipboard();

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
            const char* buf = Buf.begin();
            const char* buf_end = Buf.end();
            if (Filter.IsActive())
            {
                for (int line_no = 0; line_no < LineOffsets.Size; line_no++)
                {
                    const char* line_start = buf + LineOffsets[line_no];
                    const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
                    if (Filter.PassFilter(line_start, line_end))
                        ImGui::TextUnformatted(line_start, line_end);
                }
            }
            else
            {
                ImGuiListClipper clipper;
                clipper.Begin(LineOffsets.Size);
                while (clipper.Step())
                {
                    for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
                    {
                        const char* line_start = buf + LineOffsets[line_no];
                        const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
                        ImGui::TextUnformatted(line_start, line_end);
                    }
                }
                clipper.End();
            }
            ImGui::PopStyleVar();

            if (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                ImGui::SetScrollHereY(1.0f);
        }
        ImGui::EndChild();
        ImGui::End();
    }
};

class DebugView : public ILayer
{
public:
    IGameObjectManager* ObjM;

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
        auto keyboard = InputManager::GetKeyboardState();																		//키보드 상태 값
        auto mouse = InputManager::GetMouseState();																				//마우스 상태 값
        IFramework* rw = IFrameworkFactory::createFramework();

        ImGui::SetNextWindowSize(ImVec2(500, 600));

        if (_isActive) 
        {
            ImGui::Begin(u8"Debug View", &_isActive);

            ImGui::Text(u8"평균 프레임: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);	//프레임 값 반환 평균 / 전체
            ImGui::Text(u8"마우스 좌표: X: %d, Y: %d", mouse.x, mouse.y);															//마우스 좌표 값 x,y 반환
            ImGui::Text(u8"입력 키 W: %d A: %d S: %d D: %d", keyboard.W, keyboard.A, keyboard.S, keyboard.D);						//키보드 W,A,S,D (누른 상태 = 1),(뗀 상태 = 0) 반환

            static ExampleAppLog my_log;                                                                                            //Log View 창 출력 Start
            my_log.Draw("Log View");                                                                                                

            auto& a = rw->GetLuaManager()->GetLogBuffer();                                                                          //LuaCheck로 Error를 체크, Errormsg 출력

            for (std::string str : a)                                                                                               //반복문을 통한 메시지 출력
            {
                my_log.AddLog("%s\n", a);
            }
            a.clear();

            ImGui::End();
        }
	}
};