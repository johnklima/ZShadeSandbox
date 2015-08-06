//===============================================================================================================================
// TerrainTessellationTriShader
//
//===============================================================================================================================
// History
//
// -Created on 4/18/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __TERRAINTESSELLATIONTRISHADER_H
#define __TERRAINTESSELLATIONTRISHADER_H
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
class TerrainTessellationTriShader : public ZShadeSandboxShader::Shader
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
	
	struct cShadingConstBuffer
	{
		// Position of the camera
		XMFLOAT3	g_EyePosW;
		float		padding1;
		
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
		
		// The direction of the sun and lighting
		XMFLOAT3	g_SunDir;
		float		padding2;
		XMFLOAT4	g_AmbientColor;
		XMFLOAT4	g_SpecularColor;
		XMFLOAT4	g_DiffuseColor;
		
		// Control variables
		int			g_useClippingPlane;
		int			g_useNormalMap;
		int			g_UseSobelFilter;
		int			g_useShadowMap;
	};
	
	struct cMatrixBuffer
	{
		XMFLOAT4X4	g_WorldMatrix;
		XMFLOAT4X4	g_ViewMatrix;
		XMFLOAT4X4	g_ProjMatrix;
		
	};
	
public:
	
	TerrainTessellationTriShader(D3D* d3d);
	TerrainTessellationTriShader(const TerrainTessellationTriShader& other);
	~TerrainTessellationTriShader();
	
	bool Initialize();
	void Shutdown();
	
	bool Render(int indexCount, Camera* camera, LightCamera* lightcamera, bool bReflect, ZShadeSandboxTerrain::TerrainShadingConst terrainShadingConst,
		ID3D11ShaderResourceView* layerMapSRV, ID3D11ShaderResourceView* blendMapSRV,
		ID3D11ShaderResourceView* heightMapSRV, ID3D11ShaderResourceView* normalMapSRV,
		ID3D11ShaderResourceView* shadowMapSRV);
	
private:
	
	ID3D11Buffer* m_pTessellationCB;
	ID3D11Buffer* m_pDomainCB;
	ID3D11Buffer* m_pMatrixBufferCB;
	ID3D11Buffer* m_pShadingCB;
};
//===============================================================================================================================
//===============================================================================================================================
#endif//__TERRAINTESSELLATIONTRISHADER_H