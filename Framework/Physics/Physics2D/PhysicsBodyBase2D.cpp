#include "PhysicsBodyBase2D.h"
#include "QuadMesh.h"
#include "MaterialManager.h"
//==================================================================================================================================
//==================================================================================================================================
PhysicsBodyBase2D::PhysicsBodyBase2D()
:	boundary(0)
,	mSurfaceType(ESurfaceType::Type::eNormal)
{
}
//==================================================================================================================================
PhysicsBodyBase2D::PhysicsBodyBase2D(D3D* d3d)
:	m_D3DSystem(d3d)
,	boundary(0)
,	mSurfaceType(ESurfaceType::Type::eNormal)
{
	m_EngineOptions = d3d->GetEngineOptions();
}
//==================================================================================================================================
void PhysicsBodyBase2D::SetD3D(D3D* d3d)
{
	m_D3DSystem = d3d;
	m_EngineOptions = d3d->GetEngineOptions();
}
//==================================================================================================================================
void PhysicsBodyBase2D::InitAABB(XMFLOAT3 center, XMFLOAT3 fScale)
{
	if (boundary == 0)
		boundary = new ZShadeSandboxMath::AABB();
	
	boundary->vMin = XMFLOAT3(center.x - fScale.x, center.y - fScale.y, center.z - fScale.z);
	boundary->vMax = XMFLOAT3(center.x + fScale.x, center.y + fScale.y, center.z + fScale.z);
}
//==================================================================================================================================
bool PhysicsBodyBase2D::IntersectsAABB(XMFLOAT3 point)
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
float PhysicsBodyBase2D::DistanceAABBToPoint(XMFLOAT3 point)
{
	if (boundary == 0) return -1;
	if (m_EngineOptions->m_DimType == ZSHADE_2D)
		return boundary->DistanceToPoint2D(point);
	else if (m_EngineOptions->m_DimType == ZSHADE_3D)
		return boundary->DistanceToPoint3D(point);
}
//===============================================================================================================================
void PhysicsBodyBase2D::UpdateMovingPlatform()
{
	// This needs to be tested
	m_TopLeftPosition.x = -m_TopLeftPosition.x + fInitialX + fPlatformDx;
	m_TopLeftPosition.y = -m_TopLeftPosition.y + fInitialY + fPlatformDy;
	m_HardboxPosition.x = -m_body.x + fInitialX + fPlatformDx;
	m_HardboxPosition.y = -m_body.y + fInitialY + fPlatformDy;
	m_previousBody = m_body;
	m_body = BodyBoundsPredict(m_HardboxPosition);
}
//===============================================================================================================================
ZShadeSandboxMath::BoundingBox PhysicsBodyBase2D::BodyBoundsPredict(XMFLOAT3 point)
{
	ZShadeSandboxMath::BoundingBox box;
	box.x = m_HardboxPosition.x + point.x;
	box.y = m_HardboxPosition.y + point.y;
	box.width = iBodyWidth;
	box.width = iBodyHeight;
	return box;
}
//===============================================================================================================================
ZShadeSandboxMath::BoundingBox PhysicsBodyBase2D::BodyBoundsPredict(float x, float y)
{
	ZShadeSandboxMath::BoundingBox box;
	box.x = m_HardboxPosition.x + x;
	box.y = m_HardboxPosition.y + y;
	box.width = iBodyWidth;
	box.width = iBodyHeight;
	return box;
}
//===============================================================================================================================
void PhysicsBodyBase2D::InitCollisionPoints(int body_width, int body_height)
{
	iBodyWidth = body_width;
	iBodyHeight = body_height;
	
	if (m_CollisionPoints.size() > 0) m_CollisionPoints.clear();
	
	m_CollisionPoints.resize(8);
	
	XMFLOAT2 points[8] =
	{
		{ (float)(iBodyWidth / 4), 0.0f }, { (float)(iBodyWidth - (iBodyWidth / 4)), 0.0f }, 		   						// Top of head
		{ (float)(iBodyWidth / 4), (float)iBodyHeight }, { (float)(iBodyWidth - (iBodyWidth / 4)), (float)iBodyHeight }, 	// Feet
		{ 0.0f, (float)(iBodyHeight / 4) }, { 0.0f, (float)(iBodyHeight - (iBodyHeight / 4)) },		   						// Left Arm
		{ (float)iBodyWidth, (float)(iBodyHeight / 4) }, { (float)iBodyWidth, (float)(iBodyHeight - (iBodyHeight / 4)) }	// Right Arm
	};
	
	for (int i = 0; i < 8; i++)
	{
		m_CollisionPoints[i] = XMFLOAT3(points[i].x, points[i].y, 0);
	}
	
	m_CenterOffset = XMFLOAT3(iBodyWidth / 2, iBodyHeight / 2, 0);
	m_HalfExtents = XMFLOAT3(iBodyWidth / 2, iBodyHeight / 2, 0);
}
//===============================================================================================================================
void PhysicsBodyBase2D::GenerateBody()
{
	m_body.x = m_HardboxPosition.x;
	m_body.y = m_HardboxPosition.y;
	m_body.width = iBodyWidth;
	m_body.height = iBodyHeight;
}
//===============================================================================================================================
void PhysicsBodyBase2D::InitHardboxMesh()
{
	ZShadeSandboxMesh::MeshParameters mp;
	mp.pos = XMFLOAT3(0, 0, 0);
	mp.rot = XMFLOAT3(0, 0, 0);
	mp.scale = XMFLOAT3(1, 1, 1);
	mp.rotationAxisX = false;
	mp.rotationAxisY = false;
	mp.rotationAxisZ = false;
	mp.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
	mp.useCustomShader = false;
	mp.material = MaterialManager::Instance()->GetMaterial("White");
	mp.in2D = true;
	m_HardboxMesh = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp, iBodyWidth, iBodyHeight, true);
}
//===============================================================================================================================
void PhysicsBodyBase2D::RenderHardboxMesh(Camera* camera, XMFLOAT2 offset)
{
	if (m_body.width <= 0) return;
	if (m_body.height <= 0) return;
	if (m_HardboxMesh == NULL) return;
	m_HardboxMesh->UpdateBuffers(m_body.x + offset.x, m_body.y + offset.y, m_body.width, m_body.height);
	ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.camera = camera;
	mrp.specifyWorld = true;
	mrp.world = XMMatrixIdentity();
	m_HardboxMesh->Render(mrp);
}
//===============================================================================================================================
void PhysicsBodyBase2D::MoveHardboxMesh(float newX, float newY)
{
	m_HardboxPosition.x = newX;
	m_HardboxPosition.y = newY;
	GenerateBody();
}
//===============================================================================================================================
void PhysicsBodyBase2D::ScaleHardboxMesh(int newWidth, int newHeight)
{
	iBodyWidth = newWidth;
	iBodyHeight = newHeight;
	GenerateBody();
	InitCollisionPoints(iBodyWidth, iBodyHeight);
}
//===============================================================================================================================