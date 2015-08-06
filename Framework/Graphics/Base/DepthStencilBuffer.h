//===============================================================================================================================
// DepthStencilBuffer
//
//===============================================================================================================================
// History
//
// -Created on 7/4/2014 by Dustin Watson
//===============================================================================================================================
#ifndef __DEPTHSTENCILBUFFER_H
#define __DEPTHSTENCILBUFFER_H
//===============================================================================================================================
//===============================================================================================================================

//
//Includes
//

#include "D3D.h"
#include <vector>
using namespace std;

class D3D;
//===============================================================================================================================
//===============================================================================================================================
class DepthStencilBuffer
{
	D3D* m_pD3DSystem;

	ID3D11Texture2D*                     mTexture;
	ID3D11DepthStencilView*              mDSView;
    ID3D11DepthStencilView*              mReadOnlyDSView;
	ID3D11ShaderResourceView*            mSRView;
	UINT                                 mWidth;
	UINT                                 mHeight;
	UINT                                 mMultiSamples;
	UINT                                 mMSQuality;
	DXGI_FORMAT                          mFormat;
    UINT                                 mArraySize;
    std::vector<ID3D11DepthStencilView*> mArraySlices;

public:
	DepthStencilBuffer() {}
	DepthStencilBuffer(D3D* d3d);
	~DepthStencilBuffer();

	void SetD3D(D3D* d3d) { m_pD3DSystem = d3d; }

	void SetDSView(ID3D11DepthStencilView* dsv) { mDSView = dsv; }

	ID3D11Texture2D* GetTexture()                         { return mTexture; }
	ID3D11DepthStencilView* GetDSView()                   { return mDSView; }
    ID3D11DepthStencilView* GetReadOnlyDSView()           { return mReadOnlyDSView; }
	ID3D11ShaderResourceView* GetSRView()                 { return mSRView; }
	UINT GetWidth()                                       { return mWidth; }
	UINT GetHeight()                                      { return mHeight; }
	UINT GetMultiSamples()                                { return mMultiSamples; }
	UINT GetMSQuality()                                   { return mMSQuality; }
	DXGI_FORMAT GetFormat()                               { return mFormat; }
    UINT GetArraySize()                                   { return mArraySize; }
    std::vector<ID3D11DepthStencilView*> GetArraySlices() { return mArraySlices; }

	void Initialize(UINT width,
					UINT height,
					DXGI_FORMAT format = DXGI_FORMAT_D24_UNORM_S8_UINT,
                    bool useAsShaderResource = false,
                    UINT multiSamples = 1,
                    UINT msQuality = 0,
                    UINT arraySize = 1);
};
//===============================================================================================================================
//===============================================================================================================================
#endif//__DEPTHSTENCILBUFFER_H