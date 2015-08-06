#include "LinePrimitive.h"
#include "XMMath.h"
using ZShadeSandboxMath::LinePrimitive;
//==============================================================================================================================
//==============================================================================================================================
LinePrimitive::LinePrimitive()
{
}
//==============================================================================================================================
LinePrimitive::LinePrimitive(XMFLOAT3 p1, XMFLOAT3 p2)
:   point1(p1)
,   point2(p2)
{
}
//==============================================================================================================================
void LinePrimitive::Set(XMFLOAT3 p1, XMFLOAT3 p2)
{
	point1 = p1;
	point2 = p2;
}
//==============================================================================================================================
XMFLOAT3 LinePrimitive::Point1() const
{
	return point1;
}
//==============================================================================================================================
XMFLOAT3 LinePrimitive::Point2() const
{
	return point2;
}
//==============================================================================================================================
XMFLOAT3 LinePrimitive::Normal()
{
	ZShadeSandboxMath::XMMath3 p(point1);
	p.Cross(point2);
	return p;
}
//==============================================================================================================================
/*ESideOfLine::Type LinePrimitive::Side(XMMath3 point)
{
	Vec3<XMFLOAT3> diff(point2);
	diff -= point1;

	Vec3<XMFLOAT3> perp(-diff.Vector().y, diff.Vector().x, diff.Vector().z);

	Vec3<XMFLOAT3> p(point);
	p -= point1;

	float d = p.Dot(perp);

	if (d == 0)
	{
		return ESideOfLine::ON;
	}
	else if (d < 0)
	{
		return ESideOfLine::LEFT;
	}
	else if (d > 0)
	{
		return ESideOfLine::RIGHT;
	}
}
//==============================================================================================================================
float LinePrimitive::PointDistance(XMMath3 point, bool segment)
{
	float dist = point1.ICross(point2, point) / point1.Magnitude(point2);

	if (segment)
	{
		int dot1 = point1.IDot(point2, point);
		if (dot1 > 0)
			return point2.Magnitude(point);

		int dot2 = point2.IDot(point1, point);
		if (dot2 > 0)
			return point1.Magnitude(point);
	}

	return abs(dist);
}*/
//==============================================================================================================================
bool LinePrimitive::operator == (const LinePrimitive& l)
{
	XMFLOAT3 p11 = point1;// .Vector();
	XMFLOAT3 p12 = point2;// .Vector();

	XMFLOAT3 p21 = l.Point1();// .Vector();
	XMFLOAT3 p22 = l.Point2();// .Vector();

	bool bx = (p11.x == p21.x) && (p12.x == p22.x);
	bool by = (p11.y == p21.y) && (p12.y == p22.y);
	bool bz = (p11.z == p21.z) && (p12.z == p22.z);

	return (bx && by && bz);
}
//==============================================================================================================================