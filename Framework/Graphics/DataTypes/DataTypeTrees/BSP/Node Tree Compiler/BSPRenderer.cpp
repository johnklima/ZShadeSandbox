#include "BSPRenderer.h"
using ZShadeSandboxBSP::BSPRenderer;
//==================================================================================================================================
//==================================================================================================================================
BSPRenderer::BSPRenderer(ZShadeSandboxBSP::BSPMesh* mesh)
:	mBSPMesh(mesh)
{
}
//==================================================================================================================================
void BSPRenderer::RenderBSP(ZShadeSandboxMesh::MeshRenderParameters mrp)
{
	RenderBSP(mBSPMesh->GetBSP()->GetRootNode(), mrp);
}
//==================================================================================================================================
void BSPRenderer::RenderBSP(ZShadeSandboxBSP::BSPNode* node, ZShadeSandboxMesh::MeshRenderParameters mrp)
{
	// Cannot do anything if there is no camera
	if (mrp.camera == 0) return;
	
	if (node->isLeaf)
	{
		return;
	}

	ZShadeSandboxBSP::EPointPosition::Type result = ZShadeSandboxBSP::BSP::ClassifyPoint(mrp.camera->Position(), node->splitterPolygon);

	if (result == ZShadeSandboxBSP::EPointPosition::Type::eFront)
	{
		if (node->backChild != 0)
		{
			RenderBSP(node->backChild, mrp);
		}

		// Render the node's splitter polygon
		node->splitterPolygon->Mesh()->Render(mrp);

		if (node->frontChild != 0)
		{
			RenderBSP(node->frontChild, mrp);
		}

		return;
	}

	// this happens if the camera is at the back of the plane or on a plane

	if (node->frontChild != 0)
	{
		RenderBSP(node->frontChild, mrp);
	}

	if (node->backChild != 0)
	{
		RenderBSP(node->backChild, mrp);
	}
}
//==================================================================================================================================