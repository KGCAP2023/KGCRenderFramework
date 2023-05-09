#pragma once
#include "pch.h"
#include "GameObject.h"

class Framework;
class ResourceManager;
class GraphicManager;

static enum class SceneMode
{
	DEV = 0,
	PLAY = 1,
	EMPTY = 2
};

static std::string SceneModeMap[] = { "DEV", "PLAY","EMPTY" };

class IGameObjectManager {

public:
	/// <summary>
	/// ������Ʈ �Ŵ����� �̸��� �ҷ��ɴϴ�
	/// </summary>
	/// <returns></returns>
	virtual SceneMode GetMode() { return SceneMode::EMPTY; };

	/// <summary>
	/// ������Ʈ ����Ʈ�� �� ������Ʈ�� �����Ͽ� �߰�
	/// </summary>
	/// <param name="_name">�߰��� ������Ʈ �̸�</param>
	/// <returns>������ ������Ʈ, �ߺ��� nullptr ��ȯ</returns>
	virtual GameObject* CreateGameObject(const std::string& _name) { return 0; };

	/// <summary>
	/// ������Ʈ ����Ʈ�� �� ������Ʈ�� �����Ͽ� �߰�
	/// </summary>
	/// <param name="_name">�߰��� ������Ʈ �̸�</param>
	/// <param name="modelName">���ҽ� �Ŵ����� ��ϵ� ���� �̸�</param>
	/// <returns>������ ������Ʈ, �ߺ��� nullptr ��ȯ</returns>
	virtual GameObject* CreateGameObject(const std::string& _name, const std::string& modelName) { return nullptr; };

	/// <summary>
	/// ������Ʈ ��Ͽ� Ư�� ������Ʈ�� �����ϴ��� �˻�
	/// </summary>
	/// <param name="_name">�˻��� ������Ʈ �̸�</param>
	/// <returns>��� ������Ʈ, ����� nullptr ��ȯ</returns>
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

	/// <summary>
	/// ��Ŀ�� �� ������Ʈ�� ������ [ �������� ���� �� nullptr ��ȯ ]
	/// </summary>
	/// <returns></returns>
	virtual GameObject* GetFocusedObject() { return nullptr; };
	/// <summary>
	/// ��Ŀ�� �� ������Ʈ �����ʸ� ����մϴ�.
	/// </summary>
	/// <param name="callback"></param>
	virtual void AddFocusedObjectListener(std::function<void(GameObject*)> callback) {};
	/// <summary>
	/// ���� ������Ʈ ���� �ҷ��ɴϴ�.
	/// </summary>
	/// <returns> �ش� �Ŵ����� ������Ʈ �� </returns>
	virtual std::unordered_map<std::string, GameObject*> GetObejctMap() { return std::unordered_map<std::string, GameObject*>(); }
};



class GameObjectManager : public IGameObjectManager{

public:

	/// <summary>
	/// GameObjectManager�� �����մϴ�.
	/// </summary>
	/// <param name="framework">Framework* �������� ���� Framework�� ���޹ݽ��ϴ�.</param>
	/// <param name="graphicManager">GraphicManager* �������� ���� GraphicManager�� ���޹ݽ��ϴ�.</param>
	/// <param name="res">ResourceManager* �������� ���� ResourceManager�� ���޹ݽ��ϴ�.</param>
	GameObjectManager(Framework* framework, SceneMode type);

	virtual SceneMode GetMode() override;
	
	virtual GameObject* CreateGameObject(const std::string& _name) override;

	virtual GameObject* CreateGameObject(const std::string& _name, const std::string& path) override;

	virtual GameObject* FindGameObject(const std::string& _name) override;

	virtual void DestroyGameObject(const std::string& _name) override;
	
	virtual std::string GetObjectNames() override;

	virtual GameObject* GetFocusedObject() override;

	virtual void AddFocusedObjectListener(std::function<void(GameObject*)> callback) override;

	void notifyFousedObject(GameObject* object);
	
	virtual std::unordered_map<std::string, GameObject*> GetObejctMap() override;



	//������
	Framework* framework;
	ResourceManager* res;
	GraphicManager* graphicManager;

	//��
	std::unordered_map<std::string, GameObject*> gameObjects;
	std::vector<std::function<void(GameObject*)>> _focusedObjectCallback;
	
	//�ĺ���
	SceneMode type;
	
};



