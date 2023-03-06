#pragma once
#include "GameObject.h"
using namespace DirectX;

class Camera3D : public Component
{
public:
	Camera3D(GameObject* owner);

	void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ);

	const XMMATRIX& GetViewMatrix() const;
	const XMMATRIX& GetProjectionMatrix() const;

private:
	virtual void Update() override;

	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;

};

