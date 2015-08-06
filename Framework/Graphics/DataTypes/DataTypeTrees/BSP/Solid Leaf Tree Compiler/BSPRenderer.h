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

//==================================================================================================================================
//==================================================================================================================================
namespace ZShadeSandboxBSP {
class BSPRenderer
{
public:
	
	BSPRenderer(D3D* d3d);
	~BSPRenderer();
	
	void RenderBSP(ZShadeSandboxMesh::MeshRenderParameters mrp, XMFLOAT4 frustumPlanes[6]);
	
	bool LeafInFrustum(long leaf, XMFLOAT4 frustumPlanes[6]);
	
	bool LineOfSight(ZShadeSandboxMath::XMMath3 start, ZShadeSandboxMath::XMMath3 end, long node);
	
	bool& DontFrustumReject() { return bDontFrustumReject; }
	
private:
	
	void RenderBSP(long leaf, ZShadeSandboxMesh::MeshRenderParameters mrp, XMFLOAT4 frustumPlanes[6]);
	
	ZShadeSandboxBSP::BSPMesh* mBSPMesh;
	
	bool bDontFrustumReject;
};
}
//==================================================================================================================================
//==================================================================================================================================
#endif//__BSPRENDERER_H