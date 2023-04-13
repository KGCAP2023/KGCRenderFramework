#pragma once
#include "pch.h"

class ILayer
{
public:
	ILayer(const std::string& name) : _name(name), _isActive(true) {}
	virtual ~ILayer();
	/// <summary>
	/// �ʱ�ȭ �ϴ� �Լ��Դϴ�.
	/// RegisterLayer() �Լ��� ��Ͻ� ȣ��˴ϴ�.
	/// </summary>
	virtual void Init();
	/// <summary>
	/// ������Ʈ �Լ��Դϴ�.
	/// </summary>
	virtual void Update();
	/// <summary>
	/// ���� �Լ��Դϴ�.
	/// </summary>
	virtual void Render();
	/// <summary>
	/// �ش� 
	/// </summary>
	/// <returns></returns>
	bool IsActive() { return _isActive; }
	/// <summary>
	/// �ش�â�� �����ִ��� Ȯ���մϴ�.
	/// </summary>
	/// <returns></returns>
	bool Activate() { _isActive = true; }
	/// <summary>
	/// �ش�â�� Ȱ��ȭ ��ŵ�ϴ�.
	/// </summary>
	void Deactivate() { _isActive = false; }
	/// <summary>
	/// �ش�â�� ��Ȱ��ȭ ��ŵ�ϴ�.
	/// </summary>
	/// <returns></returns>
	const std::string& GetName() { return _name; }
protected:
	void SetName(const std::string& name) { _name = name; }
	bool _isActive;
private:
	std::string _name;
};
