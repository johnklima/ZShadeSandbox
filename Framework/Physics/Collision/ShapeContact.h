//==================================================================================================================================
// ShapeContact.h
//
// http://www.randygaul.net/2013/03/28/custom-physics-engine-part-2-manifold-generation/
// https://code.google.com/p/slimdx/source/browse/branches/lite/SlimMath/Collision.cs?r=2185
//==================================================================================================================================
// History
//
// -Created on 12/18/2014 by Dustin Watson
//==================================================================================================================================
#ifndef __SHAPECONTACT_H
#define __SHAPECONTACT_H
//==================================================================================================================================
//==================================================================================================================================
#include "ZMath.h"
#include "ContactManifold.h"
#include "AABB.h"
#include "Ray.h"
#include "PlatformerMap.h"
#include "PlanePrimitive.h"
#include "CirclePrimitive.h"
#include "TrianglePrimitive.h"
#include "LinePrimitive.h"
#include "SpherePrimitive.h"
//==================================================================================================================================
namespace EPlaneIntersectionType
{
	enum Type
	{
		Front,
		Back,
		Intersecting
	};
}
//==================================================================================================================================
namespace EContainmentType
{
	enum Type
	{
		Contains,
		Disjoint,
		Intersects
	};
}
//==================================================================================================================================
// Handles collision contact between different standard shapes
class ShapeContact
{
public:
	
	static ZShadeSandboxMath::XMMath3 NormalFromPoint(ZShadeSandboxMath::AABB box, ZShadeSandboxMath::XMMath3 point, float& distance);
	
	static void ClosestPointOnSegmentToPoint(ZShadeSandboxMath::XMMath3 segment1, ZShadeSandboxMath::XMMath3 segment2, ZShadeSandboxMath::XMMath3 point, ZShadeSandboxMath::XMMath3& result);
	static void ClosestPointOnPlaneToPoint(ZShadeSandboxMath::PlanePrimitive plane, ZShadeSandboxMath::XMMath3 point, ZShadeSandboxMath::XMMath3& result);
	static void ClosestPointOnTriangleToPoint(ZShadeSandboxMath::XMMath3 vertex1, ZShadeSandboxMath::XMMath3 vertex2, ZShadeSandboxMath::XMMath3 vertex3, ZShadeSandboxMath::XMMath3 point, ZShadeSandboxMath::XMMath3& result);
	static void ClosestPointOnTriangleToPoint(ZShadeSandboxMath::TrianglePrimitive tri, ZShadeSandboxMath::XMMath3 point, ZShadeSandboxMath::XMMath3& result);
	static void ClosestPointOnAABBToPoint(ZShadeSandboxMath::AABB box, ZShadeSandboxMath::XMMath3 point, ZShadeSandboxMath::XMMath3& result);
	static void ClosestPointOnSphereToPoint(ZShadeSandboxMath::SpherePrimitive sphere, ZShadeSandboxMath::XMMath3 point, ZShadeSandboxMath::XMMath3& result);
	static void ClosestPointOnSphereToSphere(ZShadeSandboxMath::SpherePrimitive sphere1, ZShadeSandboxMath::SpherePrimitive sphere2, ZShadeSandboxMath::XMMath3& result);
	
	static float DistancePlaneToPoint(ZShadeSandboxMath::PlanePrimitive plane, ZShadeSandboxMath::XMMath3 point);
	static float DistanceAABBToPoint(ZShadeSandboxMath::AABB box, ZShadeSandboxMath::XMMath3 point);
	static float DistanceAABBToAABB(ZShadeSandboxMath::AABB box1, ZShadeSandboxMath::AABB box2);
	static float DistanceSphereToPoint(ZShadeSandboxMath::SpherePrimitive sphere, ZShadeSandboxMath::XMMath3 point);
	static float DistanceSphereToSphere(ZShadeSandboxMath::SpherePrimitive sphere1, ZShadeSandboxMath::SpherePrimitive sphere2);
	
