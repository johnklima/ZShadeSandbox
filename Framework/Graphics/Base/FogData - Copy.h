//******************************************************************************************************
// FogData.h
//******************************************************************************************************
#ifndef __FOGDATA_H
#define __FOGDATA_H
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
//Includes
#include "D3D.h"
#include "QuadMesh.h"
#include "MaterialManager.h"
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
//Defines
#define NULL               0
#define FOW_RADIUS_MIN     3
#define FOW_RADIUS_MAX     9
#define NUM_FOW_RADII      ((FOW_RADIUS_MAX - FOW_RADIUS_MIN) + 1)
#define FOW_MAX_RADIUS_LENGTH ((FOW_RADIUS_MAX * 2) + 1)

//Define the 9 fog bits       Left Shift
#define FOW_BIT_NW (1 << 0) //00000000
#define FOW_BIT_N  (1 << 1) //00000010
#define FOW_BIT_NE (1 << 2) //00000100
#define FOW_BIT_W  (1 << 3) //00000110
#define FOW_BIT_C  (1 << 4) //00001000
#define FOW_BIT_E  (1 << 5) //00001010
#define FOW_BIT_SW (1 << 6) //00001100
#define FOW_BIT_S  (1 << 7) //00001110
#define FOW_BIT_SE (1 << 8) //00010000

// 9 Shadow fog bits
#define FOW_BIT_S_NW (1 << 0)
#define FOW_BIT_S_N  (1 << 1)
#define FOW_BIT_S_NE (1 << 2)
#define FOW_BIT_S_W  (1 << 3)
#define FOW_BIT_S_C  (1 << 4)
#define FOW_BIT_S_E  (1 << 5)
#define FOW_BIT_S_SW (1 << 6)
#define FOW_BIT_S_S  (1 << 7)
#define FOW_BIT_S_SE (1 << 8)

//Create the masks that define the bit definitions
#define fow_n_non   0

#define fow_n_all   (FOW_BIT_NW | FOW_BIT_N | FOW_BIT_NE | \
					FOW_BIT_W | FOW_BIT_C | FOW_BIT_E | \
					FOW_BIT_SW | FOW_BIT_S | FOW_BIT_SE)

//Straights
#define fow_n_EEE	(FOW_BIT_SE | FOW_BIT_E | FOW_BIT_NE)
#define fow_n_NNN	(FOW_BIT_NE | FOW_BIT_N | FOW_BIT_NW)
#define fow_n_WWW	(FOW_BIT_NW | FOW_BIT_W | FOW_BIT_SW)
#define fow_n_SSS	(FOW_BIT_SW | FOW_BIT_S | FOW_BIT_SE)
//Corners
#define fow_n_CNE	(FOW_BIT_E | FOW_BIT_NE | FOW_BIT_N | \
					 FOW_BIT_NW | FOW_BIT_C | FOW_BIT_SE)
#define fow_n_CNW	(FOW_BIT_N | FOW_BIT_NW | FOW_BIT_W | \
					 FOW_BIT_SW | FOW_BIT_C | FOW_BIT_NE)
#define fow_n_CSW	(FOW_BIT_W | FOW_BIT_SW | FOW_BIT_S | \
					 FOW_BIT_NW | FOW_BIT_C | FOW_BIT_SE)
#define fow_n_CSE	(FOW_BIT_S | FOW_BIT_SE | FOW_BIT_E | \
					 FOW_BIT_NE | FOW_BIT_C | FOW_BIT_SW)
//Joins
#define fow_n_JNE	(FOW_BIT_E | FOW_BIT_NE | FOW_BIT_N)
#define fow_n_JNW	(FOW_BIT_N | FOW_BIT_NW | FOW_BIT_W)
#define fow_n_JSW	(FOW_BIT_W | FOW_BIT_SW | FOW_BIT_S)
#define fow_n_JSE	(FOW_BIT_S | FOW_BIT_SE | FOW_BIT_E)

//
// Shadow version
//

/*
#define fow_s_all	2048
#define fow_s_EEE	2049
#define fow_s_NNN	2050
#define fow_s_WWW	2051
#define fow_s_SSS	2052
#define fow_s_CNE	2053
#define fow_s_CNW	2054
#define fow_s_CSW	2055
#define fow_s_CSE	2056
#define fow_s_JNE	2057
#define fow_s_JNW	2058
#define fow_s_JSW	2059
#define fow_s_JSE	2060
*/

#define fow_s_all	(FOW_BIT_S_NW | FOW_BIT_S_N | FOW_BIT_S_NE | \
					FOW_BIT_S_W | FOW_BIT_S_C | FOW_BIT_S_E | \
					FOW_BIT_S_SW | FOW_BIT_S_S | FOW_BIT_S_SE)
//Straights
#define fow_s_EEE	(FOW_BIT_S_SE | FOW_BIT_S_E | FOW_BIT_S_NE)
#define fow_s_NNN	(FOW_BIT_S_NE | FOW_BIT_S_N | FOW_BIT_S_NW)
#define fow_s_WWW	(FOW_BIT_S_NW | FOW_BIT_S_W | FOW_BIT_S_SW)
#define fow_s_SSS	(FOW_BIT_S_SW | FOW_BIT_S_S | FOW_BIT_S_SE)
//Corners
#define fow_s_CNE	(FOW_BIT_S_E | FOW_BIT_S_NE | FOW_BIT_S_N | \
					FOW_BIT_S_NW | FOW_BIT_S_C | FOW_BIT_S_SE)
#define fow_s_CNW	(FOW_BIT_S_N | FOW_BIT_S_NW | FOW_BIT_S_W | \
					FOW_BIT_S_SW | FOW_BIT_S_C | FOW_BIT_S_NE)
#define fow_s_CSW	(FOW_BIT_S_W | FOW_BIT_S_SW | FOW_BIT_S_S | \
					FOW_BIT_S_NW | FOW_BIT_S_C | FOW_BIT_S_SE)
#define fow_s_CSE	(FOW_BIT_S_S | FOW_BIT_S_SE | FOW_BIT_S_E | \
					FOW_BIT_S_NE | FOW_BIT_S_C | FOW_BIT_S_SW)
//Joins
#define fow_s_JNE	(FOW_BIT_S_E | FOW_BIT_S_NE | FOW_BIT_S_N)
#define fow_s_JNW	(FOW_BIT_S_N | FOW_BIT_S_NW | FOW_BIT_S_W)
#define fow_s_JSW	(FOW_BIT_S_W | FOW_BIT_S_SW | FOW_BIT_S_S)
#define fow_s_JSE	(FOW_BIT_S_S | FOW_BIT_S_SE | FOW_BIT_S_E)

#define NUM_FOW_ENTRIES fow_n_all

// The normal circle mask will ignore this bit and the shadow circle mask
// will pick up what should be there
#define fow_ignor	-1

