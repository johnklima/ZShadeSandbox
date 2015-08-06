#include "OBJGBufferShader.h"
#include "ConstantBuffer.h"
#include "CGlobal.h"
#include "LightShadingBuffers.h"
//==============================================================================================================================
OBJGBufferShader::OBJGBufferShader(D3D* d3d)
:	ZShadeSandboxShader::Shader(d3d)
{
	Initialize();
}
//==============================================================================================================================
OBJGBufferShader::OBJGBufferShader(const OBJGBufferShader& other)
:	ZShadeSandboxShader::Shader(other)
{
}
//==============================================================================================================================
OBJGBufferShader::~OBJGBufferShader()
{
}
//==============================================================================================================================
bool OBJGBufferShader::Initialize()
{
	ConstantBuffer<ZShadeSandboxLighting::cbMaterialShadingBuffer> packCB(m_pD3DSystem);
	packCB.Initialize(PAD16(sizeof(ZShadeSandboxLighting::cbMaterialShadingBuffer)));
	m_pPackCB = packCB.Buffer();
	
	ConstantBuffer<cbMatrixBuffer> matrixCB(m_pD3DSystem);
	matrixCB.Initialize(PAD16(sizeof(cbMatrixBuffer)));
	m_pMatrixCB = matrixCB.Buffer();
	
	ClearInputLayout();
	SetInputLayoutDesc("OBJGBufferShader", ZShadeSandboxMesh::VertexLayout::mesh_layout_pos_normal_tex, 3);
	LoadVertexShader("OBJGBufferVS");
	LoadPixelShader("OBJGBufferPS");
	LoadPixelShader("OBJGBufferWireframePS");
	AssignVertexShaderLayout("OBJGBufferShader");
	
	return true;
}
//==============================================================================================================================
void OBJGBufferShader::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
}
//==============================================================================================================================
bool OBJGBufferShader::Render11
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

	mrp.camera->BuildCameraConstantBuffer(m_pD3DSystem, m_pMatrixCB, mrp.world, mrp.reflection);
	
	ID3D11Buffer* vs_cbs[1] = { m_pMatrixCB };
	m_pD3DSystem->GetDeviceContext()->VSSetConstantBuffers(1, 1, vs_cbs);
	
	ID3D11Buffer* ps_cbs[1] = { m_pPackCB };
	m_pD3DSystem->GetDeviceContext()->PSSetConstantBuffers(0, 1, ps_cbs);
	
	// Assign Texture
	ID3D11ShaderResourceView* ps_srvs[11] = { diffuseArrayTexture, diffuseTexture, ambientTexture, specularTexture, emissiveTexture, normalMapTexture, blendMapTexture, detailMapTexture, alphaMapTexture, shadowMapTexture, ssaoTexture };
	ID3D11SamplerState* ps_samp[1] = { m_pD3DSystem->Linear() };
	
	if (!m_Wireframe)
	{
		m_pD3DSystem->TurnOffCulling();

		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 11, ps_srvs);
		m_pD3DSystem->GetDeviceContext()->PSSetSamplers(0, 1, ps_samp);

		SwitchTo("OBJGBufferPS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}
	else
	{
		SwitchTo("OBJGBufferWireframePS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}

	SetInputLayout("OBJGBufferShader");
	
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