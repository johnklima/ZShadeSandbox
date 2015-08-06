//===============================================================================================================================
// TessellationEnvironment
//
//===============================================================================================================================
// History
//
// -Created on 4/8/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __TESSELLATIONENVIRONMENT_H
#define __TESSELLATIONENVIRONMENT_H
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//
#pragma region "Includes"
#include "Environment3D.h"
#include "QuadMesh.h"
#include "TriangleMesh.h"
#include "CubeMesh.h"
#pragma endregion

//===============================================================================================================================
//===============================================================================================================================

//
// Tessellation Environment Test
//
class TessellationEnvironment : public Environment3D
{
public:
	TessellationEnvironment(EngineOptions* eo);
	~TessellationEnvironment();

	bool Initialize();
	void Shutdown();

	void Update();
	void Render();
	void RenderDeferred();
    
private:
	
	float fTessFactor;
	ZShadeSandboxMesh::QuadMesh* mTessellatedQuad;
	ZShadeSandboxMesh::TriangleMesh* mTessellatedTri;
	ZShadeSandboxMesh::CubeMesh* mTessellatedCube;
	ZShadeSandboxMesh::QuadMesh* mRegularQuad;
};

//===============================================================================================================================
//===============================================================================================================================
#endif//__TESSELLATIONENVIRONMENT_H