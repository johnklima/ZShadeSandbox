//===============================================================================================================================
// QMeshNode.h
//
//===============================================================================================================================
// History
//
// -Created on 4/7/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __QMESHNODE_H
#define __QMESHNODE_H
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//

#include "Vertex.h"
#include "QNodeType.h"
#include "D3D.h"
#include "BoxPrimitive.h"
#include "SpherePrimitive.h"
#include "CubeMesh.h"
#include "Triangle.h"

class AABB;

//===============================================================================================================================
//===============================================================================================================================
namespace ZShadeSandboxTerrain {
struct QMeshNode
{
	QMeshNode* parent;
	QMeshNode* children[4];
	
	// The axis aligned boundary of this node in world space coordinates.
	ZShadeSandboxMath::AABB boundary;
	
	//Bounding Coordinates for the node
	XMFLOAT3 boundingCoord[4];
	
	// Center point of the node
	XMFLOAT3 center;
	
	// Max width of this node
	float fWidth;
	
	//ID's of the children
	int branches[4];

	//id of the node
	int nodeID;

	//id of the parent node
	int parentID;
	
	// Node knows if the camera is in its parent node
	bool bContainsCamera;
	
	//Is this a leaf node or a regular node
	int type;

	// Bounding box for this node
	ZShadeSandboxMath::BoxPrimitive box;

	// Bounding sphere for this node
	ZShadeSandboxMath::SpherePrimitive sphere;

	// The actual bounding box to be rendered for this node
	ZShadeSandboxMesh::CubeMesh* box_mesh;
	
	//vector<Triangle*> internalTriangles;
	vector<XMFLOAT3> triangleVertices;
	vector<UINT>	 triangleIndices;
	
	int m_vertex_count;
	int m_index_count;
	int m_triangle_count;
	
	ZShadeSandboxMesh::VertexTex* vertices;
	unsigned long* indices;
	
	ID3D11Buffer* m_vertex_buffer11;
	ID3D11Buffer* m_index_buffer11;
	
	// Contains all the ids of each entity to be rendered when this node
	// is rendered.
	std::vector<int> entityRenderID;
	
	QMeshNode()
	:	vertices(0)
	,	indices(0)
	,	m_vertex_buffer11(0)
	,	m_index_buffer11(0)
	{
	}
};
}
//===============================================================================================================================
#endif//__QMESHNODE_H