//==============================================================================================================================
// OceanPatch.h
//
//==============================================================================================================================
// History
//
// -Created on 5/30/2015  by Dustin Watson
//==============================================================================================================================
#ifndef __OCEANPATCH_H
#define __OCEANPATCH_H
//==============================================================================================================================
//==============================================================================================================================

//
// Includes
//

#include "D3D.h"
#include "Camera.h"
#include "ZMath.h"
#include "AABB.h"
#include "Vertex.h"
#include "OceanParameters.h"

//==============================================================================================================================
//==============================================================================================================================

struct OceanPatch
{
	OceanPatch* parent;
	OceanPatch* children[4];
	
	int branches[4];
	
	XMFLOAT3 boundingCoord[4];
	XMFLOAT3 center;

	ZShadeSandboxMath::AABB boundary;

	int nodeID;

	int parentID;

	float width;

	bool isLeaf;
	bool containsCamera;

	// Mesh attributes
	struct MeshAttributes
	{
		float					  seaLevel;
		int						  patchScale;
		int						  patchSize;
		int 					  vertexCount;
		int 					  indexCount;
		int			 			  triangleCount;
		UINT 					  vertexByteWidth;
		vector<ZShadeSandboxMesh::VertexTex> vertices;
		vector<UINT>              indices;
		ID3D11Buffer*             vb;
		ID3D11Buffer*             ib;
	};

	MeshAttributes* mesh;
};

//==============================================================================================================================
//==============================================================================================================================
#endif//__OCEANPATCH_H