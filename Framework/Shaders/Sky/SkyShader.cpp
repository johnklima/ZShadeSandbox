#include "SkyShader.h"
#include "ConstantBuffer.h"
#include "TextureManager.h"
//==============================================================================================================================
SkyShader::SkyShader(D3D* d3d)
:	ZShadeSandboxShader::Shader(d3d)
{
	Initialize();
}
//==============================================================================================================================
SkyShader::SkyShader(const SkyShader& other)
:	ZShadeSandboxShader::Shader(other)
{
}
//==============================================================================================================================
SkyShader::~SkyShader()
{
}
//==============================================================================================================================
bool SkyShader::Initialize()
{
	ConstantBuffer<Const_Per_Frame> perFrameCB(m_pD3DSystem);
	perFrameCB.Initialize(PAD16(sizeof(Const_Per_Frame)));
	m_pPerFrameCB = perFrameCB.Buffer();

	ClearInputLayout();

	SetInputLayoutDesc("SkyLayout", ZShadeSandboxMesh::VertexLayout::mesh_layout_pos_normal_tex, 3);
	LoadVertexShader("SkyVS");
	LoadPixelShader("SkyPS");
	LoadPixelShader("SkyWireframePS");
	AssignVertexShaderLayout("SkyLayout");

	return true;
}
//==============================================================================================================================
void SkyShader::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	//ShutdownShader();
}
//==============================================================================================================================
void SkyShader::RenderFunc(int indexCount, ZShadeSandboxMesh::MeshRenderParameters mrp, ZShadeSandboxLighting::ShaderMaterial* material)//ID3D11ShaderResourceView* texture)
{
	// 2D Should be using a sky shader, does not need it
	if (m_pD3DSystem->GetEngineOptions()->m_DimType == DimType::ZSHADE_2D)
		return;
	
	XMMATRIX mv(1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1);
	
	Const_Per_Frame per_frame;
	XMMATRIX W = mv * mrp.world;
	per_frame.g_World = ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(W));
	per_frame.g_View = mrp.view;
	per_frame.g_Proj = mrp.camera->Proj4x4();
	// Consts
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res;
		m_pD3DSystem->GetDeviceContext()->Map(m_pPerFrameCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
		{
			assert(mapped_res.pData);
			*(Const_Per_Frame*)mapped_res.pData = per_frame;
		}
		m_pD3DSystem->GetDeviceContext()->Unmap(m_pPerFrameCB, 0);
	}
	
	ID3D11Buffer* vs_cbs[1] = { m_pPerFrameCB };
	m_pD3DSystem->GetDeviceContext()->VSSetConstantBuffers(0, 1, vs_cbs);

	ID3D11ShaderResourceView* ps_srvs[1] = { material->GetTexture(0)->getTexture11() };
	ID3D11SamplerState* ps_samp[1] = { m_pD3DSystem->Linear() };

	if (!m_Wireframe)
	{
		// Assign Texture
		m_pD3DSystem->GetDeviceContext()->PSSetSamplers(0, 1, ps_samp);
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 1, ps_srvs);

		SwitchTo("SkyPS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}
	else
	{
		SwitchTo("SkyWireframePS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}

	SwitchTo("SkyVS", ZShadeSandboxShader::EShaderTypes::ST_VERTEX);

	SetVertexShader();
	SetPixelShader();

	SetInputLayout("SkyLayout");

	//Perform Drawing
	RenderIndex11(indexCount);

	// Unbind
	if (!m_Wireframe)
	{
		ps_samp[0] = NULL;
		m_pD3DSystem->GetDeviceContext()->PSSetSamplers(0, 1, ps_samp);

		ps_srvs[0] = NULL;
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 1, ps_srvs);
	}
}
//==============================================================================================================================