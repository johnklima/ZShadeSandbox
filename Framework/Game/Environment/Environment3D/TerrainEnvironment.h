//===============================================================================================================================
// TerrainEnvironment
//
//===============================================================================================================================
// History
//
// -Created on 5/19/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __TERRAINENVIRONMENT_H
#define __TERRAINENVIRONMENT_H
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//

#include "Environment3D.h"
#include "QuadTree.h"
#include "QuadTreeRenderer.h"

//===============================================================================================================================
//===============================================================================================================================

//
// Defines what is needed for terrain to be rendered
//
class TerrainEnvironment : public Environment3D
{
public:
	TerrainEnvironment(EngineOptions* eo);
	~TerrainEnvironment();
	
	void Init();
	void Shutdown();
	
	virtual void Update();
	virtual void Render();
	void RenderDeferred();

	void RenderTerrainShadowSSAO();
	
protected:
	
	//
	// Terrain
	//
	ZShadeSandboxTerrain::QuadTreeMesh* m_pQuadTreeMesh;
	ZShadeSandboxTerrain::QuadTreeRenderer* m_pQuadTreeRenderer;
	ZShadeSandboxMesh::QuadMesh* mGroundCursorMesh;
	
	float fHeightScale;
	float fTerrSize;
	float fMinDist;
	float fMaxDist;
	float fTerrRenderSize;
	float fMapSize;
	float fLeafWidth;
	
	bool bTerrainTrianglesAdded;
	bool bToggleTerrainSSAO;
	bool bToggleTerrainShadow;
	bool bToggleTerrainNormalMap;
	bool bToggleTerrainDetailMap;
	bool bRenderFullMap;
	bool bRenderExtraRenderDist;
	bool bClipCameraToTerrain;
};

//===============================================================================================================================
//===============================================================================================================================
#endif//__OUTDOORENVIRONMENT_H