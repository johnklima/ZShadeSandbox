//==============================================================================================================================
// CapsuleLight.h
//
//==============================================================================================================================
// History
//
// -Created on 4/22/2015  by Dustin Watson
//==============================================================================================================================
#ifndef __CAPSULELIGHT_H
#define __CAPSULELIGHT_H
//==============================================================================================================================
//==============================================================================================================================
#include "D3D.h"
#include "Light.h"
//==============================================================================================================================
//==============================================================================================================================
namespace ZShadeSandboxLighting {
class CapsuleLight : public ZShadeSandboxLighting::Light
{
public:
	
	CapsuleLight();
	
	float& Range()							{ return fRange; }
	float& LightLength()					{ return fLightLength; }
	//float& CapsuleIntensity()				{ return fCapsuleIntensity; }
	XMFLOAT3& CapsuleDirectionLength()		{ return fCapsuleDirectionLength; }
	
	float Range() const						{ return fRange; }
	float LightLength() const 				{ return fLightLength; }
	//float CapsuleIntensity() const			{ return fCapsuleIntensity; }
	XMFLOAT3 CapsuleDirectionLength() const	{ return fCapsuleDirectionLength; }
	
	ZShadeSandboxLighting::Light* Clone();
	
private:
	
	float fRange;
	float fLightLength;
	//float fCapsuleIntensity;
	XMFLOAT3 fCapsuleDirectionLength;
};
}
//==============================================================================================================================
//==============================================================================================================================
#endif//__CAPSULELIGHT_H