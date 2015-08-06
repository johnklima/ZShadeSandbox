//=====================================================================================================================
//SAT (Separating Axis Theorem)
//=====================================================================================================================
#ifndef __SAT_H
#define __SAT_H
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
#include <math.h>
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
struct PROJECTION
{
	float min;
	float max;

	float GetDistance(PROJECTION o)
	{
		return sqrt((min-o.max)*(min-o.max) + (max-o.min)*(max-o.min));
	}
};
//---------------------------------------------------------------------------------------------------------------------
struct SAT_POINT
{
	float x;
	float y;
	SAT_POINT() {}
	SAT_POINT(float _x, float _y) : x(_x), y(_y) {}

	friend SAT_POINT& operator - (const SAT_POINT& p1, const SAT_POINT& p2)
	{
		SAT_POINT p;
		p.x = p1.x - p2.x;
		p.y = p1.y - p2.y;
		return p;
	}

	SAT_POINT normalize()
	{
		return SAT_POINT(x / sqrt(x*x + y*y), y / sqrt(x*x + y*y));
	}
};
//---------------------------------------------------------------------------------------------------------------------
struct SAT_RECT
{
	float x;
	float y;
	int width;
	int height;
	
	SAT_RECT() {}
	SAT_RECT(float _x, float _y, float _w, float _h) :
		x(_x), y(_y), width(_w), height(_h)
	{}
	
	SAT_POINT topLeft()     { return SAT_POINT(x, y); }
	SAT_POINT topRight()    { return SAT_POINT(x + width, y); }
	SAT_POINT bottomLeft()  { return SAT_POINT(x, y + height); }
	SAT_POINT bottomRight() { return SAT_POINT(x + width, y + height); }

	friend bool operator == (const SAT_RECT& r1, const SAT_RECT& r2)
	{
		return (r1.x == r2.x && r1.y == r2.y && r1.width == r2.width && r1.height == r2.height);
	}
};
//---------------------------------------------------------------------------------------------------------------------
struct MTV
{
	SAT_POINT _smallest;
	float     _o_value;
};
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
//If two convex objects are not penetrating,
//  there exists an axis for which the projection of
//  the objects will not overlap.

class SAT
{
	MTV m_mtv;
public:
	SAT();
	SAT(const SAT& o);
	~SAT();

	MTV GetMTV() { return m_mtv; }

	bool collides(SAT_RECT r1, SAT_RECT r2);
	bool collides2(SAT_RECT r1, SAT_RECT r2);
	
private:
	bool overlap(PROJECTION p1, PROJECTION p2);
	float dot(SAT_POINT vec1, SAT_POINT vec2);
	PROJECTION project(SAT_RECT s_rect, SAT_POINT axis);
	float overlap_value(PROJECTION p1, PROJECTION p2);
	SAT_POINT projectAxis(SAT_POINT axis, SAT_POINT point);
};
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
#endif//__SAT_H