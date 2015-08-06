#include "TopdownTile.h"
#include "FogData.h"
#include "D3D.h"
#include "MaterialManager.h"
//=================================================================================================================
//=================================================================================================================
TopdownTile::TopdownTile()
:	PhysicsBody2D()
,	m_D3DSystem(0)
,	m_engineOptions(0)
{
	InitData();
}
//=================================================================================================================
TopdownTile::TopdownTile(D3D* d3d)
:   PhysicsBody2D(d3d)
,	m_D3DSystem(d3d)
{
	InitData();
	m_engineOptions = d3d->GetEngineOptions();
	mSize = m_engineOptions->TILE_SIZE;
}
//=================================================================================================================
TopdownTile::~TopdownTile()
{
}
//==================================================================================================================================
void TopdownTile::SetD3D(D3D* d3d)
{
	m_D3DSystem = d3d;
	m_EngineOptions = d3d->GetEngineOptions();
	PhysicsBody2D::SetD3D(d3d);
}
//==================================================================================================================================
void TopdownTile::InitData()
{
	mSize = 0;
	mHard = false;
	m_displayHardbox = false;
	m_animationProfileName = "NONE";
	m_animationSpeed = 0;
	m_fogBit = fow_n_all;
	m_fogBitShadow = fow_s_all;
	bFowBitShadow = false;
	m_CurrentSequence = 0;
	m_bFOW = false;
	m_iFOWRadius = 5;
	m_bFOWFlashlight = false;
	m_bFOWUncovered = false;
	bFowUncovered = false;
	bFowAllUncovered = false;
	mBaseTextureName = "NONE";
	bInCircle = false;
	bHasFowShadow = false;
	bInPlayerFowCircle = false;
	m_fowName = "";
	
	mFOWMesh = 0;
	mBaseMesh = 0;
	
	fow_shadow_texture = 0;
	fow_shadow_CNW_texture = 0;
	fow_shadow_CNE_texture = 0;
	fow_shadow_CSW_texture = 0;
	fow_shadow_CSE_texture = 0;
	fow_shadow_JNW_texture = 0;
	fow_shadow_JNE_texture = 0;
	fow_shadow_JSW_texture = 0;
	fow_shadow_JSE_texture = 0;
	fow_shadow_NNN_texture = 0;
	fow_shadow_SSS_texture = 0;
	fow_shadow_EEE_texture = 0;
	fow_shadow_WWW_texture = 0;
	fow_all_texture = 0;
	fow_CNW_texture = 0;
	fow_CNE_texture = 0;
	fow_CSW_texture = 0;
	fow_CSE_texture = 0;
	fow_JNW_texture = 0;
	fow_JNE_texture = 0;
	fow_JSW_texture = 0;
	fow_JSE_texture = 0;
	fow_NNN_texture = 0;
	fow_SSS_texture = 0;
	fow_EEE_texture = 0;
	fow_WWW_texture = 0;
}
//=================================================================================================================
void TopdownTile::CalculateFowName()
{
	switch (m_fogBit)
	{
		case fow_n_non: m_fowName = "FOW NON"; break;
		case fow_n_all: m_fowName = "FOW ALL"; break;
		case fow_n_NNN: m_fowName = "FOW NNN"; break;
		case fow_n_SSS: m_fowName = "FOW SSS"; break;
		case fow_n_EEE: m_fowName = "FOW EEE"; break;
		case fow_n_WWW: m_fowName = "FOW WWW"; break;
		case fow_n_CNW: m_fowName = "FOW CNW"; break;
		case fow_n_CNE: m_fowName = "FOW CNE"; break;
		case fow_n_CSW: m_fowName = "FOW CSW"; break;
		case fow_n_CSE: m_fowName = "FOW CSE"; break;
		case fow_n_JNW: m_fowName = "FOW JNW"; break;
		case fow_n_JNE: m_fowName = "FOW JNE"; break;
		case fow_n_JSW: m_fowName = "FOW JSW"; break;
		case fow_n_JSE: m_fowName = "FOW JSE"; break;
	}
}
//=================================================================================================================
void TopdownTile::InitializeBody(int size, XMFLOAT3 pos)
{
	PhysicsBody2D::Initialize(pos, STATIC, ESurfaceType::Type::eNormal, size, size);
	mSize = size;
	m_d3dvers = m_D3DSystem->GetEngineOptions()->m_d3dVersion;
}
//=================================================================================================================
string TopdownTile::GetTileMeshTextureName()
{
	//if (mMesh != NULL)
	//	return mMesh->TextureName();
	return "";
}
//=================================================================================================================
string TopdownTile::GetTileBaseTextureName()
{
	if (mBaseMesh != NULL)
		return mBaseMesh->TextureName();
	return mBaseTextureName;
}
//=================================================================================================================
bool TopdownTile::InitializeMesh(string base_tile_path, string textureFilename, int bitmapWidth, int bitmapHeight)
{
	MaterialManager::Instance()->AddTransparentDiffuse(m_D3DSystem, base_tile_path,
		textureFilename, textureFilename, textureFilename, m_EngineOptions->m_TransparencyKey);
	
	ZShadeSandboxMesh::MeshParameters mpFow;
	mpFow.pos = XMFLOAT3(0, 0, 0);
	mpFow.rot = XMFLOAT3(0, 0, 0);
	mpFow.scale = XMFLOAT3(1, 1, 1);
	mpFow.rotationAxisX = false;
	mpFow.rotationAxisY = false;
	mpFow.rotationAxisZ = false;
	mpFow.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
	mpFow.useCustomShader = false;
	mpFow.material = MaterialManager::Instance()->GetMaterial("FOW ALL");
	mpFow.in2D = true;
	mFOWMesh = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mpFow, bitmapWidth, bitmapHeight, true);
	if (!mFOWMesh) return false;
	
	ZShadeSandboxMesh::MeshParameters mpFowShadow;
	mpFowShadow.pos = XMFLOAT3(0, 0, 0);
	mpFowShadow.rot = XMFLOAT3(0, 0, 0);
	mpFowShadow.scale = XMFLOAT3(1, 1, 1);
	mpFowShadow.rotationAxisX = false;
	mpFowShadow.rotationAxisY = false;
	mpFowShadow.rotationAxisZ = false;
	mpFowShadow.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
	mpFowShadow.useCustomShader = false;
	mpFowShadow.material = MaterialManager::Instance()->GetMaterial("FOW SHADOW");
	mpFowShadow.in2D = true;
	mFOWShadowMesh = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mpFowShadow, bitmapWidth, bitmapHeight, true);
	if (!mFOWShadowMesh) return false;

	mBaseTextureName = textureFilename;
	
	ZShadeSandboxMesh::MeshParameters mp;
	mp.pos = XMFLOAT3(0, 0, 0);
	mp.rot = XMFLOAT3(0, 0, 0);
	mp.scale = XMFLOAT3(1, 1, 1);
	mp.rotationAxisX = false;
	mp.rotationAxisY = false;
	mp.rotationAxisZ = false;
	mp.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
	mp.useCustomShader = false;
	mp.material = MaterialManager::Instance()->GetMaterial(textureFilename);
	mp.in2D = true;
	mBaseMesh = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp, bitmapWidth, bitmapHeight, true);
	if (!mBaseMesh) return false;
	
	/*{
		//if (m_bFOW)
		//{
		//	mMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW ALL"));
		//}

		ZShadeSandboxMesh::MeshParameters mp1;
		mp1.pos = XMFLOAT3(0, 0, 0);
		mp1.rot = XMFLOAT3(0, 0, 0);
		mp1.scale = XMFLOAT3(1, 1, 1);
		mp1.rotationAxisX = false;
		mp1.rotationAxisY = false;
		mp1.rotationAxisZ = false;
		mp1.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
		mp1.useCustomShader = false;
		mp1.in2D = true;
		mp1.material = MaterialManager::Instance()->GetMaterial("FOW SHADOW");
		fow_shadow_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp1, m_D3DSystem->GetEngineOptions()->TILE_SIZE, m_D3DSystem->GetEngineOptions()->TILE_SIZE, true);

		//
		//Corner Textures
		//

		ZShadeSandboxMesh::MeshParameters mp2;
		mp2.pos = XMFLOAT3(0, 0, 0);
		mp2.rot = XMFLOAT3(0, 0, 0);
		mp2.scale = XMFLOAT3(1, 1, 1);
		mp2.rotationAxisX = false;
		mp2.rotationAxisY = false;
		mp2.rotationAxisZ = false;
		mp2.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
		mp2.useCustomShader = false;
		mp2.in2D = true;
		mp2.material = MaterialManager::Instance()->GetMaterial("FOW SHADOW CNW");
		fow_shadow_CNW_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp2, m_D3DSystem->GetEngineOptions()->TILE_SIZE, m_D3DSystem->GetEngineOptions()->TILE_SIZE, true);

		ZShadeSandboxMesh::MeshParameters mp3;
		mp3.pos = XMFLOAT3(0, 0, 0);
		mp3.rot = XMFLOAT3(0, 0, 0);
		mp3.scale = XMFLOAT3(1, 1, 1);
		mp3.rotationAxisX = false;
		mp3.rotationAxisY = false;
		mp3.rotationAxisZ = false;
		mp3.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
		mp3.useCustomShader = false;
		mp3.in2D = true;
		mp3.material = MaterialManager::Instance()->GetMaterial("FOW SHADOW CNE");
		fow_shadow_CNE_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp3, m_D3DSystem->GetEngineOptions()->TILE_SIZE, m_D3DSystem->GetEngineOptions()->TILE_SIZE, true);

		ZShadeSandboxMesh::MeshParameters mp4;
		mp4.pos = XMFLOAT3(0, 0, 0);
		mp4.rot = XMFLOAT3(0, 0, 0);
		mp4.scale = XMFLOAT3(1, 1, 1);
		mp4.rotationAxisX = false;
		mp4.rotationAxisY = false;
		mp4.rotationAxisZ = false;
		mp4.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
		mp4.useCustomShader = false;
		mp4.in2D = true;
		mp4.material = MaterialManager::Instance()->GetMaterial("FOW SHADOW CSW");
		fow_shadow_CSW_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp4, m_D3DSystem->GetEngineOptions()->TILE_SIZE, m_D3DSystem->GetEngineOptions()->TILE_SIZE, true);

		ZShadeSandboxMesh::MeshParameters mp5;
		mp5.pos = XMFLOAT3(0, 0, 0);
		mp5.rot = XMFLOAT3(0, 0, 0);
		mp5.scale = XMFLOAT3(1, 1, 1);
		mp5.rotationAxisX = false;
		mp5.rotationAxisY = false;
		mp5.rotationAxisZ = false;
		mp5.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
		mp5.useCustomShader = false;
		mp5.in2D = true;
		mp5.material = MaterialManager::Instance()->GetMaterial("FOW SHADOW CSE");
		fow_shadow_CSE_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp5, m_D3DSystem->GetEngineOptions()->TILE_SIZE, m_D3DSystem->GetEngineOptions()->TILE_SIZE, true);

		//
		//Joint Textures
		//

		ZShadeSandboxMesh::MeshParameters mp6;
		mp6.pos = XMFLOAT3(0, 0, 0);
		mp6.rot = XMFLOAT3(0, 0, 0);
		mp6.scale = XMFLOAT3(1, 1, 1);
		mp6.rotationAxisX = false;
		mp6.rotationAxisY = false;
		mp6.rotationAxisZ = false;
		mp6.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
		mp6.useCustomShader = false;
		mp6.in2D = true;
		mp6.material = MaterialManager::Instance()->GetMaterial("FOW SHADOW JNW");
		fow_shadow_JNW_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp6, m_D3DSystem->GetEngineOptions()->TILE_SIZE, m_D3DSystem->GetEngineOptions()->TILE_SIZE, true);

		ZShadeSandboxMesh::MeshParameters mp7;
		mp7.pos = XMFLOAT3(0, 0, 0);
		mp7.rot = XMFLOAT3(0, 0, 0);
		mp7.scale = XMFLOAT3(1, 1, 1);
		mp7.rotationAxisX = false;
		mp7.rotationAxisY = false;
		mp7.rotationAxisZ = false;
		mp7.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
		mp7.useCustomShader = false;
		mp7.in2D = true;
		mp7.material = MaterialManager::Instance()->GetMaterial("FOW SHADOW JNE");
		fow_shadow_JNE_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp7, m_D3DSystem->GetEngineOptions()->TILE_SIZE, m_D3DSystem->GetEngineOptions()->TILE_SIZE, true);

		ZShadeSandboxMesh::MeshParameters mp8;
		mp8.pos = XMFLOAT3(0, 0, 0);
		mp8.rot = XMFLOAT3(0, 0, 0);
		mp8.scale = XMFLOAT3(1, 1, 1);
		mp8.rotationAxisX = false;
		mp8.rotationAxisY = false;
		mp8.rotationAxisZ = false;
		mp8.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
		mp8.useCustomShader = false;
		mp8.in2D = true;
		mp8.material = MaterialManager::Instance()->GetMaterial("FOW SHADOW JSW");
		fow_shadow_JSW_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp8, m_D3DSystem->GetEngineOptions()->TILE_SIZE, m_D3DSystem->GetEngineOptions()->TILE_SIZE, true);

		ZShadeSandboxMesh::MeshParameters mp9;
		mp9.pos = XMFLOAT3(0, 0, 0);
		mp9.rot = XMFLOAT3(0, 0, 0);
		mp9.scale = XMFLOAT3(1, 1, 1);
		mp9.rotationAxisX = false;
		mp9.rotationAxisY = false;
		mp9.rotationAxisZ = false;
		mp9.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
		mp9.useCustomShader = false;
		mp9.in2D = true;
		mp9.material = MaterialManager::Instance()->GetMaterial("FOW SHADOW JSE");
		fow_shadow_JSE_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp9, m_D3DSystem->GetEngineOptions()->TILE_SIZE, m_D3DSystem->GetEngineOptions()->TILE_SIZE, true);

		//
		//Straight Textures
		//

		ZShadeSandboxMesh::MeshParameters mp10;
		mp10.pos = XMFLOAT3(0, 0, 0);
		mp10.rot = XMFLOAT3(0, 0, 0);
		mp10.scale = XMFLOAT3(1, 1, 1);
		mp10.rotationAxisX = false;
		mp10.rotationAxisY = false;
		mp10.rotationAxisZ = false;
		mp10.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
		mp10.useCustomShader = false;
		mp10.in2D = true;
		mp10.material = MaterialManager::Instance()->GetMaterial("FOW SHADOW NNN");
		fow_shadow_NNN_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp10, m_D3DSystem->GetEngineOptions()->TILE_SIZE, m_D3DSystem->GetEngineOptions()->TILE_SIZE, true);

		ZShadeSandboxMesh::MeshParameters mp11;
		mp11.pos = XMFLOAT3(0, 0, 0);
		mp11.rot = XMFLOAT3(0, 0, 0);
		mp11.scale = XMFLOAT3(1, 1, 1);
		mp11.rotationAxisX = false;
		mp11.rotationAxisY = false;
		mp11.rotationAxisZ = false;
		mp11.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
		mp11.useCustomShader = false;
		mp11.in2D = true;
		mp11.material = MaterialManager::Instance()->GetMaterial("FOW SHADOW SSS");
		fow_shadow_SSS_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp11, m_D3DSystem->GetEngineOptions()->TILE_SIZE, m_D3DSystem->GetEngineOptions()->TILE_SIZE, true);

		ZShadeSandboxMesh::MeshParameters mp12;
		mp12.pos = XMFLOAT3(0, 0, 0);
		mp12.rot = XMFLOAT3(0, 0, 0);
		mp12.scale = XMFLOAT3(1, 1, 1);
		mp12.rotationAxisX = false;
		mp12.rotationAxisY = false;
		mp12.rotationAxisZ = false;
		mp12.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
		mp12.useCustomShader = false;
		mp12.in2D = true;
		mp12.material = MaterialManager::Instance()->GetMaterial("FOW SHADOW WWW");
		fow_shadow_WWW_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp12, m_D3DSystem->GetEngineOptions()->TILE_SIZE, m_D3DSystem->GetEngineOptions()->TILE_SIZE, true);

		ZShadeSandboxMesh::MeshParameters mp13;
		mp13.pos = XMFLOAT3(0, 0, 0);
		mp13.rot = XMFLOAT3(0, 0, 0);
		mp13.scale = XMFLOAT3(1, 1, 1);
		mp13.rotationAxisX = false;
		mp13.rotationAxisY = false;
		mp13.rotationAxisZ = false;
		mp13.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
		mp13.useCustomShader = false;
		mp13.in2D = true;
		mp13.material = MaterialManager::Instance()->GetMaterial("FOW SHADOW EEE");
		fow_shadow_EEE_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp13, m_D3DSystem->GetEngineOptions()->TILE_SIZE, m_D3DSystem->GetEngineOptions()->TILE_SIZE, true);
	}

	{
	ZShadeSandboxMesh::MeshParameters mp1;
	mp1.pos = XMFLOAT3(0, 0, 0);
	mp1.rot = XMFLOAT3(0, 0, 0);
	mp1.scale = XMFLOAT3(1, 1, 1);
	mp1.rotationAxisX = false;
	mp1.rotationAxisY = false;
	mp1.rotationAxisZ = false;
	mp1.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
	mp1.useCustomShader = false;
	mp1.in2D = true;
	mp1.material = MaterialManager::Instance()->GetMaterial("FOW ALL");
	fow_all_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp1, m_D3DSystem->GetEngineOptions()->TILE_SIZE, m_D3DSystem->GetEngineOptions()->TILE_SIZE, true);

	//
	//Corner Textures
	//

	ZShadeSandboxMesh::MeshParameters mp2;
	mp2.pos = XMFLOAT3(0, 0, 0);
	mp2.rot = XMFLOAT3(0, 0, 0);
	mp2.scale = XMFLOAT3(1, 1, 1);
	mp2.rotationAxisX = false;
	mp2.rotationAxisY = false;
	mp2.rotationAxisZ = false;
	mp2.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
	mp2.useCustomShader = false;
	mp2.in2D = true;
	mp2.material = MaterialManager::Instance()->GetMaterial("FOW CNW");
	fow_CNW_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp2, m_D3DSystem->GetEngineOptions()->TILE_SIZE, m_D3DSystem->GetEngineOptions()->TILE_SIZE, true);

	ZShadeSandboxMesh::MeshParameters mp3;
	mp3.pos = XMFLOAT3(0, 0, 0);
	mp3.rot = XMFLOAT3(0, 0, 0);
	mp3.scale = XMFLOAT3(1, 1, 1);
	mp3.rotationAxisX = false;
	mp3.rotationAxisY = false;
	mp3.rotationAxisZ = false;
	mp3.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
	mp3.useCustomShader = false;
	mp3.in2D = true;
	mp3.material = MaterialManager::Instance()->GetMaterial("FOW CNE");
	fow_CNE_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp3, m_D3DSystem->GetEngineOptions()->TILE_SIZE, m_D3DSystem->GetEngineOptions()->TILE_SIZE, true);

	ZShadeSandboxMesh::MeshParameters mp4;
	mp4.pos = XMFLOAT3(0, 0, 0);
	mp4.rot = XMFLOAT3(0, 0, 0);
	mp4.scale = XMFLOAT3(1, 1, 1);
	mp4.rotationAxisX = false;
	mp4.rotationAxisY = false;
	mp4.rotationAxisZ = false;
	mp4.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
	mp4.useCustomShader = false;
	mp4.in2D = true;
	mp4.material = MaterialManager::Instance()->GetMaterial("FOW CSW");
	fow_CSW_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp4, m_D3DSystem->GetEngineOptions()->TILE_SIZE, m_D3DSystem->GetEngineOptions()->TILE_SIZE, true);

	ZShadeSandboxMesh::MeshParameters mp5;
	mp5.pos = XMFLOAT3(0, 0, 0);
	mp5.rot = XMFLOAT3(0, 0, 0);
	mp5.scale = XMFLOAT3(1, 1, 1);
	mp5.rotationAxisX = false;
	mp5.rotationAxisY = false;
	mp5.rotationAxisZ = false;
	mp5.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
	mp5.useCustomShader = false;
	mp5.in2D = true;
	mp5.material = MaterialManager::Instance()->GetMaterial("FOW CSE");
	fow_CSE_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp5, m_D3DSystem->GetEngineOptions()->TILE_SIZE, m_D3DSystem->GetEngineOptions()->TILE_SIZE, true);

	//
	//Joint Textures
	//

	ZShadeSandboxMesh::MeshParameters mp6;
	mp6.pos = XMFLOAT3(0, 0, 0);
	mp6.rot = XMFLOAT3(0, 0, 0);
	mp6.scale = XMFLOAT3(1, 1, 1);
	mp6.rotationAxisX = false;
	mp6.rotationAxisY = false;
	mp6.rotationAxisZ = false;
	mp6.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
	mp6.useCustomShader = false;
	mp6.in2D = true;
	mp6.material = MaterialManager::Instance()->GetMaterial("FOW JNW");
	fow_JNW_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp6, m_D3DSystem->GetEngineOptions()->TILE_SIZE, m_D3DSystem->GetEngineOptions()->TILE_SIZE, true);

	ZShadeSandboxMesh::MeshParameters mp7;
	mp7.pos = XMFLOAT3(0, 0, 0);
	mp7.rot = XMFLOAT3(0, 0, 0);
	mp7.scale = XMFLOAT3(1, 1, 1);
	mp7.rotationAxisX = false;
	mp7.rotationAxisY = false;
	mp7.rotationAxisZ = false;
	mp7.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
	mp7.useCustomShader = false;
	mp7.in2D = true;
	mp7.material = MaterialManager::Instance()->GetMaterial("FOW JNE");
	fow_JNE_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp7, m_D3DSystem->GetEngineOptions()->TILE_SIZE, m_D3DSystem->GetEngineOptions()->TILE_SIZE, true);

	ZShadeSandboxMesh::MeshParameters mp8;
	mp8.pos = XMFLOAT3(0, 0, 0);
	mp8.rot = XMFLOAT3(0, 0, 0);
	mp8.scale = XMFLOAT3(1, 1, 1);
	mp8.rotationAxisX = false;
	mp8.rotationAxisY = false;
	mp8.rotationAxisZ = false;
	mp8.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
	mp8.useCustomShader = false;
	mp8.in2D = true;
	mp8.material = MaterialManager::Instance()->GetMaterial("FOW JSW");
	fow_JSW_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp8, m_D3DSystem->GetEngineOptions()->TILE_SIZE, m_D3DSystem->GetEngineOptions()->TILE_SIZE, true);

	ZShadeSandboxMesh::MeshParameters mp9;
	mp9.pos = XMFLOAT3(0, 0, 0);
	mp9.rot = XMFLOAT3(0, 0, 0);
	mp9.scale = XMFLOAT3(1, 1, 1);
	mp9.rotationAxisX = false;
	mp9.rotationAxisY = false;
	mp9.rotationAxisZ = false;
	mp9.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
	mp9.useCustomShader = false;
	mp9.in2D = true;
	mp9.material = MaterialManager::Instance()->GetMaterial("FOW JSE");
	fow_JSE_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp9, m_D3DSystem->GetEngineOptions()->TILE_SIZE, m_D3DSystem->GetEngineOptions()->TILE_SIZE, true);

	//
	//Straight Textures
	//

	ZShadeSandboxMesh::MeshParameters mp10;
	mp10.pos = XMFLOAT3(0, 0, 0);
	mp10.rot = XMFLOAT3(0, 0, 0);
	mp10.scale = XMFLOAT3(1, 1, 1);
	mp10.rotationAxisX = false;
	mp10.rotationAxisY = false;
	mp10.rotationAxisZ = false;
	mp10.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
	mp10.useCustomShader = false;
	mp10.in2D = true;
	mp10.material = MaterialManager::Instance()->GetMaterial("FOW NNN");
	fow_NNN_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp10, m_D3DSystem->GetEngineOptions()->TILE_SIZE, m_D3DSystem->GetEngineOptions()->TILE_SIZE, true);

	ZShadeSandboxMesh::MeshParameters mp11;
	mp11.pos = XMFLOAT3(0, 0, 0);
	mp11.rot = XMFLOAT3(0, 0, 0);
	mp11.scale = XMFLOAT3(1, 1, 1);
	mp11.rotationAxisX = false;
	mp11.rotationAxisY = false;
	mp11.rotationAxisZ = false;
	mp11.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
	mp11.useCustomShader = false;
	mp11.in2D = true;
	mp11.material = MaterialManager::Instance()->GetMaterial("FOW SSS");
	fow_SSS_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp11, m_D3DSystem->GetEngineOptions()->TILE_SIZE, m_D3DSystem->GetEngineOptions()->TILE_SIZE, true);

	ZShadeSandboxMesh::MeshParameters mp12;
	mp12.pos = XMFLOAT3(0, 0, 0);
	mp12.rot = XMFLOAT3(0, 0, 0);
	mp12.scale = XMFLOAT3(1, 1, 1);
	mp12.rotationAxisX = false;
	mp12.rotationAxisY = false;
	mp12.rotationAxisZ = false;
	mp12.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
	mp12.useCustomShader = false;
	mp12.in2D = true;
	mp12.material = MaterialManager::Instance()->GetMaterial("FOW WWW");
	fow_WWW_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp12, m_D3DSystem->GetEngineOptions()->TILE_SIZE, m_D3DSystem->GetEngineOptions()->TILE_SIZE, true);

	ZShadeSandboxMesh::MeshParameters mp13;
	mp13.pos = XMFLOAT3(0, 0, 0);
	mp13.rot = XMFLOAT3(0, 0, 0);
	mp13.scale = XMFLOAT3(1, 1, 1);
	mp13.rotationAxisX = false;
	mp13.rotationAxisY = false;
	mp13.rotationAxisZ = false;
	mp13.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
	mp13.useCustomShader = false;
	mp13.in2D = true;
	mp13.material = MaterialManager::Instance()->GetMaterial("FOW EEE");
	fow_EEE_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp13, m_D3DSystem->GetEngineOptions()->TILE_SIZE, m_D3DSystem->GetEngineOptions()->TILE_SIZE, true);
	}*/

	return true;
}
//=================================================================================================================
void TopdownTile::Shutdown()
{
	if (mAnimationSequences.size() > 0) mAnimationSequences.clear();
}
//=================================================================================================================
void TopdownTile::UpdateAnimation()
{
	if (m_CurrentSequence == NULL) return;

	m_CurrentSequence->Update(m_animationSpeed);
}
//=================================================================================================================
void TopdownTile::AddAnimationSequence(Animation2D* sequence)
{
	if (sequence == NULL) return;

	sequence->LoadFrameTextures();
	mAnimationSequences.push_back(sequence);
}
//=================================================================================================================
void TopdownTile::SetSequence(int i)
{
	m_CurrentSequence = mAnimationSequences[i];
}
//=================================================================================================================
TopdownTile* TopdownTile::Clone(GameDirectory2D* gd)
{
	TopdownTile* t = new TopdownTile(m_D3DSystem);

	t->InitializeBody(mSize, XMFLOAT3(m_TopLeftPosition.x, m_TopLeftPosition.y, 0));
	
	t->AnimationProfileName() = m_animationProfileName;
	t->AnimationSpeed() = m_animationSpeed;
	t->FowUncovered() = bFowUncovered;
	t->FogBit() = m_fogBit;
	t->DisplayHardbox() = m_displayHardbox;
	t->FOW() = m_bFOW;
	t->FOWRadius() = m_iFOWRadius;
	t->FOWFlashlight() = m_bFOWFlashlight;
	t->BaseTextureName() = mBaseTextureName;
	
	string base_path = "";
	base_path = gd->m_tiles_path;
	
	if (mBaseMesh != 0)
	{
		if (mBaseMesh->GetMaterial() != 0)
		{
			t->InitializeMesh(base_path, mBaseMesh->TextureName(), m_engineOptions->TILE_SIZE, m_engineOptions->TILE_SIZE);
		}
	}
	
	//If there are any animations for the tile set it
	if (m_animationProfileName != "NONE")
	{
		//Load a animation profile
		AnimationProfile2D* profile = new AnimationProfile2D(m_D3DSystem);

		bool something_wrong = false;
		
		if (profile->LoadAnimationProfileNames(m_animationProfileName, "TILE", gd->GAME_FOLDER))
		{
			if (profile->ReadProfile(m_animationProfileName, "TILE", gd->m_tiles_path, gd->GAME_FOLDER))
			{
				vector<Animation2D*> anims = profile->GetAnimations();

				t->ClearAnimationList();

				for (int i = 0; i < anims.size(); i++)
				{
					t->AddAnimationSequence(anims[i]);
				}

				t->SetSequence(0);
			}
			else something_wrong = true;
		}
		else something_wrong = true;

		if (something_wrong)
		{
			MessageBox(NULL, "The Animation2D does not exist", "Add Animations", MB_OK);
		}
	}

	return t;
}
//=================================================================================================================
void TopdownTile::UpdateToFOWAllMeshTexture()
{
	// Grab the material whether it existed or not
	//mMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW ALL"));
}
//=================================================================================================================
void TopdownTile::UpdateMeshTexture(string base_tile_path, string textureFilename)
{
	// Creates new material if one does not exist, if it does exist then do nothing
	MaterialManager::Instance()->AddTransparentDiffuse(m_D3DSystem, base_tile_path,
		textureFilename, textureFilename, textureFilename, m_EngineOptions->m_TransparencyKey);

	// Grab the material whether it existed or not
	mBaseMesh->SetMaterial(MaterialManager::Instance()->GetMaterial(textureFilename));
}
//=================================================================================================================
void TopdownTile::UpdateTexture()
{
	switch (m_fogBit)
	{
		case fow_n_all:
		{
			if (bFowUncovered)
			{
				mFOWMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW SHADOW"));
			}
			else
			{
				mFOWMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW ALL"));
			}
		}
		break;
		#pragma region "Straights"
		case fow_n_NNN:
		{
			if (bFowUncovered)
			{
				mFOWMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW SHADOW NNN"));
			}
			else
			{
				if (!bHasFowShadow || m_bFOWUncovered)
					mFOWMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW NNN"));
				else
					mFOWMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW ALL SHADOW NNN"));
			}
		}
		break;
		case fow_n_SSS:
		{
			if (bFowUncovered)
			{
				mFOWMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW SHADOW SSS"));
			}
			else
			{
				if (!bHasFowShadow || m_bFOWUncovered)
					mFOWMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW SSS"));
				else
					mFOWMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW ALL SHADOW SSS"));
			}
		}
		break;
		case fow_n_EEE:
		{
			if (bFowUncovered)
			{
				mFOWMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW SHADOW EEE"));
			}
			else
			{
				if (!bHasFowShadow || m_bFOWUncovered)
					mFOWMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW EEE"));
				else
					mFOWMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW ALL SHADOW EEE"));
			}
		}
		break;
		case fow_n_WWW:
		{
			if (bFowUncovered)
			{
				mFOWMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW SHADOW WWW"));
			}
			else
			{
				if (!bHasFowShadow || m_bFOWUncovered)
					mFOWMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW WWW"));
				else
					mFOWMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW ALL SHADOW WWW"));
			}
		}
		break;
		#pragma endregion
		#pragma region "Corners"
		case fow_n_CNW:
		{
			if (bFowUncovered)
			{
				mFOWMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW SHADOW CNW"));
			}
			else
			{
				if (!bHasFowShadow || m_bFOWUncovered)
					mFOWMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW CNW"));
				else
					mFOWMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW ALL SHADOW CNW"));
			}
		}
		break;
		case fow_n_CNE:
		{
			if (bFowUncovered)
			{
				mFOWMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW SHADOW CNE"));
			}
			else
			{
				if (!bHasFowShadow || m_bFOWUncovered)
					mFOWMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW CNE"));
				else
					mFOWMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW ALL SHADOW CNE"));
			}
		}
		break;
		case fow_n_CSW:
		{
			if (bFowUncovered)
			{
				mFOWMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW SHADOW CSW"));
			}
			else
			{
				if (!bHasFowShadow || m_bFOWUncovered)
					mFOWMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW CSW"));
				else
					mFOWMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW ALL SHADOW CSW"));
			}
		}
		break;
		case fow_n_CSE:
		{
			if (bFowUncovered)
			{
				mFOWMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW SHADOW CSE"));
			}
			else
			{
				if (!bHasFowShadow || m_bFOWUncovered)
					mFOWMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW CSE"));
				else
					mFOWMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW ALL SHADOW CSE"));
			}
		}
		break;
		#pragma endregion
		#pragma region "Joints"
		case fow_n_JNW:
		{
			if (bFowUncovered)
			{
				mFOWMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW SHADOW JNW"));
			}
			else
			{
				if (!bHasFowShadow || m_bFOWUncovered)
					mFOWMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW JNW"));
				else
					mFOWMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW ALL SHADOW JNW"));
			}
		}
		break;
		case fow_n_JNE:
		{
			if (bFowUncovered)
			{
				mFOWMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW SHADOW JNE"));
			}
			else
			{
				if (!bHasFowShadow || m_bFOWUncovered)
					mFOWMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW JNE"));
				else
					mFOWMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW ALL SHADOW JNE"));
			}
		}
		break;
		case fow_n_JSW:
		{
			if (bFowUncovered)
			{
				mFOWMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW SHADOW JSW"));
			}
			else
			{
				if (!bHasFowShadow || m_bFOWUncovered)
					mFOWMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW JSW"));
				else
					mFOWMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW ALL SHADOW JSW"));
			}
		}
		break;
		case fow_n_JSE:
		{
			if (bFowUncovered)
			{
				mFOWMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW SHADOW JSE"));
			}
			else
			{
				if (!bHasFowShadow || m_bFOWUncovered)
					mFOWMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW JSE"));
				else
					mFOWMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW ALL SHADOW JSE"));
			}
		}
		break;
		default: break;
		#pragma endregion
	}
	
	/*switch (m_fogBitShadow)
	{
		case fow_s_all:
		{
			mFOWMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW SHADOW"));
		}
		break;
		#pragma region "Shadow straights"
		case fow_s_NNN:
		{
			mFOWMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW SHADOW NNN"));
		}
		break;
		case fow_s_SSS:
		{
			mFOWMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW SHADOW SSS"));
		}
		break;
		case fow_s_EEE:
		{
			mFOWMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW SHADOW EEE"));
		}
		break;
		case fow_s_WWW:
		{
			mFOWMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW SHADOW WWW"));
		}
		break;
		#pragma endregion
		#pragma region "Shadow corners"
		case fow_s_CNW:
		{
			mFOWMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW SHADOW CNW"));
		}
		break;
		case fow_s_CNE:
		{
			mFOWMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW SHADOW CNE"));
		}
		break;
		case fow_s_CSW:
		{
			mFOWMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW SHADOW CSW"));
		}
		break;
		case fow_s_CSE:
		{
			mFOWMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW SHADOW CSE"));
		}
		break;
		#pragma endregion
		#pragma region "Shadow joints"
		case fow_s_JNW:
		{
			mFOWMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW SHADOW JNW"));
		}
		break;
		case fow_s_JNE:
		{
			mFOWMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW SHADOW JNE"));
		}
		break;
		case fow_s_JSW:
		{
			mFOWMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW SHADOW JSW"));
		}
		break;
		case fow_s_JSE:
		{
			mFOWMesh->SetMaterial(MaterialManager::Instance()->GetMaterial("FOW SHADOW JSE"));
		}
		break;
		default: break;
		#pragma endregion
	}*/
}
//=================================================================================================================
bool TopdownTile::RenderFow(Camera* camera, float blendAmount, XMFLOAT2 offset)
{
	/*ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.camera = camera;
	mrp.blendAmount = blendAmount;
	mrp.specifyWorld = true;
	mrp.world = XMMatrixIdentity();
	
	switch (m_fogBit)//m_fogBitShadow)
	{
		case fow_s_all:
		{
			if (fow_shadow_texture == NULL) return false;
			fow_shadow_texture->UpdateBuffers(m_TopLeftPosition.x + offset.x, m_TopLeftPosition.y + offset.y);
			fow_shadow_texture->Render(mrp);
		}
		break;
		#pragma region "Shadow Straights"
		case fow_s_NNN:
		{
			if (fow_shadow_NNN_texture == NULL) return false;
			fow_shadow_NNN_texture->UpdateBuffers(m_TopLeftPosition.x + offset.x, m_TopLeftPosition.y + offset.y);
			fow_shadow_NNN_texture->Render(mrp);
		}
		break;
		case fow_s_SSS:
		{
			if (fow_shadow_SSS_texture == NULL) return false;
			fow_shadow_SSS_texture->UpdateBuffers(m_TopLeftPosition.x + offset.x, m_TopLeftPosition.y + offset.y);
			fow_shadow_SSS_texture->Render(mrp);
		}
		break;
		case fow_s_EEE:
		{
			if (fow_shadow_EEE_texture == NULL) return false;
			fow_shadow_EEE_texture->UpdateBuffers(m_TopLeftPosition.x + offset.x, m_TopLeftPosition.y + offset.y);
			fow_shadow_EEE_texture->Render(mrp);
		}
		break;
		case fow_s_WWW:
		{
			if (fow_shadow_WWW_texture == NULL) return false;
			fow_shadow_WWW_texture->UpdateBuffers(m_TopLeftPosition.x + offset.x, m_TopLeftPosition.y + offset.y);
			fow_shadow_WWW_texture->Render(mrp);
		}
		break;
		#pragma endregion
		#pragma region "Shadow Corners"
		case fow_s_CNW:
		{
			if (fow_shadow_CNW_texture == NULL) return false;
			fow_shadow_CNW_texture->UpdateBuffers(m_TopLeftPosition.x + offset.x, m_TopLeftPosition.y + offset.y);
			fow_shadow_CNW_texture->Render(mrp);
		}
		break;
		case fow_s_CNE:
		{
			if (fow_shadow_CNE_texture == NULL) return false;
			fow_shadow_CNE_texture->UpdateBuffers(m_TopLeftPosition.x + offset.x, m_TopLeftPosition.y + offset.y);
			fow_shadow_CNE_texture->Render(mrp);
		}
		break;
		case fow_s_CSW:
		{
			if (fow_shadow_CSW_texture == NULL) return false;
			fow_shadow_CSW_texture->UpdateBuffers(m_TopLeftPosition.x + offset.x, m_TopLeftPosition.y + offset.y);
			fow_shadow_CSW_texture->Render(mrp);
		}
		break;
		case fow_s_CSE:
		{
			if (fow_shadow_CSE_texture == NULL) return false;
			fow_shadow_CSE_texture->UpdateBuffers(m_TopLeftPosition.x + offset.x, m_TopLeftPosition.y + offset.y);
			fow_shadow_CSE_texture->Render(mrp);
		}
		break;
		#pragma endregion
		#pragma region "Shadow Joints"
		case fow_s_JNW:
		{
			if (fow_shadow_JNW_texture == NULL) return false;
			fow_shadow_JNW_texture->UpdateBuffers(m_TopLeftPosition.x + offset.x, m_TopLeftPosition.y + offset.y);
			fow_shadow_JNW_texture->Render(mrp);
		}
		break;
		case fow_s_JNE:
		{
			if (fow_shadow_JNE_texture == NULL) return false;
			fow_shadow_JNE_texture->UpdateBuffers(m_TopLeftPosition.x + offset.x, m_TopLeftPosition.y + offset.y);
			fow_shadow_JNE_texture->Render(mrp);
		}
		break;
		case fow_s_JSW:
		{
			if (fow_shadow_JSW_texture == NULL) return false;
			fow_shadow_JSW_texture->UpdateBuffers(m_TopLeftPosition.x + offset.x, m_TopLeftPosition.y + offset.y);
			fow_shadow_JSW_texture->Render(mrp);
		}
		break;
		case fow_s_JSE:
		{
			if (fow_shadow_JSE_texture == NULL) return false;
			fow_shadow_JSE_texture->UpdateBuffers(m_TopLeftPosition.x + offset.x, m_TopLeftPosition.y + offset.y);
			fow_shadow_JSE_texture->Render(mrp);
		}
		break;
		#pragma endregion
	/*}
	
	switch (m_fogBit)
	{*
		case fow_n_all:
		{
			if (fow_all_texture == NULL) return false;
			fow_all_texture->UpdateBuffers(m_TopLeftPosition.x + offset.x, m_TopLeftPosition.y + offset.y);
			fow_all_texture->Render(mrp);
		}
		break;
		#pragma region "Straights"
		case fow_n_NNN:
		{
			if (fow_NNN_texture == NULL) return false;
			fow_NNN_texture->UpdateBuffers(m_TopLeftPosition.x + offset.x, m_TopLeftPosition.y + offset.y);
			fow_NNN_texture->Render(mrp);
		}
		break;
		case fow_n_SSS:
		{
			if (fow_SSS_texture == NULL) return false;
			fow_SSS_texture->UpdateBuffers(m_TopLeftPosition.x + offset.x, m_TopLeftPosition.y + offset.y);
			fow_SSS_texture->Render(mrp);
		}
		break;
		case fow_n_EEE:
		{
			if (fow_EEE_texture == NULL) return false;
			fow_EEE_texture->UpdateBuffers(m_TopLeftPosition.x + offset.x, m_TopLeftPosition.y + offset.y);
			fow_EEE_texture->Render(mrp);
		}
		break;
		case fow_n_WWW:
		{
			if (fow_WWW_texture == NULL) return false;
			fow_WWW_texture->UpdateBuffers(m_TopLeftPosition.x + offset.x, m_TopLeftPosition.y + offset.y);
			fow_WWW_texture->Render(mrp);
		}
		break;
		#pragma endregion
		#pragma region "Corners"
		case fow_n_CNW:
		{
			if (fow_CNW_texture == NULL) return false;
			fow_CNW_texture->UpdateBuffers(m_TopLeftPosition.x + offset.x, m_TopLeftPosition.y + offset.y);
			fow_CNW_texture->Render(mrp);
		}
		break;
		case fow_n_CNE:
		{
			if (fow_CNE_texture == NULL) return false;
			fow_CNE_texture->UpdateBuffers(m_TopLeftPosition.x + offset.x, m_TopLeftPosition.y + offset.y);
			fow_CNE_texture->Render(mrp);
		}
		break;
		case fow_n_CSW:
		{
			if (fow_CSW_texture == NULL) return false;
			fow_CSW_texture->UpdateBuffers(m_TopLeftPosition.x + offset.x, m_TopLeftPosition.y + offset.y);
			fow_CSW_texture->Render(mrp);
		}
		break;
		case fow_n_CSE:
		{
			if (fow_CSE_texture == NULL) return false;
			fow_CSE_texture->UpdateBuffers(m_TopLeftPosition.x + offset.x, m_TopLeftPosition.y + offset.y);
			fow_CSE_texture->Render(mrp);
		}
		break;
		#pragma endregion
		#pragma region "Joints"
		case fow_n_JNW:
		{
			if (fow_JNW_texture == NULL) return false;
			fow_JNW_texture->UpdateBuffers(m_TopLeftPosition.x + offset.x, m_TopLeftPosition.y + offset.y);
			fow_JNW_texture->Render(mrp);
		}
		break;
		case fow_n_JNE:
		{
			if (fow_JNE_texture == NULL) return false;
			fow_JNE_texture->UpdateBuffers(m_TopLeftPosition.x + offset.x, m_TopLeftPosition.y + offset.y);
			fow_JNE_texture->Render(mrp);
		}
		break;
		case fow_n_JSW:
		{
			if (fow_JSW_texture == NULL) return false;
			fow_JSW_texture->UpdateBuffers(m_TopLeftPosition.x + offset.x, m_TopLeftPosition.y + offset.y);
			fow_JSW_texture->Render(mrp);
		}
		break;
		case fow_n_JSE:
		{
			if (fow_JSE_texture == NULL) return false;
			fow_JSE_texture->UpdateBuffers(m_TopLeftPosition.x + offset.x, m_TopLeftPosition.y + offset.y);
			fow_JSE_texture->Render(mrp);
		}
		break;
		#pragma endregion
	}*/
	
	if (mFOWMesh == NULL) return false;
	mFOWMesh->UpdateBuffers(m_TopLeftPosition.x + offset.x, m_TopLeftPosition.y + offset.y);
	ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.camera = camera;
	mrp.blendAmount = blendAmount;
	mrp.specifyWorld = true;
	mrp.world = XMMatrixIdentity();
	mFOWMesh->Render(mrp);
	
	return true;
}
//=================================================================================================================
bool TopdownTile::RenderFowShadow(Camera* camera, float blendAmount, XMFLOAT2 offset)
{
	if (mFOWShadowMesh == NULL) return false;
	mFOWShadowMesh->UpdateBuffers(m_TopLeftPosition.x + offset.x, m_TopLeftPosition.y + offset.y);
	ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.camera = camera;
	mrp.blendAmount = blendAmount;
	mrp.specifyWorld = true;
	mrp.world = XMMatrixIdentity();
	mFOWShadowMesh->Render(mrp);

	return true;
}
//=================================================================================================================
bool TopdownTile::Render(Camera* camera)
{
	return Render(camera, XMFLOAT2(0, 0));
}
//=================================================================================================================
bool TopdownTile::Render(Camera* camera, XMFLOAT2 offset)
{
	if (m_CurrentSequence == NULL)
	{
		if (mBaseMesh == NULL) return false;
		mBaseMesh->UpdateBuffers(m_TopLeftPosition.x + offset.x, m_TopLeftPosition.y + offset.y);
		ZShadeSandboxMesh::MeshRenderParameters mrp;
		mrp.camera = camera;
		mrp.specifyWorld = true;
		mrp.world = XMMatrixIdentity();
		mBaseMesh->Render(mrp);
	}
	else
	{
		//Render TopdownTile Animation2D
		RenderAnimation(camera, offset);
	}

	if (m_displayHardbox && mHard) RenderHardboxMesh(camera, offset);

	return true;
}
//=================================================================================================================
bool TopdownTile::Render(Camera* camera, float blendAmount)
{
	return Render(camera, blendAmount, XMFLOAT2(0, 0));
}
//=================================================================================================================
bool TopdownTile::Render(Camera* camera, float blendAmount, XMFLOAT2 offset)
{
	//Render the regular texture below a fog texture that needs transparency
	/*if (m_fogBit != fow_n_non && m_fogBit != fow_n_all && m_bFOW)// || bFowUncovered)
	{
		ZShadeSandboxMesh::MeshRenderParameters mrp;
		mrp.camera = camera;
		mrp.blendAmount = blendAmount;
		mrp.specifyWorld = true;
		mrp.world = XMMatrixIdentity();
		
		if (m_CurrentSequence == NULL)
		{
			if (mBaseMesh == NULL) return false;
			mBaseMesh->UpdateBuffers(m_TopLeftPosition.x + offset.x, m_TopLeftPosition.y + offset.y);
			mBaseMesh->Render(mrp);
		}
		else
		{
			// If the tile has animation render it under the FOW tile
			RenderAnimation(camera, blendAmount, offset);
		}
		
		// Renders a transparent FOW tile above the regular tile
		RenderFow(camera, blendAmount, offset);
		//if (mMesh == NULL) return false;
		//mMesh->UpdateBuffers(m_TopLeftPosition.x + offset.x, m_TopLeftPosition.y + offset.y);
		//mMesh->Render(mrp);
	}
	else if (m_fogBit == fow_n_all && m_bFOW)
	{
		ZShadeSandboxMesh::MeshRenderParameters mrp;
		mrp.camera = camera;
		mrp.blendAmount = blendAmount;
		mrp.specifyWorld = true;
		mrp.world = XMMatrixIdentity();
		if (fow_all_texture == NULL) return false;
		fow_all_texture->UpdateBuffers(m_TopLeftPosition.x + offset.x, m_TopLeftPosition.y + offset.y);
		fow_all_texture->Render(mrp);
	}
	else
	{
		if (m_CurrentSequence == NULL)
		{
			if (mBaseMesh == NULL) return false;
			mBaseMesh->UpdateBuffers(m_TopLeftPosition.x + offset.x, m_TopLeftPosition.y + offset.y);
			ZShadeSandboxMesh::MeshRenderParameters mrp;
			mrp.camera = camera;
			mrp.blendAmount = blendAmount;
			mrp.specifyWorld = true;
			mrp.world = XMMatrixIdentity();
			mBaseMesh->Render(mrp);
		}
		else
		{
			//Render TopdownTile Animation2D
			RenderAnimation(camera, blendAmount, offset);
		}
	}

	if (m_displayHardbox && mHard) RenderHardboxMesh(camera, offset);*/

	if (m_CurrentSequence == NULL)
	{
		if (mBaseMesh == NULL) return false;
		mBaseMesh->UpdateBuffers(m_TopLeftPosition.x + offset.x, m_TopLeftPosition.y + offset.y);
		ZShadeSandboxMesh::MeshRenderParameters mrp;
		mrp.camera = camera;
		mrp.blendAmount = blendAmount;
		mrp.specifyWorld = true;
		mrp.world = XMMatrixIdentity();
		mBaseMesh->Render(mrp);
	}
	else
	{
		//Render TopdownTile Animation2D
		RenderAnimation(camera, blendAmount, offset);
	}

	if (m_displayHardbox && mHard) RenderHardboxMesh(camera, offset);

	return true;
}
//=================================================================================================================
bool TopdownTile::RenderAnimation(Camera* camera)
{
	return RenderAnimation(camera, XMFLOAT2(0, 0));
}
//=================================================================================================================
bool TopdownTile::RenderAnimation(Camera* camera, XMFLOAT2 offset)
{
	AnimationFrame2D* frame = m_CurrentSequence->GetCurrentFrame();
	if (frame == NULL) return false;
	ZShadeSandboxMesh::QuadMesh* texture = frame->GetFrameTexture();
	if (texture == NULL) return false;

	texture->UpdateBuffers(m_TopLeftPosition.x + offset.x, m_TopLeftPosition.y + offset.y);

	ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.camera = camera;
	mrp.specifyWorld = true;
	mrp.world = XMMatrixIdentity();
	texture->Render(mrp);

	return true;
}
//=================================================================================================================
bool TopdownTile::RenderAnimation(Camera* camera, float blendAmount)
{
	return RenderAnimation(camera, blendAmount, XMFLOAT2(0, 0));
}
//=================================================================================================================
bool TopdownTile::RenderAnimation(Camera* camera, float blendAmount, XMFLOAT2 offset)
{
	AnimationFrame2D* frame = m_CurrentSequence->GetCurrentFrame();
	if (frame == NULL) return false;
	ZShadeSandboxMesh::QuadMesh* texture = frame->GetFrameTexture();
	if (texture == NULL) return false;

	texture->UpdateBuffers(m_TopLeftPosition.x + offset.x, m_TopLeftPosition.y + offset.y);

	// Use the transparent shader to render the mesh
	ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.camera = camera;
	mrp.blendAmount = blendAmount;
	mrp.specifyWorld = true;
	mrp.world = XMMatrixIdentity();
	texture->Render(mrp);

	if (m_displayHardbox && mHard) RenderHardboxMesh(camera, offset);

	return true;
}
//=================================================================================================================