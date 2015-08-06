//===============================================================================================================================
// Environment3D
//
//===============================================================================================================================
// History
//
// -Created on 6/14/2014 by Dustin Watson
//===============================================================================================================================
#ifndef __ENVIRONMENT3D_H
#define __ENVIRONMENT3D_H
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//
#pragma region "Includes"
#include <memory>
#include <Windows.h>
#include <string>
#include <windowsx.h>
#include <CommCtrl.h>
#include "Window.h"
#include "D3D.h"
#include "BetterString.h"
#include "Timer.h"
#include "FPSCounter.h"
#include "Camera.h"
#include "ZMath.h"
#include "RenderTarget2D.h"
#include "input.h"
#include "ShapeContact.h"
//#include "EnvironmentBase.h"
#include "CubeMesh.h"
#include "SphereMesh.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include "PointLight.h"
#include "LightManager.h"
#include "Ray.h"
#include "ShadowMapTarget.h"
#include "Text.h"
#include "PostProcessManager.h"
#include "Sky.h"
#include "SkyPlane.h"
#include "DeferredShaderManager.h"
#pragma endregion

//===============================================================================================================================
//===============================================================================================================================

//
// 3D Environment
//
class Environment3D
{
public:
	
	Environment3D(EngineOptions* eo);
	~Environment3D();

public:
	
	bool Init();
	
	XMMATRIX WorldXM() { return XMLoadFloat4x4(&mWorld); }
	
	bool Quickwire();
	void SetFullscreen(bool fullscreen) { this->bFullScreen = bFullScreen; }
	int Run();
	
	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);
	
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void RenderDeferred() = 0;
	virtual void Shutdown();
	
	void RenderSky(bool reflections, bool deferred);

	// Override this for any 2D rendering in the scene
	virtual void Render2D() {}
	
	//Override these for reflection and refraction functionality
	virtual void RenderReflection(XMFLOAT4 clipplane) {}
	virtual void RenderRefraction(XMFLOAT4 clipplane) {}
	
	// Override this and add all scene items to get a shadow map
	virtual void RenderShadowMap() {}
	
	void SetGameDirectory3D(GameDirectory3D* gd) { m_GameDirectory3D = gd; }
	GameDirectory3D* GetGD3D() { return m_GameDirectory3D; }
	EngineOptions* GetEngineOptions()  { return mEngineOptions; }
	D3D* GetD3D() { return m_D3DSystem; }
	Camera* GetCamera() { return m_CameraSystem; }

private:
	
	// Base functions that render reflections and refractions
	void RenderRefractionToTexture();
	void RenderReflectionToTexture();
	
	// Base stuff for shadow map
	void RenderShadowMapToTexture();
	
	void RenderMaster();
	void UpdateMaster();
	
protected:
	
	Keyboard* keyboard;
	Mouse* mouse;
	D3D* m_D3DSystem;
	Camera* m_CameraSystem;
	ZShadeSandboxGlobal::Timer m_Timer;
	GameDirectory3D* m_GameDirectory3D;
	EngineOptions* mEngineOptions;
	//HUDSystem* m_HUDSystem;
	//MenuSystem* m_MenuSystem;
	bool m_GameInitialized;

	POINT mLastMousePos;
	
	XMFLOAT4X4 mWorld;
	//XMFLOAT4 mFrustumPlanes[6];
	
	ZShadeSandboxGraphics::Text mFPSText;
	XMMATRIX mFPSTextMatrix;
	ZShadeSandboxGraphics::Text mTriangleCountText;
	XMMATRIX mTriangleCountTextMatrix;
	
	Sky* mSky;
	SkyPlane* mSkyPlane;

	RenderTarget2D* mRefractionTexture;
	RenderTarget2D* mReflectionTexture;
	//RenderTarget2D* mShadowTexture;
	RenderTarget2D* mSceneTarget;
	
	ShadowMapTarget* mShadowTexture;
	
	ZShadeSandboxLighting::DirectionalLight* mDirLight1;
	ZShadeSandboxLighting::DirectionalLight* mDirLight2;
	ZShadeSandboxLighting::DirectionalLight* mDirLight3;
	ZShadeSandboxLighting::SpotLight* mSpotLight1;
	ZShadeSandboxLighting::PointLight* mPointLight;
	ZShadeSandboxLighting::CapsuleLight* mCapsuleLight;
	
	XMFLOAT3 mAmbientUp;
	XMFLOAT3 mAmbientDown;
	ZShadeSandboxLighting::SunLightBuffer* mSunLightBuffer;
	
	// For mouse picking a 3D object
	ZShadeSandboxMesh::SphereMesh* mPickingSphere;
	ZShadeSandboxMath::Ray* mPickingRay;
	
	XMFLOAT4 mFrustumPlanes[6];

	int iTriangleCount;
	
	float fFrameTime;
	float fCamSpeedFast;
	float fCamSpeedSlow;
	float fShadowMapWidth;
	float fShadowMapNear;
	float fShadowMapFar;
	float fSeaLevel;
	float fTessFactor;
	
	bool bUseEngineTextures;
	bool bLeftMouseDown;
	bool bWireframeMode;
	bool bEnableLighting;
	bool bCameraStill;
	bool bFullScreen;
	bool bEnableReflections;
	bool bEnableRefractions;
	bool bEnableShadows;
	bool bEnableTessellation;
	bool bEnableCulling;
	bool bSpawnCube;
	bool bEnablePostProcessing;
	bool bEnableDeferredShading;
	bool bToggleSkyPlane;
	bool bToggleSky;
	bool bToggleGBufferDebugging;
	bool bToggleGBufferDebuggingColors;
	bool bToggleGBufferDebuggingNormals;
	bool bToggleGBufferDebuggingDepth;
	
	PostProcessManager* mPostProcessManager;
	
	// Can spawn a new basic mesh at the press of a button
	vector<ZShadeSandboxMesh::CustomMesh*> m_SpawnedMeshContainer;
};

//===============================================================================================================================
//===============================================================================================================================
#endif//__ENVIRONMENT3D_H