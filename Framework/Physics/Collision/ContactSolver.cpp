#include "ContactSolver.h"
//==================================================================================================================================
//==================================================================================================================================
ContactSolver::ContactSolver()
{
	
}
//==================================================================================================================================
void ContactSolver::Solve(PhysicsBody3D* body1, PhysicsBody3D* body2, ContactManifold* manifold)
{
	
}
//==================================================================================================================================
void ContactSolver::CalculateContactPoints(PhysicsBody3D* body1, PhysicsBody3D* body2, GJK_MTV* mtv, ContactManifold* manifold)
{
	vector< ZShadeSandboxMath::Vec3<XMFLOAT3> > body1Points = body1->GetPoints();
	vector< ZShadeSandboxMath::Vec3<XMFLOAT3> > body2Points = body2->GetPoints();
	
	// Find the farthest vertex in a mesh along the normal of the translation vector
	int body1Index = FurthestIndex(body1Points, mtv->normal);
	int body2Index = FurthestIndex(body2Points, mtv->normal);
	
	// Find the edge that is most perpendicular
	ZShadeSandboxMath::Vec3<XMFLOAT3> v = body1Points[body1Index];
	
	// Get the vectors that are to the left and right
	ZShadeSandboxMath::Vec3<XMFLOAT3> v1;// = *v.next;
	ZShadeSandboxMath::Vec3<XMFLOAT3> v0;// = *v.prev;
	
	ZShadeSandboxMath::Vec3<XMFLOAT3> left = v - v1;
	ZShadeSandboxMath::Vec3<XMFLOAT3> right = v - v0;
	
	// Normalize the vectors
	left.Normalize();
	right.Normalize();
	
	// The edge that is most perpendicular to the translation vector will have a dot product closer to zero
	if (right.Dot( mtv->normal ) <= left.Dot( mtv->normal ))
	{
		// The right edge is better
		
	}
	else
	{
		// The left edge is better
		
	}
	
	
}
//==================================================================================================================================
int ContactSolver::FurthestIndex(vector< ZShadeSandboxMath::Vec3<XMFLOAT3> > points, ZShadeSandboxMath::Vec3<XMFLOAT3> normal)
{
	float max = 0.0f;
	int index = -9999;
	
	for (int i = 0; i < points.size(); i++)
	{
		ZShadeSandboxMath::Vec3<XMFLOAT3> vertex = points[i];
		
		float proj = normal.Dot( vertex );
		
		if (proj > max)
		{
			max = proj;
			index = i;
		}
	}
	
	return index;
}
//==================================================================================================================================