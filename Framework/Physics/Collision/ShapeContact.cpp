#include "ShapeContact.h"
#include "Scalar.h"
#include <stdlib.h>
//==================================================================================================================================
//==================================================================================================================================
ZShadeSandboxMath::XMMath3 ShapeContact::NormalFromPoint(ZShadeSandboxMath::AABB box, ZShadeSandboxMath::XMMath3 point, float& distance)
{
	ZShadeSandboxMath::XMMath3 normal(0, 0, 0);
	ZShadeSandboxMath::XMMath3 center(box.Center());
	float fmin = FLT_MAX;
	
	ZShadeSandboxMath::XMMath3 unitX(1, 0, 0);
	ZShadeSandboxMath::XMMath3 unitY(0, 1, 0);
	ZShadeSandboxMath::XMMath3 unitZ(0, 0, 1);
	
	point -= center;
	
	distance = abs(box.Extents().x - abs(point.x));
	if (distance < fmin)
	{
		fmin = distance;
		normal = unitX * -point.x;
	}
	
	distance = abs(box.Extents().y - abs(point.y));
	if (distance < fmin)
	{
		fmin = distance;
		normal = unitY * -point.y;
	}
	
	distance = abs(box.Extents().z - abs(point.z));
	if (distance < fmin)
	{
		fmin = distance;
		normal = unitZ * -point.z;
	}
	
	distance = fmin;
	
	return normal;
}
//==================================================================================================================================
void ShapeContact::ClosestPointOnSegmentToPoint(ZShadeSandboxMath::XMMath3 segment1, ZShadeSandboxMath::XMMath3 segment2, ZShadeSandboxMath::XMMath3 point, ZShadeSandboxMath::XMMath3& result)
{
	ZShadeSandboxMath::XMMath3 ab = segment2 - segment1;

	float t = (point - segment1).Dot(ab);

	t /= ab.Dot(ab);

	if (t < 0.0f)
		t = 0.0f;

	if (t > 1.0f)
		t = 1.0f;

	result = ZShadeSandboxMath::XMMath3(segment1) + (ab * t);
}
//==================================================================================================================================
void ShapeContact::ClosestPointOnPlaneToPoint(ZShadeSandboxMath::PlanePrimitive plane, ZShadeSandboxMath::XMMath3 point, ZShadeSandboxMath::XMMath3& result)
{
	ZShadeSandboxMath::XMMath3 n(plane.Normal());
	float dot = n.Dot(point);

	float t = dot - plane.Distance();

	result = point - (ZShadeSandboxMath::XMMath3(plane.Normal()) * t);
}
//==================================================================================================================================
void ShapeContact::ClosestPointOnTriangleToPoint(ZShadeSandboxMath::XMMath3 vertex1, ZShadeSandboxMath::XMMath3 vertex2, ZShadeSandboxMath::XMMath3 vertex3, ZShadeSandboxMath::XMMath3 point, ZShadeSandboxMath::XMMath3& result)
{
	// Check if the point is in vertex region outside A
	ZShadeSandboxMath::XMMath3 ab = vertex2 - vertex1;
	ZShadeSandboxMath::XMMath3 ac = vertex3 - vertex1;
	ZShadeSandboxMath::XMMath3 ap = point - vertex1;
	
	float d1 = ab.Dot(ap);
	float d2 = ac.Dot(ap);
	
	if (d1 <= 0.0f && d2 <= 0.0f)
	{
		// Barycentric coordinates (1, 0, 0)
		result = vertex1;
		return;
	}
	
	// Check if the point is in vertex region outside B
	ZShadeSandboxMath::XMMath3 bp = point - vertex2;
	
	float d3 = ab.Dot(bp);
	float d4 = ac.Dot(bp);
	
	if (d3 >= 0.0f && d4 <= d3)
	{
		// Barycentric coordinates (0, 1, 0)
		result = vertex2;
		return;
	}
	
	// Check if the point in edge region of AB
	// If it is then get the projection of the point onto AB
	float vc = d1 * d4 - d3 * d2;
	
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		float v = d1 / (d1 - d3);
		// Barycentric coordinates (1 - v, v, 0)
		result = vertex1 + (ab * v);
		return;
	}
	
	// Check if the point in the vertex region is outside C
	ZShadeSandboxMath::XMMath3 cp = point - vertex3;
	
	float d5 = ab.Dot(cp);
	float d6 = ac.Dot(cp);
	
	if (d6 >= 0.0f && d5 <= d6)
	{
		// Barycentric coordinates (0, 0, 1)
		result = vertex3;
		return;
	}
	
	// Check if the point in edge region of AC
	// If it is then get the projection of the point onto AC
	float vb = d5 * d2 - d1 * d6;
	
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		float w = d2 / (d2 - d6);
		// Barycentric coordinates (1 - w, 0, w)
		result = vertex1 + (ac * w);
		return;
	}
	
	// Check if the point in edge region of BC
	// If it is then get the projection of the point onto BC
	float va = d3 * d6 - d5 * d4;
	
	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) <= 0.0f)
	{
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		// Barycentric coordinates (0, 1 - w, w)
		result = vertex2 + ((vertex3 - vertex2) * w);
		return;
	}
	
	// The point is inside the face region. Compute Q through its barycentric coordinates
	float denom = 1.0f / (va + vb + vc);
	float v2 = vb * denom;
	float w2 = vc * denom;
	
	// u * vertex1 + v * vertex2 + w * vertex3
	// u = va * denom = 1.0f - v - w
	result = vertex1 + (ab * v2) + (ac * w2);
}
//==================================================================================================================================
void ShapeContact::ClosestPointOnTriangleToPoint(ZShadeSandboxMath::TrianglePrimitive tri, ZShadeSandboxMath::XMMath3 point, ZShadeSandboxMath::XMMath3& result)
{
	ClosestPointOnTriangleToPoint(tri.Point1(), tri.Point2(), tri.Point3(), point, result);
}
//==================================================================================================================================
void ShapeContact::ClosestPointOnAABBToPoint(ZShadeSandboxMath::AABB box, ZShadeSandboxMath::XMMath3 point, ZShadeSandboxMath::XMMath3& result)
{
	ZShadeSandboxMath::XMMath3 temp(point);
	temp = temp.Max(box.vMin);
	result = temp.Min(box.vMax);
}
//==================================================================================================================================
void ShapeContact::ClosestPointOnSphereToPoint(ZShadeSandboxMath::SpherePrimitive sphere, ZShadeSandboxMath::XMMath3 point, ZShadeSandboxMath::XMMath3& result)
{
	// Get unit direction from the sphere's center to the point
	result = point - ZShadeSandboxMath::XMMath3(sphere.Center());
	result.NormalizeMe();
	
	// Make the vector the length of the sphere
	result *= sphere.Radius();
	
	// Return the point on the sphere
	result += ZShadeSandboxMath::XMMath3(sphere.Center());
}
//==================================================================================================================================
void ShapeContact::ClosestPointOnSphereToSphere(ZShadeSandboxMath::SpherePrimitive sphere1, ZShadeSandboxMath::SpherePrimitive sphere2, ZShadeSandboxMath::XMMath3& result)
{
	// Get unit direction from the 1st sphere's center to the 2nd sphere's center
	result = ZShadeSandboxMath::XMMath3(sphere2.Center()) - ZShadeSandboxMath::XMMath3(sphere1.Center());
	result.NormalizeMe();
	
	// Make the vector the length of the 1st sphere
	result *= sphere1.Radius();
	
	// Return the point on the 1st sphere
	result += ZShadeSandboxMath::XMMath3(sphere1.Center());
}
//==================================================================================================================================
float ShapeContact::DistancePlaneToPoint(ZShadeSandboxMath::PlanePrimitive plane, ZShadeSandboxMath::XMMath3 point)
{
	float dot = ZShadeSandboxMath::XMMath3(plane.Normal()).Dot(point);
	
	return dot - plane.Distance();
}
//==================================================================================================================================
float ShapeContact::DistanceAABBToPoint(ZShadeSandboxMath::AABB box, ZShadeSandboxMath::XMMath3 point)
{
	float distance = 0.0f;
	
	if (point.x < box.vMin.x) distance += SQR(box.vMin.x - point.x);
	if (point.x > box.vMax.x) distance += SQR(point.x - box.vMax.x);
	
	if (point.y < box.vMin.y) distance += SQR(box.vMin.y - point.y);
	if (point.y > box.vMax.y) distance += SQR(point.y - box.vMax.y);
	
	if (point.z < box.vMin.z) distance += SQR(box.vMin.z - point.z);
	if (point.z > box.vMax.z) distance += SQR(point.z - box.vMax.z);
	
	return (float)sqrt(distance);
}
//==================================================================================================================================
float ShapeContact::DistanceAABBToAABB(ZShadeSandboxMath::AABB box1, ZShadeSandboxMath::AABB box2)
{
	float distance = 0.0f;
	
	if (box1.vMin.x > box2.vMax.x)
	{
		float delta = box2.vMax.x - box1.vMin.x;
		distance += SQR(delta);
	}
	else if (box2.vMin.x > box1.vMax.x)
	{
		float delta = box1.vMax.x - box2.vMin.x;
		distance += SQR(delta);
	}
	
	if (box1.vMin.y > box2.vMax.y)
	{
		float delta = box2.vMax.y - box1.vMin.y;
		distance += SQR(delta);
	}
	else if (box2.vMin.y > box1.vMax.y)
	{
		float delta = box1.vMax.y - box2.vMin.y;
		distance += SQR(delta);
	}
	
	if (box1.vMin.z > box2.vMax.z)
	{
		float delta = box2.vMax.z - box1.vMin.z;
		distance += SQR(delta);
	}
	else if (box2.vMin.z > box1.vMax.z)
	{
		float delta = box1.vMax.z - box2.vMin.z;
		distance += SQR(delta);
	}
	
	return (float)sqrt(distance);
}
//==================================================================================================================================
float ShapeContact::DistanceSphereToPoint(ZShadeSandboxMath::SpherePrimitive sphere, ZShadeSandboxMath::XMMath3 point)
{
	float distance = 0.0f;
	ZShadeSandboxMath::XMMath3 c(sphere.Center());

	distance = c.Distance(XMFLOAT3(point.x, point.y, point.z));
	
	distance -= sphere.Radius();
	
	return max(distance, 0.0f);
}
//==================================================================================================================================
float ShapeContact::DistanceSphereToSphere(ZShadeSandboxMath::SpherePrimitive sphere1, ZShadeSandboxMath::SpherePrimitive sphere2)
{
	float distance = 0.0f;
	ZShadeSandboxMath::XMMath3 c(sphere1.Center());

	distance = c.Distance(sphere2.Center());
	
	distance -= (sphere1.Radius() + sphere2.Radius());
	
	return max(distance, 0.0f);
}
//==================================================================================================================================
bool ShapeContact::RayIntersectsPoint(ZShadeSandboxMath::Ray ray, ZShadeSandboxMath::XMMath3 point)
{
	ZShadeSandboxMath::XMMath3 m = ZShadeSandboxMath::XMMath3(ray.position) - point;
	
	float b = m.Dot(ray.direction);
	float c = m.Dot(m) - 1e-6f;
	
	if (c > 0.0f && b > 0.0f)
	{
		return false;
	}

	float discriminant = b * b - c;

	if (discriminant < 0.0f)
	{
		return false;
	}

	return true;
}
//==================================================================================================================================
bool ShapeContact::RayIntersectsRay(ZShadeSandboxMath::Ray ray1, ZShadeSandboxMath::Ray ray2, ZShadeSandboxMath::XMMath3& point)
{
	ZShadeSandboxMath::XMMath3 cross(ray1.direction);
	cross.Cross(ray2.direction);
	
	float denominator = cross.Length();
	
	// The Lines are parallel
	if (abs(denominator) < 1e-6f)
	{
		// Lines are parallel and on top of each other
		if (abs(ray2.position.x - ray1.position.x) < 1e-6f &&
			abs(ray2.position.y - ray1.position.y) < 1e-6f &&
			abs(ray2.position.z - ray1.position.z) < 1e-6f)
		{
			point = XMFLOAT3(0, 0, 0);
			
			return true;
		}
	}
	
	denominator = SQR(denominator);
	
	// 3x3 matrix for the 1st ray
	float m11 = ray2.position.x - ray1.position.x;
	float m12 = ray2.position.y - ray1.position.y;
	float m13 = ray2.position.z - ray1.position.z;
	float m21 = ray2.direction.x;
	float m22 = ray2.direction.y;
	float m23 = ray2.direction.z;
	float m31 = cross.x;
	float m32 = cross.y;
	float m33 = cross.z;
	
	// Determinant of the 1st matrix
	float dets = m11 * m22 * m33 + m12 * m23 * m31 + m13 * m21 * m32 - m11 * m23 * m32 - m12 * m21 * m33 - m13 * m22 * m31;
	
	// 3x3 matrix for the 2nd ray
	m21 = ray1.direction.x;
	m22 = ray1.direction.y;
	m23 = ray1.direction.z;
	
	// Determinant of the 2nd matrix
	float dett = m11 * m22 * m33 + m12 * m23 * m31 + m13 * m21 * m32 - m11 * m23 * m32 - m12 * m21 * m33 - m13 * m22 * m31;
	
	// t values for the point of intersection
	float s = dets / denominator;
	float t = dett / denominator;
	
	// Points of intersection
	ZShadeSandboxMath::XMMath3 point1 = ZShadeSandboxMath::XMMath3(ray1.position) + (ZShadeSandboxMath::XMMath3(ray1.direction) * s);
	ZShadeSandboxMath::XMMath3 point2 = ZShadeSandboxMath::XMMath3(ray2.position) + (ZShadeSandboxMath::XMMath3(ray2.direction) * t);
	
	// if the points are not equal, then nothing has intersected
	if (abs(point2.x - point1.x) > 1e-6f &&
		abs(point2.y - point1.y) > 1e-6f &&
		abs(point2.z - point1.z) > 1e-6f)
	{
		point = XMFLOAT3(0, 0, 0);
		
		return false;
	}
	
	// The points are the same
	point = point1;
	
	return true;
}
//==================================================================================================================================
bool ShapeContact::RayIntersectsPlane(ZShadeSandboxMath::Ray ray, ZShadeSandboxMath::PlanePrimitive plane, float& distance)
{
	float direction = ZShadeSandboxMath::XMMath3(plane.Normal()).Dot(ray.direction);
	
	if (abs(direction) < 1e-6f)
	{
		distance = 0.0f;
		return false;
	}
	
	float position = ZShadeSandboxMath::XMMath3(plane.Normal()).Dot(ray.position);
	
	distance = (-plane.Distance() - position) / direction;
	
	if (distance < 0.0f)
	{
		if (distance < -1e-6f)
		{
			distance = 0.0f;
			return false;
		}
		
		distance = 0.0f;
	}
	
	return true;
}
//==================================================================================================================================
bool ShapeContact::RayIntersectsPlane(ZShadeSandboxMath::Ray ray, ZShadeSandboxMath::PlanePrimitive plane, ZShadeSandboxMath::XMMath3& point)
{
	float distance;
	
	if (!RayIntersectsPlane(ray, plane, distance))
	{
		point = XMFLOAT3(0, 0, 0);
		return false;
	}
	
	point = ZShadeSandboxMath::XMMath3(ray.position) + (ZShadeSandboxMath::XMMath3(ray.direction) * distance);
	
	return true;
}
//==================================================================================================================================
bool ShapeContact::RayIntersectsTriangle(ZShadeSandboxMath::Ray ray, ZShadeSandboxMath::TrianglePrimitive tri, float& distance)
{
	return RayIntersectsTriangle(ray, tri.Point1(), tri.Point2(), tri.Point3(), distance);
}
//==================================================================================================================================
bool ShapeContact::RayIntersectsTriangle(ZShadeSandboxMath::Ray ray, ZShadeSandboxMath::XMMath3 vertex1, ZShadeSandboxMath::XMMath3 vertex2, ZShadeSandboxMath::XMMath3 vertex3, float& distance)
{
	// Find the two vectors along two edges of the triangle
	ZShadeSandboxMath::XMMath3 edge1 = vertex2 - vertex1;
	ZShadeSandboxMath::XMMath3 edge2 = vertex3 - vertex1;
	
	// Compute the determinant
	ZShadeSandboxMath::XMMath3 directionCrossEdge2(ray.direction);
	directionCrossEdge2.Cross(edge2);
	float determinant = (edge1.x * directionCrossEdge2.x) + (edge1.y * directionCrossEdge2.y) + (edge1.z * directionCrossEdge2.z);
	
	// If the ray is parallel to the triangle plane, no collision has occurred
	// With no culling the ray might hit the front or back of the triangle
	if (determinant > -1e-6f && determinant < 1e-6f)
	{
		distance = 0.0f;
		return false;
	}
	
	float inverseDeterminant = 1.0f / determinant;
	
	// Find the U parameter of the intersection point
	ZShadeSandboxMath::XMMath3 distanceVector = ZShadeSandboxMath::XMMath3(ray.position) - vertex1;
	
	float triangleU = (distanceVector.x * directionCrossEdge2.x) + (distanceVector.y * directionCrossEdge2.y) + (distanceVector.z * directionCrossEdge2.z);
	
	triangleU *= inverseDeterminant;
	
	// Make sure it is inside the triangle
	if (triangleU < 0.0f || triangleU > 1.0f)
	{
		distance = 0.0f;
		return false;
	}
	
	// Find the V parameter of the intersection point
	ZShadeSandboxMath::XMMath3 distanceCrossEdge1(distanceVector);
	distanceCrossEdge1.Cross(edge1);
	
	float triangleV = (ray.direction.x * distanceCrossEdge1.x) + (ray.direction.y * distanceCrossEdge1.y) + (ray.direction.z * distanceCrossEdge1.z);
	
	triangleV *= inverseDeterminant;
	
	// Make sure it is inside the triangle
	if (triangleV < 0.0f || triangleU + triangleV > 1.0f)
	{
		distance = 0.0f;
		return false;
	}
	
	// Compute the distance along the ray to the triangle
	float rayDistance = (edge2.x * distanceCrossEdge1.x) + (edge2.y * distanceCrossEdge1.y) + (edge2.z * distanceCrossEdge1.z);
	
	rayDistance *= inverseDeterminant;
	
	// Is the triangle behind the ray origin
	if (triangleV < 0.0f)
	{
		distance = 0.0f;
		return false;
	}
	
	distance = rayDistance;
	
	return true;
}
//==================================================================================================================================
bool ShapeContact::RayIntersectsTriangle(ZShadeSandboxMath::Ray ray, ZShadeSandboxMath::TrianglePrimitive tri, ZShadeSandboxMath::XMMath3& point)
{
	float distance;
	
	if (!RayIntersectsTriangle(ray, tri, distance))
	{
		point = XMFLOAT3(0, 0, 0);
		return false;
	}
	
	point = ZShadeSandboxMath::XMMath3(ray.position) + (ZShadeSandboxMath::XMMath3(ray.direction) * distance);
	
	return true;
}
//==================================================================================================================================
bool ShapeContact::RayIntersectsTriangle(ZShadeSandboxMath::Ray ray, ZShadeSandboxMath::XMMath3 vertex1, ZShadeSandboxMath::XMMath3 vertex2, ZShadeSandboxMath::XMMath3 vertex3, ZShadeSandboxMath::XMMath3& point)
{
	float distance;
	
	if (!RayIntersectsTriangle(ray, vertex1, vertex2, vertex2, distance))
	{
		point = XMFLOAT3(0, 0, 0);
		return false;
	}
	
	point = ZShadeSandboxMath::XMMath3(ray.position) + (ZShadeSandboxMath::XMMath3(ray.direction) * distance);
	
	return true;
}
//==================================================================================================================================
bool ShapeContact::RayIntersectsAABB(ZShadeSandboxMath::Ray ray, ZShadeSandboxMath::AABB box, float& distance)
{
	/*distance = 0.0f;
	
	float tmax = FLT_MAX;

	if (abs(ray.direction.x) < 1e-6f)
	{
		if (ray.position.x < box.vMin.x || ray.position.x > box.vMax.x)
		{
			distance = 0.0f;
			return false;
		}
	}
	else
	{
		float inverse = 1.0f / ray.direction.x;
		float t1 = (box.vMin.x - ray.position.x) * inverse;
		float t2 = (box.vMax.x - ray.position.x) * inverse;

		if (t1 > t2)
		{
			float temp = t1;
			t1 = t2;
			t2 = temp;
		}

		distance = max(t1, distance);
		tmax = min(t2, tmax);

		if (distance > tmax)
		{
			distance = 0.0f;
			return false;
		}
	}

	if (abs(ray.direction.y) < 1e-6f)
	{
		if (ray.position.y < box.vMin.y || ray.position.y > box.vMax.y)
		{
			distance = 0.0f;
			return false;
		}
	}
	else
	{
		float inverse = 1.0f / ray.direction.y;
		float t1 = (box.vMin.y - ray.position.y) * inverse;
		float t2 = (box.vMax.y - ray.position.y) * inverse;

		if (t1 > t2)
		{
			float temp = t1;
			t1 = t2;
			t2 = temp;
		}

		distance = max(t1, distance);
		tmax = min(t2, tmax);

		if (distance > tmax)
		{
			distance = 0.0f;
			return false;
		}
	}

	if (abs(ray.direction.z) < 1e-6f)
	{
		if (ray.position.z < box.vMin.z || ray.position.z > box.vMax.z)
		{
			distance = 0.0f;
			return false;
		}
	}
	else
	{
		float inverse = 1.0f / ray.direction.z;
		float t1 = (box.vMin.z - ray.position.z) * inverse;
		float t2 = (box.vMax.z - ray.position.z) * inverse;

		if (t1 > t2)
		{
			float temp = t1;
			t1 = t2;
			t2 = temp;
		}

		distance = max(t1, distance);
		tmax = min(t2, tmax);

		if (distance > tmax)
		{
			distance = 0.0f;
			return false;
		}
	}*/
	
	// This is the form of the test used by all fast ray tracers.
	// Possible zero direction components are handled correctly on IEEE-float compliant hardware.

	float rcp_dir_x = 1.0f / ray.direction.x;
	float rcp_dir_y = 1.0f / ray.direction.y;
	float rcp_dir_z = 1.0f / ray.direction.z;

	float tx_min = (box.vMin.x - ray.position.x) * rcp_dir_x;
	float ty_min = (box.vMin.y - ray.position.y) * rcp_dir_y;
	float tz_min = (box.vMin.z - ray.position.z) * rcp_dir_z;
	float tx_max = (box.vMax.x - ray.position.x) * rcp_dir_x;
	float ty_max = (box.vMax.y - ray.position.y) * rcp_dir_y;
	float tz_max = (box.vMax.z - ray.position.z) * rcp_dir_z;

	float tx_near = fminf(tx_min, tx_max);
	float tx_far = fmaxf(tx_min, tx_max);
	float ty_near = fminf(ty_min, ty_max);
	float ty_far = fmaxf(ty_min, ty_max);
	float tz_near = fminf(tz_min, tz_max);
	float tz_far = fmaxf(tz_min, tz_max);

	// If intersecting a finite line segment of length t, replace FLT_MAX with t.
	float tmin = fmaxf(fmaxf(tx_near, ty_near), fmaxf(tz_near, 0.0f));
	float tmax = fminf(fminf(tx_far, ty_far), fminf(tz_far, FLT_MAX));

	distance = tmin;

	return tmin <= tmax;
}
//==================================================================================================================================
bool ShapeContact::RayIntersectsAABB(ZShadeSandboxMath::Ray ray, ZShadeSandboxMath::AABB box, ZShadeSandboxMath::XMMath3& point)
{
	float distance;
	
	if (!RayIntersectsAABB(ray, box, distance))
	{
		point = XMFLOAT3(0, 0, 0);
		return false;
	}
	
	point = ZShadeSandboxMath::XMMath3(ray.position) + (ZShadeSandboxMath::XMMath3(ray.direction) * distance);
	
	return true;
}
//==================================================================================================================================
bool ShapeContact::RayIntersectsSphere(ZShadeSandboxMath::Ray ray, ZShadeSandboxMath::SpherePrimitive sphere, float& distance)
{
	ZShadeSandboxMath::XMMath3 v = ZShadeSandboxMath::XMMath3(ray.position) - ZShadeSandboxMath::XMMath3(sphere.Center());
	
	float b = v.Dot(ray.direction);
	float c = v.Dot(v) - SQR(sphere.Radius());

	if (c > 0.0f && b > 0.0f)
	{
		distance = 0.0f;
		return false;
	}

	float discriminant = b * b - c;

	if (discriminant < 0.0f)
	{
		distance = 0.0f;
		return false;
	}

	distance = -b - (float)sqrt(discriminant);

	if (distance < 0.0f)
		distance = 0.0f;

	return true;
}
//==================================================================================================================================
bool ShapeContact::RayIntersectsSphere(ZShadeSandboxMath::Ray ray, ZShadeSandboxMath::SpherePrimitive sphere, ZShadeSandboxMath::XMMath3& point)
{
	float distance;
	
	if (!RayIntersectsSphere(ray, sphere, distance))
	{
		point = XMFLOAT3(0, 0, 0);
		return false;
	}
	
	point = ZShadeSandboxMath::XMMath3(ray.position) + (ZShadeSandboxMath::XMMath3(ray.direction) * distance);
	
	return true;
}
//==================================================================================================================================
bool ShapeContact::RayIntersectsSphere(ZShadeSandboxMath::Ray ray, ZShadeSandboxMath::SpherePrimitive sphere, ZShadeSandboxMath::XMMath3& point, ZShadeSandboxMath::XMMath3& normal)
{
	float distance;
	
	if (!RayIntersectsSphere(ray, sphere, distance))
	{
		point = XMFLOAT3(0, 0, 0);
		normal = XMFLOAT3(0, 0, 0);
		return false;
	}
	
	point = ZShadeSandboxMath::XMMath3(ray.position) + (ZShadeSandboxMath::XMMath3(ray.direction) * distance);
	
	normal = point - ZShadeSandboxMath::XMMath3(sphere.Center());
	normal.NormalizeMe();
	
	return true;
}
//==================================================================================================================================
bool ShapeContact::RayIntersectsSphere(ZShadeSandboxMath::Ray ray, ZShadeSandboxMath::SpherePrimitive sphere, ZShadeSandboxMath::XMMath3& entrancePoint, ZShadeSandboxMath::XMMath3& entranceNormal, ZShadeSandboxMath::XMMath3& exitPoint, ZShadeSandboxMath::XMMath3& exitNormal)
{
	ZShadeSandboxMath::XMMath3 v = ZShadeSandboxMath::XMMath3(ray.position) - ZShadeSandboxMath::XMMath3(sphere.Center());
	
	float b = v.Dot(ray.direction);
	float c = v.Dot(v) - SQR(sphere.Radius());

	if (c > 0.0f && b > 0.0f)
	{
		entrancePoint = XMFLOAT3(0, 0, 0);
		entranceNormal = XMFLOAT3(0, 0, 0);
		exitPoint = XMFLOAT3(0, 0, 0);
		exitNormal = XMFLOAT3(0, 0, 0);
		return false;
	}

	float discriminant = b * b - c;

	if (discriminant < 0.0f)
	{
		entrancePoint = XMFLOAT3(0, 0, 0);
		entranceNormal = XMFLOAT3(0, 0, 0);
		exitPoint = XMFLOAT3(0, 0, 0);
		exitNormal = XMFLOAT3(0, 0, 0);
		return false;
	}
	
	float discriminantSquared = (float)sqrt(discriminant);
	float distance1 = -b - discriminantSquared;
	float distance2 = -b + discriminantSquared;

	if (distance1 < 0.0f)
	{
		distance1 = 0.0f;
		entrancePoint = XMFLOAT3(0, 0, 0);
		entranceNormal = XMFLOAT3(0, 0, 0);
	}
	else
	{
		entrancePoint = ZShadeSandboxMath::XMMath3(ray.position) + (ZShadeSandboxMath::XMMath3(ray.direction) * distance1);
		entranceNormal = entrancePoint - ZShadeSandboxMath::XMMath3(sphere.Center());
		entranceNormal.NormalizeMe();
	}
	
	exitPoint = ZShadeSandboxMath::XMMath3(ray.position) + (ZShadeSandboxMath::XMMath3(ray.direction) * distance2);
	exitNormal = exitPoint - ZShadeSandboxMath::XMMath3(sphere.Center());
	exitNormal.NormalizeMe();

	return true;
}
//==================================================================================================================================
EPlaneIntersectionType::Type ShapeContact::PlaneIntersectsPoint(ZShadeSandboxMath::PlanePrimitive plane, ZShadeSandboxMath::XMMath3 point)
{
	float distance = ZShadeSandboxMath::XMMath3(plane.Normal()).Dot(point);
	
	distance += plane.Distance();
	
	if (distance > 0.0f)
	{
		return EPlaneIntersectionType::Front;
	}
	
	if (distance < 0.0f)
	{
		return EPlaneIntersectionType::Back;
	}
	
	return EPlaneIntersectionType::Intersecting;
}
//==================================================================================================================================
bool ShapeContact::PlaneIntersectsPlane(ZShadeSandboxMath::PlanePrimitive plane1, ZShadeSandboxMath::PlanePrimitive plane2)
{
	ZShadeSandboxMath::XMMath3 direction(plane1.Normal());
	direction.Cross(ZShadeSandboxMath::XMMath3(plane2.Normal()));
	
	// If direction is a zero vector, the planes are parallel and could be coincident.
	float denominator = direction.Dot(direction);
	
	if (abs(denominator) < 1e-6f)
		return false;
	
	return true;
}
//==================================================================================================================================
bool ShapeContact::PlaneIntersectsPlane(ZShadeSandboxMath::PlanePrimitive plane1, ZShadeSandboxMath::PlanePrimitive plane2, ZShadeSandboxMath::Ray& line)
{
	ZShadeSandboxMath::XMMath3 direction(plane1.Normal());
	direction.Cross(ZShadeSandboxMath::XMMath3(plane2.Normal()));
	
	// If direction is a zero vector, the planes are parallel and could be coincident.
	float denominator = direction.Dot(direction);
	
	//We assume the planes are normalized, therefore the denominator
	//only serves as a parallel and coincident check. Otherwise we need
	//to divide the point by the denominator.
	if (abs(denominator) < 1e-6f)
	{
		line.position = XMFLOAT3(0, 0, 0);
		ZShadeSandboxMath::XMMath3 dir(XMFLOAT3(0, 0, 0));
		dir.NormalizeMe();
		line.direction = dir;
		return false;
	}
	
	ZShadeSandboxMath::XMMath3 temp = (ZShadeSandboxMath::XMMath3(plane2.Normal()) * plane1.Distance()) - (ZShadeSandboxMath::XMMath3(plane1.Normal()) * plane2.Distance());
	ZShadeSandboxMath::XMMath3 point(temp);
	point.Cross(direction);

	line.position = XMFLOAT3(point.x, point.y, point.z);
	
	ZShadeSandboxMath::XMMath3 dir(XMFLOAT3(direction.x, direction.y, direction.z));
	dir.NormalizeMe();
	line.direction = dir;
	
	return true;
}
//==================================================================================================================================
EPlaneIntersectionType::Type ShapeContact::PlaneIntersectsTriangle(ZShadeSandboxMath::PlanePrimitive plane, ZShadeSandboxMath::TrianglePrimitive tri)
{
	return PlaneIntersectsTriangle(plane, tri.Point1(), tri.Point2(), tri.Point3());
}
//==================================================================================================================================
EPlaneIntersectionType::Type ShapeContact::PlaneIntersectsTriangle(ZShadeSandboxMath::PlanePrimitive plane, ZShadeSandboxMath::XMMath3 vertex1, ZShadeSandboxMath::XMMath3 vertex2, ZShadeSandboxMath::XMMath3 vertex3)
{
	EPlaneIntersectionType::Type test1 = PlaneIntersectsPoint(plane, vertex1);
	EPlaneIntersectionType::Type test2 = PlaneIntersectsPoint(plane, vertex2);
	EPlaneIntersectionType::Type test3 = PlaneIntersectsPoint(plane, vertex3);
	
	if (test1 == EPlaneIntersectionType::Front && test2 == EPlaneIntersectionType::Front && test3 == EPlaneIntersectionType::Front)
		return EPlaneIntersectionType::Front;
	
	if (test1 == EPlaneIntersectionType::Back && test2 == EPlaneIntersectionType::Back && test3 == EPlaneIntersectionType::Back)
		return EPlaneIntersectionType::Back;
	
	return EPlaneIntersectionType::Intersecting;
}
//==================================================================================================================================
EPlaneIntersectionType::Type ShapeContact::PlaneIntersectsAABB(ZShadeSandboxMath::PlanePrimitive plane, ZShadeSandboxMath::AABB box)
{
	ZShadeSandboxMath::XMMath3 bmin;
	ZShadeSandboxMath::XMMath3 bmax;
	
	ZShadeSandboxMath::XMMath3 normal(plane.Normal());
	
	bmax.x = (normal.x >= 0.0f) ? box.vMin.x : box.vMax.x;
	bmax.y = (normal.y >= 0.0f) ? box.vMin.y : box.vMax.y;
	bmax.z = (normal.z >= 0.0f) ? box.vMin.z : box.vMax.z;
	bmin.x = (normal.x >= 0.0f) ? box.vMax.x : box.vMin.x;
	bmin.y = (normal.y >= 0.0f) ? box.vMax.y : box.vMin.y;
	bmin.z = (normal.z >= 0.0f) ? box.vMax.z : box.vMin.z;
	
	float distance = normal.Dot(bmax);
	if (distance + plane.Distance() > 0.0f)
		return EPlaneIntersectionType::Front;
	
	distance = normal.Dot(bmin);
	if (distance + plane.Distance() < 0.0f)
		return EPlaneIntersectionType::Back;
	
	return EPlaneIntersectionType::Intersecting;
}
//==================================================================================================================================
EPlaneIntersectionType::Type ShapeContact::PlaneIntersectsSphere(ZShadeSandboxMath::PlanePrimitive plane, ZShadeSandboxMath::SpherePrimitive sphere)
{
	ZShadeSandboxMath::XMMath3 normal(plane.Normal());
	ZShadeSandboxMath::XMMath3 center(sphere.Center());

	float distance = normal.Dot(center);
	distance += plane.Distance();
	
	if (distance > sphere.Radius())
		return EPlaneIntersectionType::Front;
	
	if (distance < -sphere.Radius())
		return EPlaneIntersectionType::Back;
	
	return EPlaneIntersectionType::Intersecting;
}
//==================================================================================================================================
bool ShapeContact::AABBIntersectsTriangle(ZShadeSandboxMath::AABB box, ZShadeSandboxMath::TrianglePrimitive tri)
{
	return AABBIntersectsTriangle(box, tri.Point1(), tri.Point2(), tri.Point3());
}
//==================================================================================================================================
bool ShapeContact::AABBIntersectsTriangle(ZShadeSandboxMath::AABB box, ZShadeSandboxMath::XMMath3 vertex1, ZShadeSandboxMath::XMMath3 vertex2, ZShadeSandboxMath::XMMath3 vertex3)
{
	// This is incomplete
	//Source: Real-Time Collision Detection by Christer Ericson
	//Reference: Page 169
	
	float p0, p1, p2, r;
	
	// Compute box center and extents
	ZShadeSandboxMath::XMMath3 center = (ZShadeSandboxMath::XMMath3(box.vMin) - ZShadeSandboxMath::XMMath3(box.vMax)) * 0.5f;
	
	float e0 = (box.vMax.x - box.vMin.x) * 0.5f;
	float e1 = (box.vMax.y - box.vMin.y) * 0.5f;
	float e2 = (box.vMax.z - box.vMin.z) * 0.5f;
	
	// Translate the triangle as conceptually moving ZShadeSandboxMath::AABB to origin
	vertex1 = vertex1 - center;
	vertex2 = vertex2 - center;
	vertex3 = vertex3 - center;
	
	// Edge vectors the the triangle
	ZShadeSandboxMath::XMMath3 f0 = vertex2 - vertex1;
	ZShadeSandboxMath::XMMath3 f1 = vertex3 - vertex2;
	ZShadeSandboxMath::XMMath3 f2 = vertex1 - vertex3;
	
	// Test axis a00..a22
	p0 = vertex1.z * vertex2.y - vertex1.y * vertex2.z;
	p2 = vertex3.z * (vertex2.y - vertex1.y) - vertex3.z * (vertex2.z - vertex1.z);
	
	r = e1 * abs(f0.z) + e2 * abs(f0.y);
	
	if (max(-max(p0, p2), min(p0, p2)) > r)
	{
		// Axis is a separating axis
		return false;
	}
	
	// Do similar tests for remaining axis a01..a22
	// ...
	
	// Test the three axis corresponding to the face normals of ZShadeSandboxMath::AABB b
	if (max(max(vertex1.x, vertex2.x), vertex3.x) < -e0 ||  min(min(vertex1.x, vertex2.x), vertex3.x) > e0)
	{
		return false;
	}
	
	if (max(max(vertex1.y, vertex2.y), vertex3.y) < -e1 ||  min(min(vertex1.y, vertex2.y), vertex3.y) > e1)
	{
		return false;
	}
	
	if (max(max(vertex1.z, vertex2.z), vertex3.z) < -e2 ||  min(min(vertex1.z, vertex2.z), vertex3.z) > e2)
	{
		return false;
	}
	
	// Test the separating axis corresponding to triangle face normal
	ZShadeSandboxMath::PlanePrimitive plane;
	plane.Normal() = ZShadeSandboxMath::Vec3<XMFLOAT3>(f0.x, f0.y, f0.z).Cross(ZShadeSandboxMath::Vec3<XMFLOAT3>(f1.x, f1.y, f1.z));
	ZShadeSandboxMath::XMMath3 n(plane.Normal());
	plane.Distance() = n.Dot(vertex1);
	
	return PlaneIntersectsAABB(plane, box) == EPlaneIntersectionType::Intersecting;
}
//==================================================================================================================================
bool ShapeContact::AABBIntersectsAABB(ZShadeSandboxMath::AABB box1, ZShadeSandboxMath::AABB box2)
{
	if (box1.vMin.x > box2.vMax.x || box2.vMin.x > box1.vMax.x)
		return false;
	if (box1.vMin.y > box2.vMax.y || box2.vMin.y > box1.vMax.y)
		return false;
	if (box1.vMin.z > box2.vMax.z || box2.vMin.z > box1.vMax.z)
		return false;
	
	return true;
}
//==================================================================================================================================
bool ShapeContact::AABBIntersectsSphere(ZShadeSandboxMath::AABB box, ZShadeSandboxMath::SpherePrimitive sphere)
{
	ZShadeSandboxMath::XMMath3 vector = ZShadeSandboxMath::XMMath3::Clamp(ZShadeSandboxMath::XMMath3(sphere.Center()), ZShadeSandboxMath::XMMath3(box.vMin), ZShadeSandboxMath::XMMath3(box.vMax));
	
	ZShadeSandboxMath::XMMath3 c(sphere.Center());

	float distance = c.DistanceSquared(XMFLOAT3(vector.x, vector.y, vector.z));
	
	return distance <= SQR(sphere.Radius());
}
//==================================================================================================================================
bool ShapeContact::SphereIntersectsTriangle(ZShadeSandboxMath::SpherePrimitive sphere, ZShadeSandboxMath::TrianglePrimitive tri)
{
	return SphereIntersectsTriangle(sphere, tri.Point1(), tri.Point2(), tri.Point3());
}
//==================================================================================================================================
bool ShapeContact::SphereIntersectsPoint(ZShadeSandboxMath::SpherePrimitive sphere, ZShadeSandboxMath::XMMath3 point)
{
	ZShadeSandboxMath::XMMath3 center(sphere.Center());
	float radius = sphere.Radius();
	
	float distance = point.DistanceSquared(center);
	
	if (distance <= SQR(radius))
		return true;
	
	return false;
}
//==================================================================================================================================
bool ShapeContact::SphereIntersectsTriangle(ZShadeSandboxMath::SpherePrimitive sphere, ZShadeSandboxMath::XMMath3 vertex1, ZShadeSandboxMath::XMMath3 vertex2, ZShadeSandboxMath::XMMath3 vertex3)
{
	ZShadeSandboxMath::XMMath3 center(sphere.Center());
	ZShadeSandboxMath::XMMath3 point;
	
	ClosestPointOnTriangleToPoint(vertex1, vertex2, vertex3, center, point);
	
	ZShadeSandboxMath::XMMath3 v = point - center;
	
	float dot = v.Dot(v);
	
	return dot <= SQR(sphere.Radius());
}
//==================================================================================================================================
bool ShapeContact::SphereIntersectsSphere(ZShadeSandboxMath::SpherePrimitive sphere1, ZShadeSandboxMath::SpherePrimitive sphere2)
{
	ZShadeSandboxMath::XMMath3 center1(sphere1.Center());
	ZShadeSandboxMath::XMMath3 center2(sphere2.Center());
	
	float radiiSum = sphere1.Radius() + sphere2.Radius();
	float distance = center1.DistanceSquared(center2);
	
	return distance <= SQR(radiiSum);
}
//==================================================================================================================================
EContainmentType::Type ShapeContact::AABBContainsPoint(ZShadeSandboxMath::AABB box, ZShadeSandboxMath::XMMath3 point)
{
	if (box.vMin.x <= point.x && box.vMax.x >= point.x &&
		box.vMin.y <= point.y && box.vMax.y >= point.y &&
		box.vMin.z <= point.z && box.vMax.z >= point.z)
	{
		return EContainmentType::Contains;
	}
	
	return EContainmentType::Disjoint;
}
//==================================================================================================================================
EContainmentType::Type ShapeContact::AABBContainsTriangle(ZShadeSandboxMath::AABB box, ZShadeSandboxMath::TrianglePrimitive tri)
{
	return AABBContainsTriangle(box, tri.Point1(), tri.Point2(), tri.Point3());
}
//==================================================================================================================================
EContainmentType::Type ShapeContact::AABBContainsTriangle(ZShadeSandboxMath::AABB box, ZShadeSandboxMath::XMMath3 vertex1, ZShadeSandboxMath::XMMath3 vertex2, ZShadeSandboxMath::XMMath3 vertex3)
{
	EContainmentType::Type test1 = AABBContainsPoint(box, vertex1);
	EContainmentType::Type test2 = AABBContainsPoint(box, vertex2);
	EContainmentType::Type test3 = AABBContainsPoint(box, vertex3);
	
	if (test1 == EContainmentType::Contains && test2 == EContainmentType::Contains && test3 == EContainmentType::Contains)
		return EContainmentType::Contains;
	
	if (AABBIntersectsTriangle(box, vertex1, vertex2, vertex3))
		return EContainmentType::Intersects;
	
	return EContainmentType::Disjoint;
}
//==================================================================================================================================
EContainmentType::Type ShapeContact::AABBContainsAABB(ZShadeSandboxMath::AABB box1, ZShadeSandboxMath::AABB box2)
{
	if (box1.vMax.x < box2.vMin.x || box1.vMin.x > box2.vMax.x)
		return EContainmentType::Disjoint;
	
	if (box1.vMax.y < box2.vMin.y || box1.vMin.y > box2.vMax.y)
		return EContainmentType::Disjoint;
	
	if (box1.vMax.z < box2.vMin.z || box1.vMin.z > box2.vMax.z)
		return EContainmentType::Disjoint;
	
	if (box1.vMin.x <= box2.vMin.x && (box2.vMax.x <= box1.vMax.x &&
		box1.vMin.y <= box2.vMin.y && box2.vMax.y <= box1.vMax.y) &&
		box1.vMin.z <= box2.vMin.z && box2.vMax.z <= box1.vMax.z)
	{
		return EContainmentType::Contains;
	}
	
	return EContainmentType::Intersects;
}
//==================================================================================================================================
EContainmentType::Type ShapeContact::AABBContainsSphere(ZShadeSandboxMath::AABB box, ZShadeSandboxMath::SpherePrimitive sphere)
{
	ZShadeSandboxMath::XMMath3 center(sphere.Center());
	float radius = sphere.Radius();
	
	ZShadeSandboxMath::XMMath3 vector = ZShadeSandboxMath::XMMath3::Clamp(center, ZShadeSandboxMath::XMMath3(box.vMin), ZShadeSandboxMath::XMMath3(box.vMax));
	
	ZShadeSandboxMath::XMMath3 c(sphere.Center());

	float distance = c.DistanceSquared(XMFLOAT3(vector.x, vector.y, vector.z));
	
	if (distance > SQR(radius))
		return EContainmentType::Disjoint;
	
	if ((((box.vMin.x + radius <= center.x) && (center.x <= box.vMax.x - radius)) && ((box.vMax.x - box.vMin.x > radius) &&
		  (box.vMin.y + radius <= center.y))) && (((center.y <= box.vMax.y - radius) && (box.vMax.y - box.vMin.y > radius)) &&
		(((box.vMin.z + radius <= center.z) && (center.z <= box.vMax.z - radius)) && (box.vMax.x - box.vMin.x > radius))))
	{
		return EContainmentType::Contains;
	}
	
	return EContainmentType::Intersects;
}
//==================================================================================================================================
EContainmentType::Type ShapeContact::SphereContainsPoint(ZShadeSandboxMath::SpherePrimitive sphere, ZShadeSandboxMath::XMMath3 point)
{
	ZShadeSandboxMath::XMMath3 center(sphere.Center());
	float radius = sphere.Radius();
	
	float distance = point.DistanceSquared(center);
	
	if (distance <= SQR(radius))
		return EContainmentType::Contains;
	
	return EContainmentType::Disjoint;
}
//==================================================================================================================================
EContainmentType::Type ShapeContact::SphereContainsTriangle(ZShadeSandboxMath::SpherePrimitive sphere, ZShadeSandboxMath::TrianglePrimitive tri)
{
	return SphereContainsTriangle(sphere, tri.Point1(), tri.Point2(), tri.Point3());
}
//==================================================================================================================================
EContainmentType::Type ShapeContact::SphereContainsTriangle(ZShadeSandboxMath::SpherePrimitive sphere, ZShadeSandboxMath::XMMath3 vertex1, ZShadeSandboxMath::XMMath3 vertex2, ZShadeSandboxMath::XMMath3 vertex3)
{
	EContainmentType::Type test1 = SphereContainsPoint(sphere, vertex1);
	EContainmentType::Type test2 = SphereContainsPoint(sphere, vertex2);
	EContainmentType::Type test3 = SphereContainsPoint(sphere, vertex3);
	
	if (test1 == EContainmentType::Contains && test2 == EContainmentType::Contains && test3 == EContainmentType::Contains)
		return EContainmentType::Contains;
	
	if (SphereIntersectsTriangle(sphere, vertex1, vertex2, vertex3))
		return EContainmentType::Intersects;
	
	return EContainmentType::Disjoint;
}
//==================================================================================================================================
EContainmentType::Type ShapeContact::SphereContainsAABB(ZShadeSandboxMath::SpherePrimitive sphere, ZShadeSandboxMath::AABB box)
{
	ZShadeSandboxMath::XMMath3 vector;
	ZShadeSandboxMath::XMMath3 center(sphere.Center());
	float radiiSqr = SQR(sphere.Radius());
	
	if (!AABBIntersectsSphere(box, sphere))
		return EContainmentType::Disjoint;
	
	vector.x = center.x - box.vMin.x;
	vector.y = center.y - box.vMax.y;
	vector.z = center.z - box.vMax.z;
	
	if (vector.LengthSquared() > radiiSqr)
		return EContainmentType::Intersects;
	
	vector.x = center.x - box.vMax.x;
	vector.y = center.y - box.vMax.y;
	vector.z = center.z - box.vMax.z;
	
	if (vector.LengthSquared() > radiiSqr)
		return EContainmentType::Intersects;
	
	vector.x = center.x - box.vMax.x;
	vector.y = center.y - box.vMin.y;
	vector.z = center.z - box.vMax.z;
	
	if (vector.LengthSquared() > radiiSqr)
		return EContainmentType::Intersects;
	
	vector.x = center.x - box.vMin.x;
	vector.y = center.y - box.vMin.y;
	vector.z = center.z - box.vMax.z;
	
	if (vector.LengthSquared() > radiiSqr)
		return EContainmentType::Intersects;
	
	vector.x = center.x - box.vMin.x;
	vector.y = center.y - box.vMax.y;
	vector.z = center.z - box.vMin.z;
	
	if (vector.LengthSquared() > radiiSqr)
		return EContainmentType::Intersects;
	
	vector.x = center.x - box.vMax.x;
	vector.y = center.y - box.vMax.y;
	vector.z = center.z - box.vMin.z;
	
	if (vector.LengthSquared() > radiiSqr)
		return EContainmentType::Intersects;
	
	vector.x = center.x - box.vMax.x;
	vector.y = center.y - box.vMin.y;
	vector.z = center.z - box.vMin.z;
	
	if (vector.LengthSquared() > radiiSqr)
		return EContainmentType::Intersects;
	
	vector.x = center.x - box.vMin.x;
	vector.y = center.y - box.vMin.y;
	vector.z = center.z - box.vMin.z;
	
	if (vector.LengthSquared() > radiiSqr)
		return EContainmentType::Intersects;
	
	return EContainmentType::Contains;
}
//==================================================================================================================================
EContainmentType::Type ShapeContact::SphereContainsSphere(ZShadeSandboxMath::SpherePrimitive sphere1, ZShadeSandboxMath::SpherePrimitive sphere2)
{
	ZShadeSandboxMath::XMMath3 center1(sphere1.Center());
	ZShadeSandboxMath::XMMath3 center2(sphere2.Center());
	
	float radius1 = sphere1.Radius();
	float radius2 = sphere2.Radius();
	
	float distance = center1.Distance(center2);
	
	if (radius1 + radius2 < distance)
		return EContainmentType::Disjoint;
	
	if (radius1 - radius2 < distance)
		return EContainmentType::Intersects;
	
	return EContainmentType::Contains;
}
//==================================================================================================================================
void ShapeContact::SupportPoint(ZShadeSandboxMath::TrianglePrimitive tri, ZShadeSandboxMath::XMMath3 direction, ZShadeSandboxMath::XMMath3& result)
{
	SupportPoint(tri.Point1(), tri.Point2(), tri.Point3(), direction, result);
}
//==================================================================================================================================
void ShapeContact::SupportPoint(ZShadeSandboxMath::XMMath3 vertex1, ZShadeSandboxMath::XMMath3 vertex2, ZShadeSandboxMath::XMMath3 vertex3, ZShadeSandboxMath::XMMath3 direction, ZShadeSandboxMath::XMMath3& result)
{
	float dot1 = vertex1.Dot(direction);
	float dot2 = vertex2.Dot(direction);
	float dot3 = vertex3.Dot(direction);
	
	if (dot1 > dot2 && dot1 > dot3)
		result = vertex1;
	else if (dot2 > dot1 && dot2 > dot3)
		result = vertex2;
	else
		result = vertex3;
}
//==================================================================================================================================
void ShapeContact::SupportPoint(ZShadeSandboxMath::AABB box, ZShadeSandboxMath::XMMath3 direction, ZShadeSandboxMath::XMMath3& result)
{
	result.x = (direction.x >= 0.0f) ? box.vMax.x : box.vMin.x;
	result.y = (direction.y >= 0.0f) ? box.vMax.y : box.vMin.y;
	result.z = (direction.z >= 0.0f) ? box.vMax.z : box.vMin.z;
}
//==================================================================================================================================
void ShapeContact::SupportPoint(ZShadeSandboxMath::SpherePrimitive sphere, ZShadeSandboxMath::XMMath3 direction, ZShadeSandboxMath::XMMath3& result)
{
	ZShadeSandboxMath::XMMath3 center(sphere.Center());
	float radius = sphere.Radius();
	
	result = (radius / direction.Length()) * (direction + center);
}
//==================================================================================================================================
void ShapeContact::SupportPoint(vector<ZShadeSandboxMath::XMMath3> points, ZShadeSandboxMath::XMMath3 direction, ZShadeSandboxMath::XMMath3& result)
{
	float dot = 0;
	float dotMax = FLT_MIN;
	result = XMFLOAT3(0, 0, 0);
	
	for (int i = 0; i < points.size(); i++)
	{
		ZShadeSandboxMath::XMMath3 point = points[i];
		
		dot = direction.Dot(point);
		
		if (dot > dotMax)
		{
			dotMax = dot;
			result = point;
		}
	}
}
//==================================================================================================================================