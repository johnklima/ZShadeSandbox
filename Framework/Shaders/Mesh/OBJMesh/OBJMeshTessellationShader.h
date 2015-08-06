//===============================================================================================================================
// OBJMeshTessellationShader
//
//===============================================================================================================================
// History
//
// -Created on 4/15/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __OBJMESHTESSELLATIONSHADER_H
#define __OBJMESHTESSELLATIONSHADER_H
//===============================================================================================================================
//===============================================================================================================================
//Includes
#include "D3D.h"
#include "Shader.h"
#include "Vertex.h"
#include "ShaderMaterial.h"
#include "MeshRenderParameters.h"
using namespace std;
//===============================================================================================================================
//===============================================================================================================================
class OBJMeshTessellationShader : public ZShadeSandboxShader::Shader
{
	/*struct cbTessellationBuffer
	{
		XMFLOAT3 	g_EyePosW;
		float		g_TessellationFactor;
		
		float 		g_MinDist;
		float 		g_MaxDist;
		float 		g_MinTess;
		float 		g_MaxTess;
	};
	
	struct cbDomainBuffer
	{
		XMFLOAT4 	g_ClipPlane;
		XMFLOAT3	padding;
		float		g_FarPlane;
		XMFLOAT4X4	g_World;
		XMFLOAT4X4	g_View;
		XMFLOAT4X4	g_Proj;
	};
	
	struct cbOBJShadingBuffer
	{
		XMFLOAT4	g_DiffuseColor;
		XMFLOAT4	g_AmbientColor;
		XMFLOAT4	g_SpecularColor;
		XMFLOAT4	g_EmissiveColor;
		XMFLOAT3	g_TransmissionFilter;
		float		g_Alpha;
		float		g_SpecularPower;
		float		g_SpecularIntensity;
		float		g_RefractionIndex;
		float		g_DetailBrightness;
		int			g_IlluminationModel;
		int			g_HasDiffuseTexture;
		int			g_HasAmbientTexture;
		int			g_HasSpecularTexture;
		int			g_SpecularToggle;
		int			g_HasEmissiveTexture;
		int			g_HasNormalMap;
		int			g_HasDetailMap;
		XMFLOAT2	padding;
		int			g_HasTransparency;
		int			g_HasAlphaTexture;
		XMFLOAT3	g_EyePosW;
		float		padding2;
	};*/
	
public:
	
	OBJMeshTessellationShader(D3D* d3d);
	OBJMeshTessellationShader(const OBJMeshTessellationShader& other);
	~OBJMeshTessellationShader();

	bool Initialize();
	void Shutdown();
	
	bool Render
	(	int startIndex
	,	int indexCount
	,	ZShadeSandboxMesh::MeshRenderParameters mrp
	,	ZShadeSandboxLighting::ShaderMaterial* material
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
#endif//__OBJMESHTESSELLATIONSHADER_H