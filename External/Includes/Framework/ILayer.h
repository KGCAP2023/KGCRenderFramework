#pragma once
#include "pch.h"

class ILayer
{
public:
	ILayer(const std::string& name) : _name(name), _isActive(true) {}
	virtual ~ILayer();
	virtual void Init();
	virtual void Update();
	virtual void Render();
protected:
	const std::string& GetName() { return _name; }
	void SetName(const std::string& name) { _name = name; }
	bool IsActive() { return _isActive; }
	bool Activate() { _isActive = true; }
	void Deactivate() { _isActive = false; }
private:
	bool _isActive;
	std::string _name;
};
