#include "QuadTreeMesh.h"
#include "Convert.h"
#include "TextureManager.h"
#include "Triangle.h"
#include "ShapeContact.h"
#include "BoundingBox.h"
using ZShadeSandboxTerrain::QuadTreeMesh;
//==============================================================================================================================
//==============================================================================================================================
QuadTreeMesh::QuadTreeMesh(D3D* d3d, ZShadeSandboxTerrain::TerrainParameters tp, GameDirectory3D* gd3d)
:	m_d3d(d3d)
,	m_cameraCollided(false)
,	m_meshLoaded(false)
,	m_DefaultTextureName("checker.dds")
{
	m_EngineOptions = d3d->GetEngineOptions();

	// Create the Quad Tree
	m_QuadTree = new ZShadeSandboxTerrain::QuadTree(d3d, tp, gd3d);
	
	if (m_QuadTree->IsLoaded())
	{
		Init();
	}
}
//==============================================================================================================================
QuadTreeMesh::QuadTreeMesh(D3D* d3d, ZShadeSandboxTerrain::QuadTree* qtree)
:   m_QuadTree(qtree)
,   m_d3d(d3d)
,   m_cameraCollided(false)
,	m_meshLoaded(false)
,	m_DefaultTextureName("checker.dds")
{
	m_EngineOptions = d3d->GetEngineOptions();
	
	if (m_QuadTree->IsLoaded())
	{
		Init();
	}
}
//==============================================================================================================================
QuadTreeMesh::QuadTreeMesh(const QuadTreeMesh& qtree)
{
}
//==============================================================================================================================
QuadTreeMesh::~QuadTreeMesh()
{
}
//==============================================================================================================================
void QuadTreeMesh::Init()
{
	// Create the mesh for the Quad Tree
	BuildMeshNodes();
	GenerateMesh();
	//LatheCollisionMesh();

	// Create the collision objects for the mesh
	CreateNodeBox();
	CreateNodeSphere();
	
	ClearLayerMapNames();
	
	mMaterial = new ZShadeSandboxLighting::ShaderMaterial(m_d3d, "Terrain");
	
	// Add the default diffuse array SRV with default texture
	AddDiffuseLayerMapName(m_DefaultTextureName, 0);
	CreateDiffuseLayerMap();

	ComputeBounds(m_MeshNodes);
	
	m_meshLoaded = true;
}
//==============================================================================================================================
void QuadTreeMesh::ComputeBounds()
{
	ComputeBounds(m_MeshNodes);
}
//==============================================================================================================================
void QuadTreeMesh::BuildMeshNodes()
{
	m_maxWidth = m_QuadTree->MapSize() * m_QuadTree->LeafWidth();
	BuildMeshNodes(m_QuadTree->GetNodes(), m_maxWidth, m_MeshNodes, 0);
}
//==============================================================================================================================
void QuadTreeMesh::BuildMeshNodes(ZShadeSandboxTerrain::QNode* node, float width, ZShadeSandboxTerrain::QMeshNode*& meshNode, ZShadeSandboxTerrain::QMeshNode* parent)
{
	if (node == 0) return;
	
	meshNode = new ZShadeSandboxTerrain::QMeshNode();
	
	meshNode->children[0] = 0;
	meshNode->children[1] = 0;
	meshNode->children[2] = 0;
	meshNode->children[3] = 0;
	
	meshNode->boundingCoord[0] = node->boundingCoord[0];
	meshNode->boundingCoord[1] = node->boundingCoord[1];
	meshNode->boundingCoord[2] = node->boundingCoord[2];
	meshNode->boundingCoord[3] = node->boundingCoord[3];
	
	meshNode->branches[0] = node->branches[0];
	meshNode->branches[1] = node->branches[1];
	meshNode->branches[2] = node->branches[2];
	meshNode->branches[3] = node->branches[3];
	
	meshNode->center = XMFLOAT3(
		((meshNode->boundingCoord[2].x - meshNode->boundingCoord[1].x) / 2),
		0.0f,
		((meshNode->boundingCoord[3].z - meshNode->boundingCoord[1].z) / 2));
	
	meshNode->center.x = abs(meshNode->center.x);
	meshNode->center.y = abs(meshNode->center.y);
	meshNode->center.z = abs(meshNode->center.z);
	
	meshNode->bContainsCamera = false;
	meshNode->parent = parent;
	meshNode->m_triangle_count = 0;
	meshNode->fWidth = width;
	meshNode->boundary = node->boundary;
	meshNode->type = node->type;
	meshNode->nodeID = node->nodeID;
	meshNode->parentID = node->parentID;
	
	BuildMeshNodes(node->children[0], width / 2, meshNode->children[0], meshNode);
	BuildMeshNodes(node->children[1], width / 2, meshNode->children[1], meshNode);
	BuildMeshNodes(node->children[2], width / 2, meshNode->children[2], meshNode);
	BuildMeshNodes(node->children[3], width / 2, meshNode->children[3], meshNode);
}
//==============================================================================================================================
void QuadTreeMesh::CreateNodeBox()
{
	//
	// Create the cube
	//

	// Center the grid in model space
	int LeafWidth = m_QuadTree->LeafWidth();
	int TerrScale = m_QuadTree->TerrainScale();
	float halfWidth = ((float)LeafWidth - 1.0f) / 2.0f;
	float halfLength = ((float)LeafWidth - 1.0f) / 2.0f;

	XMFLOAT3 boxBase1 = m_MeshNodes->boundingCoord[0];
	XMFLOAT3 boxBase2 = m_MeshNodes->boundingCoord[1];
	XMFLOAT3 boxBase3 = m_MeshNodes->boundingCoord[2];
	XMFLOAT3 boxBase4 = m_MeshNodes->boundingCoord[3];

	// If this is not a leaf node we need to scale the box fit around all it's children
	float fScale = m_QuadTree->MapSize() * LeafWidth;// ((boxBase2.x - boxBase1.x) / 2) * m_QuadTree->LeafWidth();

	ZShadeSandboxMesh::MeshParameters mp;
	mp.pos = XMFLOAT3((boxBase1.x - halfWidth) * TerrScale, boxBase1.y, (boxBase1.z - halfWidth) * TerrScale);// (boxBase2.x - boxBase1.x) / 2, boxBase1.y, (boxBase3.z - boxBase1.z) / 2);
	mp.rot = XMFLOAT3(0, 0, 0);
	mp.scale = XMFLOAT3(fScale, fScale, fScale);
	mp.rotationAxisX = false;
	mp.rotationAxisY = false;
	mp.rotationAxisZ = false;
	mp.vertexType = ZShadeSandboxMesh::EVertexType::VT_Tex;
	mp.useCustomShader = false;
	mp.in2D = false;

	root_box_mesh = new ZShadeSandboxMesh::CubeMesh(m_d3d, mp);// boxBase1, boxBase2, boxBase3, boxBase4, 1024, mp);

	CreateNodeBox(m_MeshNodes);
}
//==============================================================================================================================
void QuadTreeMesh::CreateNodeBox(ZShadeSandboxTerrain::QMeshNode*& node)
{
	if (node == 0) return;

	//
	// Create the cube
	//

	// Center the grid in model space
	int TerrScale = m_QuadTree->TerrainScale();

	XMFLOAT3 boxBase1 = node->boundingCoord[0];
	XMFLOAT3 boxBase2 = node->boundingCoord[1];
	XMFLOAT3 boxBase3 = node->boundingCoord[2];
	XMFLOAT3 boxBase4 = node->boundingCoord[3];

	// If this is not a leaf node we need to scale the box fit around all it's children
	float fScale = 20;// ((boxBase2.x - boxBase1.x) / 2);// *m_QuadTree->LeafWidth();

	ZShadeSandboxMesh::MeshParameters mp;
	mp.pos = XMFLOAT3((boxBase1.x) * TerrScale, boxBase1.y, (boxBase1.z) * TerrScale);// (boxBase2.x - boxBase1.x) / 2, boxBase1.y, (boxBase3.z - boxBase1.z) / 2);
	mp.rot = XMFLOAT3(0, 0, 0);
	mp.scale = XMFLOAT3(fScale, fScale, fScale);
	mp.rotationAxisX = false;
	mp.rotationAxisY = false;
	mp.rotationAxisZ = false;
	mp.vertexType = ZShadeSandboxMesh::EVertexType::VT_Tex;
	mp.useCustomShader = false;
	mp.in2D = false;

	node->box_mesh = new ZShadeSandboxMesh::CubeMesh(m_d3d, mp);// boxBase1, boxBase2, boxBase3, boxBase4, 512, mp);

	// Generate boxes for the children
	if (node->type != ZShadeSandboxTerrain::EQuadNodeType::LEAF)
	{
		CreateNodeBox(node->children[0]);
		CreateNodeBox(node->children[1]);
		CreateNodeBox(node->children[2]);
		CreateNodeBox(node->children[3]);
	}
}
//==============================================================================================================================
void QuadTreeMesh::ReGenNodeBox()
{
	// Center the grid in model space
	int LeafWidth = m_QuadTree->LeafWidth();
	int TerrScale = m_QuadTree->TerrainScale();
	float halfWidth = ((float)LeafWidth - 1.0f) / 2.0f;
	float halfLength = ((float)LeafWidth - 1.0f) / 2.0f;

	XMFLOAT3 boxBase1 = m_MeshNodes->boundingCoord[0];
	XMFLOAT3 boxBase2 = m_MeshNodes->boundingCoord[1];
	XMFLOAT3 boxBase3 = m_MeshNodes->boundingCoord[2];
	XMFLOAT3 boxBase4 = m_MeshNodes->boundingCoord[3];

	root_box_mesh->Position() = XMFLOAT3((boxBase1.x - halfWidth) * TerrScale, boxBase1.y, (boxBase1.z - halfWidth) * TerrScale);

	ReGenNodeBox(m_MeshNodes);
}
//==============================================================================================================================
void QuadTreeMesh::ReGenNodeBox(ZShadeSandboxTerrain::QMeshNode*& node)
{
	if (node == 0) return;

	// Center the grid in model space
	int TerrScale = m_QuadTree->TerrainScale();

	XMFLOAT3 boxBase1 = node->boundingCoord[0];
	XMFLOAT3 boxBase2 = node->boundingCoord[1];
	XMFLOAT3 boxBase3 = node->boundingCoord[2];
	XMFLOAT3 boxBase4 = node->boundingCoord[3];

	node->box_mesh->Position() = XMFLOAT3((boxBase1.x) * TerrScale, boxBase1.y, (boxBase1.z) * TerrScale);

	// Generate boxes for the children
	if (node->type != ZShadeSandboxTerrain::EQuadNodeType::LEAF)
	{
		ReGenNodeBox(node->children[0]);
		ReGenNodeBox(node->children[1]);
		ReGenNodeBox(node->children[2]);
		ReGenNodeBox(node->children[3]);
	}
}
//==============================================================================================================================
void QuadTreeMesh::CreateNodeSphere()
{
	CreateNodeSphere(m_MeshNodes);
}
//==============================================================================================================================
void QuadTreeMesh::CreateNodeSphere(ZShadeSandboxTerrain::QMeshNode*& node)
{
	if (node == 0) return;

	// Find the center of this node which is the center of the box
	node->sphere.Set(node->center, 512);
	
	// Generate the sphere for the children
	if (node->type != ZShadeSandboxTerrain::EQuadNodeType::LEAF)
	{
		CreateNodeBox(node->children[0]);
		CreateNodeBox(node->children[1]);
		CreateNodeBox(node->children[2]);
		CreateNodeBox(node->children[3]);
	}
}
//==============================================================================================================================
void QuadTreeMesh::ReGenMesh(int newTerrainScale)
{
	m_QuadTree->TerrainScale() = newTerrainScale;

	ReGenNodeBox();

	ReGenMesh(m_MeshNodes);

	ComputeBounds(m_MeshNodes);
}
//==============================================================================================================================
void QuadTreeMesh::ReGenMesh(ZShadeSandboxTerrain::QMeshNode*& node)
{
	if (node == 0) return;

	int TerrScale = m_QuadTree->TerrainScale();
	
	// Recreate the AABB for the node
	node->boundary.vMin = XMFLOAT3((node->boundingCoord[0].x) * TerrScale, node->boundingCoord[0].y, (node->boundingCoord[0].z) * TerrScale);
	node->boundary.vMax = XMFLOAT3((node->boundingCoord[3].x) * TerrScale, node->boundingCoord[3].y, (node->boundingCoord[3].z) * TerrScale);

	switch (GetRenderPrimitive())
	{
		case ZShadeSandboxMesh::ERenderType::e4ControlPointPatchList:
			ReGenMesh4PointPatchTessellation(node);
		break;
		case ZShadeSandboxMesh::ERenderType::e3ControlPointPatchList:
			ReGenMesh3PointPatchTessellation(node);
		break;
		case ZShadeSandboxMesh::ERenderType::eTriangleStrip:
			ReGenMeshNT(node);
		break;
	}
	
	//Progress through the children
	ReGenMesh(node->children[0]);
	ReGenMesh(node->children[1]);
	ReGenMesh(node->children[2]);
	ReGenMesh(node->children[3]);
}
//==============================================================================================================================
void QuadTreeMesh::ReGenMeshNT(ZShadeSandboxTerrain::QMeshNode*& node)
{
	if (node == 0) return;
	if (node->type != ZShadeSandboxTerrain::EQuadNodeType::LEAF) return;

	int index = 0;
	float height = m_QuadTree->SeaLevel();
	int LeafWidth = m_QuadTree->LeafWidth();
	int TerrScale = m_QuadTree->TerrainScale();
	int MapSize = m_QuadTree->MapSize();
	
	int vert_count = (LeafWidth)* (LeafWidth);
	int index_count = ((LeafWidth)* 2) * (LeafWidth - 1) + (LeafWidth - 2);

#pragma region "Load Vertices"
	node->m_vertex_count = vert_count;

	node->vertices = new ZShadeSandboxMesh::VertexTex[vert_count];

	for (int z = (int)node->boundingCoord[0].z; z <= (int)node->boundingCoord[2].z; z++)
	{
		for (int x = (int)node->boundingCoord[0].x; x <= (int)node->boundingCoord[1].x; x++)
		{
			if (m_QuadTree->UsingHeight())
			{
				height = m_QuadTree->ReadHeight(x, z);
			}

			node->vertices[index].position = XMFLOAT3(((x) * TerrScale), height, ((z) * TerrScale));
			//node->vertices[index].normal = XMFLOAT3(0, 1, 0);
			node->vertices[index].texture = XMFLOAT2(x * (1.0f / (MapSize - 1)), z * (1.0f / (MapSize - 1)));

			index++;
		}
	}
#pragma endregion

	//GenerateBTT(node);

	// Regenerate the vertex buffer for the leaf node
	ReBuildVertexBuffer(node);
}
//==============================================================================================================================
void QuadTreeMesh::ReGenMesh4PointPatchTessellation(ZShadeSandboxTerrain::QMeshNode*& node)
{
	if (node == 0) return;
	if (node->type != ZShadeSandboxTerrain::EQuadNodeType::LEAF) return;

	int index = 0;
	int TerrScale = m_QuadTree->TerrainScale();
	int MapSize = m_QuadTree->MapSize();

	int vert_count = 4;
	int index_count = 4;

#pragma region "Load Vertices"
	node->m_vertex_count = vert_count;

	node->vertices = new ZShadeSandboxMesh::VertexTex[vert_count];

	float xPos[4];
	float yPos[4];
	float zPos[4];

	//Looks like I only need four vertices to pass to the tessellation shader
	for (int i = 0; i < 4; i++)
	{
		xPos[i] = (node->boundingCoord[i].x) * TerrScale;
		yPos[i] = node->boundingCoord[i].y;
		zPos[i] = (node->boundingCoord[i].z) * TerrScale;

		node->vertices[i].position = XMFLOAT3(xPos[i], yPos[i], zPos[i]);
		//node->vertices[i].normal = XMFLOAT3(0, 1, 0);
		node->vertices[i].texture = XMFLOAT2(node->boundingCoord[i].x * (1.0f / (MapSize - 1)), node->boundingCoord[i].z * (1.0f / (MapSize - 1)));
	}
#pragma endregion
	
	// Regenerate the vertex buffer for the leaf node
	ReBuildVertexBuffer(node);
}
//==============================================================================================================================
void QuadTreeMesh::ReGenMesh3PointPatchTessellation(ZShadeSandboxTerrain::QMeshNode*& node)
{
	if (node == 0) return;
	if (node->type != ZShadeSandboxTerrain::EQuadNodeType::LEAF) return;

	int index = 0;
	float height = m_QuadTree->SeaLevel();
	int TerrScale = m_QuadTree->TerrainScale();
	int MapSize = m_QuadTree->MapSize();

	XMFLOAT3 nodeBounds0 = XMFLOAT3((node->boundingCoord[0].x) * TerrScale, node->boundingCoord[0].y, (node->boundingCoord[0].z) * TerrScale);
	XMFLOAT3 nodeBounds1 = XMFLOAT3((node->boundingCoord[1].x) * TerrScale, node->boundingCoord[1].y, (node->boundingCoord[1].z) * TerrScale);
	XMFLOAT3 nodeBounds2 = XMFLOAT3((node->boundingCoord[2].x) * TerrScale, node->boundingCoord[2].y, (node->boundingCoord[2].z) * TerrScale);
	XMFLOAT3 nodeBounds3 = XMFLOAT3((node->boundingCoord[3].x) * TerrScale, node->boundingCoord[3].y, (node->boundingCoord[3].z) * TerrScale);

	ZShadeSandboxMath::XMMath3 v1[3], v2[3];

	v1[0].x = nodeBounds0.x;
	v1[0].y = nodeBounds0.y;
	v1[0].z = nodeBounds0.z;

	v1[1].x = nodeBounds3.x;
	v1[1].y = nodeBounds3.y;
	v1[1].z = nodeBounds3.z;

	v1[2].x = nodeBounds1.x;
	v1[2].y = nodeBounds1.y;
	v1[2].z = nodeBounds1.z;

	v2[0].x = nodeBounds0.x;
	v2[0].y = nodeBounds0.y;
	v2[0].z = nodeBounds0.z;

	v2[1].x = nodeBounds2.x;
	v2[1].y = nodeBounds2.y;
	v2[1].z = nodeBounds2.z;

	v2[2].x = nodeBounds3.x;
	v2[2].y = nodeBounds3.y;
	v2[2].z = nodeBounds3.z;

	UINT i1[3], i2[3];

	i1[0] = 0;
	i1[1] = 1;
	i1[2] = 2;

	i2[0] = 0;
	i2[1] = 2;
	i2[2] = 3;

	// Create two triangles based on the bounds for this mesh
	Triangle* tri1 = new Triangle(v1, i1);
	Triangle* tri2 = new Triangle(v2, i2);

	node->m_vertex_count = 6;
	node->vertices = new ZShadeSandboxMesh::VertexTex[node->m_vertex_count];

	node->vertices[0].position = XMFLOAT3(tri1->GetVertex()[0].x, tri1->GetVertex()[0].y, tri1->GetVertex()[0].z);
	node->vertices[0].texture = XMFLOAT2(nodeBounds0.x * (1.0f / (MapSize - 1)), nodeBounds0.z * (1.0f / (MapSize - 1)));

	node->vertices[1].position = XMFLOAT3(tri1->GetVertex()[1].x, tri1->GetVertex()[1].y, tri1->GetVertex()[1].z);
	node->vertices[1].texture = XMFLOAT2(nodeBounds3.x * (1.0f / (MapSize - 1)), nodeBounds3.z * (1.0f / (MapSize - 1)));

	node->vertices[2].position = XMFLOAT3(tri1->GetVertex()[2].x, tri1->GetVertex()[2].y, tri1->GetVertex()[2].z);
	node->vertices[2].texture = XMFLOAT2(nodeBounds1.x * (1.0f / (MapSize - 1)), nodeBounds1.z * (1.0f / (MapSize - 1)));

	node->vertices[3].position = XMFLOAT3(tri2->GetVertex()[0].x, tri2->GetVertex()[0].y, tri2->GetVertex()[0].z);
	node->vertices[3].texture = XMFLOAT2(nodeBounds0.x * (1.0f / (MapSize - 1)), nodeBounds0.z * (1.0f / (MapSize - 1)));

	node->vertices[4].position = XMFLOAT3(tri2->GetVertex()[1].x, tri2->GetVertex()[1].y, tri2->GetVertex()[1].z);
	node->vertices[4].texture = XMFLOAT2(nodeBounds2.x * (1.0f / (MapSize - 1)), nodeBounds2.z * (1.0f / (MapSize - 1)));

	node->vertices[5].position = XMFLOAT3(tri2->GetVertex()[2].x, tri2->GetVertex()[2].y, tri2->GetVertex()[2].z);
	node->vertices[5].texture = XMFLOAT2(nodeBounds3.x * (1.0f / (MapSize - 1)), nodeBounds3.z * (1.0f / (MapSize - 1)));

	// Regenerate the vertex buffer for the leaf node
	ReBuildVertexBuffer(node);
}
//==============================================================================================================================
void QuadTreeMesh::ReBuildVertexBuffer(ZShadeSandboxTerrain::QMeshNode*& node)
{
	D3D11_MAPPED_SUBRESOURCE resource;
	m_d3d->GetDeviceContext()->Map(node->m_vertex_buffer11, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	{
		memcpy(resource.pData, node->vertices, sizeof(ZShadeSandboxMesh::VertexTex) * node->m_vertex_count);
	}
	m_d3d->GetDeviceContext()->Unmap(node->m_vertex_buffer11, 0);
}
//==============================================================================================================================
void QuadTreeMesh::ReBuildIndexBuffer(ZShadeSandboxTerrain::QMeshNode*& node)
{
	D3D11_MAPPED_SUBRESOURCE resource;
	m_d3d->GetDeviceContext()->Map(node->m_index_buffer11, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	{
		memcpy(resource.pData, node->indices, sizeof(unsigned long) * node->m_index_count);
	}
	m_d3d->GetDeviceContext()->Unmap(node->m_index_buffer11, 0);
}
//==============================================================================================================================
void QuadTreeMesh::ClearMesh(bool removeIndices)
{
	ClearMesh(m_MeshNodes, removeIndices);
}
//==============================================================================================================================
void QuadTreeMesh::ClearMesh(ZShadeSandboxTerrain::QMeshNode*& node, bool removeIndices)
{
	if (node == 0) return;

	if (node->type == ZShadeSandboxTerrain::EQuadNodeType::LEAF)
	{
		delete[] node->vertices;

		if (removeIndices)
			delete[] node->indices;
	}
	else
	{
		//Progress through the children
		ClearMesh(node->children[0], removeIndices);
		ClearMesh(node->children[1], removeIndices);
		ClearMesh(node->children[2], removeIndices);
		ClearMesh(node->children[3], removeIndices);
	}
}
//==============================================================================================================================
void QuadTreeMesh::GenerateMesh()
{
	GenerateMesh(m_MeshNodes);
}
//==============================================================================================================================
void QuadTreeMesh::GenerateMesh(ZShadeSandboxTerrain::QMeshNode*& node)
{
	if (node == 0) return;
	
	switch (GetRenderPrimitive())
	{
		case ZShadeSandboxMesh::ERenderType::e4ControlPointPatchList:
			GenerateMesh4PointPatchTessellation(node);
		break;
		case ZShadeSandboxMesh::ERenderType::e3ControlPointPatchList:
			GenerateMesh3PointPatchTessellation(node);
		break;
		case ZShadeSandboxMesh::ERenderType::eTriangleStrip:
			GenerateMeshNT(node);
		break;
	}
	
	//Progress through the children
	GenerateMesh(node->children[0]);
	GenerateMesh(node->children[1]);
	GenerateMesh(node->children[2]);
	GenerateMesh(node->children[3]);
}
//==============================================================================================================================
void QuadTreeMesh::GenerateMeshNT(ZShadeSandboxTerrain::QMeshNode*& node)
{
	if (node == 0) return;
	if (node->type != ZShadeSandboxTerrain::EQuadNodeType::LEAF) return;

	int index = 0;
	float height = m_QuadTree->SeaLevel();
	int LeafWidth = m_QuadTree->LeafWidth();
	int TerrScale = m_QuadTree->TerrainScale();
	int MapSize = m_QuadTree->MapSize();

	int vert_count = (LeafWidth) * (LeafWidth);
	int index_count = ((LeafWidth) * 2) * (LeafWidth - 1) + (LeafWidth - 2);
	
#pragma region "Load Vertices"
	node->m_vertex_count = vert_count;
	node->m_triangle_count = node->m_vertex_count / 3;
	node->vertices = new ZShadeSandboxMesh::VertexTex[vert_count];

	for (int z = (int)node->boundingCoord[0].z; z <= (int)node->boundingCoord[2].z; z++)
	{
		for (int x = (int)node->boundingCoord[0].x; x <= (int)node->boundingCoord[1].x; x++)
		{
			if (m_QuadTree->UsingHeight())
			{
				/*if (x < MapSize && z < MapSize)
				{
					if (m_QuadTree->GetMapExt() == EHeightExtension::eRaw)
						height = m_QuadTree->SampleHeight(x + (z * MapSize + 1));
					else
						height = m_QuadTree->SampleHeight(x, z);
				}*/
				
				height = m_QuadTree->ReadHeight(x, z);
			}

			node->vertices[index].position = XMFLOAT3(((x) * TerrScale), height, ((z) * TerrScale));
			//node->vertices[index].normal = XMFLOAT3(0, 1, 0);
			node->vertices[index].texture = XMFLOAT2(x * (1.0f / (MapSize - 1)), z * (1.0f / (MapSize - 1)));

			index++;
		}
	}
#pragma endregion
#pragma region "Load Indices"
	index = 0;
	
	node->m_index_count = index_count;
	node->indices = new unsigned long[index_count];

	for (int z = 0; z < LeafWidth - 1; z++)
	{
		//Even rows move left to right, odd rows right to left
		if (z % 2 == 0)
		{
			//Even row
			int x;
			for (x = 0; x < LeafWidth; x++)
			{
				node->indices[index++] = x + (z * LeafWidth);
				node->indices[index++] = x + (z * LeafWidth) + LeafWidth;
			}
			//Insert degenerate vertex if this isn't the last row
			if (z != LeafWidth - 2)
			{
				node->indices[index++] = --x + (z * LeafWidth);
			}
		}
		else
		{
			//Odd row
			int x;
			for (x = LeafWidth - 1; x >= 0; x--)
			{
				node->indices[index++] = x + (z * LeafWidth);
				node->indices[index++] = x + (z * LeafWidth) + LeafWidth;
			}
			//Insert degenerate vertex if this isn't the last row
			if (z != LeafWidth - 2)
			{
				node->indices[index++] = ++x + (z * LeafWidth);
			}
		}
	}
#pragma endregion

	// Build the vertex and index buffers for the leaf node
	BuildVertexBuffer( node );
	BuildIndexBuffer( node );
}
//==============================================================================================================================
void QuadTreeMesh::GenerateMesh4PointPatchTessellation(ZShadeSandboxTerrain::QMeshNode*& node)
{
	if (node == 0) return;
	if (node->type != ZShadeSandboxTerrain::EQuadNodeType::LEAF) return;

	int index = 0;
	int TerrScale = m_QuadTree->TerrainScale();
	int MapSize = m_QuadTree->MapSize();

	int vert_count = 4;
	int index_count = 4;

#pragma region "Load Vertices"
	node->m_vertex_count = vert_count;
	node->m_triangle_count = node->m_vertex_count / 3;
	node->vertices = new ZShadeSandboxMesh::VertexTex[vert_count];
	
	//Looks like I only need four vertices to pass to the tessellation shader
	for (int i = 0; i < 4; i++)
	{
		float x = node->boundingCoord[i].x * TerrScale;
		float y = node->boundingCoord[i].y;
		float z = node->boundingCoord[i].z * TerrScale;

		node->vertices[i].position = XMFLOAT3(x, y, z);
		node->vertices[i].texture = XMFLOAT2(node->boundingCoord[i].x * (1.0f / (MapSize - 1)), node->boundingCoord[i].z * (1.0f / (MapSize - 1)));
	}
#pragma endregion
#pragma region "Load Indices"
	node->m_index_count = index_count;
	node->indices = new unsigned long[index_count];
	node->indices[0] = 1;
	node->indices[1] = 0;
	node->indices[2] = 3;
	node->indices[3] = 2;
#pragma endregion
	
	// Build the vertex and index buffers for the leaf node
	BuildVertexBuffer( node );
	BuildIndexBuffer( node );
	
	// Add the internal triangles to the node
	//LatheInternalTriangles(node);
}
//==============================================================================================================================
void QuadTreeMesh::GenerateMesh3PointPatchTessellation(ZShadeSandboxTerrain::QMeshNode*& node)
{
	if (node == 0) return;
	if (node->type != ZShadeSandboxTerrain::EQuadNodeType::LEAF) return;

	int index = 0;
	float height = m_QuadTree->SeaLevel();
	int TerrScale = m_QuadTree->TerrainScale();
	int MapSize = m_QuadTree->MapSize();

	XMFLOAT3 nodeBounds0 = XMFLOAT3((node->boundingCoord[0].x) * TerrScale, node->boundingCoord[0].y, (node->boundingCoord[0].z) * TerrScale);
	XMFLOAT3 nodeBounds1 = XMFLOAT3((node->boundingCoord[1].x) * TerrScale, node->boundingCoord[1].y, (node->boundingCoord[1].z) * TerrScale);
	XMFLOAT3 nodeBounds2 = XMFLOAT3((node->boundingCoord[2].x) * TerrScale, node->boundingCoord[2].y, (node->boundingCoord[2].z) * TerrScale);
	XMFLOAT3 nodeBounds3 = XMFLOAT3((node->boundingCoord[3].x) * TerrScale, node->boundingCoord[3].y, (node->boundingCoord[3].z) * TerrScale);

	ZShadeSandboxMath::XMMath3 v1[3], v2[3];

	v1[0].x = nodeBounds0.x;
	v1[0].y = nodeBounds0.y;
	v1[0].z = nodeBounds0.z;

	v1[1].x = nodeBounds3.x;
	v1[1].y = nodeBounds3.y;
	v1[1].z = nodeBounds3.z;

	v1[2].x = nodeBounds1.x;
	v1[2].y = nodeBounds1.y;
	v1[2].z = nodeBounds1.z;

	v2[0].x = nodeBounds0.x;
	v2[0].y = nodeBounds0.y;
	v2[0].z = nodeBounds0.z;

	v2[1].x = nodeBounds2.x;
	v2[1].y = nodeBounds2.y;
	v2[1].z = nodeBounds2.z;

	v2[2].x = nodeBounds3.x;
	v2[2].y = nodeBounds3.y;
	v2[2].z = nodeBounds3.z;

	UINT i1[3], i2[3];

	i1[0] = 0;
	i1[1] = 1;
	i1[2] = 2;

	i2[0] = 3;
	i2[1] = 4;
	i2[2] = 5;

	// Create two triangles based on the bounds for this mesh
	Triangle* tri1 = new Triangle(v1, i1);
	Triangle* tri2 = new Triangle(v2, i2);

	// There are two triangles in the vertex list
	node->m_triangle_count = 2;

	node->m_vertex_count = 6;
	node->vertices = new ZShadeSandboxMesh::VertexTex[node->m_vertex_count];

	node->vertices[0].position = XMFLOAT3(tri1->GetVertex()[0].x, tri1->GetVertex()[0].y, tri1->GetVertex()[0].z);
	node->vertices[0].texture = XMFLOAT2(nodeBounds0.x * (1.0f / (MapSize - 1)), nodeBounds0.z * (1.0f / (MapSize - 1)));

	node->vertices[1].position = XMFLOAT3(tri1->GetVertex()[1].x, tri1->GetVertex()[1].y, tri1->GetVertex()[1].z);
	node->vertices[1].texture = XMFLOAT2(nodeBounds3.x * (1.0f / (MapSize - 1)), nodeBounds3.z * (1.0f / (MapSize - 1)));

	node->vertices[2].position = XMFLOAT3(tri1->GetVertex()[2].x, tri1->GetVertex()[2].y, tri1->GetVertex()[2].z);
	node->vertices[2].texture = XMFLOAT2(nodeBounds1.x * (1.0f / (MapSize - 1)), nodeBounds1.z * (1.0f / (MapSize - 1)));

	node->vertices[3].position = XMFLOAT3(tri2->GetVertex()[0].x, tri2->GetVertex()[0].y, tri2->GetVertex()[0].z);
	node->vertices[3].texture = XMFLOAT2(nodeBounds0.x * (1.0f / (MapSize - 1)), nodeBounds0.z * (1.0f / (MapSize - 1)));

	node->vertices[4].position = XMFLOAT3(tri2->GetVertex()[1].x, tri2->GetVertex()[1].y, tri2->GetVertex()[1].z);
	node->vertices[4].texture = XMFLOAT2(nodeBounds2.x * (1.0f / (MapSize - 1)), nodeBounds2.z * (1.0f / (MapSize - 1)));

	node->vertices[5].position = XMFLOAT3(tri2->GetVertex()[2].x, tri2->GetVertex()[2].y, tri2->GetVertex()[2].z);
	node->vertices[5].texture = XMFLOAT2(nodeBounds3.x * (1.0f / (MapSize - 1)), nodeBounds3.z * (1.0f / (MapSize - 1)));

	node->m_index_count = 6;
	node->indices = new unsigned long[node->m_index_count];

	node->indices[0] = tri1->Indices()[0];
	node->indices[1] = tri1->Indices()[1];
	node->indices[2] = tri1->Indices()[2];

	node->indices[3] = tri2->Indices()[0];
	node->indices[4] = tri2->Indices()[1];
	node->indices[5] = tri2->Indices()[2];

	// Build the vertex and index buffers for the leaf node
	BuildVertexBuffer(node);
	BuildIndexBuffer(node);
	
	// Add the internal triangles to the node
	//LatheInternalTriangles(node);
}
//==============================================================================================================================
void QuadTreeMesh::BuildVertexBuffer(ZShadeSandboxTerrain::QMeshNode*& node)
{
	if (m_d3d == 0) return;
	if (m_EngineOptions == 0) return;

	HRESULT result;
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;

	//Setup the description of the static vertex buffer
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(ZShadeSandboxMesh::VertexTex) * node->m_vertex_count;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	//Give the subresource a pointer to the vertex data
	vertexData.pSysMem = node->vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	//Create the vertex buffer
	result = m_d3d->GetDevice11()->CreateBuffer(&vertexBufferDesc, &vertexData, &node->m_vertex_buffer11);
	if (FAILED(result)) return;
}
//==============================================================================================================================
void QuadTreeMesh::BuildIndexBuffer(ZShadeSandboxTerrain::QMeshNode*& node)
{
	if (m_d3d == 0) return;
	if (m_EngineOptions == 0) return;

	HRESULT result;
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA indexData;

	//Setup the description of the static index buffer
	indexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * node->m_index_count;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	//Give the subresource a pointer to the index data
	indexData.pSysMem = node->indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	//Create the index buffer
	result = m_d3d->GetDevice11()->CreateBuffer(&indexBufferDesc, &indexData, &node->m_index_buffer11);
	if (FAILED(result)) return;
}
//==============================================================================================================================
bool QuadTreeMesh::GetHeightAtPosition(XMFLOAT3 position, float& height)
{
	return GetHeightAtPosition(m_MeshNodes, position, height);
}
//==============================================================================================================================
bool QuadTreeMesh::GetHeightAtPosition(ZShadeSandboxTerrain::QMeshNode* node, XMFLOAT3 position, float& height)
{
	if (node == 0) return false;
	if (!node->boundary.ContainsPoint3DOmitY(position)) return false;

	// Bilinearly sample the heightmap.

	int horizontalScale = m_QuadTree->TerrainScale();
	float x = position.x / horizontalScale;
	float z = position.z / horizontalScale;
	int ix = (int) x;
	int iz = (int) z;
	float fx = x - ix;
	float fz = z - iz;

	float w00 = (1.0f - fx) * (1.0f - fz);
	float w10 = fx * (1.0f - fz);
	float w01 = (1.0f - fx) * fz;
	float w11 = fx * fz;

	float h00 = m_QuadTree->ReadHeight(ix, iz);
	float h01 = m_QuadTree->ReadHeight(ix, iz + 1);
	float h10 = m_QuadTree->ReadHeight(ix + 1, iz);
	float h11 = m_QuadTree->ReadHeight(ix + 1, iz + 1);

	float h = w00 * h00 + w01 * h01 + w10 * h10 + w11 * h11;
	height = ((h * m_heightScale * 100.0f) / 255.0) / ((m_terrainZScale * 2) + 1);
	return true;
}
//==============================================================================================================================
void QuadTreeMesh::Intersects(ZShadeSandboxMath::Ray ray, bool& hit, XMFLOAT3& hitPoint)
{
	Intersects(m_MeshNodes, ray, hit, hitPoint);
}
//==============================================================================================================================
void QuadTreeMesh::Intersects(ZShadeSandboxTerrain::QMeshNode* node, ZShadeSandboxMath::Ray ray, bool& hit, XMFLOAT3& hitPoint)
{
	hitPoint = XMFLOAT3(FLT_MAX, FLT_MAX, FLT_MAX);
	
	if (node == 0) return;
	
	// This is our terminating condition
	if (node->type == ZShadeSandboxTerrain::EQuadNodeType::LEAF)
	{
		float d;

		// Check if the ray intersects this leaf node's AABB
		//if (!node->boundary.Intersects(ray, d))
		if (!ShapeContact::RayIntersectsAABB(ray, node->boundary, d))
		{
			// No Intersection
			hit = false;

			return;
		}

		// Return the center point of the leaf's bounding box
		hitPoint.x = (node->boundary.vMin.x + node->boundary.vMax.x) / 2;
		hitPoint.y = (node->boundary.vMin.y + node->boundary.vMax.y) / 2;
		hitPoint.z = (node->boundary.vMin.z + node->boundary.vMax.z) / 2;

		int TerrScale = m_QuadTree->TerrainScale();

		// Locate the contact point on this node
		float h = m_QuadTree->SeaLevel();
		for (int z = (int)node->boundingCoord[0].z; z <= (int)node->boundingCoord[2].z; z++)
		{
			for (int x = (int)node->boundingCoord[0].x; x <= (int)node->boundingCoord[1].x; x++)
			{
				if (m_QuadTree->UsingHeight())
				{
					h = m_QuadTree->ReadHeight(x, z);
					h = ((h * m_heightScale * 100.0f) / 255.0) / ((m_terrainZScale * 2) + 1);
				}

				XMFLOAT3 point(x * TerrScale, h, z * TerrScale);
				ZShadeSandboxMath::SpherePrimitive sphere(point, 2);

				if (ShapeContact::RayIntersectsSphere(ray, sphere, d))
				{
					hitPoint = point;
					hit = true;
					return;
				}
			}
		}

		hit = true;
		return;
	}

	// If the node has children intersect each child
	std::map<float, ZShadeSandboxTerrain::QMeshNode*> pq;
	for (int i = 0; i < 4; i++)
	{
		float cd;

		ZShadeSandboxTerrain::QMeshNode* n = node->children[i];
		
		if (n != NULL)
		{
			//if (ray.Intersects(n->boundary, cd))
			if (ShapeContact::RayIntersectsAABB(ray, n->boundary, cd))
			{
				while (pq.find(cd) != pq.end())
				{
					// Perturb things slightly so that we don't have duplicate keys
					cd += ZShadeSandboxMath::ZMath::RandF<float>(-0.001f, 0.001f);
				}

				pq.insert(make_pair(cd, n));
			}
		}
	}
	
	// There were no child intersections
	if (pq.size() <= 0)
	{
		hit = false;
		
		return;
	}

	// check the child intersections for the nearest intersection
	bool intersect = false;

	// setup a very-far away intersection point to compare against
	XMFLOAT3 bestHit;

	bestHit.x = ray.position.x + ray.direction.x * 1000;
	bestHit.y = ray.position.y + ray.direction.y * 1000;
	bestHit.z = ray.position.z + ray.direction.z * 1000;

	std::map<float, ZShadeSandboxTerrain::QMeshNode*>::iterator pq_iter = pq.begin();

	for (; pq_iter != pq.end(); pq_iter++)
	{
		ZShadeSandboxTerrain::QMeshNode* n = (*pq_iter).second;

		XMFLOAT3 thisHit;

		// Intersect each child recursively
		bool wasHit = false;
		
		Intersects(n, ray, wasHit, thisHit);

		if (!wasHit)
		{
			// No intersection, continue and intersect other children
			continue;
		}
		
		// Make sure that the intersection point is in front of the ray's world-space origin
		ZShadeSandboxMath::XMMath3 thisHitXM(thisHit);
		ZShadeSandboxMath::XMMath3 bestHitXM(bestHit);
		ZShadeSandboxMath::XMMath3 rayPosXM(ray.position);
		ZShadeSandboxMath::XMMath3 rayDirXM(ray.direction);
		
		ZShadeSandboxMath::XMMath3 dotPos(thisHitXM - rayPosXM);
		dotPos.NormalizeMe();

		if (!(dotPos.Dot(ray.direction) > 0.9f))
			continue;

		// Check that the intersection is closer than the nearest intersection found thus far
		if (!((rayPosXM - thisHitXM).LengthSquared() < (rayPosXM - bestHitXM).LengthSquared()))
		{
			continue;
		}

		// If we found a closer intersection store the new closest intersection
		bestHit = thisHit;

		intersect = true;
	}

	// Best hit now contains the closest intersection found, or the distant sentinel value
	hitPoint = bestHit;

	hit = intersect;
}
//==============================================================================================================================
void QuadTreeMesh::GenerateHeightQuad(XMFLOAT3 point, int areaSize, ID3D11ShaderResourceView*& heightAreaSRV)
{
	// Decompose the point since it was based on getting the height from a position
	XMFLOAT3 p;// = point;
	DecomposePoint(point, p);

	vector<float> heightList;
	
	if (m_QuadTree->UsingHeight())
	{
		// Find the node that contains this point and get all the nodes around it
		GenerateHeightQuad(m_MeshNodes, p, areaSize, heightList);
	}
	else
	{
		// Not using height so the mesh is flat at sea level
		if (areaSize > 0)
		{
			for (int y = 0; y < areaSize; y++)
			{
				for (int x = 0; x < areaSize; x++)
				{
					heightList.push_back(m_QuadTree->SeaLevel());
				}
			}
		}
	}
	
	// Build the heightAreaSRV from the area chosen
	if (areaSize > 0 && heightList.size() > 0)
	{
		D3D11_TEXTURE2D_DESC texDesc;
		texDesc.Width = areaSize;
		texDesc.Height = areaSize;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_R16_FLOAT;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;
		
		vector<float> hv = heightList;
		// HALF is defined in xnamath.h, for storing 16-bit float.
		std::vector<HALF> hmap(hv.size());
		std::transform(hv.begin(), hv.end(), hmap.begin(), XMConvertFloatToHalf);
		
		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = &hmap[0];
		data.SysMemPitch = areaSize * sizeof(HALF);
		data.SysMemSlicePitch = 0;

		ID3D11Texture2D* hmapTex = 0;
		m_d3d->GetDevice11()->CreateTexture2D(&texDesc, &data, &hmapTex);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = texDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = -1;
		m_d3d->GetDevice11()->CreateShaderResourceView(hmapTex, &srvDesc, &heightAreaSRV);

		// SRV saves reference.
		SAFE_RELEASE(hmapTex);
	}
	else
	{
		heightAreaSRV = NULL;
	}
}
//==============================================================================================================================
void QuadTreeMesh::GenerateHeightQuad(ZShadeSandboxTerrain::QMeshNode* node, XMFLOAT3 point, int areaSize, vector<float>& heightList)
{
	if (node == 0) return;
	
	if (node->type == ZShadeSandboxTerrain::EQuadNodeType::LEAF)
	{
		//ZShadeSandboxMath::BoundingBox bb;
		//bb.x = point.x;
		//bb.y = point.z; // Not using height since the terrain is xz
		//bb.width = areaSize;
		//bb.height = areaSize;
		
		ZShadeSandboxMath::AABB aabb;
		aabb.Construct(point, XMFLOAT3(areaSize, 2, areaSize));

		float h = m_QuadTree->SeaLevel();
		
		for (int z = (int)node->boundingCoord[0].z; z <= (int)node->boundingCoord[2].z; z++)
		{
			for (int x = (int)node->boundingCoord[0].x; x <= (int)node->boundingCoord[1].x; x++)
			{
				if (m_QuadTree->UsingHeight())
				{
					h = m_QuadTree->ReadHeight(x, z);
					h = ((h * m_heightScale * 100.0f) / 255.0) / ((m_terrainZScale * 2) + 1);
				}
				
				XMFLOAT3 p(x, h, z);
				
				// If the point is inside the bounding box then add the height to the height list
				if (aabb.ContainsPoint3DOmitY(p))
				{
					heightList.push_back(h);
				}
			}
		}
	}
	
	//
	// Search the children to build the height list
	//
	
	GenerateHeightQuad(node->children[0], point, areaSize, heightList);
	GenerateHeightQuad(node->children[1], point, areaSize, heightList);
	GenerateHeightQuad(node->children[2], point, areaSize, heightList);
	GenerateHeightQuad(node->children[3], point, areaSize, heightList);
}
//==============================================================================================================================
void QuadTreeMesh::DecomposePoint(XMFLOAT3 pointIn, XMFLOAT3& pointOut)
{
	int TerrScale = m_QuadTree->TerrainScale();
	
	float x = (pointIn.x / TerrScale);
	float y = pointIn.y;
	float z = (pointIn.z / TerrScale);
	
	pointOut = XMFLOAT3(x, y, z);
}
//==============================================================================================================================
void QuadTreeMesh::TransformPoint(XMFLOAT3 pointIn, XMFLOAT3& pointOut)
{
	int TerrScale = m_QuadTree->TerrainScale();

	float x = (pointIn.x) * TerrScale;
	float y = pointIn.y;
	float z = (pointIn.z) * TerrScale;

	pointOut = XMFLOAT3(x, y, z);
}
//==============================================================================================================================
void QuadTreeMesh::AddMaterialColors(XMFLOAT4 ambient, XMFLOAT4 diffuse, XMFLOAT4 specular)
{
	AddDiffuseColor(diffuse);
	AddAmbientColor(ambient);
	AddSpecularColor(specular);
}
//==============================================================================================================================
void QuadTreeMesh::AddDiffuseColor(XMFLOAT4 diffuse)
{
	mMaterial->vDiffuseColor = diffuse;
}
//==============================================================================================================================
void QuadTreeMesh::AddAmbientColor(XMFLOAT4 ambient)
{
	mMaterial->vAmbientColor = ambient;
}
//==============================================================================================================================
void QuadTreeMesh::AddSpecularColor(XMFLOAT4 specular)
{
	mMaterial->vSpecularColor = specular;
}
//==============================================================================================================================
void QuadTreeMesh::AddSpecularPower(float power)
{
	mMaterial->fSpecularPower = power;
}
//==============================================================================================================================
void QuadTreeMesh::AddSpecularIntensity(float intensity)
{
	mMaterial->fSpecularIntensity = intensity;
}
//================================================================================================================
void QuadTreeMesh::ClearLayerMapNames()
{
	if (mLayerTextureNames.size() > 0)
		mLayerTextureNames.clear();
	mLayerTextureNames.resize(5);
}
//================================================================================================================
void QuadTreeMesh::AddDiffuseLayerMapName(string textureName, int position)
{
	string fullTexName = m_QuadTree->GetGD3D()->m_textures_path + "\\" + textureName;
	mLayerTextureNames[position] = fullTexName;
}
//================================================================================================================
void QuadTreeMesh::CreateDiffuseLayerMap()
{
	vector<string> names;
	for (int i = 0; i < 5; i++)
	{
		if (mLayerTextureNames[i] != "")
		{
			names.push_back(mLayerTextureNames[i]);
		}
	}
	mMaterial->CreateTexture2DArray(names);
}
//================================================================================================================
void QuadTreeMesh::AddBlendMap(string basePath, string blendMapFilename)
{
	mMaterial->AddBlendMapTexture(basePath, blendMapFilename);
}
//================================================================================================================
void QuadTreeMesh::AddNormalMap(string basePath, string normalMapFilename)
{
	mMaterial->AddNormalMapTexture(basePath, normalMapFilename);
}
//================================================================================================================
void QuadTreeMesh::AddDetailMap(string basePath, string detailMapFilename)
{
	mMaterial->AddDetailMapTexture(basePath, detailMapFilename);
}
//================================================================================================================
void QuadTreeMesh::ComputeBounds(ZShadeSandboxTerrain::QMeshNode* node)
{
	assert(node != nullptr);

	XMFLOAT3 vmin(FLT_MAX, FLT_MAX, FLT_MAX);
	XMFLOAT3 vmax(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	if (node->type == ZShadeSandboxTerrain::EQuadNodeType::LEAF)
	{
		for (int i = 0; i < node->m_vertex_count; ++i)
		{
			XMFLOAT3& vertex = node->vertices[i].position;
			vmin.x = fminf(vmin.x, vertex.x);
			vmin.y = fminf(vmin.y, vertex.y);
			vmin.z = fminf(vmin.z, vertex.z);
			vmax.x = fmaxf(vmax.x, vertex.x);
			vmax.y = fmaxf(vmax.y, vertex.y);
			vmax.z = fmaxf(vmax.z, vertex.z);
		}

		for (int x = (int) vmin.x; x < (int) vmax.x; ++x)
		{
			for (int z = (int) vmin.z; z < (int) vmax.x; ++z)
			{
				float height = m_QuadTree->ReadHeight(x, z);
				height = ((height * m_heightScale * 100.0f) / 255.0) / ((m_terrainZScale * 2) + 1);
				vmin.y = fminf(vmin.y, height);
				vmax.y = fmaxf(vmax.y, height);
			}
		}

		node->boundary = ZShadeSandboxMath::AABB(vmin, vmax);
	}
	else
	{
		node->boundary = ZShadeSandboxMath::AABB(vmin, vmax);
		
		for (int i = 0; i < 4; ++i)
		{
			ComputeBounds(node->children[i]);
			node->boundary = ZShadeSandboxMath::AABB::Union(node->boundary, node->children[i]->boundary);
		}
	}
}
//================================================================================================================
