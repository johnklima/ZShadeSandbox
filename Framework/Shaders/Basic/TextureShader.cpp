#include "TextureShader.h"
#include "ConstantBuffer.h"
#include "CGlobal.h"
//==============================================================================================================================
TextureShader::TextureShader(D3D* d3d)
:	ZShadeSandboxShader::Shader(d3d)
{
	Initialize();
}
//==============================================================================================================================
TextureShader::TextureShader(const TextureShader& other)
:	ZShadeSandboxShader::Shader(other)
{
}
//==============================================================================================================================
TextureShader::~TextureShader()
{
}
//==============================================================================================================================
bool TextureShader::Initialize()
{
	ConstantBuffer<cbShadingBuffer> shadingCB(m_pD3DSystem);
	shadingCB.Initialize(PAD16(sizeof(cbShadingBuffer)));
	m_pShadingCB = shadingCB.Buffer();
	
	ConstantBuffer<cbMatrixBuffer> matrixCB(m_pD3DSystem);
	matrixCB.Initialize(PAD16(sizeof(cbMatrixBuffer)));
	m_pMatrixCB = matrixCB.Buffer();
	
	ClearInputLayout();
	
	SetInputLayoutDesc("textureVertexShader", ZShadeSandboxMesh::VertexLayout::mesh_layout_pos_tex, 2);
	LoadVertexShader("textureVertexShader");
	LoadPixelShader("texturePixelShader");
	LoadPixelShader("TextureWireframePS");
	AssignVertexShaderLayout("textureVertexShader");
	
	return true;
}
//==============================================================================================================================
void TextureShader::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
}
//==============================================================================================================================
void TextureShader::FlipTextureHorizontally(bool flip)
{
	bFlipHorizontally = flip;
	bFlipVertically = !flip;
}
//==============================================================================================================================
bool TextureShader::Render11
(	int indexCount
,	ZShadeSandboxMath::XMMath4 clipplane
,	XMMATRIX world
,	bool reflection
,	Camera* camera
,	ID3D11ShaderResourceView* texture
)
{
	cbShadingBuffer cSB;
	cSB.g_ClipPlane = XMFLOAT4(clipplane.x, clipplane.y, clipplane.z, clipplane.w);
	cSB.padding = XMFLOAT2(0, 0);
	cSB.g_FlipTextureH = (bFlipHorizontally == true) ? 1 : 0;
	cSB.g_FlipTextureV = (bFlipVertically == true) ? 1 : 0;
	// Map shading constants
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res;
		m_pD3DSystem->GetDeviceContext()->Map(m_pShadingCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
		{
			assert(mapped_res.pData);
			*(cbShadingBuffer*)mapped_res.pData = cSB;
		}
		m_pD3DSystem->GetDeviceContext()->Unmap(m_pShadingCB, 0);
	}
	
	camera->BuildCameraConstantBuffer(m_pD3DSystem, m_pMatrixCB, world, reflection);

	ID3D11Buffer* vs_cbs[2] = { m_pShadingCB, m_pMatrixCB };
	m_pD3DSystem->GetDeviceContext()->VSSetConstantBuffers(0, 2, vs_cbs);

	ID3D11ShaderResourceView* ps_srvs[1] = { texture };

	if (!m_Wireframe)
	{
		// Assign Texture
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 1, ps_srvs);

		SwitchTo("texturePixelShader", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}
	else
	{
		SwitchTo("TextureWireframePS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}

	SwitchTo("textureVertexShader", ZShadeSandboxShader::EShaderTypes::ST_VERTEX);

	SetVertexShader();
	SetPixelShader();

	SetInputLayout("textureVertexShader");

	//Perform Drawing
	RenderIndex11(indexCount);

	// Unbind
	if (!m_Wireframe)
	{
		ps_srvs[0] = NULL;
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 1, ps_srvs);
	}

	return true;
}
//==============================================================================================================================