#include "PhysicsBodyBase3D.h"
#include "QuadMesh.h"
#include "MaterialManager.h"
//==================================================================================================================================
//==================================================================================================================================
PhysicsBodyBase3D::PhysicsBodyBase3D()
:	bColorHull(false)
,	bTextureHull(false)
,	boundary(0)
{
}
//==================================================================================================================================
PhysicsBodyBase3D::PhysicsBodyBase3D(D3D* d3d)
:	m_D3DSystem(d3d)
,	bColorHull(false)
,	bTextureHull(false)
,	boundary(0)
{
	m_EngineOptions = d3d->GetEngineOptions();
}
//==================================================================================================================================
void PhysicsBodyBase3D::SetD3D(D3D* d3d)
{
	m_D3DSystem = d3d;
	m_EngineOptions = d3d->GetEngineOptions();
}
//==================================================================================================================================
void PhysicsBodyBase3D::InitAABB(XMFLOAT3 center, XMFLOAT3 fScale)
{
	if (boundary == 0)
		boundary = new ZShadeSandboxMath::AABB();
	
	boundary->vMin = XMFLOAT3(center.x - fScale.x, center.y - fScale.y, center.z - fScale.z);
	boundary->vMax = XMFLOAT3(center.x + fScale.x, center.y + fScale.y, center.z + fScale.z);
}
//==================================================================================================================================
bool PhysicsBodyBase3D::IntersectsAABB(XMFLOAT3 point)
{
	if (boundary == 0) return false;
	
	// Calculate the dimensions of this node.
	float xMin = boundary->vMin.x;
	float xMax = boundary->vMax.x;

	float zMin = boundary->vMin.z;
	float zMax = boundary->vMax.z;

	// See if the x and z coordinate are in this node, if not then stop traversing this part of the tree.
	if ((point.x < xMin) || (point.x > xMax) || (point.z < zMin) || (point.z > zMax))
	{
		return false;
	}
	
	if (m_EngineOptions->m_DimType == ZSHADE_2D)
		return boundary->ContainsPoint2D(point);
	else if (m_EngineOptions->m_DimType == ZSHADE_3D)
		return boundary->ContainsPoint3D(point);
}
//==================================================================================================================================
float PhysicsBodyBase3D::DistanceAABBToPoint(XMFLOAT3 point)
{
	if (boundary == 0) return -1;
	if (m_EngineOptions->m_DimType == ZSHADE_2D)
		return boundary->DistanceToPoint2D(point);
	else if (m_EngineOptions->m_DimType == ZSHADE_3D)
		return boundary->DistanceToPoint3D(point);
}
//==================================================================================================================================
void PhysicsBodyBase3D::ColorTheHull(ZShadeSandboxMath::XMMath4 color)
{
	bColorHull = true;
	bTextureHull = false;
	
	vColor = color;
}
//==================================================================================================================================
void PhysicsBodyBase3D::TextureTheHull()
{
	bColorHull = false;
	bTextureHull = true;
}
//==================================================================================================================================
void PhysicsBodyBase3D::AddPoints(vector<ZShadeSandboxMesh::VertexTex> points)
{
	mMeshPoints = points;
	
	for (uint32 i = 0; i < mMeshPoints.size(); i++)
	{
		ZShadeSandboxMath::Vec3<XMFLOAT3> p(mMeshPoints[i].position.x, mMeshPoints[i].position.y, mMeshPoints[i].position.z);

		//ZShadeSandboxMesh::VertexTex nv = mMeshPoints[i].next;
		//Vec3<XMFLOAT3> nvp(nv.position.x, nv.position.y, nv.position.z);
		//p.next = &nvp;

		//ZShadeSandboxMesh::VertexTex pv = *mMeshPoints[i].prev;
		//Vec3<XMFLOAT3> pvp(pv.position.x, pv.position.y, pv.position.z);
		//p.prev = &pvp;

		mPoints.push_back(p);
	}
}
//==================================================================================================================================
void PhysicsBodyBase3D::AddPoints(vector< ZShadeSandboxMath::Vec3<XMFLOAT3> > points)
{
	for (uint32 i = 0; i < points.size(); i++)
	{
		mPoints.push_back(points[i]);
	}
}
//==================================================================================================================================
void PhysicsBodyBase3D::AddPoint(ZShadeSandboxMath::Vec3<XMFLOAT3> point)
{
	mPoints.push_back(point);
}
//==================================================================================================================================
void PhysicsBodyBase3D::CreateConvexHull()
{
	// Create the convex hull
	/*mHull = mConvexHull.Wrap( mPoints );
	
	for (uint32 i = 0; i < mHull.size(); i++)
	{
		//if (bColorHull)
		//{
		//	Vertex p(mHull[i].Vector().x, mHull[i].Vector().y, mHull[i].Vector().z, vColor.x, vColor.y, vColor.z, vColor.w);
		//	mRenderHull.push_back(p);
		//}
		
		if (bTextureHull)
		{
			ZShadeSandboxMesh::VertexTex meshPoint;
			
			// Find the matching point in mesh points
			for (uint32 j = 0; j < mMeshPoints.size(); j++)
			{
				Vec3<XMFLOAT3> p(mMeshPoints[j].position.x, mMeshPoints[j].position.y, mMeshPoints[j].position.z);
				if (mHull[i] == p)
				{
					meshPoint = mMeshPoints[j];
					break;
				}
			}
			
			ZShadeSandboxMesh::VertexTex p = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(mHull[i].Vector().x, mHull[i].Vector().y, mHull[i].Vector().z, meshPoint.texture.x, meshPoint.texture.y);
			mRenderHull.push_back(p);
		}
	}
	
	// Get the center of the hull
	mCenter = mConvexHull.Center();
	
	m_hullCreated = true;
	*/
}
//===============================================================================================================================