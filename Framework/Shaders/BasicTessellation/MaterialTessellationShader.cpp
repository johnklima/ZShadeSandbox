#include "MaterialTessellationShader.h"
#include "ConstantBuffer.h"
#include "CGlobal.h"
//==============================================================================================================================
MaterialTessellationShader::MaterialTessellationShader(D3D* d3d, ZShadeSandboxLighting::EMaterialTessellationType::Type type)
:	ZShadeSandboxShader::Shader(d3d)
,	mType(type)
{
	Initialize();
}
//==============================================================================================================================
MaterialTessellationShader::MaterialTessellationShader(const MaterialTessellationShader& other)
:	ZShadeSandboxShader::Shader(other)
{
}
//==============================================================================================================================
MaterialTessellationShader::~MaterialTessellationShader()
{
}
//==============================================================================================================================
bool MaterialTessellationShader::Initialize()
{
	ConstantBuffer<ZShadeSandboxLighting::cbMaterialTessellationBuffer> tessellationCB(m_pD3DSystem);
	tessellationCB.Initialize(PAD16(sizeof(ZShadeSandboxLighting::cbMaterialTessellationBuffer)));
	m_pTessellationCB = tessellationCB.Buffer();
	
	ConstantBuffer<ZShadeSandboxLighting::cbMaterialDomainBuffer> domainCB(m_pD3DSystem);
	domainCB.Initialize(PAD16(sizeof(ZShadeSandboxLighting::cbMaterialDomainBuffer)));
	m_pDomainCB = domainCB.Buffer();
	
	ConstantBuffer<ZShadeSandboxLighting::cbMaterialShadingBuffer> shadingCB(m_pD3DSystem);
	shadingCB.Initialize(PAD16(sizeof(ZShadeSandboxLighting::cbMaterialShadingBuffer)));
	m_pShadingCB = shadingCB.Buffer();
	
	ConstantBuffer<ZShadeSandboxLighting::cbLightBuffer> lightCB(m_pD3DSystem);
	lightCB.Initialize(PAD16(sizeof(ZShadeSandboxLighting::cbLightBuffer)));
	m_pLightCB = lightCB.Buffer();
	
	ConstantBuffer<ZShadeSandboxLighting::cbSunLightBuffer> sunCB(m_pD3DSystem);
	sunCB.Initialize(PAD16(sizeof(ZShadeSandboxLighting::cbSunLightBuffer)));
	m_pSunCB = sunCB.Buffer();
	
	ClearInputLayout();
	SetInputLayoutDesc("MaterialTessellationShader", ZShadeSandboxMesh::VertexLayout::mesh_layout_pos_normal_tex, 3);
	switch (mType)
	{
		case ZShadeSandboxLighting::EMaterialTessellationType::Type::eQuad:
		{
			LoadVertexShader("QuadMaterialTessellationVS");
			LoadHullShader("QuadMaterialTessellationHS");
			LoadDomainShader("QuadMaterialTessellationDS");
			LoadPixelShader("QuadMaterialTessellationPS");
			LoadPixelShader("QuadMaterialTessellationWireframePS");
		}
		break;
		case ZShadeSandboxLighting::EMaterialTessellationType::Type::eTri:
		{
			LoadVertexShader("TriMaterialTessellationVS");
			LoadHullShader("TriMaterialTessellationHS");
			LoadDomainShader("TriMaterialTessellationDS");
			LoadPixelShader("TriMaterialTessellationPS");
			LoadPixelShader("TriMaterialTessellationWireframePS");
		}
		break;
	}
	AssignVertexShaderLayout("MaterialTessellationShader");
	
	// Instancing
	SetInputLayoutDesc("MaterialTessellationShaderInstance", ZShadeSandboxMesh::VertexLayout::mesh_layout_pos_normal_tex_instance, 4);
	switch (mType)
	{
		case ZShadeSandboxLighting::EMaterialTessellationType::Type::eQuad:
		{
			LoadVertexShader("QuadMaterialTessellationInstanceVS");
		}
		break;
		case ZShadeSandboxLighting::EMaterialTessellationType::Type::eTri:
		{
			LoadVertexShader("TriMaterialTessellationInstanceVS");
		}
		break;
	}
	AssignVertexShaderLayout("MaterialTessellationShaderInstance");
	
	return true;
}
//==============================================================================================================================
void MaterialTessellationShader::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
}
//==============================================================================================================================
bool MaterialTessellationShader::Render11
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
	
	material->BuildMaterialTessellationBuffer(m_pTessellationCB, mrp.camera->Position());
	material->BuildMaterialDomainBuffer(m_pDomainCB, mrp.world, mrp.camera, mrp.clipplane, mrp.reflection);
	material->BuildMaterialConstantBuffer(m_pShadingCB, mrp.camera->Position(), mrp.clipplane);
	
	ZShadeSandboxLighting::LightManager::Instance()->BuildFinalLightBuffers(m_pLightCB, m_pSunCB);
	
	ID3D11Buffer* vs_cbs[1] = { m_pShadingCB };
	m_pD3DSystem->GetDeviceContext()->VSSetConstantBuffers(4, 1, vs_cbs);

	// Set the tessellation constant buffer into the Hull Shader
	ID3D11Buffer* hs_cbs[1] = { m_pTessellationCB };
	m_pD3DSystem->GetDeviceContext()->HSSetConstantBuffers(2, 1, hs_cbs);
	
	// Set the matrix constant buffer into the Domain Shader
	ID3D11Buffer* ds_cbs[2] = { m_pDomainCB, m_pShadingCB };
	m_pD3DSystem->GetDeviceContext()->DSSetConstantBuffers(3, 2, ds_cbs);
	
	ID3D11Buffer* ps_cbs[5] = { m_pLightCB, m_pSunCB, m_pTessellationCB, m_pDomainCB, m_pShadingCB };
	m_pD3DSystem->GetDeviceContext()->PSSetConstantBuffers(0, 5, ps_cbs);

	ID3D11ShaderResourceView* ps_srvs[11] = { diffuseArrayTexture, diffuseTexture, ambientTexture, specularTexture, emissiveTexture, normalMapTexture, blendMapTexture, detailMapTexture, alphaMapTexture, shadowMapTexture, ssaoTexture };
	ID3D11SamplerState* ps_samp[2] = { m_pD3DSystem->Point(), m_pD3DSystem->Linear() };

	ID3D11ShaderResourceView* disp_srvs[1] = { displacementMapTexture };

	if (!m_Wireframe)
	{
		// Assign Texture
		
		m_pD3DSystem->GetDeviceContext()->VSSetSamplers(0, 2, ps_samp);
		m_pD3DSystem->GetDeviceContext()->DSSetSamplers(0, 2, ps_samp);
		m_pD3DSystem->GetDeviceContext()->PSSetSamplers(0, 2, ps_samp);
		
		m_pD3DSystem->GetDeviceContext()->VSSetShaderResources(11, 1, disp_srvs);
		m_pD3DSystem->GetDeviceContext()->DSSetShaderResources(11, 1, disp_srvs);
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 11, ps_srvs);
		
		switch (mType)
		{
			case ZShadeSandboxLighting::EMaterialTessellationType::Type::eQuad:
				SwitchTo("QuadMaterialTessellationPS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
			break;
			case ZShadeSandboxLighting::EMaterialTessellationType::Type::eTri:
				SwitchTo("TriMaterialTessellationPS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
			break;
		}
	}
	else
	{
		switch (mType)
		{
			case ZShadeSandboxLighting::EMaterialTessellationType::Type::eQuad:
				SwitchTo("QuadMaterialTessellationWireframePS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
			break;
			case ZShadeSandboxLighting::EMaterialTessellationType::Type::eTri:
				SwitchTo("TriMaterialTessellationWireframePS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
			break;
		}
	}
	
	if (mrp.useInstancing)
	{
		SetInputLayout("MaterialTessellationShaderInstance");
		switch (mType)
		{
			case ZShadeSandboxLighting::EMaterialTessellationType::Type::eQuad:
				SwitchTo("QuadMaterialTessellationInstanceVS", ZShadeSandboxShader::EShaderTypes::ST_VERTEX);
			break;
			case ZShadeSandboxLighting::EMaterialTessellationType::Type::eTri:
				SwitchTo("TriMaterialTessellationInstanceVS", ZShadeSandboxShader::EShaderTypes::ST_VERTEX);
			break;
		}
	}
	else
	{
		SetInputLayout("MaterialTessellationShader");
		switch (mType)
		{
			case ZShadeSandboxLighting::EMaterialTessellationType::Type::eQuad:
				SwitchTo("QuadMaterialTessellationVS", ZShadeSandboxShader::EShaderTypes::ST_VERTEX);
			break;
			case ZShadeSandboxLighting::EMaterialTessellationType::Type::eTri:
				SwitchTo("TriMaterialTessellationVS", ZShadeSandboxShader::EShaderTypes::ST_VERTEX);
			break;
		}
	}
	
	SetVertexShader();
	SetHullShader();
	SetDomainShader();
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
		m_pD3DSystem->GetDeviceContext()->VSSetSamplers(0, 2, ps_samp);
		m_pD3DSystem->GetDeviceContext()->DSSetSamplers(0, 2, ps_samp);
		m_pD3DSystem->GetDeviceContext()->PSSetSamplers(0, 2, ps_samp);
		
		disp_srvs[0] = NULL;
		m_pD3DSystem->GetDeviceContext()->VSSetShaderResources(11, 1, disp_srvs);
		m_pD3DSystem->GetDeviceContext()->DSSetShaderResources(11, 1, disp_srvs);

		for (int i = 0; i < 11; i++) ps_srvs[i] = NULL;
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 11, ps_srvs);
	}
	
	// Set Hull, Domain and Geometry Shaders to null in case they are not needed
	m_pD3DSystem->GetDeviceContext()->HSSetShader(NULL, NULL, 0);
	m_pD3DSystem->GetDeviceContext()->DSSetShader(NULL, NULL, 0);
	m_pD3DSystem->GetDeviceContext()->GSSetShader(NULL, NULL, 0);

	return true;
}
//==============================================================================================================================