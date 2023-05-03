#pragma once
#include "pch.h"
#include <fstream>
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <Framework/IFrameworkFactory.h>
#include <Framework/Framework.h>
#include <Framework/GameObjectManager.h>
#include <Framework/ResourceManager.h>
#include <Framework/ILayer.h>
#include <Framework/IFramework.h>



class ScriptEditor : public ILayer
{
public:
    IGameObjectManager* ObjM;

    ScriptEditor(IGameObjectManager* manager, const std::string name) : ILayer(name)
    {
        this->ObjM = manager;
    }
    virtual ~ScriptEditor()
    {

    };

    virtual void Init()
    {
        //this->Deactivate();
        
    };

    virtual void Update()
    {

    };
    virtual void draw()
    {

    }
    virtual void Render()
    {
        


        if (_isActive)
        {
           
            ImGui::Begin(u8"Script Editor", &_isActive, ImGuiWindowFlags_MenuBar);

            if (ImGui::BeginMenuBar())
            {
                if (ImGui::MenuItem("Save"))
                {
                    if (ImGui::BeginPopup("filename"))
                    {
                        char name[100] = "";
                        ImGui::InputTextWithHint("filename", "example", name, IM_ARRAYSIZE(buffer));
                        ImGui::EndPopup();

                    }
                    std::ofstream writeFile("example.txt");
                    if (writeFile.is_open()) {
                        writeFile << buffer;
                        writeFile.close();
                    }
                }
                if (ImGui::MenuItem("Load"))
                {
                    if (ImGui::BeginPopup("filename"))
                    {
                        char name[100] = "";
                        ImGui::InputTextWithHint("filename", "example", name, IM_ARRAYSIZE(buffer));
                        ImGui::EndPopup();

                    }
                    std::ifstream readFile("example.txt");
                    if (readFile.is_open()) {
                        while (!readFile.eof()) {
                            readFile.getline(buffer, 1024 * 16);
                        }
                        readFile.close();
                    }
                }

                if (ImGui::MenuItem("Clear"))
                {
                    buffer[0] = '\0';
                }
            }
            ImGui::EndMenuBar();

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

            ImGui::BeginChild("child1", ImVec2(30, 0), false, ImGuiWindowFlags_NoScrollbar);
            for (int i = 1; i < 100; i++)
                ImGui::Text("%d\n", i);
            float scroll_y = ImGui::GetScrollY();
            ImGui::SetScrollY(scroll_y);
            ImGui::EndChild();

            ImGui::SameLine();

            ImGui::BeginChild("child2", ImVec2(0, 0), false);
            ImGui::InputTextMultiline("##source", buffer, IM_ARRAYSIZE(buffer), ImVec2(-1, -1), ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_NoHorizontalScroll);
            ImGui::EndChild();

            ImGui::PopStyleVar();
            ImGui::PopStyleVar();

            ImGui::End();
        }

    }

    char buffer[1024 * 16] = "type script";
};