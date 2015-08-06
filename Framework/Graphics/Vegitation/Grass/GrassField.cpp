//#include "stdafx.h"
#include "GrassField.h"
#include <math.h>
//===============================================================================================================================
float frac(float f)
{
	return f-(long)f;
}
//===============================================================================================================================
float dot(XMFLOAT2 p, XMFLOAT2 r)
{
	return (p.x * r.x) + (p.y * r.y);
}
//===============================================================================================================================
float RandVertex( float x, float y )
{
	XMFLOAT2 p = XMFLOAT2(x, y);
	// We need irrationals for pseudo randomness.
	// Most (all?) known transcendental numbers will (generally) work.
	const XMFLOAT2 r = XMFLOAT2(
		23.1406926327792690,  // e^pi (Gelfond's constant)
		2.6651441426902251); // 2^sqrt(2) (Gelfond–Schneider constant)
	int a = 1.0e-7;
	float t = 123456789 % a;
	return frac( cos( t + 256 * dot(p,r) ) );
}
//===============================================================================================================================
float rand_1_05( float x, float y )
{
	XMFLOAT2 uv = XMFLOAT2(x,y);
	XMFLOAT2 t(12.9898, 78.233);
	t.x *= 2;
	t.y *= 2;
	float d = dot(uv, t);
	float s = sin(d);
    float noise = frac(s) * 43758.5453;
    return abs(noise) * 0.5;
}
//===============================================================================================================================
//GrassMesh::GrassMesh(D3D* d3d, int fieldSize, int width, XMFLOAT3 Vertex, XMFLOAT3 rot, XMFLOAT3 scale, string textureName)
GrassMesh::GrassMesh(D3D* d3d, int fieldSize, int width, ZShadeSandboxMesh::MeshParameters mp)
:	ZShadeSandboxMesh::CustomMesh( d3d, mp )
,	m_RenderShader(true)
,	m_FieldSize(fieldSize)
,	m_Width(width)
{
	mMeshType = ZShadeSandboxMesh::EMeshType::CUSTOM;
}
//===============================================================================================================================
GrassMesh::~GrassMesh()
{

}
//===============================================================================================================================
void GrassMesh::SetGrassSRV(ID3D11ShaderResourceView* srv)
{

}
//===============================================================================================================================
/*void GrassMesh::Render(Camera* pCamera)
{
	//
	// Render the mesh buffers
	//

	SetBuffersNoIndex(TriangleStrip);

	//
	// Render the shader
	//

	Shade( pCamera, WorldXM(), pCamera->View(), XMFLOAT4(0,0,0,0), false, true );
}
//===============================================================================================================================
void GrassMesh::RenderClip(Camera* pCamera, XMMATRIX view, XMFLOAT4 clipplane, RenderType rt, bool colorShader, bool texShader, ZShadeSandboxShader::Shader* shader)
{
	//
	// Render the mesh buffers
	//

	SetBuffersNoIndex(rt);

	//
	// Render the shader
	//

	Shade( pCamera, WorldXM(), view, clipplane, colorShader, texShader, shader );
}
//===============================================================================================================================
void GrassMesh::Update(float dt)
{

}*/
//===============================================================================================================================
void GrassMesh::Initialize()
{

}
//===============================================================================================================================
void GrassMesh::AddQuadConnector(XMFLOAT3 fStatisVertexOneIn, XMFLOAT3 fStatisVertexTwoIn,
								 XMFLOAT3& fStatisVertexOneOut, XMFLOAT3& fStatisVertexTwoOut,
								 XMFLOAT3 fOscillationVertexOneIn, XMFLOAT3 fOscillationVertexTwoIn,
								 XMFLOAT3& fOscillationVertexOneOut, XMFLOAT3& fOscillationVertexTwoOut,
								 float rnum, float windCoeff, bool odd, bool isTop, bool oscillate)
{
	QuadConnector qconn;

	float rand_Vertex = RandVertex(fStatisVertexOneIn.x, fStatisVertexOneIn.z);


}
//===============================================================================================================================
//===============================================================================================================================
//===============================================================================================================================
GGrassField::GGrassField(D3D* d3d)
: m_pD3DSystem(d3d)
{
}
//===============================================================================================================================
GGrassField::~GGrassField()
{

}
//===============================================================================================================================
void GGrassField::Initialize(int fieldSize, int width)
{
	
}
//===============================================================================================================================
/*void GGrassField::Update(float dt)
{

}
//===============================================================================================================================
void GGrassField::Render(Camera* camera)
{

}
//===============================================================================================================================
*/