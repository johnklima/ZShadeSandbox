#include "LightManager.h"
#include "AmbientLight.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include "PointLight.h"
#include "CapsuleLight.h"
#include "LightShadingBuffers.h"
using ZShadeSandboxLighting::LightManager;
//==============================================================================================================================
//==============================================================================================================================
LightManager* LightManager::instance = 0;
int			  LightManager::mCount = 0;
int			  LightManager::mAmbientLightCount = 0;
int			  LightManager::mDirectionLightCount = 0;
int			  LightManager::mSpotLightCount = 0;
int			  LightManager::mPointLightCount = 0;
int			  LightManager::mCapsuleLightCount = 0;
//==============================================================================================================================
LightManager::LightManager(D3D* d3d)
:   m_D3DSystem(d3d)
{
	mLights.resize(MAX_LIGHTS);

	//mLightBuffer = new ZShadeSandboxLighting::LightBuffer();
	mSunLightBuffer = new ZShadeSandboxLighting::SunLightBuffer();
	
	bToggleAmbientLights = true;
	bToggleDirectionalLights = true;
	bToggleSpotLights = true;
	bTogglePointLights = true;
	bToggleCapsuleLights = true;
}
//==============================================================================================================================
LightManager::~LightManager()
{
}
//==============================================================================================================================
void LightManager::NewInstance(D3D* d3d)
{
	if (instance == 0)
		instance = new LightManager(d3d);
}
//==============================================================================================================================
LightManager* LightManager::Instance()
{
	return instance;
}
//==============================================================================================================================
/*void LightManager::BuildLightingBuffers(XMFLOAT3 ambientUp, XMFLOAT3 ambientDown, ZShadeSandboxLighting::SunLightBuffer buff)
{
	RebuildLightBuffer(ambientUp, ambientDown);
	RebuildSunBuffer(buff);
}
//==============================================================================================================================
void LightManager::RebuildLightBuffer(XMFLOAT3 ambientUp, XMFLOAT3 ambientDown)
{
	int ambientLightID = 0;
	int directionLightID = 0;
	int spotLightID = 0;
	int pointLightID = 0;
	int capsuleLightID = 0;
	
	// Get the lights in the scene
	for (int i = 0; i < ZShadeSandboxLighting::LightManager::mCount; i++)
	{
		ZShadeSandboxLighting::AmbientLight* ambientLight = GetAmbientLight(i);
		if (ambientLight != NULL)
		{
			ZShadeSandboxLighting::AmbientLightBuffer alb;
			alb.g_AmbientColor = ambientLight->AmbientColor();
			alb.g_Toggle = ambientLight->ToggleLight();
			mLightBuffer->g_AmbientLight[ambientLightID++] = alb;
		}

		ZShadeSandboxLighting::DirectionalLight* dirLight = GetDirectionalLight(i);
		if (dirLight != NULL)
		{
			ZShadeSandboxLighting::DirectionalLightBuffer dlb;
			dlb.g_LightDirection = dirLight->Direction();
			dlb.g_DiffuseColor = dirLight->DiffuseColor();
			dlb.g_AmbientColor = XMFLOAT4(0, 0, 0, 0);
			dlb.g_Toggle = dirLight->ToggleLight();
			dlb.g_Intensity = dirLight->Intensity();
			mLightBuffer->g_DirectionalLight[directionLightID++] = dlb;
		}

		ZShadeSandboxLighting::SpotLight* spotLight = GetSpotLight(i);
		if (spotLight != NULL)
		{
			ZShadeSandboxLighting::SpotLightBuffer slb;
			slb.g_DiffuseColor = spotLight->DiffuseColor();
			slb.g_LightPosition = spotLight->Position();
			slb.g_LightRange = spotLight->Range();
			slb.g_SpotCosOuterCone = spotLight->SpotCosOuterCone();
			slb.g_SpotInnerConeReciprocal = spotLight->SpotInnerConeReciprocal();
			slb.g_CosineAngle = spotLight->SpotAngle();
			slb.g_AmbientColor = XMFLOAT4(0, 0, 0, 0);
			slb.g_Toggle = spotLight->ToggleLight();
			slb.g_Intensity = spotLight->Intensity();
			mLightBuffer->g_SpotLight[spotLightID++] = slb;
		}

		ZShadeSandboxLighting::PointLight* pointLight = GetPointLight(i);
		if (pointLight != NULL)
		{
			ZShadeSandboxLighting::PointLightBuffer plb;
			plb.g_LightPosition = pointLight->Position();
			plb.g_LightRange = pointLight->Range();
			plb.g_Attenuation = XMFLOAT3(0, 0, 0);
			plb.g_AmbientColor = XMFLOAT4(0, 0, 0, 0);
			plb.g_DiffuseColor = pointLight->DiffuseColor();
			plb.g_Toggle = pointLight->ToggleLight();
			plb.g_Intensity = pointLight->Intensity();
			mLightBuffer->g_PointLight[pointLightID++] = plb;
		}

		ZShadeSandboxLighting::CapsuleLight* capsuleLight = GetCapsuleLight(i);
		if (capsuleLight != NULL)
		{
			ZShadeSandboxLighting::CapsuleLightBuffer clb;
			clb.g_LightPosition = capsuleLight->Position();
			clb.g_DiffuseColor = capsuleLight->DiffuseColor();
			clb.g_AmbientColor = capsuleLight->AmbientColor();
			clb.g_LightRange = capsuleLight->Range();
			clb.g_LightLength = capsuleLight->LightLength();
			clb.g_CapsuleIntensity = capsuleLight->Intensity();
			clb.g_CapsuleDirectionLength = capsuleLight->CapsuleDirectionLength();
			clb.g_LightDirection = capsuleLight->Direction();
			clb.g_Toggle = capsuleLight->ToggleLight();
			mLightBuffer->g_CapsuleLight[capsuleLightID++] = clb;
		}
	}
	
	mLightBuffer->g_AmbientDown = ambientDown;
	mLightBuffer->g_AmbientLightCount = ZShadeSandboxLighting::LightManager::Instance()->AmbientLightCount();
	mLightBuffer->g_DirectionalLightCount = ZShadeSandboxLighting::LightManager::Instance()->DirectionalLightCount();
	mLightBuffer->g_SpotLightCount = ZShadeSandboxLighting::LightManager::Instance()->SpotLightCount();
	mLightBuffer->g_PointLightCount = ZShadeSandboxLighting::LightManager::Instance()->PointLightCount();
	mLightBuffer->g_CapsuleLightCount = ZShadeSandboxLighting::LightManager::Instance()->CapsuleLightCount();
	mLightBuffer->g_AmbientUp = ambientUp;

	for (int i = 0; i < LightManager::mCount; i++)
	{
		mLights[i]->AddMeshLightBuffer(mLightBuffer);
	}
}*/
//==============================================================================================================================
void LightManager::RebuildSunBuffer(ZShadeSandboxLighting::SunLightBuffer buff)
{
	mSunLightBuffer->g_SunDir = buff.g_SunDir;
	mSunLightBuffer->g_SunDiffuseColor = buff.g_SunDiffuseColor;
	mSunLightBuffer->g_SunShineness = buff.g_SunShineness;
	mSunLightBuffer->g_EnableSun = buff.g_EnableSun;
	mSunLightBuffer->g_SunIntensity = buff.g_SunIntensity;

	//for (int i = 0; i < LightManager::mCount; i++)
	//{
	//	mLights[i]->AddMeshLightBuffer(mSunLightBuffer);
	//}
}
//==============================================================================================================================
void LightManager::BuildFinalLightBuffers
(	ID3D11Buffer*& lightBuffer
,	ID3D11Buffer*& sunLightBuffer
)
{
	ZShadeSandboxLighting::cbLightBuffer cLB;
	ZShadeSandboxLighting::cbSunLightBuffer cSLB;
	
	ZShadeSandboxLighting::cbAmbientLightBuffer alb;
	ZShadeSandboxLighting::cbDirectionalLightBuffer dlb;
	ZShadeSandboxLighting::cbSpotLightBuffer slb;
	ZShadeSandboxLighting::cbPointLightBuffer plb;
	ZShadeSandboxLighting::cbCapsuleLightBuffer clb;

	for (int i = 0; i < ZShadeSandboxLighting::LightManager::mAmbientLightCount; i++)
	{
		ZShadeSandboxLighting::AmbientLight* ambientLight = GetAmbientLight(i);
		if (ambientLight != NULL)
		{
			ZeroMemory(&alb, sizeof(ZShadeSandboxLighting::cbAmbientLightBuffer));
			alb.g_AmbientColor = ambientLight->AmbientColor();
			alb.ambientpadding = XMFLOAT3(0, 0, 0);
			alb.g_Toggle = ambientLight->ToggleLight();
			cLB.g_AmbientLight[i] = alb;
		}
	}
	for (int i = 0; i < ZShadeSandboxLighting::LightManager::mDirectionLightCount; i++)
	{
		ZShadeSandboxLighting::DirectionalLight* dirLight = GetDirectionalLight(i);
		if (dirLight != NULL)
		{
			ZeroMemory(&dlb, sizeof(ZShadeSandboxLighting::cbDirectionalLightBuffer));
			dlb.g_Direction = dirLight->Direction();
			dlb.g_Ambient = dirLight->AmbientColor();
			dlb.g_Diffuse = dirLight->DiffuseColor();
			dlb.directionalpadding = XMFLOAT3(0, 0, 0);
			dlb.g_Toggle = dirLight->ToggleLight();
			dlb.g_Intensity = dirLight->Intensity();
			cLB.g_DirectionalLight[i] = dlb;
		}
	}
	for (int i = 0; i < ZShadeSandboxLighting::LightManager::mSpotLightCount; i++)
	{
		ZShadeSandboxLighting::SpotLight* spotLight = GetSpotLight(i);
		if (spotLight != NULL)
		{
			ZeroMemory(&slb, sizeof(ZShadeSandboxLighting::cbSpotLightBuffer));
			slb.g_AmbientColor = spotLight->AmbientColor();
			slb.g_DiffuseColor = spotLight->DiffuseColor();
			slb.g_LightPosition = spotLight->Position();
			slb.g_LightRange = spotLight->Range();
			slb.g_SpotCosOuterCone = spotLight->SpotCosOuterCone();
			slb.g_SpotInnerConeReciprocal = spotLight->SpotInnerConeReciprocal();
			slb.g_CosineAngle = spotLight->SpotAngle();
			slb.spotpadding = XMFLOAT3(0, 0, 0);
			slb.g_Toggle = spotLight->ToggleLight();
			slb.g_Intensity = spotLight->Intensity();
			cLB.g_SpotLight[i] = slb;
		}
	}
	for (int i = 0; i < ZShadeSandboxLighting::LightManager::mPointLightCount; i++)
	{
		ZShadeSandboxLighting::PointLight* pointLight = GetPointLight(i);
		if (pointLight != NULL)
		{
			ZeroMemory(&plb, sizeof(ZShadeSandboxLighting::cbPointLightBuffer));
			plb.g_LightPosition = pointLight->Position();
			plb.g_LightRange = pointLight->Range();
			plb.g_Attenuation = pointLight->Attenuation();
			plb.g_AmbientColor = pointLight->AmbientColor();
			plb.g_DiffuseColor = pointLight->DiffuseColor();
			plb.pointpadding = XMFLOAT3(0, 0, 0);
			plb.g_Toggle = pointLight->ToggleLight();
			plb.g_Intensity = pointLight->Intensity();
			cLB.g_PointLight[i] = plb;
		}
	}
	for (int i = 0; i < ZShadeSandboxLighting::LightManager::mCapsuleLightCount; i++)
	{
		ZShadeSandboxLighting::CapsuleLight* capsuleLight = GetCapsuleLight(i);
		if (capsuleLight != NULL)
		{
			ZeroMemory(&clb, sizeof(ZShadeSandboxLighting::cbCapsuleLightBuffer));
			clb.g_LightPosition = capsuleLight->Position();
			clb.g_LightRange = capsuleLight->Range();
			clb.g_LightDirection = capsuleLight->Direction();
			clb.g_LightLength = capsuleLight->LightLength();
			clb.g_CapsuleDirectionLength = capsuleLight->CapsuleDirectionLength();
			clb.g_CapsuleIntensity = capsuleLight->Intensity();
			clb.g_AmbientColor = capsuleLight->AmbientColor();
			clb.g_DiffuseColor = capsuleLight->DiffuseColor();
			clb.capsulepadding = XMFLOAT3(0, 0, 0);
			clb.g_Toggle = capsuleLight->ToggleLight();
			cLB.g_CapsuleLight[i] = clb;
		}
	}

	cLB.g_AmbientLightCount = ZShadeSandboxLighting::LightManager::Instance()->AmbientLightCount();
	cLB.g_DirectionalLightCount = ZShadeSandboxLighting::LightManager::Instance()->DirectionalLightCount();
	cLB.g_SpotLightCount = ZShadeSandboxLighting::LightManager::Instance()->SpotLightCount();
	cLB.g_PointLightCount = ZShadeSandboxLighting::LightManager::Instance()->PointLightCount();
	cLB.g_CapsuleLightCount = ZShadeSandboxLighting::LightManager::Instance()->CapsuleLightCount();
	
	cLB.g_AmbientDown = ambientDown;
	cLB.g_AmbientUp = ambientUp;
	cLB.padding = 0;
	
	// Map the light constants
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res2;
		m_D3DSystem->GetDeviceContext()->Map(lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res2);
		{
			assert(mapped_res2.pData);
			*(ZShadeSandboxLighting::cbLightBuffer*)mapped_res2.pData = cLB;
		}
		m_D3DSystem->GetDeviceContext()->Unmap(lightBuffer, 0);
	}
	
	cSLB.g_SunDir = mSunLightBuffer->g_SunDir;
	cSLB.g_EnableSun = mSunLightBuffer->g_EnableSun;
	cSLB.g_SunDiffuseColor = mSunLightBuffer->g_SunDiffuseColor;
	cSLB.g_SunShineness = mSunLightBuffer->g_SunShineness;
	cSLB.g_SunIntensity = mSunLightBuffer->g_SunIntensity;
	
	// Map the sun light constants
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res2;
		m_D3DSystem->GetDeviceContext()->Map(sunLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res2);
		{
			assert(mapped_res2.pData);
			*(ZShadeSandboxLighting::cbSunLightBuffer*)mapped_res2.pData = cSLB;
		}
		m_D3DSystem->GetDeviceContext()->Unmap(sunLightBuffer, 0);
	}
}
//==============================================================================================================================
void LightManager::BuildAmbientLightBuffer(ID3D11Buffer*& buffer, ZShadeSandboxLighting::Light* light)
{
	if (light->LightType() == ZShadeSandboxLighting::ELightType::eAmbient)
	{
		ZShadeSandboxLighting::AmbientLight* ambientLight = (ZShadeSandboxLighting::AmbientLight*)light;
		
		ZShadeSandboxLighting::cbAmbientLightBuffer alb;
		alb.g_AmbientColor = ambientLight->AmbientColor();
		alb.ambientpadding = XMFLOAT3(0, 0, 0);
		alb.g_Toggle = ambientLight->ToggleLight();
		// Map the light constants
		{
			D3D11_MAPPED_SUBRESOURCE mapped_res2;
			m_D3DSystem->GetDeviceContext()->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res2);
			{
				assert(mapped_res2.pData);
				*(ZShadeSandboxLighting::cbAmbientLightBuffer*)mapped_res2.pData = alb;
			}
			m_D3DSystem->GetDeviceContext()->Unmap(buffer, 0);
		}
	}
}
//==============================================================================================================================
void LightManager::BuildDirectionalLightBuffer(ID3D11Buffer*& buffer, ZShadeSandboxLighting::Light* light)
{
	if (light->LightType() == ZShadeSandboxLighting::ELightType::eDirectional)
	{
		ZShadeSandboxLighting::DirectionalLight* directionalLight = (ZShadeSandboxLighting::DirectionalLight*)light;
		
		ZShadeSandboxLighting::cbDirectionalLightBuffer dlb;
		dlb.g_Direction = directionalLight->Direction();
		dlb.g_Intensity = directionalLight->Intensity();
		dlb.g_Ambient = directionalLight->AmbientColor();
		dlb.g_Diffuse = directionalLight->DiffuseColor();
		dlb.directionalpadding = XMFLOAT3(0, 0, 0);
		dlb.g_Toggle = directionalLight->ToggleLight();
		// Map the light constants
		{
			D3D11_MAPPED_SUBRESOURCE mapped_res2;
			m_D3DSystem->GetDeviceContext()->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res2);
			{
				assert(mapped_res2.pData);
				*(ZShadeSandboxLighting::cbDirectionalLightBuffer*)mapped_res2.pData = dlb;
			}
			m_D3DSystem->GetDeviceContext()->Unmap(buffer, 0);
		}
	}
}
//==============================================================================================================================
void LightManager::BuildPointLightBuffer(ID3D11Buffer*& buffer, ZShadeSandboxLighting::Light* light)
{
	if (light->LightType() == ZShadeSandboxLighting::ELightType::ePoint)
	{
		ZShadeSandboxLighting::PointLight* pointLight = (ZShadeSandboxLighting::PointLight*)light;
		
		ZShadeSandboxLighting::cbPointLightBuffer plb;
		plb.g_LightPosition = pointLight->Position();
		plb.g_LightRange = pointLight->Range();
		plb.g_Attenuation = pointLight->Attenuation();
		plb.g_Intensity = pointLight->Intensity();
		plb.g_AmbientColor = pointLight->AmbientColor();
		plb.g_DiffuseColor = pointLight->DiffuseColor();
		plb.pointpadding = XMFLOAT3(0, 0, 0);
		plb.g_Toggle = pointLight->ToggleLight();
		// Map the light constants
		{
			D3D11_MAPPED_SUBRESOURCE mapped_res2;
			m_D3DSystem->GetDeviceContext()->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res2);
			{
				assert(mapped_res2.pData);
				*(ZShadeSandboxLighting::cbPointLightBuffer*)mapped_res2.pData = plb;
			}
			m_D3DSystem->GetDeviceContext()->Unmap(buffer, 0);
		}
	}
}
//==============================================================================================================================
void LightManager::BuildSpotLightBuffer(ID3D11Buffer*& buffer, ZShadeSandboxLighting::Light* light)
{
	if (light->LightType() == ZShadeSandboxLighting::ELightType::eSpot)
	{
		ZShadeSandboxLighting::SpotLight* spotLight = (ZShadeSandboxLighting::SpotLight*)light;
		
		ZShadeSandboxLighting::cbSpotLightBuffer slb;
		slb.g_AmbientColor = spotLight->AmbientColor();
		slb.g_DiffuseColor = spotLight->DiffuseColor();
		slb.g_LightPosition = spotLight->Position();
		slb.g_Intensity = spotLight->Intensity();
		slb.g_LightRange = spotLight->Range();
		slb.g_SpotCosOuterCone = spotLight->SpotCosOuterCone();
		slb.g_SpotInnerConeReciprocal = spotLight->SpotInnerConeReciprocal();
		slb.g_CosineAngle = spotLight->SpotAngle();
		slb.spotpadding = XMFLOAT3(0, 0, 0);
		slb.g_Toggle = spotLight->ToggleLight();
		// Map the light constants
		{
			D3D11_MAPPED_SUBRESOURCE mapped_res2;
			m_D3DSystem->GetDeviceContext()->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res2);
			{
				assert(mapped_res2.pData);
				*(ZShadeSandboxLighting::cbSpotLightBuffer*)mapped_res2.pData = slb;
			}
			m_D3DSystem->GetDeviceContext()->Unmap(buffer, 0);
		}
	}
}
//==============================================================================================================================
void LightManager::BuildCapsuleLightBuffer(ID3D11Buffer*& buffer, ZShadeSandboxLighting::Light* light)
{
	if (light->LightType() == ZShadeSandboxLighting::ELightType::eCapsule)
	{
		ZShadeSandboxLighting::CapsuleLight* capsuleLight = (ZShadeSandboxLighting::CapsuleLight*)light;
		
		ZShadeSandboxLighting::cbCapsuleLightBuffer clb;
		clb.g_LightPosition = capsuleLight->Position();
		clb.g_LightRange = capsuleLight->Range();
		clb.g_LightDirection = capsuleLight->Direction();
		clb.g_LightLength = capsuleLight->LightLength();
		clb.g_CapsuleDirectionLength = capsuleLight->CapsuleDirectionLength();
		clb.g_CapsuleIntensity = capsuleLight->Intensity();
		clb.g_AmbientColor = capsuleLight->AmbientColor();
		clb.g_DiffuseColor = capsuleLight->DiffuseColor();
		clb.capsulepadding = XMFLOAT3(0, 0, 0);
		clb.g_Toggle = capsuleLight->ToggleLight();
		// Map the light constants
		{
			D3D11_MAPPED_SUBRESOURCE mapped_res2;
			m_D3DSystem->GetDeviceContext()->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res2);
			{
				assert(mapped_res2.pData);
				*(ZShadeSandboxLighting::cbCapsuleLightBuffer*)mapped_res2.pData = clb;
			}
			m_D3DSystem->GetDeviceContext()->Unmap(buffer, 0);
		}
	}
}
//==============================================================================================================================
void LightManager::ToggleAmbientLights(bool toggle)
{
	bToggleAmbientLights = toggle;
}
//==============================================================================================================================
void LightManager::ToggleDirectionalLights(bool toggle)
{
	bToggleDirectionalLights = toggle;
}
//==============================================================================================================================
void LightManager::ToggleSpotLights(bool toggle)
{
	bToggleSpotLights = toggle;
}
//==============================================================================================================================
void LightManager::TogglePointLights(bool toggle)
{
	bTogglePointLights = toggle;
}
//==============================================================================================================================
void LightManager::ToggleCapsuleLights(bool toggle)
{
	bToggleCapsuleLights = toggle;
}
//==============================================================================================================================
int LightManager::LightCount()
{
	return LightManager::mCount;
}
//==============================================================================================================================
int LightManager::TriangleCount() const
{
	int triCount = 0;
	
	for (int i = 0; i < LightManager::mCount; i++)
	{
		triCount += mLights[i]->TriangleCount();
	}
	
	return triCount;
}
//==============================================================================================================================
void LightManager::AddLight(ZShadeSandboxLighting::Light* light)
{
	light->BuildSphereMesh(m_D3DSystem);
	mLights[LightManager::mCount++] = light;

	switch (light->LightType())
	{
		case ZShadeSandboxLighting::ELightType::eAmbient:
			LightManager::mAmbientLightCount++;
		break;
		case ZShadeSandboxLighting::ELightType::eDirectional:
			LightManager::mDirectionLightCount++;
		break;
		case ZShadeSandboxLighting::ELightType::eSpot:
			LightManager::mSpotLightCount++;
		break;
		case ZShadeSandboxLighting::ELightType::ePoint:
			LightManager::mPointLightCount++;
		break;
		case ZShadeSandboxLighting::ELightType::eCapsule:
			LightManager::mCapsuleLightCount++;
		break;
	}
}
//==============================================================================================================================
void LightManager::AddAmbientLight(XMFLOAT4 ambientColor, XMFLOAT3 position)
{
	ZShadeSandboxLighting::AmbientLight* light = new ZShadeSandboxLighting::AmbientLight();
	
	light->LightType() = ZShadeSandboxLighting::ELightType::eAmbient;
	light->AmbientColor() = ambientColor;
	light->Position() = position;
	
	light->BuildSphereMesh(m_D3DSystem);
	
	mLights[LightManager::mCount++] = light;

	LightManager::mAmbientLightCount++;
}
//==============================================================================================================================
void LightManager::AddDirectionalLight
(	XMFLOAT4 diffuseColor
,	XMFLOAT4 ambientColor
,	XMFLOAT3 position
,	XMFLOAT3 direction
,	float intensity
)
{
	ZShadeSandboxLighting::DirectionalLight* light = new ZShadeSandboxLighting::DirectionalLight();
	
	light->LightType() = ZShadeSandboxLighting::ELightType::eDirectional;
	light->DiffuseColor() = diffuseColor;
	light->AmbientColor() = ambientColor;
	light->Position() = position;
	light->Direction() = direction;
	light->Intensity() = intensity;
	
	light->BuildSphereMesh(m_D3DSystem);
	
	mLights[LightManager::mCount++] = light;

	LightManager::mDirectionLightCount++;
}
//==============================================================================================================================
void LightManager::AddSpotLight
(	XMFLOAT4 diffuseColor
, 	XMFLOAT4 ambientColor
, 	XMFLOAT3 position
, 	XMFLOAT3 direction
,	float range
, 	float spotAngle
, 	XMFLOAT3 attenuation
,	float intensity
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
	light->Intensity() = intensity;
	
	light->BuildSphereMesh(m_D3DSystem);
	
	mLights[LightManager::mCount++] = light;

	LightManager::mSpotLightCount++;
}
//==============================================================================================================================
void LightManager::AddPointLight
(	XMFLOAT4 diffuseColor
, 	XMFLOAT4 ambientColor
, 	XMFLOAT3 position
, 	XMFLOAT3 direction
,	float range
, 	XMFLOAT3 attenuation
,	float intensity
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
	light->Intensity() = intensity;
	
	light->BuildSphereMesh(m_D3DSystem);
	
	mLights[LightManager::mCount++] = light;

	LightManager::mPointLightCount++;
}
//==============================================================================================================================
void LightManager::AddCapsuleLight
(	XMFLOAT4 diffuseColor
, 	XMFLOAT4 ambientColor
, 	XMFLOAT3 position
, 	XMFLOAT3 direction
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
	
	mLights[LightManager::mCount++] = light;

	LightManager::mCapsuleLightCount++;
}
//==============================================================================================================================
void LightManager::RenderLightMesh(ZShadeSandboxLighting::LightRenderParameters lrp)
{
	// Check to see if any lights are available to render
	if (!bToggleAmbientLights && !bToggleDirectionalLights && !bToggleSpotLights && !bTogglePointLights && !bToggleCapsuleLights)
		return;
	
	for (int i = 0; i < LightManager::mCount; i++)
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
		
		mLights[i]->RenderSphereMesh(lrp);
	}
}
//==============================================================================================================================
void LightManager::ClearLights()
{
	LightManager::mCount = 0;
	mLights.resize(0);
	
	// After it is cleared reinitialize
	mLights.resize(MAX_LIGHTS);
}
//==============================================================================================================================
void LightManager::DeleteLight(int i)
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
		
		LightManager::mCount--;
	}
}
//==============================================================================================================================
ZShadeSandboxLighting::AmbientLight* LightManager::GetAmbientLight(int i)
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
ZShadeSandboxLighting::DirectionalLight* LightManager::GetDirectionalLight(int i)
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
ZShadeSandboxLighting::SpotLight* LightManager::GetSpotLight(int i)
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
ZShadeSandboxLighting::PointLight* LightManager::GetPointLight(int i)
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
ZShadeSandboxLighting::CapsuleLight* LightManager::GetCapsuleLight(int i)
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