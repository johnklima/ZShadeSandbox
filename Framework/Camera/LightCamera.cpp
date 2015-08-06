#include "LightCamera.h"
#include "XMMath.h"
//==================================================================================================================================
//==================================================================================================================================
LightCamera::LightCamera()
{
	//mSceneBounds.Set(XMFLOAT3(0, 0, 0), sqrt(10 * 10 + 15 * 15));
	mSceneBounds.Set(XMFLOAT3(0, 0, 0), sqrt(256 * 256 + 256 * 256) / 2);

	mPosition = XMFLOAT3(0, 0, 0);
	mLookAt = XMFLOAT3(0, 0, 0);
}
//==================================================================================================================================
void LightCamera::SetSceneBounds(XMFLOAT3 c, float r)
{
	mSceneBounds.Set(c, r);
}
//==================================================================================================================================
void LightCamera::Update()
{
	XMVECTOR lightPos = XMVectorSet(mPosition.x, mPosition.y, mPosition.z, 0);
	XMVECTOR targetPos = XMVectorSet(mLookAt.x, mLookAt.y, mLookAt.z, 0);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);
	mLightView = XMMatrixLookAtLH(lightPos, targetPos, up);

	ZShadeSandboxMath::XMMatrix T;

	T._11 = 0.5f;
	T._22 = -0.5f;
	T._33 = 1.0f;
	T._41 = 0.5f;
	T._42 = 0.5f;
	T._44 = 1.0f;

	T.Update();

	mShadowTransform = mLightView * mLightProj * T;
}
//==================================================================================================================================
void LightCamera::SetLens()
{
	float fov = (float)PI / 2.0f;
	float aspect = 1.0f;
	//mLightProj = XMMatrixPerspectiveFovLH(fov, aspect, fNear, fFar);
	mLightProj = XMMatrixOrthographicLH(sceneWidth, sceneHeight, fNear, fFar);
}
//==================================================================================================================================
void LightCamera::Update(XMFLOAT3 lightDirection)
{
	XMFLOAT3 lightPosF;
	lightPosF.x = -2.0f * mSceneBounds.Radius() * lightDirection.x;
	lightPosF.y = -2.0f * mSceneBounds.Radius() * lightDirection.y;
	lightPosF.z = -2.0f * mSceneBounds.Radius() * lightDirection.z;
	
	XMVECTOR lightPos = ZShadeSandboxMath::ZMath::GMathFV(lightPosF);
	XMVECTOR targetPos = ZShadeSandboxMath::ZMath::GMathFV(mSceneBounds.Center());
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);
	
	mLightView = XMMatrixLookAtLH(lightPos, targetPos, up);
	
	XMVECTOR sphereCenterLS = XMVector3TransformCoord(targetPos, mLightView);
	XMFLOAT3 sphereCenter = ZShadeSandboxMath::ZMath::GMathVF(sphereCenterLS);

	float l = sphereCenter.x - mSceneBounds.Radius();
	float b = sphereCenter.y - mSceneBounds.Radius();
	float n = sphereCenter.z - mSceneBounds.Radius();
	float r = sphereCenter.x + mSceneBounds.Radius();
	float t = sphereCenter.y + mSceneBounds.Radius();
	float f = sphereCenter.z + mSceneBounds.Radius();
	
	mLightProj = XMMatrixOrthographicOffCenterLH(l, r, b, t, n, f);
	
	ZShadeSandboxMath::XMMatrix T;
	
	T._11 = 0.5f;
	T._22 = -0.5f;
	T._33 = 1.0f;
	T._41 = 0.5f;
	T._42 = 0.5f;
	T._44 = 1.0f;
	
	T.Update();
	
	mShadowTransform = mLightView * mLightProj * T;
}
//==================================================================================================================================
