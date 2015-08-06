//===============================================================================================================================
// TerrainTessellationTriShadowShader
//
//===============================================================================================================================
// History
//
// -Created on 5/14/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __TERRAINTESSELLATIONTRISHADOWSHADER_H
#define __TERRAINTESSELLATIONTRISHADOWSHADER_H
//===============================================================================================================================
//===============================================================================================================================
//Includes
#include "D3D.h"
#include "Shader.h"
#include "Vertex.h"
#include "TerrainParameters.h"
using namespace std;
//===============================================================================================================================
//===============================================================================================================================
class TerrainTessellationTriShadowShader : public ZShadeSandboxShader::Shader
{
	struct cTessellationBuffer
	{
		XMFLOAT3 	g_EyePosW;
		int   		g_FrustumCull;

		float 		g_MinDist;
		float 		g_MaxDist;
		float 		g_MinTess;
		float 		g_MaxTess;

		XMFLOAT2	padding;
		float		g_TerrSize;
		float		g_TerrScale;
	};
	
	struct cDomainConstBuffer
	{
		XMFLOAT2	g_TexScale;
		float 		g_HeightScale;
		float 		g_TerrainZScale;
		float 		g_TexelCellSpaceU;
		float 		g_TexelCellSpaceV;
		XMFLOAT2  	padding;
		XMFLOAT4	g_ClipPlane;
	};
	
	struct cMatrixBuffer
	{
		XMFLOAT4X4	g_WorldMatrix;
		XMFLOAT4X4	g_ViewMatrix;
		XMFLOAT4X4	g_ProjMatrix;
		XMFLOAT4X4  g_ShadowMatrix;
	};
	
public:
	
	TerrainTessellationTriShadowShader(D3D* d3d);
	TerrainTessellationTriShadowShader(const TerrainTessellationTriShadowShader& other);
	~TerrainTessellationTriShadowShader();
	
	bool Initialize();
	void Shutdown();
	
	bool Render(int indexCount, LightCamera* lightcamera, ZShadeSandboxTerrain::TerrainShadingConst terrainShadingConst, ID3D11ShaderResourceView* heightMapSRV);
	
private:
	
	ID3D11Buffer* m_pTessellationCB;
	ID3D11Buffer* m_pDomainCB;
	ID3D11Buffer* m_pMatrixBufferCB;
};
//===============================================================================================================================
//===============================================================================================================================
#endif//__TERRAINTESSELLATIONTRISHADOWSHADER_H