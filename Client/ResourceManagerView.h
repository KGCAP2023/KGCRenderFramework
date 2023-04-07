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
        
        ImGui::Button("add");                                   //addÏ°∞Í±¥Î¨?Í∞êÏã∏Í∏?
        ImGui::SameLine();
        ImGui::Button("delete");                                //deleteÏ°∞Í±¥Î¨?Í∞êÏã∏Í∏?
        ImGui::SameLine();
        if (ImGui::Button("file"))
        {
            openFile();
        }

        for (auto& pair : map) {
            Sprite* sp = pair.second;
            sp->GetName();
            ImGui::Image((void*)sp->Get(), ImVec2(128, 128));
            ImGui::Text(u8"%s", sp->GetName().c_str());
        }
        ImGui::End();
    }

    bool openFile()     // file Î≤ÑÌäº Íµ¨ÌòÑ
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
        std::string FilePath = c;                               //?åÏùº Í≤ΩÎ°ú Í∞Ä?∏Ïò¥!

        //  FORMAT STRING FOR EXECUTABLE NAME
        const size_t slash = sFilePath.find_last_of("/\\");
        std::string SelectedFile = sFilePath.substr(slash + 1); //?†ÌÉù???åÏùº?¥Î¶Ñ Í∞Ä?∏Ïò¥!

        //  SUCCESS, CLEAN UP
        CoTaskMemFree(f_Path);
        f_Files->Release();
        f_FileSystem->Release();
        CoUninitialize();

        return TRUE;
    }

    std::string sFilePath;                                      //?†ÌÉù???åÏùº??Í≤ΩÎ°úÎ•?String?ïÌÉúÎ°??Ä??

};