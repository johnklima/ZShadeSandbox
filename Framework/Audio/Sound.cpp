#include "Sound.h"
#include "ZShadeMessageCenter.h"
#include "Convert.h"
//================================================================================================================
Sound::Sound(GameDirectory2D* gd, HWND hwnd) :
	m_DirectSound(0),
	m_primaryBuffer(0),
	m_gd(gd),
	m_soundLoaded(false)
{
	//Initialize the direct sound interface pointer for the default sound device
	HRESULT result = DirectSoundCreate8(NULL, &m_DirectSound, NULL);

	//Set the cooperative level to priority so the format of the primary sound buffer can be modified
	result = m_DirectSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
}
//================================================================================================================
Sound::Sound(const Sound& s)
{
}
//================================================================================================================
Sound::~Sound()
{
}
//================================================================================================================
bool Sound::LoadSound(char* filename)
{
	char* path = new char[strlen(m_gd->m_music_path.c_str()) + 1];
	strcpy(path, m_gd->m_music_path.c_str());
	strcat(path, "\\");
	strcat(path, filename);

	WAVEFORMATEX  *pwfx;
	HMMIO         hmmio;
	MMCKINFO      mmckinfo;
	MMCKINFO      mmckinfoParent;

	TCHAR szName[512];

	int path_len = strlen(m_gd->m_music_path.c_str()) + 1;
	int filename_len = strlen(filename) + 1;
	int size = path_len + filename_len;

	ZShadeSandboxGlobal::Convert::ConvertCharPointerToTChar(path, szName, size);

	//Open the file
	if (WaveOpenFile(szName, &hmmio, &pwfx, &mmckinfoParent) != 0)
	{
		ZShadeMessageCenter::MsgBoxError(NULL, "LoadSound: failed to open wave file");
		return false;
	}

	//Read the file
	if (WaveStartDataRead(&hmmio, &mmckinfo, &mmckinfoParent) != 0)
	{
		ZShadeMessageCenter::MsgBoxError(NULL, "LoadSound: failed to read wave file");
		return false;
	}

	DSBUFFERDESC dsbdesc;

	//Get buffer Info
	memset( &dsbdesc, 0, sizeof(DSBUFFERDESC) );
	dsbdesc.dwSize = sizeof(DSBUFFERDESC);
	dsbdesc.dwFlags = DSBCAPS_STATIC;
	dsbdesc.dwBufferBytes = mmckinfo.cksize;
	dsbdesc.lpwfxFormat = pwfx;

	//Create the buffer
	if (FAILED(m_DirectSound->CreateSoundBuffer(&dsbdesc, &m_primaryBuffer, NULL)))
	{
		ZShadeMessageCenter::MsgBoxError(NULL, "LoadSound: failed to create wave sound buffer");
		WaveCloseReadFile(&hmmio, &pwfx);
		return false;
	}

	LPVOID lpvAudio1;
	DWORD dwBytes1;

	//Lock the buffer
	if (FAILED(m_primaryBuffer->Lock(0, 0, &lpvAudio1, &dwBytes1, NULL, NULL, DSBLOCK_ENTIREBUFFER)))
	{
		ZShadeMessageCenter::MsgBoxError(NULL, "LoadSound: failed to lock wave sound buffer");
		WaveCloseReadFile(&hmmio, &pwfx);
		return false;
	}

	UINT cbBytesRead;

	if (WaveReadFile(hmmio, dwBytes1, (BYTE*)lpvAudio1, &mmckinfo, &cbBytesRead))
	{
		ZShadeMessageCenter::MsgBoxError(NULL, "LoadSound: failed to read wave sound buffer");
		WaveCloseReadFile(&hmmio, &pwfx);
		return false;
	}

	//Unlock the buffer
	m_primaryBuffer->Unlock(lpvAudio1, dwBytes1, NULL, 0);

	//Close the file
	WaveCloseReadFile(&hmmio, &pwfx);

	m_soundLoaded = true;

	return true;
}
//================================================================================================================
void Sound::Play()
{
	if (m_soundLoaded)
	{
		m_primaryBuffer->Play(0,0,0);
	}
}
//================================================================================================================
/*
//--------------------------------------------------------------------------------------------------------------------
bool Sound::Initialize(HWND hwnd, GameDirectory2D* gd)
{
	bool result;

	//Initialize direct sound and the primary sound buffer
	result = InitializeDirectSound(hwnd);

	if (!result)
	{
		return false;
	}

	char* path = new char[strlen(gd->m_music_path.c_str()) + 1];
	strcpy(path, gd->m_music_path.c_str());
	//strcat(path, "\\sound01.wav");
	strcat(path, "\\smb_jumpsmall.wav");

	//Load a wave audio file onto a secondary buffer
	result = LoadWaveFile(path, &m_secondaryBuffer1);

	if (!result)
	{
		return false;
	}

	//Play the wave file now that is has been loaded
	result = PlayWaveFile();

	if (!result)
	{
		return false;
	}

	return true;
}
//--------------------------------------------------------------------------------------------------------------------
void Sound::Shutdown()
{
	//Release the secondary buffer
	ShutdownWaveFile(&m_secondaryBuffer1);

	//Shutdown the Direct Sound API
	ShutdownDirectSound();
}
//--------------------------------------------------------------------------------------------------------------------
bool Sound::InitializeDirectSound(HWND hwnd)
{
	HRESULT result;
	DSBUFFERDESC bufferDesc;

	WAVEFORMATEX waveFormat8000;
	WAVEFORMATEX waveFormat11025;
	WAVEFORMATEX waveFormat22050;
	WAVEFORMATEX waveFormat44100;
	WAVEFORMATEX waveFormat48000;
	WAVEFORMATEX waveFormat96000;
	WAVEFORMATEX waveFormat192000;

	//Initialize the direct sound interface pointer for the default sound device
	result = DirectSoundCreate8(NULL, &m_DirectSound, NULL);

	if (FAILED(result))
	{
		return false;
	}

	//Set the cooperative level to priority so the format of the primary sound buffer can be modified
	result = m_DirectSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);

	if (FAILED(result))
	{
		return false;
	}

	//Setup the primary buffer description
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	//====================================================================================================================
	//Get control of the primary sound buffer on the default sound device
	result = m_DirectSound->CreateSoundBuffer(&bufferDesc, &m_primaryBuffer8000, NULL);

	if (FAILED(result))
	{
		return false;
	}
	
	//Setup the format of the primary sound buffer
	waveFormat8000.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat8000.nSamplesPerSec = 8000;
	waveFormat8000.wBitsPerSample = 24;//16
	waveFormat8000.nChannels = 2;
	waveFormat8000.nBlockAlign = (waveFormat8000.wBitsPerSample / 8) * waveFormat8000.nChannels;
	waveFormat8000.nAvgBytesPerSec = waveFormat8000.nSamplesPerSec * waveFormat8000.nBlockAlign;
	waveFormat8000.cbSize = 0;

	//Set the primary buffer to be the wave format specified
	result = m_primaryBuffer8000->SetFormat(&waveFormat8000);
	
	if (FAILED(result))
	{
		return false;
	}
	//====================================================================================================================
	//Get control of the primary sound buffer on the default sound device
	result = m_DirectSound->CreateSoundBuffer(&bufferDesc, &m_primaryBuffer11025, NULL);

	if (FAILED(result))
	{
		return false;
	}
	
	//Setup the format of the primary sound buffer
	waveFormat11025.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat11025.nSamplesPerSec = 11025;
	waveFormat11025.wBitsPerSample = 24;//16
	waveFormat11025.nChannels = 2;
	waveFormat11025.nBlockAlign = (waveFormat11025.wBitsPerSample / 8) * waveFormat11025.nChannels;
	waveFormat11025.nAvgBytesPerSec = waveFormat11025.nSamplesPerSec * waveFormat11025.nBlockAlign;
	waveFormat11025.cbSize = 0;

	//Set the primary buffer to be the wave format specified
	result = m_primaryBuffer11025->SetFormat(&waveFormat11025);
	
	if (FAILED(result))
	{
		return false;
	}
	//====================================================================================================================
	//Get control of the primary sound buffer on the default sound device
	result = m_DirectSound->CreateSoundBuffer(&bufferDesc, &m_primaryBuffer22050, NULL);

	if (FAILED(result))
	{
		return false;
	}
	
	//Setup the format of the primary sound buffer
	waveFormat22050.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat22050.nSamplesPerSec = 22050;
	waveFormat22050.wBitsPerSample = 24;//16
	waveFormat22050.nChannels = 2;
	waveFormat22050.nBlockAlign = (waveFormat22050.wBitsPerSample / 8) * waveFormat22050.nChannels;
	waveFormat22050.nAvgBytesPerSec = waveFormat22050.nSamplesPerSec * waveFormat22050.nBlockAlign;
	waveFormat22050.cbSize = 0;

	//Set the primary buffer to be the wave format specified
	result = m_primaryBuffer22050->SetFormat(&waveFormat22050);
	
	if (FAILED(result))
	{
		return false;
	}
	//====================================================================================================================
	//Get control of the primary sound buffer on the default sound device
	result = m_DirectSound->CreateSoundBuffer(&bufferDesc, &m_primaryBuffer44100, NULL);

	if (FAILED(result))
	{
		return false;
	}
	
	//Setup the format of the primary sound buffer
	//In this case it is a .WAV file recorded at 44,100 samples per second in 16-bit sterio (cd audio format)
	waveFormat44100.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat44100.nSamplesPerSec = 44100;
	waveFormat44100.wBitsPerSample = 24;//16
	waveFormat44100.nChannels = 2;
	waveFormat44100.nBlockAlign = (waveFormat44100.wBitsPerSample / 8) * waveFormat44100.nChannels;
	waveFormat44100.nAvgBytesPerSec = waveFormat44100.nSamplesPerSec * waveFormat44100.nBlockAlign;
	waveFormat44100.cbSize = 0;

	//Set the primary buffer to be the wave format specified
	result = m_primaryBuffer44100->SetFormat(&waveFormat44100);
	
	if (FAILED(result))
	{
		return false;
	}
	//====================================================================================================================
	//Get control of the primary sound buffer on the default sound device
	result = m_DirectSound->CreateSoundBuffer(&bufferDesc, &m_primaryBuffer48000, NULL);

	if (FAILED(result))
	{
		return false;
	}
	
	//Setup the format of the primary sound buffer
	waveFormat48000.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat48000.nSamplesPerSec = 48000;
	waveFormat48000.wBitsPerSample = 24;//16
	waveFormat48000.nChannels = 2;
	waveFormat48000.nBlockAlign = (waveFormat48000.wBitsPerSample / 8) * waveFormat48000.nChannels;
	waveFormat48000.nAvgBytesPerSec = waveFormat48000.nSamplesPerSec * waveFormat48000.nBlockAlign;
	waveFormat48000.cbSize = 0;

	//Set the primary buffer to be the wave format specified
	result = m_primaryBuffer48000->SetFormat(&waveFormat48000);
	
	if (FAILED(result))
	{
		return false;
	}
	//====================================================================================================================
	//Get control of the primary sound buffer on the default sound device
	result = m_DirectSound->CreateSoundBuffer(&bufferDesc, &m_primaryBuffer96000, NULL);

	if (FAILED(result))
	{
		return false;
	}
	
	//Setup the format of the primary sound buffer
	waveFormat96000.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat96000.nSamplesPerSec = 96000;
	waveFormat96000.wBitsPerSample = 24;//16
	waveFormat96000.nChannels = 2;
	waveFormat96000.nBlockAlign = (waveFormat96000.wBitsPerSample / 8) * waveFormat96000.nChannels;
	waveFormat96000.nAvgBytesPerSec = waveFormat96000.nSamplesPerSec * waveFormat96000.nBlockAlign;
	waveFormat96000.cbSize = 0;

	//Set the primary buffer to be the wave format specified
	result = m_primaryBuffer96000->SetFormat(&waveFormat96000);
	
	if (FAILED(result))
	{
		return false;
	}
	//====================================================================================================================
	//Get control of the primary sound buffer on the default sound device
	result = m_DirectSound->CreateSoundBuffer(&bufferDesc, &m_primaryBuffer192000, NULL);

	if (FAILED(result))
	{
		return false;
	}
	
	//Setup the format of the primary sound buffer
	waveFormat192000.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat192000.nSamplesPerSec = 192000;
	waveFormat192000.wBitsPerSample = 24;//16
	waveFormat192000.nChannels = 2;
	waveFormat192000.nBlockAlign = (waveFormat192000.wBitsPerSample / 8) * waveFormat192000.nChannels;
	waveFormat192000.nAvgBytesPerSec = waveFormat192000.nSamplesPerSec * waveFormat192000.nBlockAlign;
	waveFormat192000.cbSize = 0;

	//Set the primary buffer to be the wave format specified
	result = m_primaryBuffer192000->SetFormat(&waveFormat192000);
	
	if (FAILED(result))
	{
		return false;
	}
	//====================================================================================================================

	return true;
}
//--------------------------------------------------------------------------------------------------------------------
void Sound::ShutdownDirectSound()
{
	//Release the primary sound buffer pointer
	if (m_primaryBuffer8000)
	{
		m_primaryBuffer8000->Release();
		m_primaryBuffer8000 = 0;
	}
	if (m_primaryBuffer11025)
	{
		m_primaryBuffer11025->Release();
		m_primaryBuffer11025 = 0;
	}
	if (m_primaryBuffer22050)
	{
		m_primaryBuffer22050->Release();
		m_primaryBuffer22050 = 0;
	}
	if (m_primaryBuffer44100)
	{
		m_primaryBuffer44100->Release();
		m_primaryBuffer44100 = 0;
	}
	if (m_primaryBuffer48000)
	{
		m_primaryBuffer48000->Release();
		m_primaryBuffer48000 = 0;
	}
	if (m_primaryBuffer96000)
	{
		m_primaryBuffer96000->Release();
		m_primaryBuffer96000 = 0;
	}
	if (m_primaryBuffer192000)
	{
		m_primaryBuffer192000->Release();
		m_primaryBuffer192000 = 0;
	}

	//Release the direct sound interface pointer
	if (m_DirectSound)
	{
		m_DirectSound->Release();
		m_DirectSound = 0;
	}
}
//--------------------------------------------------------------------------------------------------------------------
bool Sound::LoadWaveFile(char* filename, IDirectSoundBuffer8** buffer)
{
	int error;
	FILE* filePtr;
	unsigned int count;
	WaveHeaderType waveFileHeader;
	WAVEFORMATEX waveFormat;
	DSBUFFERDESC bufferDesc;
	HRESULT result;
	IDirectSoundBuffer* tempBuffer;
	unsigned char* waveData;
	unsigned char* bufferPtr;
	unsigned long bufferSize;

	//Open the wave file in binary
	error = fopen_s(&filePtr, filename, "rb");
	
	if (error != 0)
	{
		return false;
	}

	//Read the wave file header
	count = fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr);

	if (count != 1)
	{
		return false;
	}

	//Check that the chunk ID is the RIFF format
	if ((waveFileHeader.chunkId[0] != 'R' || waveFileHeader.chunkId[1] != 'I') ||
		(waveFileHeader.chunkId[2] != 'F' || waveFileHeader.chunkId[3] != 'F'))
	{
		return false;
	}

	//Check that the file format is the WAVE format
	if ((waveFileHeader.format[0] != 'W' || waveFileHeader.format[1] != 'A') ||
		(waveFileHeader.format[2] != 'V' || waveFileHeader.format[3] != 'E'))
	{
		return false;
	}

	//Check that the file format is the fmt format
	if ((waveFileHeader.subChunkId[0] != 'f' || waveFileHeader.subChunkId[1] != 'm') ||
		(waveFileHeader.subChunkId[2] != 't' || waveFileHeader.subChunkId[3] != ' '))
	{
		return false;
	}

	//Check that the audio format is WAVE_FORMAT_PCM
	if (waveFileHeader.audioFormat != WAVE_FORMAT_PCM)
	{
		return false;
	}

	//Check that the wave file was recorded in stereo format
	if (waveFileHeader.numChannels != 2)
	{
		return false;
	}

	//Check that the wave file was recorded at a sample rate of 44.1 KHz
	if (waveFileHeader.sampleRate != 44100)
	{
		return false;
	}

	//Ensure that the wave file was recorded in 16 bit format
	if (waveFileHeader.bitsPerSample != 16)
	{
		return false;
	}

	//Check for the data chunk header
	if ((waveFileHeader.dataChunkId[0] != 'd' || waveFileHeader.dataChunkId[1] != 'a') ||
		(waveFileHeader.dataChunkId[2] != 't' || waveFileHeader.dataChunkId[3] != 'a'))
	{
		return false;
	}

	//Set the wave format of secondary buffer that this wave file will be loaded onto
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	//Set the buffer description of the secondary sound buffer that the wave file will be loaded onto
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = waveFileHeader.dataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	//Create a temporary sound buffer with the specific buffer settings
	result = m_DirectSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL);

	if (FAILED(result))
	{
		return false;
	}

	//Test the buffer format against the direct sound 8 interface and create the secondary buffer
	result = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&*buffer);

	if (FAILED(result))
	{
		return false;
	}

	//Release the temporary buffer
	tempBuffer->Release();
	tempBuffer = 0;

	//Move to the beginning of the wave data which starts at the end of the data chunk header
	fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);

	//Create a temporary buffer to hold the wave file data
	waveData = new unsigned char[waveFileHeader.dataSize];

	if (!waveData)
	{
		return false;
	}

	//Read in the wave file data into the newly created buffer
	count = fread(waveData, 1, waveFileHeader.dataSize, filePtr);

	if (count != waveFileHeader.dataSize)
	{
		return false;
	}

	//Close the file once done reading
	error = fclose(filePtr);

	if (error != 0)
	{
		return false;
	}

	//Lock the secondary buffer to write wave data into it
	result = (*buffer)->Lock(0, waveFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0);

	if (FAILED(result))
	{
		return false;
	}

	//Copy the wave data into the buffer
	memcpy(bufferPtr, waveData, waveFileHeader.dataSize);

	//Unlock the secondary buffer after the data has been written to it
	result = (*buffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0);

	if (FAILED(result))
	{
		return false;
	}

	//Release the wave data since it was copied into the secondary buffer
	delete [] waveData;
	waveData = 0;

	return true;
}
//--------------------------------------------------------------------------------------------------------------------
void Sound::ShutdownWaveFile(IDirectSoundBuffer8** buffer)
{
	//Release the secondary sound buffer
	if (*buffer)
	{
		(*buffer)->Release();
		*buffer = 0;
	}
}
//--------------------------------------------------------------------------------------------------------------------
bool Sound::PlayWaveFile()
{
	HRESULT result;

	//Set position at the beginning of the sound buffer
	result = m_secondaryBuffer1->SetCurrentPosition(0);

	if (FAILED(result))
	{
		return false;
	}

	//Set volume of the buffer to 100%
	result = m_secondaryBuffer1->SetVolume(DSBVOLUME_MAX);

	if (FAILED(result))
	{
		return false;
	}

	//Play the contents of the secondary sound buffer
	result = m_secondaryBuffer1->Play(0, 0, 0);

	if (FAILED(result))
	{
		return false;
	}

	return true;
}
//--------------------------------------------------------------------------------------------------------------------
*/