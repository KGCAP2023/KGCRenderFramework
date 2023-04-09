#pragma once
#include "pch.h"
#include "ILayer.h"

class Framework;

class LayerManager
{
public:
    void Init(Framework* framework);
    void RegisterLayer(const std::string& key, ILayer* layer);
    void DeleteLayer(const std::string& key);
    ILayer* FindLayer(const std::string& key);

    void AddMenubar(std::function<void()> callback);

    void Update();
    void Render();
    void SetImGuiDemo(bool value);
    void SetDockingSpace(bool value);
    void DockingSpace();
    bool isGameViewFocus() { return _isGameViewFocus; }
private:

    bool _isActiveDemo = false;
    bool _isActiveDockingSpace = false;
    bool _isGameViewFocus = false;

    Framework* framework;
    std::vector<std::function<void()>> _menubar;
    std::unordered_map<std::string, ILayer*> _layerMap;
};