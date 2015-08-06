#include "MaterialShader.h"
#include "ConstantBuffer.h"
#include "DirectionalLight.h"
#include "CGlobal.h"
//==============================================================================================================================
ZShadeSandboxLighting::Light* MaterialShader::mDefaultLight = 0;
//==============================================================================================================================
MaterialShader::MaterialShader(D3D* d3d)
:	ZShadeSandboxShader::Shader(d3d)
{
	Initialize();
	
	if (MaterialShader::mDefaultLight == 0)
	{
		MaterialShader::mDefaultLight = new ZShadeSandboxLighting::DirectionalLight();
		MaterialShader::mDefaultLight->LightType() = ZShadeSandboxLighting::ELightType::eDirectional;
		MaterialShader::mDefaultLight->DiffuseColor() = XMFLOAT4(0.5f, 0.2f, 1.2f, 1.0f);
		MaterialShader::mDefaultLight->AmbientColor() = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		MaterialShader::mDefaultLight->SpecularColor() = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
		MaterialShader::mDefaultLight->Position() = XMFLOAT3(0, 0, 0);
		MaterialShader::mDefaultLight->Direction() = XMFLOAT3(0.575f, -0.735f, 0.5735f);
		MaterialShader::mDefaultLight->SetLens(1, 100);
	}
}
//==============================================================================================================================
MaterialShader::MaterialShader(const MaterialShader& other)
:	ZShadeSandboxShader::Shader(other)
{
}
//==============================================================================================================================
MaterialShader::~MaterialShader()
{
}
//==============================================================================================================================
bool MaterialShader::Initialize()
{
	ConstantBuffer<ZShadeSandboxLighting::cbMaterialShadingBuffer> shadingCB(m_pD3DSystem);
	shadingCB.Initialize(PAD16(sizeof(ZShadeSandboxLighting::cbMaterialShadingBuffer)));
	m_pShadingCB = shadingCB.Buffer();
	
	ConstantBuffer<cbMatrixBufferLight> matrixCB(m_pD3DSystem);
	matrixCB.Initialize(PAD16(sizeof(cbMatrixBufferLight)));
	m_pMatrixCB = matrixCB.Buffer();
	
	ConstantBuffer<ZShadeSandboxLighting::cbLightBuffer> lightCB(m_pD3DSystem);
	lightCB.Initialize(PAD16(sizeof(ZShadeSandboxLighting::cbLightBuffer)));
	m_pLightCB = lightCB.Buffer();
	
	ConstantBuffer<ZShadeSandboxLighting::cbSunLightBuffer> sunCB(m_pD3DSystem);
	sunCB.Initialize(PAD16(sizeof(ZShadeSandboxLighting::cbSunLightBuffer)));
	m_pSunCB = sunCB.Buffer();
	
	ClearInputLayout();
	
	LoadPixelShader("MaterialShaderPS");
	LoadPixelShader("MaterialShaderWireframePS");

	SetInputLayoutDesc("MaterialShader", ZShadeSandboxMesh::VertexLayout::mesh_layout_pos_normal_tex, 3);
	LoadVertexShader("MaterialShaderVS");
	AssignVertexShaderLayout("MaterialShader");
	
	SetInputLayoutDesc("MaterialShaderInstance", ZShadeSandboxMesh::VertexLayout::mesh_layout_pos_normal_tex_instance, 4);
	LoadVertexShader("MaterialShaderInstanceVS");
	AssignVertexShaderLayout("MaterialShaderInstance");

	return true;
}
//==============================================================================================================================
void MaterialShader::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
}
//==============================================================================================================================
bool MaterialShader::Render11
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
	ID3D11ShaderResourceView* displacementMapTexture = 0;

	if (material == 0) return false;

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
	
	material->fBlendAmount = mrp.blendAmount;

	material->BuildMaterialConstantBuffer(m_pShadingCB, mrp.camera->Position(), mrp.clipplane);

	// Use the default light if there is no light
	if (mrp.light == 0)
	{
		MaterialShader::mDefaultLight->Update();
		mrp.light = MaterialShader::mDefaultLight;
	}

	mrp.camera->BuildCameraConstantBuffer(m_pD3DSystem, m_pMatrixCB, mrp.light, mrp.world, mrp.reflection);
	
	if (m_pD3DSystem->GetEngineOptions()->m_DimType == DimType::ZSHADE_3D)
		ZShadeSandboxLighting::LightManager::Instance()->BuildFinalLightBuffers(m_pLightCB, m_pSunCB);
	else
	{
		material->bEnableLighting = false;
		material->bHasDetailMapTexture = false;
	}
	
	ID3D11Buffer* vs_cbs[2] = { m_pShadingCB, m_pMatrixCB };
	m_pD3DSystem->GetDeviceContext()->VSSetConstantBuffers(2, 2, vs_cbs);
	
	ID3D11Buffer* ps_cbs[3] = { m_pLightCB, m_pSunCB, m_pShadingCB };
	m_pD3DSystem->GetDeviceContext()->PSSetConstantBuffers(0, 3, ps_cbs);
	
	ID3D11ShaderResourceView* ps_srvs[11] = { diffuseArrayTexture, diffuseTexture, ambientTexture, specularTexture, emissiveTexture, normalMapTexture, blendMapTexture, detailMapTexture, alphaMapTexture, shadowMapTexture, ssaoTexture };
	ID3D11SamplerState* ps_samp[2] = { m_pD3DSystem->Point(), m_pD3DSystem->Linear() };
	
	ID3D11ShaderResourceView* vs_srvs[1] = { displacementMapTexture };

	if (!m_Wireframe)
	{
		// Assign Texture
		
		m_pD3DSystem->GetDeviceContext()->VSSetSamplers(0, 2, ps_samp);
		m_pD3DSystem->GetDeviceContext()->PSSetSamplers(0, 2, ps_samp);
		
		m_pD3DSystem->GetDeviceContext()->VSSetShaderResources(11, 1, vs_srvs);
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 11, ps_srvs);

		SwitchTo("MaterialShaderPS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}
	else
	{
		SwitchTo("MaterialShaderWireframePS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}
	
	if (mrp.useInstancing)
	{
		SetInputLayout("MaterialShaderInstance");
		SwitchTo("MaterialShaderInstanceVS", ZShadeSandboxShader::EShaderTypes::ST_VERTEX);
	}
	else
	{
		SetInputLayout("MaterialShader");
		SwitchTo("MaterialShaderVS", ZShadeSandboxShader::EShaderTypes::ST_VERTEX);
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
		ps_samp[0] = NULL;
		ps_samp[1] = NULL;
		m_pD3DSystem->GetDeviceContext()->VSSetSamplers(0, 2, ps_samp);
		m_pD3DSystem->GetDeviceContext()->PSSetSamplers(0, 2, ps_samp);
		
		vs_srvs[0] = NULL;
		m_pD3DSystem->GetDeviceContext()->VSSetShaderResources(11, 1, vs_srvs);

		for (int i = 0; i < 11; i++) ps_srvs[i] = NULL;
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 11, ps_srvs);
	}

	return true;
}
//==============================================================================================================================