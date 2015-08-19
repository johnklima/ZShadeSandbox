#include "Room.h"
#include "MaterialManager.h"
#include "Vertex.h"
//==================================================================================================================================
//==================================================================================================================================
Room::Room(D3D* d3d)
:	m_D3DSystem(d3d)
{
	m_EngineOptions = d3d->GetEngineOptions();
}
//==================================================================================================================================
void Room::Render(ZShadeSandboxMesh::MeshRenderParameters mrp)
{
	float xPos, zPos;
	int i, j;
	
	int width = mWallMap.width();
	int height = mWallMap.height();
	
	// Render Floor
	
	// Need to use instancing to be able to render multiple of each at a different position
	
	for( i = 0,zPos = 5; i < height; i++ )
	{
		xPos = -5;
		
		for( j = 0; j < width; j++ )
		{
			
		}
	}
}
//==================================================================================================================================
void Room::SetWallMapSize(int w, int h)
{
	mWallMap.resize(w, h);
}
//==================================================================================================================================
void Room::AddWallMapValue(int x, int y, int value)
{
	mWallMap.insert(x, y, value);
}
//==================================================================================================================================
void Room::Build()
{
	//
	// Build walls
	//
	
	// Front facing wall
	{
		vector<ZShadeSandboxMesh::VertexTex> vertices(6);
		
		// First Triangle
		vertices[0] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(-1,  1, -1, 0, 0);
		vertices[1] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex( 1,  1, -1, 1, 0);
		vertices[2] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(-1, -1, -1, 0, 1);
		
		// Second Triangle
		vertices[3] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex( 1,  1, -1, 1, 0);
		vertices[4] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex( 1, -1, -1, 1, 1);
		vertices[5] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(-1, -1, -1, 0, 1);
		
		ConstructPolygon(vertices, mFrontWall);
	}
	
	// Left facing wall
	{
		vector<ZShadeSandboxMesh::VertexTex> vertices(6);
		
		// First Triangle
		vertices[0] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(1,  1, -1, 0, 0);
		vertices[1] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(1,  1,  1, 1, 0);
		vertices[2] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(1, -1, -1, 0, 1);
		
		// Second Triangle
		vertices[3] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(1,  1,  1, 1, 0);
		vertices[4] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(1, -1,  1, 1, 1);
		vertices[5] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(1, -1, -1, 0, 1);
		
		ConstructPolygon(vertices, mLeftWall);
	}
	
	// Right facing wall
	{
		vector<ZShadeSandboxMesh::VertexTex> vertices(6);
		
		// First Triangle
		vertices[0] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(-1,  1, -1, 1, 0);
		vertices[1] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(-1, -1,  1, 0, 1);
		vertices[2] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(-1,  1,  1, 0, 0);
		
		// Second Triangle
		vertices[3] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(-1,  1, -1, 1, 0);
		vertices[4] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(-1, -1, -1, 1, 1);
		vertices[5] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(-1, -1,  1, 0, 1);
		
		ConstructPolygon(vertices, mRightWall);
	}
	
	// Back facing wall
	{
		vector<ZShadeSandboxMesh::VertexTex> vertices(6);
		
		// First Triangle
		vertices[0] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex( 1,  1, 1, 0, 0);
		vertices[1] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(-1,  1, 1, 1, 0);
		vertices[2] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex( 1, -1, 1, 0, 1);
		
		// Second Triangle
		vertices[3] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex( 1, -1, 1, 0, 1);
		vertices[4] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(-1,  1, 1, 1, 0);
		vertices[5] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(-1, -1, 1, 1, 1);
		
		ConstructPolygon(vertices, mBackWall);
	}
	
	//
	// Ceiling
	//
	
	{
		vector<ZShadeSandboxMesh::VertexTex> vertices(6);
		
		// First Triangle
		vertices[0] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex( 1, 1,  1, 1, 1);
		vertices[1] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(-1, 1,  1, 0, 1);
		vertices[2] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(-1, 1, -1, 0, 0);
		
		// Second Triangle
		vertices[3] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(-1, 1, -1, 0, 0);
		vertices[4] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex( 1, 1, -1, 1, 0);
		vertices[5] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex( 1, 1,  1, 1, 1);
		
		ConstructPolygon(vertices, mCeiling);
	}
	
	//
	// Floor
	//
	
	{
		vector<ZShadeSandboxMesh::VertexTex> vertices(6);
		
		// First Triangle
		vertices[0] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(-1, 0,  1, 0, 0);
		vertices[1] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex( 1, 0,  1, 1, 0);
		vertices[2] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(-1, 0, -1, 0, 1);
		
		// Second Triangle
		vertices[3] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex( 1, 0,  1, 1, 0);
		vertices[4] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex( 1, 0, -1, 1, 1);
		vertices[5] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(-1, 0, -1, 0, 1);
		
		ConstructPolygon(vertices, mFloor);
	}
}
//==================================================================================================================================
void Room::ConstructPolygon(vector<ZShadeSandboxMesh::VertexTex> verts, ZShadeSandboxMesh::PolygonMesh*& poly)
{
	ZShadeSandboxMath::Polygon* polygon = new ZShadeSandboxMath::Polygon();
	
	polygon->Create(verts);
	
	polygon->Attributes()->mD3D = m_D3DSystem;
	
	ZShadeSandboxLighting::ShaderMaterial* material = MaterialManager::Instance()->GetMaterial("Wall");
	
	ZShadeSandboxMesh::PolygonMesh* polygonMesh = new ZShadeSandboxMesh::PolygonMesh(polygon, material);
	polygonMesh->Position() = XMFLOAT3(0, 0, 0);
	polygonMesh->Scale() = XMFLOAT3(1, 1, 1);
	
	polygonMesh->BuildVertexBuffer(ZShadeSandboxMesh::EVertexType::Type::VT_Tex);
	polygonMesh->BuildIndexBuffer();
	
	poly = polygonMesh;
}
//==================================================================================================================================
