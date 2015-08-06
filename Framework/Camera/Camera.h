//==================================================================================================================================
// Camera.h
//
//==================================================================================================================================
// History
//
// -Created on 6/16/14 by Dustin Watson
// -Updated on 11/24/14 for 2D and 3D
// -Updated on 4/29/2015 added in Third Person camera support
//==================================================================================================================================
#ifndef __CAMERA_H
#define __CAMERA_H
//==================================================================================================================================
//==================================================================================================================================

//
// Includes
//

#include "D3D.h"
#include "ZMath.h"
#include "Ray.h"
#define _XM_NO_INTRINSICS_
#include <DirectXMath.h>
using namespace DirectX;

#include "Triangle.h"

//==================================================================================================================================
//==================================================================================================================================

namespace ZShadeSandboxLighting {
	class Light;
}

//==================================================================================================================================

struct cbInvMatrixBuffer
{
	XMFLOAT4X4	g_InvViewProj;
};

struct cbMatrixBuffer
{
	XMFLOAT4X4	g_matWorld;
	XMFLOAT4X4	g_matView;
	XMFLOAT4X4	g_matProj;
};

struct cbMatrixBufferLight
{
	XMFLOAT4X4	g_matWorld;
	XMFLOAT4X4	g_matView;
	XMFLOAT4X4	g_matProj;
	XMFLOAT4X4	g_ShadowMatrix;
	XMFLOAT4X4  g_LightView;
	XMFLOAT4X4  g_LightProj;
};

//==================================================================================================================================

struct Arm
{
	XMFLOAT3 mTargetPosition;
	float mTargetDist;
	Arm();
	Arm(XMFLOAT3 pos, float dist);
};

//==================================================================================================================================
static XMVECTOR gDefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
static XMVECTOR gDefaultUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
static XMVECTOR gDefaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
//==================================================================================================================================
class Camera
{
	EngineOptions* m_EngineOptions;
	
	XMMATRIX mWorld;
	XMMATRIX mCamRotation;
	XMMATRIX mView;
	XMMATRIX mProj;
	XMMATRIX mReflectionView;
	XMMATRIX mOrtho;

	XMMATRIX mInvProj;
	XMMATRIX mInvView;
	XMMATRIX mInvViewProj;

	XMFLOAT3 mCamForward;
	XMFLOAT3 mCamRight;
	XMFLOAT3 mCamTarget;
	XMFLOAT3 mCamUp;
	XMFLOAT3 mCamPosition;
	XMFLOAT3 mCamDirection;
	
	// For 2D
	XMFLOAT3 mCamMapPosition;
	
	bool mMoved;

	bool mRenderReflectionView;
	bool mTerrainCollisionOn;
	
	// Third Person camera attributes
	bool mThirdPerson;
	Arm mArm;
	
	float mMoveStrafe;
	float mMoveWalk;

	float mCamYaw;
	float mCamPitch;

	float mFovY;
	float mAspect;
	float mNearZ;
	float mFarZ;

	int mNearWindowHeight;
	int mFarWindowHeight;
	
	// Helper class that contains the six viewing frustum planes
	class Frustum
	{
		XMFLOAT4 planes[6];
		
		float PlaneDotCoord(XMFLOAT4 pplane, XMFLOAT3 point);

	public:
		
		Frustum(CXMMATRIX MVP);
		
		// Updates the viewing frustum
		void Update(CXMMATRIX MVP);
		
		// Build the six frustum planes based on the ViewProj matrix
		void BuildFrustumPlanes(CXMMATRIX MVP);
		
		// Returns the frustum planes
		void ExtractFrustumPlanes(XMFLOAT4 planes[6]);
		
		bool ContainsPoint(XMFLOAT3 point);
		bool ContainsCube(XMFLOAT3 point, float radius);
		bool ContainsSphere(XMFLOAT3 point, float radius);
		bool ContainsAABB(XMFLOAT3 point, XMFLOAT3 size);
	};
	
	// The viewing frustum for the camera
	Frustum* m_ViewingFrustum;

	ZShadeSandboxMath::SpherePrimitive m_Sphere;
	ZShadeSandboxMath::ConePrimitive m_Cone;
	
	void BuildFrustumConeSphere();

	//vector<Triangle*> mColliderTriangles;
	vector<XMFLOAT3> vertexList;
	vector<UINT> indexList;

public:
	
	Camera(EngineOptions* eo);
	Camera(const Camera& other);
	~Camera();
	
	void BuildCameraInvConstantBuffer(D3D* d3d, ID3D11Buffer*& buffer);
	void BuildCameraConstantBuffer(D3D* d3d, ID3D11Buffer*& buffer, XMMATRIX world, bool reflection);
	void BuildCameraConstantBuffer(D3D* d3d, ID3D11Buffer*& buffer, ZShadeSandboxLighting::Light* light, XMMATRIX world, bool reflection);
	
	void SetVertexList(vector<XMFLOAT3> vl) { vertexList = vl; }
	void SetIndexList(vector<UINT> il) { indexList = il; }

	//void SetColliderTriangles(vector<Triangle*> triangles) { mColliderTriangles = triangles; }

	Frustum* ViewingFrustum() { return m_ViewingFrustum; }
	ZShadeSandboxMath::SpherePrimitive Sphere() { return m_Sphere; }
	ZShadeSandboxMath::ConePrimitive Cone() { return m_Cone; }
	
