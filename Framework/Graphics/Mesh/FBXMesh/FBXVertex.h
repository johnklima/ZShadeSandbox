//===============================================================================================================================
// FBXVertex
//
// https://github.com/lang1991/FBXExporter/blob/master/FBXExporter/Vertex.h
//===============================================================================================================================
// History
//
// -Created on 6/7/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __FBXVERTEX_H
#define __FBXVERTEX_H
//==============================================================================================================================
//==============================================================================================================================

//
// Includes
//

#include <vector>
#include "CGlobal.h"
#include "Vertex.h"
using namespace std;

//==============================================================================================================================
//==============================================================================================================================
namespace ZShadeSandboxMesh {
struct VertexBlendingInfo
{
	uint32 blendingIndex;
	double blendingWeight;
	
	VertexBlendingInfo();
	
	bool operator < (const VertexBlendingInfo& rhs);
};

struct VertexNormalTexBlend
{
	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT2 texture;
	vector<VertexBlendingInfo> vertexBlendingInfos;
	
	void SortBlendingInfoByWeight();
	
	bool operator == (const VertexNormalTexBlend& rhs) const;
};
}
//==============================================================================================================================
//==============================================================================================================================
#endif//__FBXVERTEX_H