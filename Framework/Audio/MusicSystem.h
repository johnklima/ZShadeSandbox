//================================================================================================================
// MusicSystem.h
//
//================================================================================================================
// History
//
// Created in August 2013 by Dustin Watson
// Updated on 10/9/2014 by Dustin Watson
//================================================================================================================
#ifndef __MUSICSYSTEM_H
#define __MUSICSYSTEM_H
//================================================================================================================
//================================================================================================================
//Includes
#define INITGUID
#include <Windows.h>
#include <mmsystem.h>
#include <map>
#include <string>
#include <stdio.h>
#include <dmksctrl.h>
#include <dmusicc.h>
#include <dmusici.h>
#include <dmusicf.h>
#include "GameDirectory.h"
#include "CGlobal.h"
#include "ZShadeMessageCenter.h"
using namespace std;
//================================================================================================================
//DirectMusic needs wide characters - here's a conversion macro
#define MULTI_TO_WIDE( x,y )  MultiByteToWideChar(CP_ACP,\
        MB_PRECOMPOSED,y,-1,x,_MAX_PATH);
//================================================================================================================
//================================================================================================================
class MusicSystem
{
public:
	MusicSystem();
	MusicSystem(const MusicSystem& s);
	~MusicSystem();

	bool Initialize(HWND hwnd, GameDirectory2D* gd);
	void Shutdown();

	bool LoadMusic(string name);

	void PlaySegment(string name);
	void StopSegment(string name);
	bool IsSegmentPlaying(string name);
private:
	bool InitializeDirectMusic(HWND hwnd);
	void ShutdownDirectSound();

	GameDirectory2D* m_gd;

	IDirectMusicPerformance* performance;
	//IDirectMusicSegment* segment;
	IDirectMusicLoader* loader;

	map<string, IDirectMusicSegment*> m_segments;
};
//================================================================================================================
//================================================================================================================
#endif//__MUSICSYSTEM_H