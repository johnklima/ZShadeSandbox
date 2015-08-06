#include "PostProcessColor.h"
#include "ConstantBuffer.h"
//==================================================================================================================================
//==================================================================================================================================
PostProcessColor::PostProcessColor(D3D* d3d)
:   PostProcess(d3d)
{
	//sPixelShaderRenderName = "PostProcessColorPS";
	
	mType = EPostProcessTypes::Type::eColor;
	
	ConstantBuffer<cbColorBuffer> colorCB(mD3DSystem);
	colorCB.Initialize(PAD16(sizeof(cbColorBuffer)));
	m_PColorCB = colorCB.Buffer();
}
//==================================================================================================================================
PostProcessColor::~PostProcessColor()
{
}
//==================================================================================================================================
void PostProcessColor::RenderInit(ZShadeSandboxShader::Shader*& shader)
{
	// Set the pixel shader
	if (!bPixelShaderSet)
	{
		shader->LoadPixelShader("PostProcessColorPS");
		bPixelShaderSet = true;
	}
	
	// Need to set g_Color variable as a constant buffer
	{
		cbColorBuffer cCB;
		cCB.g_Color = XMFLOAT4(1, 1, 1, 1);
		D3D11_MAPPED_SUBRESOURCE mapped_res;
		mD3DSystem->GetDeviceContext()->Map(m_PColorCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
		{
			assert(mapped_res.pData);
			*(cbColorBuffer*)mapped_res.pData = cCB;
		}
		mD3DSystem->GetDeviceContext()->Unmap(m_PColorCB, 0);
	}
	
	ID3D11Buffer* ps_cbs[1] = { m_PColorCB };
	mD3DSystem->GetDeviceContext()->PSSetConstantBuffers(0, 1, ps_cbs);
}
//==================================================================================================================================