//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
static unsigned short circle_mask[NUM_FOW_RADII][FOW_MAX_RADIUS_LENGTH * FOW_MAX_RADIUS_LENGTH] =
{
	//Radius 3
	{
		fow_n_all, fow_n_all, fow_n_CNW, fow_n_NNN, fow_n_CNE, fow_n_all, fow_n_all,
		fow_n_all, fow_n_CNW, fow_n_JNW, fow_ignor, fow_n_JNE, fow_n_CNE, fow_n_all,
		fow_n_CNW, fow_n_JNW, fow_ignor, fow_ignor, fow_ignor, fow_n_JNE, fow_n_CNE,
		fow_n_WWW, fow_ignor, fow_ignor, fow_n_non, fow_ignor, fow_ignor, fow_n_EEE,
		fow_n_CSW, fow_n_JSW, fow_ignor, fow_ignor, fow_ignor, fow_n_JSE, fow_n_CSE,
		fow_n_all, fow_n_CSW, fow_n_JSW, fow_ignor, fow_n_JSE, fow_n_CSE, fow_n_all,
		fow_n_all, fow_n_all, fow_n_CSW, fow_n_SSS, fow_n_CSE, fow_n_all, fow_n_all,
	},
	//Radius 4
	{
		fow_n_all, fow_n_all, fow_n_all, fow_n_CNW, fow_n_NNN, fow_n_CNE, fow_n_all, fow_n_all, fow_n_all,
		fow_n_all, fow_n_all, fow_n_CNW, fow_n_JNW, fow_s_all, fow_n_JNE, fow_n_CNE, fow_n_all, fow_n_all,
		fow_n_all, fow_n_CNW, fow_n_JNW, fow_s_CNW, fow_s_NNN, fow_s_CNE, fow_n_JNE, fow_n_CNE, fow_n_all,
		fow_n_CNW, fow_n_JNW, fow_s_CNW, fow_s_JNW, fow_n_non, fow_s_JNE, fow_s_CNE, fow_n_JNE, fow_n_CNE,
		fow_n_WWW, fow_s_all, fow_s_WWW, fow_n_non, fow_n_non, fow_n_non, fow_s_EEE, fow_s_all, fow_n_EEE,
		fow_n_CSW, fow_n_JSW, fow_s_CSW, fow_s_JSW, fow_n_non, fow_s_JSE, fow_s_CSE, fow_n_JSE, fow_n_CSE,
		fow_n_all, fow_n_CSW, fow_n_JSW, fow_s_CSW, fow_s_SSS, fow_s_CSE, fow_n_JSE, fow_n_CSE, fow_n_all,
		fow_n_all, fow_n_all, fow_n_CSW, fow_n_JSW, fow_s_all, fow_n_JSE, fow_n_CSE, fow_n_all, fow_n_all,
		fow_n_all, fow_n_all, fow_n_all, fow_n_CSW, fow_n_SSS, fow_n_CSE, fow_n_all, fow_n_all, fow_n_all,
	},
	//Radius 5
	{
		fow_n_all, fow_n_all, fow_n_all, fow_n_CNW, fow_n_NNN, fow_n_NNN, fow_n_NNN, fow_n_CNE, fow_n_all, fow_n_all, fow_n_all,
		fow_n_all, fow_n_all, fow_n_CNW, fow_n_JNW, fow_s_all, fow_s_all, fow_s_all, fow_n_JNE, fow_n_CNE, fow_n_all, fow_n_all,
		fow_n_all, fow_n_CNW, fow_n_JNW, fow_s_CNW, fow_s_NNN, fow_s_NNN, fow_s_NNN, fow_s_CNE, fow_n_JNE, fow_n_CNE, fow_n_all,
		fow_n_CNW, fow_n_JNW, fow_s_CNW, fow_s_JNW, fow_n_non, fow_n_non, fow_n_non, fow_s_JNE, fow_s_CNE, fow_n_JNE, fow_n_CNE,
		fow_n_WWW, fow_s_all, fow_s_WWW, fow_n_non, fow_n_non, fow_n_non, fow_n_non, fow_n_non, fow_s_EEE, fow_s_all, fow_n_EEE,
		fow_n_WWW, fow_s_all, fow_s_WWW, fow_n_non, fow_n_non, fow_n_non, fow_n_non, fow_n_non, fow_s_EEE, fow_s_all, fow_n_EEE,
		fow_n_WWW, fow_s_all, fow_s_WWW, fow_n_non, fow_n_non, fow_n_non, fow_n_non, fow_n_non, fow_s_EEE, fow_s_all, fow_n_EEE,
		fow_n_CSW, fow_n_JSW, fow_s_CSW, fow_s_JSW, fow_n_non, fow_n_non, fow_n_non, fow_s_JSE, fow_s_CSE, fow_n_JSE, fow_n_CSE,
		fow_n_all, fow_n_CSW, fow_n_JSW, fow_s_CSW, fow_s_SSS, fow_s_SSS, fow_s_SSS, fow_s_CSE, fow_n_JSE, fow_n_CSE, fow_n_all,
		fow_n_all, fow_n_all, fow_n_CSW, fow_n_JSW, fow_s_all, fow_s_all, fow_s_all, fow_n_JSE, fow_n_CSE, fow_n_all, fow_n_all,
		fow_n_all, fow_n_all, fow_n_all, fow_n_CSW, fow_n_SSS, fow_n_SSS, fow_n_SSS, fow_n_CSE, fow_n_all, fow_n_all, fow_n_all,
	},
	//Radius 6
	{
		fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_CNW,fow_n_NNN,fow_n_NNN,fow_n_NNN,fow_n_CNE,fow_n_all,fow_n_all,fow_n_all,fow_n_all,
		fow_n_all,fow_n_all,fow_n_all,fow_n_CNW,fow_n_JNW,fow_s_all,fow_s_all,fow_s_all,fow_n_JNE,fow_n_CNE,fow_n_all,fow_n_all,fow_n_all,
		fow_n_all,fow_n_all,fow_n_CNW,fow_n_JNW,fow_s_CNW,fow_s_NNN,fow_s_NNN,fow_s_NNN,fow_s_CNE,fow_n_JNE,fow_n_CNE,fow_n_all,fow_n_all,
		fow_n_all,fow_n_CNW,fow_n_JNW,fow_s_CNW,fow_s_JNW,fow_n_non,fow_n_non,fow_n_non,fow_s_JNE,fow_s_CNE,fow_n_JNE,fow_n_CNE,fow_n_all,
		fow_n_CNW,fow_n_JNW,fow_s_CNW,fow_s_JNW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_s_JNE,fow_s_CNE,fow_n_JNE,fow_n_CNE,
		fow_n_WWW,fow_s_all,fow_s_WWW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_s_EEE,fow_s_all,fow_n_EEE,
		fow_n_WWW,fow_s_all,fow_s_WWW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_s_EEE,fow_s_all,fow_n_EEE,
		fow_n_WWW,fow_s_all,fow_s_WWW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_s_EEE,fow_s_all,fow_n_EEE,
		fow_n_CSW,fow_n_JSW,fow_s_CSW,fow_s_JSW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_s_JSE,fow_s_CSE,fow_n_JSE,fow_n_CSE,
		fow_n_all,fow_n_CSW,fow_n_JSW,fow_s_CSW,fow_s_JSW,fow_n_non,fow_n_non,fow_n_non,fow_s_JSE,fow_s_CSE,fow_n_JSE,fow_n_CSE,fow_n_all,
		fow_n_all,fow_n_all,fow_n_CSW,fow_n_JSW,fow_s_CSW,fow_s_SSS,fow_s_SSS,fow_s_SSS,fow_s_CSE,fow_n_JSE,fow_n_CSE,fow_n_all,fow_n_all,
		fow_n_all,fow_n_all,fow_n_all,fow_n_CSW,fow_n_JSW,fow_s_all,fow_s_all,fow_s_all,fow_n_JSE,fow_n_CSE,fow_n_all,fow_n_all,fow_n_all,
		fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_CSW,fow_n_SSS,fow_n_SSS,fow_n_SSS,fow_n_CSE,fow_n_all,fow_n_all,fow_n_all,fow_n_all,
	},
	//Radius 7
	{
		fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_CNW,fow_n_NNN,fow_n_NNN,fow_n_NNN,fow_n_CNE,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,
		fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_CNW,fow_n_JNW,fow_s_all,fow_s_all,fow_s_all,fow_n_JNE,fow_n_CNE,fow_n_all,fow_n_all,fow_n_all,fow_n_all,
		fow_n_all,fow_n_all,fow_n_all,fow_n_CNW,fow_n_JNW,fow_s_CNW,fow_s_NNN,fow_s_NNN,fow_s_NNN,fow_s_CNE,fow_n_JNE,fow_n_CNE,fow_n_all,fow_n_all,fow_n_all,
		fow_n_all,fow_n_all,fow_n_CNW,fow_n_JNW,fow_s_CNW,fow_s_JNW,fow_n_non,fow_n_non,fow_n_non,fow_s_JNE,fow_s_CNE,fow_n_JNE,fow_n_CNE,fow_n_all,fow_n_all,
		fow_n_all,fow_n_CNW,fow_n_JNW,fow_s_CNW,fow_s_JNW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_s_JNE,fow_s_CNE,fow_n_JNE,fow_n_CNE,fow_n_all,
		fow_n_CNW,fow_n_JNW,fow_s_CNW,fow_s_JNW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_s_JNE,fow_s_CNE,fow_n_JNE,fow_n_CNE,
		fow_n_WWW,fow_s_all,fow_s_WWW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_s_EEE,fow_s_all,fow_n_EEE,
		fow_n_WWW,fow_s_all,fow_s_WWW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_s_EEE,fow_s_all,fow_n_EEE,
		fow_n_WWW,fow_s_all,fow_s_WWW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_s_EEE,fow_s_all,fow_n_EEE,
		fow_n_CSW,fow_n_JSW,fow_s_CSW,fow_s_JSW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_s_JSE,fow_s_CSE,fow_n_JSE,fow_n_CSE,
		fow_n_all,fow_n_CSW,fow_n_JSW,fow_s_CSW,fow_s_JSW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_s_JSE,fow_s_CSE,fow_n_JSE,fow_n_CSE,fow_n_all,
		fow_n_all,fow_n_all,fow_n_CSW,fow_n_JSW,fow_s_CSW,fow_s_JSW,fow_n_non,fow_n_non,fow_n_non,fow_s_JSE,fow_s_CSE,fow_n_JSE,fow_n_CSE,fow_n_all,fow_n_all,
		fow_n_all,fow_n_all,fow_n_all,fow_n_CSW,fow_n_JSW,fow_s_CSW,fow_s_SSS,fow_s_SSS,fow_s_SSS,fow_s_CSE,fow_n_JSE,fow_n_CSE,fow_n_all,fow_n_all,fow_n_all,
		fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_CSW,fow_n_JSW,fow_s_all,fow_s_all,fow_s_all,fow_n_JSE,fow_n_CSE,fow_n_all,fow_n_all,fow_n_all,fow_n_all,
		fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_CSW,fow_n_SSS,fow_n_SSS,fow_n_SSS,fow_n_CSE,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,
	},
	//Radius 8
	{
		fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_CNW,fow_n_NNN,fow_n_NNN,fow_n_NNN,fow_n_CNE,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,
		fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_CNW,fow_n_JNW,fow_s_all,fow_s_all,fow_s_all,fow_n_JNE,fow_n_CNE,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,
		fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_CNW,fow_n_JNW,fow_s_CNW,fow_s_NNN,fow_s_NNN,fow_s_NNN,fow_s_CNE,fow_n_JNE,fow_n_CNE,fow_n_all,fow_n_all,fow_n_all,fow_n_all,
		fow_n_all,fow_n_all,fow_n_all,fow_n_CNW,fow_n_JNW,fow_s_CNW,fow_s_JNW,fow_n_non,fow_n_non,fow_n_non,fow_s_JNE,fow_s_CNE,fow_n_JNE,fow_n_CNE,fow_n_all,fow_n_all,fow_n_all,
		fow_n_all,fow_n_all,fow_n_CNW,fow_n_JNW,fow_s_CNW,fow_s_JNW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_s_JNE,fow_s_CNE,fow_n_JNE,fow_n_CNE,fow_n_all,fow_n_all,
		fow_n_all,fow_n_CNW,fow_n_JNW,fow_s_CNW,fow_s_JNW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_s_JNE,fow_s_CNE,fow_n_JNE,fow_n_CNE,fow_n_all,
		fow_n_CNW,fow_n_JNW,fow_s_CNW,fow_s_JNW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_s_JNE,fow_s_CNE,fow_n_JNE,fow_n_CNE,
		fow_n_WWW,fow_s_all,fow_s_WWW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_s_EEE,fow_s_all,fow_n_EEE,
		fow_n_WWW,fow_s_all,fow_s_WWW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_s_EEE,fow_s_all,fow_n_EEE,
		fow_n_WWW,fow_s_all,fow_s_WWW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_s_EEE,fow_s_all,fow_n_EEE,
		fow_n_CSW,fow_n_JSW,fow_s_CSW,fow_s_JSW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_s_JSE,fow_n_CSE,fow_n_JSE,fow_n_CSE,
		fow_n_all,fow_n_CSW,fow_n_JSW,fow_s_CSW,fow_s_JSW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_s_JSE,fow_s_CSE,fow_n_JSE,fow_n_CSE,fow_n_all,
		fow_n_all,fow_n_all,fow_n_CSW,fow_n_JSW,fow_s_CSW,fow_s_JSW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_s_JSE,fow_s_CSE,fow_n_JSE,fow_n_CSE,fow_n_all,fow_n_all,
		fow_n_all,fow_n_all,fow_n_all,fow_n_CSW,fow_n_JSW,fow_s_CSW,fow_s_JSW,fow_n_non,fow_n_non,fow_n_non,fow_s_JSE,fow_s_CSE,fow_n_JSE,fow_n_CSE,fow_n_all,fow_n_all,fow_n_all,
		fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_CSW,fow_n_JSW,fow_s_CSW,fow_s_SSS,fow_s_SSS,fow_s_SSS,fow_s_CSE,fow_n_JSE,fow_n_CSE,fow_n_all,fow_n_all,fow_n_all,fow_n_all,
		fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_CSW,fow_n_JSW,fow_s_all,fow_s_all,fow_s_all,fow_n_JSE,fow_n_CSE,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,
		fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_CSW,fow_n_SSS,fow_n_SSS,fow_n_SSS,fow_n_CSE,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,
	},
	//Radius 9
	{
		fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_CNW,fow_n_NNN,fow_n_NNN,fow_n_NNN,fow_n_CNE,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,
		fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_CNW,fow_n_NNN,fow_n_JNW,fow_n_non,fow_n_non,fow_n_non,fow_n_JNE,fow_n_NNN,fow_n_CNE,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,
		fow_n_all,fow_n_all,fow_n_all,fow_n_CNW,fow_n_NNN,fow_n_JNW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_JNE,fow_n_NNN,fow_n_CNE,fow_n_all,fow_n_all,fow_n_all,
		fow_n_all,fow_n_all,fow_n_CNW,fow_n_JNW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_JNE,fow_n_CNE,fow_n_all,fow_n_all,
		fow_n_all,fow_n_all,fow_n_WWW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_EEE,fow_n_all,fow_n_all,
		fow_n_all,fow_n_CNW,fow_n_JNW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_JNE,fow_n_CNE,fow_n_all,
		fow_n_all,fow_n_WWW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_EEE,fow_n_all,
		fow_n_CNW,fow_n_JNW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_JNE,fow_n_CNE,
		fow_n_WWW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_EEE,
		fow_n_WWW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_EEE,
		fow_n_WWW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_EEE,
		fow_n_CSW,fow_n_JSW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_JSE,fow_n_CSE,
		fow_n_all,fow_n_WWW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_EEE,fow_n_all,
		fow_n_all,fow_n_CSW,fow_n_JSW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_JSE,fow_n_CSE,fow_n_all,
		fow_n_all,fow_n_all,fow_n_WWW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_EEE,fow_n_all,fow_n_all,
		fow_n_all,fow_n_all,fow_n_CSW,fow_n_JSW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_JSE,fow_n_CSE,fow_n_all,fow_n_all,
		fow_n_all,fow_n_all,fow_n_all,fow_n_CSW,fow_n_SSS,fow_n_JSW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_JSE,fow_n_SSS,fow_n_CSE,fow_n_all,fow_n_all,fow_n_all,
		fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_CSW,fow_n_SSS,fow_n_JSW,fow_n_non,fow_n_non,fow_n_non,fow_n_JSE,fow_n_SSS,fow_n_CSE,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,
		fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_CSW,fow_n_SSS,fow_n_SSS,fow_n_SSS,fow_n_CSE,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,
	}
};
//-----------------------------------------------------------------------------------------------------------------
static unsigned short circle_mask_shadow[NUM_FOW_RADII][FOW_MAX_RADIUS_LENGTH * FOW_MAX_RADIUS_LENGTH] =
{
	//Radius 3
	{
		fow_n_all, fow_n_all, fow_n_CNW, fow_n_NNN, fow_n_CNE, fow_n_all, fow_n_all,
		fow_n_all, fow_n_CNW, fow_n_JNW, fow_s_all, fow_n_JNE, fow_n_CNE, fow_n_all,
		fow_n_CNW, fow_n_JNW, fow_s_CNW, fow_s_NNN, fow_s_CNE, fow_n_JNE, fow_n_CNE,
		fow_n_WWW, fow_s_all, fow_s_WWW, fow_n_non, fow_s_EEE, fow_s_all, fow_n_EEE,
		fow_n_CSW, fow_n_JSW, fow_s_CSW, fow_s_SSS, fow_s_CSE, fow_n_JSE, fow_n_CSE,
		fow_n_all, fow_n_CSW, fow_n_JSW, fow_s_all, fow_n_JSE, fow_n_CSE, fow_n_all,
		fow_n_all, fow_n_all, fow_n_CSW, fow_n_SSS, fow_n_CSE, fow_n_all, fow_n_all,
	},
	//Radius 4
	{
		fow_n_all, fow_n_all, fow_n_all, fow_n_CNW, fow_n_NNN, fow_n_CNE, fow_n_all, fow_n_all, fow_n_all,
		fow_n_all, fow_n_all, fow_n_CNW, fow_n_JNW, fow_s_all, fow_n_JNE, fow_n_CNE, fow_n_all, fow_n_all,
		fow_n_all, fow_n_CNW, fow_n_JNW, fow_s_CNW, fow_s_NNN, fow_s_CNE, fow_n_JNE, fow_n_CNE, fow_n_all,
		fow_n_CNW, fow_n_JNW, fow_s_CNW, fow_s_JNW, fow_n_non, fow_s_JNE, fow_s_CNE, fow_n_JNE, fow_n_CNE,
		fow_n_WWW, fow_s_all, fow_s_WWW, fow_n_non, fow_n_non, fow_n_non, fow_s_EEE, fow_s_all, fow_n_EEE,
		fow_n_CSW, fow_n_JSW, fow_s_CSW, fow_s_JSW, fow_n_non, fow_s_JSE, fow_s_CSE, fow_n_JSE, fow_n_CSE,
		fow_n_all, fow_n_CSW, fow_n_JSW, fow_s_CSW, fow_s_SSS, fow_s_CSE, fow_n_JSE, fow_n_CSE, fow_n_all,
		fow_n_all, fow_n_all, fow_n_CSW, fow_n_JSW, fow_s_all, fow_n_JSE, fow_n_CSE, fow_n_all, fow_n_all,
		fow_n_all, fow_n_all, fow_n_all, fow_n_CSW, fow_n_SSS, fow_n_CSE, fow_n_all, fow_n_all, fow_n_all,
	},
	//Radius 5
	{
		fow_n_all, fow_n_all, fow_n_all, fow_n_CNW, fow_n_NNN, fow_n_NNN, fow_n_NNN, fow_n_CNE, fow_n_all, fow_n_all, fow_n_all,
		fow_n_all, fow_n_all, fow_n_CNW, fow_n_JNW, fow_s_all, fow_s_all, fow_s_all, fow_n_JNE, fow_n_CNE, fow_n_all, fow_n_all,
		fow_n_all, fow_n_CNW, fow_n_JNW, fow_s_CNW, fow_s_NNN, fow_s_NNN, fow_s_NNN, fow_s_CNE, fow_n_JNE, fow_n_CNE, fow_n_all,
		fow_n_CNW, fow_n_JNW, fow_s_CNW, fow_s_JNW, fow_n_non, fow_n_non, fow_n_non, fow_s_JNE, fow_s_CNE, fow_n_JNE, fow_n_CNE,
		fow_n_WWW, fow_s_all, fow_s_WWW, fow_n_non, fow_n_non, fow_n_non, fow_n_non, fow_n_non, fow_s_EEE, fow_s_all, fow_n_EEE,
		fow_n_WWW, fow_s_all, fow_s_WWW, fow_n_non, fow_n_non, fow_n_non, fow_n_non, fow_n_non, fow_s_EEE, fow_s_all, fow_n_EEE,
		fow_n_WWW, fow_s_all, fow_s_WWW, fow_n_non, fow_n_non, fow_n_non, fow_n_non, fow_n_non, fow_s_EEE, fow_s_all, fow_n_EEE,
		fow_n_CSW, fow_n_JSW, fow_s_CSW, fow_s_JSW, fow_n_non, fow_n_non, fow_n_non, fow_s_JSE, fow_s_CSE, fow_n_JSE, fow_n_CSE,
		fow_n_all, fow_n_CSW, fow_n_JSW, fow_s_CSW, fow_s_SSS, fow_s_SSS, fow_s_SSS, fow_s_CSE, fow_n_JSE, fow_n_CSE, fow_n_all,
		fow_n_all, fow_n_all, fow_n_CSW, fow_n_JSW, fow_s_all, fow_s_all, fow_s_all, fow_n_JSE, fow_n_CSE, fow_n_all, fow_n_all,
		fow_n_all, fow_n_all, fow_n_all, fow_n_CSW, fow_n_SSS, fow_n_SSS, fow_n_SSS, fow_n_CSE, fow_n_all, fow_n_all, fow_n_all,
	},
	//Radius 6
	{
		fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_CNW,fow_n_NNN,fow_n_NNN,fow_n_NNN,fow_n_CNE,fow_n_all,fow_n_all,fow_n_all,fow_n_all,
		fow_n_all,fow_n_all,fow_n_all,fow_n_CNW,fow_n_JNW,fow_s_all,fow_s_all,fow_s_all,fow_n_JNE,fow_n_CNE,fow_n_all,fow_n_all,fow_n_all,
		fow_n_all,fow_n_all,fow_n_CNW,fow_n_JNW,fow_s_CNW,fow_s_NNN,fow_s_NNN,fow_s_NNN,fow_s_CNE,fow_n_JNE,fow_n_CNE,fow_n_all,fow_n_all,
		fow_n_all,fow_n_CNW,fow_n_JNW,fow_s_CNW,fow_s_JNW,fow_n_non,fow_n_non,fow_n_non,fow_s_JNE,fow_s_CNE,fow_n_JNE,fow_n_CNE,fow_n_all,
		fow_n_CNW,fow_n_JNW,fow_s_CNW,fow_s_JNW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_s_JNE,fow_s_CNE,fow_n_JNE,fow_n_CNE,
		fow_n_WWW,fow_s_all,fow_s_WWW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_s_EEE,fow_s_all,fow_n_EEE,
		fow_n_WWW,fow_s_all,fow_s_WWW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_s_EEE,fow_s_all,fow_n_EEE,
		fow_n_WWW,fow_s_all,fow_s_WWW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_s_EEE,fow_s_all,fow_n_EEE,
		fow_n_CSW,fow_n_JSW,fow_s_CSW,fow_s_JSW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_s_JSE,fow_s_CSE,fow_n_JSE,fow_n_CSE,
		fow_n_all,fow_n_CSW,fow_n_JSW,fow_s_CSW,fow_s_JSW,fow_n_non,fow_n_non,fow_n_non,fow_s_JSE,fow_s_CSE,fow_n_JSE,fow_n_CSE,fow_n_all,
		fow_n_all,fow_n_all,fow_n_CSW,fow_n_JSW,fow_s_CSW,fow_s_SSS,fow_s_SSS,fow_s_SSS,fow_s_CSE,fow_n_JSE,fow_n_CSE,fow_n_all,fow_n_all,
		fow_n_all,fow_n_all,fow_n_all,fow_n_CSW,fow_n_JSW,fow_s_all,fow_s_all,fow_s_all,fow_n_JSE,fow_n_CSE,fow_n_all,fow_n_all,fow_n_all,
		fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_CSW,fow_n_SSS,fow_n_SSS,fow_n_SSS,fow_n_CSE,fow_n_all,fow_n_all,fow_n_all,fow_n_all,
	},
	//Radius 7
	{
		fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_CNW,fow_n_NNN,fow_n_NNN,fow_n_NNN,fow_n_CNE,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,
		fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_CNW,fow_n_JNW,fow_s_all,fow_s_all,fow_s_all,fow_n_JNE,fow_n_CNE,fow_n_all,fow_n_all,fow_n_all,fow_n_all,
		fow_n_all,fow_n_all,fow_n_all,fow_n_CNW,fow_n_JNW,fow_s_CNW,fow_s_NNN,fow_s_NNN,fow_s_NNN,fow_s_CNE,fow_n_JNE,fow_n_CNE,fow_n_all,fow_n_all,fow_n_all,
		fow_n_all,fow_n_all,fow_n_CNW,fow_n_JNW,fow_s_CNW,fow_s_JNW,fow_n_non,fow_n_non,fow_n_non,fow_s_JNE,fow_s_CNE,fow_n_JNE,fow_n_CNE,fow_n_all,fow_n_all,
		fow_n_all,fow_n_CNW,fow_n_JNW,fow_s_CNW,fow_s_JNW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_s_JNE,fow_s_CNE,fow_n_JNE,fow_n_CNE,fow_n_all,
		fow_n_CNW,fow_n_JNW,fow_s_CNW,fow_s_JNW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_s_JNE,fow_s_CNE,fow_n_JNE,fow_n_CNE,
		fow_n_WWW,fow_s_all,fow_s_WWW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_s_EEE,fow_s_all,fow_n_EEE,
		fow_n_WWW,fow_s_all,fow_s_WWW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_s_EEE,fow_s_all,fow_n_EEE,
		fow_n_WWW,fow_s_all,fow_s_WWW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_s_EEE,fow_s_all,fow_n_EEE,
		fow_n_CSW,fow_n_JSW,fow_s_CSW,fow_s_JSW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_s_JSE,fow_s_CSE,fow_n_JSE,fow_n_CSE,
		fow_n_all,fow_n_CSW,fow_n_JSW,fow_s_CSW,fow_s_JSW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_s_JSE,fow_s_CSE,fow_n_JSE,fow_n_CSE,fow_n_all,
		fow_n_all,fow_n_all,fow_n_CSW,fow_n_JSW,fow_s_CSW,fow_s_JSW,fow_n_non,fow_n_non,fow_n_non,fow_s_JSE,fow_s_CSE,fow_n_JSE,fow_n_CSE,fow_n_all,fow_n_all,
		fow_n_all,fow_n_all,fow_n_all,fow_n_CSW,fow_n_JSW,fow_s_CSW,fow_s_SSS,fow_s_SSS,fow_s_SSS,fow_s_CSE,fow_n_JSE,fow_n_CSE,fow_n_all,fow_n_all,fow_n_all,
		fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_CSW,fow_n_JSW,fow_s_all,fow_s_all,fow_s_all,fow_n_JSE,fow_n_CSE,fow_n_all,fow_n_all,fow_n_all,fow_n_all,
		fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_CSW,fow_n_SSS,fow_n_SSS,fow_n_SSS,fow_n_CSE,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,
	},
	//Radius 8
	{
		fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_CNW,fow_n_NNN,fow_n_NNN,fow_n_NNN,fow_n_CNE,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,
		fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_CNW,fow_n_JNW,fow_s_all,fow_s_all,fow_s_all,fow_n_JNE,fow_n_CNE,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,
		fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_CNW,fow_n_JNW,fow_s_CNW,fow_s_NNN,fow_s_NNN,fow_s_NNN,fow_s_CNE,fow_n_JNE,fow_n_CNE,fow_n_all,fow_n_all,fow_n_all,fow_n_all,
		fow_n_all,fow_n_all,fow_n_all,fow_n_CNW,fow_n_JNW,fow_s_CNW,fow_s_JNW,fow_n_non,fow_n_non,fow_n_non,fow_s_JNE,fow_s_CNE,fow_n_JNE,fow_n_CNE,fow_n_all,fow_n_all,fow_n_all,
		fow_n_all,fow_n_all,fow_n_CNW,fow_n_JNW,fow_s_CNW,fow_s_JNW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_s_JNE,fow_s_CNE,fow_n_JNE,fow_n_CNE,fow_n_all,fow_n_all,
		fow_n_all,fow_n_CNW,fow_n_JNW,fow_s_CNW,fow_s_JNW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_s_JNE,fow_s_CNE,fow_n_JNE,fow_n_CNE,fow_n_all,
		fow_n_CNW,fow_n_JNW,fow_s_CNW,fow_s_JNW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_s_JNE,fow_s_CNE,fow_n_JNE,fow_n_CNE,
		fow_n_WWW,fow_s_all,fow_s_WWW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_s_EEE,fow_s_all,fow_n_EEE,
		fow_n_WWW,fow_s_all,fow_s_WWW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_s_EEE,fow_s_all,fow_n_EEE,
		fow_n_WWW,fow_s_all,fow_s_WWW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_s_EEE,fow_s_all,fow_n_EEE,
		fow_n_CSW,fow_n_JSW,fow_s_CSW,fow_s_JSW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_s_JSE,fow_n_CSE,fow_n_JSE,fow_n_CSE,
		fow_n_all,fow_n_CSW,fow_n_JSW,fow_s_CSW,fow_s_JSW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_s_JSE,fow_s_CSE,fow_n_JSE,fow_n_CSE,fow_n_all,
		fow_n_all,fow_n_all,fow_n_CSW,fow_n_JSW,fow_s_CSW,fow_s_JSW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_s_JSE,fow_s_CSE,fow_n_JSE,fow_n_CSE,fow_n_all,fow_n_all,
		fow_n_all,fow_n_all,fow_n_all,fow_n_CSW,fow_n_JSW,fow_s_CSW,fow_s_JSW,fow_n_non,fow_n_non,fow_n_non,fow_s_JSE,fow_s_CSE,fow_n_JSE,fow_n_CSE,fow_n_all,fow_n_all,fow_n_all,
		fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_CSW,fow_n_JSW,fow_s_CSW,fow_s_SSS,fow_s_SSS,fow_s_SSS,fow_s_CSE,fow_n_JSE,fow_n_CSE,fow_n_all,fow_n_all,fow_n_all,fow_n_all,
		fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_CSW,fow_n_JSW,fow_s_all,fow_s_all,fow_s_all,fow_n_JSE,fow_n_CSE,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,
		fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_CSW,fow_n_SSS,fow_n_SSS,fow_n_SSS,fow_n_CSE,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,
	},
	//Radius 9
	{
		fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_CNW,fow_n_NNN,fow_n_NNN,fow_n_NNN,fow_n_CNE,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,
		fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_CNW,fow_n_NNN,fow_n_JNW,fow_n_non,fow_n_non,fow_n_non,fow_n_JNE,fow_n_NNN,fow_n_CNE,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,
		fow_n_all,fow_n_all,fow_n_all,fow_n_CNW,fow_n_NNN,fow_n_JNW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_JNE,fow_n_NNN,fow_n_CNE,fow_n_all,fow_n_all,fow_n_all,
		fow_n_all,fow_n_all,fow_n_CNW,fow_n_JNW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_JNE,fow_n_CNE,fow_n_all,fow_n_all,
		fow_n_all,fow_n_all,fow_n_WWW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_EEE,fow_n_all,fow_n_all,
		fow_n_all,fow_n_CNW,fow_n_JNW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_JNE,fow_n_CNE,fow_n_all,
		fow_n_all,fow_n_WWW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_EEE,fow_n_all,
		fow_n_CNW,fow_n_JNW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_JNE,fow_n_CNE,
		fow_n_WWW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_EEE,
		fow_n_WWW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_EEE,
		fow_n_WWW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_EEE,
		fow_n_CSW,fow_n_JSW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_JSE,fow_n_CSE,
		fow_n_all,fow_n_WWW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_EEE,fow_n_all,
		fow_n_all,fow_n_CSW,fow_n_JSW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_JSE,fow_n_CSE,fow_n_all,
		fow_n_all,fow_n_all,fow_n_WWW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_EEE,fow_n_all,fow_n_all,
		fow_n_all,fow_n_all,fow_n_CSW,fow_n_JSW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_JSE,fow_n_CSE,fow_n_all,fow_n_all,
		fow_n_all,fow_n_all,fow_n_all,fow_n_CSW,fow_n_SSS,fow_n_JSW,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_non,fow_n_JSE,fow_n_SSS,fow_n_CSE,fow_n_all,fow_n_all,fow_n_all,
		fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_CSW,fow_n_SSS,fow_n_JSW,fow_n_non,fow_n_non,fow_n_non,fow_n_JSE,fow_n_SSS,fow_n_CSE,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,
		fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_CSW,fow_n_SSS,fow_n_SSS,fow_n_SSS,fow_n_CSE,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,fow_n_all,
	}
};
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
struct FowEntry
{
	signed char bits;
};

