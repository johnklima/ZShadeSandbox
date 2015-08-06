//===============================================================================================================================
// ModelEnvironment
//
//===============================================================================================================================
// History
//
// -Created on 5/12/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __MODELENVIRONMENT_H
#define __MODELENVIRONMENT_H
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//

#include "Environment3D.h"
#include "SphereMesh.h"
#include "CylindricalMesh.h"
#include "GeosphereMesh.h"
#include "CircleMesh.h"
#include "PlaneMesh.h"
#include "OBJMesh.h"

//===============================================================================================================================
//===============================================================================================================================

//
// Model Environment for the rendering of models
//
class ModelEnvironment : public Environment3D
{
public:
	
	ModelEnvironment(EngineOptions* eo);
	~ModelEnvironment();
	
	bool Initialize();
	void Update();
	void Render();
	void RenderDeferred();
	bool RenderScene();
	
	void Shutdown();
	
	void RenderShadowMap();
	
protected:
	
	// Create a textured plane with lighting in the center of the environment
	ZShadeSandboxMesh::PlaneMesh* mPlane;
	
	ZShadeSandboxMesh::SphereMesh* mSphere;
	ZShadeSandboxMesh::CylindricalMesh* mCylindricalMesh;
	ZShadeSandboxMesh::GeosphereMesh* mGeosphereMesh;
	ZShadeSandboxMesh::CircleMesh* mCircleMesh;
	ZShadeSandboxMesh::OBJMesh* mSpaceCompound;
	ZShadeSandboxMesh::OBJMesh* mHuman;
};

//===============================================================================================================================
//===============================================================================================================================
#endif//__MODELENVIRONMENT_H