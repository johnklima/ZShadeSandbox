#include "SoundSystem.h"
#include "BetterString.h"
#include "Convert.h"
//================================================================================================================
SoundSystem::SoundSystem(GameDirectory2D* gd, HWND hwnd) :
	m_gd(gd), m_hwnd(hwnd)
{
}
//================================================================================================================
SoundSystem::SoundSystem(const SoundSystem& s)
{
}
//================================================================================================================
SoundSystem::~SoundSystem()
{
}
//================================================================================================================
void SoundSystem::LoadSound(string name)
{
	Sound* sound = new Sound(m_gd, m_hwnd);
	sound->LoadSound(ZShadeSandboxGlobal::Convert::ConvertStringToCharPointer(name));
	m_sounds[name] = sound;
}
//================================================================================================================
void SoundSystem::Play(string name)
{
	m_sounds[name]->Play();
}
//================================================================================================================