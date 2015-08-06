#include "AudioSystem.h"
//================================================================================================================
AudioSystem* AudioSystem::instance = 0;
//================================================================================================================
AudioSystem::AudioSystem(GameDirectory2D* gd, HWND hwnd) :
	m_gd(gd), m_hwnd(hwnd)
{
	m_MusicSystem = new MusicSystem();
	m_MusicSystem->Initialize(hwnd, gd);
	m_SoundSystem = new SoundSystem(gd, hwnd);
}
//================================================================================================================
AudioSystem::~AudioSystem()
{
	m_MusicSystem->Shutdown();
}
//================================================================================================================
void AudioSystem::NewInstance(GameDirectory2D* gd, HWND hwnd)
{
	instance = new AudioSystem(gd, hwnd);
}
//================================================================================================================
AudioSystem* AudioSystem::Instance()
{
	return instance;
}
//================================================================================================================
bool AudioSystem::LoadMusic(string name)
{
	return m_MusicSystem->LoadMusic(name);
}
//================================================================================================================
void AudioSystem::PlayMusicSegment(string name)
{
	m_MusicSystem->PlaySegment(name);
}
//================================================================================================================
void AudioSystem::StopMusicSegment(string name)
{
	m_MusicSystem->StopSegment(name);
}
//================================================================================================================
bool AudioSystem::IsMusicSegmentPlaying(string name)
{
	return m_MusicSystem->IsSegmentPlaying(name);
}
//================================================================================================================
void AudioSystem::LoadSound(string name)
{
	m_SoundSystem->LoadSound(name);
}
//================================================================================================================
void AudioSystem::Play_Sound(string name)
{
	m_SoundSystem->Play(name);
}
//================================================================================================================