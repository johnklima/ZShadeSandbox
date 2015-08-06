#include "OBJMeshTessellationShader.h"
#include "ConstantBuffer.h"
#include "LightShadingBuffers.h"
#include "LightManager.h"
//==============================================================================================================================
OBJMeshTessellationShader::OBJMeshTessellationShader(D3D* d3d)
:	ZShadeSandboxShader::Shader(d3d)
{
	Initialize();
}
//==============================================================================================================================
OBJMeshTessellationShader::OBJMeshTessellationShader(const OBJMeshTessellationShader& other)
:	ZShadeSandboxShader::Shader(other)
{
}
//==============================================================================================================================
OBJMeshTessellationShader::~OBJMeshTessellationShader()
{
}
//==============================================================================================================================
bool OBJMeshTessellationShader::Initialize()
{
	ConstantBuffer<ZShadeSandboxLighting::cbMaterialTessellationBuffer> tessellationCB(m_pD3DSystem);
	tessellationCB.Initialize(PAD16(sizeof(ZShadeSandboxLighting::cbMaterialTessellationBuffer)));
	m_pTessellationCB = tessellationCB.Buffer();
	
	ConstantBuffer<ZShadeSandboxLighting::cbMaterialDomainBuffer> domainCB(m_pD3DSystem);
	domainCB.Initialize(PAD16(sizeof(ZShadeSandboxLighting::cbMaterialDomainBuffer)));
	m_pDomainCB = domainCB.Buffer();
	
	ConstantBuffer<ZShadeSandboxLighting::cbMaterialShadingBuffer> objShadingCB(m_pD3DSystem);
	objShadingCB.Initialize(PAD16(sizeof(ZShadeSandboxLighting::cbMaterialShadingBuffer)));
	m_pShadingCB = objShadingCB.Buffer();
	
	ConstantBuffer<ZShadeSandboxLighting::cbLightBuffer> lightCB(m_pD3DSystem);
	lightCB.Initialize(PAD16(sizeof(ZShadeSandboxLighting::cbLightBuffer)));
	m_pLightCB = lightCB.Buffer();

	ConstantBuffer<ZShadeSandboxLighting::cbSunLightBuffer> sunCB(m_pD3DSystem);
	sunCB.Initialize(PAD16(sizeof(ZShadeSandboxLighting::cbSunLightBuffer)));
	m_pSunCB = sunCB.Buffer();

	ClearInputLayout();
	SetInputLayoutDesc("OBJMeshTessellationShader", ZShadeSandboxMesh::VertexLayout::mesh_layout_pos_normal_tex, 3);
	LoadVertexShader("OBJMeshTessellationVS");
	LoadHullShader("OBJMeshTessellationHS");
	LoadDomainShader("OBJMeshTessellationDS");
	LoadPixelShader("OBJMeshTessellationPS");
	LoadPixelShader("OBJMeshTessellationWireframePS");
	AssignVertexShaderLayout("OBJMeshTessellationShader");
	
	return true;
}
//==============================================================================================================================
void OBJMeshTessellationShader::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
}
//==============================================================================================================================
bool OBJMeshTessellationShader::Render
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

	material->BuildMaterialTessellationBuffer(m_pTessellationCB, mrp.camera->Position());
	material->BuildMaterialDomainBuffer(m_pDomainCB, mrp.world, mrp.camera, mrp.clipplane, mrp.reflection);
	material->BuildMaterialConstantBuffer(m_pShadingCB, mrp.camera->Position(), mrp.clipplane);

	ZShadeSandboxLighting::LightManager::Instance()->BuildFinalLightBuffers(m_pLightCB, m_pSunCB);

	// Set the tessellation constant buffer into the Hull Shader
	ID3D11Buffer* hs_cbs[1] = { m_pTessellationCB };
	m_pD3DSystem->GetDeviceContext()->HSSetConstantBuffers(2, 1, hs_cbs);
	
	// Set the domain constant buffer into the Domain Shader
	ID3D11Buffer* ds_cbs[1] = { m_pDomainCB };
	m_pD3DSystem->GetDeviceContext()->DSSetConstantBuffers(3, 1, ds_cbs);
	
	// Set the shading constant buffer into the Pixel Shader
	ID3D11Buffer* ps_cbs[5] = { m_pLightCB, m_pSunCB, m_pTessellationCB, m_pDomainCB, m_pShadingCB };
	m_pD3DSystem->GetDeviceContext()->PSSetConstantBuffers(0, 5, ps_cbs);
	
	ID3D11ShaderResourceView* ps_srvs[11] = { diffuseArrayTexture, diffuseTexture, ambientTexture, specularTexture, emissiveTexture, normalMapTexture, blendMapTexture, detailMapTexture, alphaMapTexture, shadowMapTexture, ssaoTexture };
	ID3D11SamplerState* ps_samp[1] = { m_pD3DSystem->Linear() };

	if (!m_Wireframe)
	{
		// We want to see inside the mesh
		m_pD3DSystem->TurnOffCulling();

		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 11, ps_srvs);
		m_pD3DSystem->GetDeviceContext()->PSSetSamplers(0, 1, ps_samp);

		SwitchTo("OBJMeshTessellationPS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}
	else
	{
		SwitchTo("OBJMeshTessellationWireframePS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}
	
	// Tell the shader what input layout to use
	SetInputLayout("OBJMeshTessellationShader");
	
	// Assign the shaders to render the mesh
	SetVertexShader();
	SetHullShader();
	SetDomainShader();
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
	
	// Set Hull, Domain and Geometry Shaders to null in case they are not needed
	m_pD3DSystem->GetDeviceContext()->HSSetShader(NULL, NULL, 0);
	m_pD3DSystem->GetDeviceContext()->DSSetShader(NULL, NULL, 0);
	m_pD3DSystem->GetDeviceContext()->GSSetShader(NULL, NULL, 0);
	
	return true;
}
//==============================================================================================================================