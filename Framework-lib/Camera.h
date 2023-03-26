#pragma once
#include "GameObject.h"
using namespace DirectX;

enum class viewType { _2D, _3D };

class Camera3D : public Component
{
public:
	Camera3D(GameObject* owner);

	/// <summary>
	/// 카메라 2d, 3d 모드 변환
	/// </summary>
	/// <param name="_type">[viewType::_2D] 또는 [viewType::_3D] 입력</param>
	void ChangeProjectionValues(viewType _type);
	
	/// <summary>
	/// 카메라 기본 뷰 메트릭스 초기화
	/// </summary>
	/// <param name="fovDegrees">3D 시야각</param>
	/// <param name="aspectRatio">3D 종횡비</param>
	/// <param name="nearZ">후방 z값</param>
	/// <param name="farZ">전방 z값</param>
	/// <param name="_width">2D 시점 가로 길이</param>
	/// <param name="_hight">2D 시점 세로 길이</param>
	/// <param name="_nearZ2">2D 후방 z값</param>
	/// <param name="_farZ2">2D 전방 z값</param>
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

