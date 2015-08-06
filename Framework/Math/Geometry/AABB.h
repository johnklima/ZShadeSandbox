//==================================================================================================================================
// AABB.h
//
//==================================================================================================================================
// History
//
// -Created on 1/22/2015 by Dustin Watson
//==================================================================================================================================
#ifndef __AABB_H
#define __AABB_H
//==================================================================================================================================
//==================================================================================================================================
#include "ZMath.h"
//==================================================================================================================================
namespace ZShadeSandboxMath {
struct AABB
{
	XMFLOAT3 vMin;
	XMFLOAT3 vMax;
	
	AABB();
	AABB(XMFLOAT3 min, XMFLOAT3 max);
	
	void Construct(XMFLOAT3 center, XMFLOAT3 scale);
	
	bool ContainsPoint2D(XMFLOAT3 p);
	bool ContainsPoint3D(XMFLOAT3 p);
	bool ContainsPoint3DOmitY(XMFLOAT3 p);
	
	float DistanceToPoint2D(XMFLOAT3 p);
	float DistanceToPoint3D(XMFLOAT3 p);

	static AABB Union(const AABB& box1, const AABB& box2);
	
	static bool Overlap(const AABB& box1, const AABB& box2);

	inline XMFLOAT3 Center() const
	{
		return XMFLOAT3(0.5f * (vMin.x + vMax.x), 0.5f * (vMin.y + vMax.y), 0.5f * (vMin.z + vMax.z));
	}
	
	inline XMFLOAT3 Extents() const
	{
		return XMFLOAT3(0.5f * (vMax.x - vMin.x), 0.5f * (vMax.y - vMin.y), 0.5f * (vMax.z - vMin.z));
	}
};
}
//==================================================================================================================================
//==================================================================================================================================
#endif//__AABB_H