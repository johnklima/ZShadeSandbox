#include "FBXVertex.h"
#include "ZMath.h"
#include <algorithm>
//==============================================================================================================================
//==============================================================================================================================
ZShadeSandboxMesh::VertexBlendingInfo::VertexBlendingInfo()
{
}
//==============================================================================================================================
bool ZShadeSandboxMesh::VertexBlendingInfo::operator < (const ZShadeSandboxMesh::VertexBlendingInfo& rhs)
{
	return (blendingWeight > rhs.blendingWeight);
}
//==============================================================================================================================
void ZShadeSandboxMesh::VertexNormalTexBlend::SortBlendingInfoByWeight()
{
	std::sort(vertexBlendingInfos.begin(), vertexBlendingInfos.end());
}
//==============================================================================================================================
bool ZShadeSandboxMesh::VertexNormalTexBlend::operator == (const ZShadeSandboxMesh::VertexNormalTexBlend& rhs) const
{
	bool sameBlendingInfo = true;
	
	// We only compare the blending info when there is blending info
	if(!(vertexBlendingInfos.empty() && rhs.vertexBlendingInfos.empty()))
	{
		// Each vertex should only have 4 index-weight blending info pairs
		for (unsigned int i = 0; i < 4; ++i)
		{
			if (vertexBlendingInfos[i].blendingIndex != rhs.vertexBlendingInfos[i].blendingIndex ||
				abs(vertexBlendingInfos[i].blendingWeight - rhs.vertexBlendingInfos[i].blendingWeight) > 0.001)
			{
				sameBlendingInfo = false;
				break;
			}
		}
	}
	
	bool result1 = ZShadeSandboxMath::ZMath::CompareVector3WithEpsilon(position, rhs.position);
	bool result2 = ZShadeSandboxMath::ZMath::CompareVector3WithEpsilon(normal, rhs.normal);
	bool result3 = ZShadeSandboxMath::ZMath::CompareVector2WithEpsilon(texture, rhs.texture);
	
	return result1 && result2 && result3 && sameBlendingInfo;
}
//==============================================================================================================================