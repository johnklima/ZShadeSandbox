//==============================================================================================================================
// Light.h
//
//==============================================================================================================================
// History
//
// -Created on 4/17/2015  by Dustin Watson
//==============================================================================================================================
#ifndef __LIGHT_H
#define __LIGHT_H
//==============================================================================================================================
//==============================================================================================================================

//
// Includes
//

#include "D3D.h"
#include "Camera.h"
#include "ZMath.h"
#include "LightCamera.h"

namespace ZShadeSandboxMesh {
	class CustomMesh;
}

namespace ZShadeSandboxLighting {
	struct LightBuffer;
	struct SunLightBuffer;
	struct LightRenderParameters;
}

//==============================================================================================================================
//==============================================================================================================================
namespace ZShadeSandboxLighting {
namespace ELightType
{
	enum
	{
		eAmbient,
		eDirectional,
		eSpot,
		ePoint,
		eCapsule
	};
}
//==============================================================================================================================
//==============================================================================================================================
class Light
{
public:
	
	Light();
	
	int& LightType() 						{ return mLightType; }
	XMFLOAT4& DiffuseColor() 				{ return mDiffuseColor; }
	XMFLOAT4& AmbientColor() 				{ return mAmbientColor; }
	XMFLOAT4& SpecularColor() 				{ return mSpecularColor; }
	XMFLOAT3& Position() 					{ return mPosition; }
	XMFLOAT3& Direction() 					{ return mDirection; }
	float& Intensity()						{ return fIntensity; }
	bool& ToggleLight()						{ return bToggleLight; }
	
	LightCamera* Perspective()				{ return mLightCamera; }
	int LightType() const 					{ return mLightType; }
	XMFLOAT4 DiffuseColor() const 			{ return mDiffuseColor; }
	XMFLOAT4 AmbientColor() const 			{ return mAmbientColor; }
	XMFLOAT4 SpecularColor() const			{ return mSpecularColor; }
	XMFLOAT3 Position() const 				{ return mPosition; }
	XMFLOAT3 Direction() const 				{ return mDirection; }
	float Intensity() const					{ return fIntensity; }
	bool ToggleLight() const				{ return bToggleLight; }
	
	int FetchLightTypeIndex();
	
	// Updates the light view projection matrix for this light
	void UpdateLVP();
	void Update();
	void SetLens(float fnear, float ffar);
	
	// When a light is created from the light manager it will build the mesh and render it
	void BuildSphereMesh(D3D* d3d);
	void RenderSphereMesh(ZShadeSandboxLighting::LightRenderParameters lrp);
	
	void UpdateMeshPosition(XMFLOAT3 pos);
	void ScaleMesh(XMFLOAT3 scale);
	void SetMesh(ZShadeSandboxMesh::CustomMesh* mesh);
	
	int TriangleCount() const;
	
	virtual Light* Clone() { return NULL; }
	
protected:
	
	bool bToggleLight;
	int mLightType;
	float fIntensity;
	XMFLOAT4 mDiffuseColor;
	XMFLOAT4 mAmbientColor;
	XMFLOAT4 mSpecularColor;
	XMFLOAT3 mPosition;
	XMFLOAT3 mDirection;
	
	// Camera for the perspective of this light
	LightCamera* mLightCamera;
	
	// Mesh that represents the position of the light source in the shape of a sphere
	ZShadeSandboxMesh::CustomMesh* mMesh;
};
}
//==============================================================================================================================
//==============================================================================================================================
#endif//__LIGHT_H