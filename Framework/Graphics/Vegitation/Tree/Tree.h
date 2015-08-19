//==================================================================================================================================
// Tree.h
//
//==================================================================================================================================
// History
//
// -Created on 8/14/2015 by Dustin Watson
//==================================================================================================================================
#ifndef __TREE_H
#define __TREE_H
//==================================================================================================================================
//==================================================================================================================================

//
// Includes
//

#include "D3D.h"
#include "TreeParameters.h"
#include "TreeShader.h"
#include "OBJMesh.h"
#include "MeshAttributes.h"

//==================================================================================================================================
//==================================================================================================================================
// TODO: Billboards http://braynzarsoft.net/index.php?p=D3D11BILLBOARDS#still
class Tree
{
public:
	
	Tree(D3D* d3d, TreeParameters tp);
	
	void Init();
	
	void Render(ZShadeSandboxMesh::MeshRenderParameters mrp);
	
private:
	
	void CreateLeaves();
	void CreateMesh();
	
	// Leaf data
	ID3D11ShaderResourceView* mLeafTexture;
	ZShadeSandboxMesh::MeshAttributes* mLeafAttributes;
	
	// Actual Tree is an obj mesh (need instancing with obj mesh system)
	ZShadeSandboxMesh::OBJMesh* mTree;
	
	TreeParameters* mTreeParameters;
	
	D3D* mD3DSystem;
	TreeShader* mShader;
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__TREE_H