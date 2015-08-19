//==============================================================================================================================
// DeferredShaderManager.h
//
//==============================================================================================================================
// History
//
// -Created on 6/7/2015  by Dustin Watson
//==============================================================================================================================
#ifndef __DEFERREDSHADERMANAGER_H
#define __DEFERREDSHADERMANAGER_H
//==============================================================================================================================
//==============================================================================================================================

//
// Includes
//

#include "D3D.h"
#include "Camera.h"
#include "ZMath.h"
#include "Light.h"
#include "AmbientLightDeferredShader.h"
#include "DirectionalLightDeferredShader.h"
#include "PointLightDeferredShader.h"
#include "CapsuleLightDeferredShader.h"
#include "SpotLightDeferredShader.h"
#include "DebugGBufferDeferredShader.h"

namespace ZShadeSandboxMesh {
	class MeshRenderParameters;
}

namespace ZShadeSandboxLighting {
	class AmbientLight;
	class CapsuleLight;
	class DirectionalLight;
	class SpotLight;
	class PointLight;
}

//==============================================================================================================================
//==============================================================================================================================
namespace ZShadeSandboxLighting {
class DeferredShaderManager
{
public:

	DeferredShaderManager(D3D* d3d);
	~DeferredShaderManager();

	void AddLight(ZShadeSandboxLighting::Light* light);

	// Adds a ambient light
	void AddAmbientLight(XMFLOAT4 ambientColor, XMFLOAT3 position);

	// Adds a directional light
	void AddDirectionalLight(XMFLOAT4 diffuseColor, XMFLOAT4 ambientColor, XMFLOAT3 position, XMFLOAT3 direction);

	// Add a spot light
	void AddSpotLight
	(	XMFLOAT4 diffuseColor
	,	XMFLOAT4 ambientColor
	,	XMFLOAT3 position
	,	XMFLOAT3 direction
	,	float range
	,	float spotAngle
	,	XMFLOAT3 attenuation
	);

	// Adds a point light
	void AddPointLight
	(	XMFLOAT4 diffuseColor
	,	XMFLOAT4 ambientColor
	,	XMFLOAT3 position
	,	XMFLOAT3 direction
	,	float range
	,	XMFLOAT3 attenuation
	);

	// Adds a capsule light
	void AddCapsuleLight
	(	XMFLOAT4 diffuseColor
	,	XMFLOAT4 ambientColor
	,	XMFLOAT3 position
	,	XMFLOAT3 direction
	,	float range
	,	float lightLength
	,	float capsuleIntensity
	,	XMFLOAT3 capsuleDirectionLength
	);

	// returns the amount of lights in the scene
	int LightCount();

	int TriangleCount() const;

	int AmbientLightCount() const		{ return DeferredShaderManager::mAmbientLightCount; }
	int DirectionalLightCount() const   { return DeferredShaderManager::mDirectionLightCount; }
	int SpotLightCount() const			{ return DeferredShaderManager::mSpotLightCount; }
	int PointLightCount() const			{ return DeferredShaderManager::mPointLightCount; }
	int CapsuleLightCount() const		{ return DeferredShaderManager::mCapsuleLightCount; }

	void ClearLights();
	void DeleteLight(int i);

	void SetAmbientDown(XMFLOAT3 value) { mAmbientDown = value; }
	void SetAmbientUp(XMFLOAT3 value) { mAmbientUp = value; }

	ZShadeSandboxLighting::AmbientLight* GetAmbientLight(int i);
	ZShadeSandboxLighting::DirectionalLight* GetDirectionalLight(int i);
	ZShadeSandboxLighting::SpotLight* GetSpotLight(int i);
	ZShadeSandboxLighting::PointLight* GetPointLight(int i);
	ZShadeSandboxLighting::CapsuleLight* GetCapsuleLight(int i);
	
	bool& ToggleDebugging()			{ return bToggleDebugging; }
	bool& ToggleDebuggingColors()	{ return bToggleDebuggingColors; }
	bool& ToggleDebuggingNormals()	{ return bToggleDebuggingNormals; }
	bool& ToggleDebuggingDepth()	{ return bToggleDebuggingDepth; }
	
	bool& ToggleAmbientLights()		{ return bToggleAmbientLights; }
	bool& ToggleDirectionalLights() { return bToggleDirectionalLights; }
	bool& ToggleSpotLights()		{ return bToggleSpotLights; }
	bool& TogglePointLights()		{ return bTogglePointLights; }
	bool& ToggleCapsuleLights()		{ return bToggleCapsuleLights; }
	
	static void NewInstance(D3D* d3d);
	static DeferredShaderManager* Instance();

public:

	void Render(Camera* camera, bool wire);

	// Renders a mesh at the position of the light for debugging
	void RenderLightMesh(ZShadeSandboxMesh::MeshRenderParameters mrp);
	
	void SetWireframe(bool wireframe);
	
private:
	
	// Debugging flags
	bool bToggleDebugging;
	bool bToggleDebuggingColors;
	bool bToggleDebuggingNormals;
	bool bToggleDebuggingDepth;
	
	// Toggle lights based on type
	bool bToggleAmbientLights;
	bool bToggleDirectionalLights;
	bool bToggleSpotLights;
	bool bTogglePointLights;
	bool bToggleCapsuleLights;

	XMFLOAT3 mAmbientDown;
	XMFLOAT3 mAmbientUp;

	static DeferredShaderManager* instance;
	static int mCount;
	static int mAmbientLightCount;
	static int mDirectionLightCount;
	static int mSpotLightCount;
	static int mPointLightCount;
	static int mCapsuleLightCount;
	
	D3D* m_D3DSystem;

	AmbientLightDeferredShader* mAmbientLightDeferredShader;
	DirectionalLightDeferredShader* mDirectionalLightDeferredShader;
	PointLightDeferredShader* mPointLightDeferredShader;
	CapsuleLightDeferredShader* mCapsuleLightDeferredShader;
	SpotLightDeferredShader* mSpotLightDeferredShader;
	DebugGBufferDeferredShader* mDebugGBufferDeferredShader;
	
	vector<ZShadeSandboxLighting::Light*> mLights;
};
}
//==============================================================================================================================
//==============================================================================================================================
#endif//__DEFERREDSHADERMANAGER_H