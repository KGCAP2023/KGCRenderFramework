#pragma once
#include "Component.h"
#include "Transform.h"
#include "BoundingBox.h"
using namespace DirectX;
//클래스 변수에  XMFLOAT를 넣자
//힙에서는 16바이트 정렬이 조금 까다로움

class GameObject
{
private:
	std::unordered_map<Component::Type, Component *, Component::ComponentHash> components;
	static GameObject* game;
public:

	static enum class ObjectType
	{
		OBJECT_DEFAULT,
		OBJECT_SPRITE_BATCH
	};

	GameObject(const std::string& name) : transform(Transform(this)) 
	{
		ObjectName = name;
		isActive = true;
		isDestroy = false;
		transform.SetScale(1.0f, 1.0f, 1.0f);
	}

	GameObject(const GameObject& rhs) : transform(Transform(this))
	{
		std::cout << "[=] GameObject CLONE Process - Copy constructor called" << std::endl;

		ObjectName = rhs.ObjectName;
		isActive = rhs.isActive;
		isDestroy = rhs.isDestroy;
		bbox = rhs.bbox;
		transform.Copy(rhs.transform);
		
		for (auto& pair : rhs.components)
		{
			Component* compo = pair.second->Copy(this);
			this->AddComponent(compo);
		}
	}

	~GameObject()
	{
		for (auto& pair : components)
		{
			delete pair.second;
		}
		components.clear();
	}

	bool isDestroy;
	bool isActive;
	std::string ObjectName;

	Transform transform;

	/// <summary>
	/// 이름을 반환합니다.
	/// </summary>
	/// <returns></returns>
	std::string GetName() { return ObjectName; };
	/// <summary>
	/// Component를 추가합니다.
	/// <주의> 한오브젝트에서 렌더러를 2D렌더러,3D렌더러 둘다 사용하지 마십시요
	/// </summary>
	/// <param name="pComponent"></param>
	void AddComponent(Component* pComponent);
	/// <summary>
	/// 컴포넌트를 삭제합니다.
	/// <주의>아직 기능이 불완전합니다.
	/// </summary>
	/// <param name="componentID"></param>
	void RemoveComponent(const Component::Type componentID);
	/// <summary>
	/// 컴포넌트를 가져옵니다.
	/// </summary>
	/// <param name="componentID"></param>
	/// <returns></returns>
	Component* GetComponent(const std::string componentID);
	/// <summary>
	/// 컴포넌트를 가져옵니다.
	/// </summary>
	/// <param name="componentID"></param>
	/// <returns></returns>
	Component* GetComponent(const Component::Type componentID);
	/// <summary>
	/// 컴포넌트의 해시맵을 반환합니다.
	/// </summary>
	/// <returns></returns>
	std::unordered_map<Component::Type, Component*, Component::ComponentHash> GetComponentMap();
	/// <summary>
	/// 컴포넌트의 사이즈를 가져옵니다.
	/// </summary>
	/// <returns></returns>
	int GetComponentSize();
	/// <summary>
	/// 컴포넌트를 순회합니다 (람다식)
	/// </summary>
	/// <param name="callback"></param>
	void ComponentForeach(std::function<void(Component*)> callback);
	/// <summary>
	/// 컴포넌트를 정리합니다.
	/// <주의>아직 기능이 불완전합니다.
	/// </summary>
	void CleanUpComponent();

	/// <summary>
	/// 해당 오브젝트에게 포커스를 부여합니다.
	/// </summary>
	void SetFocus();

	/// <summary>
	/// 포커스된 오브젝트의 포거스를 해제합니다.
	/// </summary>
	static void ReleaseFocus();

	/// <summary>
	/// 현재 포커스를 가지고 있는 오브젝트를 반환합니다 [ 없을 시 nullptr 반환 ]
	/// </summary>
	/// <returns></returns>
	static GameObject* GetFocusedObject();

	void Draw(const XMMATRIX& viewProjectionMatrix);
	void DrawSprite(const XMMATRIX& viewProjectionMatrix);
	void Update();

	void Destroy() { this->isDestroy = true; }
	bool IsDestroy() { return this->isDestroy; }

	void SetActive(bool bActive);
	bool IsActive() { return this->isActive; }

	virtual bool IsCollision(GameObject* model) { return false; };
	virtual void* GetCollider() { return NULL; }
	Component::Type GetColliderType() { return colliderType; }

	void SetParent(GameObject* pParent);
	GameObject* GetParent() { return parent; }
	void AddChild(GameObject* pParent);

	BoundingBoxRenderer* bbox = nullptr;
	//콜라이더 추가해야됨 
	Component::Type colliderType;
	GameObject* parent = nullptr;
	std::vector<GameObject*> child;
};





