#include "BlurShader.h"
#include "ConstantBuffer.h"
//==============================================================================================================================
BlurShader::BlurShader(D3D* d3d)
:	ZShadeSandboxShader::Shader(d3d), m_HorizontalBlur(true)
{
	Initialize();
}
//==============================================================================================================================
BlurShader::BlurShader(const BlurShader& other)
:	ZShadeSandboxShader::Shader(other)
{
}
//==============================================================================================================================
BlurShader::~BlurShader()
{
}
//==============================================================================================================================
bool BlurShader::Initialize()
{
	ConstantBuffer<CB_Const> perCBConst(m_pD3DSystem);
	perCBConst.Initialize(PAD16(sizeof(CB_Const)));
	m_pCBConst = perCBConst.Buffer();

	ConstantBuffer<cbMatrixBuffer> perObjectCB(m_pD3DSystem);
	perObjectCB.Initialize(PAD16(sizeof(cbMatrixBuffer)));
	m_pPerObjectCB = perObjectCB.Buffer();

	ClearInputLayout();

	SetInputLayoutDesc("BlurVS", ZShadeSandboxMesh::VertexLayout::mesh_layout_pos_tex, 2);

	LoadVertexShader("HorizontalBlurVS");
	LoadVertexShader("VerticalBlurVS");
	LoadPixelShader("HorizontalBlurPS");
	LoadPixelShader("VerticalBlurPS");

	SwitchTo("HorizontalBlurVS", ZShadeSandboxShader::EShaderTypes::ST_VERTEX);

	AssignVertexShaderLayout("BlurVS");

	return true;
}
//==============================================================================================================================
void BlurShader::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	ShutdownShader();
}
//==============================================================================================================================
bool BlurShader::Render11(int indexCount, Camera* camera, float w, float h,
	ID3D11ShaderResourceView* horizontalBlurTexture,
	ID3D11ShaderResourceView* verticalBlurTexture)
{
	CB_Const per_cb_const;
	per_cb_const.g_Width = w;
	per_cb_const.g_Height = h;

	// Consts
	D3D11_MAPPED_SUBRESOURCE mapped_res;
	m_pD3DSystem->GetDeviceContext()->Map(m_pCBConst, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
	{
		assert(mapped_res.pData);
		*(CB_Const*)mapped_res.pData = per_cb_const;
	}
	m_pD3DSystem->GetDeviceContext()->Unmap(m_pCBConst, 0);

	/*Const_Per_Frame per_object;

	ZShadeSandboxMath::XMMatrix matProj;

	per_object.g_World = camera->World();
	per_object.g_View = camera->View();
	per_object.g_Proj = camera->Proj();

	D3D11_MAPPED_SUBRESOURCE mapped_res2;
	m_pD3DSystem->GetDeviceContext()->Map(m_pPerObjectCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res2);
	{
		assert(mapped_res2.pData);
		*(Const_Per_Frame*)mapped_res2.pData = per_object;
	}
	m_pD3DSystem->GetDeviceContext()->Unmap(m_pPerObjectCB, 0);*/

	camera->BuildCameraConstantBuffer(m_pD3DSystem, m_pPerObjectCB, camera->World(), false);

	ID3D11Buffer* vs_cbs[2] = { m_pCBConst, m_pPerObjectCB };
	m_pD3DSystem->GetDeviceContext()->VSSetConstantBuffers(0, 2, vs_cbs);

	if (m_HorizontalBlur)
	{
		SwitchTo("HorizontalBlurVS", ZShadeSandboxShader::EShaderTypes::ST_VERTEX);
		SwitchTo("HorizontalBlurPS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);

		ID3D11ShaderResourceView* ps_srvs[1] = { horizontalBlurTexture };

		// Assign Texture
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 1, ps_srvs);

		SetVertexShader();
		SetPixelShader();

		SetInputLayout("BlurVS");
		//Perform Drawing
		RenderIndex11(indexCount);

		// Unbind
		if (!m_Wireframe)
		{
			ps_srvs[0] = NULL;
			m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 1, ps_srvs);
		}
	}
	else
	{
		SwitchTo("VerticalBlurVS", ZShadeSandboxShader::EShaderTypes::ST_VERTEX);
		SwitchTo("VerticalBlurPS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);

		ID3D11ShaderResourceView* ps_srvs[1] = { verticalBlurTexture };

		// Assign Texture
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 1, ps_srvs);

		SetVertexShader();
		SetPixelShader();

		SetInputLayout("BlurVS");
		//Perform Drawing
		RenderIndex11(indexCount);

		// Unbind
		if (!m_Wireframe)
		{
			ps_srvs[0] = NULL;
			m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 1, ps_srvs);
		}
	}

	return true;
}
//==============================================================================================================================
bool BlurShader::InitializeShader11()
{
	/*
	HorizontalBlurTech = mFX->GetTechniqueByName("HorizontalBlurTech");
	VerticalBlurTech   = mFX->GetTechniqueByName("VerticalBlurTech");
	
	pScreenWidth       = mFX->GetVariableByName("g_ScreenWidth")->AsScalar();
	pScreenHeight      = mFX->GetVariableByName("g_ScreenHeight")->AsScalar();
	
	pWorld             = mFX->GetVariableByName("g_World")->AsMatrix();
	pView              = mFX->GetVariableByName("g_View")->AsMatrix();
	pProj              = mFX->GetVariableByName("g_Proj")->AsMatrix();
	
	pHorizontalBlurMap = mFX->GetVariableByName("g_blurHorizontalTexture")->AsShaderResource();
	pVerticalBlurMap   = mFX->GetVariableByName("g_blurVerticalTexture")->AsShaderResource();

	D3DX11_PASS_DESC passDesc;
	HorizontalBlurTech->GetPassByIndex(0)->GetDesc(&passDesc);
	d3d->GetDevice11()->CreateInputLayout(Vertex::mesh_layout_Vertex_tex, 2, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &m_layout11);
	*/

	return true;
}
//==============================================================================================================================
void BlurShader::ShutdownShader()
{
	//SAFE_RELEASE(m_layout11);
}
//==============================================================================================================================
void BlurShader::RenderShader(D3D* d3d, int indexCount)
{
	/*
	d3d->GetDeviceContext()->IASetInputLayout(m_layout11);

	if (m_HorizontalBlur)
	{
		D3DX11_TECHNIQUE_DESC techDesc;
		HorizontalBlurTech->GetDesc(&techDesc);

		ID3DX11EffectPass* pass = 0;
		for (UINT i = 0; i < techDesc.Passes; ++i)
		{
			pass = HorizontalBlurTech->GetPassByIndex(i);
			pass->Apply(0, d3d->GetDeviceContext());
			d3d->GetDeviceContext()->DrawIndexed(indexCount, 0, 0);
		}
	}
	else
	{
		D3DX11_TECHNIQUE_DESC techDesc;
		VerticalBlurTech->GetDesc(&techDesc);

		ID3DX11EffectPass* pass = 0;
		for (UINT i = 0; i < techDesc.Passes; ++i)
		{
			pass = VerticalBlurTech->GetPassByIndex(i);
			pass->Apply(0, d3d->GetDeviceContext());
			d3d->GetDeviceContext()->DrawIndexed(indexCount, 0, 0);
		}
	}
	*/
}
//==============================================================================================================================
