#include "MaterialGBufferShader.h"
#include "ConstantBuffer.h"
#include "CGlobal.h"
#include "LightShadingBuffers.h"
//==============================================================================================================================
MaterialGBufferShader::MaterialGBufferShader(D3D* d3d)
:	ZShadeSandboxShader::Shader(d3d)
{
	Initialize();
}
//==============================================================================================================================
MaterialGBufferShader::MaterialGBufferShader(const MaterialGBufferShader& other)
:	ZShadeSandboxShader::Shader(other)
{
}
//==============================================================================================================================
MaterialGBufferShader::~MaterialGBufferShader()
{
}
//==============================================================================================================================
bool MaterialGBufferShader::Initialize()
{
	ConstantBuffer<ZShadeSandboxLighting::cbMaterialShadingBuffer> packCB(m_pD3DSystem);
	packCB.Initialize(PAD16(sizeof(ZShadeSandboxLighting::cbMaterialShadingBuffer)));
	m_pPackCB = packCB.Buffer();
	
	ConstantBuffer<cbMatrixBuffer> matrixCB(m_pD3DSystem);
	matrixCB.Initialize(PAD16(sizeof(cbMatrixBuffer)));
	m_pMatrixCB = matrixCB.Buffer();
	
	ClearInputLayout();
	LoadPixelShader("MaterialGBufferPS");
	LoadPixelShader("MaterialGBufferWireframePS");
	
	SetInputLayoutDesc("MaterialGBufferShader", ZShadeSandboxMesh::VertexLayout::mesh_layout_pos_normal_tex, 3);
	LoadVertexShader("MaterialGBufferVS");
	AssignVertexShaderLayout("MaterialGBufferShader");
	
	SetInputLayoutDesc("MaterialGBufferShaderInstance", ZShadeSandboxMesh::VertexLayout::mesh_layout_pos_normal_tex_instance, 4);
	LoadVertexShader("MaterialGBufferInstanceVS");
	AssignVertexShaderLayout("MaterialGBufferShaderInstance");

	return true;
}
//==============================================================================================================================
void MaterialGBufferShader::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
}
//==============================================================================================================================
bool MaterialGBufferShader::Render11
(	int indexCount
,	int instanceCount
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
	ID3D11ShaderResourceView* displacementTexture = 0;

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
		displacementTexture
	);

	material->BuildMaterialConstantBuffer(m_pPackCB, mrp.camera->Position(), mrp.clipplane);

	mrp.camera->BuildCameraConstantBuffer(m_pD3DSystem, m_pMatrixCB, mrp.light, mrp.world, mrp.reflection);
	
	ID3D11Buffer* vs_cbs[1] = { m_pMatrixCB };
	m_pD3DSystem->GetDeviceContext()->VSSetConstantBuffers(1, 1, vs_cbs);
	
	ID3D11Buffer* ps_cbs[1] = { m_pPackCB };
	m_pD3DSystem->GetDeviceContext()->PSSetConstantBuffers(0, 1, ps_cbs);
	
	// Assign Texture
	ID3D11ShaderResourceView* ps_srvs[11] = { diffuseArrayTexture, diffuseTexture, ambientTexture, specularTexture, emissiveTexture, normalMapTexture, blendMapTexture, detailMapTexture, alphaMapTexture, shadowMapTexture, ssaoTexture };
	ID3D11SamplerState* ps_samp[1] = { m_pD3DSystem->Linear() };
	
	if (!m_Wireframe)
	{
		m_pD3DSystem->GetDeviceContext()->PSSetSamplers(0, 1, ps_samp);
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 11, ps_srvs);
		
		SwitchTo("MaterialGBufferPS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}
	else
	{
		SwitchTo("MaterialGBufferWireframePS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}

	if (mrp.useInstancing)
	{
		SetInputLayout("MaterialGBufferShaderInstance");
		SwitchTo("MaterialGBufferInstanceVS", ZShadeSandboxShader::EShaderTypes::ST_VERTEX);
	}
	else
	{
		SetInputLayout("MaterialGBufferShader");
		SwitchTo("MaterialGBufferVS", ZShadeSandboxShader::EShaderTypes::ST_VERTEX);
	}

	SetVertexShader();
	SetPixelShader();
	
	//Perform Drawing
	if (mrp.useInstancing)
	{
		RenderIndexInstanced11(indexCount, instanceCount);
	}
	else
	{
		RenderIndex11(indexCount);
	}

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