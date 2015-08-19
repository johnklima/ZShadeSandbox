#include "TerrainEnvironment.h"
#include "LightManager.h"
#include "CapsuleLight.h"
#include "AmbientLight.h"
#include "LightShadingBuffers.h"
#include "MaterialManager.h"
#include "ProjectedCamera.h"
#include "PointColliderController.h"
//===============================================================================================================================
//===============================================================================================================================
TerrainEnvironment::TerrainEnvironment(EngineOptions* eo)
: Environment3D(eo)
, bClipCameraToTerrain(false)
, bRenderExtraRenderDist(false)
, bRenderFullMap(false)
, bToggleTerrainSSAO(false)
, bToggleTerrainShadow(false)
, bToggleTerrainNormalMap(true)
, bToggleTerrainDetailMap(true)
{
	Init();
}
//===============================================================================================================================
TerrainEnvironment::~TerrainEnvironment()
{
	Shutdown();
}
//===============================================================================================================================
void TerrainEnvironment::Init()
{
	bEnableTessellation = true;
	bEnableReflections = false;
	bEnableRefractions = false;

	bTerrainTrianglesAdded = false;

	m_CameraSystem->SetPosition(0.0f, 100.0f, 100.0f);

	fSeaLevel = 0.0f;

	// No Reflections here
	m_CameraSystem->SetRenderReflectionView(false);

	// Enable directional lights for this scene
	ZShadeSandboxLighting::LightManager::Instance()->ToggleDirectionalLights(true);

	//
	// Initialize the terrain
	//

	// The reason why the terrain size has to be specified is because the quad tree builds itself from the terrain size.
	// Also with a procedural heightmap the terrain size has to be specified

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

	//ZShadeSandboxMesh::MeshParameters mp;
	//mp.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
	//mp.rotationAxisX = false;
	//mp.rotationAxisY = false;
	//mp.rotationAxisZ = false;
	//mp.pos = XMFLOAT3(0, 5, 0);
	//mp.rot = XMFLOAT3(0, 0, 0);
	//mp.scale = XMFLOAT3(10, 10, 10);
	//mp.shader = 0;
	//mp.useCustomShader = false;
	//mp.material = MaterialManager::Instance()->GetMaterial("Target");
	////mp.material->SetMaterialDisplacementMapTexture(m_pQuadTreeMesh->GetHeightMapSRV());
	//mp.material->bHasDetailMapTexture = false;
	//mp.material->bEnableLighting = false;
	//mGroundCursorMesh = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp, false, false, ZShadeSandboxMath::EAxis::Type::Axis_Y);

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
}
//===============================================================================================================================
void TerrainEnvironment::Shutdown()
{
	Environment3D::Shutdown();
}
//===============================================================================================================================
void TerrainEnvironment::Update()
{
	// Need to get toggle update time based on the amount of triangles rendered
	float dt = fFrameTime;

	if (keyboard->IsKeyDown(Keyboard::Key::P))
	{
		while (1) { Sleep(50); break; }
		fSeaLevel += fCamSpeedSlow + 1.0f;
	}
	if (keyboard->IsKeyDown(Keyboard::Key::O))
	{
		while (1) { Sleep(50); break; }
		fSeaLevel -= fCamSpeedSlow + 1.0f;
	}

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
		//m_pQuadTreeRenderer->SetRenderSize(m_TerrRenderSize);
		m_pQuadTreeRenderer->RenderSize() = fTerrRenderSize;
	}

	if (keyboard->RisingEdge(Keyboard::Key::L))
	{
		fTerrRenderSize += 1.0f;
		if (fTerrRenderSize >= 6.0f)
			fTerrRenderSize = 6.0f;
		//m_pQuadTreeRenderer->SetRenderSize(m_TerrRenderSize);
		m_pQuadTreeRenderer->RenderSize() = fTerrRenderSize;
	}

	if (keyboard->RisingEdge(Keyboard::Key::N))
	{
		while (1) { Sleep(300); break; }
		bRenderFullMap = !bRenderFullMap;
	}

	if (keyboard->RisingEdge(Keyboard::Key::M))
	{
		while (1) { Sleep(300); break; }
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

	m_pQuadTreeRenderer->RenderFullMap() = bRenderFullMap;
	m_pQuadTreeRenderer->RenderExtraRenderDist() = bRenderExtraRenderDist;
	m_pQuadTreeRenderer->SetWireframe(bWireframeMode);

	vector<ZShadeSandboxMesh::CustomMesh*>::iterator it = m_SpawnedMeshContainer.begin();
	for (; it != m_SpawnedMeshContainer.end(); it++)
	{
		(*it)->Update(dt);
		(*it)->SetWireframe(bWireframeMode);
	}

	//mGroundCursorMesh->SetWireframe(bWireframeMode);

	m_pQuadTreeRenderer->SetWireframe(bWireframeMode);

	XMFLOAT3 SpotLightPos = mSpotLight1->Position();
	XMFLOAT3 PointLightPos = mPointLight->Position();
	XMFLOAT3 CapsuleLightPos = mCapsuleLight->Position();
	XMFLOAT3 eye = m_CameraSystem->Position();
	XMFLOAT3 DirLightPos = mDirLight1->Position();
	//mCapsuleLight->UpdateMeshPosition(XMFLOAT3(eye.x, eye.y + 20.0f, eye.z));

	if (keyboard->IsKeyDown(Keyboard::Key::Up))
	{
		//mPointLight->UpdateMeshPosition(XMFLOAT3(PointLightPos.x + 1.0f, PointLightPos.y, PointLightPos.z));
		//mSpotLight1->UpdateMeshPosition(XMFLOAT3(SpotLightPos.x + 1.0f, SpotLightPos.y, SpotLightPos.z));
		//mCapsuleLight->UpdateMeshPosition(XMFLOAT3(CapsuleLightPos.x + 1.0f, CapsuleLightPos.y, CapsuleLightPos.z));
		//while (1) { Sleep(200); break; }
		mDirLight1->Direction().x += 0.01f;
		mDirLight1->UpdateMeshPosition(XMFLOAT3(DirLightPos.x + 0.01f, DirLightPos.y, DirLightPos.z));
	}
	if (keyboard->IsKeyDown(Keyboard::Key::Down))
	{
		//mPointLight->UpdateMeshPosition(XMFLOAT3(PointLightPos.x - 1.0f, PointLightPos.y, PointLightPos.z));
		//mSpotLight1->UpdateMeshPosition(XMFLOAT3(SpotLightPos.x - 1.0f, SpotLightPos.y, SpotLightPos.z));
		//mCapsuleLight->UpdateMeshPosition(XMFLOAT3(CapsuleLightPos.x - 1.0f, CapsuleLightPos.y, CapsuleLightPos.z));
		//while (1) { Sleep(200); break; }
		mDirLight1->Direction().x -= 0.01f;
		mDirLight1->UpdateMeshPosition(XMFLOAT3(DirLightPos.x - 0.01f, DirLightPos.y, DirLightPos.z));
	}
	if (keyboard->IsKeyDown(Keyboard::Key::Left))
	{
		//mPointLight->UpdateMeshPosition(XMFLOAT3(PointLightPos.x, PointLightPos.y, PointLightPos.z + 1.0f));
		//mSpotLight1->UpdateMeshPosition(XMFLOAT3(SpotLightPos.x, SpotLightPos.y, SpotLightPos.z + 1.0f));
		//mCapsuleLight->UpdateMeshPosition(XMFLOAT3(CapsuleLightPos.x, CapsuleLightPos.y, CapsuleLightPos.z + 1.0f));
		//while (1) { Sleep(200); break; }
		mDirLight1->Direction().z += 0.01f;
		mDirLight1->UpdateMeshPosition(XMFLOAT3(DirLightPos.x, DirLightPos.y, DirLightPos.z + 0.01f));
	}
	if (keyboard->IsKeyDown(Keyboard::Key::Right))
	{
		//mPointLight->UpdateMeshPosition(XMFLOAT3(PointLightPos.x, PointLightPos.y, PointLightPos.z - 1.0f));
		//mSpotLight1->UpdateMeshPosition(XMFLOAT3(SpotLightPos.x, SpotLightPos.y, SpotLightPos.z - 1.0f));
		//mCapsuleLight->UpdateMeshPosition(XMFLOAT3(CapsuleLightPos.x, CapsuleLightPos.y, CapsuleLightPos.z - 1.0f));
		//while (1) { Sleep(200); break; }
		mDirLight1->Direction().x -= 0.01f;
		mDirLight1->UpdateMeshPosition(XMFLOAT3(DirLightPos.x, DirLightPos.y, DirLightPos.z - 0.01f));
	}
	if (keyboard->IsKeyDown(Keyboard::Key::E))
	{
		//mPointLight->UpdateMeshPosition(XMFLOAT3(PointLightPos.x, PointLightPos.y + 1.0f, PointLightPos.z));
		//mSpotLight1->UpdateMeshPosition(XMFLOAT3(SpotLightPos.x, SpotLightPos.y + 1.0f, SpotLightPos.z));
		//mCapsuleLight->UpdateMeshPosition(XMFLOAT3(CapsuleLightPos.x, CapsuleLightPos.y + 1.0f, CapsuleLightPos.z));
		//while (1) { Sleep(200); break; }
		mDirLight1->Direction().y += 0.01f;
		mDirLight1->UpdateMeshPosition(XMFLOAT3(DirLightPos.x, DirLightPos.y + 0.01f, DirLightPos.z));
	}
	if (keyboard->IsKeyDown(Keyboard::Key::R))
	{
		//mPointLight->UpdateMeshPosition(XMFLOAT3(PointLightPos.x, PointLightPos.y - 1.0f, PointLightPos.z));
		//mSpotLight1->UpdateMeshPosition(XMFLOAT3(SpotLightPos.x, SpotLightPos.y - 1.0f, SpotLightPos.z));
		//mCapsuleLight->UpdateMeshPosition(XMFLOAT3(CapsuleLightPos.x, CapsuleLightPos.y - 1.0f, CapsuleLightPos.z));
		//while (1) { Sleep(200); break; }
		mDirLight1->Direction().y -= 0.01f;
		mDirLight1->UpdateMeshPosition(XMFLOAT3(DirLightPos.x, DirLightPos.y - 0.01f, DirLightPos.z));
	}

	// Count the amount of triangles in the scene
	iTriangleCount = 0;

	it = m_SpawnedMeshContainer.begin();
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

	iTriangleCount += m_pQuadTreeMesh->TriangleCount();

	iTriangleCount += mSky->TriangleCount();

	BetterString str = "Triangles: ";
	str += ZShadeSandboxGlobal::Convert::ConvertIntToString(iTriangleCount);
	mTriangleCountText.TextName() = str;
}
//===============================================================================================================================
void TerrainEnvironment::Render()
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
		//m_CameraSystem->SetPositionY(eye.y - 1.0f);

		// Get the height of the triangle that is directly underneath the given camera position.
		if (m_pQuadTreeMesh->GetHeightAtPosition(eye, height))
		{
			// If there was a triangle under the camera then position the camera just above it by two units.
			m_CameraSystem->SetPositionY(height + 4.0f);
		}
	}

	// Mouse Picking Terrain Collision Test
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
				// Create a sphere at the point of intersection
				mPickingSphere->Position() = hitPoint;
			}
		}
	}

	if (!bWireframeMode)
		m_D3DSystem->TurnOnCulling();

	//Quick wireframe mode
	if (Quickwire())
	{
		m_pQuadTreeRenderer->SetWireframe(true);

		vector<ZShadeSandboxMesh::CustomMesh*>::iterator it = m_SpawnedMeshContainer.begin();
		for (; it != m_SpawnedMeshContainer.end(); it++)
		{
			(*it)->SetWireframe(true);
		}

		mSky->SetWireframe(true);

		m_D3DSystem->TurnOnWireframe();
	}

	ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.camera = m_CameraSystem;
	mrp.light = mDirLight1;

	vector<ZShadeSandboxMesh::CustomMesh*>::iterator it = m_SpawnedMeshContainer.begin();
	for (; it != m_SpawnedMeshContainer.end(); it++)
	{
		if ((*it)->MeshType() == ZShadeSandboxMesh::EMeshType::CYLINDER)
		{
			if (!bWireframeMode && !Quickwire())
				m_D3DSystem->TurnOffCulling();
		}

		(*it)->Render(mrp);

		if ((*it)->MeshType() == ZShadeSandboxMesh::EMeshType::CYLINDER)
		{
			if (!bWireframeMode && !Quickwire())
				m_D3DSystem->TurnOnCulling();
		}
	}

	mrp.tessellate = false;
	mrp.renderType = ZShadeSandboxMesh::ERenderType::eTriangleList;

	//
	//Render the terrain
	//

	float    fMinTessellation = 0.0f;
	float    fMaxTessellation = 64.0f;

	XMFLOAT4 fogColor;
	XMStoreFloat4(&fogColor, DXColors::Silver);

	ZShadeSandboxTerrain::TerrainShadingConst tsc;
	tsc.g_EyePosW = m_CameraSystem->Position();
	tsc.g_fogThinning = 1000;
	tsc.g_fogAltitude = 10;
	tsc.g_fogNear = 5.0f;
	tsc.g_fogFar = 1000.0f;
	tsc.g_fogColor = fogColor;
	tsc.g_TextureAmount = 1; // Amount of Layer textures (Not needed)
	tsc.g_seaLevel = fSeaLevel;
	tsc.g_waterBodyColor = XMFLOAT4(0.07f, 0.15f, 0.2f, 0.0f);
	tsc.g_useClippingPlane = 0;
	tsc.g_UseSobelFilter = 0;
	tsc.g_useNormalMap = (bToggleTerrainNormalMap == true) ? 1 : 0;
	tsc.g_useShadowMap = (bToggleTerrainShadow == true) ? 1 : 0;
	tsc.g_useSSAO = (bToggleTerrainSSAO == true) ? 1 : 0;
	tsc.g_MinDist = fMinDist;
	tsc.g_MaxDist = fMaxDist;
	tsc.g_MinTess = fMinTessellation;
	tsc.g_MaxTess = fMaxTessellation;
	tsc.g_FrustumCull = 0;
	tsc.g_HeightScale = fHeightScale;// gHeightScale;
	tsc.g_TerrSize = fTerrSize;// gTerrSize;
	tsc.g_TextureWidth = fMapSize;
	tsc.g_TextureHeight = fMapSize;
	tsc.g_TexelCellSpaceU = (1 / fMapSize);
	tsc.g_TexelCellSpaceV = (1 / fMapSize);
	tsc.g_ClipPlane = XMFLOAT4(0, 0, 0, 0);
	tsc.g_FarPlane = mEngineOptions->fFarPlane;
	tsc.g_MapSize = fMapSize;
	tsc.g_DetailBrightness = 1.8f;
	tsc.g_useReflection = false;

	m_pQuadTreeMesh->HeightScale() = fHeightScale;
	m_pQuadTreeMesh->TerrainZScale() = fTerrSize;

	m_pQuadTreeRenderer->Render(m_CameraSystem, mDirLight1, tsc);

	//
	// Render the picking sphere
	//

	if (mPickingSphere != NULL)
	{
		mPickingSphere->Render(mrp);
	}

	//
	// Render the sphere mesh for the lights in the scene
	//

	RenderLightMesh(mrp);
}
//===============================================================================================================================
void TerrainEnvironment::RenderDeferred()
{

}
//===============================================================================================================================
void TerrainEnvironment::RenderTerrainShadowSSAO()
{
	XMFLOAT4 fogColor;
	XMStoreFloat4(&fogColor, DXColors::Silver);

	ZShadeSandboxTerrain::TerrainShadingConst tsc;
	tsc.g_EyePosW = m_CameraSystem->Position();
	tsc.g_fogThinning = 1000;
	tsc.g_fogAltitude = 10;
	tsc.g_fogNear = 5.0f;
	tsc.g_fogFar = 1000.0f;
	tsc.g_fogColor = fogColor;
	tsc.g_TextureAmount = 1; // Amount of Layer textures (Not needed)
	tsc.g_seaLevel = fSeaLevel;
	tsc.g_waterBodyColor = XMFLOAT4(0.07f, 0.15f, 0.2f, 0.0f);

	float    fMinTessellation = 0.0f;
	float    fMaxTessellation = 64.0f;

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
	tsc.g_useShadowMap = (bToggleTerrainShadow == true) ? 1 : 0;
	tsc.g_useSSAO = (bToggleTerrainSSAO == true) ? 1 : 0;
	tsc.g_MinDist = fMinDist;
	tsc.g_MaxDist = fMaxDist;
	tsc.g_MinTess = fMinTessellation;
	tsc.g_MaxTess = fMaxTessellation;
	tsc.g_FrustumCull = 0;
	tsc.g_HeightScale = fHeightScale;// gHeightScale;
	tsc.g_TerrSize = fTerrSize;// gTerrSize;
	tsc.g_TextureWidth = fMapSize;
	tsc.g_TextureHeight = fMapSize;
	tsc.g_TexelCellSpaceU = (1 / fMapSize);
	tsc.g_TexelCellSpaceV = (1 / fMapSize);
	tsc.g_ClipPlane = XMFLOAT4(0, 0, 0, 0);
	tsc.g_FarPlane = mEngineOptions->fFarPlane;
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
}
//===============================================================================================================================