// http://bobkoon.com/how-to-implement-a-fog-of-war-part-2-smooth/
class FogData
{
	static D3D* m_D3DSystem;
	static signed char* m_fow_frame_table;
	static FogData* instance;
public:
	static ZShadeSandboxMesh::QuadMesh* fow_shadow_texture;
	static ZShadeSandboxMesh::QuadMesh* fow_shadow_CNW_texture;
	static ZShadeSandboxMesh::QuadMesh* fow_shadow_CNE_texture;
	static ZShadeSandboxMesh::QuadMesh* fow_shadow_CSW_texture;
	static ZShadeSandboxMesh::QuadMesh* fow_shadow_CSE_texture;
	static ZShadeSandboxMesh::QuadMesh* fow_shadow_JNW_texture;
	static ZShadeSandboxMesh::QuadMesh* fow_shadow_JNE_texture;
	static ZShadeSandboxMesh::QuadMesh* fow_shadow_JSW_texture;
	static ZShadeSandboxMesh::QuadMesh* fow_shadow_JSE_texture;
	static ZShadeSandboxMesh::QuadMesh* fow_shadow_NNN_texture;
	static ZShadeSandboxMesh::QuadMesh* fow_shadow_SSS_texture;
	static ZShadeSandboxMesh::QuadMesh* fow_shadow_EEE_texture;
	static ZShadeSandboxMesh::QuadMesh* fow_shadow_WWW_texture;
	
