//===============================================================================================================================
// Direct Compute Render Texture
//
// Render to Texture from the Compute ZShadeSandboxShader::Shader
//===============================================================================================================================
// History
//
// -Created on 6/27/2014 by Dustin Watson
//===============================================================================================================================
#ifndef __DCRENDERTEXTURE_H
#define __DCRENDERTEXTURE_H
//===============================================================================================================================
//===============================================================================================================================

//
//Includes
//

#include "D3D.h"

//==============================================================================================================================
//==============================================================================================================================
class DCRenderTexture
{
public:
	DCRenderTexture(D3D* d3d, int textureWidth, int textureHeight, DXGI_FORMAT format);
	DCRenderTexture(const DCRenderTexture&);
	~DCRenderTexture();

	void Shutdown();

	void SetRenderTarget();
	
	void ResetView();

	ID3D11ShaderResourceView* GetSRV();
	
private:
	D3D* m_pD3DSystem;
	ID3D11Texture2D* m_renderTargetTexture;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11ShaderResourceView* m_shaderResourceView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilView* m_depthStencilView;
	D3D11_VIEWPORT m_viewport;
};
//==============================================================================================================================
//==============================================================================================================================
#endif//__DCRENDERTEXTURE_H