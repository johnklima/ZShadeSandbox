#include "Animation2D.h"
//-----------------------------------------------------------------------------------------------------------------
Animation2D::Animation2D(D3D* d3d)
:	mCurrentFrameIndex(0.0),
	mPause(false),
	mLastFrame(false),
	mCurrentFrame(NULL),
	m_D3DSystem(d3d)
{
	m_timer = 0;
	m_tickCount = GetTickCount();
	mAnimSleep = new SleepTimer();
}
//-----------------------------------------------------------------------------------------------------------------
Animation2D::Animation2D(const Animation2D& other)
{
}
//-----------------------------------------------------------------------------------------------------------------
Animation2D::~Animation2D()
{
}
//-----------------------------------------------------------------------------------------------------------------
AnimationFrame2D* Animation2D::GetCurrentFrame()
{
	return mCurrentFrame;
}
//-----------------------------------------------------------------------------------------------------------------
void Animation2D::AddAnimationFrame(string frameName, string path, string base_sprite_path, float x, float y, float w, float h, int dir)
{
	AnimationFrame2D* newFrame = new AnimationFrame2D(m_D3DSystem);

	newFrame->SetFrameName(frameName);
	newFrame->SetFramePath(path);
	newFrame->SetFrameBasePath(base_sprite_path);
	newFrame->SetFrameBox(x, y, w, h);
	newFrame->SetDirection(dir);

	mFrames.push_back( newFrame );
}
//-----------------------------------------------------------------------------------------------------------------
AnimationFrame2D* Animation2D::GetFrame(int i)
{
	return mFrames[i];
}
//-----------------------------------------------------------------------------------------------------------------
vector<AnimationFrame2D*> Animation2D::GetFrames()
{
	return mFrames;
}
//-----------------------------------------------------------------------------------------------------------------
void Animation2D::ResetAnimation()
{
	mCurrentFrameIndex = 0;
}
//-----------------------------------------------------------------------------------------------------------------
void Animation2D::NextFrame(int speed)
{
	//Sleep for 1/speed of a second (if speed=0 then do not sleep at all)
	mAnimSleep->Sleep(speed);

	if (mAnimSleep->IsTimeUp())
	//if (m_timer < m_tickCount)
	{
		m_timer = m_tickCount + ((rand() % 2000));

		int testFrame = (int)(mCurrentFrameIndex + 1.0f);

		if (testFrame < mFrames.size())
		{
			mLastFrame = false;
			mCurrentFrameIndex += 0.1f;
		}
		else
		{
			mLastFrame = true;
			ResetAnimation();
		}
	}
	else
		m_timer = 0;
}
//-----------------------------------------------------------------------------------------------------------------
void Animation2D::Update(int speed)
{
	//if (mCurrentFrame == NULL) return;

	if (!mPause)
	{
		//Set the current frame
		mCurrentFrame = mFrames[(int)mCurrentFrameIndex];

		//Get the next frame
		NextFrame(speed);
	}

	m_tickCount = GetTickCount();
}
//-----------------------------------------------------------------------------------------------------------------
void Animation2D::LoadFrameTextures()
{
	vector<AnimationFrame2D*>::iterator it;
	for (it = mFrames.begin(); it != mFrames.end(); it++)
	{
		AnimationFrame2D* frame = (*it);
		frame->LoadFrameTexture();
	}
}
//-----------------------------------------------------------------------------------------------------------------