//===============================================================================================================================
// TerrainTessellationQuadSSAOShader
//
//===============================================================================================================================
// History
//
// -Created on 4/9/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __TERRAINTESSELLATIONQUADSSAOSHADER_H
#define __TERRAINTESSELLATIONQUADSSAOSHADER_H
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
class TerrainTessellationQuadSSAOShader : public ZShadeSandboxShader::Shader
{
	struct cTessellationBuffer
	{
		XMFLOAT3 	g_EyePosW;
		int  		g_FrustumCull;

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

	struct cShadingConstBuffer
	{
		// Position of the camera
		XMFLOAT3	g_EyePosW;
		float		g_DetailBrightness;

		// The parameters for fog
		float		g_fogThinning;
		float		g_fogAltitude;
		float		g_fogNear;
		float		g_fogFar;
		XMFLOAT4	g_fogColor;

		// The Terrain texture parameters
		float		g_TextureAmount;
		float		g_TextureWidth;
		float		g_TextureHeight;

		// The parameters for terrain/water updating
		float		g_seaLevel;
		XMFLOAT4	g_waterBodyColor;

		// Terrain Material
		XMFLOAT4	g_MaterialDiffuseColor;
		XMFLOAT4	g_MaterialAmbientColor;
		float 		g_TexelCellSpaceU;
		float 		g_TexelCellSpaceV;
		float		g_MaterialSpecularPower;
		float		g_MaterialSpecularIntensity;

		// Control variables
		int			g_useClippingPlane;
		int			g_useNormalMap;
		int			g_UseSobelFilter;
		int			g_useShadowMap;
		XMFLOAT3	tpadding;
		int			g_useSSAO;

		XMFLOAT4X4	g_ViewMatrix;
	};

	struct cMatrixBuffer
	{
		XMFLOAT4X4	g_WorldMatrix;
		XMFLOAT4X4	g_ViewMatrix;
		XMFLOAT4X4	g_ProjMatrix;
		XMFLOAT4X4	g_TexSpaceMatrix;
		XMFLOAT4X4  g_ShadowMatrix;
	};

public:

	TerrainTessellationQuadSSAOShader(D3D* d3d);
	TerrainTessellationQuadSSAOShader(const TerrainTessellationQuadSSAOShader& other);
	~TerrainTessellationQuadSSAOShader();

	bool Initialize();
	void Shutdown();

	bool Render(int indexCount, Camera* camera, LightCamera* lightcamera, ZShadeSandboxTerrain::TerrainShadingConst terrainShadingConst, ID3D11ShaderResourceView* heightMapSRV);

private:

	ID3D11Buffer* m_pTessellationCB;
	ID3D11Buffer* m_pDomainCB;
	ID3D11Buffer* m_pMatrixBufferCB;
	ID3D11Buffer* m_pShadingCB;
	ID3D11Buffer* m_pLightCB;
	ID3D11Buffer* m_pSunCB;
};
//===============================================================================================================================
//===============================================================================================================================
#endif//__TERRAINTESSELLATIONQUADSSAOSHADER_H