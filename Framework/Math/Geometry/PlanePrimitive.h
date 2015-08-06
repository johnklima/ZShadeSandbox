//==============================================================================================================================
// PlanePrimitive.h
//==============================================================================================================================
#ifndef PLANEPRIMITIVE_H
#define PLANEPRIMITIVE_H
//==============================================================================================================================
//==============================================================================================================================
#include "Vec.h"
//==============================================================================================================================
//==============================================================================================================================
namespace ZShadeSandboxMath {
class PlanePrimitive
{
	XMFLOAT3 normal;
	XMFLOAT4 normal4;

	float d;

public:
	
	PlanePrimitive() {}
	PlanePrimitive(XMFLOAT3 normal);
	PlanePrimitive(XMFLOAT3 point1, XMFLOAT3 point2, XMFLOAT3 point3);
	PlanePrimitive(XMFLOAT4 normal);
	PlanePrimitive(XMFLOAT4 point1, XMFLOAT4 point2, XMFLOAT4 point3);

	void CreateDirection(XMFLOAT3 point1, XMFLOAT3 point2, XMFLOAT3 point3);
	void CreateDirection(XMFLOAT4 point1, XMFLOAT4 point2, XMFLOAT4 point3);
	
	float CalculateDistance(float x, float y, float z);
	float CalculateDistance(float x, float y, float z, float w);
	float CalculateDistance(XMFLOAT3 point);
	float CalculateDistance(XMFLOAT4 point);

	// Check to see if a point lies within this plane
	bool PointInPlane(XMFLOAT3 point);
	bool PointInPlane(XMFLOAT4 point);

	bool BehindPlane(XMFLOAT3 point);
	bool BehindPlane(XMFLOAT4 point);

	// Get the normal of the 3D plane
	XMFLOAT3 Normal() const;
	XMFLOAT3& Normal() { return normal; }
	
	// Get the normal of the 4D plane
	XMFLOAT4 Normal4() const;
	XMFLOAT4& Normal4() { return normal4; }
	
	// Get the distance of the plane
	float Distance() const;
	float& Distance() { return d; }
};
}
//==============================================================================================================================
//==============================================================================================================================
#endif// PLANEPRIMITIVE_H