#pragma once
#include "pch.h"
#include "GameObject.h"

class IGameObjectManager {

public:
	
	/// <summary>
	/// 오브젝트 리스트에 새 오브젝트를 생성하여 추가
	/// </summary>
	/// <param name="_name">추가할 오브젝트 이름</param>
	/// <returns>생성된 오브젝트</returns>
	virtual GameObject* CreateGameObject(const std::string& _name) { return nullptr; };

	/// <summary>
	/// 오브젝트 목록에 특정 오브젝트가 존쟂하는지 검색
	/// </summary>
	/// <param name="_name">검색할 오브젝트 이름</param>
	/// <returns>결과 오브젝트</returns>
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
};

class GameObjectManager : public IGameObjectManager{

public:

	virtual GameObject* CreateGameObject(const std::string& _name) override;

	virtual GameObject* FindGameObject(const std::string& _name) override;

	virtual void DestroyGameObject(const std::string& _name) override;
	
	virtual std::string GetObjectNames() override;

	std::unordered_map<std::string, GameObject*> gameObjects;

};

