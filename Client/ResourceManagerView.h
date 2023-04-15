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
    IFramework* framework;
    IGameObjectManager* ObjM;
    IResourceManager* ResM;
    Sprite* image;


public:
    bool show_popup = false;
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
        
        if (_isActive) {
            ImGui::Begin(u8"Resource Manager View", &_isActive, ImGuiWindowFlags_HorizontalScrollbar);

            ImGui::Button("add");                                   //add 버튼
            ImGui::SameLine();
            ImGui::Button("delete");                                //delete 버튼
            ImGui::SameLine();
            if (ImGui::Button("file"))
            {
                openFile();
            }
            ImGui::Separator();

            if (ImGui::CollapsingHeader("Sprite")) {
                auto map = ResM->GetSpriteMap();
                for (auto& pair : map) {
                    Sprite* sp = pair.second;
                    sp->GetName();

                    if (ImGui::Selectable(sp->GetName().c_str(), &show_popup))
                        ImGui::OpenPopup(sp->GetName().c_str());

                    if (ImGui::BeginPopup(sp->GetName().c_str()))
                    {
                        ImGui::Image((void*)sp->Get(), ImVec2(128, 128));
                        ImGui::EndPopup();
                    }
                }
            }
            if (ImGui::CollapsingHeader("Object")) {
                auto map = ResM->GetModelMap();
                for (auto& pair : map) {
                    Model* mo = pair.second;
                    mo->GetName();

                    if (ImGui::Selectable(mo->GetName().c_str(), &show_popup))
                        ImGui::OpenPopup(mo->GetName().c_str());

                    if (ImGui::BeginPopup(mo->GetName().c_str()))
                    {
                        ImGui::Text(u8"모델 뷰 or 모델 썸네일 삽입예정");
                        ImGui::EndPopup();
                    }
                }
            }
            

            /* 오디오 구현시 구현 예정
            if (ImGui::CollapsingHeader("Audio")) {
                auto map = this->AudM
                for (auto& pair : map) {
                    const char* name = pair.first;

                    if (ImGui::Selectable(name, &show_popup))
                        ImGui::OpenPopup(name);

                    if (ImGui::BeginPopup(name))
                    {
                        if (ImGui::Button("Play")) 
                        {
                            this->AudM->PlayAudio(name);
                        }
                        if (ImGui::Button("Stop")) 
                        {
                            this->AudM->StopAudio(name);
                        }

                        ImGui::EndPopup();
                        this->AudM->StopAudio(name);
                    }
                }
            }*/
            ImGui::End();
        }
    }

    bool openFile()     // file 버튼 구현 부분
    {
        //  CREATE FILE OBJECT INSTANCE
        HRESULT f_SysHr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
        if (FAILED(f_SysHr))
            return FALSE;

        // CREATE FileOpenDialog OBJECT
        IFileOpenDialog* f_FileSystem;
        f_SysHr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&f_FileSystem));
        if (FAILED(f_SysHr)) {
            CoUninitialize();
            return FALSE;
        }

        //  SHOW OPEN FILE DIALOG WINDOW
        f_SysHr = f_FileSystem->Show(NULL);
        if (FAILED(f_SysHr)) {
            f_FileSystem->Release();
            CoUninitialize();
            return FALSE;
        }

        //  RETRIEVE FILE NAME FROM THE SELECTED ITEM
        IShellItem* f_Files;
        f_SysHr = f_FileSystem->GetResult(&f_Files);
        if (FAILED(f_SysHr)) {
            f_FileSystem->Release();
            CoUninitialize();
            return FALSE;
        }

        //  STORE AND CONVERT THE FILE NAME
        PWSTR f_Path;
        f_SysHr = f_Files->GetDisplayName(SIGDN_FILESYSPATH, &f_Path);
        if (FAILED(f_SysHr)) {
            f_Files->Release();
            f_FileSystem->Release();
            CoUninitialize();
            return FALSE;
        }

        //  FORMAT AND STORE THE FILE PATH
        std::wstring path(f_Path);
        std::string c(path.begin(), path.end());
        std::string FilePath = c;                               //파일 경로 가져옴!

        //  FORMAT STRING FOR EXECUTABLE NAME
        const size_t slash = sFilePath.find_last_of("/\\");
        std::string SelectedFile = sFilePath.substr(slash + 1); //선택된 파일 이름 가져옴

        //  SUCCESS, CLEAN UP
        CoTaskMemFree(f_Path);
        f_Files->Release();
        f_FileSystem->Release();
        CoUninitialize();

        return TRUE;
    }

    std::string sFilePath;                                      //추가하는 파일의 이름을 string 형태로 저장

};