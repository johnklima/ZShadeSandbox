#include "ReadAnimationProfile2D.h"
#include "CGlobal.h"
#include "GlobalTokens.h"
//#include "tokens.h"
//-----------------------------------------------------------------------------------------------------------------
ReadAnimationProfile2D::ReadAnimationProfile2D(D3D* d3d)
:	m_D3DSystem(d3d)
{
}
//-----------------------------------------------------------------------------------------------------------------
ReadAnimationProfile2D::ReadAnimationProfile2D(const ReadAnimationProfile2D& other)
{
}
//-----------------------------------------------------------------------------------------------------------------
ReadAnimationProfile2D::~ReadAnimationProfile2D()
{
}
//-----------------------------------------------------------------------------------------------------------------
bool ReadAnimationProfile2D::Read(string p_pProfileFolderName, string base_sprite_path)
{
	bool HeaderFound = false;
	
	#define ONANIMATINTYPE 0
	#define ONPROFILENAME 1
	#define ONOBJECTTYPE 2
	#define ONSEQUENCENAME 3
	#define ONFRAMES 4
	#define ONNOTHING 5
	
	int CurrentProfileState = ONNOTHING;
	
	char buffer[MAX_PATH_LENGTH];
	char copy_buffer[MAX_PATH_LENGTH];
	
	ifstream streamIN(p_pProfileFolderName.c_str(), ios::in);

	while( streamIN.peek() > -1 )
	{
		// make sure we are not reading a header
		switch( CurrentProfileState )
		{
			case ONANIMATINTYPE:
			HeaderFound = true;
			break;
			case ONPROFILENAME:
			HeaderFound = true;
			break;
			case ONOBJECTTYPE:
			HeaderFound = true;
			break;
			case ONSEQUENCENAME:
			HeaderFound = true;
			break;
			case ONFRAMES:
			HeaderFound = true;
			break;
			default: break;
		}

		// begin a newline
		streamIN.getline(buffer, MAX_PATH_LENGTH);
		
		// locate a new token
		int ID = fileParser.FindToken(buffer, false);

		// purge the copy_buffer to null
		memset(copy_buffer, '\0', sizeof(copy_buffer));

		//Look for a header
		if (!HeaderFound)
		{
			bool Done = false;
			int counter = 0;
			for( int i = 0; i < MAX_PATH_LENGTH; i++ )
			{
				copy_buffer[counter] = '\0'; // set to blank char
				switch( ID )
				{
					case CLOSEB:
						fileParser.ResetCurrentPos(); // start the token parser over
						Done = true;
						break; // we are done
					case OPENB:
						ID = -1;
						continue;
						break; // we just started
				}
				if( Done ) break;
				fileParser.SetCurrentPos(i);
				ID = fileParser.FindToken(buffer, false);
				switch( ID )
				{
					case CLOSEB:
						continue;
						break; // we are done
				}
				copy_buffer[counter++] = buffer[i];
			}

			//What kind of header was it
			if (strcmp(copy_buffer, "ANIMATIONTYPE") == 0)
				CurrentProfileState = ONANIMATINTYPE;
			if (strcmp(copy_buffer, "PROFILENAME") == 0)
				CurrentProfileState = ONPROFILENAME;
			if (strcmp(copy_buffer, "OBJECTTYPE") == 0)
				CurrentProfileState = ONOBJECTTYPE;
			if (strcmp(copy_buffer, "SEQUENCENAME") == 0)
				CurrentProfileState = ONSEQUENCENAME;
			if (strcmp(copy_buffer, "FRAMES") == 0)
				CurrentProfileState = ONFRAMES;
		}

		//Look for header data
		if (HeaderFound)
		{
			//What data to we want
			switch (CurrentProfileState)
			{
				case ONANIMATINTYPE:
				{
					if (m_pObjectAnimationType == buffer || m_pObjectAnimationType == "")
						m_pObjectAnimationType = buffer;
					else
					{
						//Error

					}

					HeaderFound = false;
					CurrentProfileState = ONNOTHING;
				}
				break;
				case ONPROFILENAME:
				{
					if (m_pProfileName == buffer || m_pProfileName == "")
						m_pProfileName = buffer;
					else
					{
						//Error

					}

					HeaderFound = false;
					CurrentProfileState = ONNOTHING;
				}
				break;
				case ONOBJECTTYPE:
				{
					if (m_pAnimationType == buffer || m_pAnimationType == "")
						m_pAnimationType = buffer;
					else
					{
						//Error

					}

					HeaderFound = false;
					CurrentProfileState = ONNOTHING;
				}
				break;
				case ONSEQUENCENAME:
				{
					m_pSequenceName = buffer;
					HeaderFound = false;
					CurrentProfileState = ONNOTHING;
				}
				break;
				case ONFRAMES:
				{
					string frame = buffer;
					BetterString str(frame);

					int index = str.get_index(' ');
					BetterString frameName = str.substring(0, index);

					BetterString frameStr = str.substring(index + 1, str.size());

					vector<string> frames = frameStr.split('-');

					//Get the name of the frame
					m_pFrameNames.push_back(frameName);

					//Load the frames rectangle
					FBOX frameBox;
					frameBox.x = atol(frames[0].c_str());
					frameBox.y = atol(frames[1].c_str());
					frameBox.w = atol(frames[2].c_str());
					frameBox.h = atol(frames[3].c_str());
					m_pFrameRectangles.insert(make_pair(frameName.toString(), frameBox));

					//Load the frames direction
					m_pDirections.insert(make_pair(frameName.toString(), atoi(frames[4].c_str())));
				}
				break;
			}
		}
	}

	streamIN.close();

	//Finally Load the sequence for the animation profile
	return LoadSequence(base_sprite_path);
}
//-----------------------------------------------------------------------------------------------------------------
bool ReadAnimationProfile2D::LoadSequence(string base_sprite_path)
{
	Animation2D* sequence = new Animation2D(m_D3DSystem);

	vector<string>::iterator frame_iter;

	int next_frame = 0;

	for (frame_iter = m_pFrameNames.begin(); frame_iter != m_pFrameNames.end(); frame_iter++)
	{
		string frameName = (*frame_iter);

		//Need to get the full path for the frame image
		string path = "";

		path.append(m_pProfileName);
		path.append("\\");
		path.append(m_pAnimationType);
		path.append("\\");
		path.append(m_pSequenceName);
		path.append("\\");
		path.append(frameName);

		string path_test = base_sprite_path;
		path_test.append("\\");
		path_test.append(path);

		//Make sure the path for the sprite exists
		if (!CGlobal::DoesFileExist(path_test))
		{
			MessageBox(NULL, "Could not locate Animation2D Frame", "Read Animation2D Profile", MB_OK);
			return false;
		}

		//Is this a tile animation
		if (strcmp(m_pObjectAnimationType.c_str(), "TILE") == 0)
			;//yes
		else
			;//no

		//Set the frames box
		FBOX box = m_pFrameRectangles[(*frame_iter)];
		//Set the frames direction
		int dir = m_pDirections[(*frame_iter)];

		sequence->AddAnimationFrame(frameName, path, base_sprite_path, box.x, box.y, box.w, box.h, dir);
	}

	m_pAnimation = sequence;

	return true;
}
//-----------------------------------------------------------------------------------------------------------------
Animation2D* ReadAnimationProfile2D::GetAnimation()
{
	return m_pAnimation;
}