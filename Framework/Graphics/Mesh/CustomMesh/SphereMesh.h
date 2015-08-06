//===============================================================================================================================
// SphereMesh
//
//===============================================================================================================================
// History
//
// -Created on 11/17/2014 by Dustin Watson
//===============================================================================================================================
#ifndef __SPHEREMESH_H
#define __SPHEREMESH_H
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//
#include "CustomMesh.h"

//===============================================================================================================================
//===============================================================================================================================
namespace ZShadeSandboxMesh {
class SphereMesh : public ZShadeSandboxMesh::CustomMesh
{
	float fRadius;
	UINT  iSliceCount;
	UINT  iStackCount;
	
public:
	
	// Loads a sphere mesh from a file
	SphereMesh(D3D* d3d, ZShadeSandboxMesh::MeshParameters mp, char* filename);
	// Loads a basic sphere mesh in 3D space
	SphereMesh(float radius, UINT sliceCount, UINT stackCount, D3D* d3d, ZShadeSandboxMesh::MeshParameters mp);
	// Loads a sphere mesh in 3D space
	SphereMesh(ZShadeSandboxMath::SpherePrimitive sphere, D3D* d3d, ZShadeSandboxMesh::MeshParameters mp);
	~SphereMesh();

	void Initialize();

	ZShadeSandboxMesh::CustomMesh* Clone();
};
}
//===============================================================================================================================
//===============================================================================================================================
#endif//__SPHEREMESH_H