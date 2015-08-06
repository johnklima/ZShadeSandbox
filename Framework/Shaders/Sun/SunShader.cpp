#include "SunShader.h"
#include "ConstantBuffer.h"
#include "TextureManager.h"
using ZShadeSandboxShader::SunShader;
//==============================================================================================================================
SunShader::SunShader(D3D* d3d)
:	ZShadeSandboxShader::Shader(d3d)
{
	Initialize();
}
//==============================================================================================================================
SunShader::SunShader(const SunShader& other)
:	ZShadeSandboxShader::Shader(other)
{
}
//==============================================================================================================================
SunShader::~SunShader()
{
}
//==============================================================================================================================
bool SunShader::Initialize()
{
	ConstantBuffer<cbSunConst> sunCB(m_pD3DSystem);
	sunCB.Initialize(PAD16(sizeof(cbSunConst)));
	m_pSunCB = sunCB.Buffer();
	
	ConstantBuffer<Const_Per_Frame> perFrameCB(m_pD3DSystem);
	perFrameCB.Initialize(PAD16(sizeof(Const_Per_Frame)));
	m_pPerFrameCB = perFrameCB.Buffer();

	ClearInputLayout();

	SetInputLayoutDesc("SunVS", ZShadeSandboxMesh::VertexLayout::mesh_layout_pos_tex, 2);
	
	LoadVertexShader("SunVS");
	LoadPixelShader("SunPS");
	LoadPixelShader("SunWireframePS");

	AssignVertexShaderLayout("SunVS");

	return true;
}
//==============================================================================================================================
void SunShader::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	//ShutdownShader();
}
//==============================================================================================================================
bool SunShader::Render11(int indexCount, float alpha, float glow, Camera* camera, ID3D11ShaderResourceView* textureSRV)
{
	// 2D Should be using a sun shader, does not need it
	if (m_pD3DSystem->GetEngineOptions()->m_DimType == DimType::ZSHADE_2D)
		return false;
	
	Const_Per_Frame per_frame;

	/*if (m_UseCustomWorld)
	{
		per_frame.g_World = ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(mWorld.Get()));
	}
	else
		per_frame.g_World = ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(camera->World()));*/
	
	//XMMATRIX WVP = world * camera->View() * camera->Proj();
	per_frame.g_View = camera->View4x4();
	per_frame.g_Proj = camera->Proj4x4();
	// Map constant buffer for WVP
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res;
		m_pD3DSystem->GetDeviceContext()->Map(m_pPerFrameCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
		{
			assert(mapped_res.pData);
			*(Const_Per_Frame*)mapped_res.pData = per_frame;
		}
		m_pD3DSystem->GetDeviceContext()->Unmap(m_pPerFrameCB, 0);
	}
	
	cbSunConst cSC;
	cSC.padding = XMFLOAT2(0, 0);
	cSC.g_Alpha = alpha;
	cSC.g_Glow = glow;
	// Map constant buffer for the sun parameters
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res;
		m_pD3DSystem->GetDeviceContext()->Map(m_pSunCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
		{
			assert(mapped_res.pData);
			*(cbSunConst*)mapped_res.pData = cSC;
		}
		m_pD3DSystem->GetDeviceContext()->Unmap(m_pSunCB, 0);
	}
	
	
	ID3D11Buffer* vs_cbs[1] = { m_pPerFrameCB };
	m_pD3DSystem->GetDeviceContext()->VSSetConstantBuffers(0, 1, vs_cbs);
	
	ID3D11Buffer* ps_cbs[1] = { m_pSunCB };
	m_pD3DSystem->GetDeviceContext()->PSSetConstantBuffers(0, 1, ps_cbs);
	
	ID3D11ShaderResourceView* ps_srvs[1] = { textureSRV };

	if (!m_Wireframe)
	{
		// Assign Texture
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 1, ps_srvs);

		SwitchTo("SunPS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}
	else
	{
		SwitchTo("SunWireframePS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}

	SwitchTo("SunVS", ZShadeSandboxShader::EShaderTypes::ST_VERTEX);

	SetInputLayout("SunVS");

	SetVertexShader();
	SetPixelShader();

	//Perform Drawing
	RenderIndex11(indexCount);

	// Unbind
	if (!m_Wireframe)
	{
		ps_srvs[0] = NULL;
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 1, ps_srvs);
	}
}
//==============================================================================================================================
