//==================================================================================================================================
// Forces.h
//
//==================================================================================================================================
// History
//
// -Created on 1/20/2015 by Dustin Watson
//==================================================================================================================================
#ifndef __FORCES_H
#define __FORCES_H
//==================================================================================================================================
//==================================================================================================================================
struct Forces
{
	static float GRAVITATION_EARTH;
	static float GRAVITATION_URANUS;
	static float GRAVITATION_VENUS;
	static float GRAVITATION_MARS;
	static float GRAVITATION_MERCURY;
	static float GRAVITATION_MOON;
	static float GRAVITATION_PLUTO;
	static float JUMP_SPEED_LOW;
	static float JUMP_SPEED_HIGH;
	static float ENERGYLOSS;
	static float MAX_SPEED;
	static float GROUND_FRICTION;// friction with ground - 1=totally sticky, 0=ice
	static float DT;
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__FORCES_H