#include "Camera.h"
#include "CGlobal.h"
#include "Light.h"
#include "LightCamera.h"
#include "PointColliderController.h"
//==================================================================================================================================
//==================================================================================================================================
Arm::Arm()
:   mTargetPosition(XMFLOAT3(-1, -1, -1))
,   mTargetDist(0)
{
}
//==================================================================================================================================
Arm::Arm(XMFLOAT3 pos, float dist)
:   mTargetPosition(pos)
,   mTargetDist(dist)
{
}
//==================================================================================================================================
Camera::Camera(EngineOptions* eo)
:  m_EngineOptions(eo)
{
	SetLens(0.25f * PI, 1.0f, 1.0f, 1000.0f);

	//mCamUp.Assign(XMFLOAT3(0.0f, 1.0f, 0.0f));
	//mCamPosition.Assign(XMFLOAT3(0.0f, 0.0f, 0.0f));
	//mCamRight.Assign(XMFLOAT3(1.0f, 0.0f, 0.0f));
	//mCamForward.Assign(XMFLOAT3(0.0f, 0.0f, 1.0f));
	mCamUp = XMFLOAT3(0, 1, 0);
	mCamPosition = XMFLOAT3(0, 0, 0);
	mCamRight = XMFLOAT3(1, 0, 0);
	mCamForward = XMFLOAT3(0, 0, 1);
	
	mThirdPerson = false;
	
	mTerrainCollisionOn = false;

	//Reflection view is disabled by default
	mRenderReflectionView = false;

	mMoveStrafe = 0.0f;
	mMoveWalk = 0.0f;

	mCamYaw = 0.0f;
	mCamPitch = 0.0f;
	
	m_ViewingFrustum = 0;
}
//==================================================================================================================================
Camera::Camera(const Camera& other)
{
}
//==================================================================================================================================
Camera::~Camera()
{
}
//==================================================================================================================================
void Camera::BuildCameraInvConstantBuffer(D3D* d3d, ID3D11Buffer*& buffer)
{
	cbInvMatrixBuffer cIMB;
	cIMB.g_InvViewProj = InvViewProj4x4();
	// Map the inv matrix buffer
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res2;
		d3d->GetDeviceContext()->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res2);
		{
			assert(mapped_res2.pData);
			*(cbInvMatrixBuffer*)mapped_res2.pData = cIMB;
		}
		d3d->GetDeviceContext()->Unmap(buffer, 0);
	}
}
//==================================================================================================================================
void Camera::BuildCameraConstantBuffer(D3D* d3d, ID3D11Buffer*& buffer, XMMATRIX world, bool reflection)
{
	cbMatrixBuffer cMB;
	cMB.g_matWorld = ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(world));
	cMB.g_matView = (reflection) ? ReflectionView4x4() : View4x4();
	if (d3d->GetEngineOptions()->m_DimType == DimType::ZSHADE_2D)
		cMB.g_matProj = Ortho4x4();
	else if (d3d->GetEngineOptions()->m_DimType == DimType::ZSHADE_3D)
		cMB.g_matProj = Proj4x4();
	// Map the matrix constants
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res2;
		d3d->GetDeviceContext()->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res2);
		{
			assert(mapped_res2.pData);
			*(cbMatrixBuffer*)mapped_res2.pData = cMB;
		}
		d3d->GetDeviceContext()->Unmap(buffer, 0);
	}
}
//==================================================================================================================================
void Camera::BuildCameraConstantBuffer(D3D* d3d, ID3D11Buffer*& buffer, ZShadeSandboxLighting::Light* light, XMMATRIX world, bool reflection)
{
	if (light != 0 && light->Perspective() != 0)
	{
		LightCamera* lightcamera = light->Perspective();

		cbMatrixBufferLight cMBL;
		cMBL.g_matWorld = ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(world));
		cMBL.g_matView = (reflection) ? ReflectionView4x4() : View4x4();
		if (d3d->GetEngineOptions()->m_DimType == DimType::ZSHADE_2D)
			cMBL.g_matProj = Ortho4x4();
		else if (d3d->GetEngineOptions()->m_DimType == DimType::ZSHADE_3D)
			cMBL.g_matProj = Proj4x4();
		cMBL.g_LightView = lightcamera->LightView4x4();
		cMBL.g_LightProj = lightcamera->LightProj4x4();
		XMMATRIX shadowTransform = world * lightcamera->ShadowTransform();
		cMBL.g_ShadowMatrix = ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(shadowTransform));
		// Map the matrix constants
		{
			D3D11_MAPPED_SUBRESOURCE mapped_res2;
			d3d->GetDeviceContext()->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res2);
			{
				assert(mapped_res2.pData);
				*(cbMatrixBufferLight*)mapped_res2.pData = cMBL;
			}
			d3d->GetDeviceContext()->Unmap(buffer, 0);
		}
	}
}
//==================================================================================================================================
void Camera::SetLens(float fovY, float aspect, float zn, float zf)
{
	// cache properties
	mFovY = fovY;
	mAspect = aspect;
	mNearZ = zn;
	mFarZ = zf;

	zf *= 0.5f;

	mNearWindowHeight = 2.0f * mNearZ * tanf( 0.5f*mFovY );
	mFarWindowHeight  = 2.0f * mFarZ * tanf( 0.5f*mFovY );

	mProj = XMMatrixPerspectiveFovLH(mFovY, mAspect, mNearZ, mFarZ);
}
//==================================================================================================================================
ZShadeSandboxMath::Ray* Camera::PickingRay(int mousex, int mousey, XMMATRIX clickedMeshWorld)
{
	float width = m_EngineOptions->m_screenWidth;
	float height = m_EngineOptions->m_screenHeight;

	float vx = (((2.0f * (float)mousex) / width) - 1.0f) / mProj._11;
	float vy = ((((2.0f * (float)mousey) / height) - 1.0f) * -1.0f) / mProj._22;

	XMMATRIX invView = XMMatrixInverse(NULL, mView);

	XMFLOAT3 dir;

	dir.x = (vx * invView._11) + (vy * invView._21) + invView._31;
	dir.y = (vx * invView._12) + (vy * invView._22) + invView._32;
	dir.z = (vx * invView._13) + (vy * invView._23) + invView._33;

	XMFLOAT3 origin = Position();

	XMMATRIX invWorld = XMMatrixInverse(NULL, clickedMeshWorld);

	ZShadeSandboxMath::XMMath3 rayOriginXM(origin);
	XMFLOAT3 rayOrigin = rayOriginXM.TransformCoord(invWorld);
	
	XMVECTOR v = XMVectorSet(dir.x, dir.y, dir.z, 0.0f);
	XMVector3TransformNormal(v, invWorld);
	XMFLOAT3 rd;
	XMStoreFloat3(&rd, v);
	ZShadeSandboxMath::XMMath3 rayDir(rd.x, rd.y, rd.z);
	rayDir.NormalizeMe();

	return new ZShadeSandboxMath::Ray(rayOrigin, rayDir.ToFloat());
}
//==================================================================================================================================
void Camera::AttachTarget(Arm a)
{
	mArm.mTargetPosition = a.mTargetPosition;
	mArm.mTargetDist = a.mTargetDist;
}
//==================================================================================================================================
void Camera::CreateOrtho(float w, float h, float zn, float zf)
{
	mOrtho = XMMatrixOrthographicLH(w, h, zn, zf);
}
//==================================================================================================================================
void Camera::Clone(Camera* c)
{
	mCamRotation = c->Rotation();
	mView = c->View();
	mProj = c->Proj();
	mReflectionView = c->ReflectionView();

	mInvProj = c->InvProj();
	mInvView = c->InvView();
	mInvViewProj = c->InvViewProj();

	mCamForward = c->Forward();//c->ForwardV();
	mCamRight = c->Right();//c->RightV();
	mCamTarget = c->Target();//c->TargetV();
	mCamUp = c->Up();//c->UpV();
	mCamPosition = c->Position();//c->positionV();

	mRenderReflectionView = c->RenderReflectionView();

	mMoveStrafe = c->MoveStrafe();
	mMoveWalk = c->MoveWalk();

	mCamYaw = c->CamYaw();
	mCamPitch = c->CamPitch();

	mFovY = c->FovY();
	mAspect = c->AspectRatio();
	mNearZ = c->NearZ();
	mFarZ = c->FarZ();

	mNearWindowHeight = c->NearWindowHeight();
	mFarWindowHeight = c->FarWindowHeight();
}
//==================================================================================================================================
void Camera::Update()
{
	if (m_EngineOptions->m_DimType == ZSHADE_2D)
	{
		mCamRotation = XMMatrixRotationRollPitchYaw(mCamPitch, mCamYaw, 0);

		XMVECTOR lookAt = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
		
		lookAt = XMVector3TransformCoord(lookAt, mCamRotation);

		XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		
		up = XMVector3TransformCoord(up, mCamRotation);

		XMVECTOR pos = XMVectorSet(mCamPosition.x, mCamPosition.y, mCamPosition.z, 0);

		lookAt = XMVectorAdd(pos, lookAt);

		mView = XMMatrixLookAtLH(pos, lookAt, up);
	}
	else if (m_EngineOptions->m_DimType == ZSHADE_3D)
	{
		if (mThirdPerson)
		{
			XMVECTOR target = XMVectorSet(mArm.mTargetPosition.x, mArm.mTargetPosition.y, mArm.mTargetPosition.z, 0);
			
			// Move the camera vector up 5 units from the target object (Should not need to do this)
			target = XMVectorSetY(target, XMVectorGetY(target) + 5.0f);
			
			// Rotate the camera's target around the target
			mCamRotation = XMMatrixRotationRollPitchYaw(-mCamPitch, mCamYaw, 0);
			XMVECTOR position = XMVector3TransformNormal(gDefaultForward, mCamRotation);
			position = XMVector3Normalize(position);
			
			// Set the camera's position to rotate around the target
			position = (position * mArm.mTargetDist) + target;
			
			// Set the camera's forward and right vectors to determine the direction the target is running
			XMVECTOR forward = XMVector3Normalize(target - position);
			forward = XMVectorSetY(forward, 0.0f);
			forward = XMVector3Normalize(forward);
			
			XMVECTOR right = XMVectorSet(-XMVectorGetZ(forward), 0.0f, XMVectorGetX(forward), 0.0f);
			
			// The camera does not roll so the camera's right vector is always in the xz plane
			XMVECTOR up = XMVector3Cross(XMVector3Normalize(position - target), right);
			
			mView = XMMatrixLookAtLH(position, target, up);
			
			XMStoreFloat3(&mCamPosition, position);
			XMStoreFloat3(&mCamTarget, target);
			XMStoreFloat3(&mCamRight, right);
			XMStoreFloat3(&mCamForward, forward);
			XMStoreFloat3(&mCamUp, up);
		}
		else
		{
			//if (!mTerrainCollisionOn)
			{
				//Create the View Matrix
				mCamRotation = XMMatrixRotationRollPitchYaw(mCamPitch, mCamYaw, 0);

				XMVECTOR target = XMVector3TransformCoord(gDefaultForward, mCamRotation);
				target = XMVector3Normalize(target);
				XMVECTOR refltarget = XMVector3TransformCoord(gDefaultForward, mCamRotation);
				refltarget = XMVector3Normalize(refltarget);
				
				XMVECTOR right = XMVector3TransformCoord(gDefaultRight, mCamRotation);
				XMVECTOR forward = XMVector3TransformCoord(gDefaultForward, mCamRotation);
				XMVECTOR up = XMVector3Cross(forward, right);

				//if (mTerrainCollisionOn)// && mMoved)
				//{
				//	ZShadeSandboxPhysics::PointColliderController collider;
				//	XMFLOAT3 intersectionPoint;

				//	bool colliding = collider.Colliding(
				//		100.0f,
				//		0.0f,//-0.3f,
				//		mCamPosition,
				//		mMoveStrafe,
				//		ZShadeSandboxMath::XMMath3(right),
				//		mMoveWalk,
				//		ZShadeSandboxMath::XMMath3(forward),
				//		vertexList,
				//		indexList,
				//		intersectionPoint
				//	);

				//	mCamPosition = intersectionPoint;
				//}

				XMVECTOR position = XMVectorSet(mCamPosition.x, mCamPosition.y, mCamPosition.z, 0);
				
				//if (!mTerrainCollisionOn)
				{
					position += mMoveStrafe * right;
					position += mMoveWalk * forward;
				}
				
				XMVECTOR direction = target - position;
				direction = XMVector3Normalize(direction);
				
				mMoveStrafe = 0.0f;
				mMoveWalk = 0.0f;

				target = XMVectorAdd(position, target);

				mView = XMMatrixLookAtLH(position, target, up);
				
				XMStoreFloat3(&mCamDirection, direction);
				XMStoreFloat3(&mCamPosition, position);
				XMStoreFloat3(&mCamTarget, target);
				XMStoreFloat3(&mCamRight, right);
				XMStoreFloat3(&mCamForward, forward);
				XMStoreFloat3(&mCamUp, up);
			}
		}
		
		//if (!mTerrainCollisionOn)
		{
			//Find all the inverses
			mInvProj     = XMMatrixInverse( NULL, mProj );
			mInvView     = XMMatrixInverse( NULL, mView );
			mInvViewProj = XMMatrixInverse( NULL, ViewProj() );
			
			// Update the viewing frustum
			if (m_ViewingFrustum != 0)
				m_ViewingFrustum->Update(ViewProj());

			// Rebuild the sphere and cone for the viewing frustum
			BuildFrustumConeSphere();
		}
		
		//if (mTerrainCollisionOn)
		//{
		//	TerrainCollisionUpdateBegin();
		//}
	}
}
//==================================================================================================================================
void Camera::UpdateReflection(float planeHeight)
{
	if (m_EngineOptions->m_DimType == ZSHADE_3D)
	{
		//Create the ReflectionView Matrix
		if (mRenderReflectionView)
		{
			XMMATRIX rotation = XMMatrixRotationRollPitchYaw( mCamPitch, mCamYaw, 0 );

			XMFLOAT3 targetPos = mCamTarget;// .ToFloat();
			XMFLOAT3 camPos = mCamPosition;// .ToFloat();
			
			float coeff = planeHeight * 2.0f;

			//
			// Calculate the new camera reflection position
			//

			XMFLOAT3 reflectionCampositionF = XMFLOAT3(camPos.x, -camPos.y + coeff, camPos.z);
			//XMFLOAT3 reflectionCampositionF = XMFLOAT3(camPos.x, camPos.y, -camPos.z + coeff);
			XMVECTOR reflectionCamposition = XMLoadFloat3( &reflectionCampositionF );

			//
			// Calculate the new camera reflection target
			//

			XMFLOAT3 reflectionCamTargetF = XMFLOAT3(targetPos.x, -targetPos.y + coeff, targetPos.z);
			//XMFLOAT3 reflectionCamTargetF = XMFLOAT3(targetPos.x, targetPos.y, -targetPos.z + coeff);
			XMVECTOR reflectionCamTarget = XMLoadFloat3( &reflectionCamTargetF );
			
			XMFLOAT3 forwardVectorF = ZShadeSandboxMath::ZMath::SubFloat3(reflectionCamTargetF, reflectionCampositionF);
			XMVECTOR forwardVector = XMLoadFloat3( &forwardVectorF );
			XMVECTOR defaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
			XMVECTOR sideVector = XMVector3TransformCoord(defaultRight, rotation);
			XMVECTOR reflectionUp = XMVector3Cross(sideVector, forwardVector);

			mReflectionView = XMMatrixLookAtLH( reflectionCamposition, reflectionCamTarget, reflectionUp );
		}
	}
}
//==================================================================================================================================
void Camera::UpdateReflection(XMVECTOR plane)
{
	if (m_EngineOptions->m_DimType == ZSHADE_3D)
	{
		//Create the ReflectionView Matrix
		if (mRenderReflectionView)
		{
			mReflectionView = XMMatrixReflect(plane);
		}
	}
}
//==================================================================================================================================
void Camera::TerrainCollisionUpdateBegin()
{
	//Create the View Matrix
	mCamRotation = XMMatrixRotationRollPitchYaw(mCamPitch, mCamYaw, 0);

	XMVECTOR target = XMVector3TransformCoord(gDefaultForward, mCamRotation);
	target = XMVector3Normalize(target);
	XMVECTOR refltarget = XMVector3TransformCoord(gDefaultForward, mCamRotation);
	refltarget = XMVector3Normalize(refltarget);
	
	XMVECTOR right = XMVector3TransformCoord(gDefaultRight, mCamRotation);
	XMVECTOR forward = XMVector3TransformCoord(gDefaultForward, mCamRotation);
	XMVECTOR up = XMVector3Cross(forward, right);
	
	XMStoreFloat3(&mCamRight, right);
	XMStoreFloat3(&mCamForward, forward);
	XMStoreFloat3(&mCamUp, up);
	XMStoreFloat3(&mCamTarget, target);
}
//==================================================================================================================================
void Camera::TerrainCollisionUpdateEnd()
{
	XMVECTOR position = XMVectorSet(mCamPosition.x, mCamPosition.y, mCamPosition.z, 0);
	//position += mMoveStrafe * mCamRight;
	//position += mMoveWalk * mCamForward;

	XMVECTOR direction = ZShadeSandboxMath::XMMath3(mCamTarget).ToVector() - position;
	direction = XMVector3Normalize(direction);
	
	mMoveStrafe = 0.0f;
	mMoveWalk = 0.0f;

	XMVECTOR target = XMVectorAdd(position, ZShadeSandboxMath::XMMath3(mCamTarget).ToVector());

	mView = XMMatrixLookAtLH(position, target, ZShadeSandboxMath::XMMath3(mCamUp).ToVector());
	
	XMStoreFloat3(&mCamDirection, direction);
	XMStoreFloat3(&mCamPosition, position);
	XMStoreFloat3(&mCamTarget, target);
	
	//Find all the inverses
	mInvProj     = XMMatrixInverse( NULL, mProj );
	mInvView     = XMMatrixInverse( NULL, mView );
	mInvViewProj = XMMatrixInverse( NULL, ViewProj() );
	
	// Update the viewing frustum
	if (m_ViewingFrustum != 0)
		m_ViewingFrustum->Update(ViewProj());

	// Rebuild the sphere and cone for the viewing frustum
	BuildFrustumConeSphere();
}
//==================================================================================================================================
void Camera::AfterReset(float aspect)
{
	mAspect = aspect;
	mProj = XMMatrixPerspectiveFovLH(mFovY, mAspect, mNearZ, mFarZ);
	Update();
}
//==================================================================================================================================
void Camera::Strafe(float speed)
{
	mMoved = true;
	mMoveStrafe += speed;
}
//==================================================================================================================================
void Camera::Walk(float speed)
{
	mMoved = true;
	mMoveWalk += speed;
}
//==================================================================================================================================
void Camera::UpdateRotation(float yaw, float pitch)
{
	mCamYaw += yaw;
	mCamPitch += pitch;

	/*if (pitch < 90.0f)
	{
		mCamPitch += pitch;
	}*/
	
	if (mThirdPerson)
	{
		// Check that the camera doesn't go over the top or under the target it is following
		if(mCamPitch > 0.85f)
			mCamPitch = 0.85f;
		if(mCamPitch < -0.85f)
			mCamPitch = -0.85f;
	}
}
//==================================================================================================================================
/*XMVECTOR Camera::positionV()
{
	return XMLoadFloat3(&mCamPosition);
}
//==================================================================================================================================
XMVECTOR Camera::mapPositionV()
{
	return XMLoadFloat3(&mCamMapPosition);
}
//==================================================================================================================================
XMVECTOR Camera::TargetV()
{
	return XMLoadFloat3(&mCamTarget);
}
//==================================================================================================================================
XMVECTOR Camera::RightV()
{
	return XMLoadFloat3(&mCamRight);
}
//==================================================================================================================================
XMVECTOR Camera::ForwardV()
{
	return XMLoadFloat3(&mCamForward);
}
//==================================================================================================================================
XMVECTOR Camera::UpV()
{
	return XMLoadFloat3(&mCamUp);
}*/
//==================================================================================================================================
void Camera::SetPosition(float x, float y, float z)
{
	mCamPosition = ZShadeSandboxMath::XMMath3(x, y, z);
}
//==================================================================================================================================
void Camera::SetMapPosition(float x, float y, float z)
{
	mCamMapPosition = ZShadeSandboxMath::XMMath3(x, y, z);
}
//==================================================================================================================================
XMFLOAT4X4 Camera::ViewProj4x4()
{
	return ZShadeSandboxMath::ZMath::GMathMF(XMMatrixMultiply(XMMatrixTranspose(mView), XMMatrixTranspose(mProj)));
}
//==================================================================================================================================
XMFLOAT4X4 Camera::ReflViewProj4x4()
{
	return ZShadeSandboxMath::ZMath::GMathMF(XMMatrixMultiply(XMMatrixTranspose(mReflectionView), XMMatrixTranspose(mProj)));
}
//==================================================================================================================================
XMMATRIX Camera::ViewProj()
{
	return XMMatrixMultiply( mView, mProj );
}
//==================================================================================================================================
XMMATRIX Camera::ReflViewProj()
{
	return XMMatrixMultiply( mReflectionView, mProj );
}
//==================================================================================================================================
XMFLOAT3 Camera::Direction()
{
	return mCamDirection;
}
//==================================================================================================================================
XMFLOAT3 Camera::Position()
{
	return mCamPosition;
}
//==================================================================================================================================
XMFLOAT3 Camera::MapPosition()
{
	return mCamMapPosition;
}
//==================================================================================================================================
XMFLOAT3 Camera::Target()
{
	return mCamTarget;
}
//==================================================================================================================================
XMFLOAT3 Camera::Right()
{
	return mCamRight;
}
//==================================================================================================================================
XMFLOAT3 Camera::Forward()
{
	return mCamForward;
}
//==================================================================================================================================
XMFLOAT3 Camera::Up()
{
	return mCamUp;
}
//==================================================================================================================================
XMVECTOR Camera::VDirection()
{
	return ZShadeSandboxMath::XMMath3(mCamDirection).ToVector();
}
//==================================================================================================================================
XMVECTOR Camera::VPosition()
{
	return ZShadeSandboxMath::XMMath3(mCamPosition).ToVector();
}
//==================================================================================================================================
XMVECTOR Camera::VMapPosition()
{
	return ZShadeSandboxMath::XMMath3(mCamMapPosition).ToVector();
}
//==================================================================================================================================
XMVECTOR Camera::VTarget()
{
	return ZShadeSandboxMath::XMMath3(mCamTarget).ToVector();
}
//==================================================================================================================================
XMVECTOR Camera::VRight()
{
	return ZShadeSandboxMath::XMMath3(mCamRight).ToVector();
}
//==================================================================================================================================
XMVECTOR Camera::VForward()
{
	return ZShadeSandboxMath::XMMath3(mCamForward).ToVector();
}
//==================================================================================================================================
XMVECTOR Camera::VUp()
{
	return ZShadeSandboxMath::XMMath3(mCamUp).ToVector();
}
//==================================================================================================================================
/*void Camera::SetPositionV(XMVECTOR v)
{
	XMStoreFloat3(&mCamPosition, v);
}
//==================================================================================================================================
void Camera::SetMapPositionV(XMVECTOR v)
{
	XMStoreFloat3(&mCamMapPosition, v);
}
//==================================================================================================================================
void Camera::SetTargetV(XMVECTOR v)
{
	XMStoreFloat3(&mCamTarget, v);
}
//==================================================================================================================================
void Camera::SetRightV(XMVECTOR v)
{
	XMStoreFloat3(&mCamRight, v);
}
//==================================================================================================================================
void Camera::SetForwardV(XMVECTOR v)
{
	XMStoreFloat3(&mCamForward, v);
}
//==================================================================================================================================
void Camera::SetUpV(XMVECTOR v)
{
	XMStoreFloat3(&mCamUp, v);
}*/
//==================================================================================================================================
void Camera::BuildFrustumConeSphere()
{
	// Calculate the radius of the frustum sphere
	float viewLen = mFarZ - mNearZ;

	// Use Trig to find the height of the frustum at the far plane
	float height = viewLen * tan( mFovY * 0.5f );

	// With an aspect ratio of 1, width is same
	float width = height;

	// Halfway point between near/far planes starting at origin and extending along z axis
	ZShadeSandboxMath::Vec3<XMFLOAT3> P(0.0f, 0.0f, mNearZ + viewLen * 0.5f);

	// The far corner of the frustum
	ZShadeSandboxMath::Vec3<XMFLOAT3> Q(width, height, viewLen);

	// The vector between P and Q
	ZShadeSandboxMath::Vec3<XMFLOAT3> vDiff = P - Q;

	// The radius becomes the length of this vector
	float radius = vDiff.Magnitude();

	// Get the look vector of the camera from the view matrix
	ZShadeSandboxMath::Vec3<XMFLOAT3> look = XMFLOAT3(Forward().x, Forward().y, Forward().z);

	// Calculate the center of the sphere
	ZShadeSandboxMath::Vec3<XMFLOAT3> pos3 = XMFLOAT3(Position().x, Position().y, Position().z);
	ZShadeSandboxMath::Vec3<XMFLOAT3> center = pos3 + (look * (viewLen * 0.5f) + mNearZ);

	// Create the Sphere
	m_Sphere.Set(center, radius);

	// set the properties of the frustum cone… vLookVector is the
	//   look vector from the view matrix in the 
	// camera.  position() returns the position of the camera.  
	// fWidth is half the width of the screen (in pixels).  
	// fHeight is half the height of the screen in pixels. 
	// m_fFovRadians is the FOV of the frustum.

	// Calculate the length of the fov triangle
	float depth = height / tan( mFovY * 0.5f );

	// Calculate the corner of the screen
	float corner = sqrt( width * width + height * height );

	// Calculate the new FOV
	float fov = atan( corner / depth );

	// Create the cone
	m_Cone.Set(pos3, look, fov, height);
}
//==================================================================================================================================
void Camera::ExtractFrustumPlanes(XMFLOAT4 planes[6])
{
	// If there is no viewing frustum construct it
	if (m_ViewingFrustum == 0)
		m_ViewingFrustum = new Frustum(ViewProj());
	
	// Get the frustum planes
	m_ViewingFrustum->ExtractFrustumPlanes(planes);
}
//==================================================================================================================================
Camera::Frustum::Frustum(CXMMATRIX MVP)
{
	BuildFrustumPlanes(MVP);
}
//==================================================================================================================================
void Camera::Frustum::Update(CXMMATRIX MVP)
{
	// Rebuild the viewing frustum planes
	BuildFrustumPlanes(MVP);
}
//==================================================================================================================================
void Camera::Frustum::BuildFrustumPlanes(CXMMATRIX MVP)
{
	//
	// Left
	//
	
	planes[0].x = MVP(0,3) + MVP(0,0);
	planes[0].y = MVP(1,3) + MVP(1,0);
	planes[0].z = MVP(2,3) + MVP(2,0);
	planes[0].w = MVP(3,3) + MVP(3,0);

	//
	// Right
	//
	planes[1].x = MVP(0,3) - MVP(0,0);
	planes[1].y = MVP(1,3) - MVP(1,0);
	planes[1].z = MVP(2,3) - MVP(2,0);
	planes[1].w = MVP(3,3) - MVP(3,0);

	//
	// Bottom
	//
	planes[2].x = MVP(0,3) + MVP(0,1);
	planes[2].y = MVP(1,3) + MVP(1,1);
	planes[2].z = MVP(2,3) + MVP(2,1);
	planes[2].w = MVP(3,3) + MVP(3,1);

	//
	// Top
	//
	planes[3].x = MVP(0,3) - MVP(0,1);
	planes[3].y = MVP(1,3) - MVP(1,1);
	planes[3].z = MVP(2,3) - MVP(2,1);
	planes[3].w = MVP(3,3) - MVP(3,1);

	//
	// Near
	//
	planes[4].x = MVP(0,2);
	planes[4].y = MVP(1,2);
	planes[4].z = MVP(2,2);
	planes[4].w = MVP(3,2);

	//
	// Far
	//
	planes[5].x = MVP(0,3) - MVP(0,2);
	planes[5].y = MVP(1,3) - MVP(1,2);
	planes[5].z = MVP(2,3) - MVP(2,2);
	planes[5].w = MVP(3,3) - MVP(3,2);

	// Normalize the plane equations.
	for(int i = 0; i < 6; ++i)
	{
		XMVECTOR v = XMPlaneNormalize(XMLoadFloat4(&this->planes[i]));
		XMStoreFloat4(&this->planes[i], v);
	}
}
//==================================================================================================================================
void Camera::Frustum::ExtractFrustumPlanes(XMFLOAT4 planes[6])
{
	planes = this->planes;
}
//==================================================================================================================================
float Camera::Frustum::PlaneDotCoord(XMFLOAT4 pplane, XMFLOAT3 point)
{
	ZShadeSandboxMath::XMMath3 fp(point.x, point.y, point.z);
	ZShadeSandboxMath::XMMath4 plane(pplane.x, pplane.y, pplane.z, pplane.w);

	XMVECTOR res = XMPlaneDotCoord(plane.ToVector(), fp.ToVector());

	XMFLOAT4 r;
	XMStoreFloat4(&r, res);

	return r.x;
}
//==================================================================================================================================
bool Camera::Frustum::ContainsPoint(XMFLOAT3 point)
{
	for (int i = 0; i < 6; i++)
	{
		if (PlaneDotCoord(planes[i], point) < 0.0f)
		{
			return false;
		}
	}

	return true;
}
//==================================================================================================================================
bool Camera::Frustum::ContainsCube(XMFLOAT3 point, float radius)
{
	for (int i = 0; i < 6; i++)
	{
		XMFLOAT3 np = XMFLOAT3(point.x - radius, point.y - radius, point.z - radius);
		if (PlaneDotCoord(planes[i], np) >= 0.0f) continue;

		XMFLOAT3 np1 = XMFLOAT3(point.x + radius, point.y - radius, point.z - radius);
		if (PlaneDotCoord(planes[i], np1) >= 0.0f) continue;

		XMFLOAT3 np2 = XMFLOAT3(point.x - radius, point.y + radius, point.z - radius);
		if (PlaneDotCoord(planes[i], np2) >= 0.0f) continue;

		XMFLOAT3 np3 = XMFLOAT3(point.x + radius, point.y + radius, point.z - radius);
		if (PlaneDotCoord(planes[i], np3) >= 0.0f) continue;

		XMFLOAT3 np4 = XMFLOAT3(point.x - radius, point.y - radius, point.z + radius);
		if (PlaneDotCoord(planes[i], np4) >= 0.0f) continue;

		XMFLOAT3 np5 = XMFLOAT3(point.x + radius, point.y - radius, point.z + radius);
		if (PlaneDotCoord(planes[i], np5) >= 0.0f) continue;

		XMFLOAT3 np6 = XMFLOAT3(point.x - radius, point.y + radius, point.z + radius);
		if (PlaneDotCoord(planes[i], np6) >= 0.0f) continue;

		XMFLOAT3 np7 = XMFLOAT3(point.x + radius, point.y + radius, point.z + radius);
		if (PlaneDotCoord(planes[i], np7) >= 0.0f) continue;

		return false;
	}

	return true;
}
//==================================================================================================================================
bool Camera::Frustum::ContainsSphere(XMFLOAT3 point, float radius)
{
	for (int i = 0; i < 6; i++)
	{
		if (PlaneDotCoord(planes[i], point) < -radius)
		{
			return false;
		}
	}

	return true;
}
//==================================================================================================================================
bool Camera::Frustum::ContainsAABB(XMFLOAT3 point, XMFLOAT3 size)
{
	for (int i = 0; i < 6; i++)
	{
		XMFLOAT3 np1 = XMFLOAT3(point.x - size.x, point.y - size.y, point.z - size.z);
		if (PlaneDotCoord(planes[i], np1) >= 0.0f) continue;

		XMFLOAT3 np2 = XMFLOAT3(point.x + size.x, point.y - size.y, point.z - size.z);
		if (PlaneDotCoord(planes[i], np2) >= 0.0f) continue;

		XMFLOAT3 np3 = XMFLOAT3(point.x - size.x, point.y + size.y, point.z - size.z);
		if (PlaneDotCoord(planes[i], np3) >= 0.0f) continue;

		XMFLOAT3 np4 = XMFLOAT3(point.x - size.x, point.y - size.y, point.z + size.z);
		if (PlaneDotCoord(planes[i], np4) >= 0.0f) continue;

		XMFLOAT3 np5 = XMFLOAT3(point.x + size.x, point.y + size.y, point.z - size.z);
		if (PlaneDotCoord(planes[i], np5) >= 0.0f) continue;

		XMFLOAT3 np6 = XMFLOAT3(point.x + size.x, point.y - size.y, point.z + size.z);
		if (PlaneDotCoord(planes[i], np6) >= 0.0f) continue;

		XMFLOAT3 np7 = XMFLOAT3(point.x - size.x, point.y + size.y, point.z + size.z);
		if (PlaneDotCoord(planes[i], np7) >= 0.0f) continue;

		XMFLOAT3 np8 = XMFLOAT3(point.x + size.x, point.y + size.y, point.z + size.z);
		if (PlaneDotCoord(planes[i], np8) >= 0.0f) continue;

		return false;
	}

	return true;
}
//==================================================================================================================================
/*int Camera::Frustum::ContainsSphere(ZSphere refSphere) const
{
	float dist;

	// Calculate the distances to each of the frustum planes
	for(int i = 0; i < 6; ++i)
	{
		ZPlane plane(planes[i]);

		XMFLOAT3 center = refSphere.Center();

		// Find the distance to this plane
		dist = plane.Normal().Dot( center ) + plane.Distance();

		// If the distance is < -sphere.radius, we are outside
		if (dist < -refSphere.Radius()) return OUT;

		// If the distance is between +- radius, then we intersect
		if ((float)fabs(dist) < refSphere.Radius()) return INTERSECT;
	}

	// We are fully within view
	return IN;
}
//==================================================================================================================================
int Camera::Frustum::ContainsBox(ZBox refBox) const
{
	XMFLOAT4 vCorner[8];

	unsigned int iTotalIn = 0;

	// Get the corners of the box
	refBox.GetVertices( vCorner );

	// test all 8 corners against the 6 sides 
	// if all points are behind 1 specific plane, we are out
	// if we are in with all points, then we are fully in
	for (int p = 0; p < 6; ++p)
	{
		int iInCount = 8;
		int iPtIn = 1;

		for(int i = 0; i < 6; ++i)
		{
			ZPlane plane(planes[i]);

			// Test this point to see if it lies behind the plane
			if (plane.BehindPlane( vCorner[i] ))
			{
				iPtIn = 0;
				--iInCount;
			}
		}

		// Are all the points outside of the plane
		if (iInCount == 0) return OUT;

		// Check if the points were all on the right side of the plane
		iTotalIn += iPtIn;
	}

	// if iTotalIn is 6 then all are inside
	if (iTotalIn == 6) return IN;

	// We are partly in
	return INTERSECT;
}
//==================================================================================================================================
int Camera::Frustum::ContainsAABB(XMFLOAT3 aabbMin, XMFLOAT3 aabbMax)
{
	int ret = IN;
	XMFLOAT3 vmin, vmax;

	for (int i = 0; i < 6; ++i)
	{
		ZPlane plane(planes[i]);

		float nx = plane.Normal().Vector().x;
		float ny = plane.Normal().Vector().y;
		float nz = plane.Normal().Vector().z;

		// X axis 
		if (nx > 0)
		{
			vmin.x = aabbMin.x;
			vmax.x = aabbMax.x;
		}
		else
		{
			vmin.x = aabbMax.x;
			vmax.x = aabbMin.x;
		}
		
		// Y axis 
		if (ny > 0)
		{
			vmin.y = aabbMin.y;
			vmax.y = aabbMax.y;
		}
		else
		{
			vmin.y = aabbMax.y;
			vmax.y = aabbMin.y;
		}
		
		// Z axis
		if (nz > 0)
		{
			vmin.z = aabbMin.z;
			vmax.z = aabbMax.z;
		}
		else
		{
			vmin.z = aabbMax.z;
			vmax.z = aabbMin.z;
		}
		
		float min_dot = (nx * vmin.x + ny * vmin.y + nz * vmin.z);
		float max_dot = (nx * vmax.x + ny * vmax.y + nz * vmax.z);

		if (min_dot + plane.Distance() > 0)
			return OUT;
		if (max_dot + plane.Distance() >= 0)
			ret = INTERSECT;
	}

	return ret;
}
//==================================================================================================================================
*/