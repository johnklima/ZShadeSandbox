#include "CylindricalMesh.h"
#include "ZMath.h"
#include <algorithm>
using ZShadeSandboxMesh::CylindricalMesh;
//===============================================================================================================================
CylindricalMesh::CylindricalMesh(float bottomRadius, float topRadius,
	float height, UINT stackCount, UINT sliceCount,
		D3D* d3d, ZShadeSandboxMesh::MeshParameters mp)
:   ZShadeSandboxMesh::CustomMesh( d3d, mp )
,   fBottomRadius(bottomRadius)
,   fTopRadius(topRadius)
,   fHeight(height)
,   iStackCount(stackCount)
,   iSliceCount(sliceCount)
{
	mMeshType = ZShadeSandboxMesh::EMeshType::CYLINDER;

	Initialize();
	
	CreateBuffers();
}
//===============================================================================================================================
CylindricalMesh::~CylindricalMesh()
{
}
//===============================================================================================================================
void CylindricalMesh::BuildCylinderTopCap()
{
	UINT baseIndex = 0;
	UINT centerIndex = 0;
	float y = 0.5f * fHeight;
	float dTheta = 2.0f * PI / iSliceCount;
	
	switch (mMeshParameters.vertexType)
	{
		case ZShadeSandboxMesh::EVertexType::VT_NormalTex:
		{
#pragma region "Vertex Normal Tex"
			baseIndex = (UINT)mAttributes->mVerticesNormalTex.size();

			// Duplicate cap ring vertices because the texture coordinates and normals differ.
			for (UINT i = 0; i <= iSliceCount; ++i)
			{
				float x = fTopRadius * cosf(i * dTheta);
				float z = fTopRadius * sinf(i * dTheta);

				// Scale down by the height to try and make top cap texture coord area
				// proportional to base.
				float u = x / fHeight + 0.5f;
				float v = z / fHeight + 0.5f;

				mAttributes->mVerticesNormalTex.push_back(ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(x, y, z, 0.0f, 1.0f, 0.0f, u, v));
			}

			// Cap center vertex.
			mAttributes->mVerticesNormalTex.push_back(ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(0.0f, y, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f));

			// Index of center vertex.
			centerIndex = (UINT)mAttributes->mVerticesNormalTex.size() - 1;
#pragma endregion
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_NormalTexTan:
		{
#pragma region "Vertex Normal Tex Tan"
			baseIndex = (UINT)mAttributes->mVerticesNormalTexTan.size();
			
			// Duplicate cap ring vertices because the texture coordinates and normals differ.
			for (UINT i = 0; i <= iSliceCount; ++i)
			{
				float x = fTopRadius * cosf(i * dTheta);
				float z = fTopRadius * sinf(i * dTheta);

				// Scale down by the height to try and make top cap texture coord area
				// proportional to base.
				float u = x / fHeight + 0.5f;
				float v = z / fHeight + 0.5f;

				mAttributes->mVerticesNormalTexTan.push_back(ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(x, y, z, 0.0f, 1.0f, 0.0f, u, v, 1.0f, 0.0f, 0.0f));
			}

			// Cap center vertex.
			mAttributes->mVerticesNormalTexTan.push_back(ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(0.0f, y, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f));

			// Index of center vertex.
			centerIndex = (UINT)mAttributes->mVerticesNormalTexTan.size() - 1;
#pragma endregion
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Pos:
		{
#pragma region "Vertex Pos"
			baseIndex = (UINT)mAttributes->mVerticesPos.size();

			// Duplicate cap ring vertices because the texture coordinates and normals differ.
			for (UINT i = 0; i <= iSliceCount; ++i)
			{
				float x = fTopRadius * cosf(i * dTheta);
				float z = fTopRadius * sinf(i * dTheta);

				mAttributes->mVerticesPos.push_back(ZShadeSandboxMesh::VertexUtil::LoadVertexPos(x, y, z));
			}

			// Cap center vertex.
			mAttributes->mVerticesPos.push_back(ZShadeSandboxMesh::VertexUtil::LoadVertexPos(0.0f, y, 0.0f));

			// Index of center vertex.
			centerIndex = (UINT)mAttributes->mVerticesPos.size() - 1;
#pragma endregion
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Tex:
		{
#pragma region "Vertex Tex"
			baseIndex = (UINT)mAttributes->mVerticesTex.size();
			
			// Duplicate cap ring vertices because the texture coordinates and normals differ.
			for(UINT i = 0; i <= iSliceCount; ++i)
			{
				float x = fTopRadius * cosf(i * dTheta);
				float z = fTopRadius * sinf(i * dTheta);

				// Scale down by the height to try and make top cap texture coord area
				// proportional to base.
				float u = x / fHeight + 0.5f;
				float v = z / fHeight + 0.5f;

				mAttributes->mVerticesTex.push_back(ZShadeSandboxMesh::VertexUtil::LoadVertexTex(x, y, z, u, v));
			}

			// Cap center vertex.
			mAttributes->mVerticesTex.push_back(ZShadeSandboxMesh::VertexUtil::LoadVertexTex(0.0f, y, 0.0f, 0.5f, 0.5f));

			// Index of center vertex.
			centerIndex = (UINT)mAttributes->mVerticesTex.size() - 1;
#pragma endregion
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Color:
		{
#pragma region "Vertex Color"
			baseIndex = (UINT)mAttributes->mVerticesColor.size();
			XMFLOAT4 diffuseColor = mMaterial->vDiffuseColor;

			// Duplicate cap ring vertices because the texture coordinates and normals differ.
			for (UINT i = 0; i <= iSliceCount; ++i)
			{
				float x = fTopRadius * cosf(i * dTheta);
				float z = fTopRadius * sinf(i * dTheta);

				mAttributes->mVerticesColor.push_back(ZShadeSandboxMesh::VertexUtil::LoadVertexColor(x, y, z, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w));
			}

			// Cap center vertex.
			mAttributes->mVerticesColor.push_back(ZShadeSandboxMesh::VertexUtil::LoadVertexColor(0.0f, y, 0.0f, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w));

			// Index of center vertex.
			centerIndex = (UINT)mAttributes->mVerticesColor.size() - 1;
#pragma endregion
		}
		break;
	}
	
	UINT index = 0;
	for (UINT i = 0; i < iSliceCount; ++i)
	{
		mAttributes->mIndices.push_back(centerIndex);
		mAttributes->mIndices.push_back(baseIndex + i + 1);
		mAttributes->mIndices.push_back(baseIndex + i);
	}
}
//===============================================================================================================================
void CylindricalMesh::BuildCylinderBottomCap()
{
	// 
	// Build bottom cap.
	//

	UINT baseIndex = 0;
	UINT centerIndex = 0;
	float y = -0.5f * fHeight;

	// vertices of ring
	float dTheta = 2.0f * PI / iSliceCount;
	
	switch (mMeshParameters.vertexType)
	{
		case ZShadeSandboxMesh::EVertexType::VT_NormalTex:
		{
#pragma region "Vertex Normal Tex"
			baseIndex = (UINT)mAttributes->mVerticesNormalTex.size();

			for (UINT i = 0; i <= iSliceCount; ++i)
			{
				float x = fBottomRadius * cosf(i * dTheta);
				float z = fBottomRadius * sinf(i * dTheta);

				// Scale down by the height to try and make top cap texture coord area
				// proportional to base.
				float u = x / fHeight + 0.5f;
				float v = z / fHeight + 0.5f;
				
				mAttributes->mVerticesNormalTex.push_back(ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(x, y, z, 0.0f, -1.0f, 0.0f, u, v));
			}

			// Cap center vertex.
			mAttributes->mVerticesNormalTex.push_back(ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(0.0f, y, 0.0f, 0.0f, -1.0f, 0.0f, 0.5f, 0.5f));

			// Cache the index of center vertex.
			centerIndex = (UINT)mAttributes->mVerticesNormalTex.size() - 1;
#pragma endregion
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_NormalTexTan:
		{
#pragma region "Vertex Normal Tex Tan"
			baseIndex = (UINT)mAttributes->mVerticesNormalTexTan.size();

			for (UINT i = 0; i <= iSliceCount; ++i)
			{
				float x = fBottomRadius * cosf(i * dTheta);
				float z = fBottomRadius * sinf(i * dTheta);

				// Scale down by the height to try and make top cap texture coord area
				// proportional to base.
				float u = x / fHeight + 0.5f;
				float v = z / fHeight + 0.5f;
				
				mAttributes->mVerticesNormalTexTan.push_back(ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(x, y, z, 0.0f, -1.0f, 0.0f, u, v, 1.0f, 0.0f, 0.0f));
			}

			// Cap center vertex.
			mAttributes->mVerticesNormalTexTan.push_back(ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(0.0f, y, 0.0f, 0.0f, -1.0f, 0.0f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f));

			// Cache the index of center vertex.
			centerIndex = (UINT)mAttributes->mVerticesNormalTexTan.size() - 1;
#pragma endregion
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Pos:
		{
#pragma region "Vertex Pos"
			baseIndex = (UINT)mAttributes->mVerticesPos.size();

			for (UINT i = 0; i <= iSliceCount; ++i)
			{
				float x = fBottomRadius * cosf(i * dTheta);
				float z = fBottomRadius * sinf(i * dTheta);

				// Scale down by the height to try and make top cap texture coord area
				// proportional to base.
				float u = x / fHeight + 0.5f;
				float v = z / fHeight + 0.5f;
				
				mAttributes->mVerticesPos.push_back(ZShadeSandboxMesh::VertexUtil::LoadVertexPos(x, y, z));
			}

			// Cap center vertex.
			mAttributes->mVerticesPos.push_back(ZShadeSandboxMesh::VertexUtil::LoadVertexPos(0.0f, y, 0.0f));

			// Cache the index of center vertex.
			centerIndex = (UINT)mAttributes->mVerticesPos.size() - 1;
#pragma endregion
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Tex:
		{
#pragma region "Vertex Tex"
			baseIndex = (UINT)mAttributes->mVerticesTex.size();

			for (UINT i = 0; i <= iSliceCount; ++i)
			{
				float x = fBottomRadius * cosf(i * dTheta);
				float z = fBottomRadius * sinf(i * dTheta);

				// Scale down by the height to try and make top cap texture coord area
				// proportional to base.
				float u = x / fHeight + 0.5f;
				float v = z / fHeight + 0.5f;
				
				mAttributes->mVerticesTex.push_back(ZShadeSandboxMesh::VertexUtil::LoadVertexTex(x, y, z, u, v));
			}

			// Cap center vertex.
			mAttributes->mVerticesTex.push_back(ZShadeSandboxMesh::VertexUtil::LoadVertexTex(0.0f, y, 0.0f, 0.5f, 0.5f));

			// Cache the index of center vertex.
			centerIndex = (UINT)mAttributes->mVerticesTex.size() - 1;
#pragma endregion
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Color:
		{
#pragma region "Vertex Color"
			baseIndex = (UINT)mAttributes->mVerticesColor.size();
			XMFLOAT4 diffuseColor = mMaterial->vDiffuseColor;

			for (UINT i = 0; i <= iSliceCount; ++i)
			{
				float x = fBottomRadius * cosf(i * dTheta);
				float z = fBottomRadius * sinf(i * dTheta);

				// Scale down by the height to try and make top cap texture coord area
				// proportional to base.
				float u = x / fHeight + 0.5f;
				float v = z / fHeight + 0.5f;
				
				mAttributes->mVerticesColor.push_back(ZShadeSandboxMesh::VertexUtil::LoadVertexColor(x, y, z, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w));
			}

			// Cap center vertex.
			mAttributes->mVerticesColor.push_back(ZShadeSandboxMesh::VertexUtil::LoadVertexColor(0.0f, y, 0.0f, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w));

			// Cache the index of center vertex.
			centerIndex = (UINT)mAttributes->mVerticesColor.size() - 1;
#pragma endregion
		}
		break;
	}
	
	UINT index = 0;
	for (UINT i = 0; i < iSliceCount; ++i)
	{
		mAttributes->mIndices.push_back(centerIndex);
		mAttributes->mIndices.push_back(baseIndex + i);
		mAttributes->mIndices.push_back(baseIndex + i + 1);
	}
}
//===============================================================================================================================
void CylindricalMesh::Initialize()
{
	//int NUM_VERTS   = 0;
	//int NUM_INDICES = 0;
	
	//
	// Build Stacks.
	//
	
	float stackHeight = fHeight / iStackCount;
	
	// Amount to increment radius as we move up each stack level from bottom to top.
	float radiusStep = (fTopRadius - fBottomRadius) / iStackCount;
	
	UINT ringCount = iStackCount + 1;
	
	// Cylinder can be parameterized as follows, where we introduce v
	// parameter that goes in the same direction as the v tex-coord
	// so that the bitangent goes in the same direction as the v tex-coord.
	//   Let r0 be the bottom radius and let r1 be the top radius.
	//   y(v) = h - hv for v in [0,1].
	//   r(v) = r1 + (r0-r1)v
	//
	//   x(t, v) = r(v)*cos(t)
	//   y(t, v) = h - hv
	//   z(t, v) = r(v)*sin(t)
	// 
	//  dx/dt = -r(v)*sin(t)
	//  dy/dt = 0
	//  dz/dt = +r(v)*cos(t)
	//
	//  dx/dv = (r0-r1)*cos(t)
	//  dy/dv = -h
	//  dz/dv = (r0-r1)*sin(t)
	
	//
	// Load Vertices
	//
	
	//NUM_VERTS = ringCount;
	
	//mVerticesVNTT.resize(NUM_VERTS);
	
	switch (mMeshParameters.vertexType)
	{
		case ZShadeSandboxMesh::EVertexType::VT_NormalTex:
		{
#pragma region "Vertex Normal Tex"
			// Compute vertices for each stack ring from the bottom
			for (UINT i = 0; i < ringCount; ++i)
			{
				float y = -0.5 * fHeight + i * stackHeight;
				float r = fBottomRadius + i * radiusStep;
				
				// Vertices of the ring
				float dTheta = 2.0f * PI / iSliceCount;
				
				for(UINT j = 0; j <= iSliceCount; ++j)
				{
					ZShadeSandboxMesh::VertexNormalTex vertex;

					float c = cosf(j*dTheta);
					float s = sinf(j*dTheta);

					vertex.position = XMFLOAT3(r*c, y, r*s);

					vertex.texture.x = (float)j/iSliceCount;
					vertex.texture.y = 1.0f - (float)i/iStackCount;
					
					// This is unit length.
					vertex.normal.x = 0.0f;
					vertex.normal.y = 1.0f;
					vertex.normal.z = 0.0f;

					mAttributes->mVerticesNormalTex.push_back(vertex);
				}
			}
#pragma endregion
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_NormalTexTan:
		{
#pragma region "Vertex Normal Tex Tan"
			// Compute vertices for each stack ring from the bottom
			for (UINT i = 0; i < ringCount; ++i)
			{
				float y = -0.5 * fHeight + i * stackHeight;
				float r = fBottomRadius + i * radiusStep;
				
				// Vertices of the ring
				float dTheta = 2.0f * PI / iSliceCount;
				
				for(UINT j = 0; j <= iSliceCount; ++j)
				{
					ZShadeSandboxMesh::VertexNormalTexTan vertex;

					float c = cosf(j*dTheta);
					float s = sinf(j*dTheta);

					vertex.position = XMFLOAT3(r*c, y, r*s);

					vertex.texture.x = (float)j/iSliceCount;
					vertex.texture.y = 1.0f - (float)i/iStackCount;
					
					// This is unit length.
					vertex.tangentU = XMFLOAT3(-s, 0.0f, c);

					float dr = fBottomRadius-fTopRadius;
					float h = *reinterpret_cast<float*>(&fHeight);
					XMFLOAT3 bitangent(dr*c, -h, dr*s);

					XMVECTOR T = XMLoadFloat3(&vertex.tangentU);
					XMVECTOR B = XMLoadFloat3(&bitangent);
					XMVECTOR N = XMVector3Normalize(XMVector3Cross(T, B));
					XMStoreFloat3(&vertex.normal, N);

					mAttributes->mVerticesNormalTexTan.push_back(vertex);
				}
			}
#pragma endregion
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Pos:
		{
#pragma region "Vertex Pos"
			// Compute vertices for each stack ring from the bottom
			for (UINT i = 0; i < ringCount; ++i)
			{
				float y = -0.5 * fHeight + i * stackHeight;
				float r = fBottomRadius + i * radiusStep;
				
				// Vertices of the ring
				float dTheta = 2.0f * PI / iSliceCount;
				
				for(UINT j = 0; j <= iSliceCount; ++j)
				{
					ZShadeSandboxMesh::VertexPos vertex;

					float c = cosf(j*dTheta);
					float s = sinf(j*dTheta);

					vertex.position = XMFLOAT3(r*c, y, r*s);

					mAttributes->mVerticesPos.push_back(vertex);
				}
			}
#pragma endregion
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Tex:
		{
#pragma region "Vertex Tex"
			// Compute vertices for each stack ring from the bottom
			for (UINT i = 0; i < ringCount; ++i)
			{
				float y = -0.5f * fHeight + i * stackHeight;
				float r = fBottomRadius + i * radiusStep;
				
				// Vertices of the ring
				float dTheta = 2.0f * PI / iSliceCount;
				
				for(UINT j = 0; j <= iSliceCount; ++j)
				{
					ZShadeSandboxMesh::VertexTex vertex;

					float c = cosf(j*dTheta);
					float s = sinf(j*dTheta);

					vertex.position = XMFLOAT3(r*c, y, r*s);

					vertex.texture.x = (float)j/iSliceCount;
					vertex.texture.y = 1.0f - (float)i/iStackCount;

					mAttributes->mVerticesTex.push_back(vertex);
				}
			}
#pragma endregion
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Color:
		{
#pragma region "Vertex Color"
			XMFLOAT4 diffuseColor = mMaterial->vDiffuseColor;

			// Compute vertices for each stack ring from the bottom
			for (UINT i = 0; i < ringCount; ++i)
			{
				float y = -0.5 * fHeight + i * stackHeight;
				float r = fBottomRadius + i * radiusStep;
				
				// Vertices of the ring
				float dTheta = 2.0f * PI / iSliceCount;
				
				for(UINT j = 0; j <= iSliceCount; ++j)
				{
					ZShadeSandboxMesh::VertexColor vertex;

					float c = cosf(j*dTheta);
					float s = sinf(j*dTheta);

					vertex.position = XMFLOAT3(r*c, y, r*s);
					vertex.color = diffuseColor;

					mAttributes->mVerticesColor.push_back(vertex);
				}
			}
#pragma endregion
		}
		break;
	}
	
	//
	// Load Indices
	//
	
	// Add one because we duplicate the first and last vertex per ring
	// since the texture coordinates are different.
	UINT ringVertexCount = iSliceCount + 1;

	// Compute indices for each stack.
	
	//NUM_INDICES = (iStackCount * iSliceCount) * 6;
	
	//mIndexCount = NUM_INDICES;
	
	//mIndices.resize( NUM_INDICES );
	
	// Compute indices for each stack.
	for(UINT i = 0; i < iStackCount; ++i)
	{
		for(UINT j = 0; j < iSliceCount; ++j)
		{
			mAttributes->mIndices.push_back(i*ringVertexCount + j);
			mAttributes->mIndices.push_back((i + 1)*ringVertexCount + j);
			mAttributes->mIndices.push_back((i + 1)*ringVertexCount + j + 1);

			mAttributes->mIndices.push_back(i*ringVertexCount + j);
			mAttributes->mIndices.push_back((i + 1)*ringVertexCount + j + 1);
			mAttributes->mIndices.push_back(i*ringVertexCount + j + 1);
		}
	}

	switch (mMeshParameters.vertexType)
	{
		case ZShadeSandboxMesh::EVertexType::VT_NormalTex: mAttributes->mVertexCount = mAttributes->mVerticesNormalTex.size(); break;
		case ZShadeSandboxMesh::EVertexType::VT_NormalTexTan: mAttributes->mVertexCount = mAttributes->mVerticesNormalTexTan.size(); break;
		case ZShadeSandboxMesh::EVertexType::VT_Pos: mAttributes->mVertexCount = mAttributes->mVerticesPos.size(); break;
		case ZShadeSandboxMesh::EVertexType::VT_Tex: mAttributes->mVertexCount = mAttributes->mVerticesTex.size(); break;
		case ZShadeSandboxMesh::EVertexType::VT_Color: mAttributes->mVertexCount = mAttributes->mVerticesColor.size(); break;
	}

	BuildCylinderTopCap();

	switch (mMeshParameters.vertexType)
	{
		case ZShadeSandboxMesh::EVertexType::VT_NormalTex: mAttributes->mVertexCount = mAttributes->mVerticesNormalTex.size(); break;
		case ZShadeSandboxMesh::EVertexType::VT_NormalTexTan: mAttributes->mVertexCount = mAttributes->mVerticesNormalTexTan.size(); break;
		case ZShadeSandboxMesh::EVertexType::VT_Pos: mAttributes->mVertexCount = mAttributes->mVerticesPos.size(); break;
		case ZShadeSandboxMesh::EVertexType::VT_Tex: mAttributes->mVertexCount = mAttributes->mVerticesTex.size(); break;
		case ZShadeSandboxMesh::EVertexType::VT_Color: mAttributes->mVertexCount = mAttributes->mVerticesColor.size(); break;
	}

	BuildCylinderBottomCap();

	mAttributes->mIndexCount = mAttributes->mIndices.size();

	switch (mMeshParameters.vertexType)
	{
		case ZShadeSandboxMesh::EVertexType::VT_NormalTex: mAttributes->mVertexCount = mAttributes->mVerticesNormalTex.size(); break;
		case ZShadeSandboxMesh::EVertexType::VT_NormalTexTan: mAttributes->mVertexCount = mAttributes->mVerticesNormalTexTan.size(); break;
		case ZShadeSandboxMesh::EVertexType::VT_Pos: mAttributes->mVertexCount = mAttributes->mVerticesPos.size(); break;
		case ZShadeSandboxMesh::EVertexType::VT_Tex: mAttributes->mVertexCount = mAttributes->mVerticesTex.size(); break;
		case ZShadeSandboxMesh::EVertexType::VT_Color: mAttributes->mVertexCount = mAttributes->mVerticesColor.size(); break;
	}
	
	mAttributes->mTriangleCount = mAttributes->mVertexCount / 3;
}
//===============================================================================================================================
