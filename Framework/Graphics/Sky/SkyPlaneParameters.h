//==============================================================================
// SkyPlaneParameters.h
//
//==============================================================================
// History
//
// -Created on 4/13/2015 by Dustin Watson
//==============================================================================
#ifndef __SKYPLANEPARAMETERS_H
#define __SKYPLANEPARAMETERS_H
//==============================================================================================================================
//==============================================================================================================================
#include "Texture.h"
//==============================================================================================================================
//==============================================================================================================================
struct SkyPlaneParameters
{
	float g_Scale;
	float g_Brightness;
	int g_skyPlaneResolution;
	float g_skyPlaneWidth;
	float g_skyPlaneTop;
	float g_skyPlaneBottom;
	int g_textureRepeat;
	string cloudTexturePath;
	string cloudTextureName;
	string cloudPerturbTextureName;
	TextureType preferedTT;
	
	SkyPlaneParameters()
	{
		g_Scale = 0.3f;
		g_Brightness = 0.5f;
		g_skyPlaneResolution = 50;
		g_skyPlaneWidth = 10.0f;
		g_skyPlaneTop = 0.5f;
		g_skyPlaneBottom = 0.0f;
		g_textureRepeat = 2;
		cloudTexturePath = "";
		cloudTextureName = "cloud001.dds";
		cloudPerturbTextureName = "perturb001.dds";
		preferedTT = TextureType::DDS;
	}
};
//==============================================================================================================================
//==============================================================================================================================
#endif//__SKYPLANEPARAMETERS_H