#include "TrianglePrimitive.h"
using ZShadeSandboxMath::TrianglePrimitive;
//==============================================================================================================================
//==============================================================================================================================
TrianglePrimitive::TrianglePrimitive()
{
}
//==============================================================================================================================
TrianglePrimitive::TrianglePrimitive(XMFLOAT3 p1, XMFLOAT3 p2, XMFLOAT3 p3)
:   point1(p1)
,   point2(p2)
,   point3(p3)
{
}
//==============================================================================================================================
/*TrianglePrimitive::TrianglePrimitive(Vec3<XMFLOAT3> p1, Vec3<XMFLOAT3> p2, Vec3<XMFLOAT3> p3)
:	point1(p1)
,   point2(p2)
,   point3(p3)
{
}*/
//==============================================================================================================================
XMFLOAT3 TrianglePrimitive::Point1() const { return point1; }
//==============================================================================================================================
XMFLOAT3 TrianglePrimitive::Point2() const { return point2; }
//==============================================================================================================================
XMFLOAT3 TrianglePrimitive::Point3() const { return point3; }
//==============================================================================================================================
float TrianglePrimitive::Area()
{
	return 0.5 * (point1.x * (point2.y - point3.y) + point2.x * (point3.y - point1.y) + point3.x * (point1.y - point2.y));
}
//==============================================================================================================================
bool TrianglePrimitive::operator == (const TrianglePrimitive& tri)
{
	bool P1xEq = point1.x == tri.Point1().x;
	bool P1yEq = point2.y == tri.Point2().y;
	bool P1zEq = point3.z == tri.Point3().z;

	bool P2xEq = point1.x == tri.Point1().x;
	bool P2yEq = point2.y == tri.Point2().y;
	bool P2zEq = point3.z == tri.Point3().z;

	bool P3xEq = point1.x == tri.Point1().x;
	bool P3yEq = point2.y == tri.Point2().y;
	bool P3zEq = point3.z == tri.Point3().z;

	bool P1Eq = P1xEq && P1yEq && P1zEq;
	bool P2Eq = P2xEq && P2yEq && P2zEq;
	bool P3Eq = P3xEq && P3yEq && P3zEq;

	return P1Eq && P2Eq && P3Eq;
}
//==============================================================================================================================
bool TrianglePrimitive::Inside(XMFLOAT3 point)
{
	// Compute barycentric coordinates for the triangle

	float alpha = ((point2.y - point3.y)*(point.x - point3.x) + (point3.x - point2.x)*(point.y - point3.y)) /
		((point2.y - point3.y)*(point1.x - point3.x) + (point3.x - point2.x)*(point1.y - point3.y));
	float beta = ((point3.y - point1.y)*(point.x - point3.x) + (point1.x - point3.x)*(point.y - point3.y)) /
		((point2.y - point3.y)*(point1.x - point3.x) + (point3.x - point2.x)*(point1.y - point3.y));
	float gamma = 1.0f - alpha - beta;

	//The explanation behind this is that a point inside a triangle can be described using the points of the triangle, and three coefficients (one for each point, in the range [0,1]):
	//p = (alpha)*point1 + (beta)*point2 + (gamma)*point3

	//If all of alpha, beta, and gamma are greater than 0, then the point p lies within the triangle made of points p1, p2, and p3.
	if (alpha > 0 && beta > 0 && gamma > 0)
		return true;
	else
		return false;
}
//==============================================================================================================================