//===============================================================================================================================
// MeshRenderParameters.h
//
//===============================================================================================================================
// History
//
// -Created on 6/10/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __MESHRENDERPARAMETERS_H
#define __MESHRENDERPARAMETERS_H
//===============================================================================================================================
//===============================================================================================================================
#include "MeshRenderType.h"
//===============================================================================================================================
namespace ZShadeSandboxMesh {
struct MeshRenderParameters
{
	Camera* camera;
	// Can override what the mesh adds for the world matrix with a custom world
	bool specifyWorld;
	ZShadeSandboxLighting::Light* light;
	XMMATRIX world; // This will be added by the mesh, unless bSpecifyWorld=true
	XMFLOAT4X4 view;  // This will be added by the mesh
	ZShadeSandboxMesh::ERenderType::Type renderType;
	bool renderLight; // Uses light passed into here for material light shader
	bool reflection;
	bool centerCam;
	bool tessellate;
	bool enableDistTess;
	bool shadowMap;
	bool renderDeferred;
	bool useInstancing;
	bool translateBillboard;
	bool toggleMesh;
	bool toggleWireframe;
	float blendAmount;
	float minTessDist;
	float maxTessDist;
	float noDistTessFactor;
	float minTess;
	float maxTess;
	float seaLevel;
	XMFLOAT4 clipplane;
	
	ZShadeSandboxMesh::MeshRenderParameters()
	{
		camera = 0;
		light = 0;
		renderType = ERenderType::eTriangleList;
		renderLight = false;
		reflection = false;
		centerCam = false;
		tessellate = false;
		specifyWorld = false;
		shadowMap = false;
		renderDeferred = false;
		useInstancing = false;
		translateBillboard = false;
		enableDistTess = false;
		toggleMesh = true;
		toggleWireframe = false;
		blendAmount = 0.0f;
		minTessDist = 20.0f;
		maxTessDist = 500.0f;
		minTess = -1.0f;
		maxTess = 64.0f;
		noDistTessFactor = 64.0f;
		seaLevel = 0;
		clipplane = XMFLOAT4(0, 0, 0, 0);
	}
};
}
//===============================================================================================================================
//===============================================================================================================================
#endif//__MESHRENDERTYPE_H