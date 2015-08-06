//===============================================================================================================================
// TerrainTessellationQuadShader
//
//===============================================================================================================================
// History
//
// -Created on 4/9/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __TERRAINTESSELLATIONQUADSHADER_H
#define __TERRAINTESSELLATIONQUADSHADER_H
//===============================================================================================================================
//===============================================================================================================================
//Includes
#include "D3D.h"
#include "Shader.h"
#include "Vertex.h"
#include "TerrainParameters.h"
#include "LightManager.h"
using namespace std;
//===============================================================================================================================
//===============================================================================================================================
class TerrainTessellationQuadShader : public ZShadeSandboxShader::Shader
{
public:
	
	TerrainTessellationQuadShader(D3D* d3d);
	TerrainTessellationQuadShader(const TerrainTessellationQuadShader& other);
	~TerrainTessellationQuadShader();
	
	bool Initialize();
	void Shutdown();
	
	bool Render
	(	int indexCount
	,	Camera* camera
	,	ZShadeSandboxLighting::Light* light
	,	ZShadeSandboxTerrain::TerrainShadingConst terrainShadingConst
	,	ID3D11ShaderResourceView* heightMapSRV
	,	ZShadeSandboxLighting::ShaderMaterial* terrainMaterial
	);

private:
	
	ID3D11Buffer* m_pTessellationCB;
	ID3D11Buffer* m_pDomainCB;
	ID3D11Buffer* m_pShadingCB;
	ID3D11Buffer* m_pLightCB;
	ID3D11Buffer* m_pSunCB;
};
//===============================================================================================================================
//===============================================================================================================================
#endif//__TERRAINTESSELLATIONQUADSHADER_H