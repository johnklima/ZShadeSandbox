//===============================================================================================================================
// MaterialTessellationShader
//
//===============================================================================================================================
// History
//
// -Created on 6/10/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __MATERIALTESSELLATIONSHADER_H
#define __MATERIALTESSELLATIONSHADER_H
//===============================================================================================================================
//===============================================================================================================================

//
//Includes
//

#include "D3D.h"
#include "Shader.h"
#include "Vertex.h"
#include "ShaderMaterial.h"
#include "LightShadingBuffers.h"
#include "MeshRenderParameters.h"
using namespace std;

//===============================================================================================================================
//===============================================================================================================================
class MaterialTessellationShader : public ZShadeSandboxShader::Shader
{
public:
	
	MaterialTessellationShader(D3D* d3d, ZShadeSandboxLighting::EMaterialTessellationType::Type type);
	MaterialTessellationShader(const MaterialTessellationShader& other);
	~MaterialTessellationShader();
	
	bool Initialize();
	void Shutdown();
	
	bool Render11
	(	int indexCount
	,	int instanceCount
	,	ZShadeSandboxMesh::MeshRenderParameters mrp
	,	ZShadeSandboxLighting::ShaderMaterial* material
	);
	
private:
	
	ZShadeSandboxLighting::EMaterialTessellationType::Type mType;
	
	ID3D11Buffer* m_pTessellationCB;
	ID3D11Buffer* m_pDomainCB;
	ID3D11Buffer* m_pShadingCB;
	ID3D11Buffer* m_pLightCB;
	ID3D11Buffer* m_pSunCB;
};
//===============================================================================================================================
//===============================================================================================================================
#endif//__QUADMATERIALTESSELLATIONSHADER_H