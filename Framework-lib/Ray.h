#pragma once
#include "pch.h"
#include "BoundingBox.h"

class Framework;
class GraphicManager;

class Ray
{
public:

	Ray() {}

	Ray(Framework* framework);

	bool RaySphereIntersect(XMFLOAT3 rayOrigin, XMFLOAT3 rayDirection, float radius);

	float RayIntersectTri(XMFLOAT3 rayOrigin, XMFLOAT3 rayDirection, XMVECTOR v1, XMVECTOR v2, XMVECTOR v3);

	void CalculatePicking(int mouseX, int mouseY);

	float isPicked(BoundingBox3D* bbox);

	float screenWidth;
	float screenHeight;

	bool isOrthoGrahpicProjection = false;

	Framework* framework;
	GraphicManager* graphic;

	XMFLOAT3 direction;		//���� ����
	XMFLOAT3 origin;		//ī�޶� ��ǥ
	XMFLOAT3 rayOrigin;		//���� �������
	XMFLOAT3 rayDirection;	//���� ����
};