//==================================================================================================================================
// TreeParameters.h
//
//==================================================================================================================================
// History
//
// -Created on 8/14/2015 by Dustin Watson
//==================================================================================================================================
#ifndef __TREEPARAMETERS_H
#define __TREEPARAMETERS_H
//==================================================================================================================================
//==================================================================================================================================

//
// Includes
//

#include "ZMath.h"
#include "MeshRenderParameters.h"

#define NUM_LEAVES_PER_TREE 1000

//==================================================================================================================================
//==================================================================================================================================
struct TreeParameters
{
	GameDirectory3D* gameDirectory3D;
	int treeCount;
	string leafTextureName;
	string treeModelName;
	
	TreeParameters()
	{
		gameDirectory3D = 0;
		g_TreeCount = 400;
		leafTextureName = "";
		treeModelName = "";
	}
};
//==================================================================================================================================
struct InternalTreeRenderParameters
{
	XMMATRIX g_LeafOnTree[NUM_LEAVES_PER_TREE];
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__TREEPARAMETERS_H