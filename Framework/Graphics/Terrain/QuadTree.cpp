#include "QuadTree.h"
#include "Convert.h"
#include "TextureManager.h"
#include "ZMath.h"
using ZShadeSandboxTerrain::QuadTree;
//==============================================================================================================================
QuadTree::QuadTree(D3D* d3d, ZShadeSandboxTerrain::TerrainParameters tp, GameDirectory3D* gd3d)
:   m_d3d(d3d)
,   m_heightmapName(tp.g_heightmapName)
,	m_procedural(tp.g_procedural)
,	m_makeFlat(tp.g_makeFlat)
,	m_heightScale(tp.g_heightScale)
,	m_cellSpacing(tp.g_cellSpacing)
,	m_tessellate(tp.g_tessellate)
,	m_QuadTreeSize(tp.g_TerrainSize)
,	m_leafWidth(tp.g_leafWidth + 1)
,	m_terrScale(tp.g_terrScale)
,   m_textureRepeat(tp.g_TextureRepeat)
,	m_RenderPrimitive(tp.g_RenderPrimitive)
,   m_seaLevel(tp.g_seaLevel)
,	m_GameDirectory3D(gd3d)
,	mProceduralParameters(tp.g_proceduralParameters)
,	m_loaded(false)
{
	m_EngineOptions = m_d3d->GetEngineOptions();
	m_terrainZScale = 1;
	m_TotalLeavesInRow = 0;
	
	if (ZShadeSandboxMath::ZMath::IsPowerOf2(m_QuadTreeSize))
	{
		LoadQuadTree();
	}
}
//==============================================================================================================================
QuadTree::QuadTree(const QuadTree& qtree)
{
}
//==============================================================================================================================
QuadTree::~QuadTree()
{
}
//==============================================================================================================================
void QuadTree::LoadQuadTree()
{
	if (m_makeFlat)
	{
		m_useHeight = false;
	}
	else
	{
		if (m_procedural)
		{
			LoadMap();
		}
		else
		{
			LoadMap(m_heightmapName);
		}
	}
	
	BuildHeight();
	
	// Create the height map Shader Resource View
	BuildHeightmapSRV();

	// Initialize the resources for the Quad Tree
	InitializeTree();
	CreateTree();

	// Print the bounding box coord to a file
	Print();
	
	m_loaded = true;
}
//==============================================================================================================================
void QuadTree::BuildHeight()
{
	int index = 0, vertexCount = 0;
	float height = m_seaLevel;
	
	heightmap_vec.resize(m_QuadTreeSize * m_QuadTreeSize);

	for (int z = 0; z < m_QuadTreeSize/* + 1*/; z++)
	{
		for (int x = 0; x < m_QuadTreeSize/* + 1*/; x++)
		{
			if (m_useHeight)
			{
				height = ReadHeight(x, z);
			}
			
			//Build the heightmap vector
			if (x < m_QuadTreeSize && z < m_QuadTreeSize)
			{
				heightmap_vec[vertexCount] = height;
			}
			
			vertexCount++;
		}
	}
	
	m_triangleCount = vertexCount / 3;
}
//==============================================================================================================================
//void QuadTree::UpdateHeightValues(float heightScale, float zScale)
//{
//	//m_heightmap->UpdateHeightValues(heightScale, zScale);
//
//	m_heightScale = heightScale;
//	m_terrainZScale = zScale;
//}
//==============================================================================================================================
void QuadTree::LoadMap(string heightmap)
{
	m_useHeight = true;
	
	string heightmapName = m_GameDirectory3D->m_heightmaps_path + "\\" + heightmap;
	
	//m_heightmap = new ZShadeSandboxTerrain::Heightmap(heightmapName, m_QuadTreeSize, m_QuadTreeSize, m_heightScale, m_seaLevel, 70);
	
	m_heightmap = new ZShadeSandboxTerrain::Heightmap(heightmapName, mProceduralParameters, m_heightScale);
	
	m_QuadTreeSize = m_heightmap->HeightmapSize();
}
//==============================================================================================================================
void QuadTree::LoadMap()
{
	m_useHeight = true;
	
	m_heightmap = new ZShadeSandboxTerrain::Heightmap(mProceduralParameters, m_heightScale);

	m_QuadTreeSize = m_heightmap->HeightmapSize();
}
//==============================================================================================================================
/*bool QuadTree::InHeightmap(float x, float z)
{
	if (m_useHeight)
	{
		if (x < m_QuadTreeSize && z < m_QuadTreeSize && x >= 0 && z >= 0)
		{
			return true;
		}

		return false;
	}

	return false;
}
//==============================================================================================================================
float QuadTree::GetHeight(float x, float z)
{
	if (InHeightmap(x, z))
	{
		return m_heightmap->SampleHeight(x, z);
	}

	return 0;
}*/
//==============================================================================================================================
int QuadTree::GetNodeCount()
{
	return m_nodeCount;
}
//==============================================================================================================================
void QuadTree::Print()
{
	out.open("QuadTreeNodeLog.txt", ios::out);
	PrintNode(m_nodes);
	out.close();
}
//==============================================================================================================================
void QuadTree::InitializeTree()
{
	m_totalTreeID = 0;
	m_totalLeaves = (m_QuadTreeSize / (m_leafWidth - 1)) * (m_QuadTreeSize / (m_leafWidth - 1));
	m_TotalLeavesInRow = m_totalLeaves / 2;
	m_nodeCount = numberOfNodes(m_totalLeaves, m_leafWidth - 1);
}
//==============================================================================================================================
int QuadTree::numberOfNodes(int leaves, int leafWidth)
{
	int ctr = 0, val = 0;

	while (val == 0)
	{
		ctr += leaves;
		leaves /= leafWidth;

		if (leaves == 0)
			break;

		if (leaves == 1)
			val = 1;
	}

	ctr++;

	return ctr;
}
//==============================================================================================================================
void QuadTree::CreateTree()
{
	XMFLOAT3 RootBounds[4];
	
	// LowerLeft
	RootBounds[0].x = 0;
	RootBounds[0].y = 0;
	RootBounds[0].z = 0;
	
	// LowerRight
	RootBounds[1].x = m_QuadTreeSize;
	RootBounds[1].y = 0;
	RootBounds[1].z = 0;
	
	// UpperLeft
	RootBounds[2].x = 0;
	RootBounds[2].y = 0;
	RootBounds[2].z = m_QuadTreeSize;

	// UpperRight
	RootBounds[3].x = m_QuadTreeSize;
	RootBounds[3].y = 0;
	RootBounds[3].z = m_QuadTreeSize;

	//AABB* aabb = new AABB();
	ZShadeSandboxMath::AABB aabb;
	aabb.vMin = XMFLOAT3(0, 0, 0);
	aabb.vMax = XMFLOAT3(m_QuadTreeSize, 0, m_QuadTreeSize);
	
	CreateNode(m_nodes, RootBounds, aabb, 0, 0, 0);
}
//==============================================================================================================================
void QuadTree::CreateNode(ZShadeSandboxTerrain::QNode*& node, XMFLOAT3 bounds[4], ZShadeSandboxMath::AABB nodeAABB, ZShadeSandboxTerrain::QNode* parent, int parentID, int nodeID)
{
	int type;
	
	node = new ZShadeSandboxTerrain::QNode();

	node->children[0] = 0;
	node->children[1] = 0;
	node->children[2] = 0;
	node->children[3] = 0;
	
	float xDiff = bounds[0].x - bounds[1].x;
	float zDiff = bounds[0].z - bounds[2].z;
	
	//Find the width and height of the node
	int NodeWidth = (int)abs(xDiff);
	int NodeHeight = (int)abs(zDiff);
	
	//float width = bounds[3].x - bounds[2].x;
	//float height = bounds[2].z - bounds[0].z;
	
	if (NodeWidth == m_leafWidth - 1)
	//if (width / 2 == (2 * m_leafWidth))
	{
		type = ZShadeSandboxTerrain::EQuadNodeType::LEAF;
	}
	else
	{
		type = ZShadeSandboxTerrain::EQuadNodeType::NODE;
	}
	
	node->parent = parent;
	node->boundary = nodeAABB;
	node->type = type;
	node->nodeID = nodeID;
	node->parentID = parentID;

	/*int bounds0X = (int)bounds[0].x;
	int bounds0Z = (int)bounds[0].z;

	int bounds1X = (int)bounds[1].x;
	int bounds1Z = (int)bounds[1].z;

	int bounds2X = (int)bounds[2].x;
	int bounds2Z = (int)bounds[2].z;

	int bounds3X = (int)bounds[3].x;
	int bounds3Z = (int)bounds[3].z;

	float height0 = 0;
	float height1 = 0;
	float height2 = 0;
	float height3 = 0;
	
	if (m_useHeight)
	{
		if (bounds0X < m_QuadTreeSize && bounds0Z < m_QuadTreeSize)
		{
			if (GetMapExt() == EHeightExtension::RAW)
				height0 = m_heightmap->SampleHeight((bounds0Z * MapSize()) + bounds0X);
			else
				height0 = m_heightmap->SampleHeight(bounds0X, bounds0Z);
		}
		
		if (bounds1X < m_QuadTreeSize && bounds1Z < m_QuadTreeSize)
		{
			if (GetMapExt() == EHeightExtension::RAW)
				height1 = m_heightmap->SampleHeight((bounds1Z * MapSize()) + bounds1X);
			else
				height1 = m_heightmap->SampleHeight(bounds1X, bounds1Z);
		}
		
		if (bounds2X < m_QuadTreeSize && bounds2Z < m_QuadTreeSize)
		{
			if (GetMapExt() == EHeightExtension::RAW)
				height2 = m_heightmap->SampleHeight((bounds2Z * MapSize()) + bounds2X);
			else
				height2 = m_heightmap->SampleHeight(bounds2X, bounds2Z);
		}
		
		if (bounds3X < m_QuadTreeSize && bounds3Z < m_QuadTreeSize)
		{
			if (GetMapExt() == EHeightExtension::RAW)
				height3 = m_heightmap->SampleHeight((bounds3Z * MapSize()) + bounds3X);
			else
				height3 = m_heightmap->SampleHeight(bounds3X, bounds3Z);
		}
	}*/
	
	// Center the grid in model space
	float halfWidth = ((float)m_leafWidth - 1.0f) / 2.0f;
	float halfLength = ((float)m_leafWidth - 1.0f) / 2.0f;

	/*XMFLOAT2 minMaxY = MinMaxY(nodeAABB.vMin, nodeAABB.vMax);
	
	float minX = nodeAABB.vMin.x * m_cellSpacing - NodeWidth;// / 2;
	float maxX = nodeAABB.vMax.x * m_cellSpacing - NodeWidth;// / 2;
	float minZ = -nodeAABB.vMin.z * m_cellSpacing + NodeHeight;// / 2;
	float maxZ = -nodeAABB.vMax.z * m_cellSpacing + NodeHeight;// / 2;
	
	AABB aabb(XMFLOAT3((minX - halfWidth) * m_terrScale, minMaxY.x, (minZ - halfWidth) * m_terrScale), XMFLOAT3((maxX - halfWidth) * m_terrScale, minMaxY.y, (maxZ - halfWidth) * m_terrScale));
	
	node->boundary = aabb;
	
	float width = (int)floor((nodeAABB.vMax.x - nodeAABB.vMin.x) / 2);
	float depth = (int)floor((nodeAABB.vMax.z - nodeAABB.vMin.z) / 2);
	*/
	//Need to get the bounding coord for this node

	//XMFLOAT2 minMaxY = MinMaxY(nodeAABB.vMin, nodeAABB.vMax);
	//node->boundary.vMin.y = minMaxY.x;
	//node->boundary.vMax.y = minMaxY.y;
	
	// LowerLeft
	node->boundingCoord[0].x = bounds[0].x;
	node->boundingCoord[0].y = 0.0f;
	node->boundingCoord[0].z = bounds[0].z;
	
	// LowerRight
	node->boundingCoord[1].x = bounds[1].x;
	node->boundingCoord[1].y = 0.0f;
	node->boundingCoord[1].z = bounds[1].z;

	// UpperLeft
	node->boundingCoord[2].x = bounds[2].x;
	node->boundingCoord[2].y = 0.0f;
	node->boundingCoord[2].z = bounds[2].z;

	// UpperRight
	node->boundingCoord[3].x = bounds[3].x;
	node->boundingCoord[3].y = 0.0f;
	node->boundingCoord[3].z = bounds[3].z;

	if (type == ZShadeSandboxTerrain::EQuadNodeType::LEAF)
	{
		return;
	}
	else
	{
		#pragma region "Child Node 1"
		//======================================================================================================================
		//Child Node 1
		m_totalTreeID++;
		node->branches[0] = m_totalTreeID;
		XMFLOAT3 ChildBounds1[4];

		//Top-Left
		ChildBounds1[0].x = bounds[0].x;
		ChildBounds1[0].y = 0;
		ChildBounds1[0].z = bounds[0].z;

		//Top-Right
		ChildBounds1[1].x = bounds[0].x + abs((bounds[0].x - bounds[1].x) / 2);
		ChildBounds1[1].y = 0;
		ChildBounds1[1].z = bounds[1].z;

		//Bottom-Left
		ChildBounds1[2].x = bounds[2].x;
		ChildBounds1[2].y = 0;
		ChildBounds1[2].z = bounds[0].z + abs((bounds[0].z - bounds[2].z) / 2);

		//Bottom-Right
		ChildBounds1[3].x = bounds[0].x + abs((bounds[0].x - bounds[1].x) / 2);
		ChildBounds1[3].y = 0;
		ChildBounds1[3].z = bounds[0].z + abs((bounds[0].z - bounds[2].z) / 2);
		
		ZShadeSandboxMath::AABB ChildAABB1;// = new AABB();
		//ChildAABB1.vMin = XMFLOAT3(nodeAABB.vMin.x, 0.0f, nodeAABB.vMin.z);
		//ChildAABB1.vMax = XMFLOAT3(nodeAABB.vMin.x + width, 0.0f, nodeAABB.vMin.z + depth);
		//ChildAABB1.vMin = XMFLOAT3(ChildBounds1[0].x, 0.0f, ChildBounds1[0].z);
		//ChildAABB1.vMax = XMFLOAT3(ChildBounds1[3].x, 0.0f, ChildBounds1[3].z);

		ChildAABB1.vMin = XMFLOAT3((ChildBounds1[0].x - halfWidth) * m_terrScale, ChildBounds1[0].y, (ChildBounds1[0].z - halfWidth) * m_terrScale);
		ChildAABB1.vMax = XMFLOAT3((ChildBounds1[3].x - halfWidth) * m_terrScale, ChildBounds1[3].y, (ChildBounds1[3].z - halfWidth) * m_terrScale);

		CreateNode(node->children[0], ChildBounds1, ChildAABB1, node, nodeID, m_totalTreeID);
		//======================================================================================================================
		#pragma endregion

		#pragma region "Child Node 2"
		//======================================================================================================================
		//Child Node 2
		m_totalTreeID++;
		node->branches[1] = m_totalTreeID;
		XMFLOAT3 ChildBounds2[4];

		//Top-Left
		ChildBounds2[0].x = bounds[0].x + abs((bounds[0].x - bounds[1].x) / 2);
		ChildBounds2[0].y = 0;
		ChildBounds2[0].z = bounds[1].z;

		//Top-Right
		ChildBounds2[1].x = bounds[1].x;
		ChildBounds2[1].y = 0;
		ChildBounds2[1].z = bounds[1].z;

		//Bottom-Left
		ChildBounds2[2].x = bounds[0].x + abs((bounds[0].x - bounds[1].x) / 2);
		ChildBounds2[2].y = 0;
		ChildBounds2[2].z = bounds[1].z + abs((bounds[1].z - bounds[3].z) / 2);

		//Bottom-Right
		ChildBounds2[3].x = bounds[1].x;
		ChildBounds2[3].y = 0;
		ChildBounds2[3].z = bounds[1].z + abs((bounds[1].z - bounds[3].z) / 2);
		
		ZShadeSandboxMath::AABB ChildAABB2;// = new AABB();
		//ChildAABB2.vMin = XMFLOAT3(nodeAABB.vMin.x + width, 0.0f, nodeAABB.vMin.z);
		//ChildAABB2.vMax = XMFLOAT3(nodeAABB.vMax.x, 0.0f, nodeAABB.vMin.z + depth);
		//ChildAABB2.vMin = XMFLOAT3(ChildBounds2[0].x, 0.0f, ChildBounds2[0].z);
		//ChildAABB2.vMax = XMFLOAT3(ChildBounds2[3].x, 0.0f, ChildBounds2[3].z);

		ChildAABB2.vMin = XMFLOAT3((ChildBounds2[0].x - halfWidth) * m_terrScale, ChildBounds2[0].y, (ChildBounds2[0].z - halfWidth) * m_terrScale);
		ChildAABB2.vMax = XMFLOAT3((ChildBounds2[3].x - halfWidth) * m_terrScale, ChildBounds2[3].y, (ChildBounds2[3].z - halfWidth) * m_terrScale);

		CreateNode(node->children[1], ChildBounds2, ChildAABB2, node, nodeID, m_totalTreeID);
		//======================================================================================================================
		#pragma endregion

		#pragma region "Child Node 3"
		//======================================================================================================================
		//Child Node 3
		m_totalTreeID++;
		node->branches[2] = m_totalTreeID;
		XMFLOAT3 ChildBounds3[4];

		//Top-Left
		ChildBounds3[0].x = bounds[0].x;
		ChildBounds3[0].y = 0;
		ChildBounds3[0].z = bounds[0].z + abs((bounds[0].z - bounds[2].z) / 2);

		//Top-Right
		ChildBounds3[1].x = bounds[0].x + abs((bounds[0].x - bounds[1].x) / 2);
		ChildBounds3[1].y = 0;
		ChildBounds3[1].z = bounds[0].z + abs((bounds[0].z - bounds[2].z) / 2);

		//Bottom-Left
		ChildBounds3[2].x = bounds[2].x;
		ChildBounds3[2].y = 0;
		ChildBounds3[2].z = bounds[2].z;

		//Bottom-Right
		ChildBounds3[3].x = bounds[0].x + abs((bounds[0].x - bounds[1].x) / 2);
		ChildBounds3[3].y = 0;
		ChildBounds3[3].z = bounds[3].z;
		
		ZShadeSandboxMath::AABB ChildAABB3;// = new AABB();
		//ChildAABB3.vMin = XMFLOAT3(nodeAABB.vMin.x, 0.0f, nodeAABB.vMin.z + depth);
		//ChildAABB3.vMax = XMFLOAT3(nodeAABB.vMin.x + depth, 0.0f, nodeAABB.vMax.z);
		//ChildAABB3.vMin = XMFLOAT3(ChildBounds3[0].x, 0.0f, ChildBounds3[0].z);
		//ChildAABB3.vMax = XMFLOAT3(ChildBounds3[3].x, 0.0f, ChildBounds3[3].z);

		ChildAABB3.vMin = XMFLOAT3((ChildBounds3[0].x - halfWidth) * m_terrScale, ChildBounds3[0].y, (ChildBounds3[0].z - halfWidth) * m_terrScale);
		ChildAABB3.vMax = XMFLOAT3((ChildBounds3[3].x - halfWidth) * m_terrScale, ChildBounds3[3].y, (ChildBounds3[3].z - halfWidth) * m_terrScale);

		CreateNode(node->children[2], ChildBounds3, ChildAABB3, node, nodeID, m_totalTreeID);
		//======================================================================================================================
		#pragma endregion

		#pragma region "Child Node 4"
		//======================================================================================================================
		//Child Node 4
		m_totalTreeID++;
		node->branches[3] = m_totalTreeID;
		XMFLOAT3 ChildBounds4[4];

		//Top-Left
		ChildBounds4[0].x = bounds[0].x + abs((bounds[0].x - bounds[1].x) / 2);
		ChildBounds4[0].y = 0;
		ChildBounds4[0].z = bounds[1].z + abs((bounds[1].z - bounds[3].z) / 2);

		//Top-Right
		ChildBounds4[1].x = bounds[3].x;
		ChildBounds4[1].y = 0;
		ChildBounds4[1].z = bounds[1].z + abs((bounds[1].z - bounds[3].z) / 2);//-1

		//Bottom-Left
		ChildBounds4[2].x = bounds[0].x + abs((bounds[0].x - bounds[1].x) / 2);
		ChildBounds4[2].y = 0;
		ChildBounds4[2].z = bounds[3].z;

		//Bottom-Right
		ChildBounds4[3].x = bounds[3].x;
		ChildBounds4[3].y = 0;
		ChildBounds4[3].z = bounds[3].z;
		
		ZShadeSandboxMath::AABB ChildAABB4;// = new AABB();
		//ChildAABB4.vMin = XMFLOAT3(nodeAABB.vMin.x + width, 0.0f, nodeAABB.vMin.z + depth);
		//ChildAABB4.vMax = XMFLOAT3(nodeAABB.vMax.x, 0.0f, nodeAABB.vMax.z);
		//ChildAABB4.vMin = XMFLOAT3(ChildBounds4[0].x, 0.0f, ChildBounds4[0].z);
		//ChildAABB4.vMax = XMFLOAT3(ChildBounds4[3].x, 0.0f, ChildBounds4[3].z);

		ChildAABB4.vMin = XMFLOAT3((ChildBounds4[0].x - halfWidth) * m_terrScale, ChildBounds4[0].y, (ChildBounds4[0].z - halfWidth) * m_terrScale);
		ChildAABB4.vMax = XMFLOAT3((ChildBounds4[3].x - halfWidth) * m_terrScale, ChildBounds4[3].y, (ChildBounds4[3].z - halfWidth) * m_terrScale);

		CreateNode(node->children[3], ChildBounds4, ChildAABB4, node, nodeID, m_totalTreeID);
		//======================================================================================================================
		#pragma endregion
	}
}
//==============================================================================================================================
XMFLOAT2 QuadTree::MinMaxY(XMFLOAT3 topLeft, XMFLOAT3 bottomRight)
{
	float fmax = FLT_MIN;
	float fmin = FLT_MAX;
	
	for (int x = (int)topLeft.x; x < bottomRight.x; x++)
	{
		for (int z = (int)topLeft.z; z < bottomRight.z; z++)
		{
			fmin = min(fmin, m_heightmap->GetHeightmap().ReadHeight((z * MapSize()) + x));
			fmax = max(fmax, m_heightmap->GetHeightmap().ReadHeight((z * MapSize()) + x));
		}
	}
	
	return XMFLOAT2(fmin, fmax);
}
//==============================================================================================================================
ZShadeSandboxTerrain::QNode* QuadTree::LocateChildNode(int id)
{
	return LocateChildNode(m_nodes, id);
}
//==============================================================================================================================
ZShadeSandboxTerrain::QNode* QuadTree::LocateChildNode(ZShadeSandboxTerrain::QNode* node, int id)
{
	//Node does not exist
	if (node == 0) return 0;

	if (node->nodeID == id)
		return m_nodes;

	//Test child one
	ZShadeSandboxTerrain::QNode* n1 = LocateChildNode(node->children[0], id);
	
	//We found our node
	if (n1 != 0) return n1;

	//Test child two
	ZShadeSandboxTerrain::QNode* n2 = LocateChildNode(node->children[1], id);
	
	//We found our node
	if (n2 != 0) return n2;

	//Test child three
	ZShadeSandboxTerrain::QNode* n3 = LocateChildNode(node->children[2], id);
	
	//We found our node
	if (n3 != 0) return n3;

	//Test child four
	ZShadeSandboxTerrain::QNode* n4 = LocateChildNode(node->children[3], id);

	//We found our node
	if (n4 != 0) return n4;

	//Found Nothing
	return 0;
}
//==============================================================================================================================
ZShadeSandboxTerrain::QNode* QuadTree::LocateLeaf(ZShadeSandboxTerrain::QNode* node)
{
	int type = node->type;

	if (type == ZShadeSandboxTerrain::EQuadNodeType::LEAF)
	{
		return node;
	}
	else
	{
		ZShadeSandboxTerrain::QNode* child = LocateChildNode(node->branches[0]);

		return LocateLeaf(child);
	}
}
//==============================================================================================================================
void QuadTree::PrintNode(ZShadeSandboxTerrain::QNode* node)
{
	if (node == 0) return;

	BetterString str;

	str = "********************************************************\n";
	ZShadeSandboxTerrain::QNode* childTest = LocateChildNode(node->branches[0]);
	if (node->nodeID == 0)
		str.push("*Root*\n");
	else
	{
		if (childTest != 0)
			str.push("*Node*\n");
		else
			str.push("*Leaf*\n");
	}
	str.push("Node ID: ");
	str.push(ZShadeSandboxGlobal::Convert::ConvertIntToString(node->nodeID));
	str.push("\n");
	str.push("Node Bounding Coord: ");
	str.push("\n");
	PrintCoord(node, str);
	
	ZShadeSandboxTerrain::QNode* ChildNode1 = LocateChildNode(node->branches[0]);
	ZShadeSandboxTerrain::QNode* ChildNode2 = LocateChildNode(node->branches[1]);
	ZShadeSandboxTerrain::QNode* ChildNode3 = LocateChildNode(node->branches[2]);
	ZShadeSandboxTerrain::QNode* ChildNode4 = LocateChildNode(node->branches[3]);

	if (ChildNode1 != 0)
	{
		str.push("Node Children: ");
		str.push("\n");

		str.push("Child 1: ");
		str.push("\n");
		PrintCoord(ChildNode1, str);

		str.push("Child 2: ");
		str.push("\n");
		PrintCoord(ChildNode2, str);

		str.push("Child 3: ");
		str.push("\n");
		PrintCoord(ChildNode3, str);

		str.push("Child 4: ");
		str.push("\n");
		PrintCoord(ChildNode4, str);
	}

	out << str;

	//Look at the children nodes
	PrintNode(node->children[0]);
	PrintNode(node->children[1]);
	PrintNode(node->children[2]);
	PrintNode(node->children[3]);
}
//==============================================================================================================================
void QuadTree::PrintCoord(ZShadeSandboxTerrain::QNode* node, BetterString& bs)
{
	bs.push("Coord1: ");
	bs.push(ZShadeSandboxGlobal::Convert::ConvertIntToString(node->boundingCoord[0].x));
	bs.push(", ");
	bs.push(ZShadeSandboxGlobal::Convert::ConvertIntToString(node->boundingCoord[0].y));
	bs.push(", ");
	bs.push(ZShadeSandboxGlobal::Convert::ConvertIntToString(node->boundingCoord[0].z));
	bs.push("\n");
	
	bs.push("Coord2: ");
	bs.push(ZShadeSandboxGlobal::Convert::ConvertIntToString(node->boundingCoord[1].x));
	bs.push(", ");
	bs.push(ZShadeSandboxGlobal::Convert::ConvertIntToString(node->boundingCoord[1].y));
	bs.push(", ");
	bs.push(ZShadeSandboxGlobal::Convert::ConvertIntToString(node->boundingCoord[1].z));
	bs.push("\n");

	bs.push("Coord3: ");
	bs.push(ZShadeSandboxGlobal::Convert::ConvertIntToString(node->boundingCoord[2].x));
	bs.push(", ");
	bs.push(ZShadeSandboxGlobal::Convert::ConvertIntToString(node->boundingCoord[2].y));
	bs.push(", ");
	bs.push(ZShadeSandboxGlobal::Convert::ConvertIntToString(node->boundingCoord[2].z));
	bs.push("\n");

	bs.push("Coord4: ");
	bs.push(ZShadeSandboxGlobal::Convert::ConvertIntToString(node->boundingCoord[3].x));
	bs.push(", ");
	bs.push(ZShadeSandboxGlobal::Convert::ConvertIntToString(node->boundingCoord[3].y));
	bs.push(", ");
	bs.push(ZShadeSandboxGlobal::Convert::ConvertIntToString(node->boundingCoord[3].z));
	bs.push("\n");
}
//==============================================================================================================================
void QuadTree::BuildHeightmapSRV()
{
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = m_QuadTreeSize;
	texDesc.Height = m_QuadTreeSize;
    texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format    = DXGI_FORMAT_R16_FLOAT;
	texDesc.SampleDesc.Count   = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	vector<float> hv = heightmap_vec;
	// HALF is defined in xnamath.h, for storing 16-bit float.
	std::vector<HALF> hmap(hv.size());
	std::transform(hv.begin(), hv.end(), hmap.begin(), XMConvertFloatToHalf);
	
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &hmap[0];
    data.SysMemPitch = m_QuadTreeSize * sizeof(HALF);
    data.SysMemSlicePitch = 0;

	ID3D11Texture2D* hmapTex = 0;
	m_d3d->GetDevice11()->CreateTexture2D(&texDesc, &data, &hmapTex);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;
	m_d3d->GetDevice11()->CreateShaderResourceView(hmapTex, &srvDesc, &mHeightMapSRV);

	// SRV saves reference.
	SAFE_RELEASE(hmapTex);
}
//================================================================================================================