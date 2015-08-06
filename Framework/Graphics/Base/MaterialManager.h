//===============================================================================================================================
// MaterialManager
//
//===============================================================================================================================
// History
//
// -Created on 5/14/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __MATERIALMANAGER_H
#define __MATERIALMANAGER_H
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//

#include "BetterString.h"
#include "ShaderMaterial.h"
#include "D3D.h"
#include <map>
using namespace std;

//===============================================================================================================================
//===============================================================================================================================

class MaterialManager
{
	static MaterialManager* m_instance;

public:

	MaterialManager();
	~MaterialManager();

	static MaterialManager* Instance();

	void AddDiffuse(D3D* d3d, string base_path, string name, string filename);
	void AddTransparentDiffuse(D3D* d3d, string base_path, string name, string filename, string alphaFilename, XMFLOAT3 alphaValue);
	void Add(ZShadeSandboxLighting::ShaderMaterial* material);
	ZShadeSandboxLighting::ShaderMaterial* GetMaterial(string name);

private:

	map<string, ZShadeSandboxLighting::ShaderMaterial*> m_pMaterials;
};

//===============================================================================================================================
//===============================================================================================================================
#endif//__MATERIALMANAGER_H