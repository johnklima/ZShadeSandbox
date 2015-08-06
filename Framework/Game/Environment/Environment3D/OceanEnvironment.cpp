#include "OceanEnvironment.h"
#include "MaterialManager.h"
//===============================================================================================================================
//===============================================================================================================================
OceanEnvironment::OceanEnvironment(EngineOptions* eo)
:   Environment3D(eo)
{
	//The Size of the map is dependent on the Near Clipping Plane
	//The smaller the map the smaller the Near Clipping Plane
	//The bigger the map the bigger the Near Clipping Plane
	//The Near Clipping Plane is influenced by the size of the map
	
	Init();
}
//===============================================================================================================================
OceanEnvironment::~OceanEnvironment()
{
	Shutdown();
}
//===============================================================================================================================
bool OceanEnvironment::Init()
{
	bEnableReflections = true;
	bEnableRefractions = true;
	bEnableShadows = false;

	bEnableDeferredShading = false;

	fCamSpeedFast = 2.09f;
	fCamSpeedSlow = 2.05f;

	fSeaLevel = -5.0f;
	m_CameraSystem->SetRenderReflectionView( bEnableReflections );
	m_CameraSystem->SetPositionY(5.0f);

	WaterParameters wp;
	wp.g_LightDirection = XMFLOAT3(0.936016f, -0.343206f, 0.0780013f);
	wp.g_RefractionTint = XMFLOAT4(0.07f, 0.15f, 0.2f, 1.0f);// 0.0f, 0.8f, 1.0f, 1.0f);
	wp.g_DullColor = XMFLOAT4(0.3f, 0.3f, 0.5f, 1.0f);
	//0.1f, 0.1f, 0.2f, 1.0f
	//0.3f, 0.3f, 0.5f, 1.0f
	wp.g_SpecularShininess = 20;
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
	
	ZShadeSandboxMesh::MeshParameters mp;
	mp.useCustomShader = false;
	mp.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
	mp.rotationAxisX = false;
	mp.rotationAxisY = false;
	mp.rotationAxisZ = false;
	mp.pos = XMFLOAT3(0, -5, 0);
	mp.rot = XMFLOAT3(0, 0, 0);
	mp.scale = XMFLOAT3(3, 3, 3);
	mp.shader = 0;
	mp.material = MaterialManager::Instance()->GetMaterial("Wall");
	mCubeAboveWater = new ZShadeSandboxMesh::CubeMesh(m_D3DSystem, mp, "Models\\cube.txt");
	mp.pos = XMFLOAT3(0, -25, -40);
	mCubeBelowWater = new ZShadeSandboxMesh::CubeMesh(m_D3DSystem, mp, "Models\\cube.txt");
	
	mLakeBed = new LakeBed(m_D3DSystem, 512, 512, 512);
	mLakeBed->LoadDiffuseMap(m_GameDirectory3D->m_textures_path + "\\sand.png");
	mLakeBed->LoadWaveMap0(m_GameDirectory3D->m_textures_path + "\\wave0.dds");
	mLakeBed->LoadWaveMap1(m_GameDirectory3D->m_textures_path + "\\wave1.dds");
	mLakeBed->LoadMicroBump0(m_GameDirectory3D->m_textures_path + "\\lichen1_normal.dds");
	mLakeBed->LoadMicroBump1(m_GameDirectory3D->m_textures_path + "\\floor_nmap.dds");
	mLakeBed->LoadDepthMap(m_GameDirectory3D->m_textures_path + "\\lightmap.png");
	
	// Load the ocean patch
	OceanParameters ocean_params;
	ocean_params.g_LightDirection = mDirLight1->Direction();
	ocean_params.g_RefractionTint = XMFLOAT4(0.07f, 0.15f, 0.2f, 1.0f);
	ocean_params.g_DullColor = XMFLOAT4(0.3f, 0.3f, 0.5f, 1.0f);
	ocean_params.g_SpecularShininess = 400;
	ocean_params.g_WaveHeight = 1.3f;
	ocean_params.g_OceanSize = 8192 ;           //<JPK> the relative values of these are
	ocean_params.g_PatchSize = 256 ;            //critical to "scale" of the FFT effect  
	ocean_params.g_MinDist = 10.0f ;            //use this to get more mesh detail
	ocean_params.g_MaxDist = 2000.0f ;          //and this
	ocean_params.g_MinTess = 4.0f ;             //and this      
	ocean_params.g_MaxTess = 256.0f ;          //and this 
	ocean_params.g_SeaLevel = fSeaLevel;

	// Perlin wave parameters
	ocean_params.g_PerlinSize = 16.0f;
	ocean_params.g_PerlinSpeed = 0.06f;
	ocean_params.g_PerlinAmplitude = XMFLOAT3(35, 42, 57);
	ocean_params.g_PerlinGradient = XMFLOAT3(1.4f, 1.6f, 2.2f);
	ocean_params.g_PerlinOctave = XMFLOAT3(1.12f, 0.59f, 0.23f);

	//FFT PARAMS :
	// Adjust this parameter to control the simulation speed
	ocean_params.g_TimeScale = 0.8f;
	// A scale to control the amplitude. Not the world space height
	ocean_params.g_WaveAmplitude = 0.35f;
	// 2D wind direction. No need to be normalized
	ocean_params.g_WindDir = XMFLOAT2(0.8f, 0.6f);
	// The bigger the wind speed, the larger scale of wave crest.
	// But the wave scale can be no larger than patch_length
	ocean_params.g_WindSpeed = 600.0f;
	// Damp out the components opposite to wind direction.
	// The smaller the value, the higher wind dependency
	ocean_params.g_WindDependency = 0.07f;
	// Control the scale of horizontal movement. Higher value creates
	// pointy crests.
	ocean_params.g_ChoppyScale = 1.3f;
	//size of the height map
	ocean_params.g_MapDimension = 512;
	//<JPK> the bigger the length, the more aggressive the effect
	ocean_params.g_PatchLength = 4000;
	
	mOceanSurface = new OceanSurface(m_D3DSystem, ocean_params);
	mOceanSimulator = new OceanSimulator(ocean_params, m_D3DSystem->GetDevice11());

	//update for the first time
	mOceanSimulator->updateDisplacementMap(0);

	//pass the buffer pointers to all concerned parties
	mOceanSurface->mSurfaceShader->SetTexDisplacement(mOceanSimulator->m_pDisplacementSRV);
	mOceanSurface->mSurfaceShader->SetTexGradient(mOceanSimulator->m_pGradientSRV);
	mOceanSurface->mSurfaceShader->SetTexPerlin(mOceanSimulator->m_pPerlinSRV);

	ZShadeSandboxLighting::LightManager::Instance()->ToggleDirectionalLights(true);
	
	//<JPK> place camera in a useful position to see more of the surface
	m_CameraSystem->SetPosition(-50.0f, 300.0f, -50.0f);
	
	XMFLOAT3 campos(mOceanSurface->mPatchTree->GetPatches()->center);
	campos.y += 300;
	m_CameraSystem->SetPosition(campos);
	
	return true;
}
//===============================================================================================================================
void OceanEnvironment::Shutdown()
{
	Environment3D::Shutdown();
}
//===============================================================================================================================
void OceanEnvironment::Update()
{
	float dt = fFrameTime;
	
	if (keyboard->IsKeyDown(Keyboard::Key::P))
	{
		while(1) { Sleep(50); break; }
		fSeaLevel += fCamSpeedSlow + 1.0f;
	}
	if (keyboard->IsKeyDown(Keyboard::Key::O))
	{
		while(1) { Sleep(50); break; }
		fSeaLevel -= fCamSpeedSlow + 1.0f;
	}
	
	//mWater->SetWireframe( bWireframeMode );
	mOceanSurface->SetWireframe(bWireframeMode);
	mCubeAboveWater->SetWireframe(bWireframeMode);
	mCubeBelowWater->SetWireframe(bWireframeMode);
	//mLakeBed->SetWireframe( bWireframeMode );

	//mWater->Update(dt, fSeaLevel);
	//mLakeBed->Update(dt);// m_Timer.ConstantTime());
	
	// Update simulation
	static double app_time = 0;
	app_time += (double)dt;
	mOceanSimulator->updateDisplacementMap((float)app_time);

	// Count the amount of triangles in the scene
	iTriangleCount = 0;
	
	//iTriangleCount += 2; // Water plane has 2 triangles
	iTriangleCount += mSky->TriangleCount();
	iTriangleCount += mCubeAboveWater->TriangleCount();
	iTriangleCount += mCubeBelowWater->TriangleCount();
	//iTriangleCount += mLakeBed->TriangleCount();
	
	// Accounting for the sky + lake bed + cube reflections on the water
	iTriangleCount += mSky->TriangleCount();
	iTriangleCount += mCubeAboveWater->TriangleCount();
	iTriangleCount += mCubeBelowWater->TriangleCount();
	//iTriangleCount += mLakeBed->TriangleCount();
	
	BetterString str = "Triangles: ";
	str += ZShadeSandboxGlobal::Convert::ConvertIntToString(iTriangleCount);
	mTriangleCountText.TextName() = str;
}
//===============================================================================================================================
void OceanEnvironment::Render()
{
	//mCubeAboveWater->SetLightBuffer(ZShadeSandboxLighting::LightManager::Instance()->GetLightBuffer());
	//mCubeAboveWater->SetLightBuffer(ZShadeSandboxLighting::LightManager::Instance()->GetSunLightBuffer());

	//mCubeBelowWater->SetLightBuffer(ZShadeSandboxLighting::LightManager::Instance()->GetLightBuffer());
	//mCubeBelowWater->SetLightBuffer(ZShadeSandboxLighting::LightManager::Instance()->GetSunLightBuffer());

	//Quick wireframe mode
	if( Quickwire() )
	{
		//mWater->SetWireframe(true);
		mOceanSurface->SetWireframe(true);
		mSky->SetWireframe(true);
		mCubeAboveWater->SetWireframe(true);
		mCubeBelowWater->SetWireframe(true);
		//mLakeBed->SetWireframe(true);
		m_D3DSystem->TurnOnWireframe();
	}
	
	//
	// Render the mesh representing the floor of the water system
	//
	
	//mLakeBed->SetDepthMap(mShadowTexture->SRView);
	//mLakeBed->Render(m_CameraSystem, mDirLight1->Perspective());
	
	//
	// Render a cube in the water
	//
	
	ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.camera = m_CameraSystem;
	mrp.light = mDirLight1;
	mrp.useInstancing = false;
	
	//mCubeAboveWater->SetFarPlane(mEngineOptions->fFarPlane);
	//mCubeAboveWater->SetShadowMapSRV(mShadowTexture->SRView);
	//mCubeAboveWater->Render(mrp);
	//mCubeBelowWater->SetFarPlane(mEngineOptions->fFarPlane);
	//mCubeBelowWater->SetShadowMapSRV(mShadowTexture->SRView);
	//mCubeBelowWater->Render(mrp);
	
	//
	// Render the ocean
	//

	if (!bWireframeMode && !Quickwire()) m_D3DSystem->TurnOffCulling();
	if (Quickwire())
	{
		//mWater->SetWireframe(true);
		mOceanSurface->SetWireframe(true);
		m_D3DSystem->TurnOnWireframe();
	}
	//mWater->SetFarPlane(mEngineOptions->fFarPlane);
	//mWater->Render(m_CameraSystem.get(), false, mReflectionTexture->SRView, mRefractionTexture->SRView);

	mOceanSurface->SetReflectionMap(mReflectionTexture->SRView);
	mOceanSurface->SetRefractionMap(mRefractionTexture->SRView);
	ID3D11DeviceContext* dc;
	m_D3DSystem->GetDevice11()->GetImmediateContext(&dc);
	mOceanSurface->Render(m_CameraSystem, fSeaLevel, fFrameTime);
	if (!bWireframeMode && !Quickwire()) m_D3DSystem->TurnOnCulling();
}
//===============================================================================================================================
void OceanEnvironment::RenderDeferred()
{
	//Quick wireframe mode
	if (Quickwire())
	{
		mWater->SetWireframe(true);
		mOceanSurface->SetWireframe(true);
		mSky->SetWireframe(true);
		mCubeAboveWater->SetWireframe(true);
		mCubeBelowWater->SetWireframe(true);
		mLakeBed->SetWireframe(true);
		m_D3DSystem->TurnOnWireframe();
	}

	ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.camera = m_CameraSystem;
	mrp.light = mDirLight1;
	mrp.useInstancing = false;
	mrp.renderDeferred = true;

	mCubeAboveWater->SetFarPlane(mEngineOptions->fFarPlane);
	mCubeAboveWater->SetShadowMapSRV(mShadowTexture->SRView);
	mCubeAboveWater->Render(mrp);
	mCubeBelowWater->SetFarPlane(mEngineOptions->fFarPlane);
	mCubeBelowWater->SetShadowMapSRV(mShadowTexture->SRView);
	mCubeBelowWater->Render(mrp);

	//
	// Render the mesh representing the floor of the water system
	//

	//mLakeBed->Render(m_CameraSystem, mDirLight1->Perspective(), true);

	//
	// Render the ocean
	//

	if (!bWireframeMode && !Quickwire()) m_D3DSystem->TurnOffCulling();
	if (Quickwire())
	{
		//mWater->SetWireframe(true);
		m_D3DSystem->TurnOnWireframe();
	}
	//mWater->SetFarPlane(mEngineOptions->fFarPlane);
	//mWater->Render(m_CameraSystem, true, mReflectionTexture->SRView, mRefractionTexture->SRView);
	//mOceanSurface->SetReflectionMap(mReflectionTexture->SRView);
	//mOceanSurface->SetRefractionMap(mRefractionTexture->SRView);
	//mOceanSurface->Render(m_CameraSystem.get(), fSeaLevel, fFrameTime);
	if (!bWireframeMode && !Quickwire()) m_D3DSystem->TurnOnCulling();
}
//===============================================================================================================================
void OceanEnvironment::RenderShadowMap()
{
	//mCubeAboveWater->SetLightBuffer(ZShadeSandboxLighting::LightManager::Instance()->GetLightBuffer());
	//mCubeAboveWater->SetLightBuffer(ZShadeSandboxLighting::LightManager::Instance()->GetSunLightBuffer());

	//mCubeBelowWater->SetLightBuffer(ZShadeSandboxLighting::LightManager::Instance()->GetLightBuffer());
	//mCubeBelowWater->SetLightBuffer(ZShadeSandboxLighting::LightManager::Instance()->GetSunLightBuffer());

	//
	// Render the lake bed + cube to produce a shadow map texture for depth calculation with caustics
	// on the bed of the water
	//
	
	mDirLight1->Update();
	
	//mLakeBed->Render(m_CameraSystem, mDirLight1->Perspective(), true);
	
	ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.camera = m_CameraSystem;
	mrp.light = mDirLight1;
	mrp.shadowMap = true;
	//mCubeAboveWater->Render(mrp);
	//mCubeBelowWater->Render(mrp);
}
//===============================================================================================================================
void OceanEnvironment::RenderRefraction(XMFLOAT4 clipplane)
{
	//mCubeAboveWater->SetLightBuffer(ZShadeSandboxLighting::LightManager::Instance()->GetLightBuffer());
	//mCubeAboveWater->SetLightBuffer(ZShadeSandboxLighting::LightManager::Instance()->GetSunLightBuffer());

	//mCubeBelowWater->SetLightBuffer(ZShadeSandboxLighting::LightManager::Instance()->GetLightBuffer());
	//mCubeBelowWater->SetLightBuffer(ZShadeSandboxLighting::LightManager::Instance()->GetSunLightBuffer());

	//
	// Render the lake bed + cube using the refraction clip plane to produce the refraction effect
	//
	
	mDirLight1->Update();
	
	//mLakeBed->SetDepthMap(mShadowTexture->SRView);
	//mLakeBed->Render(m_CameraSystem, mDirLight1->Perspective(), false, false, clipplane, false);
	
	ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.camera = m_CameraSystem;
	mrp.light = mDirLight1;
	mrp.reflection = false;
	mrp.clipplane = clipplane;
	//mCubeAboveWater->SetShadowMapSRV(mShadowTexture->SRView);
	//mCubeAboveWater->Render(mrp);
	//mCubeBelowWater->SetShadowMapSRV(mShadowTexture->SRView);
	//mCubeBelowWater->Render(mrp);
}
//===============================================================================================================================
void OceanEnvironment::RenderReflection(XMFLOAT4 clipplane)
{
	//mCubeAboveWater->SetLightBuffer(ZShadeSandboxLighting::LightManager::Instance()->GetLightBuffer());
	//mCubeAboveWater->SetLightBuffer(ZShadeSandboxLighting::LightManager::Instance()->GetSunLightBuffer());

	//mCubeBelowWater->SetLightBuffer(ZShadeSandboxLighting::LightManager::Instance()->GetLightBuffer());
	//mCubeBelowWater->SetLightBuffer(ZShadeSandboxLighting::LightManager::Instance()->GetSunLightBuffer());

	//
	// Render the lake bed + cube + sky using the reflection view matrix and reflection clip plane
	//
	
	mDirLight1->Update();
	
	//mLakeBed->SetDepthMap(mShadowTexture->SRView);
	//mLakeBed->Render(m_CameraSystem, mDirLight1->Perspective(), false, false, clipplane, true);
	
	ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.camera = m_CameraSystem;
	mrp.light = mDirLight1;
	mrp.reflection = true;
	mrp.clipplane = clipplane;
	//mCubeAboveWater->SetShadowMapSRV(mShadowTexture->SRView);
	//mCubeAboveWater->Render(mrp);
	//mCubeBelowWater->SetShadowMapSRV(mShadowTexture->SRView);
	//mCubeBelowWater->Render(mrp);
}
//===============================================================================================================================