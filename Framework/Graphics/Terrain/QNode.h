//===============================================================================================================================
// QNode.h
//
//===============================================================================================================================
// History
//
// -Created on 6/18/2014 by Dustin Watson
//===============================================================================================================================
#ifndef __QNODE_H
#define __QNODE_H
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//

#include "Vertex.h"
#include "QNodeType.h"
#include "D3D.h"
#include "AABB.h"
#include "CubeMesh.h"

//===============================================================================================================================
namespace ZShadeSandboxTerrain {
struct QNode
{
	QNode* parent;
	
	QNode* children[4];

	// The axis aligned boundary of this node
	ZShadeSandboxMath::AABB boundary;

	//ID's of the children
	int branches[4];
	
	//Bounding Coordinates for the node
	XMFLOAT3 boundingCoord[4];

	//id of the node
	int nodeID;

	//id of the parent's node
	int parentID;

	//Is this a leaf node or a regular node
	int type;
	
	vector<int> mTriangleID;
};
}
//===============================================================================================================================
#endif//__QNODE_H