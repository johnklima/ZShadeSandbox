//===============================================================================================================================
// SunParameters.h
//
//===============================================================================================================================
// History
//
// -Created on 4/28/2014 by Dustin Watson
//===============================================================================================================================
#ifndef __SUNPARAMETERS_H
#define __SUNPARAMETERS_H
//==============================================================================================================================
//==============================================================================================================================

//
// Includes
//

#include "D3D.h"

//==============================================================================================================================
//==============================================================================================================================
namespace ZShadeSandboxLighting {
struct SunParameters
{
	float elevation;
	float angle;
	float lightPower;
	float verticalMoveSpeed;
	float horizontalMoveSpeed;
	float glowSetting;
	float zoomSetting;
};
}
//==============================================================================================================================
//==============================================================================================================================
#endif//__SUNPARAMETERS_H