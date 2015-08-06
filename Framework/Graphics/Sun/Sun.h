//===============================================================================================================================
// Sun.h
//
//===============================================================================================================================
// History
//
// -Created on 4/28/2014 by Dustin Watson
//===============================================================================================================================
#ifndef __SUN_H
#define __SUN_H
//==============================================================================================================================
//==============================================================================================================================

//
// Includes
//

#include "D3D.h"
#include "XMMath.h"
#include "Camera.h"
#include "SunQuad.h"
#include "SunParameters.h"
#include "ShaderMaterial.h"

//==============================================================================================================================
//==============================================================================================================================
namespace ZShadeSandboxLighting {
class Sun
{
public:
	
	Sun(D3D* d3d, SunParameters sp);
	~Sun();
	
	void Update(Camera* camera);
	
public:
	
	ZShadeSandboxLighting::SunQuad* GetQuad(int i);
	
	SunParameters GetParameters() { return mParameters; }
	
	void RegenSunQuad(XMFLOAT2 size);
	void RegenSunQuad(float y);

	XMFLOAT2 GetSize(int i) { return mQuads[i]->Size(); }

	XMFLOAT3 GetPosition(int i) { return mQuads[i]->Position(); }
	void UpdatePositions(int i, XMFLOAT3 pos);

	D3D* GetD3D() { return mD3DSystem; }
	
	XMFLOAT3& Position() 		{ return position; }
	XMFLOAT3 Position() const 	{ return position; }
	
	XMFLOAT3& Direction() 		{ return direction; }
	XMFLOAT3 Direction() const 	{ return direction; }
	
	XMFLOAT3& Rotation() 		{ return rotation; }
	XMFLOAT3 Rotation() const 	{ return rotation; }
	
	float Alpha() const			{ return mAlpha; }
	float GlowFactor() const	{ return mGlowFactor; }
	
private:
	
	XMFLOAT3 pos0, pos1, pos2, pos3;
	D3D* mD3DSystem;
	XMFLOAT3 position;
	XMFLOAT3 direction;
	XMFLOAT3 rotation;
	XMFLOAT3 targetPos;
	XMVECTOR targetPosV;

	float distance;
	float mAlpha;
	float mGlowFactor;
	float mLongitudeSpeed;
	float mLatitudeSpeed;
	
	SunParameters mParameters;
	
	// The four quads that represent the flare
	ZShadeSandboxLighting::SunQuad* mQuads[4];
};
}
//==============================================================================================================================
//==============================================================================================================================
#endif//__SUN_H