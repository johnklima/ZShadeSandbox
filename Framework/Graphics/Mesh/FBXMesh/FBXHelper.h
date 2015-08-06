//===============================================================================================================================
// FBXHelper
//
// https://github.com/lang1991/FBXExporter/blob/master/FBXExporter/Utilities.h
//===============================================================================================================================
// History
//
// -Created on 6/7/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __FBXHELPER_H
#define __FBXHELPER_H
//==============================================================================================================================
//==============================================================================================================================

//
// Includes
//

#include "FBXInclude.h"
#include <vector>
#include "CGlobal.h"
#include "FBXVertex.h"
using namespace std;

//==============================================================================================================================
//==============================================================================================================================
namespace ZShadeSandboxMesh {
struct BlendingIndexWeightPair
{
	uint32 blendingIndex;
	double blendingWeight;
	
	BlendingIndexWeightPair();
};

struct PhysicalPoint
{
	XMFLOAT3 position;
	
	vector<BlendingIndexWeightPair> blendingInfo;
	
	PhysicalPoint();
};

struct FBXKeyframe
{
	FbxLongLong frameNum;
	FbxAMatrix globalTransform;
	FBXKeyframe* next;
	
	FBXKeyframe();
};

struct FBXJoint
{
	string name;
	int parentIndex;
	FbxAMatrix globalBindposeInverse;
	FBXKeyframe* animation;
	FbxNode* node;
	
	FBXJoint();
	~FBXJoint();
};

struct FBXSkeleton
{
	vector<FBXJoint> joints;
};

struct FBXTriangle
{
	vector<uint32> indices;
	string materialName;
	uint32 materialIndex;
	
	bool operator < (const FBXTriangle& rhs);
};

class FBXHelper
{
public:
	
	static FbxAMatrix GetGeometryTransformation(FbxNode* inNode);
};
}
//==============================================================================================================================
//==============================================================================================================================
#endif//__FBXHELPER_H