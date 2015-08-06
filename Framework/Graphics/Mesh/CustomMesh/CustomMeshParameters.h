//===============================================================================================================================
// CustomMeshParameters.h
//
//===============================================================================================================================
// History
//
// -Created on 4/14/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __CUSTOMMESHPARAMETERS_H
#define __CUSTOMMESHPARAMETERS_H
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//

#include "D3D.h"
#include "Camera.h"
#include "Vertex.h"
#include "MeshRenderType.h"
#include "ShaderMaterial.h"

namespace ZShadeSandboxShader {
	class Shader;
}

//===============================================================================================================================
//===============================================================================================================================
namespace ZShadeSandboxMesh {
struct MeshParameters
{
	ZShadeSandboxMesh::EVertexType::Type vertexType;
	bool rotationAxisX;
	bool rotationAxisY;
	bool rotationAxisZ;
	XMFLOAT3 pos;
	XMFLOAT3 rot;// Represents the starting position of rotation
	XMFLOAT3 scale;
	ZShadeSandboxLighting::ShaderMaterial* material;
	//int textureWidth;
	//int textureHeight;
	ZShadeSandboxShader::Shader* shader;
	bool in2D;
	bool useCustomShader;
	bool bUseTessellation; // Decide if we are loading tessellation
	
	ZShadeSandboxMesh::MeshParameters()
	{
		useCustomShader = false;
		vertexType = ZShadeSandboxMesh::EVertexType::Type::VT_Color;
		rotationAxisX = false;
		rotationAxisY = false;
		rotationAxisZ = false;
		pos = XMFLOAT3(0,0,0);
		rot = XMFLOAT3(0,0,0);
		scale = XMFLOAT3(1,1,1);
		in2D = true;
		//textureWidth = -1;
		//textureHeight = -1;
		shader = 0;
		bUseTessellation = false;
		material = 0;
	}
};
}
//===============================================================================================================================
//===============================================================================================================================
#endif//__CUSTOMMESHPARAMETERS_H