//=====================================================================================
// LightManager.h
//
//==============================================================================================================================
// History
//
// -Created on 4/17/2014 by Dustin Watson
//=====================================================================================
#ifndef __LIGHTMANAGER_H
#define __LIGHTMANAGER_H
//==============================================================================================================================
//==============================================================================================================================

//
// Includes
//

#include "D3D.h"
#include "Light.h"

//==============================================================================================================================
//==============================================================================================================================

//
// Defines
//

// Maximum number of lights that can be rendered at one time in the scene
#define MAX_LIGHTS 16

//==============================================================================================================================
//==============================================================================================================================

//
// Forward Declarations
//

namespace ZShadeSandboxLighting {
	class AmbientLight;
	class CapsuleLight;
	class DirectionalLight;
	class SpotLight;
	class PointLight;
	struct LightRenderParameters;
	struct LightBuffer;
	struct SunLightBuffer;
	struct cbLightBuffer;
	struct cbSunLightBuffer;
}

//==============================================================================================================================
//==============================================================================================================================
namespace ZShadeSandboxLighting {
class LightManager
{
public:
	
	LightManager(D3D* d3d);
	~LightManager();
	
	void AddLight(ZShadeSandboxLighting::Light* light);
	
	// Adds a ambient light
	void AddAmbientLight(XMFLOAT4 ambientColor, XMFLOAT3 position);
	
	// Adds a directional light
	void AddDirectionalLight
	(	XMFLOAT4 diffuseColor
	,	XMFLOAT4 ambientColor
	,	XMFLOAT3 position
	,	XMFLOAT3 direction
	,	float intensity
	);
	
	// Add a spot light
	void AddSpotLight
	(	XMFLOAT4 diffuseColor
	, 	XMFLOAT4 ambientColor
	, 	XMFLOAT3 position
	, 	XMFLOAT3 direction
	,	float range
	, 	float spotAngle
	, 	XMFLOAT3 attenuation
	,	float intensity
	);
	
	// Adds a point light
	void AddPointLight
	(	XMFLOAT4 diffuseColor
	, 	XMFLOAT4 ambientColor
	, 	XMFLOAT3 position
	, 	XMFLOAT3 direction
	,	float range
	, 	XMFLOAT3 attenuation
	,	float intensity
	);
	
	// Adds a capsule light
	void AddCapsuleLight
	(	XMFLOAT4 diffuseColor
	, 	XMFLOAT4 ambientColor
	, 	XMFLOAT3 position
	, 	XMFLOAT3 direction
	,	float range
	,	float lightLength
	,	float capsuleIntensity
	,	XMFLOAT3 capsuleDirectionLength
	);
	
	// returns the amount of lights in the scene
	int LightCount();
	
	int TriangleCount() const;
	
	int AmbientLightCount() const		{ return LightManager::mAmbientLightCount; }
	int DirectionalLightCount() const   { return LightManager::mDirectionLightCount; }
	int SpotLightCount() const			{ return LightManager::mSpotLightCount; }
	int PointLightCount() const			{ return LightManager::mPointLightCount; }
	int CapsuleLightCount() const		{ return LightManager::mCapsuleLightCount; }

	void ClearLights();
	void DeleteLight(int i);
	
	ZShadeSandboxLighting::AmbientLight* GetAmbientLight(int i);
	ZShadeSandboxLighting::DirectionalLight* GetDirectionalLight(int i);
	ZShadeSandboxLighting::SpotLight* GetSpotLight(int i);
	ZShadeSandboxLighting::PointLight* GetPointLight(int i);
	ZShadeSandboxLighting::CapsuleLight* GetCapsuleLight(int i);
	
	//ZShadeSandboxLighting::LightBuffer* GetLightBuffer() { return mLightBuffer; }
	//ZShadeSandboxLighting::SunLightBuffer* GetSunLightBuffer() { return mSunLightBuffer; }
	
	void ToggleAmbientLights(bool toggle);
	void ToggleDirectionalLights(bool toggle);
	void ToggleSpotLights(bool toggle);
	void TogglePointLights(bool toggle);
	void ToggleCapsuleLights(bool toggle);
	
	XMFLOAT3& AmbientUp() { return ambientUp; }
	XMFLOAT3& AmbientDown() { return ambientDown; }
	
	//void RebuildLightBuffer(XMFLOAT3 ambientUp, XMFLOAT3 ambientDown);
	void RebuildSunBuffer(SunLightBuffer buff);
	//void BuildLightingBuffers(XMFLOAT3 ambientUp, XMFLOAT3 ambientDown, SunLightBuffer buff);
	
	void BuildFinalLightBuffers(
		ID3D11Buffer*& lightBuffer,
		ID3D11Buffer*& sunLightBuffer
	);
	
	void BuildAmbientLightBuffer(ID3D11Buffer*& buffer, ZShadeSandboxLighting::Light* light);
	void BuildDirectionalLightBuffer(ID3D11Buffer*& buffer, ZShadeSandboxLighting::Light* light);
	void BuildPointLightBuffer(ID3D11Buffer*& buffer, ZShadeSandboxLighting::Light* light);
	void BuildSpotLightBuffer(ID3D11Buffer*& buffer, ZShadeSandboxLighting::Light* light);
	void BuildCapsuleLightBuffer(ID3D11Buffer*& buffer, ZShadeSandboxLighting::Light* light);
	
	static void NewInstance(D3D* d3d);
	static LightManager* Instance();
	
public:
	
	// Renders a mesh at the position of the light for debugging
	void RenderLightMesh(ZShadeSandboxLighting::LightRenderParameters lrp);
	
private:
	
	//ZShadeSandboxLighting::LightBuffer* mLightBuffer;
	ZShadeSandboxLighting::SunLightBuffer* mSunLightBuffer;
	
	XMFLOAT3 ambientUp;
	XMFLOAT3 ambientDown;
	
	// Toggle lights based on type
	bool bToggleAmbientLights;
	bool bToggleDirectionalLights;
	bool bToggleSpotLights;
	bool bTogglePointLights;
	bool bToggleCapsuleLights;
	
	static LightManager* instance;
	static int mCount;
	static int mAmbientLightCount;
	static int mDirectionLightCount;
	static int mSpotLightCount;
	static int mPointLightCount;
	static int mCapsuleLightCount;
	D3D* m_D3DSystem;
	vector<ZShadeSandboxLighting::Light*> mLights;
};
}
//==============================================================================================================================
//==============================================================================================================================
#endif//__LIGHTMANAGER_H