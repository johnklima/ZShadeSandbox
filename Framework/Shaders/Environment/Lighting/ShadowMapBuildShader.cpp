#include "ShadowMapBuildShader.h"
#include "ConstantBuffer.h"
//==============================================================================================================================
ShadowMapBuildShader::ShadowMapBuildShader(D3D* d3d)
:	ZShadeSandboxShader::Shader(d3d)
{
	Initialize();
}
//==============================================================================================================================
ShadowMapBuildShader::ShadowMapBuildShader(const ShadowMapBuildShader& other)
:	ZShadeSandboxShader::Shader(other)
{
}
//==============================================================================================================================
ShadowMapBuildShader::~ShadowMapBuildShader()
{
}
//==============================================================================================================================
bool ShadowMapBuildShader::Initialize()
{
	ConstantBuffer<cbShadowMapConst> shadowMapCB(m_pD3DSystem);
	shadowMapCB.Initialize(PAD16(sizeof(cbShadowMapConst)));
	m_pShadowMapCB = shadowMapCB.Buffer();
	
	ClearInputLayout();
	//SetInputLayoutDesc("ShadowMapBuildShader", ZShadeSandboxMesh::VertexLayout::mesh_layout_pos, 1);
	SetInputLayoutDesc("ShadowMapBuildShader", ZShadeSandboxMesh::VertexLayout::mesh_layout_pos_normal_tex, 3);
	
	LoadVertexShader("ShadowMapBuilderVS");
	LoadPixelShader("ShadowMapBuilderPS");
	
	AssignVertexShaderLayout("ShadowMapBuildShader");
	
	return true;
}
//==============================================================================================================================
void ShadowMapBuildShader::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
}
//==============================================================================================================================
bool ShadowMapBuildShader::Render(int indexCount, XMMATRIX world, LightCamera* lightCamera)
{
	cbShadowMapConst cSMB;
	
	//m_pD3DSystem->SetRasterizerState(m_pD3DSystem->RSDepth());

	cSMB.g_World = ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(world));
	cSMB.g_LightView = lightCamera->LightView4x4();
	cSMB.g_LightProj = lightCamera->LightProj4x4();
	
	// Map matrix constants
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res;
		m_pD3DSystem->GetDeviceContext()->Map(m_pShadowMapCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
		{
			assert(mapped_res.pData);
			*(cbShadowMapConst*)mapped_res.pData = cSMB;
		}
		m_pD3DSystem->GetDeviceContext()->Unmap(m_pShadowMapCB, 0);
	}
	
	// Set the shadow map constant buffer into the Vertex Shader
	ID3D11Buffer* vs_cbs[1] = { m_pShadowMapCB };
	m_pD3DSystem->GetDeviceContext()->VSSetConstantBuffers(0, 1, vs_cbs);
	
	// Tell the shader what input layout to use
	SetInputLayout("ShadowMapBuildShader");
	
	// Assign the shaders to render the mesh
	SetVertexShader();
	//SetPixelShader();
	
	m_pD3DSystem->GetDeviceContext()->PSSetShader(NULL, NULL, 0);

	//Perform Drawing
	RenderIndex11(indexCount);
	
	return true;
}
//==============================================================================================================================