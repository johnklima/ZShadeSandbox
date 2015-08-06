//==================================================================================================================================
// Volume.h
//
// Area that has the possibility to trigger an event
// This is the base Volume class that will create all other volumes
//==================================================================================================================================
// History
//
// -Created on 4/2/2015 by Dustin Watson
//==================================================================================================================================
#ifndef __VOLUME_H
#define __VOLUME_H
//==================================================================================================================================
//==================================================================================================================================
#include "D3D.h"
#include "ZMath.h"
#include "CubeMesh.h"
#include "AABB.h"
//==================================================================================================================================
class Volume : public ZShadeSandboxMesh::CubeMesh
{
	//AABB* boundary;

public:
	
	Volume(D3D* d3d, ZShadeSandboxMesh::MeshParameters mp);
	~Volume();
	
	// Need to detect when something enters the volume (3D Cube)
	//bool Intersects(XMFLOAT3 point);
	
	// There is a Collides function for this purpose in ZShadeSandboxMesh::CustomMesh which is what a volume is
	//bool Enters(ZShadeSandboxMesh::CustomMesh* mesh);
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__VOLUME_H