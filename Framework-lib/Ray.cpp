#include "pch.h"
#include "Ray.h"
#include "Framework.h"

//Rastek Mouse Picking ����

Ray::Ray(Framework* framework)
{
	this->framework = framework;
	this->graphic = &(framework->graphics);
	this->screenWidth = framework->width;
	this->screenHeight = framework->height;
}

bool Ray::RaySphereIntersect(XMFLOAT3 rayOrigin, XMFLOAT3 rayDirection, float radius)
{
	// a, b �� c ����� ����մϴ�.
	float a = (rayDirection.x * rayDirection.x) + (rayDirection.y * rayDirection.y) + (rayDirection.z * rayDirection.z);
	float b = ((rayDirection.x * rayOrigin.x) + (rayDirection.y * rayOrigin.y) + (rayDirection.z * rayOrigin.z)) * 2.0f;
	float c = ((rayOrigin.x * rayOrigin.x) + (rayOrigin.y * rayOrigin.y) + (rayOrigin.z * rayOrigin.z)) - (radius * radius);

	// ������� ��´�
	float discriminant = (b * b) - (4 * a * c);

	// ������� �����̸� ��ŷ ���� ���� ��� ���Դϴ�. �׷��� ������ ���� �����մϴ�.
	if (discriminant < 0.0f)
	{
		return false;
	}

	return true;
}

float Ray::RayIntersectTri(XMFLOAT3 rayOrigin, XMFLOAT3 rayDirection, XMVECTOR v1, XMVECTOR v2, XMVECTOR v3)
{
	float distance;

	if (TriangleTests::Intersects(XMLoadFloat3(&rayOrigin), XMLoadFloat3(&rayDirection), v1, v2, v3, distance))
	{
		//XMFLOAT3 a;

		//a.x = rayOrigin.x + rayDirection.x * distance;
		//a.y = rayOrigin.y + rayDirection.y * distance;
		//a.z = rayOrigin.z + rayDirection.z * distance;

		//std::cout << a.x << "/" << a.y << "/" << a.z << std::endl;
		//std::cout << distance << std::endl;
		return distance;
	}
	else
		return -1;
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


	// ���콺 Ŀ�� ��ǥ�� -1���� +1 ������ �̵��մϴ�
	float pointX = ((2.0f * (float)mouseX) / (float)this->screenWidth) - 1.0f;
	float pointY = (((2.0f * (float)mouseY) / (float)this->screenHeight) - 1.0f) * -1.0f;

	// ����Ʈ�� ��Ⱦ�� ����Ͽ� ���� ����� ����Ͽ� ���� �����մϴ�

	pointX = pointX / projectionMatrix4._11;
	pointY = pointY / projectionMatrix4._22;

	// �� ����� ���Լ��� ���մϴ�.

	inverseView = XMMatrixInverse(nullptr, view);
	XMFLOAT3X3 inverseViewMatrix4;
	XMStoreFloat3x3(&inverseViewMatrix4, inverseView);

	// �� �������� ��ŷ ������ ������ ����մϴ�.
	// [x,y,z] * [������� �����]

	direction.x = (pointX * inverseViewMatrix4._11) + (pointY * inverseViewMatrix4._21) + inverseViewMatrix4._31;
	direction.y = (pointX * inverseViewMatrix4._12) + (pointY * inverseViewMatrix4._22) + inverseViewMatrix4._32;
	direction.z = (pointX * inverseViewMatrix4._13) + (pointY * inverseViewMatrix4._23) + inverseViewMatrix4._33;

	// ī�޶��� ��ġ �� picking ray�� ������ �����ɴϴ�.

	if (isOrthoGrahpicProjection)
	{
		XMVECTOR up = XMVector3Cross(camera->transform.GetLeft(), camera->transform.GetForward());
		up = XMVectorScale(up, pointY);
		XMVECTOR right = camera->transform.GetRight();
		right = XMVectorScale(right, pointX);
		XMVECTOR pos = camera->transform.GetPositionXMVector() + right + up;
		XMStoreFloat3(&origin, pos);
	}
	else
	{
		origin = camera->transform.GetPositionXMFloat3();
	}
}

float Ray::isPicked(BoundingBox3D* bbox)
{

	Transform& transform = bbox->GetOwner()->transform;
	XMMATRIX world = transform.worldMatrix;

	if (transform.scale.x == 0 || transform.scale.y == 0 || transform.scale.z == 0)
		return 0;

	// ���� ����� ���Լ��� ���Ͻʽÿ�.
	XMMATRIX inverseWorld = XMMatrixInverse(nullptr, world);
	//inverseWorld = XMMatrixTranspose(XMMatrixRotationRollPitchYaw(transform.rotation.x, transform.rotation.y, transform.rotation.z)) * inverseWorld;

	// ���� ���� ������ ���� ������ �� �������� ���� �������� ��ȯ�մϴ�.
	XMStoreFloat3(&rayOrigin, XMVector3TransformCoord(XMVectorSet(origin.x, origin.y, origin.z, 0.0f), inverseWorld));
	XMStoreFloat3(&direction, XMVector3TransformNormal(XMVectorSet(direction.x, direction.y, direction.z, 0.0f), inverseWorld));

	// ���� ������ ǥ��ȭ�մϴ�.

	if(isOrthoGrahpicProjection)
		XMStoreFloat3(&rayDirection, XMVector3Normalize(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f)));
	else
		XMStoreFloat3(&rayDirection, XMVector3Normalize(XMVectorSet(direction.x, direction.y, direction.z, 0.0f)));

	// ���� ���� �� ���� �׽�Ʈ�� �����Ͻʽÿ�.
	//if (RaySphereIntersect(rayOrigin, rayDirection, 2.0f) == true)
	//{
	//	std::cout << "picking ����" << std::endl;
	//	return true;
	//}
	//else
	//{
	//	return false;
	//}

	float min_distance = 1000000;

	for (int i = 0; i < 12; i++)
	{
		int k = i * 3;

		int a = bbox->tri[k];
		int b = bbox->tri[k+1];
		int c = bbox->tri[k+2];

		SimpleVertex& t1 = bbox->vertices[a];
		SimpleVertex& t2 = bbox->vertices[b];
		SimpleVertex& t3 = bbox->vertices[c];

		float distance = RayIntersectTri(rayOrigin, rayDirection, XMVectorSet(t1.Pos.x, t1.Pos.y, t1.Pos.z, 1.0f), XMVectorSet(t2.Pos.x, t2.Pos.y, t2.Pos.z, 1.0f), XMVectorSet(t3.Pos.x, t3.Pos.y, t3.Pos.z, 1.0f));
		if (distance != -1 && distance < min_distance)
		{
			min_distance = distance;
		}
	}

	if (min_distance == 1000000)
		return -1;
	else
		return min_distance;
}
