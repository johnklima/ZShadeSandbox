#include "AnimationProfile2D.h"
#include "CGlobal.h"
#include "Convert.h"
//-----------------------------------------------------------------------------------------------------------------
AnimationProfile2D::AnimationProfile2D(D3D* d3d)
:	m_D3DSystem(d3d)
{
}
//-----------------------------------------------------------------------------------------------------------------
AnimationProfile2D::AnimationProfile2D(const AnimationProfile2D& other)
{
}
//-----------------------------------------------------------------------------------------------------------------
AnimationProfile2D::~AnimationProfile2D()
{
}
//-----------------------------------------------------------------------------------------------------------------
bool AnimationProfile2D::LoadAnimationProfileNames(string pProfileName, char* pAnimationType, string base_game_folder)
{
	//base_game_folder.append("\\AnimationProfiles");
	string path = base_game_folder;//CGlobal::FindInitialPath();
	path.append("\\");

	string dir = CGlobal::LocateAnimationDirectory("AnimProfileList.ini", (const char*)pProfileName.c_str(), pAnimationType, false, "");

	path.append( dir );

	if (!CGlobal::DoesFileExist(path))
	{
		MessageBox(NULL, "AnimProfileList.ini does not exist", "Animation2D Profile", MB_OK);
		return false;
	}

	const char* finalPath = path.c_str();

	//Store all the profile names
	
	char buffer[MAX_PATH_LENGTH];

	ifstream in(finalPath, ios::in);

	while (in.peek() > -1)
	{
		in.getline(buffer, MAX_PATH_LENGTH);
		m_ProfileNames.push_back(ZShadeSandboxGlobal::Convert::ConvertCharBufferToString(buffer));
	}

	in.close();

	return true;
}
//-----------------------------------------------------------------------------------------------------------------
void AnimationProfile2D::SetNewFolder(string pFolderName)
{
	m_ProfileFolderName = pFolderName;
}
//-----------------------------------------------------------------------------------------------------------------
bool AnimationProfile2D::ReadProfile(string pProfileName, char* pAnimationType, string base_sprite_path, string base_game_folder)
{
	bool pLoaded = false;

	//Load the profile names from the folder
	string path = base_game_folder;//CGlobal::FindInitialPath();
	path.append("\\");

	string CurrentPath;

	//Append the main Directory
	CurrentPath.append( "AnimationProfiles\\" );

	//Is it a Sprite or a Tile
	CurrentPath.append( pAnimationType );
	CurrentPath.append( "\\" );

	//Get the profile name
	CurrentPath.append( pProfileName );

	path.append( CurrentPath );

	char* in_path = new char[strlen(path.c_str()) + 1];
	strcpy_s(in_path, strlen(path.c_str()) + 1, path.c_str());
	m_ProfileNames = CGlobal::GetFilenames(in_path, "");

	vector<string>::iterator it = m_ProfileNames.begin();

	for (; it != m_ProfileNames.end(); it++)
	{
		string path = base_game_folder;
		
		path.append("\\");
		path.append(CGlobal::LocateAnimationDirectory((*it).c_str(), (const char*)pProfileName.c_str(), pAnimationType, false, ""));

		if (!CGlobal::DoesFileExist(path))
		{
			MessageBox(NULL, "Could not locate .AnimProfile", "Animation2D Profile", MB_OK);
			return false;
		}

		//Now read the animation profile
		ReadAnimationProfile2D* profile = new ReadAnimationProfile2D(m_D3DSystem);

		if (!profile->Read(path, base_sprite_path))
		{
			return false;
		}

		//Get the loaded animation
		m_Animations.push_back(profile->GetAnimation());

		pLoaded = true;
	}

	return pLoaded;
}
//-----------------------------------------------------------------------------------------------------------------
vector<Animation2D*> AnimationProfile2D::GetAnimations()
{
	return m_Animations;
}
//-----------------------------------------------------------------------------------------------------------------