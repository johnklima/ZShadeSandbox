#include "SpherePrimitive.h"
using ZShadeSandboxMath::SpherePrimitive;
//==============================================================================================================================
//==============================================================================================================================
SpherePrimitive::SpherePrimitive()
:   center(XMFLOAT3(0, 0, 0))
,   radius(0)
,   iStackCount(16)
,   iSliceCount(16)
{
}
//==============================================================================================================================
SpherePrimitive::SpherePrimitive(XMFLOAT3 c, float r) : center(c), radius(r), iStackCount(16), iSliceCount(16)
{
}
//==============================================================================================================================
void SpherePrimitive::Set(XMFLOAT3 c, float r)
{
	center = c;
	radius = r;
}
//==============================================================================================================================
void SpherePrimitive::SetStackCount(float stackCount) { iStackCount = stackCount; }
//==============================================================================================================================
void SpherePrimitive::SetSliceCount(float sliceCount) { iSliceCount = sliceCount; }
//==============================================================================================================================
XMFLOAT3 SpherePrimitive::Center() const { return center; }
//==============================================================================================================================
float SpherePrimitive::Radius() const { return radius; }
//==============================================================================================================================
float SpherePrimitive::StackCount() { return iStackCount; }
//==============================================================================================================================
float SpherePrimitive::SliceCount() { return iSliceCount; }
//==============================================================================================================================
bool SpherePrimitive::Intersects(SpherePrimitive refSphere)
{
	// get the separating axis
	Vec3<XMFLOAT3> c1(center);
	Vec3<XMFLOAT3> c2(refSphere.Center());

	Vec3<XMFLOAT3> vSepAxis = c1 - c2;

	// get the sum of the radii
	float fRadiiSum = radius + refSphere.Radius();

	// if the distance between the centers is less than the sum
	// of the radii, then we have an intersection
	// we calculate this using the squared lengths for speed
	float mag = vSepAxis.Magnitude();
	float squaredMag = mag * mag;
	if (squaredMag < (fRadiiSum * fRadiiSum))
		return true;

	// otherwise they are separated
	return false;
}
//==============================================================================================================================
bool SpherePrimitive::IntersectsCone(ZShadeSandboxMath::ConePrimitive cone)
{
	Vec3<XMFLOAT3> U(cone.GetAxis());
	float f = radius / cone.Sin();
	U *= f;
	U -= cone.Vertex();

	//Vec4<XMFLOAT4> c(center.x, center.y, center.z, center.w);
	Vec3<XMFLOAT3> c(center);
	Vec3<XMFLOAT3> D = c - U;

	Vec3<XMFLOAT3> va(cone.GetAxis());

	if (va.Dot(D) >= D.Magnitude() * cone.Cos())
	{
		// Center is inside K''
		D = c - cone.Vertex();

		if (-va.Dot(D) >= D.Magnitude() * cone.Sin())
		{
			// Center is inside K'' and inside K'
			return D.Magnitude() <= radius;
		}
		else
		{
			// Center is inside K'' and outside K'
			return true;
		}
	}
	else
	{
		// Center is outside K''
		return false;
	}
}
//==============================================================================================================================