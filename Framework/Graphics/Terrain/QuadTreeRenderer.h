//=====================================================================================
// QuadTreeRenderer.h
//=====================================================================================
#ifndef __QUADTREERENDERER_H
#define __QUADTREERENDERER_H
//================================================================================================================
//================================================================================================================
//Windows includes
#include <Windows.h>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
#include <fstream>
using namespace std;
//================================================================================================================
//================================================================================================================
//My Includes
#include "MaterialShader.h"
#include "TerrainTessellationQuadShader.h"
#include "TerrainTessellationTriShader.h"
#include "TerrainTessellationQuadShadowShader.h"
#include "TerrainTessellationTriShadowShader.h"
#include "TerrainTessellationQuadSSAOShader.h"
#include "QuadMesh.h"
#include "QuadTreeMesh.h"
#include "Camera.h"
#include "LightCamera.h"
#include "RenderTarget2D.h"
#include "ProjectedCamera.h"
//================================================================================================================
//================================================================================================================
namespace ZShadeSandboxTerrain {
class QuadTreeRenderer
{
public:

	QuadTreeRenderer(D3D* d3d, ZShadeSandboxTerrain::QuadTreeMesh* qtm);
	~QuadTreeRenderer();
	
	void Render(Camera* pCamera, ZShadeSandboxLighting::Light* light, ZShadeSandboxTerrain::TerrainShadingConst tsc);
	void RenderShadowMap(Camera* pCamera, LightCamera* camera, ZShadeSandboxTerrain::TerrainShadingConst tsc);
	void RenderSSAO(Camera* pCamera, LightCamera* camera, ZShadeSandboxTerrain::TerrainShadingConst tsc);
	
public:
	
	bool& RenderFullMap()							{ return bRenderFullMap; }
	bool& RenderExtraRenderDist()					{ return bRenderExtraRenderDist; }
	float& RenderSize()								{ return bRenderSize; }
	bool& ShowBoundingBoxes()						{ return bShowBoundingBox; }
	bool& ShowRootBoundingBoxOnly()					{ return bShowRootBoundingBox; }
	
	XMMATRIX GetWorld();
	void SetWorld(XMFLOAT4X4 world)					{ mWorld = world; }
	void SetWireframe(bool wire)					{ bWireframe = wire; }
	
	const ZShadeSandboxTerrain::QuadTreeMesh* GetMesh() const				{ return m_quadtreeMesh; }
	
private:
	
	void Render(ZShadeSandboxTerrain::QMeshNode* node, Camera* pCamera, ZShadeSandboxLighting::Light* light, ZShadeSandboxTerrain::TerrainShadingConst tsc);
	void RenderShadowMap(ZShadeSandboxTerrain::QMeshNode* node, ZShadeSandboxTerrain::TerrainShadingConst tsc, Camera* pCamera, LightCamera* camera);
	void RenderSSAO(ZShadeSandboxTerrain::QMeshNode* node, ZShadeSandboxTerrain::TerrainShadingConst tsc, Camera* pCamera, LightCamera* camera);
	void RenderMeshBuffers(ZShadeSandboxTerrain::QMeshNode* node);
	
	void BuildBlendMapSRV();

private:
	
	//
	// Variables
	//
	
	vector<XMFLOAT4> mBlendMapValues;
	static ID3D11ShaderResourceView* mBlendMapSRV;
	ZShadeSandboxTerrain::QuadTreeMesh* m_quadtreeMesh;
	EngineOptions* m_EngineOptions;
	D3D* m_d3d;
	XMFLOAT4X4 mWorld;
	
	TerrainTessellationQuadSSAOShader*      m_TerrainTessellationQuadSSAOShader;

	TerrainTessellationQuadShadowShader* 	m_TerrainTessellationQuadShadowShader;
	TerrainTessellationTriShadowShader* 	m_TerrainTessellationTriShadowShader;
	TerrainTessellationQuadShader* 			m_TerrainTessellationQuadShader;
	TerrainTessellationTriShader* 			m_TerrainTessellationTriShader;
	MaterialShader*							m_TextureShader;
	
	RenderTarget2D* mShadowMapTarget;
	RenderTarget2D* mSSAOTarget;
	
	Texture* diffuseArray;
	Texture* blendmap;
	Texture* normalmap;
	Texture* detailmap;
	
	float bRenderSize;
	
	bool bTextureUpdated;
	bool bRenderFullMap;
	bool bRenderExtraRenderDist;
	bool bRootContainsCamera;
	bool bWireframe;
	bool bShowRootBoundingBox;
	bool bShowBoundingBox;
};
}
//================================================================================================================
//================================================================================================================
#endif//__QUADTREERENDERER_H
