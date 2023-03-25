#pragma once
#include "GameObject.h"
using namespace DirectX;

enum class viewType { _2D, _3D };

class Camera3D : public Component
{
public:
	Camera3D(GameObject* owner);

	/// <summary>
	/// ī�޶� 2d, 3d ��� ��ȯ
	/// </summary>
	/// <param name="_type">[viewType::_2D] �Ǵ� [viewType::_3D] �Է�</param>
	void ChangeProjectionValues(viewType _type);
	
	/// <summary>
	/// ī�޶� �⺻ �� ��Ʈ���� �ʱ�ȭ
	/// </summary>
	/// <param name="fovDegrees">3D �þ߰�</param>
	/// <param name="aspectRatio">3D ��Ⱦ��</param>
	/// <param name="nearZ">�Ĺ� z��</param>
	/// <param name="farZ">���� z��</param>
	/// <param name="_width">2D ���� ���� ����</param>
	/// <param name="_hight">2D ���� ���� ����</param>
	/// <param name="_nearZ2">2D �Ĺ� z��</param>
	/// <param name="_farZ2">2D ���� z��</param>
	void initViewMatrix(float fovDegrees, float aspectRatio, float nearZ, float farZ,
		float _width, float _hight, float _nearZ2, float _farZ2);
			

	const XMMATRIX& GetViewMatrix() const;
	const XMMATRIX& GetProjectionMatrix() const;

private:
	virtual void Update() override;

	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;

	XMMATRIX view3DMatrix;
	XMMATRIX view2DMatrix;

};

