#include "AmbientLight.h"
#include "SphereMesh.h"
using ZShadeSandboxLighting::AmbientLight;
//==============================================================================================================================
//==============================================================================================================================
AmbientLight::AmbientLight()
{
}
//==============================================================================================================================
ZShadeSandboxLighting::Light* AmbientLight::Clone()
{
	AmbientLight* light = (AmbientLight*)new ZShadeSandboxLighting::Light();
	
	light->LightType() = mLightType;
	light->DiffuseColor() = mDiffuseColor;
	light->AmbientColor() = mAmbientColor;
	light->SpecularColor() = mSpecularColor;
	light->Position() = mPosition;
	light->Direction() = mDirection;
	light->Intensity() = fIntensity;
	light->ToggleLight() = bToggleLight;

	// Need to clone the mesh
	//light->SetMesh((ZShadeSandboxMesh::SphereMesh*)mMesh->Clone());

	return light;
}
//==============================================================================================================================