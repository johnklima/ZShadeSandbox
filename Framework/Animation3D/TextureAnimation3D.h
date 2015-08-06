//==============================================================================================================================
// TextureAnimation3D.h
//
//==============================================================================================================================
// History
//
// -Created on 5/30/2015  by Dustin Watson
//==============================================================================================================================
#ifndef __TEXTUREANIMATION3D_H
#define __TEXTUREANIMATION3D_H
//==============================================================================================================================
//==============================================================================================================================

//
// Includes
//

#include "D3D.h"
#include "Camera.h"
#include "XMMath.h"
#include "Texture.h"
#include "SleepTimer.h"

//==============================================================================================================================
//==============================================================================================================================

class TextureAnimation3D
{
public:

	TextureAnimation3D(D3D* d3d, string basePath, vector<string> filenames);

	ID3D11ShaderResourceView* CurrentTextureSRV();

	void LoadTextureFrame(string basePath, string filename);

	//Reset the animation to the first frame
	void ResetAnimation();

	//Go to the next frame in the sequence
	void NextFrame(int speed);

	//Update the animation sequence
	void Update(int speed);

	void Pause() { bPause = true; }
	void UnPause() { bPause = false; }

private:

	D3D* mD3DSystem;

	SleepTimer* mAnimSleep;

	//Should we pause this animation
	bool bPause;

	bool bLastFrame;

	float fCurrFrameID;

	Texture* mCurrTexture;

	vector<Texture*> m_pTextures;
};

//==============================================================================================================================
//==============================================================================================================================
#endif//__TEXTUREANIMATION3D_H