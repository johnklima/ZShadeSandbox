#include "OutdoorEnvironment.h"
#include "LightManager.h"
#include "CapsuleLight.h"
#include "AmbientLight.h"
#include "LightShadingBuffers.h"
#include "MaterialManager.h"
//===============================================================================================================================
//===============================================================================================================================
OutdoorEnvironment::OutdoorEnvironment(EngineOptions* eo)
:   Environment3D(eo)
,	bWaterFog(false)
,	bToggleWater(true)
,	bRenderOcean(false)
,	bToggleTerrain(true)
,   bClipCameraToTerrain(false)
,   bRenderExtraRenderDist(false)
,   bRenderFullMap(false)
{
	Init();
}
//===============================================================================================================================
OutdoorEnvironment::~OutdoorEnvironment()
{
	Shutdown();
}
//===============================================================================================================================
bool OutdoorEnvironment::Init()
{
	fHeightScale = 1.0f;
	fTerrSize = 1.0f;
	
	bEnableReflections = true;
	bEnableRefractions = true;
	bToggleTerrainSSAO = true;
	bToggleTerrainShadow = true;
	bToggleTerrainNormalMap = true;
	bToggleTerrainDetailMap = true;

	m_CameraSystem->SetPosition(0.0f, 100.0f, 100.0f);
	
	fSeaLevel = 0.0f;// 1500.0f;
	m_CameraSystem->SetRenderReflectionView( bEnableReflections );

	//mCamSpeedFast = 2.0f;
	//mCamSpeedSlow = 0.5f;

	// Create a random infinite plane
	ZShadeSandboxMesh::MeshParameters mp;
	mp.useCustomShader = false;
	mp.vertexType = ZShadeSandboxMesh::EVertexType::VT_Tex;
	mp.rotationAxisX = false;
	mp.rotationAxisY = false;
	mp.rotationAxisZ = false;
	mp.pos = XMFLOAT3(0, 0, 0);
	mp.rot = XMFLOAT3(0, 0, 0);
	mp.scale = XMFLOAT3(1, 1, 1);
	mp.material = MaterialManager::Instance()->GetMaterial("Floor");
	mp.in2D = false;
	mp.shader = 0;
	m_pInfinitePlane = new InfinitePlane(100.0f, 0.0f, 50.0f, m_D3DSystem, mp);
	
	float size = 10;
	float r = size / 100;
	mp.scale = XMFLOAT3(size, size, size);
	mp.pos = XMFLOAT3(0, 5, 0);
	mSphere = new ZShadeSandboxMesh::SphereMesh(r, size, size, m_D3DSystem, mp);
	
	mp.pos = XMFLOAT3(0, 5, 10);
	mGeosphereMesh = new ZShadeSandboxMesh::GeosphereMesh(0.5f, 2, m_D3DSystem, mp);

	mp.scale = XMFLOAT3(1, 1, 1);
	mp.pos = XMFLOAT3(10, 5, 10);
	mCylindricalMesh = new ZShadeSandboxMesh::CylindricalMesh(0.5f, 0.5f, 3.0f, 10, 10, m_D3DSystem, mp);

	mp.pos = XMFLOAT3(0, 5, 20);
	mCircleMesh = new ZShadeSandboxMesh::CircleMesh(m_D3DSystem, 0.5f, mp);
	
	m_SpawnedMeshContainer.push_back(mSphere);
	m_SpawnedMeshContainer.push_back(mGeosphereMesh);
	m_SpawnedMeshContainer.push_back(mCylindricalMesh);
	
	//mSunQuad = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp, true);
	//mp.scale = XMFLOAT3(50, 50, 50);
	//mVolume = new Volume(m_D3DSystem, mp);
	
	// shapes
	// cube
	// spaceCompound
	// MaleLow
	mMesh = new ZShadeSandboxMesh::OBJMesh(m_D3DSystem, m_GameDirectory3D);
	mMesh->Load("Models//spaceCompound.obj", false, true);
	mMesh->Scale(XMFLOAT3(0.3f, 0.3f, 0.3f));
	mMesh->Position(XMFLOAT3(100, 15, 300));
	
	InitializeTerrain();
	InitializeWater();
	InitializeOcean();
	
	// Enable the lights for this scene
	ZShadeSandboxLighting::LightManager::Instance()->ToggleDirectionalLights(true);
	
	ZShadeSandboxMesh::MeshParameters mpSphere;
	mpSphere.useCustomShader = false;
	mpSphere.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
	mpSphere.rotationAxisX = false;
	mpSphere.rotationAxisY = false;
	mpSphere.rotationAxisZ = false;
	mpSphere.pos = XMFLOAT3(0, 5, 0);
	mpSphere.rot = XMFLOAT3(0, 0, 0);
	mpSphere.scale = XMFLOAT3(1, 1, 1);
	mpSphere.material = MaterialManager::Instance()->GetMaterial("Red");
	mpSphere.material->bEnableLighting = false;
	mpSphere.in2D = false;
	mpSphere.shader = 0;
	mPickingSphere = new ZShadeSandboxMesh::SphereMesh(m_D3DSystem, mpSphere, "Models\\sphere.txt");

	return true;
}
//===============================================================================================================================
bool OutdoorEnvironment::InitializeTerrain()
{
	//gTerrSize = 200;

	//gHeightScale = gTerrSize * 2;

	//gMinTessellationDist *= gTerrSize;
	//gMaxTessellationDist *= gTerrSize;

	/*fMapSize = 256;

	float    fMinTessellationDist = 20.0f;
	float    fMaxTessellationDist = 500.0f;
	float    fMinTessellation = 0.0f;
	float    fMaxTessellation = 64.0f;
	float    fFogStart = 5.0f;//15.0f;
	float    fFogRange = 1000.0f;//175.0f;
	XMVECTOR gFogColor = DXColors::Silver;
	int      iLeafSize = 32;
	int      iTerrSize = 1;
	float    fHeightScale = 200.0f;
	float    fCellSpacing = 0.5f;

	//fCamSpeedFast *= fMapSize;
	//fCamSpeedSlow *= fMapSize;
	
	bEnableTessellation = true;

	fMinDist = fMinTessellationDist;
	fMaxDist = fMapSize;// / 2;
	
	fTerrRenderSize = 1.0f;
	
	//m_pTerrainShader = new TerrainShader(m_D3DSystem, "Terrain\\Terrain.hlsl");

	//m_pQuadTree = new ZShadeSandboxTerrain::QuadTree(mD3DSystem, "Textures/Terrain/Island3Big.bmp", 0, gLeafSize, gCellSpacing, gHeightScale, BMP, gTessellate, gTerrSize);
	//m_pQuadTree = new ZShadeSandboxTerrain::QuadTree(m_D3DSystem, "Textures/Terrain/valleyBig.bmp", 0, gLeafSize, gCellSpacing, gHeightScale, BMP, gTessellate, gTerrSize);
	//m_pQuadTree = new ZShadeSandboxTerrain::QuadTree(m_D3DSystem, "Textures\\Terrain\\terrain.raw", gCellSize, gLeafSize, gCellSpacing, gHeightScale, RAW, gTessellate, gTerrSize);
	//m_pQuadTree = new ZShadeSandboxTerrain::QuadTree(m_D3DSystem, "Textures/Terrain/Chatham-terrain.raw", gCellSize, gLeafSize, gCellSpacing, gHeightScale, RAW, gTessellate, gTerrSize);

	ZShadeSandboxTerrain::TerrainParameters tp;
	//tp.g_heightmapName = "terrain3.bmp";
	//tp.g_heightmapName = "Island3Big.bmp";
	tp.g_heightmapName = "heightmap01.bmp";
	//tp.g_heightmapName = "valleyBig.bmp";
	//tp.g_heightmapName = "gcanyon.bmp";
	//tp.g_heightmapName = "terrain.raw";
	tp.g_TerrainSize = fMapSize;
	tp.g_leafWidth = iLeafSize;
	tp.g_cellSpacing = 0.5f;
	tp.g_heightScale = fHeightScale;
	tp.g_tessellate = bEnableTessellation;
	tp.g_terrScale = fTerrSize;

	m_pQuadTreeMesh = new ZShadeSandboxTerrain::QuadTreeMesh(m_D3DSystem, tp, m_GameDirectory3D);

	//vector<string> names;
	//names.push_back("Textures\\grass.dds");
	//names.push_back("Textures\\darkdirt.dds");
	//names.push_back("Textures\\lightdirt.dds");
	//names.push_back("Textures\\darkgrass.dds");
	//names.push_back("Textures\\soil.dds");
	
	m_pQuadTreeMesh->AddMaterialColors();
	m_pQuadTreeMesh->AddSpecularPower();
	m_pQuadTreeMesh->AddSpecularIntensity();
	m_pQuadTreeMesh->AddNormalMap(m_GameDirectory3D->m_textures_path, "lichen1_normal.dds");
	m_pQuadTreeMesh->AddDetailMap(m_GameDirectory3D->m_textures_path, "detail001.dds");
	//m_pQuadTreeMesh->AddMaterialTextures(names, "Textures", "blend.dds", "lichen1_normal.dds", "detail001.dds");

	//m_pQuadTreeMesh->BuildLayerMapArraySRV("Textures\\grass.dds");
	////m_pQuadTreeMesh->BuildLayerMapArraySRV("Textures\\darkdirt.dds");
	//m_pQuadTreeMesh->BuildLayerMapArraySRV("Textures\\darkdirt.dds");
	//m_pQuadTreeMesh->BuildLayerMapArraySRV("Textures\\lightdirt.dds");
	//m_pQuadTreeMesh->BuildLayerMapArraySRV("Textures\\darkgrass.dds", true);

	/*
	m_pQuadTreeMesh->BuildLayerMapArraySRV("Textures\\grass.dds");
	m_pQuadTreeMesh->BuildLayerMapArraySRV("Textures\\dirt01.dds");
	m_pQuadTreeMesh->BuildLayerMapArraySRV("Textures\\darkdirt.dds");
	m_pQuadTreeMesh->BuildLayerMapArraySRV("Textures\\lightdirt.dds");
	m_pQuadTreeMesh->BuildLayerMapArraySRV("Textures\\darkgrass.dds", true);
	m_pQuadTreeMesh->BuildBlendMapSRV("blend.dds");//blend.dds
	m_pQuadTreeMesh->BuildNormalMapSRV("stones_nmap.dds");// lichen1_normal.dds");
	m_pQuadTreeMesh->BuildDetailMapSRV("detail001.dds");
	*
	
	//m_pQuadTreeMesh->BuildShadowMapSRV();

	m_pQuadTreeRenderer = new ZShadeSandboxTerrain::QuadTreeRenderer(m_D3DSystem, m_pQuadTreeMesh);*/

	ZShadeSandboxTerrain::TerrainParameters tp;

	bool heightmap1 = false;
	bool heightmap2 = false;
	bool heightmap3 = false;

	bool hasHeightmap = true;

	if (heightmap1)
	{
		fMapSize = 256;
		tp.g_heightmapName = "heightmap01.bmp";
	}
	else
	{
		if (heightmap2)
		{
			fMapSize = 1024;
			tp.g_heightmapName = "valleyBig.bmp";
		}
		else
		{
			if (heightmap3)
			{
				fMapSize = 2048;
				tp.g_heightmapName = "terrain.raw";
			}
			else
			{
				// No heightmap
				fMapSize = 128;
				hasHeightmap = false;
			}
		}
	}

	fMinDist = 20.0f;
	fMaxDist = fMapSize / 2;
	fHeightScale = 1.0f;
	fTerrSize = 1.0f;
	fMinDist = 20;
	fMaxDist = fMapSize;
	fTerrRenderSize = 1.0f;
	fLeafWidth = 64.0f;

	tp.g_TerrainSize = fMapSize;
	tp.g_leafWidth = fLeafWidth;
	tp.g_cellSpacing = 0.5f;
	tp.g_heightScale = fHeightScale;
	tp.g_tessellate = bEnableTessellation;
	tp.g_terrScale = fTerrSize;
	tp.g_makeFlat = false;
	tp.g_procedural = (!hasHeightmap) ? true : false;
	tp.g_proceduralParameters.erosionValue = 0.2f;
	tp.g_proceduralParameters.normalizeFactor = 2.0f;
	tp.g_proceduralParameters.smoothingPassCount = 1.0f;
	tp.g_proceduralParameters.seaLevel = fSeaLevel;
	tp.g_proceduralParameters.maxHeight = 100;
	tp.g_proceduralParameters.terrainSize = fMapSize;
	tp.g_proceduralParameters.diamondSquareFeatureSize = fMapSize;
	tp.g_proceduralParameters.diamondSquareScale = 1;
	tp.g_proceduralParameters.proceduralType = ZShadeSandboxTerrain::EProceduralType::Type::eDiamondSquare;
	tp.g_proceduralParameters.diamondSquareType = ZShadeSandboxTerrain::EDiamondSquareType::Type::eBicubic;
	tp.g_proceduralParameters.useErosion = true;
	tp.g_proceduralParameters.useSmoothing = true;
	tp.g_proceduralParameters.normalize = false;
	tp.g_proceduralParameters.useThermalWeathering = false;
	tp.g_proceduralParameters.erosionType = ZShadeSandboxTerrain::EErosionType::Type::eWater;
	tp.g_proceduralParameters.waterErosionParameters.terrainSize = fMapSize;
	tp.g_proceduralParameters.waterErosionParameters.seaLevel = fSeaLevel;
	tp.g_proceduralParameters.waterErosionParameters.waterSourceHeight = 1;
	tp.g_proceduralParameters.waterErosionParameters.thermalPowerMultiplier = 15.0f;
	tp.g_proceduralParameters.waterErosionParameters.deltaT = 0.005f;
	tp.g_proceduralParameters.waterErosionParameters.pipeLength = 1.0f;
	tp.g_proceduralParameters.waterErosionParameters.pipeCrossectionArea = 20.0f;
	tp.g_proceduralParameters.waterErosionParameters.graviationalAcceleration = 9.7f;
	tp.g_proceduralParameters.waterErosionParameters.sedimentCapacityConstant = 1.0f;
	tp.g_proceduralParameters.waterErosionParameters.dissolvingConstant = 0.5f;
	tp.g_proceduralParameters.waterErosionParameters.depositionConstant = 1.0f;
	tp.g_proceduralParameters.waterErosionParameters.minimumComputedSurfaceTilt = 0.1f;
	tp.g_proceduralParameters.waterErosionParameters.talusAngle = 0.5f;
	tp.g_proceduralParameters.waterErosionParameters.erosionDuration = 3;
	tp.g_proceduralParameters.waterErosionParameters.erosionType = ZShadeSandboxTerrain::EWaterErosion::Type::eThermal;
	tp.g_proceduralParameters.waterErosionParameters.normalErosionRounds = 2;
	tp.g_proceduralParameters.waterErosionParameters.normalErosionFactor = 50;
	tp.g_proceduralParameters.waterErosionParameters.enableNormalErosionSedimentation = true;

	m_pQuadTreeMesh = new ZShadeSandboxTerrain::QuadTreeMesh(m_D3DSystem, tp, m_GameDirectory3D);

	// The terrain in the editor does not need to load a blend map from a file initially since it is built
	// when a texture is painted onto the terrain.
	// The blend map is only loaded from a file in the editor or from the terrain loader xml file

	m_pQuadTreeMesh->AddMaterialColors();
	m_pQuadTreeMesh->AddSpecularPower();
	m_pQuadTreeMesh->AddSpecularIntensity();
	m_pQuadTreeMesh->AddNormalMap(m_GameDirectory3D->m_textures_path, "lichen1_normal.dds");
	m_pQuadTreeMesh->AddDetailMap(m_GameDirectory3D->m_textures_path, "detail001.dds");

	m_pQuadTreeRenderer = new ZShadeSandboxTerrain::QuadTreeRenderer(m_D3DSystem, m_pQuadTreeMesh);

	return true;
}
//===============================================================================================================================
bool OutdoorEnvironment::InitializeWater()
{
	WaterParameters wp;
	wp.g_LightDirection = XMFLOAT3(0.936016f, -0.343206f, 0.0780013f);
	wp.g_RefractionTint = XMFLOAT4(0.07f, 0.15f, 0.2f, 1.0f);// 0.0f, 0.8f, 1.0f, 1.0f);
	wp.g_DullColor = XMFLOAT4(0.1f, 0.1f, 0.2f, 1.0f);
	//0.1f, 0.1f, 0.2f, 1.0f
	//0.3f, 0.3f, 0.5f, 1.0f
	wp.g_SpecularShininess = 20;
	wp.g_WaveHeight = 0.3f;
	wp.g_waterHeight = fSeaLevel;
	wp.g_waterRadius = 5000.0f;
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
bool OutdoorEnvironment::InitializeOcean()
{
	// Load the ocean patch
	OceanParameters ocean_params;
	ocean_params.g_LightDirection = mDirLight1->Direction();
	ocean_params.g_RefractionTint = XMFLOAT4(0.07f, 0.15f, 0.2f, 1.0f);
	ocean_params.g_DullColor = XMFLOAT4(0.3f, 0.3f, 0.5f, 1.0f);
	ocean_params.g_SpecularShininess = 400;
	ocean_params.g_WaveHeight = 1.3f;
	ocean_params.g_OceanSize = 8192;           //<JPK> the relative values of these are
	ocean_params.g_PatchSize = 256;            //critical to "scale" of the FFT effect  
	ocean_params.g_MinDist = 10.0f;            //use this to get more mesh detail
	ocean_params.g_MaxDist = 2000.0f;          //and this
	ocean_params.g_MinTess = 4.0f;             //and this      
	ocean_params.g_MaxTess = 256.0f;          //and this 
	ocean_params.g_SeaLevel = fSeaLevel;

	// Perlin wave parameters
	ocean_params.g_PerlinSize = 1.0f;
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

	return true;
}
//===============================================================================================================================
void OutdoorEnvironment::Shutdown()
{
	Environment3D::Shutdown();
}
//===============================================================================================================================
void OutdoorEnvironment::Update()
{
	// Need to get toggle update time based on the amount of triangles rendered
	float dt = fFrameTime;
	
	//Other update info
	if (GetAsyncKeyState( '7' ) & 0x8000 )
	{
		while(1) { Sleep(300); break; }
		bToggleWater = !bToggleWater;
	}
	if (GetAsyncKeyState( '8' ) & 0x8000 )
	{
		while(1) { Sleep(300); break; }
		bRenderOcean = !bRenderOcean;
	}
	if (GetAsyncKeyState( '9' ) & 0x8000 )
	{
		while(1) { Sleep(300); break; }
		bToggleTerrain = !bToggleTerrain;
	}
	//if (GetAsyncKeyState( '6' ) & 0x8000 )
	//{
	//	while(1) { Sleep(300); break; }
	//	bWaterFog = !bWaterFog;
	//}
	
	if (keyboard->IsKeyDown(Keyboard::Key::V))
	{
		fHeightScale *= 1.1f;
		m_pQuadTreeMesh->HeightScale() = fHeightScale;
		m_pQuadTreeMesh->ComputeBounds();
	}
	if (keyboard->IsKeyDown(Keyboard::Key::B))
	{
		fHeightScale *= 0.9f;
		m_pQuadTreeMesh->HeightScale() = fHeightScale;
		m_pQuadTreeMesh->ComputeBounds();
	}

	if (keyboard->RisingEdge(Keyboard::Key::G))
	{
		fTerrSize += 1.0f;
		fMaxDist = fMapSize * fTerrSize;
		m_pQuadTreeMesh->ReGenMesh(fTerrSize);
	}
	if (keyboard->RisingEdge(Keyboard::Key::H))
	{
		fTerrSize -= 1.0f;
		if (fTerrSize <= 0.0f)
			fTerrSize += 1.0f;
		else
		{
			fMaxDist = fMapSize * fTerrSize;
			m_pQuadTreeMesh->ReGenMesh(fTerrSize);
		}
	}
	
	if (keyboard->IsKeyDown(Keyboard::Key::T))
	{
		fMinDist -= 1.0f;
		if (fMinDist <= 0)
			fMinDist = 1.0f;
	}
	if (keyboard->IsKeyDown(Keyboard::Key::Y))
	{
		fMinDist += 1.0f;
	}

	if (keyboard->IsKeyDown(Keyboard::Key::U))
	{
		fMaxDist -= 1.0f;
		if (fMaxDist <= 0)
			fMaxDist = 1.0f;
	}
	if (keyboard->IsKeyDown(Keyboard::Key::I))
	{
		fMaxDist += 1.0f;
	}

	if (keyboard->RisingEdge(Keyboard::Key::C))
	{
		bClipCameraToTerrain = !bClipCameraToTerrain;
	}

	if (keyboard->RisingEdge(Keyboard::Key::K))
	{
		fTerrRenderSize -= 1.0f;
		if (fTerrRenderSize <= 0)
			fTerrRenderSize = 1.0f;
		
		m_pQuadTreeRenderer->RenderSize() = fTerrRenderSize;
	}

	if (keyboard->RisingEdge(Keyboard::Key::L))
	{
		fTerrRenderSize += 1.0f;
		if (fTerrRenderSize >= 6.0f)
			fTerrRenderSize = 6.0f;
		
		m_pQuadTreeRenderer->RenderSize() = fTerrRenderSize;
	}
	
	if (keyboard->RisingEdge(Keyboard::Key::N))
	{
		while(1) { Sleep(300); break; }
		bRenderFullMap = !bRenderFullMap;
	}
	
	if (keyboard->RisingEdge(Keyboard::Key::M))
	{
		while(1) { Sleep(300); break; }
		bRenderExtraRenderDist = !bRenderExtraRenderDist;
	}
	
	if (keyboard->RisingEdge(Keyboard::Key::Q))
	{
		while (1) { Sleep(30); break; }
		bToggleTerrainShadow = !bToggleTerrainShadow;
	}

	if (keyboard->RisingEdge(Keyboard::Key::Z))
	{
		while (1) { Sleep(30); break; }
		bToggleTerrainSSAO = !bToggleTerrainSSAO;
	}

	if (bToggleTerrain)
	{
		m_pQuadTreeRenderer->RenderFullMap() = bRenderFullMap;
		m_pQuadTreeRenderer->RenderExtraRenderDist() = bRenderExtraRenderDist;
		m_pQuadTreeRenderer->SetWireframe(bWireframeMode);
	}

	//m_pInfinitePlane->SetWireframe(m_wire);
	
	UpdateSpawnedMeshItems(dt);
	ToggleLightMeshWireframe(bWireframeMode);
	
	mMesh->SetWireframe(bWireframeMode);

	//mSunRenderer->SetWireframe(bWireframeMode);

	if (bToggleTerrain)
	{
		m_pQuadTreeRenderer->SetWireframe(bWireframeMode);
	}

	// Updates the world matrix for the sun
	//mSun->Update(m_CameraSystem);

	if (bToggleWater)
	{
		if (!bRenderOcean)
		{
			mWater->SetWireframe( bWireframeMode );
			mWater->Update(dt, fSeaLevel);
		}
		else
		{
			mOceanSurface->SetWireframe(bWireframeMode);

			// Update simulation
			static double app_time = 0;
			app_time += (double)dt;
			mOceanSimulator->updateDisplacementMap((float)app_time);
		}
	}
	
	// Count the amount of triangles in the scene
	iTriangleCount = 0;
	
	auto it = m_SpawnedMeshContainer.begin();
	for (; it != m_SpawnedMeshContainer.end(); it++)
	{
		iTriangleCount += (*it)->TriangleCount();
	}
	
	if (mPickingSphere != NULL)
	{
		iTriangleCount += mPickingSphere->TriangleCount();
	}
	
	// Add the triangle count of the light mesh that is rendered
	iTriangleCount += ZShadeSandboxLighting::LightManager::Instance()->TriangleCount();
	
	if (bToggleTerrain)
	{
		iTriangleCount += m_pQuadTreeMesh->TriangleCount();
	}
	
	if (bToggleWater)
	{
		if (!bRenderOcean)
			iTriangleCount += 2; // Water plane has 2 triangles
		else
			; // Ocean triangle count not set yet
	}
	
	if (bToggleSky)
	{
		iTriangleCount += mSky->TriangleCount();
		
		if (bToggleSkyPlane)
		{
			iTriangleCount += mSkyPlane->TriangleCount();
		}
	}
	
	//iTriangleCount += mSpaceCompoundMesh->TriangleCount();
	
	// The scene is rendered 3 times with reflection and refraction enabled
	if (bToggleWater)
	{
		// The sky only has reflections
		if (bToggleSky)
		{
			iTriangleCount += mSky->TriangleCount();
			
			//if (m_ToggleSkyPlane)
			//{
			//	iTriangleCount += mSkyPlane->TriangleCount();
			//}
		}
		
		for (int i = 0; i < 2; i++)
		{
			if (bToggleTerrain)
			{
				iTriangleCount += m_pQuadTreeMesh->TriangleCount();
			}
			
			if (mPickingSphere != NULL)
			{
				iTriangleCount += mPickingSphere->TriangleCount();
			}
			
			it = m_SpawnedMeshContainer.begin();
			for (; it != m_SpawnedMeshContainer.end(); it++)
			{
				iTriangleCount += (*it)->TriangleCount();
			}
			
			//iTriangleCount += mSpaceCompoundMesh->TriangleCount();
			
			iTriangleCount += ZShadeSandboxLighting::LightManager::Instance()->TriangleCount();
		}
	}
	
	BetterString str = "Triangles: ";
	str += ZShadeSandboxGlobal::Convert::ConvertIntToString(iTriangleCount);
	mTriangleCountText.TextName() = str;
}
//===============================================================================================================================
void OutdoorEnvironment::Render()
{
	//RenderTerrainShadowSSAO();

	//We now get the position of the camera and then get the height of the triangle that would
	//be directly underneath it. Once we get the height back we set the height of the camera two
	//units directly above the triangle's height. This way when the camera moves it automatically
	//follows the exact height of the terrain.
	if (bClipCameraToTerrain)
	{
		float height = 0.0f;
		XMFLOAT3 eye = m_CameraSystem->Position();
		
		// Apply simple gravity
		m_CameraSystem->SetPositionY(eye.y - 1.0f);

		// Get the height of the triangle that is directly underneath the given camera position.
		if (m_pQuadTreeMesh->GetHeightAtPosition(eye, height))
		{
			// If there was a triangle under the camera then position the camera just above it by two units.
			m_CameraSystem->SetPositionY(height + 2.0f);
		}
	}
	
	// Mouse Picking Terrain Collision Test
	//POINT p;
	//GetCursorPos(&p);
	//ScreenToClient(m_BaseWindow->GetHwnd(), &p);
	//mPickingRay = m_CameraSystem->PickingRay(p.x, p.y, m_pQuadTreeRenderer->GetWorld());
	mPickingRay = m_CameraSystem->PickingRay(mLastMousePos.x, mLastMousePos.y, m_pQuadTreeRenderer->GetWorld());
	
	if (mPickingRay != NULL)
	{
		//if (bLeftMouseDown)
		{
			//bLeftMouseDown = false;
			bool hit = false;
			XMFLOAT3 hitPoint;
			ZShadeSandboxMath::Ray ray = *mPickingRay;
			m_pQuadTreeMesh->Intersects(ray, hit, hitPoint);
			if (hit)
			{
				mPickingSphere->Position() = hitPoint;
			}
		}
	}

	if (!bWireframeMode)
		m_D3DSystem->TurnOnCulling();
	
	//m_D3DSystem->TurnOnZBuffer();

	//Quick wireframe mode
	if( Quickwire() )
	{
		if (bToggleWater)
		{
			if (!bRenderOcean)
				mWater->SetWireframe( true );
			else
				mOceanSurface->SetWireframe(true);
		}

		if (bToggleTerrain)
		{
			m_pQuadTreeRenderer->SetWireframe(true);
		}

		ToggleSpawnedMeshItemsWireframe(true);
		ToggleLightMeshWireframe(true);

		mMesh->SetWireframe(true);
		
		//mSunRenderer->SetWireframe(true);

		if (bToggleSky)
		{
			mSky->SetWireframe(true);
			
			if (bToggleSkyPlane)
				mSkyPlane->SetWireframe(true);
		}
		
		//m_pInfinitePlane->SetWireframe(true);
		
		m_D3DSystem->TurnOnWireframe();
	}

	ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.camera = m_CameraSystem;
	mrp.light = mDirLight1;
	
	RenderSpawnedMeshItems(mrp);
	
	//
	//Render the terrain
	//

	if (bToggleTerrain)
		RenderTerrain( XMFLOAT4(0,0,0,0), false, false );
	
	// Render a random infinite plane
	//m_D3DSystem->TurnOnWireframe();
	//mVolume->SetWireframe(true);
	//mVolume->Render(mrp);
	//m_pInfinitePlane->SetWireframe(true);
	//mrp.fTessellationFactor = 30.0f;
	//mrp.bTessellate = true;
	//mrp.rt = RenderType::e3ControlPointPatchList;
	//m_pInfinitePlane->Render(mrp);
	//if (!m_wire || Quickwire())
	//	m_D3DSystem->TurnOnCulling();
	//mSphere->Render(mrp);
	//mGeosphereMesh->Render(mrp);
	//mCircleMesh->Render(mrp);
	
	if (mPickingSphere != NULL)
		mPickingSphere->Render(mrp);

	/*if (!m_wire && !Quickwire())
		m_D3DSystem->TurnOffCulling();
	mCylindricalMesh->Render(mrp);
	if (!m_wire && !Quickwire())
		m_D3DSystem->TurnOnCulling();*/

	//
	//Render some lake water or ocean water
	//

	if (bToggleWater)
	{
		if (!bRenderOcean)
			RenderWater();
		else
			RenderOcean();
	}

	//ZShadeSandboxMesh::OBJMeshRenderParameters omrp;
	//omrp.camera = m_CameraSystem;
	//omrp.renderType = ZShadeSandboxMesh::ERenderType::e3ControlPointPatchList;
	//omrp.tessellate = true;
	mMesh->SetFarPlane(mEngineOptions->fFarPlane);
	mMesh->Render(mrp);

	//
	// Render the sphere mesh for the lights in the scene
	//
	
	RenderLightMesh(mrp);
}
//===============================================================================================================================
void OutdoorEnvironment::RenderDeferred()
{
	//Quick wireframe mode
	if (Quickwire())
	{
		mWater->SetWireframe(true);
		//mOceanSurface->SetWireframe(true);
		mSky->SetWireframe(true);
		ToggleSpawnedMeshItemsWireframe(true);
		ToggleLightMeshWireframe(true);
		
		m_D3DSystem->TurnOnWireframe();
	}

	//
	// Render the water
	//

	if (bToggleWater)
	{
		if (!bRenderOcean)
		{
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
		else
		{
			// No GBuffer for Ocean yet
		}
	}

	ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.camera = m_CameraSystem;
	mrp.renderDeferred = true;
	mrp.light = mDirLight1;

	RenderSpawnedMeshItems(mrp);

	mMesh->SetFarPlane(mEngineOptions->fFarPlane);
	mMesh->Render(mrp);

	mPickingSphere->Render(mrp);

	RenderLightMesh(mrp);
}
//===============================================================================================================================
void OutdoorEnvironment::RenderShadowMap()
{

}
//===============================================================================================================================
void OutdoorEnvironment::RenderReflection(XMFLOAT4 clipplane)
{
	ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.camera = m_CameraSystem;
	mrp.light = mDirLight1;
	mrp.reflection = true;
	mrp.clipplane = clipplane;
	
	RenderSpawnedMeshItems(mrp);
	
	//
	// Render the terrain using the reflection view matrix and reflection clip plane
	//

	if (bToggleTerrain)
		RenderTerrain(clipplane, true, false);
	
	if (mPickingSphere != NULL)
		mPickingSphere->Render(mrp);
	
	RenderLightMesh(mrp);
	
	mMesh->SetFarPlane(mEngineOptions->fFarPlane);
	mMesh->Render(mrp);
}
//===============================================================================================================================
void OutdoorEnvironment::RenderRefraction(XMFLOAT4 clipplane)
{
	ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.camera = m_CameraSystem;
	mrp.light = mDirLight1;
	mrp.reflection = false;
	mrp.clipplane = clipplane;
	
	RenderSpawnedMeshItems(mrp);

	//
	// Render the terrain using the refraction clip plane to produce the refraction effect
	//

	if (bToggleTerrain)
		RenderTerrain(clipplane, false, true);
	
	if (mPickingSphere != NULL)
		mPickingSphere->Render(mrp);
	
	RenderLightMesh(mrp);
	
	mMesh->SetFarPlane(mEngineOptions->fFarPlane);
	mMesh->Render(mrp);
}
//===============================================================================================================================
bool OutdoorEnvironment::RenderTerrainShadowSSAO()
{
	XMFLOAT4 fogColor;
	XMStoreFloat4(&fogColor, DXColors::Silver);

	ZShadeSandboxTerrain::TerrainShadingConst tsc;
	tsc.g_EyePosW = m_CameraSystem->Position();
	tsc.g_fogThinning = 1000;
	tsc.g_fogAltitude = 10;
	tsc.g_fogNear = 1.0f;
	tsc.g_fogFar = 1000.0f;
	tsc.g_fogColor = fogColor;
	tsc.g_TextureAmount = 1; // Amount of Layer textures (Not needed)
	tsc.g_seaLevel = fSeaLevel;
	tsc.g_waterBodyColor = XMFLOAT4(0.07f, 0.15f, 0.2f, 0.0f);

	float minTess = 0.0f;
	float maxTess = 64.0f;

	/*tsc.g_AmbientDown = mAmbientDown;
	tsc.g_AmbientLightCount = ZShadeSandboxLighting::LightManager::Instance()->AmbientLightCount();
	tsc.g_DirectionalLightCount = ZShadeSandboxLighting::LightManager::Instance()->DirectionalLightCount();
	tsc.g_SpotLightCount = ZShadeSandboxLighting::LightManager::Instance()->SpotLightCount();
	tsc.g_PointLightCount = ZShadeSandboxLighting::LightManager::Instance()->PointLightCount();
	tsc.g_CapsuleLightCount = ZShadeSandboxLighting::LightManager::Instance()->CapsuleLightCount();
	tsc.g_AmbientUp = mAmbientUp;*/
	tsc.g_useClippingPlane = 0;
	tsc.g_UseSobelFilter = 0;
	tsc.g_useNormalMap = (bToggleTerrainNormalMap == true) ? 1 : 0;
	tsc.g_useShadowMap = 0;// (bToggleTerrainShadow == true) ? 1 : 0;
	tsc.g_useSSAO = 0;// (bToggleTerrainSSAO == true) ? 1 : 0;
	tsc.g_MinDist = fMinDist;
	tsc.g_MaxDist = fMaxDist;
	tsc.g_MinTess = minTess;
	tsc.g_MaxTess = maxTess;
	tsc.g_FrustumCull = 0;
	tsc.g_HeightScale = fHeightScale;// gHeightScale;
	tsc.g_TerrSize = fTerrSize;// gTerrSize;
	tsc.g_TextureWidth = fMapSize;
	tsc.g_TextureHeight = fMapSize;
	tsc.g_TexelCellSpaceU = (1 / fMapSize);
	tsc.g_TexelCellSpaceV = (1 / fMapSize);
	tsc.g_ClipPlane = XMFLOAT4(0, 0, 0, 0);
	tsc.g_MapSize = fMapSize;
	tsc.g_DetailBrightness = 1.8f;

	m_pQuadTreeMesh->HeightScale() = fHeightScale;
	m_pQuadTreeMesh->TerrainZScale() = fTerrSize;

	// Render the shadow map for the terrain
	m_pQuadTreeRenderer->RenderShadowMap(m_CameraSystem, mDirLight1->Perspective(), tsc);
	
	m_D3DSystem->SetBackBufferRenderTarget();
	// Pop RT and reset to the normal view
	m_D3DSystem->ResetViewport();
	
	m_pQuadTreeRenderer->RenderSSAO(m_CameraSystem, mDirLight1->Perspective(), tsc);

	m_D3DSystem->SetBackBufferRenderTarget();
	// Pop RT and reset to the normal view
	m_D3DSystem->ResetViewport();

	return true;
}
//===============================================================================================================================
bool OutdoorEnvironment::RenderTerrain(XMFLOAT4 clipplane, bool bReflection, bool bRefraction)
{
	XMFLOAT4 fogColor;
	XMStoreFloat4(&fogColor, DXColors::Silver);

	ZShadeSandboxTerrain::TerrainShadingConst tsc;
	tsc.g_EyePosW = m_CameraSystem->Position();
	tsc.g_fogThinning = 1000;
	tsc.g_fogAltitude = 10;
	tsc.g_fogNear = 1.0f;
	tsc.g_fogFar = 1000.0f;
	tsc.g_fogColor = fogColor;
	tsc.g_TextureAmount = 1; // Amount of Layer textures (Not needed)
	tsc.g_seaLevel = fSeaLevel;
	tsc.g_waterBodyColor = XMFLOAT4(0.07f, 0.15f, 0.2f, 0.0f);
	
	float minTess = 0.0f;
	float maxTess = 64.0f;

	/*tsc.g_AmbientDown = mAmbientDown;
	tsc.g_AmbientLightCount = ZShadeSandboxLighting::LightManager::Instance()->AmbientLightCount();
	tsc.g_DirectionalLightCount = ZShadeSandboxLighting::LightManager::Instance()->DirectionalLightCount();
	tsc.g_SpotLightCount = ZShadeSandboxLighting::LightManager::Instance()->SpotLightCount();
	tsc.g_PointLightCount = ZShadeSandboxLighting::LightManager::Instance()->PointLightCount();
	tsc.g_CapsuleLightCount = ZShadeSandboxLighting::LightManager::Instance()->CapsuleLightCount();
	tsc.g_AmbientUp = mAmbientUp;*/
	tsc.g_useClippingPlane = ((bReflection || bRefraction) == true) ? 1 : 0;
	tsc.g_UseSobelFilter = 0;
	tsc.g_useNormalMap = (bToggleTerrainNormalMap == true) ? 1 : 0;
	tsc.g_useShadowMap = 0;// (bToggleTerrainShadow == true) ? 1 : 0;
	tsc.g_useSSAO = 0;// (bToggleTerrainSSAO == true) ? 1 : 0;
	tsc.g_MinDist = fMinDist;
	tsc.g_MaxDist = fMaxDist;
	tsc.g_MinTess = minTess;
	tsc.g_MaxTess = maxTess;
	tsc.g_FrustumCull = 0;
	tsc.g_HeightScale = fHeightScale;// gHeightScale;
	tsc.g_TerrSize = fTerrSize;// gTerrSize;
	tsc.g_TextureWidth = fMapSize;
	tsc.g_TextureHeight = fMapSize;
	tsc.g_TexelCellSpaceU = (1 / fMapSize);
	tsc.g_TexelCellSpaceV = (1 / fMapSize);
	tsc.g_ClipPlane = clipplane;
	tsc.g_MapSize = fMapSize;
	tsc.g_DetailBrightness = 1.8f;
	tsc.g_useReflection = bReflection;

	m_pQuadTreeMesh->HeightScale() = fHeightScale;
	m_pQuadTreeMesh->TerrainZScale() = fTerrSize;
	
	m_pQuadTreeRenderer->Render(m_CameraSystem, mDirLight1, tsc);

	return true;
}
//===============================================================================================================================
bool OutdoorEnvironment::RenderWater()
{
	//
	//Render the Water
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

	return true;
}
//===============================================================================================================================
bool OutdoorEnvironment::RenderOcean()
{
	//
	//Render the Ocean
	//

	if (!bWireframeMode && !Quickwire()) m_D3DSystem->TurnOffCulling();
	if (Quickwire())
	{
		mOceanSurface->SetWireframe(true);
		m_D3DSystem->TurnOnWireframe();
	}
	
	mOceanSurface->SetReflectionMap(mReflectionTexture->SRView);
	mOceanSurface->SetRefractionMap(mRefractionTexture->SRView);
	mOceanSurface->Render(m_CameraSystem, fSeaLevel, fFrameTime);
	if (!bWireframeMode && !Quickwire()) m_D3DSystem->TurnOnCulling();

	return true;
}
//===============================================================================================================================