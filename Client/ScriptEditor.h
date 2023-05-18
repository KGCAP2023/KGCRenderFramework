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
    bool confirm = false;
    int warning = 0;

    char savename[20]="";
    char loadname[20]="";
    char buffer[1024 * 16] = "function Start()\nend\n\nfunction Update()\nend";

    ScriptEditor(IGameObjectManager* manager, const std::string name) : ILayer(name)
    {
        this->ObjM = manager;
    }
    virtual ~ScriptEditor()
    {

    };

    virtual void Init()
    {
        this->Deactivate();
        
    };

    virtual void Update()
    {

    };
    virtual void draw()
    {

    }
    void Warning()
    {

        switch(warning)
        {
            case 1:
                ImGui::OpenPopup(u8"존재하지 않는 파일명");
                break;
            case 2:
                ImGui::OpenPopup(u8"중복되는 파일");
                break;
        }

        if (ImGui::BeginPopupModal(u8"존재하지 않는 파일명", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text(u8"파일이 존재하지 않습니다.");
            if (ImGui::Button(u8"확인") || ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)))
            {
                ImGui::CloseCurrentPopup();
                warning = 0;
            }
            ImGui::EndPopup();
        }
        if (ImGui::BeginPopupModal(u8"중복되는 파일", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text(u8"덮어 씌우시겠습니까?");
            if (ImGui::Button(u8"네"))
            {
                std::ofstream writeFile(savename);
                if (writeFile.is_open()) {
                    writeFile << buffer;
                    writeFile.close();
                }
                _Savewin = false;
                savename[0] = '\0';
                ImGui::CloseCurrentPopup();
                warning = 0;
            }
            ImGui::SameLine();
            if (ImGui::Button(u8"아니오"))
            {
                ImGui::CloseCurrentPopup();
                warning = 0;
            }
            ImGui::EndPopup();
        }
    }

    virtual void Render()
    {
        Warning();


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
                    strcpy(buffer, "function Start()\nend\n\nfunction Update()\nend");
                }
            }
            ImGui::EndMenuBar();

            if (_Savewin)
            {
                ImGui::Begin(u8"Script Editor", &_Savewin);
                
                
                ImGui::InputTextWithHint("Save filename", "example", savename, IM_ARRAYSIZE(loadname));
                
                if (ImGui::Button("Save") && savename[0] != '\0')
                {
                    strcat(savename, ".lua");
                    char path[100] = "../Lua/";
                    strcat(path, savename);
                    strcpy(savename, path);
                    if (_access(savename, 0) == 0)
                    {
                        warning = 2;
                    }
                    else
                    {
                        std::ofstream writeFile(savename);
                        if (writeFile.is_open()) {
                            writeFile << buffer;
                            writeFile.close();
                        }
                        confirm = false;
                        _Savewin = false;
                        savename[0] = '\0';
                    }
                    

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
                
                if (ImGui::Button("Load") && loadname[0] != '\0')
                {
                    strcat(loadname, ".lua");
                    char path[100] = "../Lua/";
                    strcat(path, loadname);
                    strcpy(loadname, path);
                    strcpy(path, "../Lua/");
                    if (_access(loadname, 0) != -1)
                    {
                        std::ifstream readFile(loadname);
                        readFile.read(buffer, sizeof(buffer));
                        int numCharsRead = readFile.gcount();
                        buffer[numCharsRead] = '\0';
                        _Loadwin = false;
                    }
                    else
                    {
                        warning = 1;
                    }
                    

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