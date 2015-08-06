#include "PointColliderController.h"
using ZShadeSandboxPhysics::PointColliderController;
//===============================================================================================================================
//===============================================================================================================================
PointColliderController::PointColliderController()
:	fUnitsPerMeter(100.0f)
,	fGravity(-2.0f)
{
}
//===============================================================================================================================
PointColliderController::~PointColliderController()
{
}
//===============================================================================================================================
//bool TerrainPointCollider::CameraColliding(float unitsPerMeter, float gravity, Camera* camera, ZShadeSandboxTerrain::QMeshNode* node, XMFLOAT3& intersectionPoint)
bool PointColliderController::Colliding
(	float unitsPerMeter
,	float gravity
,	XMFLOAT3 position
,	float moveStrafe
,	XMFLOAT3 right
,	float moveWalk
,	XMFLOAT3 forward
,	vector<XMFLOAT3> vertices
,	vector<UINT> indices
,	XMFLOAT3& intersectionPoint
)
{
	fUnitsPerMeter = unitsPerMeter;
	fGravity = gravity;
	
	// Initialize the collision packet and locate the collision
	mCollisionPacket.ellipsoidSpace = XMVectorSet(1.0f, 3.0f, 1.0f, 0.0f);
	mCollisionPacket.wPosition = ZShadeSandboxMath::XMMath3(position).ToVector();
	mCollisionPacket.wVelocity = (moveStrafe * ZShadeSandboxMath::XMMath3(right).ToVector()) + (moveWalk * ZShadeSandboxMath::XMMath3(forward).ToVector());
	intersectionPoint = ZShadeSandboxMath::XMMath3(CollisionSlide(vertices, indices));

	return mCollisionPacket.collisionFound;
}
//===============================================================================================================================
XMVECTOR PointColliderController::CollisionSlide(vector<XMFLOAT3> vertices, vector<UINT> indices)
{
	mCollisionPacket.eVelocity = mCollisionPacket.wVelocity / mCollisionPacket.ellipsoidSpace;
	mCollisionPacket.ePosition = mCollisionPacket.wPosition / mCollisionPacket.ellipsoidSpace;
	
	mCollisionPacket.collisionRecursionDepth = 0;
	
	XMVECTOR finalPosition = CollisionWithWorld(vertices, indices);
	
	mCollisionPacket.eVelocity = XMVectorSet(0, fGravity, 0, 0) / mCollisionPacket.ellipsoidSpace;
	mCollisionPacket.ePosition = finalPosition;
	
	mCollisionPacket.collisionRecursionDepth = 0;
	
	finalPosition = CollisionWithWorld(vertices, indices);
	
	finalPosition *= mCollisionPacket.ellipsoidSpace;
	
	return finalPosition;
}
//===============================================================================================================================
XMVECTOR PointColliderController::CollisionWithWorld(vector<XMFLOAT3> vertices, vector<UINT> indices)
{
	float unitScale = fUnitsPerMeter / 100.0f;
	
	// Keeps the sphere from touching
	float veryCloseDistance = 0.005f * unitScale;
	
	if (mCollisionPacket.collisionRecursionDepth > 5)
	{
		return mCollisionPacket.ePosition;
	}
	
	mCollisionPacket.eNormalizedVelocity = XMVector3Normalize(mCollisionPacket.eVelocity);
	
	mCollisionPacket.collisionFound = false;
	mCollisionPacket.nearestDistance = 0.0f;
	
	XMVECTOR vertex0, vertex1, vertex2, tempVec, triNormal;
	
	for (int triID = 0; triID < indices.size() / 3; triID++)
	{
		//vertex0 = tris[triID]->GetVertex(0).ToVector();
		//vertex1 = tris[triID]->GetVertex(1).ToVector();
		//vertex2 = tris[triID]->GetVertex(2).ToVector();
		
		vertex0 = ZShadeSandboxMath::XMMath3(vertices[indices[triID + 0]]).ToVector();
		vertex1 = ZShadeSandboxMath::XMMath3(vertices[indices[triID + 1]]).ToVector();
		vertex2 = ZShadeSandboxMath::XMMath3(vertices[indices[triID + 2]]).ToVector();

		// Put the triangle in ellipsoid space
		vertex0 /= mCollisionPacket.ellipsoidSpace;
		vertex1 /= mCollisionPacket.ellipsoidSpace;
		vertex2 /= mCollisionPacket.ellipsoidSpace;
		
		// Calculate normal for the triangle
		triNormal = XMVector3Normalize(XMVector3Cross((vertex1 - vertex0), (vertex2 - vertex0)));
		
		// Check to see if the sphere is colliding with the triangle
		if (SphereCollidingWithTriangle(vertex0, vertex1, vertex2, triNormal))
		{
			break;
		}
	}
	
	XMVECTOR destinationPoint = mCollisionPacket.ePosition + mCollisionPacket.eVelocity;
	
	if (!mCollisionPacket.collisionFound)
	{
		return destinationPoint;
	}
	
	XMVECTOR newPosition = mCollisionPacket.ePosition;
	
	if (ZShadeSandboxMath::XMMath3(mCollisionPacket.ePosition) >= veryCloseDistance)
	{
		XMVECTOR v = mCollisionPacket.eVelocity;
		v = XMVector3Normalize(v);
		v *= (mCollisionPacket.nearestDistance - veryCloseDistance);
		v = XMVector3Normalize(v);
		mCollisionPacket.intersectionPoint -= veryCloseDistance * v;
	}
	
	XMVECTOR slidingPlaneOrigin = mCollisionPacket.intersectionPoint;
	XMVECTOR slidingPlaneNormal = newPosition - mCollisionPacket.intersectionPoint;
	slidingPlaneNormal = XMVector3Normalize(slidingPlaneNormal);
	
	float x = XMVectorGetX(slidingPlaneOrigin);
	float y = XMVectorGetY(slidingPlaneOrigin);
	float z = XMVectorGetZ(slidingPlaneOrigin);
	
	float a = XMVectorGetX(slidingPlaneNormal);
	float b = XMVectorGetY(slidingPlaneNormal);
	float c = XMVectorGetZ(slidingPlaneNormal);
	float planeConstant = -((a * x) + (b * y) + (c * z));
	
	float signedDistanceFromDestPointToSlidingPlane = XMVectorGetX(XMVector3Dot(destinationPoint, slidingPlaneNormal)) + planeConstant;
	
	XMVECTOR newDestinationPoint = destinationPoint - signedDistanceFromDestPointToSlidingPlane * slidingPlaneNormal;
	XMVECTOR newVelocityVector = newDestinationPoint - mCollisionPacket.intersectionPoint;
	
	if (XMVectorGetX(XMVector3Length(newVelocityVector)) < veryCloseDistance)
	{
		return newPosition;
	}
	
	mCollisionPacket.collisionRecursionDepth++;
	mCollisionPacket.ePosition = newPosition;
	mCollisionPacket.eVelocity = newVelocityVector;
	
	return CollisionWithWorld(vertices, indices);
}
//===============================================================================================================================
bool PointColliderController::SphereCollidingWithTriangle(XMVECTOR& vertex0, XMVECTOR& vertex1, XMVECTOR& vertex2, XMVECTOR& normal)
{
	float facing = XMVectorGetX(XMVector3Dot(normal, mCollisionPacket.eNormalizedVelocity));
	
	if (facing <= 0)
	{
		XMVECTOR velocity = mCollisionPacket.eVelocity;
		XMVECTOR position = mCollisionPacket.ePosition;
		
		float t0, t1;
		
		bool sphereInPlane = false;
		
		float x = XMVectorGetX(vertex0);
		float y = XMVectorGetY(vertex0);
		float z = XMVectorGetZ(vertex0);
		
		float a = XMVectorGetX(normal);
		float b = XMVectorGetY(normal);
		float c = XMVectorGetZ(normal);
		
		float planeConstant = -((a * x) + (b * y) + (c * z));
		
		float signedDistanceFromPositionToTriPlane = XMVectorGetX(XMVector3Dot(position, normal)) * planeConstant;
		
		float planeNormalDotVelocity = XMVectorGetX(XMVector3Dot(normal, velocity));
		
		if (planeNormalDotVelocity == 0.0f)
		{
			if (fabs(signedDistanceFromPositionToTriPlane) >= 1.0f)
			{
				return false;
			}
			else
			{
				sphereInPlane = true;
			}
		}
		else
		{
			t0 = ( 1.0f - signedDistanceFromPositionToTriPlane) / planeNormalDotVelocity;
			t1 = (-1.0f - signedDistanceFromPositionToTriPlane) / planeNormalDotVelocity;
			
			if (t0 > t1)
			{
				float temp = t0;
				t0 = t1;
				t1 = temp;
			}
			
			if (t0 > 1.0f || t1 < 0.0f)
			{
				return false;
			}
			
			if (t0 < 0.0f)
			{
				t0 = 0.0f;
			}
			
			if (t1 > 1.0f)
			{
				t1 = 1.0f;
			}
		}
		
		XMVECTOR collisionPoint;
		bool collidingWithTri = false;
		float t = 1.0f;
		
		if (!sphereInPlane)
		{
			XMVECTOR planeIntersectionPoint = (position + t0 * velocity - normal);
			
			if (CheckPointInTriangle(planeIntersectionPoint, vertex0, vertex1, vertex2))
			{
				collidingWithTri = true;
				t = t0;
				collisionPoint = planeIntersectionPoint;
			}
		}
		
		if (!collidingWithTri)
		{
			float a, b, c;
			
			float velocityLengthSquared = XMVectorGetX(XMVector3Length(velocity));
			velocityLengthSquared *= velocityLengthSquared;
			
			a = velocityLengthSquared;
			
			float newT;
			
			// Collision test with sphere and vertex0
			b = 2.0f * (XMVectorGetX(XMVector3Dot(velocity, position - vertex0)));
			c = XMVectorGetX(XMVector3Length(vertex0 - position));
			c = SQR(c) - 1.0f;
			
			if (GetLowestRoot(a, b, c, t, &newT))
			{
				t = newT;
				collidingWithTri = true;
				collisionPoint = vertex0;
			}
			
			// Collision test with sphere and vertex1
			b = 2.0f * (XMVectorGetX(XMVector3Dot(velocity, position - vertex1)));
			c = XMVectorGetX(XMVector3Length(vertex1 - position));
			c = SQR(c) - 1.0f;
			
			if (GetLowestRoot(a, b, c, t, &newT))
			{
				t = newT;
				collidingWithTri = true;
				collisionPoint = vertex1;
			}
			
			// Collision test with sphere and vertex2
			b = 2.0f * (XMVectorGetX(XMVector3Dot(velocity, position - vertex2)));
			c = XMVectorGetX(XMVector3Length(vertex2 - position));
			c = SQR(c) - 1.0f;
			
			if (GetLowestRoot(a, b, c, t, &newT))
			{
				t = newT;
				collidingWithTri = true;
				collisionPoint = vertex2;
			}
			
			// Collision test with sphere and edge (p0, p1)
			XMVECTOR edge = vertex1 - vertex0;
			XMVECTOR spherePositionToVertex = vertex0 - position;
			
			float edgeLengthSquared = XMVectorGetX(XMVector3Length(edge));
			edgeLengthSquared *= edgeLengthSquared;
			
			float edgeDotVelocity = XMVectorGetX(XMVector3Dot(edge, velocity));
			float edgeDotSpherePositionToVertex = XMVectorGetX(XMVector3Dot(edge, spherePositionToVertex));
			
			float spherePositionToVertexLengthSquared = XMVectorGetX(XMVector3Length(spherePositionToVertex));
			spherePositionToVertexLengthSquared *= spherePositionToVertexLengthSquared;
			
			a = edgeLengthSquared * -velocityLengthSquared + SQR(edgeDotVelocity);
			b = edgeLengthSquared * (2.0f * XMVectorGetX(XMVector3Dot(velocity, spherePositionToVertex)) - (2.0f * edgeDotVelocity * edgeDotSpherePositionToVertex));
			c = edgeLengthSquared * (1.0f - spherePositionToVertexLengthSquared) + SQR(edgeDotSpherePositionToVertex);
			
			if (GetLowestRoot(a, b, c, t, &newT))
			{
				float f = (edgeDotVelocity * newT - edgeDotSpherePositionToVertex) / edgeLengthSquared;
				
				if (f >= 0.0f && f <= 1.0f)
				{
					t = newT;
					collidingWithTri = true;
					collisionPoint = vertex0 + f * edge;
				}
			}
			
			// Collision test with sphere and edge (p1, p2)
			edge = vertex2 - vertex1;
			spherePositionToVertex = vertex1 - position;
			
			edgeLengthSquared = XMVectorGetX(XMVector3Length(edge));
			edgeLengthSquared *= edgeLengthSquared;
			
			edgeDotVelocity = XMVectorGetX(XMVector3Dot(edge, velocity));
			edgeDotSpherePositionToVertex = XMVectorGetX(XMVector3Dot(edge, spherePositionToVertex));
			
			spherePositionToVertexLengthSquared = XMVectorGetX(XMVector3Length(spherePositionToVertex));
			spherePositionToVertexLengthSquared *= spherePositionToVertexLengthSquared;
			
			a = edgeLengthSquared * -velocityLengthSquared + SQR(edgeDotVelocity);
			b = edgeLengthSquared * (2.0f * XMVectorGetX(XMVector3Dot(velocity, spherePositionToVertex)) - (2.0f * edgeDotVelocity * edgeDotSpherePositionToVertex));
			c = edgeLengthSquared * (1.0f - spherePositionToVertexLengthSquared) + SQR(edgeDotSpherePositionToVertex);
			
			if (GetLowestRoot(a, b, c, t, &newT))
			{
				float f = (edgeDotVelocity * newT - edgeDotSpherePositionToVertex) / edgeLengthSquared;
				
				if (f >= 0.0f && f <= 1.0f)
				{
					t = newT;
					collidingWithTri = true;
					collisionPoint = vertex1 + f * edge;
				}
			}
			
			// Collision test with sphere and edge (p2, p0)
			edge = vertex0 - vertex2;
			spherePositionToVertex = vertex2 - position;
			
			edgeLengthSquared = XMVectorGetX(XMVector3Length(edge));
			edgeLengthSquared *= edgeLengthSquared;
			
			edgeDotVelocity = XMVectorGetX(XMVector3Dot(edge, velocity));
			edgeDotSpherePositionToVertex = XMVectorGetX(XMVector3Dot(edge, spherePositionToVertex));
			
			spherePositionToVertexLengthSquared = XMVectorGetX(XMVector3Length(spherePositionToVertex));
			spherePositionToVertexLengthSquared *= spherePositionToVertexLengthSquared;
			
			a = edgeLengthSquared * -velocityLengthSquared + SQR(edgeDotVelocity);
			b = edgeLengthSquared * (2.0f * XMVectorGetX(XMVector3Dot(velocity, spherePositionToVertex)) - (2.0f * edgeDotVelocity * edgeDotSpherePositionToVertex));
			c = edgeLengthSquared * (1.0f - spherePositionToVertexLengthSquared) + SQR(edgeDotSpherePositionToVertex);
			
			if (GetLowestRoot(a, b, c, t, &newT))
			{
				float f = (edgeDotVelocity * newT - edgeDotSpherePositionToVertex) / edgeLengthSquared;
				
				if (f >= 0.0f && f <= 1.0f)
				{
					t = newT;
					collidingWithTri = true;
					collisionPoint = vertex2 + f * edge;
				}
			}
		}
		
		if (collidingWithTri)
		{
			float distanceToCollision = t * XMVectorGetX(XMVector3Length(velocity));
			
			if (!mCollisionPacket.collisionFound || distanceToCollision < mCollisionPacket.nearestDistance)
			{
				mCollisionPacket.nearestDistance = distanceToCollision;
				mCollisionPacket.intersectionPoint = collisionPoint;
				mCollisionPacket.collisionFound = true;
				
				return true;
			}
		}
	}
	
	return false;
}
//===============================================================================================================================
bool PointColliderController::CheckPointInTriangle(const XMVECTOR& point, XMVECTOR& vertex0, XMVECTOR& vertex1, XMVECTOR& vertex2)
{
	XMVECTOR cp1 = XMVector3Cross((vertex2 - vertex1), (point - vertex1));
	XMVECTOR cp2 = XMVector3Cross((vertex2 - vertex1), (vertex0 - vertex1));
	
	if (XMVectorGetX(XMVector3Dot(cp1, cp2)) >= 0)
	{
		cp1 = XMVector3Cross((vertex2 - vertex0), (point - vertex0));
		cp2 = XMVector3Cross((vertex2 - vertex0), (vertex1 - vertex0));
		
		if (XMVectorGetX(XMVector3Dot(cp1, cp2)) >= 0)
		{
			cp1 = XMVector3Cross((vertex1 - vertex0), (point - vertex0));
			cp2 = XMVector3Cross((vertex1 - vertex0), (vertex2 - vertex0));
			
			if (XMVectorGetX(XMVector3Dot(cp1, cp2)) >= 0)
			{
				return true;
			}
		}
	}
	
	return false;
}
//===============================================================================================================================
bool PointColliderController::GetLowestRoot(float a, float b, float c, float maxR, float* root)
{
	float determinant = SQR(b) - 4.0 * a * c;
	
	if (determinant < 0.0f)
	{
		return false;
	}
	
	float sqrtD = sqrt(determinant);
	
	float r1 = (-b - sqrtD) / (2 * a);
	float r2 = (-b + sqrtD) / (2 * a);
	
	if (r1 > r2)
	{
		float temp = r2;
		r2 = r1;
		r1 = temp;
	}
	
	if (r1 > 0 && r1 < maxR)
	{
		*root = r1;
		return true;
	}
	
	if (r2 > 0 && r2 < maxR)
	{
		*root = r2;
		return true;
	}
	
	return false;
}
//===============================================================================================================================
