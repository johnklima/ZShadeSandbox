//==============================================================================================================================
//QuadTree.h
//
//http://richardssoftware.net/Home/Post/29
//http://www.rastertek.com/tertut06.html
//==============================================================================================================================
// History
//
// 7/1/2014 Created by Dustin Watson
// 4/7/2015 - Dustin Watson - Took out mesh stuff and created a separate class
//==============================================================================================================================
#ifndef __QUADTREE_H
#define __QUADTREE_H
//==============================================================================================================================
//==============================================================================================================================
//Windows includes
#include <Windows.h>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
#include <fstream>
using namespace std;
//==============================================================================================================================
//==============================================================================================================================
#include "QNode.h"
#include "D3D.h"
#include "Heightmap.h"
#include "BetterString.h"
#include "AABB.h"
#include "TerrainParameters.h"
#include "GameDirectory.h"
#include "HeightmapData.h"
//==============================================================================================================================
namespace ZShadeSandboxTerrain {
//Default QuadTree -> 4*4 w/ 16x16 node size
class QuadTree
{
public:
	
	// Constructor for loading a Quad Tree
	QuadTree(D3D* d3d, ZShadeSandboxTerrain::TerrainParameters tp, GameDirectory3D* gd3d);
	
	// Copy Constructor
	QuadTree(const QuadTree& qtree);
	
	// Destructor
	~QuadTree();
	
public:
	
	//Load elevation for the map
	void LoadMap(string heightmap);
	void LoadMap();
	
	// Prints the bounding box of a leaf node from the root
	void Print();
	
	// Loads the data and mesh for the Quad Tree
	void LoadQuadTree();

	// Returns the amount of nodes in the Quad Tree
	int GetNodeCount();
	
public:
	
	bool IsLoaded() const						{ return m_loaded; }
	bool& Tessellate()							{ return m_tessellate; }
	bool Tessellate() const						{ return m_tessellate; }
	int TriangleCount()const					{ return m_triangleCount; }
	int TotalLeaves() const 					{ return m_totalLeaves; }
	int TotalLeavesInRow() const				{ return m_TotalLeavesInRow; }
	int LeafWidth() const						{ return m_leafWidth; }
	int MapSize() const							{ return m_QuadTreeSize; }
	int& TerrainScale()							{ return m_terrScale; }
	int TerrainScale() const					{ return m_terrScale; }
	bool UsingHeight() const					{ return m_useHeight; }
	float GetCellSpacing()const					{ return m_cellSpacing; }
	bool IsProcedural() const					{ return m_procedural; }
	float SeaLevel() const						{ return m_seaLevel; }
	GameDirectory3D* GetGD3D()					{ return m_GameDirectory3D; }
	
	EHeightExtension::Type GetMapExt()const		{ return m_mapExt; }
	ZShadeSandboxTerrain::QNode* GetNodes()		{ return m_nodes; }
	ID3D11ShaderResourceView* GetHeightMapSRV()	{ return mHeightMapSRV; }
	
	ZShadeSandboxMesh::ERenderType::Type GetRenderPrimitive() const { return m_RenderPrimitive; }
	
	// Returns the height in a safe way
	float ReadX(int index) 						{ return m_heightmap->ReadX(index); }
	float ReadX(int x, int z) 					{ return m_heightmap->ReadX(x, z); }
	float ReadZ(int index) 						{ return m_heightmap->ReadZ(index); }
	float ReadZ(int x, int z) 					{ return m_heightmap->ReadZ(x, z); }
	float ReadHeight(int index) 				{ return m_heightmap->ReadHeight(index); }
	float ReadHeight(int x, int z) 				{ return m_heightmap->ReadHeight(x, z); }
	bool InHeightmap(int x, int z)				{ return m_heightmap->InBounds(x, z); }
	bool InHeightmap(int index)					{ return m_heightmap->InBounds(index); }
	
private:
	
	XMFLOAT2 MinMaxY(XMFLOAT3 topLeft, XMFLOAT3 bottomRight);
	