	// Returns a ray from the camera to the mouse position in in 3D
	ZShadeSandboxMath::Ray* PickingRay(int mousex, int mousey, XMMATRIX clickedMeshWorld);
	
	// Packs the matrix data into column-major order for the shaders
	XMFLOAT4X4 ViewProj4x4();
	XMFLOAT4X4 ReflViewProj4x4();
	XMFLOAT4X4 World4x4()			{ return ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(mWorld)); }
	XMFLOAT4X4 View4x4()			{ return ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(mView)); }
	XMFLOAT4X4 Proj4x4()			{ return ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(mProj)); }
	XMFLOAT4X4 Ortho4x4()			{ return ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(mOrtho)); }
	XMFLOAT4X4 ReflectionView4x4()	{ return ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(mReflectionView)); }
	XMFLOAT4X4 Rotation4x4()		{ return ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(mCamRotation)); }
	XMFLOAT4X4 InvProj4x4()			{ return ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(mInvProj)); }
	XMFLOAT4X4 InvView4x4()			{ return ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(mInvView)); }
	XMFLOAT4X4 InvViewProj4x4()		{ return ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(mInvViewProj)); }
	
	// Returns the matrix data with row-major order
	XMMATRIX ViewProj();
	XMMATRIX ReflViewProj();
	XMMATRIX World()				{ return mWorld; }
	XMMATRIX View()					{ return mView; }
	XMMATRIX Proj()					{ return mProj; }
	XMMATRIX Ortho()				{ return mOrtho; }
	XMMATRIX ReflectionView()		{ return mReflectionView; }
	XMMATRIX Rotation()				{ return mCamRotation; }
	XMMATRIX InvProj()				{ return mInvProj; }
	XMMATRIX InvView()				{ return mInvView; }
	XMMATRIX InvViewProj()			{ return mInvViewProj; }
	
	XMFLOAT3 Direction();
	XMFLOAT3 Position();
	XMFLOAT3 MapPosition();
	XMFLOAT3 Target();
	XMFLOAT3 Right();
	XMFLOAT3 Forward();
	XMFLOAT3 Up();

	XMVECTOR VDirection();
	XMVECTOR VPosition();
	XMVECTOR VMapPosition();
	XMVECTOR VTarget();
	XMVECTOR VRight();
	XMVECTOR VForward();
	XMVECTOR VUp();

	void SetWorld(XMMATRIX m)			{ mWorld = m; }
	void SetView(XMMATRIX m)			{ mView = m; }
	void SetProj(XMMATRIX m)			{ mProj = m; }
	void SetOrtho(XMMATRIX m)			{ mOrtho = m; }
	void SetReflectionView(XMMATRIX m)  { mReflectionView = m; }
	void SetRotation(XMMATRIX m)		{ mCamRotation = m; }
	void SetInvProj(XMMATRIX m)			{ mInvProj = m; }
	void SetInvView(XMMATRIX m)			{ mInvView = m; }
	void SetInvViewProj(XMMATRIX m)		{ mInvViewProj = m; }

	void SetDirection(XMFLOAT3 f)		{ mCamDirection = f; }
	void SetPosition(XMFLOAT3 f)		{ mCamPosition = f; }
	void SetMapPosition(XMFLOAT3 f)		{ mCamMapPosition = f; }
	void SetTarget(XMFLOAT3 f)			{ mCamTarget = f; }
	void SetRight(XMFLOAT3 f)			{ mCamRight = f; }
	void SetForward(XMFLOAT3 f)			{ mCamForward = f; }
	void SetUp(XMFLOAT3 f)				{ mCamUp = f; }

	float MoveStrafe() { return mMoveStrafe; }
	float MoveWalk() { return mMoveWalk; }

	float CamYaw() { return mCamYaw; }
	float CamPitch() { return mCamPitch; }

	void AfterReset(float aspect);
	
	void AttachTarget(Arm a);
	bool& ThirdPerson() 						{ return mThirdPerson; }
	bool ThirdPerson() const					{ return mThirdPerson; }
	
	void TerrainCollisionUpdateBegin();
	void TerrainCollisionUpdateEnd();
	
	bool& TerrainCollisionOn() 					{ return mTerrainCollisionOn; }
	bool TerrainCollisionOn() const				{ return mTerrainCollisionOn; }
	
	float FovY()			{ return mFovY; }
	float AspectRatio()		{ return mAspect; }
	float NearZ()			{ return mNearZ; }
	float FarZ()			{ return mFarZ; }
	int NearWindowHeight()	{ return mNearWindowHeight; }
	int FarWindowHeight()	{ return mFarWindowHeight; }

	void ExtractFrustumPlanes(XMFLOAT4 planes[6]);

	bool RenderReflectionView()				{ return mRenderReflectionView; }
	void SetRenderReflectionView(bool r)	{ mRenderReflectionView = r; }

	void SetPositionY(float y) { mCamPosition.y = y; }
	void SetPosition(float x, float y, float z);
	void SetMapPosition(float x, float y, float z);
	void SetLens(float fovY, float aspect, float zn, float zf);
	void CreateOrtho(float w, float h, float zn, float zf);
	
	void Update();
	void UpdateReflection(float planeHeight);
	void UpdateReflection(XMVECTOR plane);

	void Strafe(float speed);
	void Walk(float speed);
	void UpdateRotation(float yaw, float pitch);

	void Clone(Camera* c);
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__CAMERA_H