	static bool RayIntersectsPoint(ZShadeSandboxMath::Ray ray, ZShadeSandboxMath::XMMath3 point);
	static bool RayIntersectsRay(ZShadeSandboxMath::Ray ray1, ZShadeSandboxMath::Ray ray2, ZShadeSandboxMath::XMMath3& point);
	static bool RayIntersectsPlane(ZShadeSandboxMath::Ray ray, ZShadeSandboxMath::PlanePrimitive plane, float& distance);
	static bool RayIntersectsPlane(ZShadeSandboxMath::Ray ray, ZShadeSandboxMath::PlanePrimitive plane, ZShadeSandboxMath::XMMath3& point);
	static bool RayIntersectsTriangle(ZShadeSandboxMath::Ray ray, ZShadeSandboxMath::TrianglePrimitive tri, float& distance);
	static bool RayIntersectsTriangle(ZShadeSandboxMath::Ray ray, ZShadeSandboxMath::XMMath3 vertex1, ZShadeSandboxMath::XMMath3 vertex2, ZShadeSandboxMath::XMMath3 vertex3, float& distance);
	static bool RayIntersectsTriangle(ZShadeSandboxMath::Ray ray, ZShadeSandboxMath::TrianglePrimitive tri, ZShadeSandboxMath::XMMath3& point);
	static bool RayIntersectsTriangle(ZShadeSandboxMath::Ray ray, ZShadeSandboxMath::XMMath3 vertex1, ZShadeSandboxMath::XMMath3 vertex2, ZShadeSandboxMath::XMMath3 vertex3, ZShadeSandboxMath::XMMath3& point);
	static bool RayIntersectsAABB(ZShadeSandboxMath::Ray ray, ZShadeSandboxMath::AABB box, float& distance);
	static bool RayIntersectsAABB(ZShadeSandboxMath::Ray ray, ZShadeSandboxMath::AABB box, ZShadeSandboxMath::XMMath3& point);
	static bool RayIntersectsSphere(ZShadeSandboxMath::Ray ray, ZShadeSandboxMath::SpherePrimitive sphere, float& distance);
	static bool RayIntersectsSphere(ZShadeSandboxMath::Ray ray, ZShadeSandboxMath::SpherePrimitive sphere, ZShadeSandboxMath::XMMath3& point);
	static bool RayIntersectsSphere(ZShadeSandboxMath::Ray ray, ZShadeSandboxMath::SpherePrimitive sphere, ZShadeSandboxMath::XMMath3& point, ZShadeSandboxMath::XMMath3& normal);
	static bool RayIntersectsSphere(ZShadeSandboxMath::Ray ray, ZShadeSandboxMath::SpherePrimitive sphere, ZShadeSandboxMath::XMMath3& entrancePoint, ZShadeSandboxMath::XMMath3& entranceNormal, ZShadeSandboxMath::XMMath3& exitPoint, ZShadeSandboxMath::XMMath3& exitNormal);
	
	static EPlaneIntersectionType::Type PlaneIntersectsPoint(ZShadeSandboxMath::PlanePrimitive plane, ZShadeSandboxMath::XMMath3 point);
	static bool PlaneIntersectsPlane(ZShadeSandboxMath::PlanePrimitive plane1, ZShadeSandboxMath::PlanePrimitive plane2);
	static bool PlaneIntersectsPlane(ZShadeSandboxMath::PlanePrimitive plane1, ZShadeSandboxMath::PlanePrimitive plane2, ZShadeSandboxMath::Ray& line);
	static EPlaneIntersectionType::Type PlaneIntersectsTriangle(ZShadeSandboxMath::PlanePrimitive plane, ZShadeSandboxMath::TrianglePrimitive tri);
	static EPlaneIntersectionType::Type PlaneIntersectsTriangle(ZShadeSandboxMath::PlanePrimitive plane, ZShadeSandboxMath::XMMath3 vertex1, ZShadeSandboxMath::XMMath3 vertex2, ZShadeSandboxMath::XMMath3 vertex3);
	static EPlaneIntersectionType::Type PlaneIntersectsAABB(ZShadeSandboxMath::PlanePrimitive plane, ZShadeSandboxMath::AABB box);
	static EPlaneIntersectionType::Type PlaneIntersectsSphere(ZShadeSandboxMath::PlanePrimitive plane, ZShadeSandboxMath::SpherePrimitive sphere);

