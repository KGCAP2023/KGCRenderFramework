#pragma once
#include "pch.h"
#include "ILayer.h"

class LayerManager
{
public:
    void Init() {};
    void RegisterLayer(const std::string& key, ILayer* layer);
    void DeleteLayer(const std::string& key);
    ILayer* FindLayer(const std::string& key);

    void Update();
    void Render();
    void SetImGuiDemo(bool value);
    void DockingSpace();
private:
    bool _isActiveDemo;

    std::unordered_map<std::string, ILayer*> _layerMap;
};