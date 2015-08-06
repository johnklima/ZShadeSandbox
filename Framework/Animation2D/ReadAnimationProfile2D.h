//******************************************************************************************************
// ReadAnimationProfile2D.h
//******************************************************************************************************
#ifndef __READANIMATIONPROFILE_H
#define __READANIMATIONPROFILE_H
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
//Includes
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include "FileParser.h"
#include "Animation2D.h"
using namespace std;
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
//Reads one .AnimProfile
class ReadAnimationProfile2D
{
public:
	ReadAnimationProfile2D(D3D* d3d);
	ReadAnimationProfile2D(const ReadAnimationProfile2D& other);
	~ReadAnimationProfile2D();

	bool Read(string p_pProfileFolderName, string base_sprite_path);

	bool LoadSequence(string base_sprite_path);

	Animation2D* GetAnimation();

private:
	struct FBOX
	{
		float x;
		float y;
		float w;
		float h;
	};

	D3D* m_D3DSystem;

	FileParser fileParser;

	Animation2D* m_pAnimation;

	// This store the data needed to load the profile
	string m_pObjectAnimationType;
	// THIS IS THE MAIN ACCESSOR NAME FOR THIS PROFILE
	// This is the profile name that this object animation pertains to.
	string m_pProfileName;
	string m_pAnimationType;
	string m_pSequenceName;
	
	// These are the frame attributes
	vector<string> m_pFrameNames;		   // this variable will hold all the frame names
	map<string, FBOX> m_pFrameRectangles;   // this variable will hold all the frame rectangles
	map<string, int> m_pDirections;         // this variable will hold all the frame directions
};
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
#endif//__READANIMATIONPROFILE_H