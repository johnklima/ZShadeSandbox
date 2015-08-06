//==============================================================================================================================
// LinePrimitive.h
//==============================================================================================================================
#ifndef ZLINEPRIMITIVE_H
#define ZLINEPRIMITIVE_H
//==============================================================================================================================
//==============================================================================================================================
#include "XMMath.h"
class XMMath3;
//using namespace DirectX;
//template <typename T> class Vec3;
//==============================================================================================================================
//==============================================================================================================================
namespace ZShadeSandboxMath {
namespace ESideOfLine
{
	enum Type
	{
		LEFT,
		RIGHT,
		ON
	};
}

class LinePrimitive
{
protected:
	
	XMFLOAT3 point1;
	XMFLOAT3 point2;
	
public:
	
	LinePrimitive();
	LinePrimitive(XMFLOAT3 p1, XMFLOAT3 p2);
	
	void Set(XMFLOAT3 p1, XMFLOAT3 p2);
	
	XMFLOAT3 Point1() const;
	XMFLOAT3 Point2() const;
	
	// Get the normal for this line segment
	XMFLOAT3 Normal();
	
	// Determine which side the point is on
	// - means left of the line
	// + means right of the line
	// = means on the line
	//ESideOfLine::Type Side(XMMath3 point);
	
	//Compute the distance from AB to C
	//if segment is true, AB is a segment, not a line.
	//float PointDistance(XMMath3 point, bool segment = false);

	bool operator == (const LinePrimitive& l);
};
}
//==============================================================================================================================
//==============================================================================================================================
#endif // ZLINEPRIMITIVE_H