#include "pch.h"
#include "IFramework.h"
#include "Camera.h"


void IFramework::RegisterLayer(const std::string& key, ILayer* layer)
{
}

void IFramework::DeleteLayer(const std::string& key)
{
}

ILayer* IFramework::FindLayer(const std::string& key)
{
    return nullptr;
}

void IFramework::AddMenubar(std::function<void()> callback)
{
}

bool IFramework::Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height)
{
    return false;
}

void IFramework::run()
{
}

void IFramework::ChangeCameraViewType(Camera3D::ViewType viewType)
{
}

