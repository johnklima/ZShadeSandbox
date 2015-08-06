//==============================================================================================================================
// SpherePrimitive.h
//==============================================================================================================================
#ifndef SPHEREPRIMITIVE_H
#define SPHEREPRIMITIVE_H
//==============================================================================================================================
//==============================================================================================================================

#include "ZMath.h"
#include "Vec.h"
#include "ConePrimitive.h"

namespace ZShadeSandboxMath {
	class ConePrimitive;
}

//==============================================================================================================================
//==============================================================================================================================
namespace ZShadeSandboxMath {
class SpherePrimitive
{
	XMFLOAT3 center;
	float radius;
	float iStackCount;
	float iSliceCount;
	
public:
	
	SpherePrimitive();
	SpherePrimitive(XMFLOAT3 c, float r = 100);
	
	void Set(XMFLOAT3 c, float r = 100);
	
	void SetStackCount(float stackCount);
	void SetSliceCount(float sliceCount);

	XMFLOAT3& Center() { return center; }
	float& Radius() { return radius; }

	XMFLOAT3 Center() const;
	float Radius() const;
	float StackCount();
	float SliceCount();

	// tests if ‘this’ sphere intersects another Sphere
	bool Intersects(SpherePrimitive refSphere);

	// The coneVertex is the tip of the cone
	bool IntersectsCone(ZShadeSandboxMath::ConePrimitive cone);
};
}
//==============================================================================================================================
//==============================================================================================================================
#endif// SPHEREPRIMITIVE_H