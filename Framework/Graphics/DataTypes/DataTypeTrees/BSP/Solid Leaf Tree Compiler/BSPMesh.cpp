#include "BSPMesh.h"
#include "MemoryMgr.h"
#include "XMMath.h"
#include "ZMath.h"
#include "MaterialManager.h"
#include "BSPArrayData.h"
using ZShadeSandboxBSP::BSPMesh;
//==================================================================================================================================
//==================================================================================================================================
BSPMesh::BSPMesh(D3D* d3d)
:   m_D3DSystem(d3d)
{
	m_EngineOptions = m_D3DSystem->GetEngineOptions();
	mMaterial = MaterialManager::Instance()->GetMaterial("Metal");
	mMaterial->bEnableLighting = false;
	GenerateBuffers();
}
//==================================================================================================================================
void BSPMesh::GenerateBuffers()
{
	BSPArrayData* arrayData = BSPArrayData::Instance();
	
	// Loop through the polygon array and create mesh for all the polygons
	for (int i = 0; i < arrayData->lNumberOfPolygons; i++)
	{
		arrayData->polygonArray[i]->CreateMesh(m_D3DSystem, mMaterial);
		arrayData->polygonArray[i]->BuildVertexBuffer();
		arrayData->polygonArray[i]->BuildIndexBuffer();
	}
}
//==================================================================================================================================