	void BuildHeight();
	
	//Initialize the basic components of the QuadTree
	void InitializeTree();
	
	//Create the QuadTree by calling a recursive function that will create the tree
	void CreateTree();
	
	// Calculates the maximum amount of nodes needed in the Quad Tree
	int numberOfNodes(int leaves, int leafWidth);
	
	// Recursively creates a node in the Quad Tree
	void CreateNode(ZShadeSandboxTerrain::QNode*& node, XMFLOAT3 bounds[4], ZShadeSandboxMath::AABB nodeAABB, ZShadeSandboxTerrain::QNode* parent, int parentID, int nodeID);
	
	// Finds a leaf node by its index
	ZShadeSandboxTerrain::QNode* LocateChildNode(int id);
	ZShadeSandboxTerrain::QNode* LocateChildNode(ZShadeSandboxTerrain::QNode* node, int id);
	
	// Finds the first available leaf node
	ZShadeSandboxTerrain::QNode* LocateLeaf(ZShadeSandboxTerrain::QNode* node);
	
	// Prints the bounding box of a leaf node
	void PrintNode(ZShadeSandboxTerrain::QNode* node);
	void PrintCoord(ZShadeSandboxTerrain::QNode* node, BetterString& bs);
	
	// Creates the ZShadeSandboxShader::Shader Resource View for the height map
	void BuildHeightmapSRV();

private:

	//
	//Variables
	//
	
	D3D* m_d3d;
	EngineOptions* m_EngineOptions;
	ofstream out;
	
	// The total amount of leaves in the Quad Tree
	int m_totalLeaves;
	
	// The total amount of leaves in one row in the Quad Tree
	int m_TotalLeavesInRow;
	
	// Size of a leaf, must to be one more then it should be
	int m_leafWidth;
	
	// The width and height of the Quad Tree
	int m_QuadTreeSize;
	
	// Incremented as the Quad Tree is being build
	int m_totalTreeID;
	
	// The amount of nodes in the Quad Tree
	int m_nodeCount;
	
	// Maximum amount of triangles in this mesh
	int m_triangleCount;
	
	// The bounding box for the Quad Tree
	ZShadeSandboxMath::BoundingBox bounds;
	
	// Scales the x and z positions of each vertex
	int m_terrScale;
	
	// extension to load for the height map (.bmp, .raw)
	ZShadeSandboxTerrain::EHeightExtension::Type m_mapExt;
	
	// Amount to scale the height map after it is loaded
	float m_heightScale;
	float m_terrainZScale;
	
	// The world cell space used to create the normal in the shader
	float m_cellSpacing;
	
	// Name of the height map image to load
	string m_heightmapName;
	
	// The actual height map for the height values in the Quad Tree
	ZShadeSandboxTerrain::Heightmap* m_heightmap;
	
	// Use height from a height map or create a flat Quad Tree
	bool m_useHeight;
	
	// Specifies the render primitive the terrain must be generated with
	ZShadeSandboxMesh::ERenderType::Type m_RenderPrimitive;
	
	// Enables tessellation in DX11
	bool m_tessellate;

	// procedurally generate a height map for the Quad Tree when enabled
	bool m_procedural;
	
	// Flattens the terrain to sea level
	bool m_makeFlat;
	
	// How many times is the texture repeated
	int m_textureRepeat;
	
	// list of values to use for the height map to create the height texture
	vector<float> heightmap_vec;
	
	// Lets the quad tree know where sea level is to load a flat Quad Tree at water level
	float m_seaLevel;
	
	bool m_loaded;
	
	ZShadeSandboxTerrain::ProceduralParameters mProceduralParameters;

	// The Shader Resource View for the base height map
	ID3D11ShaderResourceView* mHeightMapSRV;
	
	GameDirectory3D* m_GameDirectory3D;
	
	// root node of the Quad Tree
	ZShadeSandboxTerrain::QNode* m_nodes;
};
}
//==============================================================================================================================
//==============================================================================================================================
#endif//__QUADTREE_H