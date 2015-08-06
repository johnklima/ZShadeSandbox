#include "OBJMeshShader.h"
#include "ConstantBuffer.h"
#include "LightManager.h"
#include "LightShadingBuffers.h"
#include "MeshRenderParameters.h"
//==============================================================================================================================
OBJMeshShader::OBJMeshShader(D3D* d3d)
:	ZShadeSandboxShader::Shader(d3d)
{
	Initialize();
}
//==============================================================================================================================
OBJMeshShader::OBJMeshShader(const OBJMeshShader& other)
:	ZShadeSandboxShader::Shader(other)
{
}
//==============================================================================================================================
OBJMeshShader::~OBJMeshShader()
{
	m_pShadingCB->Release();
	m_pMatrixCB->Release();
}
//==============================================================================================================================
bool OBJMeshShader::Initialize()
{
	ConstantBuffer<ZShadeSandboxLighting::cbMaterialShadingBuffer> objShadingCB(m_pD3DSystem);
	objShadingCB.Initialize(PAD16(sizeof(ZShadeSandboxLighting::cbMaterialShadingBuffer)));
	m_pShadingCB = objShadingCB.Buffer();
	
	ConstantBuffer<cbMatrixBuffer> matrixBufferCB(m_pD3DSystem);
	matrixBufferCB.Initialize(PAD16(sizeof(cbMatrixBuffer)));
	m_pMatrixCB = matrixBufferCB.Buffer();
	
	ConstantBuffer<ZShadeSandboxLighting::cbLightBuffer> lightCB(m_pD3DSystem);
	lightCB.Initialize(PAD16(sizeof(ZShadeSandboxLighting::cbLightBuffer)));
	m_pLightCB = lightCB.Buffer();

	ConstantBuffer<ZShadeSandboxLighting::cbSunLightBuffer> sunCB(m_pD3DSystem);
	sunCB.Initialize(PAD16(sizeof(ZShadeSandboxLighting::cbSunLightBuffer)));
	m_pSunCB = sunCB.Buffer();

	ClearInputLayout();
	SetInputLayoutDesc("OBJMeshShader", ZShadeSandboxMesh::VertexLayout::mesh_layout_pos_normal_tex, 3);
	LoadVertexShader("OBJMeshVS");
	LoadPixelShader("OBJMeshPS");
	LoadPixelShader("OBJMeshWireframePS");
	AssignVertexShaderLayout("OBJMeshShader");
	
	return true;
}
//==============================================================================================================================
void OBJMeshShader::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
}
//==============================================================================================================================
bool OBJMeshShader::Render
(	int startIndex
,	int indexCount
,	ZShadeSandboxMesh::MeshRenderParameters mrp
,	ZShadeSandboxLighting::ShaderMaterial* material
)
{
	ID3D11ShaderResourceView* diffuseArrayTexture = 0;
	ID3D11ShaderResourceView* diffuseTexture = 0;
	ID3D11ShaderResourceView* ambientTexture = 0;
	ID3D11ShaderResourceView* specularTexture = 0;
	ID3D11ShaderResourceView* emissiveTexture = 0;
	ID3D11ShaderResourceView* normalMapTexture = 0;
	ID3D11ShaderResourceView* blendMapTexture = 0;
	ID3D11ShaderResourceView* detailMapTexture = 0;
	ID3D11ShaderResourceView* alphaMapTexture = 0;
	ID3D11ShaderResourceView* shadowMapTexture = 0;
	ID3D11ShaderResourceView* ssaoTexture = 0;
	ID3D11ShaderResourceView* displacementMapTexture = 0;

	material->GetTextures(
		diffuseArrayTexture,
		diffuseTexture,
		ambientTexture,
		specularTexture,
		emissiveTexture,
		normalMapTexture,
		blendMapTexture,
		detailMapTexture,
		alphaMapTexture,
		shadowMapTexture,
		ssaoTexture,
		displacementMapTexture
	);

	material->BuildMaterialConstantBuffer(m_pShadingCB, mrp.camera->Position(), mrp.clipplane);
	
	mrp.camera->BuildCameraConstantBuffer(m_pD3DSystem, m_pMatrixCB, mrp.world, mrp.reflection);
	
	ZShadeSandboxLighting::LightManager::Instance()->BuildFinalLightBuffers(m_pLightCB, m_pSunCB);

	// Set the shading constant buffer and matrix constant buffer into the Vertex Shader
	ID3D11Buffer* vs_cbs[2] = { m_pShadingCB, m_pMatrixCB };
	m_pD3DSystem->GetDeviceContext()->VSSetConstantBuffers(2, 2, vs_cbs);
	
	// Set the shading constant buffer into the Pixel Shader
	ID3D11Buffer* ps_cbs[3] = { m_pLightCB, m_pSunCB, m_pShadingCB };
	m_pD3DSystem->GetDeviceContext()->PSSetConstantBuffers(0, 3, ps_cbs);
	
	ID3D11ShaderResourceView* ps_srvs[11] = { diffuseArrayTexture, diffuseTexture, ambientTexture, specularTexture, emissiveTexture, normalMapTexture, blendMapTexture, detailMapTexture, alphaMapTexture, shadowMapTexture, ssaoTexture };
	ID3D11SamplerState* ps_samp[1] = { m_pD3DSystem->Linear() };
	
	if (!m_Wireframe)
	{
		m_pD3DSystem->TurnOffCulling();

		// Set the texture into the Pixel Shader
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 11, ps_srvs);
		m_pD3DSystem->GetDeviceContext()->PSSetSamplers(0, 1, ps_samp);

		SwitchTo("OBJMeshPS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}
	else
	{
		SwitchTo("OBJMeshWireframePS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}

	// Tell the shader what input layout to use
	SetInputLayout("OBJMeshShader");
	
	// Assign the shaders to render the mesh
	SetVertexShader();
	SetPixelShader();
	
	//Perform Drawing
	RenderIndex11(startIndex, indexCount);

	// Unbind
	if (!m_Wireframe)
	{
		m_pD3DSystem->TurnOnCulling();

		ps_samp[0] = NULL;
		m_pD3DSystem->GetDeviceContext()->PSSetSamplers(0, 1, ps_samp);

		for (int i = 0; i < 11; i++) ps_srvs[i] = NULL;
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 11, ps_srvs);
	}
	
	return true;
}
//==============================================================================================================================