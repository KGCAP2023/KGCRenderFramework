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

    void Update();
    void Render();
    void SetImGuiDemo(bool value);
    void DockingSpace();
private:
    bool _isActiveDemo = false;

    Framework* framework;
    std::unordered_map<std::string, ILayer*> _layerMap;
};