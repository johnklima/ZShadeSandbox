#include "GBuffer.h"
#include "D3D.h"
#include "RenderTarget2D.h"
//https://github.com/NVIDIAGameWorks/D3DSamples/blob/master/samples/DeferredShadingMSAA/src/DeferredRenderer.cpp
//===============================================================================================================================
//===============================================================================================================================
GBuffer::GBuffer(D3D* d3d)
:   mD3DSystem(d3d)
{
}
//===============================================================================================================================
GBuffer::~GBuffer()
{
}
//===============================================================================================================================
void GBuffer::Init(int width, int height, float nearPlane, float farPlane)
{
	//https://github.com/NVIDIAGameWorks/D3DSamples/blob/master/samples/DeferredShadingMSAA/src/DirectXUtil.h
	
	//mColorTarget = new RenderTarget2D(mD3DSystem);
	//mColorTarget->Initialize(width, height, farPlane, nearPlane, DXGI_FORMAT_R32G32B32A32_FLOAT);
	
	//mNormalTarget = new RenderTarget2D(mD3DSystem);
	//mNormalTarget->Initialize(width, height, farPlane, nearPlane, DXGI_FORMAT_R32G32B32A32_FLOAT);
	
	//mDepthTarget = new RenderTarget2D(mD3DSystem);
	//mDepthTarget->Initialize(width, height, farPlane, nearPlane, DXGI_FORMAT_R32G32B32A32_FLOAT);
	
	mColorTarget = new RenderTarget2D(mD3DSystem);
	mColorTarget->Initialize(width, height, DXGI_FORMAT_R32G32B32A32_FLOAT);
	
	mNormalTarget = new RenderTarget2D(mD3DSystem);
	mNormalTarget->Initialize(width, height, DXGI_FORMAT_R32G32B32A32_FLOAT);
	
	mDepthTarget = new RenderTarget2D(mD3DSystem);
	mDepthTarget->Initialize(width, height, DXGI_FORMAT_R32G32B32A32_FLOAT);
}
//===============================================================================================================================
void GBuffer::Begin(ID3D11DepthStencilView* dsv)
{
	ID3D11RenderTargetView* rtv[3] = { mColorTarget->RTView, mNormalTarget->RTView, mDepthTarget->RTView };
	
	// Bind the render target view and depth stencil buffer to the output render pipeline.
	mD3DSystem->GetDeviceContext()->OMSetRenderTargets(3, rtv, dsv);
	mD3DSystem->GetDeviceContext()->OMSetDepthStencilState(NULL, 0);
	
	ClearRenderTargets();
	
	// Set the viewport.
	D3D11_VIEWPORT viewports[3];
	viewports[0] = mColorTarget->viewport;
	viewports[1] = mNormalTarget->viewport;
	viewports[2] = mDepthTarget->viewport;
	mD3DSystem->GetDeviceContext()->RSSetViewports(3, viewports);

	// Clear the depth buffer.
	//mD3DSystem->GetDeviceContext()->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}
//===============================================================================================================================
void GBuffer::ClearRenderTargets()
{
	float color[4];
	
	// Setup the color to clear the buffer to.
	color[0] = 0.1f;
	color[1] = 0.2f;
	color[2] = 0.4f;
	color[3] = 0.0f;
	mD3DSystem->GetDeviceContext()->ClearRenderTargetView(mColorTarget->RTView, color);
	
	// Setup the color to clear the buffer to.
	color[0] = 0.5f;
	color[1] = 0.5f;
	color[2] = 0.5f;
	color[3] = 1;
	mD3DSystem->GetDeviceContext()->ClearRenderTargetView(mNormalTarget->RTView, color);
	
	// Setup the color to clear the buffer to.
	color[0] = 1;
	color[1] = 0;
	color[2] = 0;
	color[3] = 1;
	mD3DSystem->GetDeviceContext()->ClearRenderTargetView(mDepthTarget->RTView, color);
    
	// Clear the depth buffer.
	//mD3DSystem->GetDeviceContext()->ClearDepthStencilView(mDepthTarget->DSView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}
//===============================================================================================================================
void GBuffer::End()
{
	ID3D11RenderTargetView* rtv[] = { NULL, NULL, NULL };
	mD3DSystem->GetDeviceContext()->OMSetRenderTargets(3, rtv, NULL);
}
//===============================================================================================================================
/*ID3D11ShaderResourceView* GBuffer::GetColorSRV()
{
	return mColorTarget->SRView;
}
//===============================================================================================================================
ID3D11ShaderResourceView* GBuffer::GetNormalSRV()
{
	return mNormalTarget->SRView;
}
//===============================================================================================================================
ID3D11ShaderResourceView* GBuffer::GetDepthSRV()
{
	return mDepthTarget->SRView;
}*/
//===============================================================================================================================
RenderTarget2D* GBuffer::ColorTarget()
{
	return mColorTarget;
}
//===============================================================================================================================
RenderTarget2D* GBuffer::NormalTarget()
{
	return mNormalTarget;
}
//===============================================================================================================================
RenderTarget2D* GBuffer::DepthTarget()
{
	return mDepthTarget;
}
//===============================================================================================================================
RenderTarget2D*& GBuffer::ColorTargetRef()
{
	return mColorTarget;
}
//===============================================================================================================================
RenderTarget2D*& GBuffer::NormalTargetRef()
{
	return mNormalTarget;
}
//===============================================================================================================================
RenderTarget2D*& GBuffer::DepthTargetRef()
{
	return mDepthTarget;
}
//===============================================================================================================================