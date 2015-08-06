#include "MaterialManager.h"
//===============================================================================================================================
//===============================================================================================================================
MaterialManager* MaterialManager::m_instance = 0;
//===============================================================================================================================
//===============================================================================================================================
MaterialManager::MaterialManager()
{
}
//===============================================================================================================================
MaterialManager::~MaterialManager()
{
}
//===============================================================================================================================
MaterialManager* MaterialManager::Instance()
{
	if (m_instance == NULL)
		m_instance = new MaterialManager();
	return m_instance;
}
//===============================================================================================================================
void MaterialManager::AddDiffuse(D3D* d3d, string base_path, string name, string filename)
{
	ZShadeSandboxLighting::ShaderMaterial* mat = new ZShadeSandboxLighting::ShaderMaterial(d3d, name);
	mat->vAmbientColor = XMFLOAT4(1, 1, 1, 1);
	mat->vDiffuseColor = XMFLOAT4(1, 1, 1, 1);
	mat->vSpecularColor = XMFLOAT4(1, 1, 1, 1);
	mat->vEmissiveColor = XMFLOAT4(1, 1, 1, 1);
	mat->fSpecularPower = 5.0f;
	mat->fSpecularIntensity = 0.3f;
	mat->bHasTransparency = false;
	mat->bEnableLighting = true;
	mat->fDetailBrightness = 1.8f;
	mat->AddDiffuseTexture(base_path, filename);
	mat->AddDetailMapTexture(base_path, "detail001.dds");
	//mat->AddNormalMapTexture(base_path, "lichen1_normal.dds");
	Add(mat);
}
//===============================================================================================================================
void MaterialManager::AddTransparentDiffuse(D3D* d3d, string base_path, string name, string filename, string alphaFilename, XMFLOAT3 alphaValue)
{
	ZShadeSandboxLighting::ShaderMaterial* mat = new ZShadeSandboxLighting::ShaderMaterial(d3d, name);
	mat->vAmbientColor = XMFLOAT4(1, 1, 1, 1);
	mat->vDiffuseColor = XMFLOAT4(1, 1, 1, 1);
	mat->vSpecularColor = XMFLOAT4(1, 1, 1, 1);
	mat->vEmissiveColor = XMFLOAT4(1, 1, 1, 1);
	mat->fSpecularPower = 5.0f;
	mat->fSpecularIntensity = 0.3f;
	mat->bHasTransparency = true;
	mat->bEnableLighting = true;
	mat->fDetailBrightness = 1.8f;
	mat->vAlphaToCoverageValue = alphaValue;
	mat->AddDiffuseTexture(base_path, filename);
	mat->AddDetailMapTexture(base_path, "detail001.dds");
	//mat->AddNormalMapTexture(base_path, "lichen1_normal.dds");
	mat->AddAlphaMapTexture(base_path, alphaFilename);
	Add(mat);
}
//===============================================================================================================================
void MaterialManager::Add(ZShadeSandboxLighting::ShaderMaterial* material)
{
	map<string, ZShadeSandboxLighting::ShaderMaterial*>::iterator it = m_pMaterials.find(material->sMaterialName);
	if (it == m_pMaterials.end())
	{
		m_pMaterials.insert(make_pair(material->sMaterialName, material));
	}
}
//===============================================================================================================================
ZShadeSandboxLighting::ShaderMaterial* MaterialManager::GetMaterial(string name)
{
	map<string, ZShadeSandboxLighting::ShaderMaterial*>::iterator it = m_pMaterials.find(name);
	if (it != m_pMaterials.end())
	{
		return (*it).second;
	}

	return NULL;
}
//===============================================================================================================================