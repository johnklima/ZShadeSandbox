//===============================================================================================================================
// GBuffer.h
//
//===============================================================================================================================
// History
//
// -Created 5/13/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __GBUFFER_H
#define __GBUFFER_H
//===============================================================================================================================
//===============================================================================================================================

//
//Includes
//

#include "D3D.h"

class RenderTarget2D;

//===============================================================================================================================
//===============================================================================================================================

class GBuffer
{
public:
	
	GBuffer(D3D* d3d);
	~GBuffer();
	
	void Init(int width, int height, float nearPlane, float farPlane);
	
	void Begin(ID3D11DepthStencilView* dsv);
	void ClearRenderTargets();
	void End();
	
	RenderTarget2D* ColorTarget();
	RenderTarget2D* NormalTarget();
	RenderTarget2D* DepthTarget();
	
	RenderTarget2D*& ColorTargetRef();
	RenderTarget2D*& NormalTargetRef();
	RenderTarget2D*& DepthTargetRef();
	
	//ID3D11ShaderResourceView* GetColorSRV();
	//ID3D11ShaderResourceView* GetNormalSRV();
	//ID3D11ShaderResourceView* GetDepthSRV();
	
private:
	
	D3D* mD3DSystem;
	
	RenderTarget2D* mColorTarget;
	RenderTarget2D* mNormalTarget;
	RenderTarget2D* mDepthTarget;
};

//===============================================================================================================================
//===============================================================================================================================
#endif//__GBUFFER_H