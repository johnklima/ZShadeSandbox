//==============================================================================================================================
//RenderTarget2D.h
//
// The RenderTextureClass allows you to set it as the render target instead of the back buffer.
// It also allows you to retrieve the data rendered to it in the form of a ID3D11ShaderResourceView
//==============================================================================================================================
// History
//
//  -Created on 7/9/14 by Dustin Watson
//==============================================================================================================================
#ifndef __RENDERTARGET2D_H
#define __RENDERTARGET2D_H
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
class RenderTarget2D
{
public:
	
	RenderTarget2D() {}
	RenderTarget2D(D3D* d3d);
	RenderTarget2D(const RenderTarget2D&);
	~RenderTarget2D();
	
	bool Initialize
	(	int textureWidth
	,	int textureHeight
	//,	float screenDepth
	//,	float screenNear
	,	DXGI_FORMAT format
	,	UINT multiSamples = 1
	,	UINT msQuality = 0
	,	UINT numMipLevels = 1
	,	bool autoGenMipMaps = false
    ,	bool createUAV = false
	,	bool srvAsCube = false
	);
	
	// Creates a render target from an existing texture
	bool Initialize
	(	ID3D11Texture2D* texture
	,	int textureWidth
	,	int textureHeight
	,	UINT multiSamples = 1
	,	UINT msQuality = 0
	,	UINT numMipLevels = 1
	,	bool autoGenMipMaps = false
    ,   bool createUAV = false
	);
	
	bool Compare
	(	int textureWidth
	,	int textureHeight
	,	DXGI_FORMAT format
	,	UINT multiSamples
	,	UINT msQuality
	);
	
	void Shutdown();

	void SetD3D(D3D* d3d) { m_pD3DSystem = d3d; }

	void SetRenderTarget();
	void ClearRenderTarget(float red, float green, float blue, float alpha);
	void ResetView();

	void GetProjectionMatrix(XMMATRIX&);
	void GetOrthoMatrix(XMMATRIX&);

	//void GetSRV(ID3D11ShaderResourceView*& srv) { srv = m_pSRV; }
	//void GetRTV(ID3D11RenderTargetView*& rtv) { rtv = m_pRTV; }
	//void GetUAV(ID3D11UnorderedAccessView*& uav) { uav = m_pUAV; }

	void GetTexDesc
	(
		int& textureWidth,
		int& textureHeight,
		//float& screenDepth,
		//float& screenNear,
		DXGI_FORMAT& format,
		UINT& multiSamples,
		UINT& msQuality,
		UINT& numMipLevels,
		bool& autoGenMipMaps,
        bool& createUAV,
        UINT& arraySize
	);

	void SetInUse(bool inUse) { InUse = inUse; }

	void IsInUse(bool& inUse) { inUse = InUse; }
	
	//ID3D11Texture2D* SRVTexture;
	ID3D11Texture2D* RTVTexture;
	ID3D11Texture2D* DSVTexture;
	//ID3D11Texture2D* UAVTexture;
	
    ID3D11ShaderResourceView* SRView;
    ID3D11RenderTargetView* RTView;
    ID3D11UnorderedAccessView* UAView;
    ID3D11DepthStencilView* DSView;
	
	D3D11_VIEWPORT viewport;
	
	UINT Width;
    UINT Height;
    DXGI_FORMAT Format;
    UINT MSCount;
    UINT MSQuality;
    BOOL InUse;

private:
	
	ID3D11Texture2D* CreateTexture2D
	(	int width
	,	int height
	,	DXGI_FORMAT format
	,	bool rtv
	,	bool srv
	,	bool dsv
	,	bool uav
	,	UINT multiSamples
	,	UINT msQuality
	,	UINT numMipLevels
	,	bool autoGenMipMaps
	);

	D3D* m_pD3DSystem;
	
	int id;
	//float m_depth;
	//float m_near;
	bool createUAV;
	UINT NumMipLevels;
	bool AutoGenMipMaps;
    UINT ArraySize;
	
    std::vector<ID3D11RenderTargetView*> ArraySlices;
	
	//ID3D11Texture2D* m_depthStencilBuffer;
	
	XMMATRIX m_projectionMatrix;
	XMMATRIX m_orthoMatrix;
};
//==============================================================================================================================
//==============================================================================================================================
#endif//__RENDERTARGET2D_H