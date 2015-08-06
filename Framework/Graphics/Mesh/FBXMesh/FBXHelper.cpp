#include "FBXHelper.h"
#include <exception>
//==============================================================================================================================
//==============================================================================================================================
ZShadeSandboxMesh::BlendingIndexWeightPair::BlendingIndexWeightPair()
:	blendingIndex(0)
,	blendingWeight(0)
{
}
//==============================================================================================================================
ZShadeSandboxMesh::PhysicalPoint::PhysicalPoint()
{
	blendingInfo.reserve(4);
}
//==============================================================================================================================
ZShadeSandboxMesh::FBXKeyframe::FBXKeyframe()
:	next(nullptr)
{
}
//==============================================================================================================================
ZShadeSandboxMesh::FBXJoint::FBXJoint()
:	node(nullptr)
,	animation(nullptr)
,	parentIndex(-1)
{
	globalBindposeInverse.SetIdentity();
}
//==============================================================================================================================
ZShadeSandboxMesh::FBXJoint::~FBXJoint()
{
	while (animation)
	{
		FBXKeyframe* temp = animation->next;
		delete animation;
		animation = temp;
	}
}
//==============================================================================================================================
bool ZShadeSandboxMesh::FBXTriangle::operator < (const FBXTriangle& rhs)
{
	return materialIndex < rhs.materialIndex;
}
//==============================================================================================================================
FbxAMatrix ZShadeSandboxMesh::FBXHelper::GetGeometryTransformation(FbxNode* inNode)
{
	if (!inNode)
	{
		throw std::exception("Null for mesh geometry");
	}
	
	const FbxVector4 lT = inNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4 lR = inNode->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4 lS = inNode->GetGeometricScaling(FbxNode::eSourcePivot);
	
	return FbxAMatrix(lT, lR, lS);
}
//==============================================================================================================================