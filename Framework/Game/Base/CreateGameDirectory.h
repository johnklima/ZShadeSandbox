//================================================================================================================
// CreateGameDirectory.h
//
//================================================================================================================
// History
//
// Created on 10/4/2014 by Dustin Watson
//================================================================================================================
#ifndef __CREATEGAMEDIRECTORY_H
#define __CREATEGAMEDIRECTORY_H
//================================================================================================================
//================================================================================================================
//C/C++ Includes
#include <Windows.h>
#include <CommCtrl.h>
#include <shlobj.h>
#include <direct.h>
#include "GameDirectory.h"
#include "EngineOptions.h"
//================================================================================================================
class CreateGameDirectory
{
	void CopyAllFiles(char path_in[MAX_PATH], char path_out[MAX_PATH], char sub_path[MAX_PATH]);
	
	void CopyFolder(string folderName, string newPath);

public:
	
	CreateGameDirectory(GameDirectory2D* gd, GameType2D gameType);
	CreateGameDirectory(GameDirectory3D* gd);
};
//================================================================================================================
//================================================================================================================
#endif//__CREATEGAMEDIRECTORY_H