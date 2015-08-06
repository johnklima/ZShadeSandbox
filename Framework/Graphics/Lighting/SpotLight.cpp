#include "SpotLight.h"
#include "SphereMesh.h"
using ZShadeSandboxLighting::SpotLight;
//==============================================================================================================================
//==============================================================================================================================
SpotLight::SpotLight()
{
}
//==============================================================================================================================
ZShadeSandboxLighting::Light* SpotLight::Clone()
{
	SpotLight* light = (SpotLight*)new ZShadeSandboxLighting::Light();

	light->LightType() = mLightType;
	light->DiffuseColor() = mDiffuseColor;
	light->AmbientColor() = mAmbientColor;
	light->SpecularColor() = mSpecularColor;
	light->Position() = mPosition;
	light->Direction() = mDirection;
	light->Range() = fRange;
	light->SpotAngle() = fSpotAngle;
	light->Attenuation() = fAttenuation;
	light->Intensity() = fIntensity;
	light->ToggleLight() = bToggleLight;

	// Need to clone the mesh
	//light->SetMesh((ZShadeSandboxMesh::SphereMesh*)mMesh->Clone());

	return light;
}
//==============================================================================================================================