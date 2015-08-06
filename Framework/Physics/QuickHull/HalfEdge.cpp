#include "HalfEdge.h"
#include "HullVertex.h"
#include "HullFace.h"
#include "Convert.h"
using ZShadeSandboxMath::HalfEdge;
//===============================================================================================================================
//===============================================================================================================================
HalfEdge::HalfEdge()
{
}
//===============================================================================================================================
HalfEdge::HalfEdge(ZShadeSandboxMath::HullVertex* v, ZShadeSandboxMath::HullFace* f)
{
	vertex = v;
	face = f;
}
//===============================================================================================================================
ZShadeSandboxMath::HullVertex* HalfEdge::head()
{
	return vertex;
}
//===============================================================================================================================
ZShadeSandboxMath::HullVertex* HalfEdge::tail()
{
	return prev != nullptr ? prev->vertex : nullptr;
}
//===============================================================================================================================
ZShadeSandboxMath::HullFace* HalfEdge::oppositeFace()
{
	return opposite != nullptr ? opposite->face : nullptr;
}
//===============================================================================================================================
BetterString HalfEdge::getVertexString()
{
	if (tail() != nullptr)
	{
		return "" + ZShadeSandboxGlobal::Convert::ConvertIntToString(tail()->index) + "-" + ZShadeSandboxGlobal::Convert::ConvertIntToString(head()->index);
	}
	else
	{
		BetterString str("?-");
		str += ZShadeSandboxGlobal::Convert::ConvertIntToString(head()->index);
		return str;
	}
}
//===============================================================================================================================
float HalfEdge::length()
{
	if (tail() != nullptr)
	{
		ZShadeSandboxMath::Vec3<XMFLOAT3> p1 = head()->point;
		ZShadeSandboxMath::Vec3<XMFLOAT3> p2 = tail()->point;
		return p1.Distance(p2);
	}

	return -1;
}
//===============================================================================================================================
float HalfEdge::lengthSquared()
{
	if (tail() != nullptr)
	{
		ZShadeSandboxMath::Vec3<XMFLOAT3> p1 = head()->point;
		ZShadeSandboxMath::Vec3<XMFLOAT3> p2 = tail()->point;
		return p1.DistanceSquared(p2);
	}

	return -1;
}
//===============================================================================================================================