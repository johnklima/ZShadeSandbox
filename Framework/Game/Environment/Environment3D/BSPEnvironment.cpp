#include "BSPEnvironment.h"
#include "ShapeContact.h"
#include "MaterialManager.h"
#include "XMMath3.h"
//===============================================================================================================================
//===============================================================================================================================
BSPEnvironment::BSPEnvironment(EngineOptions* eo)
:   Environment3D(eo)
{
	Initialize();
}
//===============================================================================================================================
BSPEnvironment::~BSPEnvironment()
{
	Shutdown();
}
//===============================================================================================================================
bool BSPEnvironment::Initialize()
{
	//m_CameraSystem->SetPosition(0.0f, 40.0f, 10.0f);

	// Not using any reflections
	m_CameraSystem->SetRenderReflectionView(false);

	mBSP = new ZShadeSandboxBSP::BSP();
	//mBSPMesh = new ZShadeSandboxBSP::BSPMesh(m_D3DSystem, mBSP);
	mBSPRenderer = new ZShadeSandboxBSP::BSPRenderer(m_D3DSystem);
	return true;
}
//===============================================================================================================================
void BSPEnvironment::Shutdown()
{
	Environment3D::Shutdown();
}
//===============================================================================================================================
void BSPEnvironment::Update()
{
	
}
//===============================================================================================================================
void BSPEnvironment::Render()
{
	if( Quickwire() )
	{
		
		m_D3DSystem->TurnOnWireframe();
	}
	else
	{
		// We want to see inside the cube
		if (!bWireframeMode)
			m_D3DSystem->TurnOffCulling();
	}
	
	//m_D3DSystem->TurnOffZBuffer();
	ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.camera = m_CameraSystem;
	mrp.light = mDirLight1;
	mBSPRenderer->RenderBSP(mrp, mFrustumPlanes);
	//m_D3DSystem->TurnOnZBuffer();
}
//===============================================================================================================================
void BSPEnvironment::RenderDeferred()
{
}
//===============================================================================================================================