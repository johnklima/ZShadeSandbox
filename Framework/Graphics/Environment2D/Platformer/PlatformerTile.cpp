#include "PlatformerTile.h"
#include "Scalar.h"
#include "MaterialManager.h"
//==================================================================================================================================
//==================================================================================================================================
PlatformerTile::PlatformerTile()
:	PhysicsBody2D()
,	m_D3DSystem(0)
,	m_EngineOptions(0)
,	mSize(0)
,	mHard(false)
,	m_displayHardbox(false)
,	m_animationProfileName("NONE")
,	m_animationSpeed(0)
,	mMesh(0)
,	m_CurrentSequence(NULL)
{
}
//==================================================================================================================================
PlatformerTile::PlatformerTile(D3D* d3d)
:	PhysicsBody2D(d3d)
,	m_D3DSystem(d3d)
,	mHard(false)
,	m_displayHardbox(false)
,	m_animationProfileName("NONE")
,	m_animationSpeed(0)
,	mMesh(0)
,	m_CurrentSequence(NULL)
{
	m_EngineOptions = d3d->GetEngineOptions();
	mSize = m_EngineOptions->TILE_SIZE;
}
//==================================================================================================================================
PlatformerTile::~PlatformerTile()
{
}
//==================================================================================================================================
void PlatformerTile::InitializeBody(int size, PhysicsType pt, ESurfaceType::Type surfaceType, XMFLOAT3 pos)
{
	mSize = size;
	PhysicsBody2D::Initialize(pos, pt, surfaceType, size, size);
}
//==================================================================================================================================
string PlatformerTile::GetTileTextureName()
{
	if (mMesh != NULL)
	{
		return mMesh->TextureName();
	}

	return "";
}
//==================================================================================================================================
bool PlatformerTile::InitializeMesh(string base_tile_path, string textureFilename, int bitmapWidth, int bitmapHeight)
{
	// Using transparent alpha texture capability since this tile belongs in a platformer level and transparency could be needed
	MaterialManager::Instance()->AddTransparentDiffuse(m_D3DSystem, base_tile_path,
		textureFilename, textureFilename, textureFilename, m_EngineOptions->m_TransparencyKey);
	
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
	mMesh = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp, bitmapWidth, bitmapHeight, true);
	if (!mMesh) return false;
	
	return true;
}
//==================================================================================================================================
void PlatformerTile::UpdateMeshTexture(string base_tile_path, string textureFilename)
{
	// Creates new material if one does not exist, if it does exist then do nothing
	MaterialManager::Instance()->AddTransparentDiffuse(m_D3DSystem, base_tile_path,
		textureFilename, textureFilename, textureFilename, m_EngineOptions->m_TransparencyKey);
	
	// Grab the material whether it existed or not
	mMesh->SetMaterial(MaterialManager::Instance()->GetMaterial(textureFilename));
}
//==================================================================================================================================
void PlatformerTile::Shutdown()
{
}
//==================================================================================================================================
void PlatformerTile::DisableTexture()
{
	if (mMesh != NULL)
	{
		delete mMesh;
		mMesh = NULL;
	}
}
//==================================================================================================================================
void PlatformerTile::UpdateAnimation()
{
	if (m_CurrentSequence == NULL) return;

	m_CurrentSequence->Update(m_animationSpeed);
}
//==================================================================================================================================
void PlatformerTile::AddAnimationSequence(Animation2D* sequence)
{
	if (sequence == NULL) return;

	sequence->LoadFrameTextures();
	mAnimationSequences.push_back(sequence);
}
//==================================================================================================================================
void PlatformerTile::SetSequence(int i)
{
	m_CurrentSequence = mAnimationSequences[i];
}
//==================================================================================================================================
PlatformerTile* PlatformerTile::Clone(GameDirectory2D* gd)
{
	PlatformerTile* t = new PlatformerTile(m_D3DSystem);
	
	t->InitializeBody(mSize, eType, mSurfaceType, XMFLOAT3(m_TopLeftPosition.x, m_TopLeftPosition.y, 0));
	
	t->Hard() = mHard;
	t->TileInvisible() = bTileInvisible;
	t->AnimationProfileName() = m_animationProfileName;
	t->AnimationSpeed() = m_animationSpeed;
	t->DisplayHardbox() = m_displayHardbox;
	
	string base_path = "";
	base_path = gd->m_tiles_path;
	
	if (mMesh != 0)
	{
		t->InitializeMesh(base_path, mMesh->TextureName(), mSize, mSize);
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

				PlatformerTile* tile = t;

				tile->ClearAnimationList();

				for (int i = 0; i < anims.size(); i++)
				{
					tile->AddAnimationSequence(anims[i]);
				}

				tile->SetSequence(0);
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
//==================================================================================================================================
bool PlatformerTile::Render(Camera* camera)
{
	return Render(camera, XMFLOAT2(0, 0));
}
//==================================================================================================================================
bool PlatformerTile::Render(Camera* camera, XMFLOAT2 offset)
{
	if (m_CurrentSequence == NULL)
	{
		if (mMesh == NULL) return false;
		
		mMesh->UpdateBuffers(m_TopLeftPosition.x + offset.x, m_TopLeftPosition.y + offset.y);
		
		ZShadeSandboxMesh::MeshRenderParameters mrp;
		mrp.camera = camera;
		mrp.specifyWorld = true;
		mrp.world = XMMatrixIdentity();
		mMesh->Render(mrp);
	}
	else
	{
		//Render Tile Animation2D
		RenderAnimation(camera, offset);
	}

	if (m_displayHardbox && mHard) RenderHardboxMesh(camera, offset);

	return true;
}
//==================================================================================================================================
bool PlatformerTile::Render(Camera* camera, float blendAmount)
{
	return Render(camera, blendAmount, XMFLOAT2(0, 0));
}
//==================================================================================================================================
bool PlatformerTile::Render(Camera* camera, float blendAmount, XMFLOAT2 offset)
{
	if (m_CurrentSequence == NULL)
	{
		if (mMesh == NULL) return false;
		
		mMesh->UpdateBuffers(m_TopLeftPosition.x + offset.x, m_TopLeftPosition.y + offset.y);
		
		// Use the transparent shader to render the mesh
		ZShadeSandboxMesh::MeshRenderParameters mrp;
		mrp.camera = camera;
		mrp.blendAmount = blendAmount;
		mrp.specifyWorld = true;
		mrp.world = XMMatrixIdentity();
		mMesh->Render(mrp);
	}
	else
	{
		RenderAnimation(camera, blendAmount, offset);
	}

	if (m_displayHardbox && mHard) RenderHardboxMesh(camera, offset);
	
	return true;
}
//==================================================================================================================================
bool PlatformerTile::RenderAnimation(Camera* camera)
{
	return RenderAnimation(camera, XMFLOAT2(0, 0));
}
//==================================================================================================================================
bool PlatformerTile::RenderAnimation(Camera* camera, XMFLOAT2 offset)
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
//==================================================================================================================================
bool PlatformerTile::RenderAnimation(Camera* camera, float blendAmount)
{
	return RenderAnimation(camera, blendAmount, XMFLOAT2(0, 0));
}
//==================================================================================================================================
bool PlatformerTile::RenderAnimation(Camera* camera, float blendAmount, XMFLOAT2 offset)
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
//==================================================================================================================================