	static ZShadeSandboxMesh::QuadMesh* fow_all_texture;
	static ZShadeSandboxMesh::QuadMesh* fow_CNW_texture;
	static ZShadeSandboxMesh::QuadMesh* fow_CNE_texture;
	static ZShadeSandboxMesh::QuadMesh* fow_CSW_texture;
	static ZShadeSandboxMesh::QuadMesh* fow_CSE_texture;
	static ZShadeSandboxMesh::QuadMesh* fow_JNW_texture;
	static ZShadeSandboxMesh::QuadMesh* fow_JNE_texture;
	static ZShadeSandboxMesh::QuadMesh* fow_JSW_texture;
	static ZShadeSandboxMesh::QuadMesh* fow_JSE_texture;
	static ZShadeSandboxMesh::QuadMesh* fow_NNN_texture;
	static ZShadeSandboxMesh::QuadMesh* fow_SSS_texture;
	static ZShadeSandboxMesh::QuadMesh* fow_EEE_texture;
	static ZShadeSandboxMesh::QuadMesh* fow_WWW_texture;

	FogData(D3D* d3d);
	FogData(const FogData& other);
	~FogData();

	static FogData* NewInstance(D3D* d3d);
	static FogData* Instance();

	static signed char* Frames() { return m_fow_frame_table; }

