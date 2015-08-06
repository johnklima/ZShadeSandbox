#include "PlanePrimitive.h"
using ZShadeSandboxMath::PlanePrimitive;
//==============================================================================================================================
//==============================================================================================================================
PlanePrimitive::PlanePrimitive(XMFLOAT3 normal)
:   normal(normal)
{
	d = CalculateDistance(0, 0, 0);
}
//==============================================================================================================================
PlanePrimitive::PlanePrimitive(XMFLOAT3 point1, XMFLOAT3 point2, XMFLOAT3 point3)
{
	CreateDirection(point1, point2, point3);
	d = CalculateDistance(point1);
}
//==============================================================================================================================
PlanePrimitive::PlanePrimitive(XMFLOAT4 normal)
:   normal4(normal)
{
	d = CalculateDistance(0, 0, 0);
}
//==============================================================================================================================
PlanePrimitive::PlanePrimitive(XMFLOAT4 point1, XMFLOAT4 point2, XMFLOAT4 point3)
{
	CreateDirection(point1, point2, point3);
	d = CalculateDistance(point1);
}
//==============================================================================================================================
void PlanePrimitive::CreateDirection(XMFLOAT3 point1, XMFLOAT3 point2, XMFLOAT3 point3)
{
	// Equation of a plane is ax + by + cz = d

	// Plane is lifted in a direction until it coincides with the one that you want

	// Create a perpendicular line

	ZShadeSandboxMath::Vec3<XMFLOAT3> A(point1);
	ZShadeSandboxMath::Vec3<XMFLOAT3> B(point2);
	ZShadeSandboxMath::Vec3<XMFLOAT3> C(point3);

	// Axis of the triangle
	ZShadeSandboxMath::Vec3<XMFLOAT3> u = B - A;
	ZShadeSandboxMath::Vec3<XMFLOAT3> v = C - A;

	// Direction of the plane
	normal = u.Cross(v);

	// Normalize the plane
	//normal.Normalize();
}
//==============================================================================================================================
void PlanePrimitive::CreateDirection(XMFLOAT4 point1, XMFLOAT4 point2, XMFLOAT4 point3)
{
	// Equation of a plane is ax + by + cz = d

	// Plane is lifted in a direction until it coincides with the one that you want

	// Create a perpendicular line

	ZShadeSandboxMath::Vec4<XMFLOAT4> A(point1);
	ZShadeSandboxMath::Vec4<XMFLOAT4> B(point2);
	ZShadeSandboxMath::Vec4<XMFLOAT4> C(point3);

	// Axis of the triangle
	ZShadeSandboxMath::Vec4<XMFLOAT4> u = B - A;
	ZShadeSandboxMath::Vec4<XMFLOAT4> v = C - A;

	// Direction of the plane
	normal4 = u.Cross(v);

	// Normalize the plane
	//normal.Normalize();
}
//==============================================================================================================================
float PlanePrimitive::CalculateDistance(float x, float y, float z)
{
	// Distance of the plane
	ZShadeSandboxMath::Vec3<XMFLOAT3> point(x, y, z);
	ZShadeSandboxMath::Vec3<XMFLOAT3> n(normal);
	return n.Dot(point);
}
//==============================================================================================================================
float PlanePrimitive::CalculateDistance(float x, float y, float z, float w)
{
	// Distance of the plane
	ZShadeSandboxMath::Vec4<XMFLOAT4> point(x, y, z, w);
	ZShadeSandboxMath::Vec4<XMFLOAT4> n(normal4);
	return n.Dot(point);
}
//==============================================================================================================================
float PlanePrimitive::CalculateDistance(XMFLOAT3 point)
{
	// Distance of the plane
	ZShadeSandboxMath::Vec3<XMFLOAT3> n(normal);
	return n.Dot(point);
}
//==============================================================================================================================
float PlanePrimitive::CalculateDistance(XMFLOAT4 point)
{
	// Distance of the plane
	ZShadeSandboxMath::Vec4<XMFLOAT4> n(normal4);
	return n.Dot(point);
}
//==============================================================================================================================
bool PlanePrimitive::PointInPlane(XMFLOAT3 point)
{
	float dist = CalculateDistance(point);

	// If d=0 then the point is on the plane and they are orthogonal
	return dist == 0 ? true : false;
}
//==============================================================================================================================
bool PlanePrimitive::PointInPlane(XMFLOAT4 point)
{
	float dist = CalculateDistance(point);

	// If d=0 then the point is on the plane and they are orthogonal
	return dist == 0 ? true : false;
}
//==============================================================================================================================
bool PlanePrimitive::BehindPlane(XMFLOAT3 point)
{
	float dist = CalculateDistance(point);

	if (dist < 0) return true;
	else if (dist > 0) return false;

	//We are in the plane so we don't care here
	return false;
}
//==============================================================================================================================
bool PlanePrimitive::BehindPlane(XMFLOAT4 point)
{
	float dist = CalculateDistance(point);

	if (dist < 0) return true;
	else if (dist > 0) return false;

	//We are in the plane so we don't care here
	return false;
}
//==============================================================================================================================
XMFLOAT3 PlanePrimitive::Normal() const { return normal; }
//==============================================================================================================================
XMFLOAT4 PlanePrimitive::Normal4() const { return normal4; }
//==============================================================================================================================
float PlanePrimitive::Distance() const { return d; }
//==============================================================================================================================