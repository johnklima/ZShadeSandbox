//==============================================================================================================================
// BoxPrimitive.h
//==============================================================================================================================
#ifndef BOXPRIMITIVE_H
#define BOXPRIMITIVE_H
//==============================================================================================================================
//==============================================================================================================================
#include "Vec.h"
#include "ZMath.h"
//==============================================================================================================================
//==============================================================================================================================
namespace ZShadeSandboxMath {
class BoxPrimitive
{
	XMFLOAT3 vertices[8];
	XMFLOAT4 vertices4[8];

public:
	
	BoxPrimitive();
	BoxPrimitive(XMFLOAT3 v[8]);
	BoxPrimitive(XMFLOAT4 v[8]);

	void GetVertices(XMFLOAT3 v[8]);
	void GetVertices(XMFLOAT4 v[8]);
	
	XMFLOAT3 Center();
	XMFLOAT4 Center4();
	
	bool ContainsPoint(XMFLOAT3 point);
	bool ContainsPoint(XMFLOAT4 point);
	
	// Create a box from four points, that extend from a distance in the z direction at the specified axis
	//  This assumes that the four points are the base of the box
	//void CreateFrom4Points(XMFLOAT3 p1, XMFLOAT3 p2, XMFLOAT3 p3, XMFLOAT3 p4, float max = 100, float min = 0, ZShadeSandboxMath::EAxis::Type axis = ZShadeSandboxMath::EAxis::Axis_Y);
	//void CreateFrom4Points(XMFLOAT4 p1, XMFLOAT4 p2, XMFLOAT4 p3, XMFLOAT4 p4, float max = 100, float min = 0, ZShadeSandboxMath::EAxis::Type axis = ZShadeSandboxMath::EAxis::Axis_Y);
};
}
//==============================================================================================================================
//==============================================================================================================================
#endif// BOXPRIMITIVE_H