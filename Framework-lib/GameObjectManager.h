#pragma once
#include "pch.h"
#include "GameObject.h"

class Framework;
class ResourceManager;
class GraphicManager;

class IGameObjectManager {

public:
	/// <summary>
	/// ������Ʈ ����Ʈ�� �� ������Ʈ�� �����Ͽ� �߰�
	/// </summary>
	/// <param name="_name">�߰��� ������Ʈ �̸�</param>
	/// <returns>������ ������Ʈ</returns>
	virtual GameObject* CreateGameObject(const std::string& _name) { return nullptr; };

	/// <summary>
	/// ������Ʈ ����Ʈ�� �� ������Ʈ�� �����Ͽ� �߰�
	/// </summary>
	/// <param name="_name">�߰��� ������Ʈ �̸�</param>
	/// <param name="path">�߰��� ������Ʈ ������ ���</param>
	/// <returns>������ ������Ʈ</returns>
	virtual GameObject* CreateGameObject(const std::string& _name, const std::string& path) { return nullptr; };

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
	/// <summary>
	/// GameObjectManager�� �����մϴ�.
	/// </summary>
	/// <param name="framework">Framework* �������� ���� Framework�� ���޹ݽ��ϴ�.</param>
	/// <param name="graphicManager">GraphicManager* �������� ���� GraphicManager�� ���޹ݽ��ϴ�.</param>
	/// <param name="res">ResourceManager* �������� ���� ResourceManager�� ���޹ݽ��ϴ�.</param>
	GameObjectManager(Framework* framework);
	
	
	Framework* framework; 

	ResourceManager* res;

	GraphicManager* graphicManager;
	
	virtual GameObject* CreateGameObject(const std::string& _name) override;

	virtual GameObject* CreateGameObject(const std::string& _name, const std::string& path) override;

	virtual GameObject* FindGameObject(const std::string& _name) override;

	virtual void DestroyGameObject(const std::string& _name) override;
	
	virtual std::string GetObjectNames() override;

	std::unordered_map<std::string, GameObject*> gameObjects;

	

};



