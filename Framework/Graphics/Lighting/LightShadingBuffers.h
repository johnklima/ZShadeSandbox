//==============================================================================================================================
// LightShadingBuffers.h
//
// buffers needed for lighting needed in a shader
//==============================================================================================================================
// History
//
// -Created on 4/20/2015  by Dustin Watson
//==============================================================================================================================
#ifndef __LIGHTSHADERBUFFERS_H
#define __LIGHTSHADERBUFFERS_H
//==============================================================================================================================
//==============================================================================================================================
#include "D3D.h"
#include "LightManager.h"
//==============================================================================================================================
//==============================================================================================================================
namespace ZShadeSandboxLighting {
// These structs are for passing data to the packed constant buffers so the human does not need to worry about packing
/*struct AmbientLightBuffer
{
	XMFLOAT4	g_AmbientColor;
	int			g_Toggle;
};
//==============================================================================================================================
//==============================================================================================================================
struct DirectionalLightBuffer
{
	XMFLOAT3	g_LightDirection;
	XMFLOAT4	g_AmbientColor;
	XMFLOAT4	g_DiffuseColor;
	float		g_Intensity;
	int			g_Toggle;
};
//==============================================================================================================================
//==============================================================================================================================
struct SpotLightBuffer
{
	XMFLOAT4	g_AmbientColor;
	XMFLOAT4	g_DiffuseColor;
	XMFLOAT3	g_LightPosition;
	float		g_LightRange;
	float		g_SpotCosOuterCone;
	float		g_SpotInnerConeReciprocal;
	float		g_CosineAngle;
	float		g_Intensity;
	int			g_Toggle;
};
//==============================================================================================================================
//==============================================================================================================================
struct PointLightBuffer
{
	XMFLOAT3	g_LightPosition;
	float		g_LightRange;
	XMFLOAT3	g_Attenuation;
	XMFLOAT4	g_AmbientColor;
	XMFLOAT4	g_DiffuseColor;
	float		g_Intensity;
	int			g_Toggle;
};
//==============================================================================================================================
//==============================================================================================================================
struct CapsuleLightBuffer
{
	XMFLOAT3 	g_LightPosition;
	float 		g_LightRange;
	XMFLOAT3 	g_LightDirection;
	float 		g_LightLength;
	XMFLOAT3 	g_CapsuleDirectionLength;
	float 		g_CapsuleIntensity;
	XMFLOAT4	g_AmbientColor;
	XMFLOAT4	g_DiffuseColor;
	float		g_Intensity;
	int			g_Toggle;
};
//==============================================================================================================================
//==============================================================================================================================
struct LightBuffer
{
	AmbientLightBuffer			g_AmbientLight[MAX_LIGHTS];
	DirectionalLightBuffer		g_DirectionalLight[MAX_LIGHTS];
	SpotLightBuffer				g_SpotLight[MAX_LIGHTS];
	PointLightBuffer			g_PointLight[MAX_LIGHTS];
	CapsuleLightBuffer			g_CapsuleLight[MAX_LIGHTS];
	
	int				 			g_DirectionalLightCount;
	int				 			g_SpotLightCount;
	int				 			g_PointLightCount;
	int              			g_CapsuleLightCount;
	
	XMFLOAT3					g_AmbientDown;
	int							g_AmbientLightCount;
	XMFLOAT3					g_AmbientUp;
};*/
//==============================================================================================================================
//==============================================================================================================================
struct SunLightBuffer
{
	XMFLOAT3	g_SunDir;
	int			g_EnableSun;
	XMFLOAT4	g_SunDiffuseColor;
	float		g_SunShineness;
	float		g_SunIntensity;
};
//==============================================================================================================================
//==============================================================================================================================
// These structs are properly packed for the shading constant buffer
struct cbAmbientLightBuffer
{
	XMFLOAT4	g_AmbientColor;
	XMFLOAT3	ambientpadding;
	int			g_Toggle;
};
//==============================================================================================================================
//==============================================================================================================================
struct cbDirectionalLightBuffer
{
	XMFLOAT3	g_Direction;
	float		g_Intensity;
	XMFLOAT4	g_Ambient;
	XMFLOAT4	g_Diffuse;
	XMFLOAT3	directionalpadding;
	int			g_Toggle;
};
//==============================================================================================================================
//==============================================================================================================================
struct cbSpotLightBuffer
{
	XMFLOAT4	g_AmbientColor;
	XMFLOAT4	g_DiffuseColor;
	XMFLOAT3	g_LightPosition;
	float		g_Intensity;
	float		g_LightRange;
	float		g_SpotCosOuterCone;
	float		g_SpotInnerConeReciprocal;
	float		g_CosineAngle;
	XMFLOAT3	spotpadding;
	int			g_Toggle;
};
//==============================================================================================================================
//==============================================================================================================================
struct cbPointLightBuffer
{
	XMFLOAT3	g_LightPosition;
	float		g_LightRange;
	XMFLOAT3	g_Attenuation;
	float		g_Intensity;
	XMFLOAT4	g_AmbientColor;
	XMFLOAT4	g_DiffuseColor;
	XMFLOAT3	pointpadding;
	int			g_Toggle;
};
//==============================================================================================================================
//==============================================================================================================================
struct cbCapsuleLightBuffer
{
	XMFLOAT3 	g_LightPosition;
	float 		g_LightRange;
	XMFLOAT3 	g_LightDirection;
	float 		g_LightLength;
	XMFLOAT3 	g_CapsuleDirectionLength;
	float 		g_CapsuleIntensity;
	XMFLOAT4	g_AmbientColor;
	XMFLOAT4	g_DiffuseColor;
	XMFLOAT3	capsulepadding;
	int			g_Toggle;
};
//==============================================================================================================================
//==============================================================================================================================
struct cbLightBuffer
{
	cbAmbientLightBuffer		g_AmbientLight[MAX_LIGHTS];
	cbDirectionalLightBuffer	g_DirectionalLight[MAX_LIGHTS];
	cbSpotLightBuffer			g_SpotLight[MAX_LIGHTS];
	cbPointLightBuffer			g_PointLight[MAX_LIGHTS];
	cbCapsuleLightBuffer		g_CapsuleLight[MAX_LIGHTS];
	
