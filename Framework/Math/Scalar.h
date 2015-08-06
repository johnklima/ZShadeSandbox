//==================================================================================================================================
// Scalar.h
//
//==================================================================================================================================
// History
//
// -Created on 1/22/2015 by Dustin Watson
//==================================================================================================================================
#ifndef __SCALAR_H
#define __SCALAR_H
//==================================================================================================================================
//==================================================================================================================================
#include "ZMath.h"
#include "CGlobal.h"
//==================================================================================================================================
namespace ZShadeSandboxMath {
class Scalar
{
public:
	
	const static uint32 kMaxRandValue = 65535;
	
	Scalar();
	
	// Return only the fractional component of n - always positive
	static float Frac(float n);
	
	// x = 1.5, range = 1
	// t = 1.5 / 1 = 1.5
	// ft = 0.5
	// return = 1*0.5 = 0.5 
	static float Wrap(float x, float range);
	
	static float EaseOut(float x);
	
	static float EaseOutVel(float x);
	
	static float RandBetween(float a, float b);
	
	static int RandBetweenInt(int a, int b);
	
	static int RandUint();
	
	static int RandInt();
	
	static float FromVector2(XMFLOAT2 v);
	static float FromVector3(XMFLOAT3 v);
	
	static uint32 MakeColor(uint32 r, uint32 g, uint32 b);
	static uint32 RedFromColor(uint32 c);
	static uint32 GreenFromColor(uint32 c);
	static uint32 BlueFromColor(uint32 c);
	
	// maps 0-infinity to 0-1
	static float InfinityCurve(float x);
	
	static float RadToDeg(float radians);
	static float DegToRad(float degrees);
	
	static float Clamp(float a, float pmin, float pmax);
	
	static float Sign(float a);
};
}
//==================================================================================================================================
//==================================================================================================================================
#endif//__SCALAR_H