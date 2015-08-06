//******************************************************************************************************
// Animation2D.h
//******************************************************************************************************
#ifndef __ANIMATION_H
#define __ANIMATION_H
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
#include "AnimationFrame2D.h"
#include "QuadMesh.h"
#include "D3D.h"
#include "SleepTimer.h"
#include <vector>
using namespace std;
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
//Animation2D Defines
#define IDLE_UP 0
#define IDLE_DOWN 1
#define IDLE_LEFT 2
#define IDLE_RIGHT 3
#define WALK_UP 4
#define WALK_DOWN 5
#define WALK_LEFT 6
#define WALK_RIGHT 7
#define ATTACK_UP 8
#define ATTACK_DOWN 9
#define ATTACK_LEFT 10
#define ATTACK_RIGHT 11
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
//Animation2D Sequence
class Animation2D
{
public:
	Animation2D(D3D* d3d);
	Animation2D(const Animation2D& other);
	~Animation2D();

	void AddAnimationFrame(string frameName, string path, string base_sprite_path, float x, float y, float w, float h, int dir);

	AnimationFrame2D* GetCurrentFrame();

	AnimationFrame2D* GetFrame(int i);

	vector<AnimationFrame2D*> GetFrames();

	//Reset the animation to the first frame
	void ResetAnimation();

	//Go to the next frame in the sequence
	void NextFrame(int speed);

	//Update the animation sequence
	void Update(int speed);

	void Pause() { mPause = true; }
	void UnPause() { mPause = false; }

	int GetCurrentFrameIndex() { return mCurrentFrameIndex; }
	bool IsOnLastFrame() { return mLastFrame; }
	void SetNotOnLastFrame() { mLastFrame = false; }

	void LoadFrameTextures();

private:
	D3D* m_D3DSystem;

	DWORD m_tickCount;
	DWORD m_timer;

	vector<AnimationFrame2D*> mFrames;

	SleepTimer* mAnimSleep;

	float mCurrentFrameIndex;

	//The currently active frame
	AnimationFrame2D* mCurrentFrame;

	//Should we pause this animation
	bool mPause;

	//Are we on the last frame
	bool mLastFrame;
};
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
#endif//__ANIMATION_H