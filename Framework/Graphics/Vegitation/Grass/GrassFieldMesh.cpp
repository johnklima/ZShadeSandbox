//#include "stdafx.h"
#include "GrassFieldMesh.h"
#include <algorithm>
#include "ZMath.h"
//===============================================================================================================================
//GrassFieldMesh::GrassFieldMesh(D3D* d3d, int fieldSize, int width, vector<float> heightmap, XMFLOAT3 Vertex, XMFLOAT3 rot, XMFLOAT3 scale, string textureName)
GrassFieldMesh::GrassFieldMesh(D3D* d3d, int fieldSize, int width, vector<float> heightmap, ZShadeSandboxMesh::MeshParameters mp)
:	ZShadeSandboxMesh::CustomMesh( d3d, mp )
,	m_RenderShader(true)
,	m_FieldSize(fieldSize)
,	m_Width(width)
,	m_Heightmap(heightmap)
{
	mMeshType = ZShadeSandboxMesh::EMeshType::CUSTOM;

	m_pGrassFieldShader = new GrassFieldShader(mD3DSystem);//, "Vegitation\\Grass\\GrassField.fxo");

	mp.shader = m_pGrassFieldShader;
	mp.useCustomShader = true;

	Initialize();
	CreateBuffers();
}
//===============================================================================================================================
GrassFieldMesh::~GrassFieldMesh()
{

}
//===============================================================================================================================
void GrassFieldMesh::SetGrassAlphaTopSRV(ID3D11ShaderResourceView* srv)
{
	//m_pGrassFieldShader->SetGrassAlphaTopSRV(srv);
}
//===============================================================================================================================
void GrassFieldMesh::SetGrassDiffuseTopSRV(ID3D11ShaderResourceView* srv)
{
	//m_pGrassFieldShader->SetGrassDiffuseTopSRV(srv);
}
//===============================================================================================================================
void GrassFieldMesh::SetGrassAlphaBodySRV(ID3D11ShaderResourceView* srv)
{
	//m_pGrassFieldShader->SetGrassAlphaBodySRV(srv);
}
//===============================================================================================================================
void GrassFieldMesh::SetGrassDiffuseBodySRV(ID3D11ShaderResourceView* srv)
{
	//m_pGrassFieldShader->SetGrassDiffuseBodySRV(srv);
}
//===============================================================================================================================
/*void GrassFieldMesh::Render(Camera* pCamera)//, RenderType rt, bool colorShader, bool texShader, ZShadeSandboxShader::Shader* shader)
{
	//
	// Render the mesh buffers
	//

	SetBuffersNoIndex(TriangleStrip);
	//SetBuffersNoIndex(PointList);

	//
	// Render the shader
	//

	/*
	m_pGrassFieldShader->SetHalfScale( mScale.y );
	m_pGrassFieldShader->SetKHalfPi( PI / 2 );
	m_pGrassFieldShader->SetPix2( 6.28318530f );
	m_pGrassFieldShader->SetWindVec( XMFLOAT2(0.2f, 0.4f) );
	m_pGrassFieldShader->SetWindCoeff( 0.8f );
	//m_pGrassFieldShader->SetWaveDistortX( XMFLOAT4(0.2f, 0.4f, 0.0f, 0.3f) );
	//m_pGrassFieldShader->SetWaveDistortY( XMFLOAT4(-0.0f, -0.13f, -0.33f, -0.10f) );
	//m_pGrassFieldShader->SetWaveDistortZ( XMFLOAT4(0.1f, 0.4f, 0.0f, 0.3f) );
	m_pGrassFieldShader->SetWaveDistortX( XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f) );
	m_pGrassFieldShader->SetWaveDistortY( XMFLOAT4(-0.0f, -0.0f, -0.0f, -0.0f) );
	m_pGrassFieldShader->SetWaveDistortZ( XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f) );
	m_pGrassFieldShader->SetLightingWaveScale( XMFLOAT4( 0.35f, 0.10f, 0.10f, 0.03f ) );
	m_pGrassFieldShader->SetLightingScaleBias( XMFLOAT4( 0.6f, 0.7f, 0.2f, 0.0f ) );
	m_pGrassFieldShader->SetWaveSpeed( XMFLOAT4( 0.1f, 0.1f, 0.06f, 0.04f ) );
	m_pGrassFieldShader->SetEyeVertex( pCamera->position() );
	m_pGrassFieldShader->SetNumVerts( mVertexCount );
	*

	Shade(pCamera, WorldXM(), pCamera->View(), XMFLOAT4(0, 0, 0, 0), false);
	//Shade( pCamera, WorldXM(), pCamera->View(), XMFLOAT4(0,0,0,0), false, false, m_pGrassFieldShader );
}*/
//===============================================================================================================================
/*void GrassFieldMesh::RenderClip(Camera* pCamera, XMMATRIX view, XMFLOAT4 clipplane, RenderType rt, bool colorShader, bool texShader, ZShadeSandboxShader::Shader* shader)
{
	//
	// Render the mesh buffers
	//

	SetBuffersNoIndex(rt);

	//
	// Render the shader
	//

	Shade( pCamera, WorldXM(), view, clipplane, colorShader, texShader, shader );
}*/
//===============================================================================================================================
/*void GrassFieldMesh::Update(float dt)
{
	//mRotation.x += dt;
	//mRotation.y += dt;
	//mRotation.z += dt;
	//m_pGrassFieldShader->SetTime( dt );
}*/
//===============================================================================================================================
void GrassFieldMesh::Initialize()
{
	//
	// Load Vertices
	//

	srand( 0 );

	mAttributes->mVerticesPos.resize(m_FieldSize);

	for (int n = 0; n < m_FieldSize; n++)
	{
		//Find a random position based on the full field size
		int i = rand() % m_Width;
		int j = rand() % m_Width;

		float height = m_Heightmap[(m_Width * i) + j];

		mAttributes->mVerticesPos[n] = ZShadeSandboxMesh::VertexUtil::LoadVertexPos(float(i), height / mAttributes->mScale.y, float(j));
	}

	/*ZShadeSandboxMesh::VertexColor verts[numVerts] =
	{
		ZShadeSandboxMesh::VertexColor(10.0f, 1.0f, 10.0f, 1.0f, 0.0f, 0.0f, 1.0f),

		ZShadeSandboxMesh::VertexColor(20.0f, 1.0f, 20.0f, 0.0f, 1.0f, 0.0f, 1.0f),

		ZShadeSandboxMesh::VertexColor(30.0f, 1.0f, 30.0f, 0.0f, 0.0f, 1.0f, 1.0f),
	};

	mVertexColorVertices.resize( numVerts );
	
	for (int i = 0; i < numVerts; i++)
	{
		mVertexColorVertices[i] = verts[i];
	}*/

	mAttributes->mVertexCount = m_FieldSize;

	//Disable the index buffer
	m_DisableIndexBuffer = true;
}
//===============================================================================================================================