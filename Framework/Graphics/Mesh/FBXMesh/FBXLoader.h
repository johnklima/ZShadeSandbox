//===============================================================================================================================
// FBXLoader
//
// http://www.walkerb.net/blog/dx-4/
// http://gamedev.stackexchange.com/questions/59419/c-fbx-animation-importer-using-the-fbx-sdk
// http://www.gamedev.net/page/resources/_/technical/graphics-programming-and-theory/how-to-work-with-fbx-sdk-r3582
// http://www.gamedev.net/page/resources/_/technical/graphics-programming-and-theory/skinned-mesh-animation-using-matrices-r3577
// https://github.com/lang1991/FBXExporter
// https://github.com/lang1991/FBXExporter/blob/master/FBXExporter/FBXExporter.h
//===============================================================================================================================
// History
//
// -Created on 4/2/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __FBXLOADER_H
#define __FBXLOADER_H
//==============================================================================================================================
//==============================================================================================================================

//
// Includes
//

#include <unordered_map>
#include <vector>
#include <fstream>
#include "FBXInclude.h"
#include "CGlobal.h"
#include "ShaderMaterial.h"
#include "FBXVertex.h"
#include "FBXHelper.h"
using namespace std;

//==============================================================================================================================
//==============================================================================================================================
namespace ZShadeSandboxMesh {
class FBXLoader
{
public:
	
	FBXLoader(D3D* d3d, GameDirectory3D* gd3d);
	~FBXLoader();
	
	bool Load(const char* model_path);
	
private:
	
	// Removes duplicated vertices
	void Optimize();
	void CleanupFbxManager();
	
	bool LoadScene();
	
	// FBX Mesh Data
	
	void LoadMesh(FbxNode* node);
	void LoadGeometry(FbxNode* node);
	void LoadVertexNormal(FbxMesh* mesh, int inCtrlPointIndex, int inVertexCounter, XMFLOAT3& outNormal);
	void LoadVertexTexture(FbxMesh* mesh, int inCtrlPointIndex, int inTextureIndex, int inTextureLayer, XMFLOAT2& outTexture);
	void LoadVertexBinormal(FbxMesh* mesh, int inCtrlPointIndex, int inVertexCounter, XMFLOAT3& outBinormal);
	void LoadVertexTangent(FbxMesh* mesh, int inCtrlPointIndex, int inVertexCounter, XMFLOAT3& outTangent);
	int FindVertex(const ZShadeSandboxMesh::VertexNormalTexBlend& inTargetVertex, const std::vector<ZShadeSandboxMesh::VertexNormalTexBlend>& uniqueVertices);
	
	// FBX Animation
	
	void LoadSkeletonHierarchy(FbxNode* inRootNode);
	void LoadSkeletonHierarchy(FbxNode* inNode, int inDepth, int myIndex, int parentIndex);
	void LoadJointsAndAnimation(FbxNode* inNode);
	void LoadControlPoints(FbxNode* node);
	uint32 FindJointIndexUsingName(string& name);
	
	// Material Data
	
	void AssociateMaterialToMesh(FbxNode* inNode);
	void LoadMaterials(FbxNode* inNode);
	void LoadMaterialAttribute(FbxSurfaceMaterial* inMaterial, uint32 inMaterialIndex);
	void LoadMaterialTexture(FbxSurfaceMaterial* inMaterial, ZShadeSandboxLighting::ShaderMaterial* ioMaterial);
	
private:
	
	ofstream outFile;
	D3D* m_pD3DSystem;
	GameDirectory3D* m_pGD3D;
	FbxManager* m_pFBXManager;
	FbxScene* m_pFbxScene;
	string mInputFilePath;
	bool bHasAnimation;
	unordered_map<uint32, ZShadeSandboxMesh::PhysicalPoint*> mControlPoints;
	uint32 mTriangleCount;
	vector<FBXTriangle> mTriangles;
	vector<ZShadeSandboxMesh::VertexNormalTexBlend> mVertices;
	ZShadeSandboxMesh::FBXSkeleton mSkeleton;
	unordered_map<uint32, ZShadeSandboxLighting::ShaderMaterial*> mMaterials;
	string mAnimationName;
	FbxLongLong mAnimationLength;
	LARGE_INTEGER mCPUFreq;
};
}
//==============================================================================================================================
//==============================================================================================================================
#endif//__FBXLOADER_H