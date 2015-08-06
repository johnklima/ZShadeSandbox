//===============================================================================================================================
// OBJGBufferShader
//
//===============================================================================================================================
// History
//
// -Created on 6/8/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __OBJGBUFFERSHADER_H
#define __OBJGBUFFERSHADER_H
//===============================================================================================================================
//===============================================================================================================================
//Includes
#include "D3D.h"
#include "Shader.h"
#include "Vertex.h"
#include "ShaderMaterial.h"
using namespace std;
//===============================================================================================================================
//===============================================================================================================================
class OBJGBufferShader : public ZShadeSandboxShader::Shader
{
	/*struct cbPackBuffer
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
	};
	
	struct cbMatrixBuffer
	{
		XMFLOAT4X4	g_matWorld;
		XMFLOAT4X4	g_matView;
		XMFLOAT4X4	g_matProj;
	};*/
	
public:
	
	OBJGBufferShader(D3D* d3d);
	OBJGBufferShader(const OBJGBufferShader& other);
	~OBJGBufferShader();
	
	bool Initialize();
	void Shutdown();
	
	bool Render11
	(	int startIndex
	,	int indexCount
	,	ZShadeSandboxMesh::MeshRenderParameters mrp
	,	ZShadeSandboxLighting::ShaderMaterial* material
	);
	
private:
	
	ID3D11Buffer* m_pPackCB;
	ID3D11Buffer* m_pMatrixCB;
};
//===============================================================================================================================
//===============================================================================================================================
#endif//__OBJGBUFFERSHADER_H