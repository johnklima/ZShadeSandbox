//==============================================================================================================================
// TerrainParameters.h
//==============================================================================================================================
#ifndef TERRAINPARAMETERS_H
#define TERRAINPARAMETERS_H
//==============================================================================================================================
//==============================================================================================================================

//
// Includes
//

#include "ZMath.h"
#include "Heightmap.h"
#include "LightManager.h"
#include "LightShadingBuffers.h"
#include "ProceduralParameters.h"

//==============================================================================================================================
//==============================================================================================================================
namespace ZShadeSandboxTerrain {
struct TerrainParameters
{
	string 		g_heightmapName;
	int 		g_TextureRepeat;
	int 		g_TerrainSize;
	int 		g_leafWidth;
	float 		g_cellSpacing;
	float 		g_heightScale;
	//int 		g_extension;
	bool 		g_tessellate;
	bool 		g_procedural;
	bool		g_makeFlat;
	int 		g_terrScale;
	float 		g_seaLevel;

	ZShadeSandboxTerrain::ProceduralParameters g_proceduralParameters;
	
	ZShadeSandboxMesh::ERenderType::Type g_RenderPrimitive;
	
	TerrainParameters()
	{
		g_heightmapName = "";
		g_TextureRepeat = 8;
		g_TerrainSize = 16;
		g_leafWidth = 5;
		g_cellSpacing = 0.5f;
		g_heightScale = 2;
		g_terrScale = 2;
		//g_extension = EHeightExtension::eNone;
		g_tessellate = false;
		g_procedural = false;
		g_makeFlat = true;
		g_seaLevel = 0;
		g_RenderPrimitive = ZShadeSandboxMesh::ERenderType::Type::e4ControlPointPatchList;
	}
};
//==============================================================================================================================
//==============================================================================================================================
struct TerrainShadingConst
{
	// Position of the camera
	XMFLOAT3        g_EyePosW;
	float			g_DetailBrightness;
	
	// The parameters for fog
	float       	g_fogAltitude;
	float       	g_fogNear;
	float       	g_fogFar;
	float       	g_fogThinning;
	XMFLOAT4      	g_fogColor;
	
	// The parameters to scale the terrain
	//float       	g_WorldCellSpace;
	
	// The Terrain texture parameters
	float       	g_TextureAmount;
	float			g_TextureWidth;
	float			g_TextureHeight;
	
	// The parameters for terrain/water updating
	float       	g_seaLevel;
	XMFLOAT4      	g_waterBodyColor;
	
	// Terrain Material
	XMFLOAT4		g_MaterialDiffuseColor;
	XMFLOAT4		g_MaterialAmbientColor;
	float			g_MaterialSpecularPower;
	float			g_MaterialSpecularIntensity;

	// Control variables
	int        		g_useClippingPlane;
	int        		g_useNormalMap;
	int				g_UseSobelFilter;
	int        		g_useShadowMap;
	int				g_useSSAO;
	int				g_useReflection;

	// Are we performing frustum culling
	int  			g_FrustumCull;

	// Amount of tessellation to perform
	float			g_MinDist;
	float			g_MaxDist;
	float			g_MinTess;
	float			g_MaxTess;
	
	float			g_MapSize;
	float   		g_FarPlane;
	
	// The parameters to scale the terrain
	float       	g_TerrSize;
	float       	g_HeightScale;
	float       	g_TexelCellSpaceU;
	float       	g_TexelCellSpaceV;

