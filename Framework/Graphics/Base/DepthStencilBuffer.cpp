//#include "stdafx.h"
#include "DepthStencilBuffer.h"
#include "CGlobal.h"
//===============================================================================================================================
//===============================================================================================================================
DepthStencilBuffer::DepthStencilBuffer(D3D* d3d)
:	m_pD3DSystem(d3d)
,	mWidth(0)
,	mHeight(0)
,	mMultiSamples(0)
,	mMSQuality(0)
,	mFormat(DXGI_FORMAT_UNKNOWN)
,	mArraySize(1)
{
}
//===============================================================================================================================
DepthStencilBuffer::~DepthStencilBuffer()
{
	/*SAFE_RELEASE(mTexture);
	SAFE_RELEASE(mDSView);
	SAFE_RELEASE(mReadOnlyDSView);
	SAFE_RELEASE(mSRView);*/
}
//===============================================================================================================================
void DepthStencilBuffer::Initialize
(	UINT width
,	UINT height
,	DXGI_FORMAT format
,	bool useAsShaderResource
,	UINT multiSamples
,	UINT msQuality
,	UINT arraySize
)
{
	UINT bindFlags = D3D11_BIND_DEPTH_STENCIL;
	if (useAsShaderResource)
		bindFlags |= D3D11_BIND_SHADER_RESOURCE;

    DXGI_FORMAT dsTexFormat;
    if (!useAsShaderResource)
        dsTexFormat = format;
    else if (format == DXGI_FORMAT_D16_UNORM)
        dsTexFormat = DXGI_FORMAT_R16_TYPELESS;
    else if(format == DXGI_FORMAT_D24_UNORM_S8_UINT)
        dsTexFormat = DXGI_FORMAT_R24G8_TYPELESS;
    else
        dsTexFormat = DXGI_FORMAT_R32_TYPELESS;

	D3D11_TEXTURE2D_DESC desc;
	desc.Width = width;
	desc.Height = height;
	desc.ArraySize = arraySize;
	desc.BindFlags = bindFlags;
	desc.CPUAccessFlags = 0;
	desc.Format = dsTexFormat;
	desc.MipLevels = 1;
	desc.MiscFlags = 0;
	desc.SampleDesc.Count = multiSamples;
	desc.SampleDesc.Quality = msQuality;
	desc.Usage = D3D11_USAGE_DEFAULT;
	m_pD3DSystem->GetDevice11()->CreateTexture2D(&desc, NULL, &mTexture);

    mArraySlices.clear();
    for (UINT i = 0; i < arraySize; ++i)
    {
        D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
        ID3D11DepthStencilView* dsView;
        dsvDesc.Format = format;

        if (arraySize == 1)
        {
            dsvDesc.ViewDimension = multiSamples > 1 ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;
            dsvDesc.Texture2D.MipSlice = 0;        
        }
        else
        {
            dsvDesc.ViewDimension = multiSamples > 1 ? D3D11_DSV_DIMENSION_TEXTURE2DMSARRAY : D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
            dsvDesc.Texture2DArray.ArraySize = 1;
            dsvDesc.Texture2DArray.FirstArraySlice = i;
            dsvDesc.Texture2DArray.MipSlice = 0;
        }

        dsvDesc.Flags = 0;
        m_pD3DSystem->GetDevice11()->CreateDepthStencilView(mTexture, &dsvDesc, &dsView);
        mArraySlices.push_back(dsView);

        if (i == 0)
        {
            // Also create a read-only DSV
            dsvDesc.Flags = D3D11_DSV_READ_ONLY_DEPTH;
            if (format == DXGI_FORMAT_D24_UNORM_S8_UINT || format == DXGI_FORMAT_D32_FLOAT_S8X24_UINT)
                dsvDesc.Flags |= D3D11_DSV_READ_ONLY_STENCIL;
            m_pD3DSystem->GetDevice11()->CreateDepthStencilView(mTexture, &dsvDesc, &mReadOnlyDSView);
            dsvDesc.Flags = 0;
        }
    }

    mDSView = mArraySlices[0];

    if (useAsShaderResource)
    {
        DXGI_FORMAT dsSRVFormat;
        if (format == DXGI_FORMAT_D16_UNORM)
            dsSRVFormat = DXGI_FORMAT_R16_UNORM;
        else if(format == DXGI_FORMAT_D24_UNORM_S8_UINT)
            dsSRVFormat = DXGI_FORMAT_R24_UNORM_X8_TYPELESS ;
        else
            dsSRVFormat = DXGI_FORMAT_R32_FLOAT;

        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
        srvDesc.Format = dsSRVFormat;

        if (arraySize == 1)
        {
            srvDesc.ViewDimension = multiSamples > 1 ? D3D11_SRV_DIMENSION_TEXTURE2DMS : D3D11_SRV_DIMENSION_TEXTURE2D;
            srvDesc.Texture2D.MipLevels = 1;
            srvDesc.Texture2D.MostDetailedMip = 0;
        }
        else
        {
            srvDesc.ViewDimension = multiSamples > 1 ? D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY : D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
            srvDesc.Texture2DArray.ArraySize = arraySize;            
            srvDesc.Texture2DArray.FirstArraySlice = 0;
            srvDesc.Texture2DArray.MipLevels = 1;
            srvDesc.Texture2DArray.MostDetailedMip = 0;
        }
	    
        m_pD3DSystem->GetDevice11()->CreateShaderResourceView(mTexture, &srvDesc, &mSRView);
    }
    else
        mSRView = NULL;

	mWidth = width;
	mHeight = height;
	mMultiSamples = multiSamples;
	mFormat = format;
    mArraySize = arraySize;
}
//===============================================================================================================================