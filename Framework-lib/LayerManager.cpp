#include "pch.h"
#include "LayerManager.h"

void LayerManager::RegisterLayer(const std::string& key, ILayer* layer)
{
	this->_layerMap.insert(std::make_pair<>(key,layer));
}

void LayerManager::DeleteLayer(const std::string& key)
{
	this->_layerMap.erase(key);
}

ILayer* LayerManager::FindLayer(const std::string& key)
{
	if (this->_layerMap.find(key) != this->_layerMap.end()) {
		return this->_layerMap[key];
	}
	return nullptr;
}

void LayerManager::Update()
{
	for (auto& layer : this->_layerMap)
	{
		layer.second->Update();
	}
}

void LayerManager::Render()
{
	for (auto& layer : this->_layerMap)
	{
		layer.second->Render();
	}
}

void LayerManager::SetImGuiDemo(bool value)
{
	this->_isActiveDemo = value;
}

void LayerManager::DockingSpace()
{

}
