#pragma once
#include "pch.h"

class ILayer
{
public:
	ILayer(const std::string& name) : _name(name), _isActive(true) {}
	virtual ~ILayer();
	/// <summary>
	/// 초기화 하는 함수입니다.
	/// RegisterLayer() 함수로 등록시 호출됩니다.
	/// </summary>
	virtual void Init();
	/// <summary>
	/// 업데이트 함수입니다.
	/// </summary>
	virtual void Update();
	/// <summary>
	/// 렌더 함수입니다.
	/// </summary>
	virtual void Render();
	/// <summary>
	/// 해당 
	/// </summary>
	/// <returns></returns>
	bool IsActive() { return _isActive; }
	/// <summary>
	/// 해당창이 켜져있는지 확인합니다.
	/// </summary>
	/// <returns></returns>
	bool Activate() { _isActive = true; }
	/// <summary>
	/// 해당창을 활성화 시킵니다.
	/// </summary>
	void Deactivate() { _isActive = false; }
	/// <summary>
	/// 해당창을 비활성화 시킵니다.
	/// </summary>
	/// <returns></returns>
	const std::string& GetName() { return _name; }
protected:
	void SetName(const std::string& name) { _name = name; }
	bool _isActive;
private:
	std::string _name;
};
