#include "TessellationEnvironment.h"
#include "ZShadeMessageCenter.h"
#include "MaterialManager.h"
//===============================================================================================================================
//===============================================================================================================================
TessellationEnvironment::TessellationEnvironment(EngineOptions* eo)
:   Environment3D(eo)
,   fTessFactor(6.0f)
{
	bEnableCulling = false;

	ZShadeSandboxMesh::MeshParameters mp;
	mp.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
	mp.rotationAxisX = false;
	mp.rotationAxisY = false;
	mp.rotationAxisZ = false;
	mp.pos = XMFLOAT3(0, 0, 0);
	mp.rot = XMFLOAT3(0, 1, 0);
	mp.scale = XMFLOAT3(1, 1, 1);
	mp.material = MaterialManager::Instance()->GetMaterial("Bricks");
	mp.in2D = false;
	mp.shader = 0;
	mp.useCustomShader = false;
	mp.bUseTessellation = true; // Enable Tessellation
	
	mTessellatedQuad = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp, 1024, 1024, false);
	
	mp.pos = XMFLOAT3(4, 0, 4);
	mTessellatedTri = new ZShadeSandboxMesh::TriangleMesh(m_D3DSystem, mp);
	
	mp.pos = XMFLOAT3(4, 4, 4);
	mp.rotationAxisY = true;
	mTessellatedCube = new ZShadeSandboxMesh::CubeMesh(m_D3DSystem, mp);

	mp.pos = XMFLOAT3(0, 0, 2);
	mp.bUseTessellation = false;
	mRegularQuad = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp, 1024, 1024, false);
}
//===============================================================================================================================
TessellationEnvironment::~TessellationEnvironment()
{
	Shutdown();
}
//===============================================================================================================================
bool TessellationEnvironment::Initialize()
{
	return true;
}
//===============================================================================================================================
void TessellationEnvironment::Shutdown()
{
	Environment3D::Shutdown();
}
//===============================================================================================================================
void TessellationEnvironment::Update()
{
	mTessellatedQuad->SetWireframe( bWireframeMode );
	mTessellatedQuad->Update( m_Timer.DeltaSecondsF() );
	
	mTessellatedTri->SetWireframe(bWireframeMode);
	mTessellatedTri->Update( m_Timer.DeltaSecondsF() );
	
	mTessellatedCube->SetWireframe(bWireframeMode);
	mTessellatedCube->Update(m_Timer.DeltaSecondsF());

	mRegularQuad->SetWireframe(bWireframeMode);
	mRegularQuad->Update(m_Timer.DeltaSecondsF());
	
	if (keyboard->RisingEdge(Keyboard::Key::F9))
	{
		while (1) { Sleep(100); break; }
		fTessFactor -= 1.0f;
		if (fTessFactor <= 0)
			fTessFactor += 1.0f;
	}

	if (keyboard->RisingEdge(Keyboard::Key::F10))
	{
		while (1) { Sleep(100); break; }
		fTessFactor += 1.0f;
		if (fTessFactor >= 6.0f)
			fTessFactor -= 1.0f;
	}
}
//===============================================================================================================================
void TessellationEnvironment::Render()
{
	if (!bWireframeMode) m_D3DSystem->TurnOffCulling();
	
	//Quick wireframe mode
	if( Quickwire() )
	{
		mRegularQuad->SetWireframe(true);
		mTessellatedQuad->SetWireframe(true);
		mTessellatedTri->SetWireframe(true);
		mTessellatedCube->SetWireframe(true);
		m_D3DSystem->TurnOnWireframe();
	}
	
	ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.camera = m_CameraSystem;
	
	//
	// Render the tessellated quad
	//
	
	mrp.tessellate = true;
	//mrp.fTessellationFactor = fTessFactor;
	mrp.renderType = ZShadeSandboxMesh::ERenderType::e4ControlPointPatchList;
	mTessellatedQuad->Render(mrp);
	
	//
	// Render the tessellated triangle
	//
	
	mrp.renderType = ZShadeSandboxMesh::ERenderType::e3ControlPointPatchList;
	mTessellatedTri->Render(mrp);
	
	//
	// Render the tessellated cube
	//

	mTessellatedCube->Render(mrp);

	//
	// Render the regular quad
	//
	
	mrp.tessellate = false;
	mrp.renderType = ZShadeSandboxMesh::ERenderType::eTriangleList;
	mRegularQuad->Render(mrp);

	m_D3DSystem->TurnOnCulling();
}
//===============================================================================================================================
void TessellationEnvironment::RenderDeferred()
{

}
//===============================================================================================================================