//===============================================================================================================================
// OBJMeshShader
//
//===============================================================================================================================
// History
//
// -Created on 4/15/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __OBJMESHSHADER_H
#define __OBJMESHSHADER_H
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
class OBJMeshShader : public ZShadeSandboxShader::Shader
{
public:
	
	OBJMeshShader(D3D* d3d);
	OBJMeshShader(const OBJMeshShader& other);
	~OBJMeshShader();

	bool Initialize();
	void Shutdown();
	
	bool Render
	(	int startIndex
	,	int indexCount
	,	ZShadeSandboxMesh::MeshRenderParameters mrp
	,	ZShadeSandboxLighting::ShaderMaterial* material
	);

private:
	
	ID3D11Buffer* m_pShadingCB;
	ID3D11Buffer* m_pMatrixCB;
	ID3D11Buffer* m_pLightCB;
	ID3D11Buffer* m_pSunCB;
};
//===============================================================================================================================
//===============================================================================================================================
#endif//__OBJMESHSHADER_H