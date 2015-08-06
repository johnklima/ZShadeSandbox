//******************************************************************************************************
// AnimationProfile2D.h
//******************************************************************************************************
#ifndef __ANIMATIONPROFILE_H
#define __ANIMATIONPROFILE_H
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
//Includes
#include <string>
#include <vector>
#include <fstream>
#include "ReadAnimationProfile2D.h"
using namespace std;
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
//Profiles are in .AnimProfile ext
//-----------------------------------------------------------------------------------------------------------------
class AnimationProfile2D
{
public:
	AnimationProfile2D(D3D* d3d);
	AnimationProfile2D(const AnimationProfile2D& other);
	~AnimationProfile2D();

	//Load the names of all the animation for each profile from the AnimProfileList.ini file
	bool LoadAnimationProfileNames(string pProfileName, char* pAnimationType, string base_game_folder);

	//Read a new sequence
	void SetNewFolder(string pFolderName);
	//AnimationType : SPRITE, TILE, GLYPH
	bool ReadProfile(string pProfileName, char* pAnimationType, string base_sprite_path, string base_game_folder);

	vector<Animation2D*> GetAnimations();

private:
	D3D* m_D3DSystem;
	string m_ProfileFolderName;
	vector<string> m_ProfileNames;
	vector<Animation2D*> m_Animations;
};
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
#endif//__ANIMATIONPROFILE_H