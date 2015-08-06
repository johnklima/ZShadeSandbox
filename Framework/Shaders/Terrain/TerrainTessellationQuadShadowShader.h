//===============================================================================================================================
// TerrainTessellationQuadShadowShader
//
//===============================================================================================================================
// History
//
// -Created on 5/14/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __TERRAINTESSELLATIONQUADSHADOWSHADER_H
#define __TERRAINTESSELLATIONQUADSHADOWSHADER_H
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
class TerrainTessellationQuadShadowShader : public ZShadeSandboxShader::Shader
{
	struct cTessellationBuffer
	{
		XMFLOAT3 	g_EyePosW;
		float  		g_FrustumCull;

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
		XMFLOAT4	g_ClipPlane;
		XMFLOAT3  	g_tpadding;
		float   	g_FarPlane;
	};
	
	struct cMatrixBuffer
	{
		XMFLOAT4X4	g_WorldMatrix;
		XMFLOAT4X4	g_ViewMatrix;
		XMFLOAT4X4	g_ProjMatrix;
		XMFLOAT4X4  g_TexSpaceMatrix;
		XMFLOAT4X4  g_ShadowMatrix;
	};
	
public:
	
	TerrainTessellationQuadShadowShader(D3D* d3d);
	TerrainTessellationQuadShadowShader(const TerrainTessellationQuadShadowShader& other);
	~TerrainTessellationQuadShadowShader();
	
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
#endif//__TERRAINTESSELLATIONQUADSHADOWSHADER_H