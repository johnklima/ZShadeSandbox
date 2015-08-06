//===============================================================================================================================
// StructuredBuffer
//
//===============================================================================================================================
// History
//
// -Created on 7/3/2014 by Dustin Watson
//===============================================================================================================================
#ifndef __STRUCTUREDBUFFER_H
#define __STRUCTUREDBUFFER_H
//===============================================================================================================================
//===============================================================================================================================

//
//Includes
//

#include "D3D.h"
using namespace std;

//===============================================================================================================================
//===============================================================================================================================
class StructuredBuffer
{
	D3D* m_pD3DSystem;

	ID3D11Buffer* mBuffer;
    UINT mSize;
    UINT mStride;
    UINT mNumElements;

public:
	StructuredBuffer();
	StructuredBuffer(D3D* d3d);
	~StructuredBuffer();

	void SetD3D(D3D* d3d) { m_pD3DSystem = d3d; }
	
	ID3D11ShaderResourceView* SRView;
    ID3D11UnorderedAccessView* UAView;
	
	ID3D11Buffer* GetBuffer()              { return mBuffer; }
    //ID3D11ShaderResourceView* GetSRView()  { return mSRView; }
    //ID3D11UnorderedAccessView* GetUAView() { return mUAView; }
    UINT GetSize()                         { return mSize; }
	UINT GetStride()                       { return mStride; }
	UINT GetNumElements()                  { return mNumElements; }

    void Initialize(UINT stride, UINT numElements, BOOL useAsUAV = false, BOOL appendConsume = false, BOOL useAsDrawIndirect = false, void* initData = NULL);
	void Initialize(UINT stride, UINT numElements, UINT byteWidth, UINT srvElementCount = 0, BOOL useAsUAV = false, UINT uavElementCount = 0, BOOL appendConsume = false, BOOL useAsDrawIndirect = false, void* initData = NULL);

	void WriteToFile(const CHAR* path);
	void ReadFromFile(const CHAR* path);
};
//===============================================================================================================================
//===============================================================================================================================
#endif//__STRUCTUREDBUFFER_H