//==================================================================================================================================
// PlatformerTile.h
//
//==================================================================================================================================
// History
//
// -Created on 1/15/2015 by Dustin Watson
//==================================================================================================================================
#ifndef __PLATFORMERTILE_H
#define __PLATFORMERTILE_H
//==================================================================================================================================
//==================================================================================================================================
#include "D3D.h"
#include "ZMath.h"
#include "QuadMesh.h"
#include "Sprite.h"
#include "Animation2D.h"
#include "AnimationProfile2D.h"
#include "PhysicsBody2D.h"
class Sprite;
//==================================================================================================================================
class PlatformerTile : public PhysicsBody2D
{
public:
	
	PlatformerTile();
	PlatformerTile(D3D* d3d);
	~PlatformerTile();
	
	void SetD3D(D3D* d3d);
	
	void InitializeBody(int size, PhysicsType pt, ESurfaceType::Type surfaceType, XMFLOAT3 pos);
	bool InitializeMesh(string base_tile_path, string textureFilename, int bitmapWidth, int bitmapHeight);
	void Shutdown();
	
	void UpdateMeshTexture(string base_tile_path, string textureFilename);
	void DisableTexture();
	
	void ClearAnimationList() { if (mAnimationSequences.size() > 0) mAnimationSequences.clear(); }
	Animation2D* CurrentSequenceExists() { return m_CurrentSequence; }
	int GetCurrentFrameIndex() { if (m_CurrentSequence != NULL) return m_CurrentSequence->GetCurrentFrameIndex(); else return -1; }
	bool IsCurrentSequenceOnLastFrame() { if (m_CurrentSequence != NULL) return m_CurrentSequence->IsOnLastFrame(); else return false; }
	void SetCurrentSequenceNotOnLastFrame() { if (m_CurrentSequence != NULL) m_CurrentSequence->SetNotOnLastFrame(); }
	void UpdateAnimation();
	void AddAnimationSequence(Animation2D* sequence);
	void SetSequence(int i);
	void NullSequence() { m_CurrentSequence = NULL; ClearAnimationList(); }

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
	
	PlatformerTile* Clone(GameDirectory2D* gd);
	
public:
	
	string GetTileTextureName();
	string GetTileTextureCopyName();
	ZShadeSandboxMesh::QuadMesh* GetMesh() { return mMesh; }
	bool& Hard() { return mHard; }
	bool Hard() const { return mHard; }
	int Size() { return mSize; }
	int Width() { return m_EngineOptions->TILE_SIZE; }
	int Height() { return m_EngineOptions->TILE_SIZE; }
	int& AnimationSpeed() { return m_animationSpeed; }
	int AnimationSpeed() const { return m_animationSpeed; }
	bool& DisplayHardbox() { return m_displayHardbox; }
	bool DisplayHardbox() const { return m_displayHardbox; }
	string& AnimationProfileName() { return m_animationProfileName; }
	string AnimationProfileName() const { return m_animationProfileName; }
	
protected:
	
	D3D* m_D3DSystem;
	EngineOptions* m_EngineOptions;
	
	// Need to implement a movable block
	// Can this be a broken block (This needs to be done in physics body)
	bool bCanBreak;
	
	// Not really using these
	bool bUseSlope;
	int iSlopeDir;
	
	int m_animationSpeed;
	int mSize;
	bool mHard;
	bool m_displayHardbox;
	
	ZShadeSandboxMesh::QuadMesh *mMesh;
	string m_animationProfileName;
	Animation2D* m_CurrentSequence;
	//The animation sequences for the tile
	vector<Animation2D*> mAnimationSequences;
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__PLATFORMERTILE_H