#include "IndoorEnvironment.h"
#include "ShapeContact.h"
#include "MaterialManager.h"
#include "XMMath3.h"
//===============================================================================================================================
//===============================================================================================================================
IndoorEnvironment::IndoorEnvironment(EngineOptions* eo)
:   Environment3D(eo)
{
	ZShadeSandboxMesh::MeshParameters mp;
	mp.useCustomShader = false;
	mp.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
	mp.rotationAxisX = false;
	mp.rotationAxisY = false;
	mp.rotationAxisZ = false;
	mp.pos = XMFLOAT3(0, 0, 0);
	mp.rot = XMFLOAT3(0, 0, 0);
	mp.scale = XMFLOAT3(100, 100, 100);
	mp.material = MaterialManager::Instance()->GetMaterial("Wall");
	mp.in2D = false;
	mp.shader = 0;
	mCube = new ZShadeSandboxMesh::CubeMesh(m_D3DSystem, mp);
}
//===============================================================================================================================
IndoorEnvironment::~IndoorEnvironment()
{
	Shutdown();
}
//===============================================================================================================================
bool IndoorEnvironment::Initialize()
{
	return true;
}
//===============================================================================================================================
void IndoorEnvironment::Shutdown()
{
	Environment3D::Shutdown();
}
//===============================================================================================================================
void IndoorEnvironment::Update()
{
	mCube->SetWireframe( bWireframeMode );
	
	// Enable collision against the bottom of the cube when gravity is turned on
	
	// Apply simple gravity
	//m_CameraSystem->SetPositionY(eye.y - 0.3f);
	
	// I think that this will make the camera collide on top of the cube
	// Since the check is whether or not the camera is inside the cube
	// for the contact normal to be applied.
	
	// Clip the camera to the cube
	XMFLOAT3 eye = m_CameraSystem->Position();
	if (mCube->IntersectsAABB(eye))
	{
		// Find the contact normal and push the camera away from the cube
		
		float dist;
		ZShadeSandboxMath::XMMath3 contactNormal = ShapeContact::NormalFromPoint(*mCube->GetAABB(), eye, dist);
		
		// Point the contact normal to look inside the cube
		if (contactNormal.x > 0) contactNormal.x = -contactNormal.x;
		if (contactNormal.y > 0) contactNormal.y = -contactNormal.y;
		if (contactNormal.z > 0) contactNormal.z = -contactNormal.z;
		
		// The contact normal is already normalized
		ZShadeSandboxMath::XMMath3 newPoint(eye);
		newPoint += contactNormal;
		
		m_CameraSystem->SetPosition(newPoint.x, newPoint.y, newPoint.z);
	}
}
//===============================================================================================================================
void IndoorEnvironment::Render()
{
	if( Quickwire() )
	{
		mCube->SetWireframe( true );
		
		m_D3DSystem->TurnOnWireframe();
	}
	else
	{
		// We want to see inside the cube
		if (!bWireframeMode)
			m_D3DSystem->TurnOffCulling();
	}
	
	ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.camera = m_CameraSystem;
	mCube->Render(mrp);
}
//===============================================================================================================================
void IndoorEnvironment::RenderDeferred()
{

}
//===============================================================================================================================