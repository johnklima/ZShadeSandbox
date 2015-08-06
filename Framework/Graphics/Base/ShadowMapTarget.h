//==============================================================================================================================
// ShadowMapTarget.h
//
//==============================================================================================================================
// History
//
//  -Created on 5/17/2015 by Dustin Watson
//==============================================================================================================================
#ifndef __SHADOWMAPTARGET_H
#define __SHADOWMAPTARGET_H
//==============================================================================================================================
//==============================================================================================================================

//
// Includes
//

#include "ZMath.h"
#include "D3D.h"
#include <vector>
using namespace std;

class D3D;

//==============================================================================================================================
//==============================================================================================================================
class ShadowMapTarget
{
public:
	
	ShadowMapTarget(D3D* d3d);
	ShadowMapTarget(const ShadowMapTarget&);
	~ShadowMapTarget();
	
	bool Initialize(int textureWidth, int textureHeight);
	
	void Shutdown();
	
	void SetRenderTarget();
	void ResetView();
	
	ID3D11ShaderResourceView* SRView;
	ID3D11RenderTargetView* RTView;
	ID3D11DepthStencilView* DSView;
	UINT Width;
	UINT Height;
	
private:
	
	D3D* m_pD3DSystem;
	
	ID3D11Texture2D* RTVTexture;
	//ID3D11Texture2D* SRVTexture;
	ID3D11Texture2D* DSVTexture;
	
	D3D11_VIEWPORT m_viewport;
};
//==============================================================================================================================
//==============================================================================================================================
#endif//__SHADOWMAPTARGET_H