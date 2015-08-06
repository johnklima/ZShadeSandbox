//======================================================================================================
// TopdownTile.h
//
//======================================================================================================
// History
//
// Created in fall 2013 by Dustin Watson
//======================================================================================================
#ifndef __TOPDOWNTILE_H
#define __TOPDOWNTILE_H
//=================================================================================================================
//=================================================================================================================

//
//Includes
//

#include "QuadMesh.h"
#include "Sprite.h"
#include "D3D.h"
#include "Animation2D.h"
#include "AnimationProfile2D.h"
#include "PhysicsBody2D.h"

//=================================================================================================================
//=================================================================================================================
class TopdownTile : public PhysicsBody2D
{
public:
	
	TopdownTile();
	TopdownTile(D3D* d3d);
	~TopdownTile();

	void SetD3D(D3D* d3d);
	
	void InitData();
	void InitializeBody(int size, XMFLOAT3 pos);
	bool InitializeMesh(string base_tile_path, string textureFilename, int bitmapWidth, int bitmapHeight);
	void Shutdown();
	
	void UpdateToFOWAllMeshTexture();
	void UpdateMeshTexture(string base_tile_path, string textureFilename);
	//void DisableTexture() { if (mMesh != NULL) delete mMesh; mMesh = NULL; }
	
	void ClearAnimationList() { if (mAnimationSequences.size() > 0) mAnimationSequences.clear(); }
	Animation2D* CurrentSequenceExists() { return m_CurrentSequence; }
	int GetCurrentFrameIndex() { if (m_CurrentSequence != NULL) return m_CurrentSequence->GetCurrentFrameIndex(); else return -1; }
	bool IsCurrentSequenceOnLastFrame() { if (m_CurrentSequence != NULL) return m_CurrentSequence->IsOnLastFrame(); else return false; }
	void SetCurrentSequenceNotOnLastFrame() { if (m_CurrentSequence != NULL) m_CurrentSequence->SetNotOnLastFrame(); }
	void UpdateAnimation();
	void AddAnimationSequence(Animation2D* sequence);
	void SetSequence(int i);
	void NullSequence() { m_CurrentSequence = NULL; ClearAnimationList(); }
	
	void CalculateFowName();

	//If a fog bit is present the texture will be updated
	//No Fog, All Fog, In Fog Circle
	void UpdateTexture();

	bool RenderFow(Camera* camera, float blendAmount, XMFLOAT2 offset);
	bool RenderFowShadow(Camera* camera, float blendAmount, XMFLOAT2 offset);

	// Render non-transparent texture
	bool Render(Camera* camera);
	bool Render(Camera* camera, XMFLOAT2 offset);
	
	// Render transparent texture
	bool Render(Camera* camera, float blendAmount);
	bool Render(Camera* camera, float blendAmount, XMFLOAT2 offset);
	
	// Render non-transparent animation
	bool RenderAnimation(Camera* camera);
	bool RenderAnimation(Camera* camera, XMFLOAT2 offset);
	
	// Render transparent animation
	bool RenderAnimation(Camera* camera, float blendAmount);
	bool RenderAnimation(Camera* camera, float blendAmount, XMFLOAT2 offset);

	TopdownTile* Clone(GameDirectory2D* gd);
	
public:
	
	string GetTileMeshTextureName();
	string GetTileBaseTextureName();
	
	ZShadeSandboxMesh::QuadMesh* Mesh() { return mBaseMesh; }
	bool& FOW() { return m_bFOW; }
	bool FOW() const { return m_bFOW; }
	int& FOWRadius() { return m_iFOWRadius; }
	int FOWRadius() const { return m_iFOWRadius; }
	bool& FOWFlashlight() { return m_bFOWFlashlight; }
	bool FOWFlashlight() const { return m_bFOWFlashlight; }
	bool& FOWUncovered() { return m_bFOWUncovered; }
	bool FOWUncovered() const { return m_bFOWUncovered; }
	bool& FowUncovered() { return bFowUncovered; }
	bool FowUncovered() const { return bFowUncovered; }
	unsigned short& FogBit() { return m_fogBit; }
	unsigned short FogBit() const { return m_fogBit; }
	unsigned short& FogBitShadow() { return m_fogBitShadow; }
	unsigned short FogBitShadow() const { return m_fogBitShadow; }
	bool& Hard() { return mHard; }
	bool Hard() const { return mHard; }
	int Size() { return mSize; }
	int Width() { return m_engineOptions->TILE_SIZE; }
	int Height() { return m_engineOptions->TILE_SIZE; }
	int& AnimationSpeed() { return m_animationSpeed; }
	int AnimationSpeed() const { return m_animationSpeed; }
	bool& DisplayHardbox() { return m_displayHardbox; }
	bool DisplayHardbox() const { return m_displayHardbox; }
	string& AnimationProfileName() { return m_animationProfileName; }
	string AnimationProfileName() const { return m_animationProfileName; }
	string& BaseTextureName() { return mBaseTextureName; }
	string BaseTextureName() const { return mBaseTextureName; }
	bool& IsInFowCircle() { return bInCircle; }
	bool IsInFowCircle() const { return bInCircle; }
	bool& FowBitShadow() { return bFowBitShadow; }
	bool FowBitShadow() const { return bFowBitShadow; }
	bool& FowAllUncovered() { return bFowAllUncovered; }
	bool FowAllUncovered() const { return bFowAllUncovered; }
	bool& HasFowShadow() { return bHasFowShadow; }
	bool HasFowShadow() const { return bHasFowShadow; }
	bool& InPlayerFowCircle() { return bInPlayerFowCircle; }
	bool InPlayerFowCircle() const { return bInPlayerFowCircle; }
	string& FowName() { return m_fowName; }
	string FowName() const { return m_fowName; }

private:
	
