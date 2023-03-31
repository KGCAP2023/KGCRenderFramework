#include "pch.h"
#include "Ray.h"
#include "Framework.h"

//Rastek Mouse Picking 참조

Ray::Ray(Framework* framework)
{
	this->framework = framework;
	this->graphic = &(framework->graphics);
	this->screenWidth = framework->width;
	this->screenHeight = framework->height;
}

bool Ray::RaySphereIntersect(XMFLOAT3 rayOrigin, XMFLOAT3 rayDirection, float radius)
{
	// a, b 및 c 계수를 계산합니다.
	float a = (rayDirection.x * rayDirection.x) + (rayDirection.y * rayDirection.y) + (rayDirection.z * rayDirection.z);
	float b = ((rayDirection.x * rayOrigin.x) + (rayDirection.y * rayOrigin.y) + (rayDirection.z * rayOrigin.z)) * 2.0f;
	float c = ((rayOrigin.x * rayOrigin.x) + (rayOrigin.y * rayOrigin.y) + (rayOrigin.z * rayOrigin.z)) - (radius * radius);

	// 결과값을 얻는다
	float discriminant = (b * b) - (4 * a * c);

	// 결과값이 음수이면 피킹 선이 구를 벗어난 것입니다. 그렇지 않으면 구를 선택합니다.
	if (discriminant < 0.0f)
	{
		return false;
	}

	return true;
}

bool Ray::RayIntersectTri(XMFLOAT3 rayOrigin, XMFLOAT3 rayDirection, XMVECTOR v1, XMVECTOR v2, XMVECTOR v3)
{
	float distance;
	if (TriangleTests::Intersects(XMLoadFloat3(&rayOrigin), XMLoadFloat3(&rayDirection), v1, v2, v3, distance))
		return true;
	else
		return false;
}

void Ray::CalculatePicking(int mouseX, int mouseY)
{
	GameObject* camera = this->graphic->camera;
	Camera3D* cameraComponent = this->graphic->cameraComponent;

	XMMATRIX proj, view;
	XMMATRIX inverseView;
	XMMATRIX translate;


	proj = cameraComponent->GetProjectionMatrix();
	view = cameraComponent->GetViewMatrix();

	XMFLOAT3X3 projectionMatrix4;
	XMStoreFloat3x3(&projectionMatrix4, proj);


	// 마우스 커서 좌표를 -1에서 +1 범위로 이동합니다
	float pointX = ((2.0f * (float)mouseX) / (float)this->screenWidth) - 1.0f;
	float pointY = (((2.0f * (float)mouseY) / (float)this->screenHeight) - 1.0f) * -1.0f;

	// 뷰포트의 종횡비를 고려하여 투영 행렬을 사용하여 점을 조정합니다

	pointX = pointX / projectionMatrix4._11;
	pointY = pointY / projectionMatrix4._22;

	// 뷰 행렬의 역함수를 구합니다.

	inverseView = XMMatrixInverse(nullptr, view);
	XMFLOAT3X3 inverseViewMatrix4;
	XMStoreFloat3x3(&inverseViewMatrix4, inverseView);

	// 뷰 공간에서 피킹 레이의 방향을 계산합니다.
	// [x,y,z] * [뷰행렬의 역행렬]
	// 뷰 -> 월드

	direction.x = (pointX * inverseViewMatrix4._11) + (pointY * inverseViewMatrix4._21) + inverseViewMatrix4._31;
	direction.y = (pointX * inverseViewMatrix4._12) + (pointY * inverseViewMatrix4._22) + inverseViewMatrix4._32;
	direction.z = (pointX * inverseViewMatrix4._13) + (pointY * inverseViewMatrix4._23) + inverseViewMatrix4._33;

	// 카메라의 위치 인 picking ray의 원점을 가져옵니다.
	origin = camera->transform.GetPositionXMFloat3();
	
}

bool Ray::isPicked(BoundingBox3D* bbox)
{
	// 월드 행렬의 역함수를 구하십시오.
	XMMATRIX world = bbox->GetOwner()->transform.worldMatrix;
	XMMATRIX inverseWorld = XMMatrixInverse(nullptr, world);

	// 이제 광선 원점과 광선 방향을 뷰 공간에서 월드 공간으로 변환합니다.
	XMStoreFloat3(&rayOrigin, XMVector3TransformCoord(XMVectorSet(origin.x, origin.y, origin.z, 0.0f), inverseWorld));
	XMStoreFloat3(&direction, XMVector3TransformNormal(XMVectorSet(direction.x, direction.y, direction.z, 0.0f), inverseWorld));

	// 광선 방향을 표준화합니다.
	XMStoreFloat3(&rayDirection, XMVector3Normalize(XMVectorSet(direction.x, direction.y, direction.z, 0.0f)));

	// 이제 광선 구 교차 테스트를 수행하십시오.
	//if (RaySphereIntersect(rayOrigin, rayDirection, 2.0f) == true)
	//{
	//	std::cout << "picking 감지" << std::endl;
	//	return true;
	//}
	//else
	//{
	//	return false;
	//}

	for (int i = 0; i < 12; i++)
	{
		int k = i * 3;

		int a = bbox->tri[k];
		int b = bbox->tri[k+1];
		int c = bbox->tri[k+2];

		SimpleVertex& t1 = bbox->vertices[a];
		SimpleVertex& t2 = bbox->vertices[b];
		SimpleVertex& t3 = bbox->vertices[c];
		
		if (RayIntersectTri(rayOrigin, rayDirection, XMVectorSet(t1.Pos.x, t1.Pos.y, t1.Pos.z, 1.0f), XMVectorSet(t2.Pos.x, t2.Pos.y, t2.Pos.z, 1.0f), XMVectorSet(t3.Pos.x, t3.Pos.y, t3.Pos.z, 1.0f)))
		{
			std::cout << bbox->GetOwner()->GetName() << " 감지" << std::endl;
			return true;
		}
	}

	return false;
}
