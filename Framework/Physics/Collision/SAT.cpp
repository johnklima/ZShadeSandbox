#include "SAT.h"
//---------------------------------------------------------------------------------------------------------------------
SAT::SAT()
{

}
//---------------------------------------------------------------------------------------------------------------------
SAT::SAT(const SAT& o)
{

}
//---------------------------------------------------------------------------------------------------------------------
SAT::~SAT()
{

}
//---------------------------------------------------------------------------------------------------------------------
float SAT::dot(SAT_POINT vec1, SAT_POINT vec2)
{
	return (
		vec1.x * vec2.x +
		vec1.y * vec2.y
	);
}
//---------------------------------------------------------------------------------------------------------------------
PROJECTION SAT::project(SAT_RECT s_rect, SAT_POINT axis)
{
	SAT_POINT v[4];
	v[0] = s_rect.topLeft();
	v[1] = s_rect.topRight();
	v[2] = s_rect.bottomLeft();
	v[3] = s_rect.bottomRight();

	SAT_POINT projection = projectAxis(axis, v[0]);
	float min = dot(axis, projection);

	float max = min;

	for (int i = 1; i < 4; i++)
	{
		projection = projectAxis(axis, v[i]);
		float p = dot(axis, projection);
		
		if (p < min)
		{
			min = p;
		}
		else if (p > max)
		{
			max = p;
		}
	}

	PROJECTION proj;
	proj.min = min;
	proj.max = max;

	return proj;
}
//---------------------------------------------------------------------------------------------------------------------
bool SAT::collides(SAT_RECT r1, SAT_RECT r2)
{
	SAT_POINT axis[4];

	axis[0] = SAT_POINT(r1.topRight() - r1.topLeft());
	axis[1] = SAT_POINT(r1.topRight() - r1.bottomRight());
	axis[2] = SAT_POINT(r2.topLeft() - r2.bottomLeft());
	axis[3] = SAT_POINT(r2.topLeft() - r2.topRight());

	SAT_POINT smallest;
	float o_value = 90000000000.0f;

	for (int i = 0; i < 4; i++)
	{
		PROJECTION p1 = project(r1, axis[i]);
		PROJECTION p2 = project(r2, axis[i]);

		if (!overlap(p1, p2))
		{
			return false;
		}
		else
		{
			//Get the overlap
			float o = overlap_value(p1, p2);
			o = abs( o );
			if (o < o_value)
			{
				o_value = o;
				smallest = axis[i];
			}

			/*
			//Get the overlap
			float o = overlap_value(p1, p2);

			//Check the minimum
			if (o < o_value)
			{
				//set this one as the smallest
				o_value = o;
				smallest = axis[i];
			}
			*/
		}
	}

	
	m_mtv._smallest.x = -smallest.x;
	m_mtv._smallest.y = smallest.y;

	if (m_mtv._smallest.x == -0) m_mtv._smallest.x = 0;
	
	//if (m_mtv._smallest.y < 0)
	//	m_mtv._smallest.y = -m_mtv._smallest.y;
	
	//if (m_mtv._smallest.y == -0) m_mtv._smallest.y = 0;

	m_mtv._o_value = o_value;

	return true;
}
//---------------------------------------------------------------------------------------------------------------------
bool SAT::collides2(SAT_RECT r1, SAT_RECT r2)
{
	SAT_POINT axis[4];

	axis[0] = SAT_POINT(r1.topRight() - r1.topLeft());
	axis[1] = SAT_POINT(r1.topRight() - r1.bottomRight());
	axis[2] = SAT_POINT(r2.topLeft() - r2.bottomLeft());
	axis[3] = SAT_POINT(r2.topLeft() - r2.topRight());

	for (int i = 0; i < 4; i++)
	{
		PROJECTION p1 = project(r1, axis[i]);
		PROJECTION p2 = project(r2, axis[i]);

		if (!overlap(p1, p2))
		{
			return false;
		}
	}

	return true;
}
//---------------------------------------------------------------------------------------------------------------------
bool SAT::overlap(PROJECTION p1, PROJECTION p2)
{
	return p1.max >= p2.min && p1.min <= p2.max;
}
//---------------------------------------------------------------------------------------------------------------------
float SAT::overlap_value(PROJECTION p1, PROJECTION p2)
{
	if (!overlap(p1, p2))
		return 0;
	else
	{
		//if (p1.max > p2.min)
		//	return sqrt((p2.min-p1.max)*(p2.min-p1.max) + (p2.max-p1.min)*(p2.max-p1.min));//p1.GetDistance(p2);//sqrt(p1.min*p1.min + p2.max*p2.max);
		//else if (p1.min > p2.max)
		//	return sqrt((p1.min-p2.max)*(p1.min-p2.max) + (p1.max-p2.min)*(p1.max-p2.min));//p2.GetDistance(p1);//sqrt(p1.max*p1.max + p2.min*p2.min);
		//else
		//	return 0;

		if (p1.min < p2.min)
			return p2.min - p1.max;
		else
			return p1.min - p2.max;
	}
}
//---------------------------------------------------------------------------------------------------------------------
SAT_POINT SAT::projectAxis(SAT_POINT axis, SAT_POINT point)
{
	float p = (point.x * axis.x + point.y * axis.y) / (pow(axis.x, 2) + pow(axis.y, 2));
	SAT_POINT proj;
	proj.x = p * axis.x;
	proj.y = p * axis.y;
	return proj;
}