	static void InitializeTable()
	{
		//Initialize the lookup table
		m_fow_frame_table = new signed char[NUM_FOW_ENTRIES];

		for (int i = 0; i < NUM_FOW_ENTRIES; i++)
		{
			m_fow_frame_table[i] = -1;
		}

		//                         Frame ID's
		m_fow_frame_table[0x5F]  = 0;
		m_fow_frame_table[0x137] = 1;
		m_fow_frame_table[0x1F4] = 2;
		m_fow_frame_table[0x1D9] = 3;
		m_fow_frame_table[0x7]   = 4;
		m_fow_frame_table[0x1C0] = 5;
		m_fow_frame_table[0x49]  = 6;
		m_fow_frame_table[0x124] = 7;
		m_fow_frame_table[0xB]   = 8;
		m_fow_frame_table[0x26]  = 9;
		m_fow_frame_table[0x1A0] = 10;
		m_fow_frame_table[0xC8]  = 11;
		m_fow_frame_table[0x1FF] = 12;
		m_fow_frame_table[0x0]   = 13;
	}
	
	/*static void LoadTexturesShadow(string base_path)
	{
		ZShadeSandboxMesh::MeshParameters mp1;
		mp1.pos = XMFLOAT3(0, 0, 0);
		mp1.rot = XMFLOAT3(0, 0, 0);
		mp1.scale = XMFLOAT3(1, 1, 1);
		mp1.rotationAxisX = false;
		mp1.rotationAxisY = false;
		mp1.rotationAxisZ = false;
		mp1.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
		mp1.useCustomShader = false;
		mp1.in2D = true;
		mp1.material = MaterialManager::Instance()->GetMaterial("FOW SHADOW");
		fow_shadow_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp1, m_D3DSystem->GetEngineOptions()->TILE_SIZE, m_D3DSystem->GetEngineOptions()->TILE_SIZE, true);
		
		//
		//Corner Textures
		//

		ZShadeSandboxMesh::MeshParameters mp2;
		mp2.pos = XMFLOAT3(0, 0, 0);
		mp2.rot = XMFLOAT3(0, 0, 0);
		mp2.scale = XMFLOAT3(1, 1, 1);
		mp2.rotationAxisX = false;
		mp2.rotationAxisY = false;
		mp2.rotationAxisZ = false;
		mp2.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
		mp2.useCustomShader = false;
		mp2.in2D = true;
		mp2.material = MaterialManager::Instance()->GetMaterial("FOW SHADOW CNW");
		fow_shadow_CNW_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp2, m_D3DSystem->GetEngineOptions()->TILE_SIZE, m_D3DSystem->GetEngineOptions()->TILE_SIZE, true);

		ZShadeSandboxMesh::MeshParameters mp3;
		mp3.pos = XMFLOAT3(0, 0, 0);
		mp3.rot = XMFLOAT3(0, 0, 0);
		mp3.scale = XMFLOAT3(1, 1, 1);
		mp3.rotationAxisX = false;
		mp3.rotationAxisY = false;
		mp3.rotationAxisZ = false;
		mp3.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
		mp3.useCustomShader = false;
		mp3.in2D = true;
		mp3.material = MaterialManager::Instance()->GetMaterial("FOW SHADOW CNE");
		fow_shadow_CNE_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp3, m_D3DSystem->GetEngineOptions()->TILE_SIZE, m_D3DSystem->GetEngineOptions()->TILE_SIZE, true);

		ZShadeSandboxMesh::MeshParameters mp4;
		mp4.pos = XMFLOAT3(0, 0, 0);
		mp4.rot = XMFLOAT3(0, 0, 0);
		mp4.scale = XMFLOAT3(1, 1, 1);
		mp4.rotationAxisX = false;
		mp4.rotationAxisY = false;
		mp4.rotationAxisZ = false;
		mp4.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
		mp4.useCustomShader = false;
		mp4.in2D = true;
		mp4.material = MaterialManager::Instance()->GetMaterial("FOW SHADOW CSW");
		fow_shadow_CSW_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp4, m_D3DSystem->GetEngineOptions()->TILE_SIZE, m_D3DSystem->GetEngineOptions()->TILE_SIZE, true);
		
		ZShadeSandboxMesh::MeshParameters mp5;
		mp5.pos = XMFLOAT3(0, 0, 0);
		mp5.rot = XMFLOAT3(0, 0, 0);
		mp5.scale = XMFLOAT3(1, 1, 1);
		mp5.rotationAxisX = false;
		mp5.rotationAxisY = false;
		mp5.rotationAxisZ = false;
		mp5.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
		mp5.useCustomShader = false;
		mp5.in2D = true;
		mp5.material = MaterialManager::Instance()->GetMaterial("FOW SHADOW CSE");
		fow_shadow_CSE_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp5, m_D3DSystem->GetEngineOptions()->TILE_SIZE, m_D3DSystem->GetEngineOptions()->TILE_SIZE, true);
		
		//
		//Joint Textures
		//

		ZShadeSandboxMesh::MeshParameters mp6;
		mp6.pos = XMFLOAT3(0, 0, 0);
		mp6.rot = XMFLOAT3(0, 0, 0);
		mp6.scale = XMFLOAT3(1, 1, 1);
		mp6.rotationAxisX = false;
		mp6.rotationAxisY = false;
		mp6.rotationAxisZ = false;
		mp6.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
		mp6.useCustomShader = false;
		mp6.in2D = true;
		mp6.material = MaterialManager::Instance()->GetMaterial("FOW SHADOW JNW");
		fow_shadow_JNW_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp6, m_D3DSystem->GetEngineOptions()->TILE_SIZE, m_D3DSystem->GetEngineOptions()->TILE_SIZE, true);
		
		ZShadeSandboxMesh::MeshParameters mp7;
		mp7.pos = XMFLOAT3(0, 0, 0);
		mp7.rot = XMFLOAT3(0, 0, 0);
		mp7.scale = XMFLOAT3(1, 1, 1);
		mp7.rotationAxisX = false;
		mp7.rotationAxisY = false;
		mp7.rotationAxisZ = false;
		mp7.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
		mp7.useCustomShader = false;
		mp7.in2D = true;
		mp7.material = MaterialManager::Instance()->GetMaterial("FOW SHADOW JNE");
		fow_shadow_JNE_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp7, m_D3DSystem->GetEngineOptions()->TILE_SIZE, m_D3DSystem->GetEngineOptions()->TILE_SIZE, true);
		
		ZShadeSandboxMesh::MeshParameters mp8;
		mp8.pos = XMFLOAT3(0, 0, 0);
		mp8.rot = XMFLOAT3(0, 0, 0);
		mp8.scale = XMFLOAT3(1, 1, 1);
		mp8.rotationAxisX = false;
		mp8.rotationAxisY = false;
		mp8.rotationAxisZ = false;
		mp8.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
		mp8.useCustomShader = false;
		mp8.in2D = true;
		mp8.material = MaterialManager::Instance()->GetMaterial("FOW SHADOW JSW");
		fow_shadow_JSW_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp8, m_D3DSystem->GetEngineOptions()->TILE_SIZE, m_D3DSystem->GetEngineOptions()->TILE_SIZE, true);
		
		ZShadeSandboxMesh::MeshParameters mp9;
		mp9.pos = XMFLOAT3(0, 0, 0);
		mp9.rot = XMFLOAT3(0, 0, 0);
		mp9.scale = XMFLOAT3(1, 1, 1);
		mp9.rotationAxisX = false;
		mp9.rotationAxisY = false;
		mp9.rotationAxisZ = false;
		mp9.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
		mp9.useCustomShader = false;
		mp9.in2D = true;
		mp9.material = MaterialManager::Instance()->GetMaterial("FOW SHADOW JSE");
		fow_shadow_JSE_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp9, m_D3DSystem->GetEngineOptions()->TILE_SIZE, m_D3DSystem->GetEngineOptions()->TILE_SIZE, true);
		
		//
		//Straight Textures
		//

		ZShadeSandboxMesh::MeshParameters mp10;
		mp10.pos = XMFLOAT3(0, 0, 0);
		mp10.rot = XMFLOAT3(0, 0, 0);
		mp10.scale = XMFLOAT3(1, 1, 1);
		mp10.rotationAxisX = false;
		mp10.rotationAxisY = false;
		mp10.rotationAxisZ = false;
		mp10.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
		mp10.useCustomShader = false;
		mp10.in2D = true;
		mp10.material = MaterialManager::Instance()->GetMaterial("FOW SHADOW NNN");
		fow_shadow_NNN_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp10, m_D3DSystem->GetEngineOptions()->TILE_SIZE, m_D3DSystem->GetEngineOptions()->TILE_SIZE, true);
		
		ZShadeSandboxMesh::MeshParameters mp11;
		mp11.pos = XMFLOAT3(0, 0, 0);
		mp11.rot = XMFLOAT3(0, 0, 0);
		mp11.scale = XMFLOAT3(1, 1, 1);
		mp11.rotationAxisX = false;
		mp11.rotationAxisY = false;
		mp11.rotationAxisZ = false;
		mp11.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
		mp11.useCustomShader = false;
		mp11.in2D = true;
		mp11.material = MaterialManager::Instance()->GetMaterial("FOW SHADOW SSS");
		fow_shadow_SSS_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp11, m_D3DSystem->GetEngineOptions()->TILE_SIZE, m_D3DSystem->GetEngineOptions()->TILE_SIZE, true);
		
		ZShadeSandboxMesh::MeshParameters mp12;
		mp12.pos = XMFLOAT3(0, 0, 0);
		mp12.rot = XMFLOAT3(0, 0, 0);
		mp12.scale = XMFLOAT3(1, 1, 1);
		mp12.rotationAxisX = false;
		mp12.rotationAxisY = false;
		mp12.rotationAxisZ = false;
		mp12.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
		mp12.useCustomShader = false;
		mp12.in2D = true;
		mp12.material = MaterialManager::Instance()->GetMaterial("FOW SHADOW WWW");
		fow_shadow_WWW_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp12, m_D3DSystem->GetEngineOptions()->TILE_SIZE, m_D3DSystem->GetEngineOptions()->TILE_SIZE, true);
		
		ZShadeSandboxMesh::MeshParameters mp13;
		mp13.pos = XMFLOAT3(0, 0, 0);
		mp13.rot = XMFLOAT3(0, 0, 0);
		mp13.scale = XMFLOAT3(1, 1, 1);
		mp13.rotationAxisX = false;
		mp13.rotationAxisY = false;
		mp13.rotationAxisZ = false;
		mp13.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
		mp13.useCustomShader = false;
		mp13.in2D = true;
		mp13.material = MaterialManager::Instance()->GetMaterial("FOW SHADOW EEE");
		fow_shadow_EEE_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp13, m_D3DSystem->GetEngineOptions()->TILE_SIZE, m_D3DSystem->GetEngineOptions()->TILE_SIZE, true);
	}
	
	static void LoadTextures(string base_path)
	{
		LoadTexturesShadow(base_path);

		ZShadeSandboxMesh::MeshParameters mp1;
		mp1.pos = XMFLOAT3(0, 0, 0);
		mp1.rot = XMFLOAT3(0, 0, 0);
		mp1.scale = XMFLOAT3(1, 1, 1);
		mp1.rotationAxisX = false;
		mp1.rotationAxisY = false;
		mp1.rotationAxisZ = false;
		mp1.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
		mp1.useCustomShader = false;
		mp1.in2D = true;
		mp1.material = MaterialManager::Instance()->GetMaterial("FOW ALL");
		fow_all_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp1, m_D3DSystem->GetEngineOptions()->TILE_SIZE, m_D3DSystem->GetEngineOptions()->TILE_SIZE, true);
		
		//
		//Corner Textures
		//

		ZShadeSandboxMesh::MeshParameters mp2;
		mp2.pos = XMFLOAT3(0, 0, 0);
		mp2.rot = XMFLOAT3(0, 0, 0);
		mp2.scale = XMFLOAT3(1, 1, 1);
		mp2.rotationAxisX = false;
		mp2.rotationAxisY = false;
		mp2.rotationAxisZ = false;
		mp2.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
		mp2.useCustomShader = false;
		mp2.in2D = true;
		mp2.material = MaterialManager::Instance()->GetMaterial("FOW CNW");
		fow_CNW_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp2, m_D3DSystem->GetEngineOptions()->TILE_SIZE, m_D3DSystem->GetEngineOptions()->TILE_SIZE, true);

		ZShadeSandboxMesh::MeshParameters mp3;
		mp3.pos = XMFLOAT3(0, 0, 0);
		mp3.rot = XMFLOAT3(0, 0, 0);
		mp3.scale = XMFLOAT3(1, 1, 1);
		mp3.rotationAxisX = false;
		mp3.rotationAxisY = false;
		mp3.rotationAxisZ = false;
		mp3.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
		mp3.useCustomShader = false;
		mp3.in2D = true;
		mp3.material = MaterialManager::Instance()->GetMaterial("FOW CNE");
		fow_CNE_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp3, m_D3DSystem->GetEngineOptions()->TILE_SIZE, m_D3DSystem->GetEngineOptions()->TILE_SIZE, true);

		ZShadeSandboxMesh::MeshParameters mp4;
		mp4.pos = XMFLOAT3(0, 0, 0);
		mp4.rot = XMFLOAT3(0, 0, 0);
		mp4.scale = XMFLOAT3(1, 1, 1);
		mp4.rotationAxisX = false;
		mp4.rotationAxisY = false;
		mp4.rotationAxisZ = false;
		mp4.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
		mp4.useCustomShader = false;
		mp4.in2D = true;
		mp4.material = MaterialManager::Instance()->GetMaterial("FOW CSW");
		fow_CSW_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp4, m_D3DSystem->GetEngineOptions()->TILE_SIZE, m_D3DSystem->GetEngineOptions()->TILE_SIZE, true);
		
		ZShadeSandboxMesh::MeshParameters mp5;
		mp5.pos = XMFLOAT3(0, 0, 0);
		mp5.rot = XMFLOAT3(0, 0, 0);
		mp5.scale = XMFLOAT3(1, 1, 1);
		mp5.rotationAxisX = false;
		mp5.rotationAxisY = false;
		mp5.rotationAxisZ = false;
		mp5.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
		mp5.useCustomShader = false;
		mp5.in2D = true;
		mp5.material = MaterialManager::Instance()->GetMaterial("FOW CSE");
		fow_CSE_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp5, m_D3DSystem->GetEngineOptions()->TILE_SIZE, m_D3DSystem->GetEngineOptions()->TILE_SIZE, true);
		
		//
		//Joint Textures
		//

		ZShadeSandboxMesh::MeshParameters mp6;
		mp6.pos = XMFLOAT3(0, 0, 0);
		mp6.rot = XMFLOAT3(0, 0, 0);
		mp6.scale = XMFLOAT3(1, 1, 1);
		mp6.rotationAxisX = false;
		mp6.rotationAxisY = false;
		mp6.rotationAxisZ = false;
		mp6.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
		mp6.useCustomShader = false;
		mp6.in2D = true;
		mp6.material = MaterialManager::Instance()->GetMaterial("FOW JNW");
		fow_JNW_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp6, m_D3DSystem->GetEngineOptions()->TILE_SIZE, m_D3DSystem->GetEngineOptions()->TILE_SIZE, true);
		
		ZShadeSandboxMesh::MeshParameters mp7;
		mp7.pos = XMFLOAT3(0, 0, 0);
		mp7.rot = XMFLOAT3(0, 0, 0);
		mp7.scale = XMFLOAT3(1, 1, 1);
		mp7.rotationAxisX = false;
		mp7.rotationAxisY = false;
		mp7.rotationAxisZ = false;
		mp7.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
		mp7.useCustomShader = false;
		mp7.in2D = true;
		mp7.material = MaterialManager::Instance()->GetMaterial("FOW JNE");
		fow_JNE_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp7, m_D3DSystem->GetEngineOptions()->TILE_SIZE, m_D3DSystem->GetEngineOptions()->TILE_SIZE, true);
		
		ZShadeSandboxMesh::MeshParameters mp8;
		mp8.pos = XMFLOAT3(0, 0, 0);
		mp8.rot = XMFLOAT3(0, 0, 0);
		mp8.scale = XMFLOAT3(1, 1, 1);
		mp8.rotationAxisX = false;
		mp8.rotationAxisY = false;
		mp8.rotationAxisZ = false;
		mp8.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
		mp8.useCustomShader = false;
		mp8.in2D = true;
		mp8.material = MaterialManager::Instance()->GetMaterial("FOW JSW");
		fow_JSW_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp8, m_D3DSystem->GetEngineOptions()->TILE_SIZE, m_D3DSystem->GetEngineOptions()->TILE_SIZE, true);
		
		ZShadeSandboxMesh::MeshParameters mp9;
		mp9.pos = XMFLOAT3(0, 0, 0);
		mp9.rot = XMFLOAT3(0, 0, 0);
		mp9.scale = XMFLOAT3(1, 1, 1);
		mp9.rotationAxisX = false;
		mp9.rotationAxisY = false;
		mp9.rotationAxisZ = false;
		mp9.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
		mp9.useCustomShader = false;
		mp9.in2D = true;
		mp9.material = MaterialManager::Instance()->GetMaterial("FOW JSE");
		fow_JSE_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp9, m_D3DSystem->GetEngineOptions()->TILE_SIZE, m_D3DSystem->GetEngineOptions()->TILE_SIZE, true);
		
		//
		//Straight Textures
		//

		ZShadeSandboxMesh::MeshParameters mp10;
		mp10.pos = XMFLOAT3(0, 0, 0);
		mp10.rot = XMFLOAT3(0, 0, 0);
		mp10.scale = XMFLOAT3(1, 1, 1);
		mp10.rotationAxisX = false;
		mp10.rotationAxisY = false;
		mp10.rotationAxisZ = false;
		mp10.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
		mp10.useCustomShader = false;
		mp10.in2D = true;
		mp10.material = MaterialManager::Instance()->GetMaterial("FOW NNN");
		fow_NNN_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp10, m_D3DSystem->GetEngineOptions()->TILE_SIZE, m_D3DSystem->GetEngineOptions()->TILE_SIZE, true);
		
		ZShadeSandboxMesh::MeshParameters mp11;
		mp11.pos = XMFLOAT3(0, 0, 0);
		mp11.rot = XMFLOAT3(0, 0, 0);
		mp11.scale = XMFLOAT3(1, 1, 1);
		mp11.rotationAxisX = false;
		mp11.rotationAxisY = false;
		mp11.rotationAxisZ = false;
		mp11.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
		mp11.useCustomShader = false;
		mp11.in2D = true;
		mp11.material = MaterialManager::Instance()->GetMaterial("FOW SSS");
		fow_SSS_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp11, m_D3DSystem->GetEngineOptions()->TILE_SIZE, m_D3DSystem->GetEngineOptions()->TILE_SIZE, true);
		
		ZShadeSandboxMesh::MeshParameters mp12;
		mp12.pos = XMFLOAT3(0, 0, 0);
		mp12.rot = XMFLOAT3(0, 0, 0);
		mp12.scale = XMFLOAT3(1, 1, 1);
		mp12.rotationAxisX = false;
		mp12.rotationAxisY = false;
		mp12.rotationAxisZ = false;
		mp12.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
		mp12.useCustomShader = false;
		mp12.in2D = true;
		mp12.material = MaterialManager::Instance()->GetMaterial("FOW WWW");
		fow_WWW_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp12, m_D3DSystem->GetEngineOptions()->TILE_SIZE, m_D3DSystem->GetEngineOptions()->TILE_SIZE, true);
		
		ZShadeSandboxMesh::MeshParameters mp13;
		mp13.pos = XMFLOAT3(0, 0, 0);
		mp13.rot = XMFLOAT3(0, 0, 0);
		mp13.scale = XMFLOAT3(1, 1, 1);
		mp13.rotationAxisX = false;
		mp13.rotationAxisY = false;
		mp13.rotationAxisZ = false;
		mp13.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
		mp13.useCustomShader = false;
		mp13.in2D = true;
		mp13.material = MaterialManager::Instance()->GetMaterial("FOW EEE");
		fow_EEE_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp13, m_D3DSystem->GetEngineOptions()->TILE_SIZE, m_D3DSystem->GetEngineOptions()->TILE_SIZE, true);
	}*/
};
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
#endif//__FOGDATA_H