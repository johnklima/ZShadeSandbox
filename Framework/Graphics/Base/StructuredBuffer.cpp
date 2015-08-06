//#include "stdafx.h"
#include "StructuredBuffer.h"
#include "StagingBuffer.h"
#include "CGlobal.h"
//===============================================================================================================================
//===============================================================================================================================
StructuredBuffer::StructuredBuffer()
:	m_pD3DSystem(0), mSize(0), mStride(0), mNumElements(0)
{
}
//===============================================================================================================================
StructuredBuffer::StructuredBuffer(D3D* d3d)
:	m_pD3DSystem(d3d), mSize(0), mStride(0), mNumElements(0)
{
}
//===============================================================================================================================
StructuredBuffer::~StructuredBuffer()
{
	SAFE_RELEASE(mBuffer);
	SAFE_RELEASE(SRView);
	SAFE_RELEASE(UAView);
}
//===============================================================================================================================
void StructuredBuffer::Initialize(UINT stride, UINT numElements, BOOL useAsUAV, BOOL appendConsume, BOOL useAsDrawIndirect, void* initData)
{    
    mSize = stride * numElements;
    mStride = stride;
    mNumElements = numElements;

    if(appendConsume || useAsDrawIndirect)
        useAsUAV = true;

    D3D11_BUFFER_DESC bufferDesc;
    bufferDesc.ByteWidth = stride * numElements;
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    bufferDesc.BindFlags |= useAsUAV ? D3D11_BIND_UNORDERED_ACCESS : 0;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    bufferDesc.MiscFlags |= useAsDrawIndirect ? D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS : 0;
    bufferDesc.StructureByteStride = stride;

    D3D11_SUBRESOURCE_DATA subresourceData;
    subresourceData.pSysMem = initData;
    subresourceData.SysMemPitch = 0;
    subresourceData.SysMemSlicePitch = 0;

    m_pD3DSystem->GetDevice11()->CreateBuffer(&bufferDesc, initData != NULL ? &subresourceData : NULL, &mBuffer);

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    srvDesc.Format = DXGI_FORMAT_UNKNOWN;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
    srvDesc.Buffer.ElementOffset = 0;
    srvDesc.Buffer.ElementWidth = numElements;
    m_pD3DSystem->GetDevice11()->CreateShaderResourceView(mBuffer, &srvDesc, &SRView);

    if(useAsUAV)
    {
        D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
        uavDesc.Format = DXGI_FORMAT_UNKNOWN;
        uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
        uavDesc.Buffer.FirstElement = 0;
        uavDesc.Buffer.Flags = appendConsume ? D3D11_BUFFER_UAV_FLAG_APPEND : 0;
        uavDesc.Buffer.NumElements = numElements;
        m_pD3DSystem->GetDevice11()->CreateUnorderedAccessView(mBuffer, &uavDesc, &UAView);
    }
}
//===============================================================================================================================
void StructuredBuffer::Initialize(UINT stride, UINT numElements, UINT byteWidth, UINT srvElementCount, BOOL useAsUAV, UINT uavElementCount, BOOL appendConsume, BOOL useAsDrawIndirect, void* initData)
{    
    mSize = stride * numElements;
    mStride = stride;
    mNumElements = numElements;

    if(appendConsume || useAsDrawIndirect)
        useAsUAV = true;

    D3D11_BUFFER_DESC bufferDesc;
    bufferDesc.ByteWidth = byteWidth;
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    bufferDesc.BindFlags |= useAsUAV ? D3D11_BIND_UNORDERED_ACCESS : 0;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    bufferDesc.MiscFlags |= useAsDrawIndirect ? D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS : 0;
    bufferDesc.StructureByteStride = stride;

    D3D11_SUBRESOURCE_DATA subresourceData;
    subresourceData.pSysMem = initData;
    subresourceData.SysMemPitch = 0;
    subresourceData.SysMemSlicePitch = 0;

    m_pD3DSystem->GetDevice11()->CreateBuffer(&bufferDesc, initData != NULL ? &subresourceData : NULL, &mBuffer);

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    srvDesc.Format = DXGI_FORMAT_UNKNOWN;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
    srvDesc.Buffer.ElementOffset = 0;
    srvDesc.Buffer.ElementWidth = srvElementCount;
    m_pD3DSystem->GetDevice11()->CreateShaderResourceView(mBuffer, &srvDesc, &SRView);

    if(useAsUAV)
    {
        D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
        uavDesc.Format = DXGI_FORMAT_UNKNOWN;
        uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
        uavDesc.Buffer.FirstElement = 0;
        uavDesc.Buffer.Flags = appendConsume ? D3D11_BUFFER_UAV_FLAG_APPEND : 0;
        uavDesc.Buffer.NumElements = uavElementCount;
        m_pD3DSystem->GetDevice11()->CreateUnorderedAccessView(mBuffer, &uavDesc, &UAView);
    }
}
//===============================================================================================================================
void StructuredBuffer::WriteToFile(const CHAR* path)
{
    assert(mBuffer != NULL);

    // Get the buffer info
    D3D11_BUFFER_DESC desc;
    mBuffer->GetDesc(&desc);

    UINT32 useAsUAV = (desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS) ? 1 : 0;
    UINT32 useAsDrawIndirect = (desc.MiscFlags & D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS) ? 1 : 0;

    UINT32 appendConsume = 0;
    if(useAsUAV)
    {
        D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
        UAView->GetDesc(&uavDesc);
        appendConsume = (uavDesc.Format & D3D11_BUFFER_UAV_FLAG_APPEND) ? 1 : 0;
    }

	// If the exists, delete it
	if(FileExists(path))
		DeleteFile(path);
    
	// Create the file
	HANDLE fileHandle = CreateFile(path, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if(fileHandle == INVALID_HANDLE_VALUE)
		;//fail

	// Write the buffer info
	DWORD bytesWritten = 0;
	WriteFile(fileHandle, &mSize, 4, &bytesWritten, NULL);
	WriteFile(fileHandle, &mStride, 4, &bytesWritten, NULL);
	WriteFile(fileHandle, &mNumElements, 4, &bytesWritten, NULL);
	WriteFile(fileHandle, &useAsUAV, 4, &bytesWritten, NULL);
	WriteFile(fileHandle, &useAsDrawIndirect, 4, &bytesWritten, NULL);
	WriteFile(fileHandle, &appendConsume, 4, &bytesWritten, NULL);

	// Get the buffer data
	StagingBuffer stagingBuffer(m_pD3DSystem);
	stagingBuffer.Initialize(mSize);
	m_pD3DSystem->GetDeviceContext()->CopyResource(stagingBuffer.GetBuffer(), mBuffer);
	const void* bufferData= stagingBuffer.Map();

	// Write the data to the file
	WriteFile(fileHandle, bufferData, mSize, &bytesWritten, NULL);

	// Un-map the staging buffer
	stagingBuffer.Unmap();

	// Close the file    
	CloseHandle(fileHandle);
}
//===============================================================================================================================
void StructuredBuffer::ReadFromFile(const CHAR* path)
{
    // Open the file    
    HANDLE fileHandle = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if(fileHandle == INVALID_HANDLE_VALUE)
        ;//fail

    // Read the buffer info
    UINT32 useAsUAV, useAsDrawIndirect, appendConsume;
    DWORD bytesRead = 0;
    ReadFile(fileHandle, &mSize, 4, &bytesRead, NULL);
    ReadFile(fileHandle, &mStride, 4, &bytesRead, NULL);
    ReadFile(fileHandle, &mNumElements, 4, &bytesRead, NULL);
    ReadFile(fileHandle, &useAsUAV, 4, &bytesRead, NULL);
    ReadFile(fileHandle, &useAsDrawIndirect, 4, &bytesRead, NULL);
    ReadFile(fileHandle, &appendConsume, 4, &bytesRead, NULL);

    // Read the buffer data
    UINT8* bufferData = new UINT8[mSize];
    ReadFile(fileHandle, bufferData, mSize, &bytesRead, NULL);

    // Close the file
    CloseHandle(fileHandle);

    // Init
    Initialize(mStride, mNumElements, useAsUAV, appendConsume, useAsDrawIndirect, bufferData);

    // Clean up
    delete [] bufferData;
}
//===============================================================================================================================