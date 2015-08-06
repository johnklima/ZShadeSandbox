#include "TextureAnimation3D.h"
//==============================================================================================================================
//==============================================================================================================================
TextureAnimation3D::TextureAnimation3D(D3D* d3d, string basePath, vector<string> filenames)
:   mD3DSystem(d3d)
,   fCurrFrameID(0.0f)
,   bPause(false)
,   bLastFrame(false)
{
	mAnimSleep = new SleepTimer();
	mCurrTexture = 0;

	for (int i = 0; i < filenames.size(); i++)
	{
		LoadTextureFrame(basePath, filenames[i]);
	}
}
//==============================================================================================================================
void TextureAnimation3D::LoadTextureFrame(string basePath, string filename)
{
	Texture* tex = new Texture(mD3DSystem);
	tex->Initialize(basePath, filename);
	m_pTextures.push_back(tex);
}
//==============================================================================================================================
ID3D11ShaderResourceView* TextureAnimation3D::CurrentTextureSRV()
{
	if (mCurrTexture == 0) return 0;
	return mCurrTexture->getTexture11();
}
//==============================================================================================================================
void TextureAnimation3D::ResetAnimation()
{
	fCurrFrameID = 0.0f;
}
//==============================================================================================================================
void TextureAnimation3D::NextFrame(int speed)
{
	//Sleep for 1/speed of a second (if speed=0 then do not sleep at all)
	mAnimSleep->Sleep(speed);

	if (mAnimSleep->IsTimeUp())
	{
		int testFrame = (int)(fCurrFrameID + 1.0f);

		if (testFrame < m_pTextures.size())
		{
			bLastFrame = false;
			fCurrFrameID += 0.1f;
		}
		else
		{
			bLastFrame = true;
			ResetAnimation();
		}
	}
}
//==============================================================================================================================
void TextureAnimation3D::Update(int speed)
{
	if (!bPause)
	{
		//Set the current frame
		mCurrTexture = m_pTextures[(int)fCurrFrameID];

		//Get the next frame
		NextFrame(speed);
	}
}
//==============================================================================================================================