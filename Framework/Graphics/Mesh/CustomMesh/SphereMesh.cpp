#include "SphereMesh.h"
#include <algorithm>
using ZShadeSandboxMesh::SphereMesh;
//===============================================================================================================================
SphereMesh::SphereMesh(D3D* d3d, ZShadeSandboxMesh::MeshParameters mp, char* filename)
: ZShadeSandboxMesh::CustomMesh(d3d, mp, filename)
{
	mMeshType = ZShadeSandboxMesh::EMeshType::SPHERE;
}
//===============================================================================================================================
SphereMesh::SphereMesh(float radius, UINT sliceCount, UINT stackCount, D3D* d3d, ZShadeSandboxMesh::MeshParameters mp)
:   ZShadeSandboxMesh::CustomMesh( d3d, mp )
,   fRadius(radius)
,   iSliceCount(sliceCount)
,   iStackCount(stackCount)
{
	mMeshType = ZShadeSandboxMesh::EMeshType::SPHERE;

	Initialize();
	
	CreateBuffers();
}
//===============================================================================================================================
SphereMesh::SphereMesh(ZShadeSandboxMath::SpherePrimitive sphere, D3D* d3d, ZShadeSandboxMesh::MeshParameters mp)
:   ZShadeSandboxMesh::CustomMesh( d3d, mp )
{
	// The translated position of the sphere mesh will be the center of the sphere
	ZShadeSandboxMath::Vec3<XMFLOAT3> c = sphere.Center();
	mAttributes->mPosition = XMFLOAT3(c.Vector().x, c.Vector().y, c.Vector().z);
	fRadius = sphere.Radius();
	iSliceCount = sphere.SliceCount();
	iStackCount = sphere.StackCount();
	
	mMeshType = ZShadeSandboxMesh::EMeshType::SPHERE;

	Initialize();
	
	CreateBuffers();
}
//===============================================================================================================================
SphereMesh::~SphereMesh()
{

}
//===============================================================================================================================
void SphereMesh::Initialize()
{
	float phi   = PI / iStackCount;
	float theta = 2.0f * PI / iSliceCount;
	
	int NUM_SPHERE_VERTS   = 0;
	int NUM_SPHERE_INDICES = 0;
	
	//
	// Load Vertices
	//
	
	//NUM_SPHERE_VERTS = (iStackCount - 2) * (iSliceCount - 1);
	
	switch (mMeshParameters.vertexType)
	{
		case ZShadeSandboxMesh::EVertexType::VT_NormalTex:
		{
			//mAttributes->mVerticesNormalTex.resize( NUM_SPHERE_VERTS );
			
			// Create the poles of the sphere
			ZShadeSandboxMesh::VertexNormalTex topVertex = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(0.0f, fRadius, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
			ZShadeSandboxMesh::VertexNormalTex bottomVertex = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(0.0f, -fRadius, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f);
			
			// Add the top vertex of the sphere
			mAttributes->mVerticesNormalTex.push_back(topVertex);
			
			UINT index = 0;
			
			// Create the vertices for each stack ring (poles not included)
			for (UINT i = 1; i <= iStackCount - 1; ++i)
			{
				float p = i * phi;
				
				// Create the vertices of the ring
				for (UINT j = 0; j <= iSliceCount; ++j)
				{
					float t = j * theta;
					
					ZShadeSandboxMesh::VertexNormalTex vertex;
					
					// ZShadeSandboxGlobal::Convert from Spherical to Cartesian
					vertex.position.x = fRadius * sinf(p) * cosf(t);
					vertex.position.y = fRadius * cosf(p);
					vertex.position.z = fRadius * sinf(p) * sinf(t); // Last one was cosf
					
					XMVECTOR pos = XMLoadFloat3(&vertex.position);
					XMStoreFloat3(&vertex.normal, XMVector3Normalize(pos));

					// Calculate the texture coordinates of the sphere
					vertex.texture.x = t / TWO_PI;
					vertex.texture.y = p / PI;
					
					mAttributes->mVerticesNormalTex.push_back(vertex);
				}
			}
			
			mAttributes->mVerticesNormalTex.push_back(bottomVertex);

			mAttributes->mVertexCount = mAttributes->mVerticesNormalTex.size();
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_NormalTexTan:
		{
			//mAttributes->mVerticesNormalTexTan.resize( NUM_SPHERE_VERTS );
			
			// Create the poles of the sphere
			ZShadeSandboxMesh::VertexNormalTexTan topVertex = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(0.0f, fRadius, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTexTan bottomVertex = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(0.0f, -fRadius, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
			
			// Add the top vertex of the sphere
			mAttributes->mVerticesNormalTexTan.push_back(topVertex);
			
			UINT index = 0;
			
			// Create the vertices for each stack ring (poles not included)
			for (UINT i = 1; i <= iStackCount - 1; ++i)
			{
				float p = i * phi;
				
				// Create the vertices of the ring
				for (UINT j = 0; j <= iSliceCount; ++j)
				{
					float t = j * theta;
					
					ZShadeSandboxMesh::VertexNormalTexTan vertex;
					
					// ZShadeSandboxGlobal::Convert from Spherical to Cartesian
					vertex.position.x = fRadius * sinf(p) * cosf(t);
					vertex.position.y = fRadius * cosf(p);
					vertex.position.z = fRadius * sinf(p) * sinf(t); // Last one was cosf
					
					XMVECTOR pos = XMLoadFloat3(&vertex.position);
					XMStoreFloat3(&vertex.normal, XMVector3Normalize(pos));
					
					// Calculate the texture coordinates of the sphere
					vertex.texture.x = t / TWO_PI;
					vertex.texture.y = p / PI;
					
					// Partial derivative of P with respect to theta
					vertex.tangentU.x = -fRadius * sinf(p) * sinf(t);
					vertex.tangentU.y = 0.0f;
					vertex.tangentU.z = fRadius * sinf(p) * sinf(t);
					
					XMVECTOR T = XMLoadFloat3(&vertex.tangentU);
					XMStoreFloat3(&vertex.tangentU, XMVector3Normalize(T));

					mAttributes->mVerticesNormalTexTan.push_back(vertex);
				}
			}
			
			mAttributes->mVerticesNormalTexTan.push_back(bottomVertex);

			mAttributes->mVertexCount = mAttributes->mVerticesNormalTexTan.size();
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Pos:
		{
			//mAttributes->mVerticesPos.resize( NUM_SPHERE_VERTS );
			
			// Create the poles of the sphere
			ZShadeSandboxMesh::VertexPos topVertex = ZShadeSandboxMesh::VertexUtil::LoadVertexPos(0.0f, fRadius, 0.0f);
			ZShadeSandboxMesh::VertexPos bottomVertex = ZShadeSandboxMesh::VertexUtil::LoadVertexPos(0.0f, -fRadius, 0.0f);
			
			// Add the top vertex of the sphere
			mAttributes->mVerticesPos.push_back(topVertex);
			
			UINT index = 0;
			
			// Create the vertices for each stack ring (poles not included)
			for (UINT i = 1; i <= iStackCount - 1; ++i)
			{
				float p = i * phi;
				
				// Create the vertices of the ring
				for (UINT j = 0; j <= iSliceCount; ++j)
				{
					float t = j * theta;
					
					ZShadeSandboxMesh::VertexPos vertex;
					
					// ZShadeSandboxGlobal::Convert from Spherical to Cartesian
					vertex.position.x = fRadius * sinf(p) * cosf(t);
					vertex.position.y = fRadius * cosf(p);
					vertex.position.z = fRadius * sinf(p) * sinf(t); // Last one was cosf
					
					mAttributes->mVerticesPos[index++] = vertex;
				}
			}
			
			mAttributes->mVerticesPos.push_back(bottomVertex);

			mAttributes->mVertexCount = mAttributes->mVerticesPos.size();
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Tex:
		{
			//mAttributes->mVerticesTex.resize( NUM_SPHERE_VERTS );
			
			// Create the poles of the sphere
			ZShadeSandboxMesh::VertexTex topVertex = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(0.0f, fRadius, 0.0f, 0.0f, 1.0f);
			ZShadeSandboxMesh::VertexTex bottomVertex = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(0.0f, -fRadius, 0.0f, 0.0f, -1.0f);
			
			// Add the top vertex of the sphere
			//mAttributes->mVerticesTex[0] = topVertex;
			mAttributes->mVerticesTex.push_back(topVertex);
			
			UINT index = 0;
			
			// Create the vertices for each stack ring (poles not included)
			for (UINT i = 1; i <= iStackCount - 1; ++i)
			{
				float p = i * phi;
				
				// Create the vertices of the ring
				for (UINT j = 0; j <= iSliceCount; ++j)
				{
					float t = j * theta;
					
					ZShadeSandboxMesh::VertexTex vertex;
					
					// ZShadeSandboxGlobal::Convert from Spherical to Cartesian
					vertex.position.x = fRadius * sinf(p) * cosf(t);
					vertex.position.y = fRadius * cosf(p);
					vertex.position.z = fRadius * sinf(p) * sinf(t); // Last one was cosf
					
					// Calculate the texture coordinates of the sphere
					vertex.texture.x = t / TWO_PI;
					vertex.texture.y = p / PI;
					
					//mAttributes->mVerticesTex[index++] = vertex;
					mAttributes->mVerticesTex.push_back(vertex);
				}
			}
			
			mAttributes->mVerticesTex.push_back(bottomVertex);

			mAttributes->mVertexCount = mAttributes->mVerticesTex.size();
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Color:
		{
			//mAttributes->mVerticesColor.resize( NUM_SPHERE_VERTS );
			
			XMFLOAT4 diffuseColor = mMaterial->vDiffuseColor;

			// Create the poles of the sphere
			ZShadeSandboxMesh::VertexColor topVertex = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(0.0f, fRadius, 0.0f, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
			ZShadeSandboxMesh::VertexColor bottomVertex = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(0.0f, -fRadius, 0.0f, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
			
			// Add the top vertex of the sphere
			mAttributes->mVerticesColor.push_back(topVertex);
			
			UINT index = 0;
			
			// Create the vertices for each stack ring (poles not included)
			for (UINT i = 1; i <= iStackCount - 1; ++i)
			{
				float p = i * phi;
				
				// Create the vertices of the ring
				for (UINT j = 0; j <= iSliceCount; ++j)
				{
					float t = j * theta;
					
					ZShadeSandboxMesh::VertexColor vertex;
					
					// ZShadeSandboxGlobal::Convert from Spherical to Cartesian
					vertex.position.x = fRadius * sinf(p) * cosf(t);
					vertex.position.y = fRadius * cosf(p);
					vertex.position.z = fRadius * sinf(p) * sinf(t); // Last one was cosf
					
					// Add the color for the vertex
					vertex.color = diffuseColor;
					
					mAttributes->mVerticesColor.push_back(vertex);
				}
			}
			
			mAttributes->mVerticesColor.push_back(bottomVertex);

			mAttributes->mVertexCount = mAttributes->mVerticesColor.size();
		}
		break;
	}
	
	mAttributes->mTriangleCount = mAttributes->mVertexCount / 3;
	
	//
	// Load Indices
	//
	
	NUM_SPHERE_INDICES = (iSliceCount * 3) + (((iStackCount - 1) * iSliceCount) * 6) + (iSliceCount * 3);
	
	//mIndexCount = NUM_SPHERE_INDICES;
	
	//mAttributes->mIndices.resize( NUM_SPHERE_INDICES );
	
	// Create the indices for the top stack
	int index = 0;
	for (UINT i = 1; i <= iSliceCount; ++i)
	{
		mAttributes->mIndices.push_back(0);
		mAttributes->mIndices.push_back(i + 1);
		mAttributes->mIndices.push_back(i);
	}
	
	// Create the indices for the inner stack (poles not included)
	UINT baseIndex = 1;
	UINT ringVertexCount = iSliceCount + 1;
	index = 0;
	
	for (UINT i = 0; i <= iStackCount - 2; ++i)
	{
		for (UINT j = 0; j <= iSliceCount; ++j)
		{
			mAttributes->mIndices.push_back(baseIndex + i * ringVertexCount + j);
			mAttributes->mIndices.push_back(baseIndex + i * ringVertexCount + j + 1);
			mAttributes->mIndices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
			
			mAttributes->mIndices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
			mAttributes->mIndices.push_back(baseIndex + i * ringVertexCount + j + 1);
			mAttributes->mIndices.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
		}
	}
	
	// Create the indices for the bottom stack
	UINT southPoleIndex = (UINT)mAttributes->mVertexCount - 1;
	
	// Offset the indices to the index of the first vertex in the last ring
	baseIndex = southPoleIndex - ringVertexCount;
	
	index = 0;
	for (UINT i = 0; i < iSliceCount; ++i)
	{
		mAttributes->mIndices.push_back(southPoleIndex);
		mAttributes->mIndices.push_back(baseIndex + i);
		mAttributes->mIndices.push_back(baseIndex + i + 1);
	}

	mAttributes->mIndexCount = mAttributes->mIndices.size();
}
//===============================================================================================================================
ZShadeSandboxMesh::CustomMesh* SphereMesh::Clone()
{
	SphereMesh* mesh = new SphereMesh(fRadius, iSliceCount, iStackCount, mD3DSystem, mMeshParameters);

	return mesh;
}
//===============================================================================================================================