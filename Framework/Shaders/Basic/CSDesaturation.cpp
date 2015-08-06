//#include "stdafx.h"
#include "CSDesaturation.h"
#include "TextureManager.h"
//==============================================================================================================================
CSDesaturation::CSDesaturation(D3D* d3d, string imageName)
:	ZShadeSandboxShader::Shader(d3d)
,   p_TextureData(0)
,   m_TextureDataSize(0)
,   p_srcTexture(0)
,   p_textureSRV(0)
,   m_ImageName(imageName)
{
	Initialize();
}
//==============================================================================================================================
CSDesaturation::CSDesaturation(const CSDesaturation& other)
:   ZShadeSandboxShader::Shader(other)
{
}
//==============================================================================================================================
CSDesaturation::~CSDesaturation()
{
}
//==============================================================================================================================
bool CSDesaturation::Initialize()
{
	//Load the texture data, Load a DDS texture for now
	TextureManager::Instance()->loadTexture(m_ImageName, &p_srcTexture, m_TextureDataSize, p_TextureData, DDS);

	//
	// Create the Input buffer
	//

	D3D11_BUFFER_DESC srcdesc;
	ZeroMemory( &srcdesc, sizeof(srcdesc) );
	srcdesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	srcdesc.ByteWidth = m_TextureDataSize;
	srcdesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	srcdesc.StructureByteStride = 4;//RGBA format

	D3D11_SUBRESOURCE_DATA srcdata;
	srcdata.pSysMem = p_TextureData;

	m_pD3DSystem->GetDevice11()->CreateBuffer(
		&srcdesc, &srcdata, &p_srcGPUBuffer);

	//Create a view on the resource
	D3D11_BUFFER_DESC descbuf;
	ZeroMemory( &descbuf, sizeof(descbuf) );
	p_srcGPUBuffer->GetDesc( &descbuf );

	D3D11_SHADER_RESOURCE_VIEW_DESC descview;
	ZeroMemory( &descview, sizeof(descview) );
	descview.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	descview.BufferEx.FirstElement = 0;
	descview.Format = DXGI_FORMAT_UNKNOWN;
	descview.BufferEx.NumElements = descbuf.ByteWidth / descbuf.StructureByteStride;

	m_pD3DSystem->GetDevice11()->CreateShaderResourceView(
		p_srcGPUBuffer, &descview, &p_srcData);

	//
	// Create the Output buffer
	//

	D3D11_BUFFER_DESC dstdesc;
	ZeroMemory( &dstdesc, sizeof(dstdesc) );
	dstdesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	dstdesc.ByteWidth = m_TextureDataSize;
	dstdesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	dstdesc.StructureByteStride = 4;//RGBA format

	m_pD3DSystem->GetDevice11()->CreateBuffer(
		&dstdesc, NULL, &p_dstGPUBuffer);

	//Create the view for the output UAV.
	//Allows the Compute ZShadeSandboxShader::Shader to write anywhere in the buffer
	ZeroMemory( &descbuf, sizeof(descbuf) );
	p_dstGPUBuffer->GetDesc( &descbuf );

	D3D11_UNORDERED_ACCESS_VIEW_DESC descview_uav;
	ZeroMemory( &descview_uav, sizeof(descview_uav) );
	descview_uav.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	descview_uav.Buffer.FirstElement = 0;
	descview_uav.Format = DXGI_FORMAT_UNKNOWN;
	descview_uav.Buffer.NumElements = descbuf.ByteWidth / descbuf.StructureByteStride;

	m_pD3DSystem->GetDevice11()->CreateUnorderedAccessView(
		p_dstGPUBuffer, &descview_uav, &p_dstData);
	
	LoadComputeShader("main");
	SwitchTo("main", ZShadeSandboxShader::EShaderTypes::ST_VERTEX);

	return true;
}
//==============================================================================================================================
void CSDesaturation::Shutdown()
{
	SAFE_RELEASE(p_srcData);
	SAFE_RELEASE(p_dstData);
}
//==============================================================================================================================
void CSDesaturation::Render11(int threadX, int threadY, int threadZ)
{
	/*D3DX11_TECHNIQUE_DESC techDesc;
	DesaturationTech->GetDesc(&techDesc);
	ID3DX11EffectPass* pass = 0;
	//for (UINT i = 0; i < techDesc.Passes; ++i)
	{
		pass = DesaturationTech->GetPassByIndex(0);
		pass->Apply(0, m_pD3DSystem->GetDeviceContext());
		m_pD3DSystem->GetDeviceContext()->Dispatch( threadX, threadY, threadZ );

		if (p_textureSRV)
			p_textureSRV->Release();

		//Copy the result from the GPU
		ID3D11Device* device = m_pD3DSystem->GetDevice11();
		ID3D11DeviceContext* immediateContext = m_pD3DSystem->GetDeviceContext();

		D3D11_TEXTURE2D_DESC desc;
		p_srcTexture->GetDesc(&desc);
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.MipLevels = 1;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		device->CreateTexture2D(&desc, NULL, &p_dstTexture);

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		immediateContext->Map(p_dstTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		{
			byte* gpuDestBufferCopy = imageCopyFromGPU();
			memcpy(mappedResource.pData,
				gpuDestBufferCopy, m_TextureDataSize);
		}
		immediateContext->Unmap(p_dstTexture, 0);

		// Create a view of the output texture
		D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc; 
		ZeroMemory(&viewDesc, sizeof(viewDesc));
		viewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; 
		viewDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
		viewDesc.Texture2D.MipLevels = 1;
		viewDesc.Texture2D.MostDetailedMip = 0;
		device->CreateShaderResourceView(
			p_dstTexture, &viewDesc, &p_textureSRV);
	}*/
	
	// Buffers
	if (p_textureSRV)
		p_textureSRV->Release();

	//Copy the result from the GPU
	ID3D11Device* device = m_pD3DSystem->GetDevice11();
	ID3D11DeviceContext* immediateContext = m_pD3DSystem->GetDeviceContext();

	D3D11_TEXTURE2D_DESC desc;
	p_srcTexture->GetDesc(&desc);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.MipLevels = 1;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	device->CreateTexture2D(&desc, NULL, &p_dstTexture);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	immediateContext->Map(p_dstTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	{
		byte* gpuDestBufferCopy = imageCopyFromGPU();
		memcpy(mappedResource.pData,
			gpuDestBufferCopy, m_TextureDataSize);
	}
	immediateContext->Unmap(p_dstTexture, 0);

	// Create a view of the output texture
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc; 
	ZeroMemory(&viewDesc, sizeof(viewDesc));
	viewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; 
	viewDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipLevels = 1;
	viewDesc.Texture2D.MostDetailedMip = 0;
	device->CreateShaderResourceView(
		p_dstTexture, &viewDesc, &p_textureSRV);
	
	// Bind the resources
	ID3D11ShaderResourceView* cs_srvs[1] = {p_srcData};
	immediateContext->CSSetShaderResources(0, 1, cs_srvs);

	ID3D11UnorderedAccessView* cs_uavs[1] = {p_dstData};
	immediateContext->CSSetUnorderedAccessViews(0, 1, cs_uavs, (UINT*)(&cs_uavs[0]));
	
	// Dispatch the Compute ZShadeSandboxShader::Shader
	RenderCS11(threadX, threadY, threadZ);
	
	// Unbind resource
	cs_srvs[0] = NULL;
	immediateContext->CSSetShaderResources(0, 1, cs_srvs);

	cs_uavs[0] = NULL;
	immediateContext->CSSetUnorderedAccessViews(0, 1, cs_uavs, (UINT*)(&cs_uavs[0]));
}
//==============================================================================================================================
byte* CSDesaturation::imageCopyFromGPU()
{
	ID3D11Device* device = m_pD3DSystem->GetDevice11();
	ID3D11DeviceContext* immediateContext = m_pD3DSystem->GetDeviceContext();
	ID3D11Buffer* debugbuf = NULL;

	D3D11_BUFFER_DESC desc;
	ZeroMemory( &desc, sizeof(desc) );
	p_dstGPUBuffer->GetDesc( &desc );

	UINT byteSize = desc.ByteWidth;

	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	desc.Usage = D3D11_USAGE_STAGING;
	desc.BindFlags = 0;
	desc.MiscFlags = 0;

	if ( SUCCEEDED(device->CreateBuffer(&desc, NULL, &debugbuf)) )
	{
		immediateContext->CopyResource( debugbuf, p_dstGPUBuffer );

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		if(immediateContext->Map(debugbuf, 0, D3D11_MAP_READ, 0, &mappedResource) != S_OK)
			return false;

		byte* outBuff = new byte[byteSize];
		memcpy(outBuff, mappedResource.pData, byteSize);

		immediateContext->Unmap(debugbuf, 0);

		debugbuf->Release();

		return outBuff;
	}
	return NULL;
}
//==============================================================================================================================