//===============================================================================================================================
// Shaders
//
//===============================================================================================================================
// History
//
// -Created on 6/13/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __SHADERS_H
#define __SHADERS_H
//=================================================================================================================
//=================================================================================================================

//
//Includes
//

#include "D3D.h"

class MaterialShader;
class ShadowMapBuildShader;
class MaterialGBufferShader;
class MaterialLightShader;
class MaterialTessellationShader;
class MaterialLightTessellationShader;

//=================================================================================================================
//=================================================================================================================
namespace ZShadeSandboxShader {
struct Shaders
{
	static MaterialShader*					pMaterialShader;
	static MaterialLightShader*				pMaterialLightShader;
	static MaterialTessellationShader*		pQuadMaterialTessellationShader;
	static MaterialTessellationShader*		pTriMaterialTessellationShader;
	static MaterialLightTessellationShader*	pQuadMaterialLightTessellationShader;
	static MaterialLightTessellationShader* pTriMaterialLightTessellationShader;
	static ShadowMapBuildShader*			pShadowMapBuildShader;
	static MaterialGBufferShader*			pMaterialGBufferShader;

	static void Build(D3D* d3d);
};
}
//=================================================================================================================
//=================================================================================================================
#endif//__SHADERS_H