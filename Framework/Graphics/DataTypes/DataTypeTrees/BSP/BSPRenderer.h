//==================================================================================================================================
// BSPRenderer
//
//==================================================================================================================================
// History
//
// -Created on 6/24/2015 by Dustin Watson
//==================================================================================================================================
#ifndef __BSPRENDERER_H
#define __BSPRENDERER_H
//==================================================================================================================================
//==================================================================================================================================

//
// Includes
//

#include "BSPPolygon.h"
#include "BSPParameters.h"
#include "Camera.h"
#include "BSPNode.h"
#include "BSP.h"
#include "BSPMesh.h"
#include "MeshRenderParameters.h"

//==================================================================================================================================
//==================================================================================================================================
namespace ZShadeSandboxBSP {
class BSPRenderer
{
public:
	
	BSPRenderer(D3D* d3d);
	~BSPRenderer();
	
	void RenderBSP(ZShadeSandboxMesh::MeshRenderParameters mrp, XMFLOAT4 frustumPlanes[6]);
	
	bool LineOfSight(ZShadeSandboxMath::XMMath3 start, ZShadeSandboxMath::XMMath3 end);
	
	/*
		Point collision with the BSP

		D3DXVECTOR3 NewPos=position+(25.4f*look_vector)*time_elapsed;
		D3DXVECTOR3 NewPos2=position+(65.4f*look_vector)*time_elapsed;

		if (CollideSphere(&NewPos2,0.5,0)==false)
		{
			position=NewPos;
		}
	*/
	bool CollideSphere(ZShadeSandboxMath::XMMath3 center, float radius);

	// Format is in .bsp format

	// Save the level data
	void SaveBSP(string filename);
	
	// Loads the level data from a saved game
	void LoadBSP(string filename);
	
	bool& DontFrustumReject() { return bDontFrustumReject; }
	
private:
	
	bool LeafInFrustum(long leaf, XMFLOAT4 frustumPlanes[6]);

	bool LineOfSight(ZShadeSandboxMath::XMMath3 start, ZShadeSandboxMath::XMMath3 end, long node);
	bool CollideSphere(ZShadeSandboxMath::XMMath3 center, float radius, long node);

	void RenderBSP(long leaf, ZShadeSandboxMesh::MeshRenderParameters mrp, XMFLOAT4 frustumPlanes[6]);
	
	ZShadeSandboxBSP::BSPMesh* mBSPMesh;
	
	bool bDontFrustumReject;
};
}
//==================================================================================================================================
//==================================================================================================================================
#endif//__BSPRENDERER_H