	ZShadeSandboxMesh::QuadMesh* fow_shadow_texture;
	ZShadeSandboxMesh::QuadMesh* fow_shadow_CNW_texture;
	ZShadeSandboxMesh::QuadMesh* fow_shadow_CNE_texture;
	ZShadeSandboxMesh::QuadMesh* fow_shadow_CSW_texture;
	ZShadeSandboxMesh::QuadMesh* fow_shadow_CSE_texture;
	ZShadeSandboxMesh::QuadMesh* fow_shadow_JNW_texture;
	ZShadeSandboxMesh::QuadMesh* fow_shadow_JNE_texture;
	ZShadeSandboxMesh::QuadMesh* fow_shadow_JSW_texture;
	ZShadeSandboxMesh::QuadMesh* fow_shadow_JSE_texture;
	ZShadeSandboxMesh::QuadMesh* fow_shadow_NNN_texture;
	ZShadeSandboxMesh::QuadMesh* fow_shadow_SSS_texture;
	ZShadeSandboxMesh::QuadMesh* fow_shadow_EEE_texture;
	ZShadeSandboxMesh::QuadMesh* fow_shadow_WWW_texture;
	ZShadeSandboxMesh::QuadMesh* fow_all_texture;
	ZShadeSandboxMesh::QuadMesh* fow_CNW_texture;
	ZShadeSandboxMesh::QuadMesh* fow_CNE_texture;
	ZShadeSandboxMesh::QuadMesh* fow_CSW_texture;
	ZShadeSandboxMesh::QuadMesh* fow_CSE_texture;
	ZShadeSandboxMesh::QuadMesh* fow_JNW_texture;
	ZShadeSandboxMesh::QuadMesh* fow_JNE_texture;
	ZShadeSandboxMesh::QuadMesh* fow_JSW_texture;
	ZShadeSandboxMesh::QuadMesh* fow_JSE_texture;
	ZShadeSandboxMesh::QuadMesh* fow_NNN_texture;
	ZShadeSandboxMesh::QuadMesh* fow_SSS_texture;
	ZShadeSandboxMesh::QuadMesh* fow_EEE_texture;
	ZShadeSandboxMesh::QuadMesh* fow_WWW_texture;
	
	// Rendered mesh of the tile where FOW is be rendered if enabled
	ZShadeSandboxMesh::QuadMesh *mFOWMesh;
	
	// Rendered mesh of the tile where a FOW shadow will be rendered inbetween the base image and FOW
	ZShadeSandboxMesh::QuadMesh *mFOWShadowMesh;

	// Non-animated mesh that contains the base image for the tile
	ZShadeSandboxMesh::QuadMesh *mBaseMesh;

	bool bFowBitShadow;

	D3D* m_D3DSystem;
	D3D_VERSION m_d3dvers;
	EngineOptions* m_engineOptions;
	
	string mBaseTextureName;
	
	string m_animationProfileName;
	Animation2D* m_CurrentSequence;
	//The animation sequences for the sprite
	vector<Animation2D*> mAnimationSequences;
	
	string m_fowName;

	bool bInPlayerFowCircle;
	bool bHasFowShadow;
	bool bInCircle;
	int mSize;
	bool mHard;
	bool m_displayHardbox;
	int m_animationSpeed;
	unsigned short m_fogBit;
	unsigned short m_fogBitShadow;
	bool m_bFOW;
	int m_iFOWRadius;
	bool m_bFOWFlashlight;
	bool m_bFOWUncovered;
	bool bFowUncovered;
	bool bFowAllUncovered;
};
//=================================================================================================================
//=================================================================================================================
#endif//__TOPDOWNTILE_H