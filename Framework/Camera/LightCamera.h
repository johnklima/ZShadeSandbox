//==================================================================================================================================
// LightCamera.h
//
// Renders a light view projection matrix based on the light source presented along with the shadow transform for the light
//==================================================================================================================================
// History
//
// -Created on 5/14/2015 by Dustin Watson
//==================================================================================================================================
#ifndef __LIGHTCAMERA_H
#define __LIGHTCAMERA_H
//==================================================================================================================================
//==================================================================================================================================

//
// Includes
//

#include "ZMath.h"
#include "D3D.h"
#include "SpherePrimitive.h"

//==================================================================================================================================
class LightCamera
{
public:
	
	LightCamera();
	void Update(XMFLOAT3 lightDirection);
	void Update();

	void SetLens();

public:
	
	void SetSceneBounds(XMFLOAT3 c, float r);

	XMFLOAT4X4 LightView4x4()		{ return ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(mLightView)); }
	XMFLOAT4X4 LightProj4x4()		{ return ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(mLightProj)); }
	XMFLOAT4X4 ShadowTransform4x4()	{ return ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(mShadowTransform)); }
	XMFLOAT4X4 LightViewProj4x4()	{ return ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(mLightView * mLightProj)); }

	XMMATRIX LightView() 			{ return mLightView; }
	XMMATRIX LightProj() 			{ return mLightProj; }
	XMMATRIX ShadowTransform() 		{ return mShadowTransform; }
	XMMATRIX LightViewProj() 		{ return mLightView * mLightProj; }

	float& Near()					{ return fNear; }
	float& Far()					{ return fFar; }

	float Near() const				{ return fNear; }
	float Far() const				{ return fFar; }

	XMFLOAT3& Position()			{ return mPosition; }
	XMFLOAT3& LookAt()				{ return mLookAt; }

	XMFLOAT3 Position() const		{ return mPosition; }
	XMFLOAT3 LookAt() const			{ return mLookAt; }

	float& SceneWidth()				{ return sceneWidth; }
	float& SceneHeight()			{ return sceneHeight; }

private:
	
	float fNear;
	float fFar;

	float sceneWidth;
	float sceneHeight;

	XMFLOAT3 mPosition;
	XMFLOAT3 mLookAt;

	XMMATRIX mLightView;
	XMMATRIX mLightProj;
	XMMATRIX mShadowTransform;
	ZShadeSandboxMath::SpherePrimitive mSceneBounds;
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__LIGHTCAMERA_H