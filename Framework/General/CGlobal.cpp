#include "CGlobal.h"
#include "Convert.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <windows.h>
#include <Lmcons.h>
#include "CreateGameDirectory.h"
#include "ZShadeMessageCenter.h"
#include "FileParser.h"
#include "GlobalTokens.h"
#pragma comment(lib, "userenv.lib")
#pragma comment(lib, "Advapi32.lib")
//================================================================================================================
//================================================================================================================
CFileWriter::CFileWriter()
{
}
//================================================================================================================
void CFileWriter::Open(string filename)
{
	outputFile.open(filename.c_str());
}
//================================================================================================================
void CFileWriter::Write(string contents)
{
	outputFile << contents;
}
//================================================================================================================
void CFileWriter::Close()
{
	outputFile.close();
}
//================================================================================================================
uintmax_t CGlobal::FileSize(std::string filename)
{
	filesystem::path p(filename);
	if (filesystem::exists(p) && filesystem::is_regular_file(p))
		return filesystem::file_size(p);
	return 0;
}
//================================================================================================================
vector<string> CGlobal::GetAvailableDrives()
{
	DWORD bitmask = GetLogicalDrives();

	vector<string> drivesAvailable;
	vector<string> driveList;

	//Can add the rest of the alphabet.
	driveList.push_back("a:");
	driveList.push_back("b:");
	driveList.push_back("c:");
	driveList.push_back("d:");
	driveList.push_back("e:");
	driveList.push_back("f:");
	driveList.push_back("g:");
	driveList.push_back("h:");
	driveList.push_back("i:");

	for (int i = 0; i < driveList.size(); i++)
	{
		string drive = driveList[i];

		if ((bitmask & (1 << i)) == 0) //Shift bitmask and if 0 drive is free
		{
			// Free and does not exist in the system
		}
		else
		{
			drivesAvailable.push_back(drive);
		}
	}

	return drivesAvailable;
}
//================================================================================================================
BOOL CGlobal::GetMediaFile( char* file, char path[] )
{
    char exeName[MAX_PATH] = {0};
    char exeFolder[MAX_PATH] = {0};

    // Get full executable path
    GetModuleFileName( NULL, exeFolder, MAX_PATH );
    exeFolder[MAX_PATH - 1] = 0;

    // Get pointer to beginning of executable file name
    // which is after the last slash
    char* pCutPoint = NULL;
    for ( int i = 0; i < MAX_PATH; i++ )
    {
        if ( exeFolder[i] == '\\' )
        {
            pCutPoint = &exeFolder[i + 1];
        }
    }

    if ( pCutPoint )
    {
        // Copy over the exe file name
        strcpy_s( exeName, pCutPoint );

        // Chop off the exe file name from the path so we
        // just have the exe directory
        *pCutPoint = 0;

        // Get pointer to start of the .exe extension 
        pCutPoint = NULL;
        for ( int i = 0; i < MAX_PATH; i++ )
        {
            if ( exeName[i] == '.' )
            {
                pCutPoint = &exeName[i];
            }
        }
        // Chop the .exe extension from the exe name
        if ( pCutPoint )
        {
            *pCutPoint = 0;
        }

        // Add a slash
        strcat( exeName, "\\" );
    }

    // Search all the folders in searchFolders
    if ( SearchFolders( file, exeFolder, exeName, path ) )
    {
        return TRUE;
    }

    // Search all the folders in searchFolders with media\ appended to the end
    char mediaFile[MAX_PATH] = "resources\\";
    strcat( mediaFile, file );
    if ( SearchFolders( mediaFile, exeFolder, exeName, path ) )
    {
        return TRUE;
    }

    return FALSE;
}
//================================================================================================================
BOOL CGlobal::SearchFolders( char* filename, char* exeFolder, char* exeName, char fullPath[] )
{
    char* searchFolders[] = 
    { 
        ".\\", "..\\", "..\\..\\", "%s", "%s..\\", "%s..\\..\\", "%s..\\%s", "%s..\\..\\%s"
    };

    // Look through each folder to find the file
    char currentPath[MAX_PATH] = {0};
    for ( int i = 0; i < 8; i++ )
    {
        sprintf( currentPath, searchFolders[i], exeFolder, exeName );
        strcat( currentPath, filename );
        if ( GetFileAttributes( currentPath ) != INVALID_FILE_ATTRIBUTES )
        {
            strcpy( fullPath, currentPath );
            return TRUE;
        }
    }

    // Crap...didn't find it
    return FALSE;
}
//================================================================================================================
string CGlobal::FindInitialPath()
{
	char szFullPath[ MAX_PATH + 1 ];

	GetCurrentDirectory( MAX_PATH, szFullPath );

	return szFullPath;
}
//================================================================================================================
string CGlobal::LocateAnimationDirectory(const char* pFileName, const char* pProfileName, char* pExtraType, bool pIsExt, char* pExtName)
{
	string CurrentPath;

	//Append the main Directory
	CurrentPath.append( "AnimationProfiles\\" );

	//Is it a Sprite or a Tile
	CurrentPath.append( pExtraType );
	CurrentPath.append( "\\" );

	//Get the profile name
	CurrentPath.append( pProfileName );
	CurrentPath.append( "\\" );

	//Get the file name
	CurrentPath.append( pFileName );
	
	//Check to see if we need a file extension
	if (pIsExt)
	{
		CurrentPath.append( pExtName );
	}

	return CurrentPath;
}
//================================================================================================================
/*
string CGlobal::substring(string str, int start, int end)
{
	string temp = "";

	temp = str;

	temp = temp.substr(start, end - start);

	return temp;
}
//================================================================================================================
vector<string> CGlobal::split(string str, char separator)
{
	string val = str;

	vector<string> result;

	int start = 0;
	int end = 0;

	end = val.find_first_of(separator, start);

	while (end != -1)
	{
		//add to vector
		result.push_back(val.substr(start, end - start));

		//Jump past separator
		start = end + 1;

		//Find next
		end = val.find_first_of(separator, start);

		if (end == -1)
		{
			//Last, no 2nd param required to end of string
			result.push_back(val.substr(start));
		}
	}

	return result;
}
//================================================================================================================
int CGlobal::getIndex(string str, char c)
{
	const char* s = str.c_str();

	int index = 0;

	while (*s != c)
	{
		if (*s == (char)NULL)
		{
			index = -1;
			break;
		}

		*s++;
		index++;
	}

	return index;
}
*/
//================================================================================================================
void CGlobal::ClientResize(HWND hWnd, int nWidth, int nHeight)
{
	RECT r;
	GetWindowRect(hWnd, &r);
	if (r.right != nWidth && r.bottom != nHeight)
	{
		RECT rcClient, rcWindow;

		POINT ptDiff;

		GetClientRect(hWnd, &rcClient);

		GetWindowRect(hWnd, &rcWindow);

		ptDiff.x = (rcWindow.right - rcWindow.left) - rcClient.right;

		ptDiff.y = (rcWindow.bottom - rcWindow.top) - rcClient.bottom;

		MoveWindow(hWnd,rcWindow.left, rcWindow.top, nWidth + ptDiff.x, nHeight + ptDiff.y, TRUE);
	}
}
//================================================================================================================
bool CGlobal::DoesFolderExist(string folder)
{
	struct stat status;
	stat( folder.c_str(), &status );

	if ( _access( folder.c_str(), 0 ) == 0 )
	{
		if ( status.st_mode & S_IFDIR )
		{
			return true;
		}
		else//It is a file
		{
			return false;
		}
	}
	else//Path does not exist
	{
		return false;
	}
}
//================================================================================================================
bool CGlobal::DoesFileExist(string file)
{
	struct stat status;
	stat( file.c_str(), &status );

	if ( _access( file.c_str(), 0 ) == 0 )
	{
		if ( !(status.st_mode & S_IFDIR) )
		{
			return true;
		}
		else//It is a folder
		{
			return false;
		}
	}
	else//Path does not exist
	{
		return false;
	}
}
//================================================================================================================
bool CGlobal::IsMouseOverWindow(HWND hWnd, const int mx, const int my,
                          const bool inClientSpace /*= false */)
{
    RECT windowRect;
 
    // Get the window in screen space
    ::GetWindowRect( hWnd, &windowRect );
 
    if ( inClientSpace )
    {
        POINT offset;
        offset.x = offset.y = 0;
        ClientToScreen( hWnd, &offset );
 
        // Offset the window to client space
        windowRect.left -= offset.x;
        windowRect.top -= offset.y;
        // NOTE: left and top should now be 0, 0
        windowRect.right -= offset.x;
        windowRect.bottom -= offset.y;
    }
 
    // Test if mouse over window
    POINT cursorPos = { mx, my };
    return PtInRect( &windowRect, cursorPos );
}
//================================================================================================================
vector<string> CGlobal::GetFilenames(char path_in[MAX_PATH], char sub_path[MAX_PATH])
{
    if (path_in[strlen(path_in) - 1] != '\\')
        strcat(path_in, "\\");
    
    //variables used for searching
	vector<string> filenames;
    HANDLE hFind;
    WIN32_FIND_DATA dataFind;
    bool bMoreFiles = true;
	bool p_add = true;
    
    //strings
    char findpath[250];
    char fullsrcpath[250];
    char fulldestpath[250];
    char in_temp[250];
    char out_temp[250];
    
    //tell it to find *.*
    sprintf(fullsrcpath, "%s%s*.*", path_in, sub_path);
    hFind = FindFirstFile(fullsrcpath, &dataFind);
    
    while (hFind != INVALID_HANDLE_VALUE && bMoreFiles == true)
    {
		p_add = true;

        sprintf(in_temp, "%s%s%s", path_in, sub_path, dataFind.cFileName);
        
        //Get the filename
		if (strcmp(dataFind.cFileName, ".") == 0)
			p_add = false;
		if (strcmp(dataFind.cFileName, "..") == 0)
			p_add = false;
		
		if (p_add)
		{
			filenames.push_back( dataFind.cFileName );
		}

        bMoreFiles = FindNextFile(hFind, &dataFind);
    }

	return filenames;
}
//================================================================================================================
vector<string> CGlobal::CacheNames(string filepath)
{
	char* in_path = new char[strlen(filepath.c_str()) + 1];
	strcpy_s(in_path, strlen(filepath.c_str()) + 1, filepath.c_str());
	vector<string> names = CGlobal::GetFilenames(in_path, "");

	FileParser parser;

	vector<string> p_valuesToRemove;

	//if there is a name without a .* then remove it
	for (int i = 0; i < names.size(); i++)
	{
		parser.ResetCurrentPos();

		string name = names[i];

		char* in_path = new char[strlen(name.c_str()) + 1];
		strcpy_s(in_path, strlen(name.c_str()) + 1, name.c_str());

		bool containsPeriod = false;

		int val;
		while ((val = parser.FindToken(in_path, true)) != -1)
		{
			if (val == PERIOD)
			{
				containsPeriod = true;
				break;
			}
		}

		if (!containsPeriod)
		{
			p_valuesToRemove.push_back(name);
		}
	}

	if (p_valuesToRemove.size() > 0)
	{
		vector<string> temp_names;
		vector<string>::iterator it = names.begin();
		for (; it != names.end(); it++)
		{
			for (int i = 0; i < p_valuesToRemove.size(); i++)
			{
				bool good = true;
				for (int t = 0; t < p_valuesToRemove.size(); t++)
				{
					if ((*it) == p_valuesToRemove[t])
					{
						good = false;
					}
				}

				if (good)
				{
					bool exists = false;
					for (int j = 0; j < temp_names.size(); j++)
					{
						if ((*it) == temp_names[j])
						{
							exists = true;
							break;
						}
					}
					if (!exists)
					{
						temp_names.push_back((*it));
						break;
					}
				}
				else break;
			}
		}

		if (names.size() > 0) names.clear();

		for (int i = 0; i < temp_names.size(); i++)
		{
			names.push_back(temp_names[i]);
		}
	}

	return names;
}
//================================================================================================================
void CGlobal::GetDesktopResolution(int& horizontal, int& vertical)
{
   RECT desktop;
   // Get a handle to the desktop window
   const HWND hDesktop = GetDesktopWindow();
   // Get the size of screen to the variable desktop
   GetWindowRect(hDesktop, &desktop);
   // The top left corner will have coordinates (0,0)
   // and the bottom right corner will have coordinates
   // (horizontal, vertical)
   horizontal = desktop.right;
   vertical = desktop.bottom;
}
//================================================================================================================
string CGlobal::GrabUserName()
{
	TCHAR name [ UNLEN + 1 ];
	DWORD size = UNLEN + 1;
	
	GetUserName( (TCHAR*)name, &size );
	
	string username = ZShadeSandboxGlobal::Convert::ConvertTCharToString(name);
	
	return username;
}
//================================================================================================================
string CGlobal::GetFileExt(string filename)
{
	BetterString name(filename);
	int index = name.get_index('.');
	if (index == 0) return "";
	return name.substring(index + 1, filename.size()).toString();
}
//================================================================================================================
GameDirectory2D* CGlobal::LoadGameDirectory2D(string game_folder)
{
	//Full path of game directory
	string username = CGlobal::GrabUserName();
	string path = "C:\\Users\\";
	path.append(username);
	path.append("\\AppData\\Roaming\\");
	path.append(game_folder);
	game_folder = path;
	
	//If the path does not exist something is wrong
	if (!CGlobal::DoesFolderExist(game_folder))
	{
		ZShadeMessageCenter::MsgBoxError(NULL, "Game Directory Does Not Exist!!!");
		return 0;
	}
	
	//Load the game directory
	GameDirectory2D* gameDirectory = new GameDirectory2D();
	
	gameDirectory->GAME_FOLDER = game_folder;

	string required_textures = gameDirectory->GAME_FOLDER; required_textures.append("\\RequiredTextures");
	gameDirectory->m_required_textures = required_textures;
	
	string music_folder = gameDirectory->GAME_FOLDER; music_folder.append("\\Music");
	gameDirectory->m_music_path = music_folder;
	
	string AP_folder = gameDirectory->GAME_FOLDER; AP_folder.append("\\AnimationProfiles");
	gameDirectory->m_animation_profiles_path = AP_folder;

	string AP_Sprite_folder = gameDirectory->GAME_FOLDER; AP_Sprite_folder.append("\\AnimationProfiles\\SPRITE");
	gameDirectory->m_animation_profile_sprites_path = AP_Sprite_folder;
	
	string AP_Tile_folder = gameDirectory->GAME_FOLDER; AP_Tile_folder.append("\\AnimationProfiles\\TILE");
	gameDirectory->m_animation_profile_tiles_path = AP_Tile_folder;
	
	string scripts_folder = gameDirectory->GAME_FOLDER; scripts_folder.append("\\Scripts");
	gameDirectory->m_scripts_path = scripts_folder;
	
	string sprites_folder = gameDirectory->GAME_FOLDER; sprites_folder.append("\\Sprites");
	gameDirectory->m_sprites_path = sprites_folder;
	
	string menu_sprites_folder = gameDirectory->GAME_FOLDER; menu_sprites_folder.append("\\MenuSprites");
	gameDirectory->m_menu_sprites_path = menu_sprites_folder;

	string hud_sprites_folder = gameDirectory->GAME_FOLDER; hud_sprites_folder.append("\\HUD");
	gameDirectory->m_hud_path = hud_sprites_folder;

	string tiles_folder = gameDirectory->GAME_FOLDER; tiles_folder.append("\\Tiles");
	gameDirectory->m_tiles_path = tiles_folder;

	string xml_folder = gameDirectory->GAME_FOLDER; xml_folder.append("\\XML");
	gameDirectory->m_xml_path = xml_folder;

	string xml_maps_folder = gameDirectory->GAME_FOLDER; xml_maps_folder.append("\\XML\\Maps");
	gameDirectory->m_xml_maps_path = xml_maps_folder;
	
	string xml_sprites_folder = gameDirectory->GAME_FOLDER; xml_sprites_folder.append("\\XML\\Sprites");
	gameDirectory->m_xml_sprites_path = xml_sprites_folder;
	
	string xml_main_menu_folder = gameDirectory->GAME_FOLDER; xml_main_menu_folder.append("\\XML\\MainMenu.xml");
	gameDirectory->m_xml_main_menu_path = xml_main_menu_folder;
	
	string xml_ingame_menu_folder = gameDirectory->GAME_FOLDER; xml_ingame_menu_folder.append("\\XML\\InGameMenu.xml");
	gameDirectory->m_xml_ingame_menu_path = xml_ingame_menu_folder;
	
	string xml_inventory_folder = gameDirectory->GAME_FOLDER; xml_inventory_folder.append("\\XML\\Inventory.xml");
	gameDirectory->m_xml_inventory_path = xml_inventory_folder;

	string xml_hud_folder = gameDirectory->GAME_FOLDER; xml_hud_folder.append("\\XML\\HUD.xml");
	gameDirectory->m_xml_hud_path = xml_hud_folder;

	string xml_worlds_folder = gameDirectory->GAME_FOLDER; xml_worlds_folder.append("\\XML\\Worlds");
	gameDirectory->m_xml_worlds_path = xml_worlds_folder;

	string xml_game_ini_file = gameDirectory->GAME_FOLDER; xml_game_ini_file.append("\\Game.ini");
	gameDirectory->m_game_ini_path = xml_game_ini_file;

	//If the path does not exist then create it
	//if (!CGlobal::DoesFolderExist(gameDirectory->GAME_FOLDER))
	//{
	//	CreateGameDirectory cd(gameDirectory, m_engineOptions->m_GameType2D);
	//}
	
	return gameDirectory;
}
//================================================================================================================
GameDirectory2D* CGlobal::CreateGD2D(string gameFolder, GameType2D gameType2D)
{
	// Load the game directory
	GameDirectory2D* gameDirectory = new GameDirectory2D();

	gameDirectory->GAME_FOLDER = gameFolder;

	string required_textures = gameDirectory->GAME_FOLDER; required_textures.append("\\RequiredTextures");
	gameDirectory->m_required_textures = required_textures;

	string music_folder = gameDirectory->GAME_FOLDER; music_folder.append("\\Music");
	gameDirectory->m_music_path = music_folder;

	string AP_folder = gameDirectory->GAME_FOLDER; AP_folder.append("\\AnimationProfiles");
	gameDirectory->m_animation_profiles_path = AP_folder;

	string AP_Sprite_folder = gameDirectory->GAME_FOLDER; AP_Sprite_folder.append("\\AnimationProfiles\\SPRITE");
	gameDirectory->m_animation_profile_sprites_path = AP_Sprite_folder;

	string AP_Tile_folder = gameDirectory->GAME_FOLDER; AP_Tile_folder.append("\\AnimationProfiles\\TILE");
	gameDirectory->m_animation_profile_tiles_path = AP_Tile_folder;

	string scripts_folder = gameDirectory->GAME_FOLDER; scripts_folder.append("\\Scripts");
	gameDirectory->m_scripts_path = scripts_folder;

	string sprites_folder = gameDirectory->GAME_FOLDER; sprites_folder.append("\\Sprites");
	gameDirectory->m_sprites_path = sprites_folder;

	string menu_sprites_folder = gameDirectory->GAME_FOLDER; menu_sprites_folder.append("\\MenuSprites");
	gameDirectory->m_menu_sprites_path = menu_sprites_folder;

	string hud_sprites_folder = gameDirectory->GAME_FOLDER; hud_sprites_folder.append("\\HUD");
	gameDirectory->m_hud_path = hud_sprites_folder;

	string tiles_folder = gameDirectory->GAME_FOLDER; tiles_folder.append("\\Tiles");
	gameDirectory->m_tiles_path = tiles_folder;

	string xml_folder = gameDirectory->GAME_FOLDER; xml_folder.append("\\XML");
	gameDirectory->m_xml_path = xml_folder;

	string xml_maps_folder = gameDirectory->GAME_FOLDER; xml_maps_folder.append("\\XML\\Maps");
	gameDirectory->m_xml_maps_path = xml_maps_folder;

	string xml_sprites_folder = gameDirectory->GAME_FOLDER; xml_sprites_folder.append("\\XML\\Sprites");
	gameDirectory->m_xml_sprites_path = xml_sprites_folder;

	string xml_main_menu_folder = gameDirectory->GAME_FOLDER; xml_main_menu_folder.append("\\XML\\MainMenu.xml");
	gameDirectory->m_xml_main_menu_path = xml_main_menu_folder;
	
	string xml_ingame_menu_folder = gameDirectory->GAME_FOLDER; xml_ingame_menu_folder.append("\\XML\\InGameMenu.xml");
	gameDirectory->m_xml_ingame_menu_path = xml_ingame_menu_folder;
	
	string xml_inventory_folder = gameDirectory->GAME_FOLDER; xml_inventory_folder.append("\\XML\\Inventory.xml");
	gameDirectory->m_xml_inventory_path = xml_inventory_folder;

	string xml_hud_folder = gameDirectory->GAME_FOLDER; xml_hud_folder.append("\\XML\\HUD.xml");
	gameDirectory->m_xml_hud_path = xml_hud_folder;

	string xml_worlds_folder = gameDirectory->GAME_FOLDER; xml_worlds_folder.append("\\XML\\Worlds");
	gameDirectory->m_xml_worlds_path = xml_worlds_folder;

	string xml_game_ini_file = gameDirectory->GAME_FOLDER; xml_game_ini_file.append("\\Game.ini");
	gameDirectory->m_game_ini_path = xml_game_ini_file;

	//If the path does not exist then create it
	if (!CGlobal::DoesFolderExist(gameDirectory->GAME_FOLDER))
	{
		CreateGameDirectory cd(gameDirectory, gameType2D);
	}

	//If the path does not exist something is wrong
	if (!CGlobal::DoesFolderExist(gameDirectory->GAME_FOLDER))
	{
		ZShadeMessageCenter::MsgBoxError(NULL, "Game Directory Does Not Exist!!!");
		return 0;
	}

	return gameDirectory;
}
//================================================================================================================
GameDirectory3D* CGlobal::LoadGameDirectory3D(string game_folder)
{
	//Full path of game directory
	string username = CGlobal::GrabUserName();
	string path = "C:\\Users\\";
	path.append(username);
	path.append("\\AppData\\Roaming\\");
	path.append(game_folder);
	game_folder = path;

	//Load the game directory
	GameDirectory3D* gameDirectory = new GameDirectory3D();

	gameDirectory->GAME_FOLDER = game_folder;

	string required_textures = gameDirectory->GAME_FOLDER; required_textures.append("\\RequiredTextures");
	gameDirectory->m_required_textures = required_textures;
	
	string material_folder = gameDirectory->GAME_FOLDER; material_folder.append("\\Materials");
	gameDirectory->m_materials_path = material_folder;
	
	string heightmaps_folder = gameDirectory->GAME_FOLDER; heightmaps_folder.append("\\Heightmaps");
	gameDirectory->m_heightmaps_path = heightmaps_folder;
	
	string template_poly_folder = gameDirectory->GAME_FOLDER; template_poly_folder.append("\\TemplatePolys");
	gameDirectory->m_template_poly_path = template_poly_folder;

	string music_folder = gameDirectory->GAME_FOLDER; music_folder.append("\\Music");
	gameDirectory->m_music_path = music_folder;

	string scripts_folder = gameDirectory->GAME_FOLDER; scripts_folder.append("\\Scripts");
	gameDirectory->m_scripts_path = scripts_folder;

	string models_folder = gameDirectory->GAME_FOLDER; models_folder.append("\\Models");
	gameDirectory->m_models_path = models_folder;

	string textures_folder = gameDirectory->GAME_FOLDER; textures_folder.append("\\Textures");
	gameDirectory->m_textures_path = textures_folder;

	string menu_sprites_folder = gameDirectory->GAME_FOLDER; menu_sprites_folder.append("\\MenuSprites");
	gameDirectory->m_menu_sprites_path = menu_sprites_folder;

	string hud_sprites_folder = gameDirectory->GAME_FOLDER; hud_sprites_folder.append("\\HUD");
	gameDirectory->m_hud_path = hud_sprites_folder;

	string xml_folder = gameDirectory->GAME_FOLDER; xml_folder.append("\\XML");
	gameDirectory->m_xml_path = xml_folder;

	string xml_maps_folder = gameDirectory->GAME_FOLDER; xml_maps_folder.append("\\XML\\Maps");
	gameDirectory->m_xml_maps_path = xml_maps_folder;

	string xml_main_menu_folder = gameDirectory->GAME_FOLDER; xml_main_menu_folder.append("\\XML\\MainMenu.xml");
	gameDirectory->m_xml_main_menu_path = xml_main_menu_folder;
	
	string xml_ingame_menu_folder = gameDirectory->GAME_FOLDER; xml_ingame_menu_folder.append("\\XML\\InGameMenu.xml");
	gameDirectory->m_xml_ingame_menu_path = xml_ingame_menu_folder;
	
	string xml_inventory_folder = gameDirectory->GAME_FOLDER; xml_inventory_folder.append("\\XML\\Inventory.xml");
	gameDirectory->m_xml_inventory_path = xml_inventory_folder;

	string xml_hud_folder = gameDirectory->GAME_FOLDER; xml_hud_folder.append("\\XML\\HUD.xml");
	gameDirectory->m_xml_hud_path = xml_hud_folder;

	string xml_game_ini_file = gameDirectory->GAME_FOLDER; xml_game_ini_file.append("\\Game.ini");
	gameDirectory->m_game_ini_path = xml_game_ini_file;

	////If the path does not exist something is wrong
	//if (!CGlobal::DoesFolderExist(game_folder)) 
	//{
	//	ZShadeMessageCenter::MsgBoxError(NULL, "Game Directory Does Not Exist!!!");
	//	return 0;
	//}

	//If the path does not exist then create it
	if (!CGlobal::DoesFolderExist(gameDirectory->GAME_FOLDER))
	{
		CreateGameDirectory cd(gameDirectory);
	}

	return gameDirectory;
}
//================================================================================================================
GameDirectory3D* CGlobal::CreateGD3D(string gameFolder)
{
	// Load the game directory
	GameDirectory3D* gameDirectory = new GameDirectory3D();

	gameDirectory->GAME_FOLDER = gameFolder;

	string required_textures = gameDirectory->GAME_FOLDER; required_textures.append("\\RequiredTextures");
	gameDirectory->m_required_textures = required_textures;

	string music_folder = gameDirectory->GAME_FOLDER; music_folder.append("\\Music");
	gameDirectory->m_music_path = music_folder;
	
	string material_folder = gameDirectory->GAME_FOLDER; material_folder.append("\\Materials");
	gameDirectory->m_materials_path = material_folder;
	
	string heightmaps_folder = gameDirectory->GAME_FOLDER; heightmaps_folder.append("\\Heightmaps");
	gameDirectory->m_heightmaps_path = heightmaps_folder;

	string template_poly_folder = gameDirectory->GAME_FOLDER; template_poly_folder.append("\\TemplatePolys");
	gameDirectory->m_template_poly_path = template_poly_folder;
	
	string scripts_folder = gameDirectory->GAME_FOLDER; scripts_folder.append("\\Scripts");
	gameDirectory->m_scripts_path = scripts_folder;

	string menu_sprites_folder = gameDirectory->GAME_FOLDER; menu_sprites_folder.append("\\MenuSprites");
	gameDirectory->m_menu_sprites_path = menu_sprites_folder;

	string hud_sprites_folder = gameDirectory->GAME_FOLDER; hud_sprites_folder.append("\\HUD");
	gameDirectory->m_hud_path = hud_sprites_folder;

	string models_folder = gameDirectory->GAME_FOLDER; models_folder.append("\\Models");
	gameDirectory->m_models_path = models_folder;

	string textures_folder = gameDirectory->GAME_FOLDER; textures_folder.append("\\Textures");
	gameDirectory->m_textures_path = textures_folder;

	string xml_folder = gameDirectory->GAME_FOLDER; xml_folder.append("\\XML");
	gameDirectory->m_xml_path = xml_folder;

	string xml_maps_folder = gameDirectory->GAME_FOLDER; xml_maps_folder.append("\\XML\\Maps");
	gameDirectory->m_xml_maps_path = xml_maps_folder;

	string xml_main_menu_folder = gameDirectory->GAME_FOLDER; xml_main_menu_folder.append("\\XML\\MainMenu.xml");
	gameDirectory->m_xml_main_menu_path = xml_main_menu_folder;
	
	string xml_ingame_menu_folder = gameDirectory->GAME_FOLDER; xml_ingame_menu_folder.append("\\XML\\InGameMenu.xml");
	gameDirectory->m_xml_ingame_menu_path = xml_ingame_menu_folder;
	
	string xml_inventory_folder = gameDirectory->GAME_FOLDER; xml_inventory_folder.append("\\XML\\Inventory.xml");
	gameDirectory->m_xml_inventory_path = xml_inventory_folder;

	string xml_hud_folder = gameDirectory->GAME_FOLDER; xml_hud_folder.append("\\XML\\HUD.xml");
	gameDirectory->m_xml_hud_path = xml_hud_folder;

	string xml_game_ini_file = gameDirectory->GAME_FOLDER; xml_game_ini_file.append("\\Game.ini");
	gameDirectory->m_game_ini_path = xml_game_ini_file;

	//If the path does not exist then create it
	if (!CGlobal::DoesFolderExist(gameDirectory->GAME_FOLDER))
	{
		CreateGameDirectory cd(gameDirectory);
	}

	//If the path does not exist something is wrong
	if (!CGlobal::DoesFolderExist(gameDirectory->GAME_FOLDER))
	{
		ZShadeMessageCenter::MsgBoxError(NULL, "Game Directory Does Not Exist!!!");
		return 0;
	}

	return gameDirectory;
}
//================================================================================================================