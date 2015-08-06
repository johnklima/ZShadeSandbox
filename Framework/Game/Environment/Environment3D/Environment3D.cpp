#include "Exceptions.h"
#include "Environment3D.h"
#include "Convert.h"
#include "ZException.h"
#include "ShaderCompiler.h"
#include "LightManager.h"
#include "CapsuleLight.h"
#include "AmbientLight.h"
#include "MaterialManager.h"
#include "DefaultMaterials.h"
#include "TextureManager.h"
#include "ZShadeINIParser.h"
#include "HDR.h"
#include "PostProcessColor.h"
#include "Shaders.h"
//===============================================================================================================================
//===============================================================================================================================
Environment3D::Environment3D(EngineOptions* eo)
:	mEngineOptions(eo)
{
	Init();
}
//===============================================================================================================================
Environment3D::~Environment3D()
{
	Shutdown();
}
//===============================================================================================================================
bool Environment3D::Init()
{
	mPickingSphere = NULL;
	iTriangleCount = 0;
	fTessFactor = 6.0f;
	fSeaLevel = 0.0f;
	fCamSpeedFast = 0.09f;
	fCamSpeedSlow = 0.05f;
	bWireframeMode = false;
	bEnableLighting = true;
	bCameraStill = false;
	bFullScreen = false;
	bEnableTessellation = true;
	bEnableReflections = false;
	bEnableRefractions = false;
	bEnableShadows = false;
	bSpawnCube = false;
	bEnableCulling = true;
	bLeftMouseDown = false;
	bEnablePostProcessing = false;
	bEnableDeferredShading = false;
	bToggleSky = true;
	bToggleSkyPlane = false;
	bToggleGBufferDebugging = false;
	bToggleGBufferDebuggingColors = true;
	bToggleGBufferDebuggingNormals = false;
	bToggleGBufferDebuggingDepth = false;
	
	m_D3DSystem = new D3D();
	if (!m_D3DSystem) return false;
	
	if (!m_D3DSystem->Initialize11(mEngineOptions))
	{
		MessageBox(NULL, "Direct3D Init Failed", "Error", MB_OK);
		return false;
	}
	
	TextureManager::NewInstance(m_D3DSystem);
	
	ZShadeSandboxShader::Shaders::Build(m_D3DSystem);

	// This should be false when a 3D game is being loaded
	bUseEngineTextures = true;

	// Load the Game Directory since the game is starting from here
	if (!mEngineOptions->m_inEditor)
	{
		//Load the game directory
		m_GameDirectory3D = CGlobal::LoadGameDirectory3D(mEngineOptions->m_GameFolderName);

		// Use the textures folder from development
		if (bUseEngineTextures)
		{
			m_GameDirectory3D->m_textures_path = "Textures";
			m_GameDirectory3D->m_models_path = "Models";
			m_GameDirectory3D->m_heightmaps_path = "Textures\\Terrain";
		}

		mEngineOptions->m_GD3D = m_GameDirectory3D;

		// Read the Game.ini file
		ZShadeINIParser iniParser(m_GameDirectory3D->m_game_ini_path, false);
		string gameType = iniParser.GetString("GAME", "GameType");

		if (BetterString(gameType) == "3D")
		{
			// This is what we want
		}

		// Create the default materials and add them to the material manager
		DefaultMaterials materials;
		materials.Build(m_D3DSystem, m_GameDirectory3D->m_textures_path);

		// NOTE: The audio and scripting system currently does not load from a 3D Game Directory

		// Initialize Audio System
		//AudioSystem::NewInstance(m_GameDirectory3D, m_RenderWindow->GetHwnd());

		// Initialize lua scripting system
		//Scripting::NewInstance(m_D3DSystem, m_GameDirectory3D);
	}
	else
	{
		// Probably loading from the editor
	}

	//Load the world matrix
	XMStoreFloat4x4(&mWorld, XMMatrixIdentity());
	
	m_Timer.Initialize();
	
	m_CameraSystem = new Camera(mEngineOptions);
	float aspect = (float)mEngineOptions->m_screenWidth / (float)mEngineOptions->m_screenHeight;
	m_CameraSystem->SetLens(PI / 4, aspect, mEngineOptions->fNearPlane, mEngineOptions->fFarPlane);
	m_CameraSystem->SetPosition(0.0f, 0.0f, -10.0f);
	m_CameraSystem->SetWorld(XMMatrixIdentity());
	
	float xPos = 0.0f;
	float yPos = 10.0f;
	
	fShadowMapWidth = 2048.0f;
	fShadowMapNear = 0.1f;
	fShadowMapFar = 50.0f;

	mDirLight1 = new ZShadeSandboxLighting::DirectionalLight();
	mDirLight1->LightType() = ZShadeSandboxLighting::ELightType::eDirectional;
	mDirLight1->DiffuseColor() = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mDirLight1->AmbientColor() = XMFLOAT4(0.15f, 0.15f, 0.15f, 1.0f);
	mDirLight1->SpecularColor() = XMFLOAT4(0.8f, 0.8f, 0.7f, 1.0f);
	mDirLight1->Position() = XMFLOAT3(xPos, yPos, 0);
	//mDirLight1->Direction() = XMFLOAT3(0.0f, 1.0f, 0.0f);
	//mDirLight1->Direction() = XMFLOAT3(xPos, yPos, 0);
	//mDirLight1->Direction() = XMFLOAT3(0.57735f, -0.57735f, -0.57735f);
	mDirLight1->Direction() = XMFLOAT3(0.936016f, -0.343206f, 0.0780013f);
	//mDirLight1->Direction() = XMFLOAT3(0.37735f, -0.47735f, -0.57735f);
	//mDirLight1->Direction() = XMFLOAT3(0.707f, -0.707f, 0.0f);
	//mDirLight1->Direction() = XMFLOAT3(0.0f, 0.0f, 0.707f);
	mDirLight1->Perspective()->SceneWidth() = fShadowMapWidth;
	mDirLight1->Perspective()->SceneHeight() = fShadowMapWidth;
	mDirLight1->SetLens(fShadowMapNear, fShadowMapFar);

	xPos += 10;
	mDirLight2 = new ZShadeSandboxLighting::DirectionalLight();
	mDirLight2->LightType() = ZShadeSandboxLighting::ELightType::eDirectional;
	mDirLight2->DiffuseColor() = XMFLOAT4(0.5f, 0.2f, 1.2f, 1.0f);
	mDirLight2->AmbientColor() = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mDirLight2->SpecularColor() = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLight2->Position() = XMFLOAT3(xPos, yPos, 0);
	mDirLight2->Direction() = XMFLOAT3(0.575f, -0.735f, 0.5735f);
	mDirLight2->SetLens(mEngineOptions->fNearPlane, mEngineOptions->fFarPlane);

	xPos += 10;
	mDirLight3 = new ZShadeSandboxLighting::DirectionalLight();
	mDirLight3->LightType() = ZShadeSandboxLighting::ELightType::eDirectional;
	mDirLight3->DiffuseColor() = XMFLOAT4(2.2f, 2.2f, 2.2f, 1.0f);
	mDirLight3->AmbientColor() = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mDirLight3->SpecularColor() = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLight3->Position() = XMFLOAT3(xPos, yPos, 0);
	mDirLight3->Direction() = XMFLOAT3(-0.57735f, -0.57735f, -0.57735f);
	mDirLight3->SetLens(mEngineOptions->fNearPlane, mEngineOptions->fFarPlane);

	xPos += 50;
	mSpotLight1 = new ZShadeSandboxLighting::SpotLight();
	mSpotLight1->LightType() = ZShadeSandboxLighting::ELightType::eSpot;
	mSpotLight1->DiffuseColor() = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mSpotLight1->AmbientColor() = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mSpotLight1->SpecularColor() = XMFLOAT4(0.8f, 0.8f, 0.7f, 1.0f);
	mSpotLight1->Position() = XMFLOAT3(xPos, yPos, 0);
	mSpotLight1->Direction() = XMFLOAT3(0.707f, -0.707f, 0.0f);
	mSpotLight1->Range() = 1000.0f;
	mSpotLight1->SpotAngle() = 45.0f;
	mSpotLight1->SpotCosOuterCone() = 10.0f;
	mSpotLight1->SpotInnerConeReciprocal() = 1.0f / 10.0f;
	mSpotLight1->Attenuation() = XMFLOAT3(0.5f, 3.0f, 0.1f);
	mSpotLight1->SetLens(mEngineOptions->fNearPlane, mEngineOptions->fFarPlane);

	//xPos += 50;
	mPointLight = new ZShadeSandboxLighting::PointLight();
	mPointLight->LightType() = ZShadeSandboxLighting::ELightType::ePoint;
	mPointLight->DiffuseColor() = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mPointLight->AmbientColor() = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mPointLight->SpecularColor() = XMFLOAT4(0.8f, 0.8f, 0.7f, 1.0f);
	mPointLight->Position() = XMFLOAT3(10, 10, 0);
	mPointLight->Direction() = XMFLOAT3(0.707f, -0.707f, 0.0f);
	mPointLight->Range() = 100.0f;
	mPointLight->Attenuation() = XMFLOAT3(0.5f, 3.0f, 0.1f);
	mPointLight->SetLens(mEngineOptions->fNearPlane, mEngineOptions->fFarPlane);

	xPos += 30;
	mCapsuleLight = new ZShadeSandboxLighting::CapsuleLight();
	mCapsuleLight->LightType() = ZShadeSandboxLighting::ELightType::eCapsule;
	mCapsuleLight->DiffuseColor() = XMFLOAT4(1.9f, 1.9f, 1.9f, 1.0f);
	mCapsuleLight->AmbientColor() = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mCapsuleLight->SpecularColor() = XMFLOAT4(0.8f, 0.8f, 0.7f, 1.0f);
	mCapsuleLight->Position() = XMFLOAT3(-40, 10, 0);
	mCapsuleLight->Range() = 40.0f;
	//mCapsuleLight->Direction() = XMFLOAT3(0.707f, -0.707f, 0.0f);
	mCapsuleLight->Direction() = XMFLOAT3(0.936016f, -0.343206f, 0.0780013f);
	mCapsuleLight->LightLength() = 50.0f;
	mCapsuleLight->CapsuleDirectionLength() = XMFLOAT3(0.707f, -0.707f, 0.0f);
	mCapsuleLight->Intensity() = 2.0f;
	mCapsuleLight->SetLens(mEngineOptions->fNearPlane, mEngineOptions->fFarPlane);

	xPos += 30;
	ZShadeSandboxLighting::AmbientLight* ambientLight = new ZShadeSandboxLighting::AmbientLight();
	ambientLight->Position() = XMFLOAT3(xPos, yPos, 100);
	ambientLight->AmbientColor() = XMFLOAT4(0.3f, 1.5f, 0.3f, 1.0f);
	ambientLight->SetLens(mEngineOptions->fNearPlane, mEngineOptions->fFarPlane);

	xPos += 30;
	ZShadeSandboxLighting::AmbientLight* ambientLight1 = new ZShadeSandboxLighting::AmbientLight();
	ambientLight1->Position() = XMFLOAT3(xPos, yPos, 100);
	ambientLight1->AmbientColor() = XMFLOAT4(0.3f, 1.5f, 10.3f, 1.0f);
	ambientLight1->SetLens(mEngineOptions->fNearPlane, mEngineOptions->fFarPlane);

	// Initialize the light manager that handles all the basic lights in the scene
	ZShadeSandboxLighting::LightManager::NewInstance(m_D3DSystem);

	ZShadeSandboxLighting::LightManager::Instance()->AddLight(ambientLight);
	//ZShadeSandboxLighting::LightManager::Instance()->AddLight(ambientLight1);
	ZShadeSandboxLighting::LightManager::Instance()->AddLight(mDirLight1);
	//ZShadeSandboxLighting::LightManager::Instance()->AddLight(mDirLight2);
	//ZShadeSandboxLighting::LightManager::Instance()->AddLight(mDirLight3);
	//ZShadeSandboxLighting::LightManager::Instance()->AddLight(mPointLight);
	//ZShadeSandboxLighting::LightManager::Instance()->AddLight(mCapsuleLight);
	//ZShadeSandboxLighting::LightManager::Instance()->AddLight(mSpotLight1);
	
	// Disable all the lights
	//ZShadeSandboxLighting::LightManager::Instance()->ToggleAmbientLights(false);
	//ZShadeSandboxLighting::LightManager::Instance()->ToggleDirectionalLights(false);
	//ZShadeSandboxLighting::LightManager::Instance()->ToggleSpotLights(false);
	//ZShadeSandboxLighting::LightManager::Instance()->TogglePointLights(false);
	//ZShadeSandboxLighting::LightManager::Instance()->ToggleCapsuleLights(false);
	
	mAmbientUp = XMFLOAT3(1.3f, 1.3f, 1.3f);
	mAmbientDown = XMFLOAT3(0.3f, 0.3f, 0.3f);

	ZShadeSandboxLighting::LightManager::Instance()->AmbientUp() = mAmbientUp;
	ZShadeSandboxLighting::LightManager::Instance()->AmbientDown() = mAmbientDown;

	// Build the initial forward lighting buffers
	mSunLightBuffer = new ZShadeSandboxLighting::SunLightBuffer();
	mSunLightBuffer->g_SunDir = XMFLOAT3(0.936016f, -0.343206f, 0.0780013f);
	mSunLightBuffer->g_SunDiffuseColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mSunLightBuffer->g_SunShineness = 500.0f;
	mSunLightBuffer->g_EnableSun = 1;
	ZShadeSandboxLighting::SunLightBuffer sb = *mSunLightBuffer;
	ZShadeSandboxLighting::LightManager::Instance()->RebuildSunBuffer(sb);
	
	// Create the deferred shader
	ZShadeSandboxLighting::DeferredShaderManager::NewInstance(m_D3DSystem);

	ZShadeSandboxLighting::DeferredShaderManager::Instance()->SetAmbientUp(mAmbientUp);
	ZShadeSandboxLighting::DeferredShaderManager::Instance()->SetAmbientDown(mAmbientDown);

	ZShadeSandboxLighting::DeferredShaderManager::Instance()->AddLight(ambientLight);
	//ZShadeSandboxLighting::DeferredShaderManager::Instance()->AddLight(ambientLight1);
	ZShadeSandboxLighting::DeferredShaderManager::Instance()->AddLight(mDirLight1);
	ZShadeSandboxLighting::DeferredShaderManager::Instance()->AddLight(mDirLight2);
	ZShadeSandboxLighting::DeferredShaderManager::Instance()->AddLight(mDirLight3);
	ZShadeSandboxLighting::DeferredShaderManager::Instance()->AddLight(mPointLight);
	//ZShadeSandboxLighting::DeferredShaderManager::Instance()->AddLight(mCapsuleLight);
	ZShadeSandboxLighting::DeferredShaderManager::Instance()->AddLight(mSpotLight1);
	
	//Create the refraction render to texture object
	mRefractionTexture = new RenderTarget2D(m_D3DSystem);
	mRefractionTexture->Initialize(mEngineOptions->m_screenWidth, mEngineOptions->m_screenHeight, DXGI_FORMAT_R32G32B32A32_FLOAT);

	//Create the reflection render to texture object
	mReflectionTexture = new RenderTarget2D(m_D3DSystem);
	mReflectionTexture->Initialize(mEngineOptions->m_screenWidth, mEngineOptions->m_screenHeight, DXGI_FORMAT_R32G32B32A32_FLOAT);
	
	//Create the shadow map render to texture object
	//mShadowTexture = new RenderTarget2D(m_D3DSystem);
	//mShadowTexture->Initialize(fShadowMapWidth, fShadowMapWidth, DXGI_FORMAT_R32G32B32A32_FLOAT);//DXGI_FORMAT_R24G8_TYPELESS
	
	mShadowTexture = new ShadowMapTarget(m_D3DSystem);
	mShadowTexture->Initialize(fShadowMapWidth, fShadowMapWidth);

	mSceneTarget = new RenderTarget2D(m_D3DSystem);
	mSceneTarget->Initialize(mEngineOptions->m_screenWidth, mEngineOptions->m_screenHeight, DXGI_FORMAT_R32G32B32A32_FLOAT);

	GDI::Init();
	
	int text_size = 16;
	mFPSText.SetD3D(m_D3DSystem);
	mFPSText.Initialize();
	mFPSText.TextName() = BetterString("");
	//mFPSText.CreateShadow();
	mFPSText.ChangeFont(L"Times New Roman", text_size, FontStyle::BoldItalic, true);
	mFPSTextMatrix = XMMatrixTranslation(2, 2, 0);
	mFPSTextMatrix = XMMatrixMultiply(mFPSTextMatrix, XMMatrixScaling(1, 1, 1));
	mFPSText.TextTransform() = mFPSTextMatrix;
	mFPSText.TextColor() = XMFLOAT4(1, 0, 0, 1);
	
	mTriangleCountText.SetD3D(m_D3DSystem);
	mTriangleCountText.Initialize();
	mTriangleCountText.TextName() = BetterString("");
	//mTriangleCountText.CreateShadow();
	mTriangleCountText.ChangeFont(L"Times New Roman", text_size, FontStyle::BoldItalic, true);
	mTriangleCountTextMatrix = XMMatrixTranslation(2, text_size + 2, 0);
	mTriangleCountTextMatrix = XMMatrixMultiply(mTriangleCountTextMatrix, XMMatrixScaling(1, 1, 1));
	mTriangleCountText.TextTransform() = mTriangleCountTextMatrix;
	mTriangleCountText.TextColor() = XMFLOAT4(1, 0, 0, 1);
	
	mPostProcessManager = new PostProcessManager(m_D3DSystem);
	mPostProcessManager->AddPostProcess(new HDR(m_D3DSystem));
	
	mSky = new Sky(m_D3DSystem, m_GameDirectory3D->m_textures_path, "sky_cube.dds", 8000.0f);
	// Loads defaults
	SkyPlaneParameters spp;
	spp.g_skyPlaneWidth = 40000.0f;
	spp.cloudTexturePath = m_GameDirectory3D->m_textures_path + "\\";
	mSkyPlane = new SkyPlane(m_D3DSystem, spp);

	return true;
}
//===============================================================================================================================
void Environment3D::Shutdown()
{
	m_D3DSystem->Shutdown();
}
//===============================================================================================================================
bool Environment3D::Quickwire()
{
	// LeftAlt or Space
	if (keyboard->IsKeyDown(Keyboard::Key::Space))
		return true;
	else
		return false;
}
//==============================================================================================================================
void Environment3D::UpdateMaster()
{
	keyboard = Keyboard::GetKeyboard();
	mouse = Mouse::GetMouse();
	
	HWND wnd = GetForegroundWindow();
	//if (wnd != m_BaseWindow->GetHwnd() && wnd != m_RenderWindow->GetHwnd()) return;
	
	FPSCounter::GetInstance()->Update();
	BetterString str = "FPS: ";
	str += ZShadeSandboxGlobal::Convert::ConvertIntToString(FPSCounter::GetInstance()->FPS());
	mFPSText.TextName() = str;
	
	m_Timer.Frame();
	
	// What is the difference between these
	fFrameTime = m_Timer.DeltaSecondsF();
	float time = m_Timer.GetTime();
	
	// Generate the view and reflection view matrix based on the camera's position.
	//XMFLOAT4 frustumPlanes[6];
	m_CameraSystem->ExtractFrustumPlanes(mFrustumPlanes);
	m_CameraSystem->Update();
	
	if (!bCameraStill)
	{
		if( keyboard->IsKeyDown(Keyboard::Key::W) )
		{
			//if( GetAsyncKeyState(VK_LSHIFT) & 0x8000 )
			if (keyboard->IsKeyDown(Keyboard::Key::LeftShift) )
				m_CameraSystem->Walk(fCamSpeedFast * time);
			else
				m_CameraSystem->Walk(fCamSpeedSlow * time);
		}

		if( keyboard->IsKeyDown(Keyboard::Key::S) )
		{
			if (keyboard->IsKeyDown(Keyboard::Key::LeftShift) )
				m_CameraSystem->Walk(-fCamSpeedFast * time);
			else
				m_CameraSystem->Walk(-fCamSpeedSlow * time);
		}

		if( keyboard->IsKeyDown(Keyboard::Key::A) )
		{
			if (keyboard->IsKeyDown(Keyboard::Key::LeftShift) )
				m_CameraSystem->Strafe(-fCamSpeedFast * time);
			else
				m_CameraSystem->Strafe(-fCamSpeedSlow * time);
		}

		if( keyboard->IsKeyDown(Keyboard::Key::D) )
		{
			if (keyboard->IsKeyDown(Keyboard::Key::LeftShift) )
				m_CameraSystem->Strafe(fCamSpeedFast * time);
			else
				m_CameraSystem->Strafe(fCamSpeedSlow * time);
		}
	}

	// This is not used to toggle forward lighting system
	if (keyboard->RisingEdge(Keyboard::Key::F1) )
	{
		//while(1) { Sleep(300); break; }
		bEnableLighting = !bEnableLighting;
	}

	//if( GetAsyncKeyState(VK_F2) & 0x8000 )
	if (keyboard->RisingEdge(Keyboard::Key::F2) )
	{
		//while(1) { Sleep(300); break; }
		bWireframeMode = !bWireframeMode;
	}

	if (keyboard->RisingEdge(Keyboard::Key::F3))
	{
		bEnablePostProcessing = !bEnablePostProcessing;
	}
	
	if (keyboard->RisingEdge(Keyboard::Key::F4))
	{
		bEnableDeferredShading = !bEnableDeferredShading;
	}

	if (keyboard->RisingEdge(Keyboard::Key::F5))
	{
		while(1) { Sleep(30); break; }
		bEnableTessellation = !bEnableTessellation;
	}

	if (keyboard->RisingEdge(Keyboard::Key::F))
	{
		// TODO: Randomly spawn a mesh object
		
		ZShadeSandboxMesh::MeshParameters mp;
		mp.useCustomShader = false;
		mp.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
		mp.rotationAxisX = false;
		mp.rotationAxisY = true;
		mp.rotationAxisZ = false;
		mp.pos = m_CameraSystem->Position();
		mp.rot = XMFLOAT3(0, 1, 0);
		mp.scale = XMFLOAT3(30, 30, 30);
		mp.material = MaterialManager::Instance()->GetMaterial("Wall");
		mp.in2D = false;
		mp.shader = 0;
		m_SpawnedMeshContainer.push_back(new ZShadeSandboxMesh::CubeMesh(m_D3DSystem, mp));
	}

	//Fullscreen mode
	/*if ( keyboard->RisingEdge(Keyboard::Key::F4) )
	{
		//while(1) { Sleep(5); break; }
		fullScreen = !fullScreen;
		if(fullScreen != m_D3DSystem->GetEngineOptions()->fullscreen)
		{
			BeforeReset();

			//The concrete class that uses this will set the device to fullscreen
			m_D3DSystem->SetFullscreen(fullScreen);

			AfterReset();
		}
	}*/

	// Update camera speed
	if (keyboard->RisingEdge(Keyboard::Key::F6))
	{
		while (1) { Sleep(30); break; }
		fCamSpeedFast -= 0.01f;
		fCamSpeedSlow -= 0.01f;
		if (fCamSpeedFast <= 0)
			fCamSpeedFast += 0.01f;
		if (fCamSpeedSlow <= 0)
			fCamSpeedSlow += 0.01f;
	}
	if (keyboard->RisingEdge(Keyboard::Key::F7))
	{
		while (1) { Sleep(30); break; }
		fCamSpeedFast += 0.01f;
		fCamSpeedSlow += 0.01f;
	}
	
	if (keyboard->IsKeyDown(Keyboard::Key::F8))
	{
		while (1) { Sleep(30); break; }
		fTessFactor -= 1.0f;
		if (fTessFactor <= 0)
			fTessFactor += 1.0f;
	}

	if (keyboard->IsKeyDown(Keyboard::Key::F9))
	{
		while (1) { Sleep(30); break; }
		fTessFactor += 1.0f;
		if (fTessFactor >= 64.0f)
			fTessFactor -= 1.0f;
	}
	
	if ((GetAsyncKeyState('1') & 0x8000) && bEnableDeferredShading)
	{
		while (1) { Sleep(200); break; }
		bToggleGBufferDebugging = !bToggleGBufferDebugging;
	}
	if ((GetAsyncKeyState('2') & 0x8000) && bEnableDeferredShading)
	{
		while (1) { Sleep(200); break; }
		bToggleGBufferDebuggingColors = true;

		bToggleGBufferDebuggingNormals = !bToggleGBufferDebuggingColors;
		bToggleGBufferDebuggingDepth = bToggleGBufferDebuggingNormals;
	}
	if ((GetAsyncKeyState('3') & 0x8000) && bEnableDeferredShading)
	{
		while (1) { Sleep(200); break; }
		bToggleGBufferDebuggingNormals = true;

		bToggleGBufferDebuggingColors = !bToggleGBufferDebuggingNormals;
		bToggleGBufferDebuggingDepth = bToggleGBufferDebuggingColors;
	}
	if ((GetAsyncKeyState('4') & 0x8000) && bEnableDeferredShading)
	{
		while (1) { Sleep(200); break; }
		bToggleGBufferDebuggingDepth = true;

		bToggleGBufferDebuggingColors = !bToggleGBufferDebuggingDepth;
		bToggleGBufferDebuggingNormals = bToggleGBufferDebuggingColors;
	}
	
	if (bEnableDeferredShading)
	{
		ZShadeSandboxLighting::DeferredShaderManager::Instance()->ToggleDebugging() = bToggleGBufferDebugging;
		ZShadeSandboxLighting::DeferredShaderManager::Instance()->ToggleDebuggingColors() = bToggleGBufferDebuggingColors;
		ZShadeSandboxLighting::DeferredShaderManager::Instance()->ToggleDebuggingNormals() = bToggleGBufferDebuggingNormals;
		ZShadeSandboxLighting::DeferredShaderManager::Instance()->ToggleDebuggingDepth() = bToggleGBufferDebuggingDepth;
	}
	
	if (GetAsyncKeyState('5') & 0x8000)
	{
		while (1) { Sleep(300); break; }
		bToggleSky = !bToggleSky;
	}
	if (GetAsyncKeyState('6') & 0x8000)
	{
		while (1) { Sleep(300); break; }
		bToggleSkyPlane = !bToggleSkyPlane;
	}

	if (bToggleSky)
	{
		mSky->SetWireframe(bWireframeMode);

		if (bToggleSkyPlane)
		{
			mSkyPlane->SetWireframe(bWireframeMode);
			mSkyPlane->Update(fFrameTime);
		}
	}

	XMFLOAT3 eye = m_CameraSystem->Position();
	XMFLOAT3 Dir1LightPos = mDirLight1->Position();
	XMFLOAT3 Dir2LightPos = mDirLight2->Position();
	XMFLOAT3 Dir3LightPos = mDirLight3->Position();
	XMFLOAT3 PointLightPos = mPointLight->Position();
	XMFLOAT3 CapsuleLightPos = mCapsuleLight->Position();
	XMFLOAT3 SpotLightPos = mSpotLight1->Position();

	if (keyboard->IsKeyDown(Keyboard::Key::Up))
	{
		mDirLight1->Direction().x += 0.01f;
		mDirLight1->UpdateMeshPosition(XMFLOAT3(Dir1LightPos.x + 0.01f, Dir1LightPos.y, Dir1LightPos.z));

		mDirLight2->Direction().x += 0.01f;
		mDirLight2->UpdateMeshPosition(XMFLOAT3(Dir2LightPos.x + 0.01f, Dir2LightPos.y, Dir2LightPos.z));

		mDirLight3->Direction().x += 0.01f;
		mDirLight3->UpdateMeshPosition(XMFLOAT3(Dir3LightPos.x + 0.01f, Dir3LightPos.y, Dir3LightPos.z));

		if (mPointLight != NULL) mPointLight->UpdateMeshPosition(XMFLOAT3(PointLightPos.x + 0.01f, PointLightPos.y, PointLightPos.z));
		if (mCapsuleLight != NULL) mCapsuleLight->UpdateMeshPosition(XMFLOAT3(CapsuleLightPos.x + 0.01f, CapsuleLightPos.y, CapsuleLightPos.z));
		if (mSpotLight1 != NULL) mSpotLight1->UpdateMeshPosition(XMFLOAT3(SpotLightPos.x + 0.01f, SpotLightPos.y, SpotLightPos.z));
	}
	if (keyboard->IsKeyDown(Keyboard::Key::Down))
	{
		mDirLight1->Direction().x -= 0.01f;
		mDirLight1->UpdateMeshPosition(XMFLOAT3(Dir1LightPos.x - 0.01f, Dir1LightPos.y, Dir1LightPos.z));

		mDirLight2->Direction().x -= 0.01f;
		mDirLight2->UpdateMeshPosition(XMFLOAT3(Dir2LightPos.x - 0.01f, Dir2LightPos.y, Dir2LightPos.z));

		mDirLight3->Direction().x -= 0.01f;
		mDirLight3->UpdateMeshPosition(XMFLOAT3(Dir3LightPos.x - 0.01f, Dir3LightPos.y, Dir3LightPos.z));

		if (mPointLight != NULL) mPointLight->UpdateMeshPosition(XMFLOAT3(PointLightPos.x - 0.01f, PointLightPos.y, PointLightPos.z));
		if (mCapsuleLight != NULL) mCapsuleLight->UpdateMeshPosition(XMFLOAT3(CapsuleLightPos.x - 0.01f, CapsuleLightPos.y, CapsuleLightPos.z));
		if (mSpotLight1 != NULL) mSpotLight1->UpdateMeshPosition(XMFLOAT3(SpotLightPos.x - 0.01f, SpotLightPos.y, SpotLightPos.z));
	}
	if (keyboard->IsKeyDown(Keyboard::Key::Left))
	{
		mDirLight1->Direction().z += 0.01f;
		mDirLight1->UpdateMeshPosition(XMFLOAT3(Dir1LightPos.x, Dir1LightPos.y, Dir1LightPos.z + 0.01f));

		mDirLight2->Direction().z += 0.01f;
		mDirLight2->UpdateMeshPosition(XMFLOAT3(Dir2LightPos.x, Dir2LightPos.y, Dir2LightPos.z + 0.01f));

		mDirLight3->Direction().z += 0.01f;
		mDirLight3->UpdateMeshPosition(XMFLOAT3(Dir3LightPos.x, Dir3LightPos.y, Dir3LightPos.z + 0.01f));

		if (mPointLight != NULL) mPointLight->UpdateMeshPosition(XMFLOAT3(PointLightPos.x, PointLightPos.y, PointLightPos.z + 0.01f));
		if (mCapsuleLight != NULL) mCapsuleLight->UpdateMeshPosition(XMFLOAT3(CapsuleLightPos.x, CapsuleLightPos.y, CapsuleLightPos.z + 0.01f));
		if (mSpotLight1 != NULL) mSpotLight1->UpdateMeshPosition(XMFLOAT3(SpotLightPos.x, SpotLightPos.y, SpotLightPos.z + 0.01f));
	}
	if (keyboard->IsKeyDown(Keyboard::Key::Right))
	{
		mDirLight1->Direction().x -= 0.01f;
		mDirLight1->UpdateMeshPosition(XMFLOAT3(Dir1LightPos.x, Dir1LightPos.y, Dir1LightPos.z - 0.01f));

		mDirLight2->Direction().x -= 0.01f;
		mDirLight2->UpdateMeshPosition(XMFLOAT3(Dir2LightPos.x, Dir2LightPos.y, Dir2LightPos.z - 0.01f));

		mDirLight3->Direction().x -= 0.01f;
		mDirLight3->UpdateMeshPosition(XMFLOAT3(Dir3LightPos.x, Dir3LightPos.y, Dir3LightPos.z - 0.01f));

		if (mPointLight != NULL) mPointLight->UpdateMeshPosition(XMFLOAT3(PointLightPos.x, PointLightPos.y, PointLightPos.z - 0.01f));
		if (mCapsuleLight != NULL) mCapsuleLight->UpdateMeshPosition(XMFLOAT3(CapsuleLightPos.x, CapsuleLightPos.y, CapsuleLightPos.z - 0.01f));
		if (mSpotLight1 != NULL) mSpotLight1->UpdateMeshPosition(XMFLOAT3(SpotLightPos.x, SpotLightPos.y, SpotLightPos.z - 0.01f));
	}
	if (keyboard->IsKeyDown(Keyboard::Key::E))
	{
		mDirLight1->Direction().y += 0.01f;
		mDirLight1->UpdateMeshPosition(XMFLOAT3(Dir1LightPos.x, Dir1LightPos.y + 0.01f, Dir1LightPos.z));

		mDirLight2->Direction().y += 0.01f;
		mDirLight2->UpdateMeshPosition(XMFLOAT3(Dir2LightPos.x, Dir2LightPos.y + 0.01f, Dir2LightPos.z));

		mDirLight3->Direction().y += 0.01f;
		mDirLight3->UpdateMeshPosition(XMFLOAT3(Dir3LightPos.x, Dir3LightPos.y + 0.01f, Dir3LightPos.z));

		if (mPointLight != NULL) mPointLight->UpdateMeshPosition(XMFLOAT3(PointLightPos.x, PointLightPos.y + 0.01f, PointLightPos.z));
		if (mCapsuleLight != NULL) mCapsuleLight->UpdateMeshPosition(XMFLOAT3(CapsuleLightPos.x, CapsuleLightPos.y + 0.01f, CapsuleLightPos.z));
		if (mSpotLight1 != NULL) mSpotLight1->UpdateMeshPosition(XMFLOAT3(SpotLightPos.x, SpotLightPos.y + 0.01f, SpotLightPos.z));
	}
	if (keyboard->IsKeyDown(Keyboard::Key::R))
	{
		mDirLight1->Direction().y -= 0.01f;
		mDirLight1->UpdateMeshPosition(XMFLOAT3(Dir1LightPos.x, Dir1LightPos.y - 0.01f, Dir1LightPos.z));

		mDirLight2->Direction().y -= 0.01f;
		mDirLight2->UpdateMeshPosition(XMFLOAT3(Dir2LightPos.x, Dir2LightPos.y - 0.01f, Dir2LightPos.z));

		mDirLight3->Direction().y -= 0.01f;
		mDirLight3->UpdateMeshPosition(XMFLOAT3(Dir3LightPos.x, Dir3LightPos.y - 0.01f, Dir3LightPos.z));

		if (mPointLight != NULL) mPointLight->UpdateMeshPosition(XMFLOAT3(PointLightPos.x, PointLightPos.y - 0.01f, PointLightPos.z));
		if (mCapsuleLight != NULL) mCapsuleLight->UpdateMeshPosition(XMFLOAT3(CapsuleLightPos.x, CapsuleLightPos.y - 0.01f, CapsuleLightPos.z));
		if (mSpotLight1 != NULL) mSpotLight1->UpdateMeshPosition(XMFLOAT3(SpotLightPos.x, SpotLightPos.y - 0.01f, SpotLightPos.z));
	}

	// Set the capsule light to the camera's position so things can be seen
	//mCapsuleLight->UpdateMeshPosition(XMFLOAT3(eye.x, eye.y, eye.z));

	//static float lightAngle = 270.0f;
	//float radians;
	//static float lightPosX = 9.0f;

	//// Update the position of the light each frame.
	//lightPosX -= 0.003f * fFrameTime;

	//// Update the angle of the light each frame.
	//lightAngle -= 0.03f * fFrameTime;
	//if (lightAngle < 90.0f)
	//{
	//	lightAngle = 270.0f;

	//	// Reset the light position also.
	//	lightPosX = 9.0f;
	//}
	//radians = lightAngle * 0.0174532925f;

	//// Update the direction of the light.
	//mDirLight1->Direction() = XMFLOAT3(sinf(radians), cosf(radians), 0.0f);

	//// Set the position and lookat for the light.
	//mDirLight1->Position() = XMFLOAT3(lightPosX, 8.0f, -0.1f);
	//mDirLight1->Perspective()->LookAt() = XMFLOAT3(-lightPosX, 0.0f, 0.0f);

	/*XMFLOAT3 dir = mDirLight1->Direction();
	static float lightDirX = dir.x;
	lightDirX += 0.0005f;
	if (lightDirX > (0.57735f + 5.0f))
	{
		lightDirX = 0.57735f;
	}
	mDirLight1->Direction() = XMFLOAT3(lightDirX, dir.y, dir.z);*/

	// Update the view projection matrix from the perspective of the light
	//mDirLight1->Update();
	//mDirLight2->Update();
	//mDirLight1->UpdateLVP();

	//mDirLight1->Perspective()->SetSceneBounds(XMFLOAT3(0, 0, 0), 2048);
	//mDirLight2->Perspective()->SetSceneBounds(XMFLOAT3(0, 0, 0), 2048);
	//mDirLight3->Perspective()->SetSceneBounds(XMFLOAT3(0, 0, 0), 2048);

	mPointLight->Perspective()->SetSceneBounds(XMFLOAT3(0, 0, 0), 2048);
	mCapsuleLight->Perspective()->SetSceneBounds(XMFLOAT3(0, 0, 0), 2048);

	// Update the view projection matrix from the perspective of the light
	//mDirLight1->UpdateLVP();
	//mDirLight2->UpdateLVP();
	mDirLight1->Update();
	mDirLight2->Update();
	mDirLight3->Update();
	mPointLight->Update();
	mCapsuleLight->Update();

	// Update other scene components
	Update();
}
//===============================================================================================================================
void Environment3D::RenderMaster()
{
	// Set cull mode or wireframe mode for the scene
	if (bWireframeMode)
	{
		m_D3DSystem->TurnOnWireframe();
	}
	else
	{
		m_D3DSystem->TurnOnCulling();
	}
	
	if (bEnableReflections)
	{
		RenderReflectionToTexture();
	}
	
	if (bEnableRefractions)
	{
		RenderRefractionToTexture();
	}
	
	if (bEnableShadows)
	{
		RenderShadowMapToTexture();
	}
	
	if (bEnableDeferredShading && !bWireframeMode && !Quickwire())
	{
		m_D3DSystem->GBufferBegin();
		{
			RenderDeferred();
		}
		m_D3DSystem->GBufferEnd();

		ZShadeSandboxLighting::DeferredShaderManager::Instance()->Render(m_CameraSystem, bWireframeMode);
		
		RenderSky(false, false);

		if (bEnablePostProcessing)
		{
			mPostProcessManager->Render(m_D3DSystem->GBufferColorTarget()->SRView, m_D3DSystem->GetBackbufferRenderTarget()->RTView);
		}
	}
	else
	{
		// Normal 3D Rendering for the scene Happens Here
		Render();

		RenderSky(false, false);

		if (bEnablePostProcessing)
		{
			mPostProcessManager->Render(m_D3DSystem->GetBackbufferRenderTarget()->SRView, m_D3DSystem->GetBackbufferRenderTarget()->RTView);
		}
	}
	
	//
	// 2D Rendering
	//
	
	m_D3DSystem->TurnOffZBuffer();
	{
		mFPSTextMatrix = XMMatrixTranslation(2, 2, 0);
		mFPSTextMatrix = XMMatrixMultiply(mFPSTextMatrix, XMMatrixScaling(1, 1, 1));
		mFPSText.Render11();
		
		mTriangleCountTextMatrix = XMMatrixTranslation(2, 4, 0);
		mTriangleCountTextMatrix = XMMatrixMultiply(mTriangleCountTextMatrix, XMMatrixScaling(1, 1, 1));
		mTriangleCountText.Render11();
		
		Render2D();
	}
	m_D3DSystem->TurnOnZBuffer();
}
//===============================================================================================================================
void Environment3D::RenderSky(bool reflections, bool deferred)
{
	if (!bToggleSky)
		return;

	if (Quickwire() || bWireframeMode)
		return;

	//
	//Render the sky
	//

	//m_D3DSystem->TurnOffZBuffer();
	m_D3DSystem->TurnOffCulling();
	m_D3DSystem->TurnOnAdditiveBlending();

	if (bWireframeMode)
	{
		m_D3DSystem->TurnOnWireframe();
	}

	//Quickwire stuff that default environment supports
	if (Quickwire())
	{
		mSky->SetWireframe(true);
		mSkyPlane->SetWireframe(true);
		m_D3DSystem->TurnOnWireframe();
	}

	if (reflections)
	{
		mSky->RenderWithReflection(m_D3DSystem, m_CameraSystem, fSeaLevel, deferred);
		
		if (bToggleSkyPlane)
		{
			mSkyPlane->RenderWithReflection(m_D3DSystem, m_CameraSystem, fSeaLevel);
		}
	}
	else// Normal Sky
	{
		mSky->Render(m_D3DSystem, m_CameraSystem, deferred);
		
		if (bToggleSkyPlane)
		{
			mSkyPlane->Render(m_D3DSystem, m_CameraSystem, fSeaLevel);
		}
	}

	m_D3DSystem->TurnOffAdditiveBlending();
	m_D3DSystem->TurnOnCulling();
	//m_D3DSystem->TurnOnZBuffer();
}
//===============================================================================================================================
void Environment3D::OnMouseDown(WPARAM btnState, int x, int y)
{
	mLastMousePos.x = x;
	mLastMousePos.y = y;

	if ((btnState & MK_LBUTTON) != 0)
	{
		bLeftMouseDown = true;
	}
	
	//if (mEngineOptions->m_inEditor)
	//	;
	//else
	//	SetCapture(m_RenderWindow->GetHwnd());
}
//===============================================================================================================================
void Environment3D::OnMouseUp(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		bLeftMouseDown = false;
	}

	ReleaseCapture();
}
//===============================================================================================================================
void Environment3D::OnMouseMove(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_RBUTTON) != 0)
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = XMConvertToRadians(0.25f*static_cast<float>(x - mLastMousePos.x));
		float dy = XMConvertToRadians(0.25f*static_cast<float>(y - mLastMousePos.y));
		
		if (!bCameraStill)
			m_CameraSystem->UpdateRotation(dx, dy);
	}

	mLastMousePos.x = x;
	mLastMousePos.y = y;
}
//===============================================================================================================================
void Environment3D::RenderRefractionToTexture()
{
	//Use the camera to render the reflection and craete a reflection view matrix
	//Get the camera reflection view matrix instead of the normal view matrix
	m_CameraSystem->UpdateReflection(fSeaLevel);

	XMVECTOR planeNormalDirection = XMVectorSet(0.0f, -1.0f, 0.0f, fSeaLevel);
	planeNormalDirection = XMVector3Normalize(planeNormalDirection);

	XMFLOAT4 planeNormalDirectionF;
	XMStoreFloat4(&planeNormalDirectionF, planeNormalDirection);
	XMFLOAT4 planeCoefficients = XMFLOAT4(planeNormalDirectionF.x, planeNormalDirectionF.y, planeNormalDirectionF.z, planeNormalDirectionF.w);
	XMMATRIX camMatrix = XMMatrixMultiply(m_CameraSystem->View(), m_CameraSystem->Proj());
	XMVECTOR dummy;
	XMMATRIX invCamMatrix = XMMatrixInverse(&dummy, camMatrix);
	invCamMatrix = XMMatrixTranspose(invCamMatrix);
	XMVECTOR planeCoefficientsV = XMLoadFloat4(&planeCoefficients);
	XMVECTOR planeCoefficientsTrans = XMVector4Transform(planeCoefficientsV, invCamMatrix);
	XMFLOAT4 refractionClipPlane;
	XMStoreFloat4(&refractionClipPlane, planeCoefficientsTrans);

	//Use the camera to render the reflection and craete a reflection view matrix
	//Get the camera reflection view matrix instead of the normal view matrix
	//m_CameraSystem->UpdateReflection(m_SeaLevel);

	//Set the render target to be the refraction render to texture
	mRefractionTexture->SetRenderTarget();

	//Clear the refraction render to texture
	mRefractionTexture->ClearRenderTarget(0.0f, 0.0f, 0.0f, 1.0f);

	//Rendering happens here
	{
		RenderRefraction(refractionClipPlane);
	}

	//Reset the render target back to the original back buffer and not the render to texture anymore
	//if (m_PostProcessEnable)
	//{
	//	m_D3DSystem->SetColorRenderTarget();
	//}
	//else
	m_D3DSystem->SetBackBufferRenderTarget();

	//Reset the viewport back to the original
	m_D3DSystem->ResetViewport();
}
//===============================================================================================================================
void Environment3D::RenderReflectionToTexture()
{
	//Use the camera to render the reflection and craete a reflection view matrix
	//Get the camera reflection view matrix instead of the normal view matrix
	m_CameraSystem->UpdateReflection(fSeaLevel);

	XMVECTOR planeNormalDirection = XMVectorSet(0.0f, 1.0f, 0.0f, -fSeaLevel);
	planeNormalDirection = XMVector3Normalize(planeNormalDirection);

	XMFLOAT4 planeNormalDirectionF;
	XMStoreFloat4(&planeNormalDirectionF, planeNormalDirection);
	XMFLOAT4 planeCoefficients = XMFLOAT4(planeNormalDirectionF.x, planeNormalDirectionF.y, planeNormalDirectionF.z, planeNormalDirectionF.w + 1.0);
	XMMATRIX camMatrix = XMMatrixMultiply(m_CameraSystem->ReflectionView(), m_CameraSystem->Proj());
	XMVECTOR dummy;
	XMMATRIX invCamMatrix = XMMatrixInverse(&dummy, camMatrix);
	invCamMatrix = XMMatrixTranspose(invCamMatrix);
	XMVECTOR planeCoefficientsV = XMLoadFloat4(&planeCoefficients);
	XMVECTOR planeCoefficientsTrans = XMVector4Transform(planeCoefficientsV, invCamMatrix);
	XMFLOAT4 reflectionClipPlane;
	XMStoreFloat4(&reflectionClipPlane, planeCoefficientsTrans);

	//Set the render target to be the reflection render to texture
	mReflectionTexture->SetRenderTarget();

	//Clear the reflection render to texture
	mReflectionTexture->ClearRenderTarget(0.0f, 0.0f, 0.0f, 1.0f);

	//Rendering happens here
	{
		RenderReflection(reflectionClipPlane);

		RenderSky(true, false);
	}

	//Reset the render target to the original back buffer and not the render to texture anymore
	//if (m_PostProcessEnable)
	//{
	//	m_D3DSystem->SetColorRenderTarget();
	//}
	//else
	m_D3DSystem->SetBackBufferRenderTarget();

	//Reset the viewport back to the original
	m_D3DSystem->ResetViewport();
}
//===============================================================================================================================
void Environment3D::RenderShadowMapToTexture()
{
	mShadowTexture->SetRenderTarget();
	//Rendering happens here
	{
		RenderShadowMap();
	}
	mShadowTexture->ResetView();
}
//===============================================================================================================================
int Environment3D::Run()
{
	UpdateMaster();

	if (m_D3DSystem->BeginScene(0.1f, 0.2f, 0.4f, 0.0f))
	{
		//Rendering Happens Here

		RenderMaster();

		m_D3DSystem->EndScene();
	}
	
	return 1;
}
//===============================================================================================================================