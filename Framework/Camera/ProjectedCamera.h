//==================================================================================================================================
// ProjectedCamera.h
//
//==================================================================================================================================
// History
//
// -Created on 5/14/2015 by Dustin Watson
//==================================================================================================================================
#ifndef __PROJECTEDCAMERA_H
#define __PROJECTEDCAMERA_H
//==================================================================================================================================
//==================================================================================================================================

//
// Includes
//

#include "ZMath.h"
#include "D3D.h"

//==================================================================================================================================
class ProjectedCamera
{
public:

	ProjectedCamera();
	void Update(XMFLOAT3 position);
	void SetLens(int width, int height, int nearPlane, int farPlane);

public:

	XMFLOAT4X4 View4x4()			{ return ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(mView)); }
	XMFLOAT4X4 Proj4x4()			{ return ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(mProj)); }
	XMFLOAT4X4 ViewProj4x4()		{ return ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(mView * mProj)); }

	XMMATRIX View() 				{ return mView; }
	XMMATRIX Proj() 				{ return mProj; }
	XMMATRIX ViewProj() 			{ return mView * mProj; }

	float& Near()					{ return fNear; }
	float& Far()					{ return fFar; }
	float& FOV()					{ return fFOV; }
	float& Aspect()					{ return fAspect; }

	XMFLOAT3& Position()			{ return mPosition; }
	XMFLOAT3& LookAt()				{ return mLookAt; }

	float Near() const				{ return fNear; }
	float Far() const				{ return fFar; }
	float FOV() const				{ return fFOV; }
	float Aspect() const			{ return fAspect; }

	XMFLOAT3 Position() const		{ return mPosition; }
	XMFLOAT3 LookAt() const			{ return mLookAt; }

private:

	float fNear;
	float fFar;
	float fFOV;
	float fAspect;
	
	XMFLOAT3 mPosition;
	XMFLOAT3 mLookAt;

	XMMATRIX mView;
	XMMATRIX mProj;
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__PROJECTEDCAMERA_H