//===============================================================================================================================
// ConstantBuffer
//
//===============================================================================================================================
// History
//
// -Created on 7/3/2014 by Dustin Watson
//===============================================================================================================================
#ifndef __CONSTANTBUFFER_H
#define __CONSTANTBUFFER_H
//===============================================================================================================================
//===============================================================================================================================

//
//Includes
//

#include "D3D.h"
using namespace std;

//===============================================================================================================================
//===============================================================================================================================
template<typename T> class ConstantBuffer
{
public:

    T Data;

protected:

	D3D* m_pD3DSystem;
    ID3D11Buffer* buffer;
    bool initialized;

public:

	ConstantBuffer() : initialized(false), m_pD3DSystem(0)
    {
        ZeroMemory(&Data, sizeof(T));
    }

    ConstantBuffer(D3D* d3d) : initialized(false), m_pD3DSystem(d3d)
    {
        ZeroMemory(&Data, sizeof(T));
    }

	void SetD3D(D3D* d3d)
	{
		m_pD3DSystem = d3d;
	}

    ID3D11Buffer* Buffer() const
    {
        return buffer;
    }

	void Initialize(UINT byteWidth, void* initData=nullptr, bool immutable=false)
	{
		if (!immutable)//Create a Dynamic buffer
		{
			D3D11_BUFFER_DESC desc;
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			desc.MiscFlags = 0;
			desc.ByteWidth = byteWidth;

			m_pD3DSystem->GetDevice11()->CreateBuffer(&desc, NULL, &buffer);
		}
		else
		{
			D3D11_SUBRESOURCE_DATA init_cb0 = { initData, 0, 0 };

			D3D11_BUFFER_DESC desc;
			desc.Usage = D3D11_USAGE_IMMUTABLE;
			desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			desc.CPUAccessFlags = 0;
			desc.MiscFlags = 0;
			desc.ByteWidth = byteWidth;

			m_pD3DSystem->GetDevice11()->CreateBuffer(&desc, &init_cb0, &buffer);
		}

		initialized = true;
	}

	void Initialize(void* initData=nullptr, bool immutable = false)
    {
		if (!immutable)//Create a Dynamic buffer
		{
			D3D11_BUFFER_DESC desc;
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			desc.MiscFlags = 0;
			desc.ByteWidth = static_cast<UINT>(sizeof(T) + (16 - (sizeof(T) % 16)));

			m_pD3DSystem->GetDevice11()->CreateBuffer(&desc, NULL, &buffer);
		}
		else
		{
			D3D11_SUBRESOURCE_DATA init_cb0 = { initData, 0, 0 };

			D3D11_BUFFER_DESC desc;
			desc.Usage = D3D11_USAGE_IMMUTABLE;
			desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			desc.CPUAccessFlags = 0;
			desc.MiscFlags = 0;
			desc.ByteWidth = static_cast<UINT>(sizeof(T) + (16 - (sizeof(T) % 16)));

			m_pD3DSystem->GetDevice11()->CreateBuffer(&desc, &init_cb0, &buffer);
		}

        initialized = true;
    }

    void ApplyChanges()
    {
        _ASSERT(initialized);

        D3D11_MAPPED_SUBRESOURCE mappedResource;
        m_pD3DSystem->GetDeviceContext()->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
        CopyMemory(mappedResource.pData, &Data, sizeof(T));
        m_pD3DSystem->GetDeviceContext()->Unmap(buffer, 0);
    }

    void SetVS(UINT slot) const
    {
        _ASSERT(initialized);

        ID3D11Buffer* bufferArray[1];
        bufferArray[0] = buffer;
        m_pD3DSystem->GetDeviceContext()->VSSetConstantBuffers(slot, 1, bufferArray);
    }

    void SetPS(UINT slot) const
    {
        _ASSERT(initialized);

        ID3D11Buffer* bufferArray[1];
        bufferArray[0] = buffer;
        m_pD3DSystem->GetDeviceContext()->PSSetConstantBuffers(slot, 1, bufferArray);
    }

    void SetGS(UINT slot) const
    {
        _ASSERT(initialized);

        ID3D11Buffer* bufferArray[1];
        bufferArray[0] = buffer;
        m_pD3DSystem->GetDeviceContext()->GSSetConstantBuffers(slot, 1, bufferArray);
    }

    void SetHS(UINT slot) const
    {
        _ASSERT(initialized);

        ID3D11Buffer* bufferArray[1];
        bufferArray[0] = buffer;
        m_pD3DSystem->GetDeviceContext()->HSSetConstantBuffers(slot, 1, bufferArray);
    }

    void SetDS(UINT slot) const
    {
        _ASSERT(initialized);

        ID3D11Buffer* bufferArray[1];
        bufferArray[0] = buffer;
        m_pD3DSystem->GetDeviceContext()->DSSetConstantBuffers(slot, 1, bufferArray);
    }

    void SetCS(UINT slot) const
    {
        _ASSERT(initialized);

        ID3D11Buffer* bufferArray[1];
        bufferArray[0] = buffer;
        m_pD3DSystem->GetDeviceContext()->CSSetConstantBuffers(slot, 1, bufferArray);
    }
};
//===============================================================================================================================
//===============================================================================================================================
#endif//__CONSTANTBUFFER_H