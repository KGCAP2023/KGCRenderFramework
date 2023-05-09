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
    bool _Savewin = false;
    bool _Loadwin = false;


    char savename[20]="";
    char loadname[20]="";
    char buffer[1024 * 16] = "type script";

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
                    if (_Loadwin||!_Savewin)
                    {
                        _Loadwin = false;
                        _Savewin = true;
                    }
                    
                }
                if (ImGui::MenuItem("Load"))
                {
                    if (_Savewin||!_Loadwin)
                    {
                        _Savewin = false;
                        _Loadwin = true;
                    }

                }

                if (ImGui::MenuItem("Clear"))
                {
                    buffer[0] = '\0';
                }
            }
            ImGui::EndMenuBar();

            if (_Savewin)
            {
                ImGui::Begin(u8"Script Editor", &_Savewin);
                
                
                ImGui::InputTextWithHint("Save filename", "example", savename, IM_ARRAYSIZE(loadname));
                
                if (ImGui::Button("Save") && savename!="")
                {
                    strcat(savename, ".lua");
                    std::ofstream writeFile(savename);
                    if (writeFile.is_open()) {
                        writeFile << buffer;
                        writeFile.close();
                    }
                    _Savewin = false;
                    savename[0] = '\0';
                }
                ImGui::SameLine();
                if (ImGui::Button("Close"))
                {
                    _Savewin = false;
                    savename[0] = '\0';
                }
                ImGui::End();
            }

            if (_Loadwin)
            {
                ImGui::Begin(u8"Script Editor", &_Loadwin);
                
                ImGui::InputTextWithHint("Load filename", "example", loadname, IM_ARRAYSIZE(loadname));
                
                if (ImGui::Button("Load") && loadname != "")
                {
                    strcat(loadname, ".lua");
                    std::ifstream readFile(loadname);
                    if (readFile.is_open()) {
                        while (!readFile.eof()) {
                            readFile.getline(buffer, 1024 * 16);
                        }
                        readFile.close();
                    }
                    _Loadwin = false;
                    loadname[0] = '\0';
                }
                ImGui::SameLine();
                if (ImGui::Button("Close"))
                {
                    _Loadwin = false;
                    loadname[0] = '\0';
                }
                ImGui::End();
            }

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

    
};