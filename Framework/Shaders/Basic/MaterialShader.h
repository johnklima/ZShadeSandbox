//===============================================================================================================================
// MaterialShader
//
//===============================================================================================================================
// History
//
// -Created on 4/23/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __MATERIALSHADER_H
#define __MATERIALSHADER_H
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
class MaterialShader : public ZShadeSandboxShader::Shader
{
public:
	
	MaterialShader(D3D* d3d);
	MaterialShader(const MaterialShader& other);
	~MaterialShader();
	
	bool Initialize();
	void Shutdown();
	
	bool Render11
	(	int indexCount
	,	int instanceCount
	,	ZShadeSandboxMesh::MeshRenderParameters mrp
	,	ZShadeSandboxLighting::ShaderMaterial* material
	);
	
private:
	
	static ZShadeSandboxLighting::Light* mDefaultLight;
	
	ID3D11Buffer* m_pShadingCB;
	ID3D11Buffer* m_pMatrixCB;
	ID3D11Buffer* m_pLightCB;
	ID3D11Buffer* m_pSunCB;
};
//===============================================================================================================================
//===============================================================================================================================
#endif//__MATERIALSHADER_H