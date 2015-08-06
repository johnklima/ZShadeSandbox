//===============================================================================================================================
// OutdoorEnvironment
//
//===============================================================================================================================
// History
//
// -Created on 6/15/2014 by Dustin Watson
//===============================================================================================================================
#ifndef __OUTDOORENVIRONMENT_H
#define __OUTDOORENVIRONMENT_H
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//
#pragma region "Includes"
#include "Environment3D.h"
#include "QuadTree.h"
#include "QuadTreeRenderer.h"
#include "OceanSimulator.h"
#include "OceanParameters.h"
#include "OceanSurface.h"
#include "Water.h"
#include "Sky.h"
#include "SkyPlane.h"
#include "InfinitePlane.h"
#include "Volume.h"
#include "OBJMesh.h"
#include "SphereMesh.h"
#include "CylindricalMesh.h"
#include "GeosphereMesh.h"
#include "CircleMesh.h"
#include "Sun.h"
#include "SunRenderer.h"
#pragma endregion

//===============================================================================================================================
//===============================================================================================================================

//
// Outdoor Environment Defines what is needed for outdoors
//
class OutdoorEnvironment : public Environment3D
{
public:
	OutdoorEnvironment(EngineOptions* eo);
	~OutdoorEnvironment();
	
	virtual bool Init();
	bool InitializeWater();
	bool InitializeOcean();
	bool InitializeTerrain();
	
	void Shutdown();
	
	virtual void Update();
	virtual void Render();
	virtual void RenderDeferred();
	
	bool RenderWater();
	bool RenderOcean();
	bool RenderTerrain(XMFLOAT4 clipplane, bool bReflection, bool bRefraction);
	bool RenderTerrainShadowSSAO();
	
	void RenderReflection(XMFLOAT4 clipplane);
	void RenderRefraction(XMFLOAT4 clipplane);
	void RenderShadowMap();
	
protected:
	
	InfinitePlane* m_pInfinitePlane;
	Volume* mVolume;
	ZShadeSandboxMesh::SphereMesh* mSphere;
	ZShadeSandboxMesh::CylindricalMesh* mCylindricalMesh;
	ZShadeSandboxMesh::GeosphereMesh* mGeosphereMesh;
	ZShadeSandboxMesh::CircleMesh* mCircleMesh;

	//
	// obj mesh
	//
	ZShadeSandboxMesh::OBJMesh* mMesh;

	//
	// Terrain
	//
	ZShadeSandboxTerrain::QuadTreeMesh* m_pQuadTreeMesh;
	ZShadeSandboxTerrain::QuadTreeRenderer* m_pQuadTreeRenderer;

	//
	// Sun (Currently does not work)
	//
	ZShadeSandboxLighting::Sun* mSun;
	ZShadeSandboxLighting::SunRenderer* mSunRenderer;

	//
	// Ocean
	//
	OceanSimulator* mOceanSimulator;
	OceanSurface* mOceanSurface;

	//
	// Water
	//
	Water* mWater;
	
	float fHeightScale;
	float fTerrSize;
	float fMinDist;
	float fMaxDist;
	float fTerrRenderSize;
	float fMapSize;
	float fLeafWidth;
	
	bool bToggleTerrainSSAO;
	bool bToggleTerrainShadow;
	bool bToggleTerrainNormalMap;
	bool bToggleTerrainDetailMap;
	bool bRenderFullMap;
	bool bRenderExtraRenderDist;
	bool bClipCameraToTerrain;
	bool bWaterFog;
	bool bToggleTerrain;
	bool bToggleWater;
	bool bRenderOcean;
};

//===============================================================================================================================
//===============================================================================================================================
#endif//__OUTDOORENVIRONMENT_H