	int				 			g_DirectionalLightCount;
	int				 			g_SpotLightCount;
	int				 			g_PointLightCount;
	int              			g_CapsuleLightCount;
	
	XMFLOAT3					g_AmbientDown;
	int							g_AmbientLightCount;
	XMFLOAT3					g_AmbientUp;
	float						padding;
};
//==============================================================================================================================
//==============================================================================================================================
struct cbSunLightBuffer
{
	XMFLOAT3	g_SunDir;
	int			g_EnableSun;
	XMFLOAT4	g_SunDiffuseColor;
	XMFLOAT3	sunpadding1;
	float		g_SunShineness;
	XMFLOAT3	sunpadding2;
	float		g_SunIntensity;
};
//==============================================================================================================================
//==============================================================================================================================
struct cbMaterialShadingBuffer
{
	XMFLOAT3	g_EyePosW;
	float		g_Alpha;
	XMFLOAT4 	g_ClipPlane;
	XMFLOAT4	g_MaterialDiffuseColor;
	XMFLOAT4	g_MaterialAmbientColor;
	XMFLOAT4	g_MaterialSpecularColor;
	XMFLOAT4	g_MaterialEmissiveColor;
	float		g_Emissivity;
	float		g_Reflectivity;
	float		g_Transmissivity;
	float		g_RefractionIndex;
	XMFLOAT3	g_AlphaToCoverageValue;
	float		g_DetailBrightness;
	XMFLOAT3	g_TransmissionFilter;
	float		g_MaterialSpecularPower;
	float		g_MaterialSpecularIntensity;
	int			g_IlluminationModel;
	int	   		g_FlipTextureH;
	int	   		g_FlipTextureV;
	int			g_UsingDiffuseArrayTexture;
	int	   		g_UsingDiffuseTexture;
	int	   		g_UsingAmbientTexture;
	int	   		g_UsingSpecularTexture;
	int	   		g_UsingEmissiveTexture;
	int	   		g_UsingNormalMapTexture;
	int	   		g_UsingBlendMapTexture;
	int	   		g_UsingDetailMapTexture;
	int			g_UsingAlphaMapTexture;
	int			g_UsingTransparency;
	int			g_UsingShadowMap;
	int			g_UsingSSAOMap;
	int			g_UsingDisplacementMap;
	float		g_FarPlane;
	int			g_SpecularToggle;
	int			g_EnableLighting;
	XMFLOAT3	matpadding;
	float		g_BlendAmount;
};
//==============================================================================================================================
//==============================================================================================================================
struct cbMaterialTessellationBuffer
{
	XMFLOAT3 	g_EyePosW;
	float 		g_NoDistTessFactor;
	float 		g_MinTessDist;
	float 		g_MaxTessDist;
	float 		g_MinTessFactor;
	float 		g_MaxTessFactor;
	XMFLOAT3	tesspadding;
	int			g_EnableDistTess;
};
//==============================================================================================================================
//==============================================================================================================================
struct cbMaterialDomainBuffer
{
	XMFLOAT4	g_ClipPlane;
	XMFLOAT3	domainpadding;
	float		g_FarPlane;
	XMFLOAT4X4	g_matWorld;
	XMFLOAT4X4	g_matView;
	XMFLOAT4X4	g_matProj;
};
//==============================================================================================================================
//==============================================================================================================================
struct cbMaterialVertexBuffer
{
	XMFLOAT3	materialpadding2;
	int			g_UsingDisplacementMap;
};
//==============================================================================================================================
//==============================================================================================================================
struct LightRenderParameters
{
	Camera* camera;
	bool toggleMesh;
	bool reflect;
	bool toggleWireframe;
	bool renderDeferred;
	XMFLOAT4 clipplane;

	LightRenderParameters()
	{
		camera = 0;
		toggleMesh = false;
		reflect = false;
		toggleWireframe = false;
		renderDeferred = false;
		clipplane = XMFLOAT4(0, 0, 0, 0);
	}
};
}
//==============================================================================================================================
//==============================================================================================================================
#endif//__LIGHTSHADERBUFFERS_H