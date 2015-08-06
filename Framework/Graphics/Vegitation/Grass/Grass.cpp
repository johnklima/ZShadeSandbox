//#include "stdafx.h"
#include "Grass.h"
#include "Exceptions.h"
#include "TextureManager.h"
//===============================================================================================================================
//===============================================================================================================================
GrassField::GrassField(D3D* d3d)
:	m_pD3DSystem(d3d)
,	m_pGFM(0)
,	m_RenderPoints(false)
{
	//m_pAlphaTopSRV = TextureManager::GetInstance()->GetTexture( m_pD3DSystem, "GrassAlphaMapTop.png" );
	//m_pDiffuseTopSRV = TextureManager::GetInstance()->GetTexture( m_pD3DSystem, "GrassDiffuseMapTop.png" );
	//m_pAlphaBodySRV = TextureManager::GetInstance()->GetTexture( m_pD3DSystem, "GrassAlphaMapBody.png" );
	//m_pDiffuseBodySRV = TextureManager::GetInstance()->GetTexture( m_pD3DSystem, "GrassDiffuseMapBody.png" );

	m_pAlphaTopSRV = TextureManager::Instance()->GetTexture( "blade.png" );
	m_pDiffuseTopSRV = TextureManager::Instance()->GetTexture( "blade.png" );
	m_pAlphaBodySRV = TextureManager::Instance()->GetTexture( "blade.png" );
	m_pDiffuseBodySRV = TextureManager::Instance()->GetTexture( "blade.png" );
}
//===============================================================================================================================
GrassField::~GrassField()
{
}
//===============================================================================================================================
void GrassField::Initialize(int fieldSize, int width, vector<float> heightmap)
{
	//I can set up an elevation clipper in the shader
	//so that grass can only spawn at a certain elevation
	//Compare the height in the pixel shader and do a clip(-1) if the grass blade is not in the elevation

	//
	// Load the grass field mesh
	//

	//fieldSize can be no bigger than width*width
	if (fieldSize > SQR(width))
	{
		throw Exception("GrassField FieldSize cannot be bigger than the square of terrain width");
	}

	ZShadeSandboxMesh::MeshParameters mp;

	//mp.color = XMFLOAT4(0, 0, 0, 0);
	mp.pos = XMFLOAT3(0, 4, 0);
	mp.rot = XMFLOAT3(0, 0, 0);
	mp.scale = XMFLOAT3(1, 1, 1);
	mp.rotationAxisX = false;
	mp.rotationAxisY = false;
	mp.rotationAxisZ = false;
	mp.vertexType = ZShadeSandboxMesh::EVertexType::VT_Tex;
	mp.useCustomShader = true; // The grass shader uses a custom shader so ignore vertex type and texture

	m_pGFM = new GrassFieldMesh( m_pD3DSystem, fieldSize, width, heightmap, mp );

	
	//Starting out write a shader that highlights the root positions (POINT SHADER)

}
//===============================================================================================================================
void GrassField::Update(float dt)
{
	//Update simulation time for grass waving
	m_pGFM->Update(dt);
}
//===============================================================================================================================
void GrassField::Render(Camera* camera)
{
	//
	// Start the grass shading
	//

	m_pGFM->SetGrassAlphaTopSRV(m_pAlphaTopSRV);
	m_pGFM->SetGrassDiffuseTopSRV(m_pDiffuseTopSRV);
	m_pGFM->SetGrassAlphaBodySRV(m_pAlphaBodySRV);
	m_pGFM->SetGrassDiffuseBodySRV(m_pDiffuseBodySRV);

	ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.camera = camera;
	m_pGFM->Render(mrp);
}
//===============================================================================================================================