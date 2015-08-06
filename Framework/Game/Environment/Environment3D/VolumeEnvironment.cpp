#include "VolumeEnvironment.h"
#include "ZShadeMessageCenter.h"
#include "MaterialManager.h"
//===============================================================================================================================
//===============================================================================================================================
VolumeEnvironment::VolumeEnvironment(EngineOptions* eo)
:   Environment3D(eo)
{
	ZShadeSandboxMesh::MeshParameters mp;
	mp.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
	mp.rotationAxisX = false;
	mp.rotationAxisY = false;
	mp.rotationAxisZ = false;
	mp.pos = XMFLOAT3(0, 0, 0);
	mp.rot = XMFLOAT3(0, 0, 0);
	mp.scale = XMFLOAT3(30, 30, 30);
	mp.material = MaterialManager::Instance()->GetMaterial("Bricks");
	mp.in2D = false;
	mp.shader = 0;
	mp.useCustomShader = false;
	
	mCube = new ZShadeSandboxMesh::CubeMesh(m_D3DSystem, mp);
	mCube->CreatePhysicsBody();
	
	mp.vertexType = ZShadeSandboxMesh::EVertexType::VT_Color;
	mp.pos = XMFLOAT3(80, 100, 80);
	mp.scale = XMFLOAT3(100, 100, 100);

	mVolume = new Volume(m_D3DSystem, mp);
}
//===============================================================================================================================
VolumeEnvironment::~VolumeEnvironment()
{
	Shutdown();
}
//===============================================================================================================================
bool VolumeEnvironment::Initialize()
{
	return true;
}
//===============================================================================================================================
void VolumeEnvironment::Shutdown()
{
	Environment3D::Shutdown();
}
//===============================================================================================================================
void VolumeEnvironment::Update()
{
	// Check to see if the cube enters the volume
	/*if (mVolume->IntersectsAABB(m_CameraSystem->Position()))
	{
		m_PostProcessEnable = true;
	}
	else
	{
		m_PostProcessEnable = false;
	}
	*/
	mCube->SetWireframe( bWireframeMode );
	mCube->Update( m_Timer.DeltaSecondsF() );
	
	mVolume->SetWireframe(true);
	mVolume->Update( m_Timer.DeltaSecondsF() );
}
//===============================================================================================================================
void VolumeEnvironment::Render()
{
	//Quick wireframe mode
	if( Quickwire() )
	{
		mCube->SetWireframe( true );
		mVolume->SetWireframe( true );
		m_D3DSystem->TurnOnWireframe();
	}
	
	ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.camera = m_CameraSystem;
	
	//
	// Render the cube with defaults
	//
	
	mCube->Render(mrp);
	
	//
	// Render the volume with defaults
	//
	
	m_D3DSystem->TurnOnWireframe();
	mVolume->Render(mrp);
}
//===============================================================================================================================
void VolumeEnvironment::RenderDeferred()
{

}
//===============================================================================================================================