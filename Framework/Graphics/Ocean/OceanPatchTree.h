//==============================================================================================================================
// OceanPatchTree.h
//
//==============================================================================================================================
// History
//
// -Created on 5/31/2015  by Dustin Watson
//==============================================================================================================================
#ifndef __OCEANPATCHTREE_H
#define __OCEANPATCHTREE_H
//==============================================================================================================================
//==============================================================================================================================

//
// Includes
//

#include "OceanPatch.h"

//==============================================================================================================================
//==============================================================================================================================

class OceanPatchTree
{
	D3D* mD3DSystem;

public:

	OceanPatchTree(D3D* d3d, int oceanSize, int patchSize, float seaLevel);

	OceanPatch* GetPatches() { return mPatches; }

private:

	//Initialize the basic components of the OceanPatchTree
	void InitializeTree();

	//Create the OceanPatchTree by calling a recursive function that will create the tree
	void CreateTree();

	// Calculates the maximum amount of patches needed in the OceanPatchTree
	int numberOfPatches(int leaves, int leafWidth);

	// Recursively creates a patch in the OceanPatchTree
	void CreatePatch(OceanPatch*& patch, XMFLOAT3 bounds[4], float width, ZShadeSandboxMath::AABB aabb, OceanPatch* parentPatch, int parentID, int nodeID);

private:

	float fSeaLevel;
	int iOceanSize;
	int iPatchSize;
	int iPatchCount;
	int iTotalPatches;
	int iTotalPatchID;

	// Root patch of the ocean
	OceanPatch* mPatches;
};

//==============================================================================================================================
//==============================================================================================================================
#endif//__OCEANPATCHTREE_H