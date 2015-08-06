#include "AABB.h"
#include "ZShadeMessageCenter.h"
#include "Scalar.h"
using ZShadeSandboxMath::AABB;
//==================================================================================================================================
//==================================================================================================================================
AABB::AABB()
:   vMin(XMFLOAT3(0, 0, 0))
,   vMax(XMFLOAT3(0, 0, 0))
{
}
//==================================================================================================================================
AABB::AABB(XMFLOAT3 min, XMFLOAT3 max)
:   vMin(min)
,   vMax(max)
{
}
//==================================================================================================================================
void AABB::Construct(XMFLOAT3 center, XMFLOAT3 scale)
{
	vMin = XMFLOAT3(center.x - scale.x, center.y - scale.y, center.z - scale.z);
	vMax = XMFLOAT3(center.x + scale.x, center.y + scale.y, center.z + scale.z);
}
//==================================================================================================================================
bool AABB::ContainsPoint2D(XMFLOAT3 p)
{
	if (p.x >= vMin.x && p.x <= vMax.x &&
		p.y >= vMin.y && p.y <= vMax.y)
		return true;
	return false;
}
//==================================================================================================================================
bool AABB::ContainsPoint3D(XMFLOAT3 p)
{
	if (p.x >= vMin.x && p.x <= vMax.x &&
		p.y >= vMin.y && p.y <= vMax.y &&
		p.z >= vMin.z && p.z <= vMax.z)
		return true;
	return false;
}
//==================================================================================================================================
bool AABB::ContainsPoint3DOmitY(XMFLOAT3 p)
{
	if (p.x >= vMin.x && p.x <= vMax.x &&
		p.z >= vMin.z && p.z <= vMax.z)
		return true;
	return false;
}
//==================================================================================================================================
float AABB::DistanceToPoint2D(XMFLOAT3 p)
{
	float dx = max(vMin.x - p.x, p.x - vMax.x);
	float dy = max(vMin.y - p.y, p.y - vMax.y);
	return sqrt(dx * dx + dy * dy);
}
//==================================================================================================================================
float AABB::DistanceToPoint3D(XMFLOAT3 p)
{
	float dx = max(vMin.x - p.x, p.x - vMax.x);
	float dy = max(vMin.y - p.y, p.y - vMax.y);
	float dz = max(vMin.z - p.z, p.z - vMax.z);
	return sqrt(dx * dx + dy * dy + dz * dz);
}
//==================================================================================================================================
AABB AABB::Union(const AABB& box1, const AABB& box2)
{
	XMFLOAT3 vmin = XMFLOAT3(fminf(box1.vMin.x, box2.vMin.x), fminf(box1.vMin.y, box2.vMin.y), fminf(box1.vMin.z, box2.vMin.z));
	XMFLOAT3 vmax = XMFLOAT3(fmaxf(box1.vMax.x, box2.vMax.x), fmaxf(box1.vMax.y, box2.vMax.y), fmaxf(box1.vMax.z, box2.vMax.z));
	return AABB(vmin, vmax);
}
//==================================================================================================================================
bool AABB::Overlap(const AABB& box1, const AABB& box2)
{
	XMFLOAT3 centerDelta = box2.Center();

	centerDelta.x -= box1.Center().x;
	centerDelta.y -= box1.Center().y;
	centerDelta.z -= box1.Center().z;

	centerDelta.x = ZShadeSandboxMath::ZMath::Abs(centerDelta.x);
	centerDelta.y = ZShadeSandboxMath::ZMath::Abs(centerDelta.y);
	centerDelta.z = ZShadeSandboxMath::ZMath::Abs(centerDelta.z);

	XMFLOAT3 halfExtentsSum = box2.Extents();

	halfExtentsSum.x += box1.Extents().x;
	halfExtentsSum.y += box1.Extents().y;
	halfExtentsSum.z += box1.Extents().z;

	centerDelta.x -= halfExtentsSum.x;
	centerDelta.y -= halfExtentsSum.y;
	centerDelta.z -= halfExtentsSum.z;

	return centerDelta.x < 0 && centerDelta.y < 0 && centerDelta.z < 0;
}
//==================================================================================================================================