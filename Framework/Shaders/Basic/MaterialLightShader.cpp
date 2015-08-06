#include "MaterialLightShader.h"
#include "ConstantBuffer.h"
#include "CGlobal.h"
#include "DirectionalLight.h"
//==============================================================================================================================
MaterialLightShader::MaterialLightShader(D3D* d3d)
:	ZShadeSandboxShader::Shader(d3d)
{
	Initialize();
}
//==============================================================================================================================
MaterialLightShader::MaterialLightShader(const MaterialLightShader& other)
:	ZShadeSandboxShader::Shader(other)
{
}
//==============================================================================================================================
MaterialLightShader::~MaterialLightShader()
{
}
//==============================================================================================================================
bool MaterialLightShader::Initialize()
{
	ConstantBuffer<ZShadeSandboxLighting::cbAmbientLightBuffer> ambientCB(m_pD3DSystem);
	ambientCB.Initialize(PAD16(sizeof(ZShadeSandboxLighting::cbAmbientLightBuffer)));
	m_pAmbientLightCB = ambientCB.Buffer();
	
	ConstantBuffer<ZShadeSandboxLighting::cbDirectionalLightBuffer> dirCB(m_pD3DSystem);
	dirCB.Initialize(PAD16(sizeof(ZShadeSandboxLighting::cbDirectionalLightBuffer)));
	m_pDirectionalLightCB = dirCB.Buffer();
	
	ConstantBuffer<ZShadeSandboxLighting::cbPointLightBuffer> pointCB(m_pD3DSystem);
	pointCB.Initialize(PAD16(sizeof(ZShadeSandboxLighting::cbPointLightBuffer)));
	m_pPointLightCB = pointCB.Buffer();
	
	ConstantBuffer<ZShadeSandboxLighting::cbSpotLightBuffer> spotCB(m_pD3DSystem);
	spotCB.Initialize(PAD16(sizeof(ZShadeSandboxLighting::cbSpotLightBuffer)));
	m_pSpotLightCB = spotCB.Buffer();
	
	ConstantBuffer<ZShadeSandboxLighting::cbCapsuleLightBuffer> capsuleCB(m_pD3DSystem);
	capsuleCB.Initialize(PAD16(sizeof(ZShadeSandboxLighting::cbCapsuleLightBuffer)));
	m_pCapsuleLightCB = capsuleCB.Buffer();
	
	ConstantBuffer<cbLightTypeBuffer> lightTypeCB(m_pD3DSystem);
	lightTypeCB.Initialize(PAD16(sizeof(cbLightTypeBuffer)));
	m_pLightTypeCB = lightTypeCB.Buffer();
	
	ConstantBuffer<ZShadeSandboxLighting::cbMaterialShadingBuffer> shadingCB(m_pD3DSystem);
	shadingCB.Initialize(PAD16(sizeof(ZShadeSandboxLighting::cbMaterialShadingBuffer)));
	m_pShadingCB = shadingCB.Buffer();
	
	ConstantBuffer<cbMatrixBuffer> matrixCB(m_pD3DSystem);
	matrixCB.Initialize(PAD16(sizeof(cbMatrixBuffer)));
	m_pMatrixCB = matrixCB.Buffer();
	
	ClearInputLayout();
	SetInputLayoutDesc("MaterialLightShader", ZShadeSandboxMesh::VertexLayout::mesh_layout_pos_normal_tex, 3);
	LoadVertexShader("MaterialLightVS");
	LoadPixelShader("MaterialLightPS");
	LoadPixelShader("MaterialLightWireframePS");
	AssignVertexShaderLayout("MaterialLightShader");
	
	SetInputLayoutDesc("MaterialLightShaderInstance", ZShadeSandboxMesh::VertexLayout::mesh_layout_pos_normal_tex_instance, 4);
	LoadVertexShader("MaterialLightInstanceVS");
	AssignVertexShaderLayout("MaterialLightShaderInstance");
	
	return true;
}
//==============================================================================================================================
void MaterialLightShader::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
}
//==============================================================================================================================
bool MaterialLightShader::Render11
(	int indexCount
,	int instanceCount
,	ZShadeSandboxMesh::MeshRenderParameters mrp
,	ZShadeSandboxLighting::ShaderMaterial* material
)
{
	if (mrp.light == 0) return false;
	
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
	
	ZShadeSandboxLighting::LightManager::Instance()->BuildAmbientLightBuffer(m_pAmbientLightCB, mrp.light);
	ZShadeSandboxLighting::LightManager::Instance()->BuildDirectionalLightBuffer(m_pDirectionalLightCB, mrp.light);
	ZShadeSandboxLighting::LightManager::Instance()->BuildPointLightBuffer(m_pPointLightCB, mrp.light);
	ZShadeSandboxLighting::LightManager::Instance()->BuildSpotLightBuffer(m_pSpotLightCB, mrp.light);
	ZShadeSandboxLighting::LightManager::Instance()->BuildCapsuleLightBuffer(m_pCapsuleLightCB, mrp.light);
	
	cbLightTypeBuffer cLTB;
	cLTB.g_UsingAmbientLight = (mrp.light->LightType() == ZShadeSandboxLighting::ELightType::eAmbient) ? 1 : 0;
	cLTB.g_UsingDirectionalLight = (mrp.light->LightType() == ZShadeSandboxLighting::ELightType::eDirectional) ? 1 : 0;
	cLTB.g_UsingPointLight = (mrp.light->LightType() == ZShadeSandboxLighting::ELightType::ePoint) ? 1 : 0;
	cLTB.g_UsingSpotLight = (mrp.light->LightType() == ZShadeSandboxLighting::ELightType::eSpot) ? 1 : 0;
	cLTB.lighttypepadding = XMFLOAT3(0, 0, 0);
	cLTB.g_UsingCapsuleLight = (mrp.light->LightType() == ZShadeSandboxLighting::ELightType::eCapsule) ? 1 : 0;
	// Map the light type constants
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res2;
		m_pD3DSystem->GetDeviceContext()->Map(m_pLightTypeCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res2);
		{
			assert(mapped_res2.pData);
			*(cbLightTypeBuffer*)mapped_res2.pData = cLTB;
		}
		m_pD3DSystem->GetDeviceContext()->Unmap(m_pLightTypeCB, 0);
	}
	
	mrp.camera->BuildCameraConstantBuffer(m_pD3DSystem, m_pMatrixCB, mrp.light, mrp.world, mrp.reflection);
	
	ID3D11Buffer* cbs[8] = {
		m_pShadingCB,
		m_pLightTypeCB,
		m_pAmbientLightCB,
		m_pDirectionalLightCB,
		m_pPointLightCB,
		m_pSpotLightCB,
		m_pCapsuleLightCB,
		m_pMatrixCB
	};
	
	//ID3D11Buffer* vs_cbs[1] = { m_pMatrixCB };
	m_pD3DSystem->GetDeviceContext()->VSSetConstantBuffers(2, 8, cbs);
	
	//ID3D11Buffer* ps_cbs[7] = { m_pShadingCB, m_pLightTypeCB, m_pAmbientLightCB, m_pDirectionalLightCB, m_pPointLightCB, m_pSpotLightCB, m_pCapsuleLightCB };
	m_pD3DSystem->GetDeviceContext()->PSSetConstantBuffers(2, 8, cbs);
	
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
		
		SwitchTo("MaterialLightPS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}
	else
	{
		SwitchTo("MaterialLightWireframePS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}
	
	if (mrp.useInstancing)
	{
		SetInputLayout("MaterialLightShaderInstance");
		SwitchTo("MaterialLightInstanceVS", ZShadeSandboxShader::EShaderTypes::ST_VERTEX);
	}
	else
	{
		SetInputLayout("MaterialLightShader");
		SwitchTo("MaterialLightVS", ZShadeSandboxShader::EShaderTypes::ST_VERTEX);
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