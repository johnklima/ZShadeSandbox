//=====================================================================================
//SpotLight.h
//
// http://www.braynzarsoft.net/index.php?p=D3D11SPOTLIGHT
//==============================================================================================================================
// History
//
// -Created on 6/16/2014  by Dustin Watson
//=====================================================================================
#ifndef __SPOTLIGHT_H
#define __SPOTLIGHT_H
//==============================================================================================================================
//==============================================================================================================================

//
// Includes
//

#include "D3D.h"
#include "Light.h"

//==============================================================================================================================
//==============================================================================================================================
namespace ZShadeSandboxLighting {
class SpotLight : public ZShadeSandboxLighting::Light
{
public:
	
	SpotLight();
	
	float& Range()						{ return fRange; }
	float& SpotAngle()					{ return fSpotAngle; }
	XMFLOAT3& Attenuation()				{ return fAttenuation; }
	float& SpotCosOuterCone()			{ return fSpotCosOuterCone; }
	float& SpotInnerConeReciprocal()	{ return fSpotInnerConeReciprocal; }
	
	float Range() const						{ return fRange; }
	float SpotAngle() const 				{ return fSpotAngle; }
	XMFLOAT3 Attenuation() const			{ return fAttenuation; }
	float SpotCosOuterCone() const			{ return fSpotCosOuterCone; }
	float SpotInnerConeReciprocal() const	{ return fSpotInnerConeReciprocal; }

	ZShadeSandboxLighting::Light* Clone();

private:

	float fRange;

	// Angle of the spotlight cone in radians
	float fSpotAngle;

	float fSpotCosOuterCone;
	float fSpotInnerConeReciprocal;

	// Simulate the apparent fall-off of the intensity of the light as the distance to the light source increases.
	// This only applies to point lights and spot lights.
	XMFLOAT3 fAttenuation;
};
}
//==============================================================================================================================
//==============================================================================================================================
#endif//__SPOTLIGHT_H