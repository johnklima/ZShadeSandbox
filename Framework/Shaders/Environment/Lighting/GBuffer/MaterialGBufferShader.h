//===============================================================================================================================
// Material GBuffer Shader
//
//===============================================================================================================================
// History
//
// -Created on 6/7/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __MATERIALGBUFFERSHADER_H
#define __MATERIALGBUFFERSHADER_H
//===============================================================================================================================
//===============================================================================================================================
//Includes
#include "D3D.h"
#include "Shader.h"
#include "Vertex.h"
#include "MeshRenderParameters.h"
#include "ShaderMaterial.h"
using namespace std;
//===============================================================================================================================
//===============================================================================================================================
class MaterialGBufferShader : public ZShadeSandboxShader::Shader
{
	//struct cbPackBuffer
	//{
	//	XMFLOAT3	g_EyePosW;
	//	int			g_UsingDiffuseArrayTexture;
	//	XMFLOAT4 	g_ClipPlane;
	//	XMFLOAT4	g_MaterialDiffuseColor;
	//	XMFLOAT4	g_MaterialAmbientColor;
	//	float		g_MaterialSpecularPower;
	//	float		g_MaterialSpecularIntensity;
	//	int	   		g_UsingDiffuseTexture;
	//	int	   		g_UsingNormalMapTexture;
	//	int	   		g_UsingBlendMapTexture;
	//	int	   		g_UsingDetailMapTexture;
	//	int			g_UseAlphaMapTexture;
	//	int			g_EnableTransparency;
	//	int			g_EnableLighting;
	//	float		g_DetailBrightness;
	//	int	   		g_FlipTextureH;
	//	int	   		g_FlipTextureV;
	//	XMFLOAT3	g_AlphaToCoverageValue; // Value that clips pixel during alpha blending
	//	float		g_padding1;
	//	XMFLOAT2	g_padding2;
	//	int			g_UseShadowMap;
	//	int			g_UseSSAOMap;
	//};
	//
	//struct cbMatrixBuffer
	//{
	//	XMFLOAT4X4	g_matWorld;
	//	XMFLOAT4X4	g_matView;
	//	XMFLOAT4X4	g_matProj;
	//};
	
public:
	
	MaterialGBufferShader(D3D* d3d);
	MaterialGBufferShader(const MaterialGBufferShader& other);
	~MaterialGBufferShader();
	
	bool Initialize();
	void Shutdown();
	
	bool Render11
	(	int indexCount
	,	int instanceCount
	,	ZShadeSandboxMesh::MeshRenderParameters mrp
	,	ZShadeSandboxLighting::ShaderMaterial* material
	);

private:
	
	ID3D11Buffer* m_pPackCB;
	ID3D11Buffer* m_pMatrixCB;
};
//===============================================================================================================================
//===============================================================================================================================
#endif//__MATERIALGBUFFERSHADER_H