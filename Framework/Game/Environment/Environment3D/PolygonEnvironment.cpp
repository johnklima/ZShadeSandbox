#include "PolygonEnvironment.h"
#include "ShapeContact.h"
#include "MaterialManager.h"
#include "ShaderMaterial.h"
#include "XMMath3.h"
#include "Vertex.h"
//===============================================================================================================================
//===============================================================================================================================
PolygonEnvironment::PolygonEnvironment(EngineOptions* eo)
:   Environment3D(eo)
{
	Initialize();
}
//===============================================================================================================================
PolygonEnvironment::~PolygonEnvironment()
{
	Shutdown();
}
//===============================================================================================================================
bool PolygonEnvironment::Initialize()
{
	// Position the camera around the polygon
	m_CameraSystem->SetPosition(5.0f, 0.0f, 5.0f);
	
	bEnableReflections = true;
	bEnableRefractions = true;
	
	m_CameraSystem->SetRenderReflectionView(bEnableReflections);
	
	fSeaLevel = -5.0f;
	
	mPolygon = new ZShadeSandboxMath::Polygon();
	
	// The UV coordinate will calculated automatically by the polygon for each added vertex
	mPolygon->AddVertex(ZShadeSandboxMesh::VertexUtil::LoadVertexTex(0, 0, 0, 0, 0));
	mPolygon->AddVertex(ZShadeSandboxMesh::VertexUtil::LoadVertexTex(1, 0, 1, 0, 0));
	mPolygon->AddVertex(ZShadeSandboxMesh::VertexUtil::LoadVertexTex(0, 0, 1, 0, 0));
	mPolygon->AddVertex(ZShadeSandboxMesh::VertexUtil::LoadVertexTex(0, 0, 0, 0, 0));
	mPolygon->AddVertex(ZShadeSandboxMesh::VertexUtil::LoadVertexTex(1, 0, 0, 0, 0));
	mPolygon->AddVertex(ZShadeSandboxMesh::VertexUtil::LoadVertexTex(1, 0, 1, 0, 0));
	
	ZShadeSandboxLighting::ShaderMaterial* material = MaterialManager::Instance()->GetMaterial("Wall");

	mPolygon->Attributes()->mD3D = m_D3DSystem;

	mPolygonMesh = new ZShadeSandboxMesh::PolygonMesh(mPolygon, material);
	mPolygonMesh->Position() = XMFLOAT3(0, 0, 0); // Position the mesh around the origin
	mPolygonMesh->Scale() = XMFLOAT3(1, 1, 1);
	
	mPolygonMesh->BuildVertexBuffer(ZShadeSandboxMesh::EVertexType::Type::VT_Tex);
	mPolygonMesh->BuildIndexBuffer();

	WaterParameters wp;
	wp.g_LightDirection = XMFLOAT3(0.936016f, -0.343206f, 0.0780013f);
	wp.g_RefractionTint = XMFLOAT4(0.07f, 0.15f, 0.2f, 1.0f);// 0.0f, 0.8f, 1.0f, 1.0f);
	wp.g_DullColor = XMFLOAT4(0.3f, 0.3f, 0.5f, 1.0f);
	wp.g_SpecularShininess = 40;
	wp.g_WaveHeight = 0.3f;
	wp.g_waterHeight = fSeaLevel;
	wp.g_waterRadius = 2000.0f;
	wp.g_TexScale = 2.5f;
	
	mWater = new Water();
	mWater->Initialize(m_D3DSystem, wp);
	//mWater->SetUseFlowmap(true);
	mWater->LoadFlowMapSRV(m_GameDirectory3D->m_textures_path + "\\flowmap.png");
	mWater->LoadNoiseMapSRV(m_GameDirectory3D->m_textures_path + "\\noise.png");
	mWater->LoadWaveMap0SRV(m_GameDirectory3D->m_textures_path + "\\wave0.dds");
	mWater->LoadWaveMap1SRV(m_GameDirectory3D->m_textures_path + "\\wave1.dds");
	
	return true;
}
//===============================================================================================================================
void PolygonEnvironment::Shutdown()
{
	Environment3D::Shutdown();
}
//===============================================================================================================================
void PolygonEnvironment::Update()
{
	mPolygonMesh->Wireframe() = bWireframeMode;
	
	mWater->SetWireframe( bWireframeMode );
	
	mWater->Update(fFrameTime, fSeaLevel);
	
	UpdateSpawnedMeshItems(fFrameTime);
	
	ToggleLightMeshWireframe(bWireframeMode);
}
//===============================================================================================================================
void PolygonEnvironment::Render()
{
	if( Quickwire() )
	{
		mWater->SetWireframe(true);
		mPolygonMesh->Wireframe() = true;
		ToggleSpawnedMeshItemsWireframe(true);
		ToggleLightMeshWireframe(true);
		
		m_D3DSystem->TurnOnWireframe();
	}
	else
	{
		if (!bWireframeMode)
		{
			m_D3DSystem->TurnOffCulling();
		}
	}
	
	ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.camera = m_CameraSystem;
	mrp.light = mDirLight1;
	
	mPolygonMesh->Render(mrp);
	
	RenderSpawnedMeshItems(mrp);
	
	RenderLightMesh(mrp);
	
	//
	// Render the water
	//
	
	if (!bWireframeMode && !Quickwire()) m_D3DSystem->TurnOffCulling();
	if (Quickwire())
	{
		mWater->SetWireframe(true);
		m_D3DSystem->TurnOnWireframe();
	}
	mWater->SetFarPlane(mEngineOptions->fFarPlane);
	mWater->Render(m_CameraSystem, false, mReflectionTexture->SRView, mRefractionTexture->SRView);
	if (!bWireframeMode && !Quickwire()) m_D3DSystem->TurnOnCulling();
}
//===============================================================================================================================
void PolygonEnvironment::RenderDeferred()
{
	if( Quickwire() )
	{
		mWater->SetWireframe(true);
		mPolygonMesh->Wireframe() = true;
		ToggleSpawnedMeshItemsWireframe(true);
		ToggleLightMeshWireframe(true);
		
		m_D3DSystem->TurnOnWireframe();
	}
	else
	{
		if (!bWireframeMode)
		{
			m_D3DSystem->TurnOffCulling();
		}
	}
	
	ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.camera = m_CameraSystem;
	mrp.renderDeferred = true;
	mrp.light = mDirLight1;
	
	mPolygonMesh->Render(mrp);
	
	RenderSpawnedMeshItems(mrp);
	
	RenderLightMesh(mrp);
	
	//
	// Render the water
	//
	
	if (!bWireframeMode && !Quickwire()) m_D3DSystem->TurnOffCulling();
	if (Quickwire())
	{
		mWater->SetWireframe(true);
		m_D3DSystem->TurnOnWireframe();
	}
	mWater->SetFarPlane(mEngineOptions->fFarPlane);
	mWater->Render(m_CameraSystem, true, mReflectionTexture->SRView, mRefractionTexture->SRView);
	if (!bWireframeMode && !Quickwire()) m_D3DSystem->TurnOnCulling();
}
//===============================================================================================================================
void PolygonEnvironment::RenderRefraction(XMFLOAT4 clipplane)
{
	//
	// Render the polygon + spawned mesh container using the refraction clip plane to produce the refraction effect
	//
	
	ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.camera = m_CameraSystem;
	mrp.light = mDirLight1;
	mrp.reflection = false;
	mrp.clipplane = clipplane;
	
	mPolygonMesh->Render(mrp);
	
	RenderSpawnedMeshItems(mrp);
	
	RenderLightMesh(mrp);
}
//===============================================================================================================================
void PolygonEnvironment::RenderReflection(XMFLOAT4 clipplane)
{
	//
	// Render the polygon + spawned mesh container using the reflection view matrix and reflection clip plane
	//
	
	ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.camera = m_CameraSystem;
	mrp.light = mDirLight1;
	mrp.reflection = true;
	mrp.clipplane = clipplane;
	
	mPolygonMesh->Render(mrp);
	
	RenderSpawnedMeshItems(mrp);
	
	RenderLightMesh(mrp);
}
//===============================================================================================================================