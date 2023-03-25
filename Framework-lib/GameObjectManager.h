#pragma once
#include "pch.h"
#include "GameObject.h"

class IGameObjectManager {

public:
	
	/// <summary>
	/// ������Ʈ ����Ʈ�� �� ������Ʈ�� �����Ͽ� �߰�
	/// </summary>
	/// <param name="_name">�߰��� ������Ʈ �̸�</param>
	/// <returns>������ ������Ʈ</returns>
	virtual GameObject* CreateGameObject(const std::string& _name) { return nullptr; };

	/// <summary>
	/// ������Ʈ ��Ͽ� Ư�� ������Ʈ�� ���F�ϴ��� �˻�
	/// </summary>
	/// <param name="_name">�˻��� ������Ʈ �̸�</param>
	/// <returns>��� ������Ʈ</returns>
	virtual GameObject* FindGameObject(const std::string& _name) { return nullptr; };

	/// <summary>
	/// ������Ʈ ��Ͽ��� Ư�� ������Ʈ ����
	/// </summary>
	/// <param name="_name">������ ������Ʈ �̸�</param>
	virtual void DestroyGameObject(const std::string& _name) {};

	/// <summary>
	/// ���� ����Ʈ �� ������Ʈ���� �̸����� ������
	/// </summary>
	/// <returns></returns>
	virtual std::string GetObjectNames() { return 0; };
};

class GameObjectManager : public IGameObjectManager{

public:

	virtual GameObject* CreateGameObject(const std::string& _name) override;

	virtual GameObject* FindGameObject(const std::string& _name) override;

	virtual void DestroyGameObject(const std::string& _name) override;
	
	virtual std::string GetObjectNames() override;

	std::unordered_map<std::string, GameObject*> gameObjects;

};

