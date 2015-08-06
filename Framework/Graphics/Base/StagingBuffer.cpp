//#include "stdafx.h"
#include "StagingBuffer.h"
#include "CGlobal.h"
//===============================================================================================================================
//===============================================================================================================================
StagingBuffer::StagingBuffer(D3D* d3d)
:	m_pD3DSystem(d3d), mSize(0)
{
}
//===============================================================================================================================
StagingBuffer::~StagingBuffer()
{
	SAFE_RELEASE(mBuffer);
}
//===============================================================================================================================
void StagingBuffer::Initialize(UINT size)
{
    mSize = size;

    D3D11_BUFFER_DESC bufferDesc;
    bufferDesc.ByteWidth = mSize;
    bufferDesc.Usage = D3D11_USAGE_STAGING;
    bufferDesc.BindFlags = 0;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;
    m_pD3DSystem->GetDevice11()->CreateBuffer(&bufferDesc, NULL, &mBuffer);
}
//===============================================================================================================================
void* StagingBuffer::Map()
{
    D3D11_MAPPED_SUBRESOURCE mapped;
    m_pD3DSystem->GetDeviceContext()->Map(mBuffer, 0, D3D11_MAP_READ, 0, &mapped);
    return mapped.pData;
}
//===============================================================================================================================
void StagingBuffer::Unmap()
{
    m_pD3DSystem->GetDeviceContext()->Unmap(mBuffer, 0);
}
//===============================================================================================================================