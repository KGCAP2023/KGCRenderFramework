#pragma once
#include "Component.h"
#include "Transform.h"
#include "BoundingBox.h"
using namespace DirectX;
//Ŭ���� ������  XMFLOAT�� ����
//�������� 16����Ʈ ������ ���� ��ٷο�

class GameObject
{
private:
	std::unordered_map<Component::Type, Component *, Component::ComponentHash> components;
public:

	static enum class ObjectType
	{
		OBJECT_DEFAULT,
		OBJECT_SPRITE
	};

	GameObject(const std::string& name) : transform(Transform(this)) 
	{
		ObjectName = name;
		isActive = true;
		isDestroy = false;
		this->objectType = GameObject::ObjectType::OBJECT_DEFAULT;
	}

	~GameObject();

	bool isDestroy;
	bool isActive;
	std::string ObjectName;

	Transform transform;
	GameObject::ObjectType objectType = GameObject::ObjectType::OBJECT_DEFAULT;
	BoundingBoxRenderer* bbox = nullptr;

	BoundingBox3D* bb3d = nullptr;

	void SetObjectType(GameObject::ObjectType type);

	/// <summary>
	/// �̸��� ��ȯ�մϴ�.
	/// </summary>
	/// <returns></returns>
	std::string GetName() { return ObjectName; };
	/// <summary>
	/// Component�� �߰��մϴ�.
	/// <����> �ѿ�����Ʈ���� �������� 2D������,3D������ �Ѵ� ������� ���ʽÿ�
	/// </summary>
	/// <param name="pComponent"></param>
	void AddComponent(Component* pComponent);
	/// <summary>
	/// ������Ʈ�� �����մϴ�.
	/// <����>���� ����� �ҿ����մϴ�.
	/// </summary>
	/// <param name="componentID"></param>
	void RemoveComponent(const Component::Type componentID);
	/// <summary>
	/// ������Ʈ�� �����ɴϴ�.
	/// </summary>
	/// <param name="componentID"></param>
	/// <returns></returns>
	Component* GetComponent(const std::string componentID);
	/// <summary>
	/// ������Ʈ�� �����ɴϴ�.
	/// </summary>
	/// <param name="componentID"></param>
	/// <returns></returns>
	Component* GetComponent(const Component::Type componentID);
	/// <summary>
	/// ������Ʈ�� �ؽø��� ��ȯ�մϴ�.
	/// </summary>
	/// <returns></returns>
	std::unordered_map<Component::Type, Component*, Component::ComponentHash> GetComponentMap();
	/// <summary>
	/// ������Ʈ�� ����� �����ɴϴ�.
	/// </summary>
	/// <returns></returns>
	int GetComponentSize();
	/// <summary>
	/// ������Ʈ�� ��ȸ�մϴ� (���ٽ�)
	/// </summary>
	/// <param name="callback"></param>
	void ComponentForeach(std::function<void(Component*)> callback);
	/// <summary>
	/// ������Ʈ�� �����մϴ�.
	/// <����>���� ����� �ҿ����մϴ�.
	/// </summary>
	void CleanUpComponent();

	void Draw(const XMMATRIX& viewProjectionMatrix);
	void Update();

	void Destroy() { this->isDestroy = true; }
	bool IsDestroy() { return this->isDestroy; }

	void SetActive(bool bActive);
	bool IsActive() { return this->isActive; }

	BoundingBoxRenderer* GetBoundingBox() { return bbox; }

	virtual bool IsCollision(GameObject* model) { return false; };
	virtual void* GetCollider() { return NULL; }
	Component::Type GetColliderType() { return colliderType; }

	void SetParent(GameObject* pParent);
	GameObject* GetParent() { return parent; }
	void AddChild(GameObject* pParent);


	//�ݶ��̴� �߰��ؾߵ� 
	
	Component::Type colliderType;
	GameObject* parent = nullptr;
	std::vector<GameObject*> child;
};





