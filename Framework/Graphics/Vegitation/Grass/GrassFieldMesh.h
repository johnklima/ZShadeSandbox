//===============================================================================================================================
// GrassFieldMesh
//
//===============================================================================================================================
// History
//
// -Created on 7/13/2014 by Dustin Watson
//===============================================================================================================================
#ifndef __GRASSFIELDMESH_H
#define __GRASSFIELDMESH_H
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//

#include "CustomMesh.h"
#include "Camera.h"
#include "GrassFieldShader.h"

//===============================================================================================================================
//===============================================================================================================================
class GrassFieldMesh : public ZShadeSandboxMesh::CustomMesh
{
	vector<float> m_Heightmap;
	GrassFieldShader* m_pGrassFieldShader;
	bool m_RenderShader;
	int m_FieldSize;
	int m_Width;
public:

	//GrassFieldMesh(D3D* d3d, int fieldSize, int width, vector<float> heightmap, XMFLOAT3 Vertex = XMFLOAT3(0,0,0), XMFLOAT3 rot = XMFLOAT3(0,0,0), XMFLOAT3 scale = XMFLOAT3(1,1,1), string textureName = "");
	GrassFieldMesh(D3D* d3d, int fieldSize, int width, vector<float> heightmap, ZShadeSandboxMesh::MeshParameters mp);
	~GrassFieldMesh();

	void SetHeightmap(vector<float> heightmap) { m_Heightmap = heightmap; }

	void SetGrassAlphaTopSRV(ID3D11ShaderResourceView* srv);
	void SetGrassDiffuseTopSRV(ID3D11ShaderResourceView* srv);
	void SetGrassAlphaBodySRV(ID3D11ShaderResourceView* srv);
	void SetGrassDiffuseBodySRV(ID3D11ShaderResourceView* srv);

	//void Render(Camera* pCamera);//, RenderType rt = TriangleList, bool colorShader = true, bool texShader = false, ZShadeSandboxShader::Shader* shader = 0);
	//void RenderClip(Camera* pCamera, XMMATRIX view, XMFLOAT4 clipplane, RenderType rt = TriangleList, bool colorShader = true, bool texShader = false, ZShadeSandboxShader::Shader* shader = 0);
	//void Update(float dt);
	void Initialize();
};
//===============================================================================================================================
//===============================================================================================================================
#endif//__GRASSFIELDMESH_H