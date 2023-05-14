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
    bool delete_condition = false;
    
    // �� ���� �ȿ��� ���� ǥ��
    int spriteSelect = 0;
    int modelSelect = 0;
    int tileSelect = 0;
    int audioSelect = 0;

    //  check�� ���� ������ ���� ���ϱ�

    int sprite_check = false;
    int audio_check = true;
    std::vector<Sprite*> spriteList;
    std::vector <std::string> audioList;

    bool openFile()     // add ��ư (���� Ž���� ȣ��)
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
        std::string FilePath = c;                                                   //���� ��� ������

        const size_t slash = FilePath.find_last_of("/\\");
        std::string SelectedFile = FilePath.substr(slash + 1);                      //���õ� �����̸� ������
        std::string ext = getFileExtension(FilePath);
        std::string FileName = SelectedFile.substr(0,SelectedFile.length()-4);      //mp3,obj,jpg,png �̸�ó��
        std::string jFileName = SelectedFile.substr(0, SelectedFile.length() - 5);  //jpeg �̸�ó��

        CoTaskMemFree(f_Path);
        f_Files->Release();
        f_FileSystem->Release();
        CoUninitialize();
        
        if (ext._Equal("mp3"))                                                      //����� ����
        {
            ResM->LoadAudio(FileName.c_str(), FilePath.c_str());
        }

        else if (ext._Equal("obj"))                                                 //�� ����
        { 
            ResM->LoadModel(FileName, FilePath);
        }

        else if (ext._Equal("jpg") || ext._Equal("jpeg") || ext._Equal("png"))      //���� ���� #1
        {
            ResM->LoadSprite(FileName, FilePath);
        }

        else if (ext._Equal("jpeg"))                                                //���� ���� #2
        {
            ResM->LoadSprite(jFileName, FilePath);
        }

        else
        {
            printf("�������� �ʴ� �����Դϴ�.\n");
        }
        return TRUE;
    }

    std::string getFileExtension(std::string& path)                                 //Ȯ���� ó��
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
        if (_isActive)
        {
            ImGui::Begin(u8"Resource Manager View", &_isActive, ImGuiWindowFlags_HorizontalScrollbar);

            if (ImGui::Button("add"))
            {                                                           //add ��ư
                openFile();
            }
            ImGui::SameLine();
           
            ImGui::Separator();

            if (ImGui::CollapsingHeader("Sprite"))                              //���� ���
            {
                auto map = ResM->GetSpriteMap();
                spriteList.clear();
                for (auto& pair : map)
                {
                    Sprite* sp = pair.second;
                    spriteList.push_back(sp);

                }

                for (int i = 0; i < spriteList.size(); i++)
                {
                    std::string spriteName = spriteList.at(i)->GetName();

                    if (ImGui::Selectable(spriteName.c_str(), spriteSelect == i))
                    {
                        ImGui::OpenPopup(spriteName.c_str());

                        spriteSelect = i;

                    }

                    if (ImGui::BeginPopup(spriteName.c_str()))
                    {
                        ImGui::Image((void*)spriteList.at(i)->Get(), ImVec2(128, 128));
                        ImGui::EndPopup();
                    }
                }
            }

            if (ImGui::CollapsingHeader("Object"))                              //�� ���
            {
                auto map = ResM->GetModelMap();
                for (auto& pair : map)
                {
                    Model* mo = pair.second;
                    mo->GetName();

                    if (ImGui::Selectable(mo->GetName().c_str()))
                    {
                        ImGui::OpenPopup(mo->GetName().c_str());
                    }

                    if (ImGui::BeginPopup(mo->GetName().c_str()))
                    {
                        ImGui::Text(u8"�� �� or �� ����� ���Կ���");
                        ImGui::EndPopup();
                    }
                }
            }

            if (ImGui::CollapsingHeader("Audio"))                               //����� ���
            {
                auto map = ResM->GetAudioMap();
                audioList.clear();
                for (auto& pair : map)
                {
                    std::string name = pair.first;
                    audioList.push_back(name);
                }
                for (int i = 0;  i < audioList.size(); i++)
                {

                    std::string audioName = audioList.at(i);

                    if (ImGui::Selectable(audioName.c_str(), audioSelect == i))
                    {
                        ImGui::OpenPopup(audioName.c_str());
                        audio_check == true;
                        audioSelect = i;

                    }

                    if (ImGui::BeginPopup(audioName.c_str()))
                    {
                        if (ImGui::Button("Play"))
                        {
                            if (isplaying)
                                this->AudM->StopAudio(audioName.c_str());

                            this->AudM->PlayAudio(audioName.c_str());
                            isplaying = true;
                        }
                        if (ImGui::Button("Stop"))
                        {
                            this->AudM->StopAudio(audioName.c_str());
                            isplaying = false;
                        }
                        ImGui::EndPopup();
                    }



                    if (!ImGui::IsPopupOpen(audioName.c_str()))
                    {
                        if (isplaying)
                        {
                            this->AudM->StopAudio(audioName.c_str());
                            isplaying = false;
                        }
                    }
                }

            }
            ImGui::End();
        }
    }
};