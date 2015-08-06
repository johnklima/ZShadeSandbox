#include "Sun.h"
#include <fstream>
#include "Camera.h"
#include "Vertex.h"
#include "CGlobal.h"
#include "TextureManager.h"
#include "ZMath.h"
using namespace std;
using ZShadeSandboxLighting::Sun;
using ZShadeSandboxLighting::SunParameters;
//==============================================================================================================================
//==============================================================================================================================
Sun::Sun(D3D* d3d, SunParameters sp)
:   mParameters(sp)
,   mD3DSystem(d3d)
{
	mQuads[0] = new SunQuad(d3d, XMFLOAT2(10.0f, 10.0f), 120.0f);
	mQuads[1] = new SunQuad(d3d, XMFLOAT2(12.0f, 12.0f), 12.0f);
	mQuads[2] = new SunQuad(d3d, XMFLOAT2(10.0f, 10.0f), 30.0f);
	mQuads[3] = new SunQuad(d3d, XMFLOAT2(15.0f, 15.0f), 15.0f);
	
	rotation = XMFLOAT3(ZShadeSandboxMath::ZMath::ToRadians(mParameters.elevation), ZShadeSandboxMath::ZMath::ToRadians(mParameters.angle), 0.0f);
	
	int rad = ZShadeSandboxMath::ZMath::ToRadians(360.0f);
	int x = rotation.x;
	int y = rotation.y;
	
	x %= rad;
	y %= rad;

	rotation.x = x;
	rotation.y = y;
}
//==============================================================================================================================
Sun::~Sun()
{
}
//==============================================================================================================================
void Sun::Update(Camera* camera)
{
	float lightPower = mParameters.lightPower * 0.1f;
	mAlpha = mParameters.glowSetting * 0.1f; // SunQuad alpha setting

	rotation.y += mLongitudeSpeed;
	rotation.x -= mLatitudeSpeed;
	
	XMFLOAT3 eye = camera->Position();
	
	position = eye;
	
	XMVECTOR positionV = XMVectorSet(position.x, position.y, position.z, 0);
	XMMATRIX rotationMatrix = XMMatrixRotationX(rotation.x) * XMMatrixRotationY(rotation.y);
	//XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, 0);
	targetPosV = positionV + XMVector3TransformCoord(XMVectorSet(0, 0, 1, 0), rotationMatrix);
	XMVECTOR up = XMVector3TransformCoord(XMVectorSet(0, 1, 0, 0), rotationMatrix);
	XMVECTOR dir = XMVector3Normalize(targetPosV - positionV);

	XMStoreFloat3(&targetPos, targetPosV);
	XMStoreFloat3(&direction, dir);

	mLatitudeSpeed = mParameters.verticalMoveSpeed * 0.001f;
	mLongitudeSpeed = mParameters.horizontalMoveSpeed * 0.001f;
	
	distance = mParameters.zoomSetting;
	
	pos0.x = direction.x * distance + position.x;
	pos0.y = direction.y * distance + position.y;
	pos0.z = direction.z * distance + position.z;
	
	pos1.x = direction.x * (distance / 1.4f) + position.x + camera->Direction().x * distance * 0.4f;
	pos1.y = direction.y * (distance / 1.4f) + position.y + camera->Direction().y * distance * 0.4f;
	pos1.z = direction.z * (distance / 1.4f) + position.z + camera->Direction().z * distance * 0.4f;
	
	pos2.x = direction.x * (distance / 2.0f) + position.x + camera->Direction().x * distance * 0.6f;
	pos2.y = direction.y * (distance / 2.0f) + position.y + camera->Direction().y * distance * 0.6f;
	pos2.z = direction.z * (distance / 2.0f) + position.z + camera->Direction().z * distance * 0.6f;
	
	pos3.x = direction.x * (distance / 4.0f) + position.x + camera->Direction().x * distance * 1.0f;
	pos3.y = direction.y * (distance / 4.0f) + position.y + camera->Direction().y * distance * 1.0f;
	pos3.z = direction.z * (distance / 4.0f) + position.z + camera->Direction().z * distance * 1.0f;
	
	ZShadeSandboxMath::XMMath3 dotPos(eye);
	dotPos.x -= camera->Target().x;
	dotPos.y -= camera->Target().y;
	dotPos.z -= camera->Target().z;
	dotPos.NormalizeMe();
	
	// This is not used
	float dot = dotPos.Dot(direction) * 50.0f;
	
	ZShadeSandboxMath::XMMath3 crossPos(direction);
	crossPos.Cross(dotPos);
	
	ZShadeSandboxMath::XMMath3 viewNormal0(pos0);
	viewNormal0.x -= eye.x;
	viewNormal0.y -= eye.y;
	viewNormal0.z -= eye.z;
	viewNormal0.NormalizeMe();
	
	ZShadeSandboxMath::XMMath3 viewNormal1(pos1);
	viewNormal1.x -= eye.x;
	viewNormal1.y -= eye.y;
	viewNormal1.z -= eye.z;
	viewNormal1.NormalizeMe();
	
	ZShadeSandboxMath::XMMath3 viewNormal2(pos2);
	viewNormal2.x -= eye.x;
	viewNormal2.y -= eye.y;
	viewNormal2.z -= eye.z;
	viewNormal2.NormalizeMe();
	
	ZShadeSandboxMath::XMMath3 viewNormal3(pos3);
	viewNormal3.x -= eye.x;
	viewNormal3.y -= eye.y;
	viewNormal3.z -= eye.z;
	viewNormal3.NormalizeMe();
	
	mGlowFactor = 1.0f * (0.5f - crossPos.Length());
	
	mQuads[0]->Rotation() = viewNormal0.ToFloat();
	mQuads[1]->Rotation() = viewNormal1.ToFloat();
	mQuads[2]->Rotation() = viewNormal2.ToFloat();
	mQuads[3]->Rotation() = viewNormal3.ToFloat();
	
	mQuads[0]->Position() = pos0;
	mQuads[1]->Position() = pos1;
	mQuads[2]->Position() = pos2;
	mQuads[3]->Position() = pos3;
	
	mQuads[0]->CalculateWorldMatrix();
	mQuads[1]->CalculateWorldMatrix();
	mQuads[2]->CalculateWorldMatrix();
	mQuads[3]->CalculateWorldMatrix();
}
//==============================================================================================================================
void Sun::UpdatePositions(int i, XMFLOAT3 pos)
{
	mQuads[i]->Position() = pos;
}
//==============================================================================================================================
void Sun::RegenSunQuad(XMFLOAT2 size)
{
	mQuads[0]->RegenSunQuad(size);
	mQuads[1]->RegenSunQuad(size);
	mQuads[2]->RegenSunQuad(size);
	mQuads[3]->RegenSunQuad(size);
}
//==============================================================================================================================
void Sun::RegenSunQuad(float y)
{
	mQuads[0]->RegenSunQuad(y);
	mQuads[1]->RegenSunQuad(y);
	mQuads[2]->RegenSunQuad(y);
	mQuads[3]->RegenSunQuad(y);
}
//==============================================================================================================================
ZShadeSandboxLighting::SunQuad* Sun::GetQuad(int i)
{
	return mQuads[i];
}
//==============================================================================================================================