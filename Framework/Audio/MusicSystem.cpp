#include "MusicSystem.h"
#include "Convert.h"
//================================================================================================================
MusicSystem::MusicSystem()
{
}
//================================================================================================================
MusicSystem::MusicSystem(const MusicSystem& s)
{
}
//================================================================================================================
MusicSystem::~MusicSystem()
{
}
//================================================================================================================
bool MusicSystem::Initialize(HWND hwnd, GameDirectory2D* gd)
{
	bool result;

	m_gd = gd;

	//Initialize direct sound and the primary sound buffer
	result = InitializeDirectMusic(hwnd);

	if (!result)
	{
		return false;
	}
}
//================================================================================================================
void MusicSystem::Shutdown()
{
	ShutdownDirectSound();
}
//================================================================================================================
bool MusicSystem::InitializeDirectMusic(HWND hwnd)
{
	HRESULT hr;

	//First, make sure COM is initialized. This is nessecary because
	//DirectMusic objects must be created through COM. Normally, you
	//would place this in a WM_CREATE handler, or in your game's
	//start-up code. It's placed here for demonstration purposes
	CoInitialize( NULL );

	//Now that COM is initialized, we can get busy. We'll start by creating
	//the performance object via CoCreateInstance. I hope the comments
	//don't damage the readability too much...
	if (FAILED(CoCreateInstance((REFCLSID)CLSID_DirectMusicPerformance, NULL, CLSCTX_INPROC, (REFIID)IID_IDirectMusicPerformance, (LPVOID*)&performance)))
	{
		ZShadeMessageCenter::MsgBoxError(hwnd, "Failed to create the performance object");
		return false;
	}

	//Now that the performance object has been successfully created,
	//we need to initialize it
	hr = performance->Init(NULL, NULL, hwnd);

	if (FAILED(hr))
	{
		ZShadeMessageCenter::MsgBoxError(hwnd, "Failed to initialize performance object");
		return false;
	}

	//Now we need to choose a port. As stated in the article, you could
	//enumerate all available ports and let the user choose one, but it's
	//much simpler to just use the default Windows synth by passing
	//NULL as the port parameter. Sounds better, too.
	hr = performance->AddPort( NULL );

	if (FAILED(hr))
	{
		ZShadeMessageCenter::MsgBoxError(hwnd, "AddPort failed");
		return false;
	}

	//Now we'll create the loader object. This will be used to load the
	//midi file. Again, we need to use CoCreateInstance
	//and pass the appropriate ID parameters
	if (FAILED(CoCreateInstance((REFCLSID)CLSID_DirectMusicLoader, NULL, CLSCTX_INPROC, (REFIID)IID_IDirectMusicLoader, (LPVOID*)&loader)))
	{
		ZShadeMessageCenter::MsgBoxError(hwnd, "Failed to create loader object");
		return false;
	}

	//that's it for initialization. If we made it this far we
	//were successful
	return true;
}
//================================================================================================================
void MusicSystem::ShutdownDirectSound()
{
	//It's always a good idea to release DirectX objects in the
	//order you created them, so the first thing we'll do is kill
	//the segment. This is done by making a call to
	//IDirectMusicSegment::SetParam and passing GUID_Unload as the
	//parameter to set. The important parts are the first and
	//last parameters. You will usually (always) pass the
	//performance object as the last parameter, and the reference
	//ID of the segement parameter you wish to set as the first.
	//The other 3 arguments are for advanced DMusic stuff, which
	//we won't get into here. Just always use -1, 0, 0 for now.
	map<string, IDirectMusicSegment*>::iterator iter = m_segments.begin();
	for (; iter != m_segments.end(); iter++)
	{
		if (NULL != m_segments[(*iter).first])
		{
			m_segments[(*iter).first]->SetParam((REFGUID)GUID_Unload, -1, 0, 0, (LPVOID*)performance);
			m_segments[(*iter).first]->Release();
			m_segments[(*iter).first] = NULL;
		}
	}

	//Next up is the loader object.
	if (NULL != loader)
	{
		loader->Release();
		loader = NULL;
	}

	//And finally the performance. This requires a call to
	//IDirectMusicPerformance::CloseDown, which takes no
	//arguments, before calling Release
	if (NULL != performance)
	{
		performance->CloseDown();
		performance->Release();
		performance = NULL;
	}

	//And now we need to call CoUninitialize in order to shutdown COM.
	//Normally, this would go in a WM_CLOSE or WM_DESTROY handler,
	//or in your game's main shutdown function, but is here for
	//demonstration purposes.
	CoUninitialize();
}
//================================================================================================================
bool MusicSystem::LoadMusic(string name)
{
	//The DMUS_OBJECTDESC structure works like a surface desc in DDraw, or a buffer
	//desc in DSound. It is used by the loader object to identify the object it is
	//supposed to load
	DMUS_OBJECTDESC obj_desc;
	HRESULT hr;

	char directory[_MAX_PATH];

	WCHAR w_directory[_MAX_PATH];
	WCHAR w_filename[_MAX_PATH];

	strcpy(directory, m_gd->m_music_path.c_str());

	MULTI_TO_WIDE( w_directory, directory );

	hr = loader->SetSearchDirectory((REFGUID)GUID_DirectMusicAllTypes, w_directory, FALSE);

	if (FAILED(hr))
	{
		ZShadeMessageCenter::MsgBoxError(NULL, "LoadMIDI: SetSearchDirectory failed");
		return false;
	}

	//Now we need to setup the object description. First, clear the memory and set
	//the dwSize parameter (like you do with all DirectX structures).
	ZeroMemory( &obj_desc, sizeof(obj_desc) );
	obj_desc.dwSize = sizeof(obj_desc);

	//First, change the ascii filename to wide-character format
	MULTI_TO_WIDE(w_filename, ZShadeSandboxGlobal::Convert::ConvertStringToCharPointer(name));

	//The loader needs to know the class id of the object it is going to load.
	//Since we want to load a MIDI file, which will be stored in an
	//IDirectMusicSegment object, we need to pass the class id of
	//CLSID_DirectMusicSegment.
	obj_desc.guidClass = CLSID_DirectMusicSegment;

	//The loader object also needs to know the filename. We'll set this up
	//by using the function wcscpy from string.h, since we're using
	//wide-character strings.
	wcscpy( obj_desc.wszFileName, w_filename );

	//Now we also need to tell the loader which members of the object
	//description we are using, in our case the guidClass and wszFilename members
	obj_desc.dwValidData = DMUS_OBJ_CLASS | DMUS_OBJ_FILENAME;

	IDirectMusicSegment* p_seg;
	//Now that everything is set up, we call IDirectMusicLoader::GetObject
	//to load the file into the segment. We need to pass the object description,
	//the reference ID of the type of object to create (in this case a
	//DirectMusicSegment) and the address where a pointer to the object will
	//be stored.
	hr = loader->GetObject(&obj_desc, (REFIID)IID_IDirectMusicSegment, (LPVOID*)&p_seg);

	if (FAILED(hr))
	{
		ZShadeMessageCenter::MsgBoxError(NULL, "LoadMIDI: GetObject failed");
		return false;
	}

	//Next we need to tell the segment what kind of data it contains. We do this
	//with the IDirectMusicSegment::SetParam function.
	hr = p_seg->SetParam((REFGUID)GUID_StandardMIDIFile, -1, 0, 0, (LPVOID*)performance);

	if (FAILED(hr))
	{
		ZShadeMessageCenter::MsgBoxError(NULL, "LoadMIDI: SetParam (MIDI file) failed");
		return false;
	}

	//Finally, we need to tell the segment to 'download' the instruments
	//required to play this file. This is all handled internally, so all
	//you need to do is to say "Do it!".
	hr = p_seg->SetParam((REFGUID)GUID_Download, -1, 0, 0, (LPVOID*)performance);

	if (FAILED(hr))
	{
		ZShadeMessageCenter::MsgBoxError(NULL, "LoadMIDI: Failed to download instruments");
		return false;
	}

	m_segments[name] = p_seg;

	return true;
}
//================================================================================================================
void MusicSystem::PlaySegment(string name)
{
	HRESULT hr;

	hr = performance->PlaySegment(m_segments[name], 0, 0, NULL);

	if (FAILED(hr))
	{
		ZShadeMessageCenter::MsgBoxError(NULL, "PlaySegment failed");
	}
}
//================================================================================================================
void MusicSystem::StopSegment(string name)
{
	HRESULT hr;

	hr = performance->Stop(m_segments[name], NULL, 0, 0);

	if (FAILED(hr))
	{
		ZShadeMessageCenter::MsgBoxError(NULL, "StopSegment failed");
	}
}
//================================================================================================================
bool MusicSystem::IsSegmentPlaying(string name)
{
	HRESULT hr;

	hr = performance->IsPlaying(m_segments[name], NULL);

	if (hr == S_OK)
		return true;
	else
		return false;
}
//================================================================================================================
