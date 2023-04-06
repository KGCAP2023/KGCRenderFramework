#pragma once
#include "pch.h"
#include "GameObject.h"

class Framework;
class ResourceManager;
class GraphicManager;

class IGameObjectManager {

public:
	/// <summary>
	/// 오브젝트 리스트에 새 오브젝트를 생성하여 추가
	/// </summary>
	/// <param name="_name">추가할 오브젝트 이름</param>
	/// <returns>생성된 오브젝트, 중복시 nullptr 반환</returns>
	virtual GameObject* CreateGameObject(const std::string& _name) { return nullptr; };

	/// <summary>
	/// 오브젝트 리스트에 새 오브젝트를 생성하여 추가
	/// </summary>
	/// <param name="_name">추가할 오브젝트 이름</param>
	/// <param name="path">추가할 오브젝트 파일의 경로</param>
	/// <returns>생성된 오브젝트, 중복시 nullptr 반환</returns>
	virtual GameObject* CreateGameObject(const std::string& _name, const std::string& path) { return nullptr; };

	/// <summary>
	/// 오브젝트 목록에 특정 오브젝트가 존재하는지 검색
	/// </summary>
	/// <param name="_name">검색할 오브젝트 이름</param>
	/// <returns>결과 오브젝트, 부재시 nullptr 반환</returns>
	virtual GameObject* FindGameObject(const std::string& _name) { return nullptr; };

	/// <summary>
	/// 오브젝트 목록에서 특정 오브젝트 제거
	/// </summary>
	/// <param name="_name">제거할 오브젝트 이름</param>
	virtual void DestroyGameObject(const std::string& _name) {};

	/// <summary>
	/// 현재 리스트 내 오브젝트들의 이름들을 가져옴
	/// </summary>
	/// <returns></returns>
	virtual std::string GetObjectNames() { return 0; };

	/// <summary>
	/// 포커스 된 오브젝트를 가져옴 [ 존재하지 않을 시 nullptr 반환 ]
	/// </summary>
	/// <returns></returns>
	virtual GameObject* GetFocusedObject() { return nullptr; };
};



class GameObjectManager : public IGameObjectManager{

public:
	/// <summary>
	/// GameObjectManager를 생성합니다.
	/// </summary>
	/// <param name="framework">Framework* 형식으로 현재 Framework를 전달반습니다.</param>
	/// <param name="graphicManager">GraphicManager* 형식으로 현재 GraphicManager를 전달반습니다.</param>
	/// <param name="res">ResourceManager* 형식으로 현재 ResourceManager를 전달반습니다.</param>
	GameObjectManager(Framework* framework);
	
	virtual GameObject* CreateGameObject(const std::string& _name) override;

	virtual GameObject* CreateGameObject(const std::string& _name, const std::string& path) override;

	virtual GameObject* FindGameObject(const std::string& _name) override;

	virtual void DestroyGameObject(const std::string& _name) override;
	
	virtual std::string GetObjectNames() override;

	virtual GameObject* GetFocusedObject() override;


	//의존성
	Framework* framework;
	ResourceManager* res;
	GraphicManager* graphicManager;

	//맵
	std::unordered_map<std::string, GameObject*> gameObjects;
	
};



