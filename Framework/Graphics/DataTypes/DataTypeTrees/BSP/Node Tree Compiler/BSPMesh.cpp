#include "BSPMesh.h"
#include "MemoryMgr.h"
#include "XMMath.h"
#include "ZMath.h"
#include "MaterialManager.h"
using ZShadeSandboxBSP::BSPMesh;
//==================================================================================================================================
//==================================================================================================================================
BSPMesh::BSPMesh(D3D* d3d, ZShadeSandboxBSP::BSP* bsp)
:   m_D3DSystem(d3d)
,	mBSP(bsp)
{
	m_EngineOptions = m_D3DSystem->GetEngineOptions();
	mMaterial = MaterialManager::Instance()->GetMaterial("Metal");
	mMaterial->bEnableLighting = false;
	GenerateBuffers();
}
//==================================================================================================================================
void BSPMesh::GenerateBuffers()
{
	GenerateBuffers(mBSP->GetRootNode());
}
//==================================================================================================================================
void BSPMesh::GenerateBuffers(ZShadeSandboxBSP::BSPNode*& node)
{
	if (node->isLeaf)
	{
		return;
	}
	
	if (node->splitterPolygon != 0)
	{
		node->splitterPolygon->CreateMesh(m_D3DSystem, mMaterial);
		node->splitterPolygon->BuildVertexBuffer();
		node->splitterPolygon->BuildIndexBuffer();
	}
	
	if (node->frontChild != 0)
	{
		GenerateBuffers(node->frontChild);
	}
	
	if (node->backChild != 0)
	{
		GenerateBuffers(node->backChild);
	}
}
//==================================================================================================================================