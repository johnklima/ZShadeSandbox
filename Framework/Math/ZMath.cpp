#include "ZMath.h"
#include <float.h>
#include <cmath>
#include "XMMath.h"
using ZShadeSandboxMath::ZMath;
//==============================================================================================================================
//==============================================================================================================================
const XMFLOAT2 ZMath::vec2Epsilon = XMFLOAT2(0.00001f, 0.00001f);
const XMFLOAT3 ZMath::vec3Epsilon = XMFLOAT3(0.00001f, 0.00001f, 0.00001f);
//==============================================================================================================================
bool ZMath::CompareVector3WithEpsilon(const XMFLOAT3& lhs, const XMFLOAT3& rhs)
{
	return XMVector3NearEqual(XMLoadFloat3(&lhs), XMLoadFloat3(&rhs), XMLoadFloat3(&vec3Epsilon)) == TRUE;
}
//==============================================================================================================================
bool ZMath::CompareVector2WithEpsilon(const XMFLOAT2& lhs, const XMFLOAT2& rhs)
{
	return XMVector3NearEqual(XMLoadFloat2(&lhs), XMLoadFloat2(&rhs), XMLoadFloat2(&vec2Epsilon)) == TRUE;
}
//==============================================================================================================================
XMMATRIX ZMath::FromRowVectors(XMFLOAT3 m0, XMFLOAT3 m1, XMFLOAT3 m2, XMFLOAT3 m3)
{
	return XMMATRIX
	(
		m0.x, m0.y, m0.z, 0.0f,
		m1.x, m1.y, m1.z, 0.0f,
		m2.x, m2.y, m2.z, 0.0f,
		m3.x, m3.y, m3.z, 0.0f
	);
}
//==================================================================================================================================
bool ZMath::GetIntersect
(	ZShadeSandboxMath::XMMath3 linestart
,	ZShadeSandboxMath::XMMath3 lineend
,	ZShadeSandboxMath::XMMath3 vertex
,	ZShadeSandboxMath::XMMath3 normal
,	ZShadeSandboxMath::XMMath3& intersection
,	float& percentage
)
{
	ZShadeSandboxMath::XMMath3 direction, L1;
	float lineLength, distFromPlane;

	direction.x = lineend.x - linestart.x;
	direction.y = lineend.y - linestart.y;
	direction.z = lineend.z - linestart.z;

	lineLength = direction.Dot(normal);

	if (fabsf(lineLength) < 0.0001f)
	{
		return false;
	}

	L1.x = vertex.x - linestart.x;
	L1.y = vertex.y - linestart.y;
	L1.z = vertex.z - linestart.z;

	distFromPlane = L1.Dot(normal);

	percentage = distFromPlane / lineLength;

	if (percentage < 0.0f)
	{
		return false;
	}
	else if (percentage > 1.0f)
	{
		return false;
	}

	intersection.x = linestart.x + direction.x * percentage;
	intersection.y = linestart.y + direction.y * percentage;
	intersection.z = linestart.z + direction.z * percentage;

	return true;
}
//==============================================================================================================================
XMMATRIX ZMath::MatrixFromNormal(XMFLOAT3 normal)
{
	bool lookingUp = false;

	XMFLOAT3 right = XMFLOAT3(normal.y, -normal.x, 0.0f);

	if (right.x == 0.0f && right.y == 0.0f)
	{
		lookingUp = true;
		right.x = 1.0f;
	}

	XMMATRIX m;

	XMMath3 RcrossN(right);
	RcrossN.Cross(normal);

	if (lookingUp)
	{
		m = FromRowVectors(right, RcrossN, normal, XMFLOAT3(0, 0, 0));
	}
	else
	{
		m = FromRowVectors(RcrossN, right, normal, XMFLOAT3(0, 0, 0));
	}

	return m;
}
//==============================================================================================================================
XMVECTOR ZMath::RandUnitVec3()
{
	XMVECTOR One  = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	XMVECTOR Zero = XMVectorZero();

	// Keep trying until we get a point on/in the hemisphere.
	while(true)
	{
		// Generate random point in the cube [-1,1]^3.
		XMVECTOR v = XMVectorSet(RandF(-1.0f, 1.0f), RandF(-1.0f, 1.0f), RandF(-1.0f, 1.0f), 0.0f);

		// Ignore points outside the unit sphere in order to get an even distribution 
		// over the unit sphere.  Otherwise points will clump more on the sphere near 
		// the corners of the cube.

		if( XMVector3Greater( XMVector3LengthSq(v), One) )
			continue;

		return XMVector3Normalize(v);
	}
}
//==============================================================================================================================
XMVECTOR ZMath::RandHemisphereUnitVec3(XMVECTOR n)
{
	XMVECTOR One  = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	XMVECTOR Zero = XMVectorZero();

	// Keep trying until we get a point on/in the hemisphere.
	while(true)
	{
		// Generate random point in the cube [-1,1]^3.
		XMVECTOR v = XMVectorSet(RandF(-1.0f, 1.0f), RandF(-1.0f, 1.0f), RandF(-1.0f, 1.0f), 0.0f);

		// Ignore points outside the unit sphere in order to get an even distribution 
		// over the unit sphere.  Otherwise points will clump more on the sphere near 
		// the corners of the cube.
		
		if( XMVector3Greater( XMVector3LengthSq(v), One) )
			continue;

		// Ignore points in the bottom hemisphere.
		if( XMVector3Less( XMVector3Dot(n, v), Zero ) )
			continue;

		return XMVector3Normalize(v);
	}
}
//==============================================================================================================================
float ZMath::AngleFromXY(float x, float y)
{
	float theta = 0.0f;

	// Quadrant I or IV
	if (x >= 0.0f)
	{
		// If x = 0, then atanf(y/x) = +pi/2 if y > 0
		//                atanf(y/x) = -pi/2 if y < 0
		theta = atanf(y / x); // in [-pi/2, +pi/2]

		if (theta < 0.0f)
			theta += 2.0f * PI; // in [0, 2*pi).
	}

	// Quadrant II or III
	else
		theta = atanf(y / x) + PI; // in [0, 2*pi).

	return theta;
}
//==============================================================================================================================