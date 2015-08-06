//===============================================================================================================================
// MaterialLightShader
//
//===============================================================================================================================
// History
//
// -Created on 6/10/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __MATERIALLIGHTSHADER_H
#define __MATERIALLIGHTSHADER_H
//===============================================================================================================================
//===============================================================================================================================
//Includes
#include "D3D.h"
#include "Shader.h"
#include "Vertex.h"
#include "ShaderMaterial.h"
#include "LightShadingBuffers.h"
#include "MeshRenderParameters.h"
using namespace std;
//===============================================================================================================================
//===============================================================================================================================
class MaterialLightShader : public ZShadeSandboxShader::Shader
{
	struct cbLightTypeBuffer
	{
		int	g_UsingAmbientLight;
		int	g_UsingDirectionalLight;
		int	g_UsingPointLight;
		int	g_UsingSpotLight;
		XMFLOAT3 lighttypepadding;
		int	g_UsingCapsuleLight;
	};
	
public:
	
	MaterialLightShader(D3D* d3d);
	MaterialLightShader(const MaterialLightShader& other);
	~MaterialLightShader();
	
	bool Initialize();
	void Shutdown();
	
	bool Render11
	(	int indexCount
	,	int instanceCount
	,	ZShadeSandboxMesh::MeshRenderParameters mrp
	,	ZShadeSandboxLighting::ShaderMaterial* material
	);
	
private:
	
	ID3D11Buffer* m_pAmbientLightCB;
	ID3D11Buffer* m_pDirectionalLightCB;
	ID3D11Buffer* m_pPointLightCB;
	ID3D11Buffer* m_pSpotLightCB;
	ID3D11Buffer* m_pCapsuleLightCB;
	
	ID3D11Buffer* m_pLightTypeCB;
	ID3D11Buffer* m_pShadingCB;
	ID3D11Buffer* m_pMatrixCB;
};
//===============================================================================================================================
//===============================================================================================================================
#endif//__MATERIALLIGHTSHADER_H