	// The parameters for terrain/water updating
	XMFLOAT4      	g_ClipPlane;
};
//==============================================================================================================================
//==============================================================================================================================
struct cbTessellationBuffer
{
	XMFLOAT3 	g_EyePosW;
	int  		g_FrustumCull;
	float 		g_MinDist;
	float 		g_MaxDist;
	float 		g_MinTess;
	float 		g_MaxTess;
};
//==============================================================================================================================
//==============================================================================================================================
struct cbDomainConstBuffer
{
	XMFLOAT2	g_TexScale;
	float 		g_HeightScale;
	float 		g_TerrainZScale;
	XMFLOAT4	g_ClipPlane;
	XMFLOAT3  	g_tpadding;
	float   	g_FarPlane;
	XMFLOAT4X4	g_WorldMatrix;
	XMFLOAT4X4	g_ViewMatrix;
	XMFLOAT4X4	g_ProjMatrix;
	XMFLOAT4X4	g_TexSpaceMatrix;
	XMFLOAT4X4  g_ShadowMatrix;
};
//==============================================================================================================================
//==============================================================================================================================
struct cbShadingConstBuffer
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
//==============================================================================================================================
//==============================================================================================================================
inline void BuildTerrainTessellationConstantBuffer(D3D* d3d, ID3D11Buffer*& buffer, TerrainShadingConst tsc)
{
	ZShadeSandboxTerrain::cbTessellationBuffer cTB;
	cTB.g_EyePosW = tsc.g_EyePosW;
	cTB.g_FrustumCull = tsc.g_FrustumCull;
	cTB.g_MinDist = tsc.g_MinDist;
	cTB.g_MaxDist = tsc.g_MaxDist;
	cTB.g_MinTess = tsc.g_MinTess;
	cTB.g_MaxTess = tsc.g_MaxTess;
	// Map the terrain tessellation constant buffer
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res;
		d3d->GetDeviceContext()->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
		{
			assert(mapped_res.pData);
			*(ZShadeSandboxTerrain::cbTessellationBuffer*)mapped_res.pData = cTB;
		}
		d3d->GetDeviceContext()->Unmap(buffer, 0);
	}
}
//==============================================================================================================================
//==============================================================================================================================
inline void BuildTerrainDomainConstantBuffer(D3D* d3d, ID3D11Buffer*& buffer, XMMATRIX world, Camera* camera, ZShadeSandboxLighting::Light* light, TerrainShadingConst tsc)
{
	ZShadeSandboxTerrain::cbDomainConstBuffer cDB;
	cDB.g_TexScale = XMFLOAT2(66.0f, 66.0f);
	cDB.g_HeightScale = tsc.g_HeightScale;
	cDB.g_TerrainZScale = tsc.g_TerrSize;
	cDB.g_ClipPlane = tsc.g_ClipPlane;
	cDB.g_tpadding = XMFLOAT3(0, 0, 0);
	cDB.g_FarPlane = tsc.g_FarPlane;
	cDB.g_WorldMatrix = ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(world));
	if (tsc.g_useReflection)
		cDB.g_ViewMatrix = camera->ReflectionView4x4();
	else
		cDB.g_ViewMatrix = camera->View4x4();
	cDB.g_ProjMatrix = camera->Proj4x4();
	XMMATRIX toTexSpace = XMMatrixScaling(0.5f, -0.5f, 1.0f) * XMMatrixTranslation(0.5f, 0.5f, 0);
	cDB.g_TexSpaceMatrix = ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(toTexSpace));
	cDB.g_ShadowMatrix = light->Perspective()->ShadowTransform4x4();
	// Map the terrain domain constant buffer
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res;
		d3d->GetDeviceContext()->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
		{
			assert(mapped_res.pData);
			*(ZShadeSandboxTerrain::cbDomainConstBuffer*)mapped_res.pData = cDB;
		}
		d3d->GetDeviceContext()->Unmap(buffer, 0);
	}
}
//==============================================================================================================================
//==============================================================================================================================
inline void BuildTerrainShadingConstantBuffer(D3D* d3d, ID3D11Buffer*& buffer, XMFLOAT4X4 view, TerrainShadingConst tsc)
{
	ZShadeSandboxTerrain::cbShadingConstBuffer cSB;
	cSB.g_EyePosW = tsc.g_EyePosW;
	cSB.g_DetailBrightness = tsc.g_DetailBrightness;
	cSB.g_fogThinning = tsc.g_fogThinning;
	cSB.g_fogAltitude = tsc.g_fogAltitude;
	cSB.g_fogNear = tsc.g_fogNear;
	cSB.g_fogFar = tsc.g_fogFar;
	cSB.g_fogColor = tsc.g_fogColor;
	cSB.g_TextureAmount = tsc.g_TextureAmount;
	cSB.g_TextureWidth = tsc.g_TextureWidth;
	cSB.g_TextureHeight = tsc.g_TextureHeight;
	cSB.g_seaLevel = tsc.g_seaLevel;
	cSB.g_waterBodyColor = tsc.g_waterBodyColor;
	cSB.g_MaterialDiffuseColor = tsc.g_MaterialDiffuseColor;
	cSB.g_MaterialAmbientColor = tsc.g_MaterialAmbientColor;
	cSB.g_TexelCellSpaceU = tsc.g_TexelCellSpaceU;
	cSB.g_TexelCellSpaceV = tsc.g_TexelCellSpaceV;
	cSB.g_MaterialSpecularPower = tsc.g_MaterialSpecularPower;
	cSB.g_MaterialSpecularIntensity = tsc.g_MaterialSpecularIntensity;
	cSB.g_useClippingPlane = tsc.g_useClippingPlane;
	cSB.g_useNormalMap = tsc.g_useNormalMap;
	cSB.g_UseSobelFilter = tsc.g_UseSobelFilter;
	cSB.g_useShadowMap = tsc.g_useShadowMap;
	cSB.tpadding = XMFLOAT3(0, 0, 0);
	cSB.g_useSSAO = tsc.g_useSSAO;
	cSB.g_ViewMatrix = view;
	// Map the terrain tessellation constant buffer
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res;
		d3d->GetDeviceContext()->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
		{
			assert(mapped_res.pData);
			*(ZShadeSandboxTerrain::cbShadingConstBuffer*)mapped_res.pData = cSB;
		}
		d3d->GetDeviceContext()->Unmap(buffer, 0);
	}
}
}
//==============================================================================================================================
//==============================================================================================================================
#endif//TERRAINPARAMETERS_H