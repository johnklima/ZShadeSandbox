//#include "stdafx.h"
#include "GrassFieldShader.h"
//==============================================================================================================================
GrassFieldShader::GrassFieldShader(D3D* d3d)
:	ZShadeSandboxShader::Shader(d3d)
{
	Initialize(d3d);
}
//==============================================================================================================================
GrassFieldShader::GrassFieldShader(const GrassFieldShader& other)
:	ZShadeSandboxShader::Shader(other)
{
}
//==============================================================================================================================
GrassFieldShader::~GrassFieldShader()
{

}
//==============================================================================================================================
bool GrassFieldShader::Initialize(D3D* d3d)
{
	return true;
}
//==============================================================================================================================
void GrassFieldShader::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	//ShutdownShader();
}
//==============================================================================================================================
void GrassFieldShader::RenderFunc(Camera* pCamera, XMMATRIX view, XMMATRIX world, XMFLOAT4 clipplane)
{
	//SetWireframe( m_Wireframe );
	//SetWorld( world );
	//SetView( view );
	//SetProj( pCamera->Proj() );
	Render11(m_pD3DSystem, m_NumVerts);
}
//==============================================================================================================================
bool GrassFieldShader::Render11(D3D* d3d, int vertexCount)
{
	bool result;

	// Now render the prepared buffers with the shader.
	//RenderShader(d3d, vertexCount);

	return true;
}
//==============================================================================================================================
