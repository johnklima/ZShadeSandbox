//===============================================================================================================================
// OBJMesh.h
//
//===============================================================================================================================
// History
//
// -Created on 5/18/2015 by henriyl
// -Updated 5/21/2015 for the engine by Dustin Watson
//===============================================================================================================================
#ifndef __OBJMESH_H
#define __OBJMESH_H
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//

#include <vector>
#include <fstream>
#include <istream>
#include "Vertex.h"
#include "D3D.h"
#include "Camera.h"
#include "GameDirectory.h"
#include "OBJMeshShader.h"
#include "OBJMeshTessellationShader.h"
#include "ShaderMaterial.h"
#include "OBJGBufferShader.h"
#include "AABB.h"
#include "SpherePrimitive.h"
#include "MeshAttributes.h"

//===============================================================================================================================
//===============================================================================================================================

typedef unsigned int IndexType;

// SamplerStates etc in hlsl files do nothing if Effect framework is not used
// MaleLow.obj has quads, retriangulated in VS. Some of the triangles still have wrong winding order...

// Need a separate vertex array that includes the real positions of the vertices

namespace ZShadeSandboxMesh
{
class OBJMesh
{
	struct Group
	{
		ZShadeSandboxLighting::ShaderMaterial Material;
		string Name;
		int IndexStart;
		int IndexCount;
	};

	class Mesh
	{
	public:
		
		Mesh(D3D* d3d);
		~Mesh();
		
		bool Load(string filename, bool bRebuildNormals, bool isRHCoordSys);
		void Render(MeshRenderParameters render_param);
		void SetWireframe(bool wire);
		void SetFarPlane(float farPlane);

		void Scale(XMFLOAT3 v);
		void Rotate(XMFLOAT3 v);
		void Position(XMFLOAT3 v);
		
		ZShadeSandboxMath::AABB* GetAABB();
		bool IntersectsAABB(XMFLOAT3 point);
		float DistanceAABBToPoint(XMFLOAT3 point);
		void UpdateAABB(XMFLOAT3 center, XMFLOAT3 scale);
		
		ZShadeSandboxMath::SpherePrimitive* GetSphere();
		bool IntersectsSphere(XMFLOAT3 point);
		float DistanceSphereToPoint(XMFLOAT3 point);

	private:

		//void CalculateWorldMatrix();

		MeshAttributes* mAttributes;

		//int						mVertexCount;
		//int						mIndexCount;
		//int						mTriangleCount;
		//ID3D11Buffer*				mVB;
		//ID3D11Buffer*				mIB;
		//XMMATRIX					mWorld;
		//XMFLOAT3					mPosition;
		//XMFLOAT3					mScale;
		//XMFLOAT3					mRotation;
		
		OBJMeshShader*      	   	mShader;
		OBJMeshTessellationShader*  mTessellationShader;
		OBJGBufferShader*			mOBJGBufferShader;
		D3D*						mD3DSystem;
		vector<Group>				mGroups;

		ZShadeSandboxMath::AABB* mAABB;
		ZShadeSandboxMath::SpherePrimitive* mSphere;
	};

public:

	OBJMesh(D3D* d3d, GameDirectory3D* g3D);
	~OBJMesh();

	void Load(string filename, bool bRebuildNormals = false, bool isRHCoordSys = false);
	void Render(MeshRenderParameters render_param);
	void SetWireframe(bool wire);
	void SetFarPlane(float farPlane);

	void Scale(XMFLOAT3 v);
	void Rotate(XMFLOAT3 v);
	void Position(XMFLOAT3 v);
	
	ZShadeSandboxMath::AABB* GetAABB();
	bool IntersectsAABB(XMFLOAT3 point);
	float DistanceAABBToPoint(XMFLOAT3 point);
	void UpdateAABB(XMFLOAT3 center, XMFLOAT3 scale);
	
	ZShadeSandboxMath::SpherePrimitive* GetSphere();
	bool IntersectsSphere(XMFLOAT3 point);
	float DistanceSphereToPoint(XMFLOAT3 point);
	
private:
	
	static GameDirectory3D* mGD3D;
	static D3D*  mD3DSystem;
	
	Mesh* m_pMesh;
	
	static bool LoadMtl(string filename, D3D* d3d, map<string, ZShadeSandboxLighting::ShaderMaterial>& materials);
	
	static bool LoadObj(
		string filename,
		D3D* d3d,
		vector<VertexNormalTex>& vertices,
		vector<IndexType>& indices,
		vector<Group>& groups,
		bool& hadNormals,
		bool isRHCoordSys
	);
};
}
//===============================================================================================================================
//===============================================================================================================================
#endif//__OBJMESH_H