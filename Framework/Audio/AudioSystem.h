//================================================================================================================
// AudioSystem.h
//
// Encompasses all the music and sound into one accessible system
//================================================================================================================
// History
//
// Created on 10/9/2014 by Dustin Watson
//================================================================================================================
#ifndef __AUDIOSYSTEM_H
#define __AUDIOSYSTEM_H
//================================================================================================================
//================================================================================================================
//Includes
#include "SoundSystem.h"
#include "MusicSystem.h"
//================================================================================================================
//================================================================================================================
class AudioSystem
{
public:
	AudioSystem(GameDirectory2D* gd, HWND hwnd);
	~AudioSystem();
	
	// ================================================ Music
	
	bool LoadMusic(string name);
	void PlayMusicSegment(string name);
	void StopMusicSegment(string name);
	bool IsMusicSegmentPlaying(string name);
	
	// ================================================ Sound
	
	void LoadSound(string name);
	void Play_Sound(string name);
	
private:
	GameDirectory2D* m_gd;
	HWND m_hwnd;
	
	MusicSystem* m_MusicSystem;
	SoundSystem* m_SoundSystem;
	
private:
	
	static AudioSystem* instance;
	
public:
	
	static void NewInstance(GameDirectory2D* gd, HWND hwnd);
	static AudioSystem* Instance();
};
//================================================================================================================
//================================================================================================================
#endif//__AUDIOSYSTEM_H