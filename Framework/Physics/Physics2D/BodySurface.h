//==================================================================================================================================
// BodySurface
//
// https://katyscode.wordpress.com/2013/01/28/2d-platform-games-part-6-creating-platforms-and-geometry-the-player-can-be-inside-ladders-ropes-and-water/
//==================================================================================================================================
// History
//
// -Created on 7/10/2015 by Dustin Watson
//==================================================================================================================================
#ifndef __BODYSURFACE_H
#define __BODYSURFACE_H
//==================================================================================================================================
//==================================================================================================================================
struct SBodySurface
{
	float fSurfaceAccXf;
	float fSurfaceAccXb;
	float fSurfaceDecX;
	float fSurfaceMaxSpeedX;
	float fSurfaceMaxSpeedY;
	
	// Y-acceleration to be applied on platforms allowing up/down movement (eg. ladders)
	// and body does not attempt to move up or down, or jump
	float fSurfaceDecY;
	
	// Y-acceleration of surface type to be applied due to the force of gravity
	float fSurfaceAccY;
	
	// The absolute Y movement speed of the body on platforms allowing up/down movement (eg. ladders)
	// when the body tries to move up or down
	float fSurfaceSpeedUp;
	float fSurfaceSpeedDown;
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__BODYSURFACE_H