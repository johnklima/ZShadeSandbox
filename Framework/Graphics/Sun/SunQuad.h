//===============================================================================================================================
// SunQuad.h
//
//===============================================================================================================================
// History
//
// -Created on 4/28/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __SUNQUAD_H
#define __SUNQUAD_H
//==============================================================================================================================
//==============================================================================================================================
#include "D3D.h"
#include "Vertex.h"
//==============================================================================================================================
//==============================================================================================================================
namespace ZShadeSandboxLighting {
class SunQuad
{
public:
	
	SunQuad(D3D* d3d, XMFLOAT2 size, float scale);
	~SunQuad();
	
public:
	
	// Mesh regeneration
	void RegenSunQuad(XMFLOAT2 size);
	void RegenSunQuad(float y);

	XMFLOAT2 Size() { return mSize; }

	XMMATRIX CalculateWorldMatrix();
	
	XMFLOAT3& Position() 		{ return mPosition; }
	XMFLOAT3 Position() const 	{ return mPosition; }
	
	XMFLOAT3& Scale() 			{ return mScale; }
	XMFLOAT3 Scale() const 		{ return mScale; }
	
	XMFLOAT3& Rotation() 		{ return mRotation; }
	XMFLOAT3 Rotation() const 	{ return mRotation; }
	
	ID3D11Buffer* VB() 			{ return mVB; }
	ID3D11Buffer* IB() 			{ return mIB; }
	
	XMMATRIX& World() 			{ return mWorld; }
	XMMATRIX World() const		{ return mWorld; }
	
	int IndexCount() const		{ return mIndexCount; }
	int TriangleCount() const	{ return mTriangleCount; }
	UINT VertexByteWidth()		{ return mVertexByteWidth; }
	
private:
	
	vector<ZShadeSandboxMesh::VertexTex> mVertices;
	vector<unsigned int>				 mIndices;
	
	int					mRenderType;
	int 				mVertexCount;
	int 				mIndexCount;
	int			 		mTriangleCount;
	UINT 				mVertexByteWidth;
	D3D*				mD3DSystem;
	XMMATRIX			mWorld;
	XMFLOAT2			mSize;
	XMFLOAT3			mPosition;
	XMFLOAT3			mScale;
	XMFLOAT3			mRotation;
	ID3D11Buffer*		mVB;
	ID3D11Buffer*		mIB;
};
}
//==============================================================================================================================
//==============================================================================================================================
#endif//__SUNQUAD_H