	static bool AABBIntersectsTriangle(ZShadeSandboxMath::AABB box, ZShadeSandboxMath::TrianglePrimitive tri);
	static bool AABBIntersectsTriangle(ZShadeSandboxMath::AABB box, ZShadeSandboxMath::XMMath3 vertex1, ZShadeSandboxMath::XMMath3 vertex2, ZShadeSandboxMath::XMMath3 vertex3);
	static bool AABBIntersectsAABB(ZShadeSandboxMath::AABB box1, ZShadeSandboxMath::AABB box2);
	static bool AABBIntersectsSphere(ZShadeSandboxMath::AABB box, ZShadeSandboxMath::SpherePrimitive sphere);
	
	static bool SphereIntersectsPoint(ZShadeSandboxMath::SpherePrimitive sphere, ZShadeSandboxMath::XMMath3 point);
	static bool SphereIntersectsTriangle(ZShadeSandboxMath::SpherePrimitive sphere, ZShadeSandboxMath::TrianglePrimitive tri);
	static bool SphereIntersectsTriangle(ZShadeSandboxMath::SpherePrimitive sphere, ZShadeSandboxMath::XMMath3 vertex1, ZShadeSandboxMath::XMMath3 vertex2, ZShadeSandboxMath::XMMath3 vertex3);
	static bool SphereIntersectsSphere(ZShadeSandboxMath::SpherePrimitive sphere1, ZShadeSandboxMath::SpherePrimitive sphere2);

	static EContainmentType::Type AABBContainsPoint(ZShadeSandboxMath::AABB box, ZShadeSandboxMath::XMMath3 point);
	static EContainmentType::Type AABBContainsTriangle(ZShadeSandboxMath::AABB box, ZShadeSandboxMath::TrianglePrimitive tri);
	static EContainmentType::Type AABBContainsTriangle(ZShadeSandboxMath::AABB box, ZShadeSandboxMath::XMMath3 vertex1, ZShadeSandboxMath::XMMath3 vertex2, ZShadeSandboxMath::XMMath3 vertex3);
	static EContainmentType::Type AABBContainsAABB(ZShadeSandboxMath::AABB box1, ZShadeSandboxMath::AABB box2);
	static EContainmentType::Type AABBContainsSphere(ZShadeSandboxMath::AABB box, ZShadeSandboxMath::SpherePrimitive sphere);

	static EContainmentType::Type SphereContainsPoint(ZShadeSandboxMath::SpherePrimitive sphere, ZShadeSandboxMath::XMMath3 point);
	static EContainmentType::Type SphereContainsTriangle(ZShadeSandboxMath::SpherePrimitive sphere, ZShadeSandboxMath::TrianglePrimitive tri);
	static EContainmentType::Type SphereContainsTriangle(ZShadeSandboxMath::SpherePrimitive sphere, ZShadeSandboxMath::XMMath3 vertex1, ZShadeSandboxMath::XMMath3 vertex2, ZShadeSandboxMath::XMMath3 vertex3);
	static EContainmentType::Type SphereContainsAABB(ZShadeSandboxMath::SpherePrimitive sphere, ZShadeSandboxMath::AABB box);
	static EContainmentType::Type SphereContainsSphere(ZShadeSandboxMath::SpherePrimitive sphere1, ZShadeSandboxMath::SpherePrimitive sphere2);

	static void SupportPoint(ZShadeSandboxMath::TrianglePrimitive tri, ZShadeSandboxMath::XMMath3 direction, ZShadeSandboxMath::XMMath3& result);
	static void SupportPoint(ZShadeSandboxMath::XMMath3 vertex1, ZShadeSandboxMath::XMMath3 vertex2, ZShadeSandboxMath::XMMath3 vertex3, ZShadeSandboxMath::XMMath3 direction, ZShadeSandboxMath::XMMath3& result);
	static void SupportPoint(ZShadeSandboxMath::AABB box, ZShadeSandboxMath::XMMath3 direction, ZShadeSandboxMath::XMMath3& result);
	static void SupportPoint(ZShadeSandboxMath::SpherePrimitive sphere, ZShadeSandboxMath::XMMath3 direction, ZShadeSandboxMath::XMMath3& result);
	static void SupportPoint(vector<ZShadeSandboxMath::XMMath3> points, ZShadeSandboxMath::XMMath3 direction, ZShadeSandboxMath::XMMath3& result);
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__SHAPECONTACT_H