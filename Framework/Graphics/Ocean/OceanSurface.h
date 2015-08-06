//==============================================================================================================================
// OceanSurface.h
//
//==============================================================================================================================
// History
//
// -Created on 5/30/2015  by Dustin Watson
//==============================================================================================================================
#ifndef __OCEANSURFACE_H
#define __OCEANSURFACE_H
//==============================================================================================================================
//==============================================================================================================================

//
// Includes
//

#include "OceanPatchTree.h"
#include "OceanParameters.h"
#include "OceanSurfaceShader.h"


//==============================================================================================================================
//==============================================================================================================================

class OceanSurface
{
public:

	OceanSurface(D3D* d3d, OceanParameters ocean_params);

	void Initialize();

	void SetWireframe(bool wire);

	void Update(float seaLevel, float dt);
	void Render(Camera* camera, float seaLevel, float dt);

	void SetReflectionMap(ID3D11ShaderResourceView* srv);
	void SetRefractionMap(ID3D11ShaderResourceView* srv);

	OceanSurfaceShader* mSurfaceShader;
	OceanPatchTree* mPatchTree;

private:

	void Render(Camera* camera, float seaLevel, float dt, OceanPatch* patch);

	D3D* mD3DSystem;

	float fTime;
	int iFrustumCull;
	bool bRootContainsCamera;

	OceanParameters* mOceanParams;
};

//==============================================================================================================================
//==============================================================================================================================
#endif//__OCEANSURFACE_H