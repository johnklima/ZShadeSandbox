//===============================================================================================================================
// Grass
//
//===============================================================================================================================
// History
//
// -Created on 9/13/2014 by Dustin Watson
//===============================================================================================================================
#ifndef __GRASSFIELD_H
#define __GRASSFIELD_H
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//

#include "D3D.h"
#include "Camera.h"
//#include "GrassFieldMesh.h"
#include "CustomMesh.h"

/*
	Create a grass field in c++ with all Quad connections in code and do not use the Geometry ZShadeSandboxShader::Shader just a Texture ZShadeSandboxShader::Shader
*/

struct QuadConnector
{
	XMFLOAT3 Vertex;
	XMFLOAT2 tex;
	float       rnum;
	XMFLOAT4 color;
};

class GrassMesh : public ZShadeSandboxMesh::CustomMesh
{
	//GrassFieldShader* m_pGrassFieldShader;
	bool m_RenderShader;
	int m_FieldSize;
	int m_Width;

	vector<QuadConnector> m_QuadConnectors;

public:

	//GrassMesh(D3D* d3d, int fieldSize, int width, XMFLOAT3 Vertex = XMFLOAT3(0,0,0), XMFLOAT3 rot = XMFLOAT3(0,0,0), XMFLOAT3 scale = XMFLOAT3(1,1,1), string textureName = "");
	GrassMesh(D3D* d3d, int fieldSize, int width, ZShadeSandboxMesh::MeshParameters mp);
	~GrassMesh();

	void SetGrassSRV(ID3D11ShaderResourceView* srv);
	
	void AddQuadConnector(XMFLOAT3 fStatisVertexOneIn, XMFLOAT3 fStatisVertexTwoIn, XMFLOAT3& fStatisVertexOneOut, XMFLOAT3& fStatisVertexTwoOut, XMFLOAT3 fOscillationVertexOneIn, XMFLOAT3 fOscillationVertexTwoIn, XMFLOAT3& fOscillationVertexOneOut, XMFLOAT3& fOscillationVertexTwoOut, float rnum, float windCoeff, bool odd, bool isTop, bool oscillate);

	//void Render(Camera* pCamera);//, RenderType rt = TriangleList, bool colorShader = true, bool texShader = false, ZShadeSandboxShader::Shader* shader = 0);
	//void RenderClip(Camera* pCamera, XMMATRIX view, XMFLOAT4 clipplane, RenderType rt = TriangleList, bool colorShader = true, bool texShader = false, ZShadeSandboxShader::Shader* shader = 0);
	//void Update(float dt);
	void Initialize();
};

//===============================================================================================================================
//===============================================================================================================================
class GGrassField
{
	D3D* m_pD3DSystem;
	GrassMesh* mesh;
public:
	GGrassField(D3D* d3d);
	~GGrassField();

	void Initialize(int fieldSize, int width);

	//void Update(float dt);
	//void Render(Camera* camera);
};
//===============================================================================================================================
//===============================================================================================================================
#endif//__GRASSFIELD_H