#include "DeferredShaderManager.h"
#include "AmbientLight.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include "PointLight.h"
#include "CapsuleLight.h"
#include "MeshRenderParameters.h"
#include "LightShadingBuffers.h"
using ZShadeSandboxLighting::DeferredShaderManager;
//==============================================================================================================================
//==============================================================================================================================
DeferredShaderManager* DeferredShaderManager::instance = 0;
int			  DeferredShaderManager::mCount = 0;
int			  DeferredShaderManager::mAmbientLightCount = 0;
int			  DeferredShaderManager::mDirectionLightCount = 0;
int			  DeferredShaderManager::mSpotLightCount = 0;
int			  DeferredShaderManager::mPointLightCount = 0;
int			  DeferredShaderManager::mCapsuleLightCount = 0;
//==============================================================================================================================
//==============================================================================================================================
DeferredShaderManager::DeferredShaderManager(D3D* d3d)
:   m_D3DSystem(d3d)
{
	mAmbientLightDeferredShader = new AmbientLightDeferredShader(m_D3DSystem);
	mDirectionalLightDeferredShader = new DirectionalLightDeferredShader(m_D3DSystem);
	mPointLightDeferredShader = new PointLightDeferredShader(m_D3DSystem);
	mCapsuleLightDeferredShader = new CapsuleLightDeferredShader(m_D3DSystem);
	mSpotLightDeferredShader = new SpotLightDeferredShader(m_D3DSystem);
	mDebugGBufferDeferredShader = new DebugGBufferDeferredShader(m_D3DSystem);
	
	bToggleDebugging = false;
	bToggleDebuggingColors = true;
	bToggleDebuggingNormals = false;
	bToggleDebuggingDepth = false;
	
	bToggleAmbientLights = true;
	bToggleDirectionalLights = true;
	bToggleSpotLights = true;
	bTogglePointLights = true;
	bToggleCapsuleLights = true;
}
//==============================================================================================================================
DeferredShaderManager::~DeferredShaderManager()
{
}
//==============================================================================================================================
void DeferredShaderManager::NewInstance(D3D* d3d)
{
	if (instance == 0)
		instance = new DeferredShaderManager(d3d);
}
//==============================================================================================================================
DeferredShaderManager* DeferredShaderManager::Instance()
{
	return instance;
}
//==============================================================================================================================
int DeferredShaderManager::LightCount()
{
	return DeferredShaderManager::mCount;
}
//==============================================================================================================================
int DeferredShaderManager::TriangleCount() const
{
	int triCount = 0;

	for (int i = 0; i < DeferredShaderManager::mCount; i++)
	{
		triCount += mLights[i]->TriangleCount();
	}

	return triCount;
}
//==============================================================================================================================
void DeferredShaderManager::AddLight(ZShadeSandboxLighting::Light* light)
{
	light->BuildSphereMesh(m_D3DSystem);

	mLights.push_back(light);
	DeferredShaderManager::mCount++;

	switch (light->LightType())
	{
	case ZShadeSandboxLighting::ELightType::eAmbient:
		DeferredShaderManager::mAmbientLightCount++;
		break;
	case ZShadeSandboxLighting::ELightType::eDirectional:
		DeferredShaderManager::mDirectionLightCount++;
		break;
	case ZShadeSandboxLighting::ELightType::eSpot:
		DeferredShaderManager::mSpotLightCount++;
		break;
	case ZShadeSandboxLighting::ELightType::ePoint:
		DeferredShaderManager::mPointLightCount++;
		break;
	case ZShadeSandboxLighting::ELightType::eCapsule:
		DeferredShaderManager::mCapsuleLightCount++;
		break;
	}
}
//==============================================================================================================================
void DeferredShaderManager::AddAmbientLight(XMFLOAT4 ambientColor, XMFLOAT3 position)
{
	ZShadeSandboxLighting::AmbientLight* light = new ZShadeSandboxLighting::AmbientLight();

	light->LightType() = ZShadeSandboxLighting::ELightType::eAmbient;
	light->AmbientColor() = ambientColor;
	light->Position() = position;

	light->BuildSphereMesh(m_D3DSystem);

	mLights.push_back(light);
	DeferredShaderManager::mCount++;

	DeferredShaderManager::mAmbientLightCount++;
}
//==============================================================================================================================
void DeferredShaderManager::AddDirectionalLight(XMFLOAT4 diffuseColor, XMFLOAT4 ambientColor, XMFLOAT3 position, XMFLOAT3 direction)
{
	ZShadeSandboxLighting::DirectionalLight* light = new ZShadeSandboxLighting::DirectionalLight();

	light->LightType() = ZShadeSandboxLighting::ELightType::eDirectional;
	light->DiffuseColor() = diffuseColor;
	light->AmbientColor() = ambientColor;
	light->Position() = position;
	light->Direction() = direction;

	light->BuildSphereMesh(m_D3DSystem);

	mLights.push_back(light);
	DeferredShaderManager::mCount++;

	DeferredShaderManager::mDirectionLightCount++;
}
//==============================================================================================================================
void DeferredShaderManager::AddSpotLight
(	XMFLOAT4 diffuseColor
,	XMFLOAT4 ambientColor
,	XMFLOAT3 position
,	XMFLOAT3 direction
,	float range
,	float spotAngle
,	XMFLOAT3 attenuation
)
{
	ZShadeSandboxLighting::SpotLight* light = new ZShadeSandboxLighting::SpotLight();

	light->LightType() = ZShadeSandboxLighting::ELightType::eSpot;
	light->DiffuseColor() = diffuseColor;
	light->AmbientColor() = ambientColor;
	light->Position() = position;
	light->Direction() = direction;
	light->Range() = range;
	light->SpotAngle() = spotAngle;
	light->Attenuation() = attenuation;

	light->BuildSphereMesh(m_D3DSystem);

	mLights.push_back(light);
	DeferredShaderManager::mCount++;

	DeferredShaderManager::mSpotLightCount++;
}
//==============================================================================================================================
void DeferredShaderManager::AddPointLight
(	XMFLOAT4 diffuseColor
,	XMFLOAT4 ambientColor
,	XMFLOAT3 position
,	XMFLOAT3 direction
,	float range
,	XMFLOAT3 attenuation
)
{
	ZShadeSandboxLighting::PointLight* light = new ZShadeSandboxLighting::PointLight();

	light->LightType() = ZShadeSandboxLighting::ELightType::ePoint;
	light->DiffuseColor() = diffuseColor;
	light->AmbientColor() = ambientColor;
	light->Position() = position;
	light->Direction() = direction;
	light->Range() = range;
	light->Attenuation() = attenuation;

	light->BuildSphereMesh(m_D3DSystem);

	mLights.push_back(light);
	DeferredShaderManager::mCount++;

	DeferredShaderManager::mPointLightCount++;
}
//==============================================================================================================================
void DeferredShaderManager::AddCapsuleLight
(	XMFLOAT4 diffuseColor
,	XMFLOAT4 ambientColor
,	XMFLOAT3 position
,	XMFLOAT3 direction
,	float range
,	float lightLength
,	float capsuleIntensity
,	XMFLOAT3 capsuleDirectionLength
)
{
	ZShadeSandboxLighting::CapsuleLight* light = new ZShadeSandboxLighting::CapsuleLight();

	light->LightType() = ZShadeSandboxLighting::ELightType::eCapsule;
	light->DiffuseColor() = diffuseColor;
	light->AmbientColor() = ambientColor;
	light->Position() = position;
	light->Direction() = direction;
	light->Range() = range;
	light->LightLength() = lightLength;
	light->Intensity() = capsuleIntensity;
	light->CapsuleDirectionLength() = capsuleDirectionLength;

	light->BuildSphereMesh(m_D3DSystem);

	mLights.push_back(light);
	DeferredShaderManager::mCount++;

	DeferredShaderManager::mCapsuleLightCount++;
}
//==============================================================================================================================
void DeferredShaderManager::RenderLightMesh(ZShadeSandboxMesh::MeshRenderParameters mrp)
{
	// No lights to render
	if (!bToggleAmbientLights && !bToggleDirectionalLights && !bToggleSpotLights && !bTogglePointLights && !bToggleCapsuleLights)
		return;

	for (int i = 0; i < DeferredShaderManager::mCount; i++)
	{
		// Can this light be seen
		if (!mLights[i]->ToggleLight())
			continue;
		
		int lightType = mLights[i]->LightType();

		if (!bToggleAmbientLights && lightType == ZShadeSandboxLighting::ELightType::eAmbient) continue;
		if (!bToggleDirectionalLights && lightType == ZShadeSandboxLighting::ELightType::eDirectional) continue;
		if (!bToggleSpotLights && lightType == ZShadeSandboxLighting::ELightType::eSpot) continue;
		if (!bTogglePointLights && lightType == ZShadeSandboxLighting::ELightType::ePoint) continue;
		if (!bToggleCapsuleLights && lightType == ZShadeSandboxLighting::ELightType::eCapsule) continue;

		mLights[i]->RenderSphereMesh(mrp);
	}
}
//==============================================================================================================================
void DeferredShaderManager::SetWireframe(bool wireframe)
{
	// Check to see if any lights are available to render
	if (!bToggleAmbientLights && !bToggleDirectionalLights && !bToggleSpotLights && !bTogglePointLights && !bToggleCapsuleLights)
		return;
	
	for (int i = 0; i < DeferredShaderManager::mCount; i++)
	{
		// Can this light be seen
		if (!mLights[i]->ToggleLight())
			continue;
		
		mLights[i]->SetWireframe(wireframe);
	}
}
//==============================================================================================================================
void DeferredShaderManager::ClearLights()
{
	DeferredShaderManager::mCount = 0;
	if (mLights.size() > 0) mLights.clear();
}
//==============================================================================================================================
void DeferredShaderManager::DeleteLight(int i)
{
	if (mLights[i] != NULL)
	{
		// This will need updated and testing

		mLights.erase(mLights.begin() + i);

		//// Shuffle all the lights downward from the deleted position
		//for (int j = 0; j < MAX_LIGHTS - 1; j++)
		//{
		//	if (mLights[j] == NULL)
		//	{
		//		mLights[j] = mLights[j + 1]->Clone();
		//		
		//		mLights.erase(mLights.begin() + (j + 1));

		//		mLights[j + 1] = NULL;
		//	}
		//}

		DeferredShaderManager::mCount--;
	}
}
//==============================================================================================================================
ZShadeSandboxLighting::AmbientLight* DeferredShaderManager::GetAmbientLight(int i)
{
	if (mLights[i] != NULL)
	{
		if (mLights[i]->LightType() == ZShadeSandboxLighting::ELightType::eAmbient)
		{
			return (ZShadeSandboxLighting::AmbientLight*)mLights[i];
		}
	}

	return NULL;
}
//==============================================================================================================================
ZShadeSandboxLighting::DirectionalLight* DeferredShaderManager::GetDirectionalLight(int i)
{
	if (mLights[i] != NULL)
	{
		if (mLights[i]->LightType() == ZShadeSandboxLighting::ELightType::eDirectional)
		{
			return (ZShadeSandboxLighting::DirectionalLight*)mLights[i];
		}
	}

	return NULL;
}
//==============================================================================================================================
ZShadeSandboxLighting::SpotLight* DeferredShaderManager::GetSpotLight(int i)
{
	if (mLights[i] != NULL)
	{
		if (mLights[i]->LightType() == ZShadeSandboxLighting::ELightType::eSpot)
		{
			return (ZShadeSandboxLighting::SpotLight*)mLights[i];
		}
	}

	return NULL;
}
//==============================================================================================================================
ZShadeSandboxLighting::PointLight* DeferredShaderManager::GetPointLight(int i)
{
	if (mLights[i] != NULL)
	{
		if (mLights[i]->LightType() == ZShadeSandboxLighting::ELightType::ePoint)
		{
			return (ZShadeSandboxLighting::PointLight*)mLights[i];
		}
	}

	return NULL;
}
//==============================================================================================================================
ZShadeSandboxLighting::CapsuleLight* DeferredShaderManager::GetCapsuleLight(int i)
{
	if (mLights[i] != NULL)
	{
		if (mLights[i]->LightType() == ZShadeSandboxLighting::ELightType::eCapsule)
		{
			return (ZShadeSandboxLighting::CapsuleLight*)mLights[i];
		}
	}

	return NULL;
}
//==============================================================================================================================
void DeferredShaderManager::Render(Camera* camera, bool wire)
{
	if (bToggleDebugging)
	{
		if (bToggleDebuggingColors)
		{
			mDebugGBufferDeferredShader->Render11(m_D3DSystem->GBufferColorTarget()->SRView);
		}
		
		if (bToggleDebuggingNormals)
		{
			mDebugGBufferDeferredShader->Render11(m_D3DSystem->GBufferNormalTarget()->SRView);
		}
		
		if (bToggleDebuggingDepth)
		{
			mDebugGBufferDeferredShader->RenderDepth11(m_D3DSystem->GBufferDepthTarget()->SRView, camera);
		}
	}
	else
	{
		m_D3DSystem->TurnOffZBuffer();
		m_D3DSystem->TurnOnAlphaBlending();
		
		for (int i = 0; i < mLights.size(); i++)
		{
			if (DeferredShaderManager::mAmbientLightCount > 0)
			{
				AmbientLight* al = GetAmbientLight(i);

				if (al != NULL)
				{
					mAmbientLightDeferredShader->Wireframe() = wire;
					mAmbientLightDeferredShader->Render11(
						camera,
						al,
						mAmbientUp,
						mAmbientDown,
						m_D3DSystem->GBufferColorTarget()->SRView,
						m_D3DSystem->GBufferNormalTarget()->SRView,
						m_D3DSystem->GBufferDepthTarget()->SRView
					);
				}
			}

			if (DeferredShaderManager::mDirectionLightCount > 0)
			{
				DirectionalLight* dl = GetDirectionalLight(i);

				if (dl != NULL)
				{
					mDirectionalLightDeferredShader->Wireframe() = wire;
					mDirectionalLightDeferredShader->Render11(
						camera,
						dl,
						m_D3DSystem->GBufferColorTarget()->SRView,
						m_D3DSystem->GBufferNormalTarget()->SRView,
						m_D3DSystem->GBufferDepthTarget()->SRView
					);
				}
			}

			if (DeferredShaderManager::mPointLightCount > 0)
			{
				PointLight* pl = GetPointLight(i);

				if (pl != NULL)
				{
					mPointLightDeferredShader->Wireframe() = wire;
					mPointLightDeferredShader->Render11(
						camera,
						pl,
						m_D3DSystem->GBufferColorTarget()->SRView,
						m_D3DSystem->GBufferNormalTarget()->SRView,
						m_D3DSystem->GBufferDepthTarget()->SRView
					);
				}
			}

			if (DeferredShaderManager::mSpotLightCount > 0)
			{
				SpotLight* sl = GetSpotLight(i);

				if (sl != NULL)
				{
					mSpotLightDeferredShader->Wireframe() = wire;
					mSpotLightDeferredShader->Render11(
						camera,
						sl,
						m_D3DSystem->GBufferColorTarget()->SRView,
						m_D3DSystem->GBufferNormalTarget()->SRView,
						m_D3DSystem->GBufferDepthTarget()->SRView
					);
				}
			}

			if (DeferredShaderManager::mCapsuleLightCount > 0)
			{
				CapsuleLight* cl = GetCapsuleLight(i);

				if (cl != NULL)
				{
					mCapsuleLightDeferredShader->Wireframe() = wire;
					mCapsuleLightDeferredShader->Render11(
						camera,
						cl,
						m_D3DSystem->GBufferColorTarget()->SRView,
						m_D3DSystem->GBufferNormalTarget()->SRView,
						m_D3DSystem->GBufferDepthTarget()->SRView
					);
				}
			}
		}
		m_D3DSystem->TurnOffAlphaBlending();
		m_D3DSystem->TurnOnZBuffer();
	}
}
//==============================================================================================================================