#include "GeosphereMesh.h"
#include "ZMath.h"
#include <algorithm>
using ZShadeSandboxMesh::GeosphereMesh;
//===============================================================================================================================
GeosphereMesh::GeosphereMesh(float radius, UINT subdivisions, D3D* d3d, ZShadeSandboxMesh::MeshParameters mp)
:   ZShadeSandboxMesh::CustomMesh( d3d, mp )
,   fRadius(radius)
,   iSubdivisions(subdivisions)
{
	mMeshType = ZShadeSandboxMesh::EMeshType::GEOSPHERE;

	Initialize();
	
	CreateBuffers();
}
//===============================================================================================================================
GeosphereMesh::~GeosphereMesh()
{

}
//===============================================================================================================================
void GeosphereMesh::Initialize()
{
	int NUM_SPHERE_VERTS   = 0;
	int NUM_SPHERE_INDICES = 0;
	
	// Approximate a sphere by tessellating an icosahedron.
	
	// Make sure the number of subdivisions do not get out of control
	iSubdivisions = ZShadeSandboxMath::ZMath::Min(iSubdivisions, 5u);
	
	const float X = 0.525731f; 
	const float Z = 0.850651f;

	XMFLOAT3 pos[12] = 
	{
		XMFLOAT3(-X, 0.0f, Z),  XMFLOAT3(X, 0.0f, Z),  
		XMFLOAT3(-X, 0.0f, -Z), XMFLOAT3(X, 0.0f, -Z),    
		XMFLOAT3(0.0f, Z, X),   XMFLOAT3(0.0f, Z, -X), 
		XMFLOAT3(0.0f, -Z, X),  XMFLOAT3(0.0f, -Z, -X),    
		XMFLOAT3(Z, X, 0.0f),   XMFLOAT3(-Z, X, 0.0f), 
		XMFLOAT3(Z, -X, 0.0f),  XMFLOAT3(-Z, -X, 0.0f)
	};

	DWORD k[60] = 
	{
		1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,    
		1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,    
		3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0, 
		10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7 
	};
	
	//
	// Load Vertices
	//
	
	NUM_SPHERE_VERTS = 12;
	
	switch (mMeshParameters.vertexType)
	{
		case ZShadeSandboxMesh::EVertexType::VT_NormalTex:
		{
#pragma region "Vertex Normal Tex"
			mAttributes->mVerticesNormalTex.resize(NUM_SPHERE_VERTS);
			
			for(UINT i = 0; i < NUM_SPHERE_VERTS; ++i)
				mAttributes->mVerticesNormalTex[i].position = pos[i];
			
			mAttributes->mVertexCount = NUM_SPHERE_VERTS;
			
			//
			// Load Indices
			//
			
			NUM_SPHERE_INDICES = 60;
			mAttributes->mIndexCount = NUM_SPHERE_INDICES;
			mAttributes->mIndices.resize(NUM_SPHERE_INDICES);
			
			for(UINT i = 0; i < NUM_SPHERE_INDICES; ++i)
				mAttributes->mIndices[i] = k[i];
			
			//
			// Subdivide and create the mesh
			//
			
			for (UINT i = 0; i < iSubdivisions; ++i)
			{
				mAttributes->Subdivide(mMeshParameters.vertexType);
			}
			
			ZShadeSandboxMesh::VertexNormalTex vertex;

			// Project vertices onto sphere and scale.
			for (UINT i = 0; i < mAttributes->mVertexCount; ++i)
			{
				// Project onto a unit sphere
				ZShadeSandboxMath::Vec3<XMFLOAT3> n = mAttributes->mVerticesNormalTex[i].position;
				
				n.Normalize();
				
				// Project onto a sphere
				ZShadeSandboxMath::Vec3<XMFLOAT3> p = n;
				
				p *= fRadius;

				vertex.position = p;
				vertex.normal = n;
				
				// Derive the texture coordinates from spherical coordinates
				float theta = ZShadeSandboxMath::ZMath::AngleFromXY(vertex.position.x, vertex.position.z);
				
				float phi = acosf(vertex.position.y / fRadius);
				
				vertex.texture.x = theta / TWO_PI;
				vertex.texture.y = phi / PI;

				mAttributes->mVerticesNormalTex[i] = vertex;
			}
#pragma endregion
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_NormalTexTan:
		{
#pragma region "Vertex Normal Tex Tan"
			mAttributes->mVerticesNormalTexTan.resize(NUM_SPHERE_VERTS);
			
			for(UINT i = 0; i < NUM_SPHERE_VERTS; ++i)
				mAttributes->mVerticesNormalTexTan[i].position = pos[i];
			
			mAttributes->mVertexCount = NUM_SPHERE_VERTS;
			
			//
			// Load Indices
			//
			
			NUM_SPHERE_INDICES = 60;
			mAttributes->mIndexCount = NUM_SPHERE_INDICES;
			mAttributes->mIndices.resize(NUM_SPHERE_INDICES);
			
			for(UINT i = 0; i < NUM_SPHERE_INDICES; ++i)
				mAttributes->mIndices[i] = k[i];
			
			//
			// Subdivide and create the mesh
			//
			
			for (UINT i = 0; i < iSubdivisions; ++i)
			{
				mAttributes->Subdivide(mMeshParameters.vertexType);
			}
			
			ZShadeSandboxMesh::VertexNormalTexTan vertex;

			// Project vertices onto sphere and scale.
			for (UINT i = 0; i < mAttributes->mVertexCount; ++i)
			{
				// Project onto a unit sphere
				ZShadeSandboxMath::Vec3<XMFLOAT3> n = mAttributes->mVerticesNormalTexTan[i].position;
				
				n.Normalize();
				
				// Project onto a sphere
				ZShadeSandboxMath::Vec3<XMFLOAT3> p = n;
				
				p *= fRadius;

				vertex.position = p;
				vertex.normal = n;
				
				// Derive the texture coordinates from spherical coordinates
				float theta = ZShadeSandboxMath::ZMath::AngleFromXY(vertex.position.x, vertex.position.z);
				
				float phi = acosf(vertex.position.y / fRadius);
				
				vertex.texture.x = theta / TWO_PI;
				vertex.texture.y = phi / PI;
				
				// Partial derivative of P with respect to theta
				vertex.tangentU.x = -fRadius * sinf(phi) * sinf(theta);
				vertex.tangentU.y = 0.0f;
				vertex.tangentU.z = fRadius * sinf(phi) * sinf(theta);
				
				ZShadeSandboxMath::Vec3<XMFLOAT3> T = vertex.tangentU;
				
				T.Normalize();
				
				vertex.tangentU = T.Vector();

				mAttributes->mVerticesNormalTexTan[i] = vertex;
			}
#pragma endregion
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Pos:
		{
#pragma region "Vertex Pos"
			mAttributes->mVerticesPos.resize(NUM_SPHERE_VERTS);
			
			for(UINT i = 0; i < NUM_SPHERE_VERTS; ++i)
				mAttributes->mVerticesPos[i].position = pos[i];
			
			mAttributes->mVertexCount = NUM_SPHERE_VERTS;
			
			//
			// Load Indices
			//
			
			NUM_SPHERE_INDICES = 60;
			mAttributes->mIndexCount = NUM_SPHERE_INDICES;
			mAttributes->mIndices.resize(NUM_SPHERE_INDICES);
			
			for(UINT i = 0; i < NUM_SPHERE_INDICES; ++i)
				mAttributes->mIndices[i] = k[i];
			
			//
			// Subdivide and create the mesh
			//
			
			for (UINT i = 0; i < iSubdivisions; ++i)
			{
				mAttributes->Subdivide(mMeshParameters.vertexType);
			}
			
			ZShadeSandboxMesh::VertexPos vertex;

			// Project vertices onto sphere and scale.
			for (UINT i = 0; i < mAttributes->mVertexCount; ++i)
			{
				// Project onto a unit sphere
				ZShadeSandboxMath::Vec3<XMFLOAT3> n = mAttributes->mVerticesPos[i].position;
				
				n.Normalize();
				
				// Project onto a sphere
				ZShadeSandboxMath::Vec3<XMFLOAT3> p = n;
				
				p *= fRadius;

				vertex.position = p;

				mAttributes->mVerticesPos[i] = vertex;
			}
#pragma endregion
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Tex:
		{
#pragma region "Vertex Tex"
			mAttributes->mVerticesTex.resize(NUM_SPHERE_VERTS);
			
			for(UINT i = 0; i < NUM_SPHERE_VERTS; ++i)
				mAttributes->mVerticesTex[i].position = pos[i];
			
			mAttributes->mVertexCount = NUM_SPHERE_VERTS;
			
			//
			// Load Indices
			//
			
			NUM_SPHERE_INDICES = 60;
			mAttributes->mIndexCount = NUM_SPHERE_INDICES;
			mAttributes->mIndices.resize(NUM_SPHERE_INDICES);
			
			for(UINT i = 0; i < NUM_SPHERE_INDICES; ++i)
				mAttributes->mIndices[i] = k[i];
			
			//
			// Subdivide and create the mesh
			//
			
			for (UINT i = 0; i < iSubdivisions; ++i)
			{
				mAttributes->Subdivide(mMeshParameters.vertexType);
			}
			
			ZShadeSandboxMesh::VertexTex vertex;

			// Project vertices onto sphere and scale.
			for (UINT i = 0; i < mAttributes->mVertexCount; ++i)
			{
				// Project onto a unit sphere
				ZShadeSandboxMath::Vec3<XMFLOAT3> n = mAttributes->mVerticesTex[i].position;
				
				n.Normalize();
				
				// Project onto a sphere
				ZShadeSandboxMath::Vec3<XMFLOAT3> p = n;
				
				p *= fRadius;

				vertex.position = p;
				
				// Derive the texture coordinates from spherical coordinates
				float theta = ZShadeSandboxMath::ZMath::AngleFromXY(vertex.position.x, vertex.position.z);
				
				float phi = acosf(vertex.position.y / fRadius);
				
				vertex.texture.x = theta / TWO_PI;
				vertex.texture.y = phi / PI;

				mAttributes->mVerticesTex[i] = vertex;
			}
#pragma endregion
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Color:
		{
#pragma region "Vertex Color"
			XMFLOAT4 diffuseColor = mMaterial->vDiffuseColor;

			mAttributes->mVerticesColor.resize(NUM_SPHERE_VERTS);
			
			for(UINT i = 0; i < NUM_SPHERE_VERTS; ++i)
				mAttributes->mVerticesColor[i].position = pos[i];
			
			mAttributes->mVertexCount = NUM_SPHERE_VERTS;
			
			//
			// Load Indices
			//
			
			NUM_SPHERE_INDICES = 60;
			mAttributes->mIndexCount = NUM_SPHERE_INDICES;
			mAttributes->mIndices.resize(NUM_SPHERE_INDICES);
			
			for(UINT i = 0; i < NUM_SPHERE_INDICES; ++i)
				mAttributes->mIndices[i] = k[i];
			
			//
			// Subdivide and create the mesh
			//
			
			for (UINT i = 0; i < iSubdivisions; ++i)
			{
				mAttributes->Subdivide(mMeshParameters.vertexType);
			}
			
			ZShadeSandboxMesh::VertexColor vertex;

			// Project vertices onto sphere and scale.
			for (UINT i = 0; i < mAttributes->mVertexCount; ++i)
			{
				// Project onto a unit sphere
				ZShadeSandboxMath::Vec3<XMFLOAT3> n = mAttributes->mVerticesColor[i].position;
				
				n.Normalize();
				
				// Project onto a sphere
				ZShadeSandboxMath::Vec3<XMFLOAT3> p = n;
				
				p *= fRadius;

				vertex.position = p;
				vertex.color = diffuseColor;

				mAttributes->mVerticesColor[i] = vertex;
			}
#pragma endregion
		}
		break;
	}
	
	mAttributes->mTriangleCount = mAttributes->mVertexCount / 3;
}
//===============================================================================================================================
