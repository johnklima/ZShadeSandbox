#include "ProjectedCamera.h"
//==================================================================================================================================
//==================================================================================================================================
ProjectedCamera::ProjectedCamera()
{
	fNear = 0;
	fFar = 0;
	fFOV = 0;
	fAspect = 0;
	mPosition = XMFLOAT3(0, 0, 0);
	mLookAt = XMFLOAT3(0, 0, 0);
}
//==================================================================================================================================
void ProjectedCamera::Update(XMFLOAT3 position)
{
	mPosition = position;
	XMVECTOR positionV = XMVectorSet(mPosition.x, mPosition.y, mPosition.z, 0);
	XMVECTOR lookAtV = XMVectorSet(mLookAt.x, mLookAt.y, mLookAt.z, 0);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);
	mView = XMMatrixLookAtLH(positionV, lookAtV, up);
}
//==================================================================================================================================
void ProjectedCamera::SetLens(int width, int height, int nearPlane, int farPlane)
{
	fFOV = (float)XM_PI / 2.0f;
	fAspect = (float)width / (float)height;
	fNear = nearPlane;
	fFar = farPlane;
	mProj = XMMatrixPerspectiveFovLH(fFOV, fAspect, fNear, fFar);
	//mProj = XMMatrixOrthographicLH(width, height, fNear, fFar);
}
//==================================================================================================================================