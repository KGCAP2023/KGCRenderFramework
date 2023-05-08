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
    IAudioManager* AudM;
    Sprite* image;

public:
    bool show_popup = false;
    bool isplaying = false;
    bool openFile()     // file 버튼 구현 부분
    {
        HRESULT f_SysHr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
        if (FAILED(f_SysHr))
            return FALSE;

        IFileOpenDialog* f_FileSystem;
        f_SysHr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&f_FileSystem));
        if (FAILED(f_SysHr)) {
            CoUninitialize();
            return FALSE;
        }

        f_SysHr = f_FileSystem->Show(NULL);
        if (FAILED(f_SysHr)) {
            f_FileSystem->Release();
            CoUninitialize();
            return FALSE;
        }

        IShellItem* f_Files;
        f_SysHr = f_FileSystem->GetResult(&f_Files);
        if (FAILED(f_SysHr)) {
            f_FileSystem->Release();
            CoUninitialize();
            return FALSE;
        }

        PWSTR f_Path;
        f_SysHr = f_Files->GetDisplayName(SIGDN_FILESYSPATH, &f_Path);
        if (FAILED(f_SysHr)) {
            f_Files->Release();
            f_FileSystem->Release();
            CoUninitialize();
            return FALSE;
        }

        std::wstring path(f_Path);
        std::string c(path.begin(), path.end());
        std::string FilePath = c;                                               //파일 경로 가져옴!

        const size_t slash = FilePath.find_last_of("/\\");
        std::string SelectedFile = FilePath.substr(slash + 1);                  //선택된 파일 이름 가져옴
        std::string ext = getFileExtension(FilePath);
        std::string FileName = SelectedFile.substr(0,SelectedFile.length()-4);
        std::string jFileName = SelectedFile.substr(0, SelectedFile.length() - 5);
        CoTaskMemFree(f_Path);
        f_Files->Release();
        f_FileSystem->Release();
        CoUninitialize();

        std::cout << "선택파일)" << SelectedFile << std::endl;
        
        if (ext._Equal("mp3"))                                                  //오디오 파일
        {
            ResM->LoadAudio(FileName.c_str(), FilePath.c_str());
        }

        else if (ext._Equal("obj"))                                             //모델 파일
        { 
            ResM->LoadModel(FileName, FilePath);
        }

        else if (ext._Equal("jpg") || ext._Equal("jpeg") || ext._Equal("png"))  //사진 파일 #1
        {
            ResM->LoadSprite(FileName, FilePath);
        }

        else if (ext._Equal("jpeg"))                                            //사진 파일 #2
        {
            ResM->LoadSprite(jFileName, FilePath);
        }

        else
        {
            printf("지원하지 않는 형식입니다.\n");
        }
        return TRUE;
    }

    std::string getFileExtension(std::string& path)                             //확장자 처리
    {
        size_t index = path.rfind(".");
        if (index != std::wstring::npos) {
            return path.substr(index + 1, path.length() - index);
        }
        return std::string();
    }

    ResourceManagerView(IGameObjectManager* manager, IResourceManager* res, IAudioManager* audio ,const std::string name) : ILayer(name)
    {
        this->ObjM = manager;
        this->ResM = res;
        this->AudM = audio;
        image = ResM->FindSprite("test");
        this->ResM->LoadAudio("bgm", "../Resource/Audios/bgm.mp3");   
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

            if(ImGui::Button("add")) {                                   //add 버튼
                ImGui::SetNextWindowSize(ImVec2(600, 600), ImGuiCond_FirstUseEver);

            }
            ImGui::SameLine();
            ImGui::Button("delete");                                    //delete 버튼
            {

            }
            ImGui::SameLine();
            if (ImGui::Button("file"))                                  //file 버튼
            {
                openFile();
            }
            ImGui::Separator();

            if (ImGui::CollapsingHeader("Sprite")) {
                auto map = ResM->GetSpriteMap();
                for (auto& pair : map) {
                    Sprite* sp = pair.second;
                    sp->GetName();

                    if (ImGui::Selectable(sp->GetName().c_str()))
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

                    if (ImGui::Selectable(mo->GetName().c_str()))
                        ImGui::OpenPopup(mo->GetName().c_str());

                    if (ImGui::BeginPopup(mo->GetName().c_str()))
                    {
                        ImGui::Text(u8"모델 뷰 or 모델 썸네일 삽입예정");
                        ImGui::EndPopup();
                    }
                }
            }
           
            if (ImGui::CollapsingHeader("Audio")) {
                auto map = ResM->GetAudioMap();
                for (auto& pair : map) {
                    const char* name = pair.first.c_str();

                    if (ImGui::Selectable(name))
                    {
                        ImGui::OpenPopup(name);
                    }

                    if (ImGui::BeginPopup(name))
                    {
                        if (ImGui::Button("Play")) 
                        {
                            if(isplaying)
                                this->AudM->StopAudio(name);
                            
                            this->AudM->PlayAudio(name);
                            isplaying = true;
                        }
                        if (ImGui::Button("Stop")) 
                        {
                            this->AudM->StopAudio(name);
                            isplaying = false;
                        }
                        ImGui::EndPopup();
                    }

                    if (!ImGui::IsPopupOpen(name))
                    {
                        if (isplaying)
                        {
                            this->AudM->StopAudio(name);
                            isplaying = false;
                        }
                    }
                }
            }
            ImGui::End();
        }
    }
};