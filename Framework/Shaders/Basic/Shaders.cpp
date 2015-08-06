#include "Shaders.h"
#include "MaterialShader.h"
#include "ShadowMapBuildShader.h"
#include "MaterialGBufferShader.h"
#include "MaterialLightShader.h"
#include "MaterialTessellationShader.h"
#include "MaterialLightTessellationShader.h"
using ZShadeSandboxShader::Shaders;
//=================================================================================================================
//=================================================================================================================
MaterialShader*						Shaders::pMaterialShader = 0;
MaterialLightShader*				Shaders::pMaterialLightShader = 0;
MaterialTessellationShader*			Shaders::pQuadMaterialTessellationShader = 0;
MaterialTessellationShader* 		Shaders::pTriMaterialTessellationShader = 0;
MaterialLightTessellationShader*	Shaders::pQuadMaterialLightTessellationShader = 0;
MaterialLightTessellationShader*	Shaders::pTriMaterialLightTessellationShader = 0;
ShadowMapBuildShader*				Shaders::pShadowMapBuildShader = 0;
MaterialGBufferShader*				Shaders::pMaterialGBufferShader = 0;
//=================================================================================================================
void Shaders::Build(D3D* d3d)
{
	// Create the shaders only once if they do not exist
	if (Shaders::pMaterialShader == 0
		&& Shaders::pMaterialLightShader == 0
		&& Shaders::pQuadMaterialTessellationShader == 0
		&& Shaders::pTriMaterialTessellationShader == 0
		&& Shaders::pQuadMaterialLightTessellationShader == 0
		&& Shaders::pTriMaterialLightTessellationShader == 0
		&& Shaders::pShadowMapBuildShader == 0
		&& Shaders::pMaterialGBufferShader == 0)
	{
		Shaders::pMaterialShader = new MaterialShader(d3d);
		Shaders::pMaterialLightShader = new MaterialLightShader(d3d);
		Shaders::pQuadMaterialTessellationShader = new MaterialTessellationShader(d3d, ZShadeSandboxLighting::EMaterialTessellationType::eQuad);
		Shaders::pTriMaterialTessellationShader = new MaterialTessellationShader(d3d, ZShadeSandboxLighting::EMaterialTessellationType::eTri);
		Shaders::pQuadMaterialLightTessellationShader = new MaterialLightTessellationShader(d3d, ZShadeSandboxLighting::EMaterialTessellationType::eQuad);
		Shaders::pTriMaterialLightTessellationShader = new MaterialLightTessellationShader(d3d, ZShadeSandboxLighting::EMaterialTessellationType::eTri);
		Shaders::pShadowMapBuildShader = new ShadowMapBuildShader(d3d);
		Shaders::pMaterialGBufferShader = new MaterialGBufferShader(d3d);
	}
}
//=================================================================================================================