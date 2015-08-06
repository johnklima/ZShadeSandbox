#include "SunQuad.h"
#include "Vertex.h"
#include "ZMath.h"
using ZShadeSandboxLighting::SunQuad;
//===============================================================================================================================
//===============================================================================================================================
SunQuad::SunQuad(D3D* d3d, XMFLOAT2 size, float scale)
:   mD3DSystem(d3d)
{
	size.x *= 0.5f;
	size.y *= 0.5f;
	
	mSize = size;

	mWorld = XMMatrixIdentity();

	mPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
	mRotation = XMFLOAT3(0.0f, 0.0f, 0.0f);

	mScale = XMFLOAT3(scale, scale, scale);
	
	//
	// Load the mesh buffers
	//
	
	XMFLOAT3 vv0(-1.0f, 1.0f, 0.0f);
	XMFLOAT3 vv1(1.0f, 1.0f, 0.0f);
	XMFLOAT3 vv2(-1.0f, -1.0f, 0.0f);
	XMFLOAT3 vv3(1.0f, -1.0f, 0.0f);

	ZShadeSandboxMesh::VertexTex v0 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(-size.x, -size.y, 0.0f, 0.0f, 0.0f);
	ZShadeSandboxMesh::VertexTex v1 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(-size.x, size.y, 0.0f, 0.0f, 1.0f);
	ZShadeSandboxMesh::VertexTex v2 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(size.x, -size.y, 0.0f, 1.0f, 0.0f);
	ZShadeSandboxMesh::VertexTex v3 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(size.x, size.y, 0.0f, 1.0f, 1.0f);
	
	ZShadeSandboxMesh::VertexTex verts[4] =
	{
		v0, v1, v2, v3
	};

	mVertices.resize(4);

	for (int i = 0; i < 4; i++)
	{
		mVertices[i] = verts[i];
	}

	/*ZShadeSandboxMesh::VertexTex v0 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(vv0.x, vv0.y, vv0.z, 0.0f, 0.0f);
	ZShadeSandboxMesh::VertexTex v1 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(vv1.x, vv1.y, vv1.z, 0.0f, 1.0f);
	ZShadeSandboxMesh::VertexTex v2 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(vv2.x, vv2.y, vv2.z, 1.0f, 0.0f);
	ZShadeSandboxMesh::VertexTex v3 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(vv3.x, vv3.y, vv3.z, 1.0f, 1.0f);

	ZShadeSandboxMesh::VertexTex verts[4] =
	{
		v0, v1, v2, v3
	};

	mVertices.resize(4);

	for (int i = 0; i < 4; i++)
	{
		mVertices[i] = verts[i];
	}*/

	////First Triangle
	//ZShadeSandboxMesh::VertexTex c1 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	//ZShadeSandboxMesh::VertexTex c2 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(1.0f, 1.0f, 0.0f, 1.0f, 1.0f);
	//ZShadeSandboxMesh::VertexTex c3 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(0.0f, 1.0f, 0.0f, 0.0f, 1.0f);

	////Second Triangle
	//ZShadeSandboxMesh::VertexTex c4 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	//ZShadeSandboxMesh::VertexTex c5 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	//ZShadeSandboxMesh::VertexTex c6 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(1.0f, 1.0f, 0.0f, 1.0f, 1.0f);

	//ZShadeSandboxMesh::VertexTex verts[6] =
	//{
	//	c3, c2, c1, c6, c5, c4
	//};

	//mVertices.resize(6);

	//for (int i = 0; i < 6; i++)
	//{
	//	mVertices[i] = verts[i];
	//}

	UINT indices[6] =
	{
		0, 1, 2, 1, 3, 2
	};

	mIndices.resize(6);

	for (int i = 0; i < 6; i++)
	{
		mIndices[i] = indices[i];
	}

	mVertexCount = 6;
	mIndexCount = 6;
	mTriangleCount = 2;
	mVertexByteWidth = sizeof(ZShadeSandboxMesh::VertexTex);
	
	//
	// Create Vertex Buffer
	//

	HRESULT result;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	//Setup the description of the static vertex buffer
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = mVertexByteWidth * mVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource a pointer to the vertex data
	vertexData.pSysMem = &mVertices[0];
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	//Create the vertex buffer
	result = mD3DSystem->GetDevice11()->CreateBuffer(&vertexBufferDesc, &vertexData, &mVB);
	if (FAILED(result)) return;

	//
	// Create Index Buffer
	//

	//Setup the description of the static index buffer
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * mIndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	//Give the subresource a pointer to the index data
	indexData.pSysMem = &mIndices[0];
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	//Create the index buffer
	result = mD3DSystem->GetDevice11()->CreateBuffer(&indexBufferDesc, &indexData, &mIB);
	if (FAILED(result)) return;
}
//===============================================================================================================================
void SunQuad::RegenSunQuad(XMFLOAT2 size)
{
	//
	// Load the mesh buffers
	//
	
	ZShadeSandboxMesh::VertexTex v0 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(-size.x, -size.y, 0.0f, 0.0f, 0.0f);
	ZShadeSandboxMesh::VertexTex v1 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(-size.x, size.y, 0.0f, 0.0f, 1.0f);
	ZShadeSandboxMesh::VertexTex v2 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(size.x, -size.y, 0.0f, 1.0f, 0.0f);
	ZShadeSandboxMesh::VertexTex v3 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(size.x, size.y, 0.0f, 1.0f, 1.0f);
	
	mVertices.clear();
	
	mVertices.push_back(v0);
	mVertices.push_back(v1);
	mVertices.push_back(v2);
	mVertices.push_back(v3);
	
	D3D11_MAPPED_SUBRESOURCE resource;
	mD3DSystem->GetDeviceContext()->Map(mVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, &mVertices[0], mVertexByteWidth * mVertexCount);
	mD3DSystem->GetDeviceContext()->Unmap(mVB, 0);
}
//===============================================================================================================================
void SunQuad::RegenSunQuad(float y)
{
	//
	// Load the mesh buffers
	//

	ZShadeSandboxMesh::VertexTex v0 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(-mSize.x, -y, 0.0f, 0.0f, 0.0f);
	ZShadeSandboxMesh::VertexTex v1 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(-mSize.x, y, 0.0f, 0.0f, 1.0f);
	ZShadeSandboxMesh::VertexTex v2 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(mSize.x, -y, 0.0f, 1.0f, 0.0f);
	ZShadeSandboxMesh::VertexTex v3 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(mSize.x, y, 0.0f, 1.0f, 1.0f);

	mVertices.clear();

	mVertices.push_back(v0);
	mVertices.push_back(v1);
	mVertices.push_back(v2);
	mVertices.push_back(v3);

	D3D11_MAPPED_SUBRESOURCE resource;
	mD3DSystem->GetDeviceContext()->Map(mVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, &mVertices[0], mVertexByteWidth * mVertexCount);
	mD3DSystem->GetDeviceContext()->Unmap(mVB, 0);
}
//===============================================================================================================================
XMMATRIX SunQuad::CalculateWorldMatrix()
{
	XMMATRIX world, scale, rotation, translation;
	
	scale = XMMatrixScaling(mScale.x, mScale.y, mScale.z);
	rotation = ZShadeSandboxMath::ZMath::MatrixFromNormal(mRotation);
	translation = XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);
	
	mWorld = XMMatrixIdentity();
	mWorld = XMMatrixMultiply(mWorld, scale);
	mWorld = XMMatrixMultiply(mWorld, rotation);
	mWorld = XMMatrixMultiply(mWorld, translation);

	return mWorld;
}
//===============================================================================================================================