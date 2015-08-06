#include "Triangle.h"
#include "ZShadeMessageCenter.h"
//==================================================================================================================================
//==================================================================================================================================
Triangle::Triangle()
:   bUseIndices(false)
{
}
//==================================================================================================================================
Triangle::Triangle(const Triangle& tri)
{
	deep_copy(tri);
}
//==================================================================================================================================
Triangle::Triangle(ZShadeSandboxMath::XMMath3 v[3])
:   bUseIndices(false)
{
	copy_vertices(v);
}
//==================================================================================================================================
Triangle::Triangle(ZShadeSandboxMath::XMMath3 v1, ZShadeSandboxMath::XMMath3 v2, ZShadeSandboxMath::XMMath3 v3)
:   bUseIndices(false)
{
	copy_vertices(v1, v2, v3);
}
//==================================================================================================================================
Triangle::Triangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3)
:   bUseIndices(false)
{
	copy_vertices(x1, y1, z1, x2, y2, z2, x3, y3, z3);
}
//==================================================================================================================================
Triangle::Triangle(ZShadeSandboxMath::XMMath3 v[3], UINT ind[3])
:   bUseIndices(true)
{
	copy_vertices(v);
	copy_indices(ind);
}
//==================================================================================================================================
Triangle::Triangle(ZShadeSandboxMath::XMMath3 v1, ZShadeSandboxMath::XMMath3 v2, ZShadeSandboxMath::XMMath3 v3, UINT ind1, UINT ind2, UINT ind3)
:   bUseIndices(true)
{
	copy_vertices(v1, v2, v3);
	copy_indices(ind1, ind2, ind3);
}
//==================================================================================================================================
Triangle::Triangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, UINT ind1, UINT ind2, UINT ind3)
:   bUseIndices(true)
{
	copy_vertices(x1, y1, z1, x2, y2, z2, x3, y3, z3);
	copy_indices(ind1, ind2, ind3);
}
//==================================================================================================================================
void Triangle::deep_copy(const Triangle& tri)
{
	mVertex[0] = tri.GetVertex()[0];
	mVertex[1] = tri.GetVertex()[1];
	mVertex[2] = tri.GetVertex()[2];
	
	mIndices[0] = tri.Indices()[0];
	mIndices[1] = tri.Indices()[1];
	mIndices[2] = tri.Indices()[2];
}
//==================================================================================================================================
void Triangle::copy_vertices(ZShadeSandboxMath::XMMath3 v[3])
{
	mVertex[0] = v[0];
	mVertex[1] = v[1];
	mVertex[2] = v[2];
}
//==================================================================================================================================
void Triangle::copy_vertices(ZShadeSandboxMath::XMMath3 v1, ZShadeSandboxMath::XMMath3 v2, ZShadeSandboxMath::XMMath3 v3)
{
	mVertex[0] = v1;
	mVertex[1] = v2;
	mVertex[2] = v3;
}
//==================================================================================================================================
void Triangle::copy_vertices(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3)
{
	mVertex[0] = ZShadeSandboxMath::XMMath3(x1, y1, z1);
	mVertex[1] = ZShadeSandboxMath::XMMath3(x2, y2, z2);
	mVertex[2] = ZShadeSandboxMath::XMMath3(x3, y3, z3);
}
//==================================================================================================================================
void Triangle::copy_indices(UINT ind[3])
{
	mIndices[0] = ind[0];
	mIndices[1] = ind[1];
	mIndices[2] = ind[2];
}
//==================================================================================================================================
void Triangle::copy_indices(UINT ind1, UINT ind2, UINT ind3)
{
	mIndices[0] = ind1;
	mIndices[1] = ind2;
	mIndices[2] = ind3;
}
//==================================================================================================================================
vector<Triangle*> Triangle::split4()
{
	ZShadeSandboxMath::XMMath3 A = mVertex[0];
	ZShadeSandboxMath::XMMath3 B = mVertex[1];
	ZShadeSandboxMath::XMMath3 C = mVertex[2];
	
	UINT ind1 = mIndices[0];
	UINT ind2 = mIndices[1];
	UINT ind3 = mIndices[2];
	
	//       B
	//       *
	//      / \
	//     /   \
	//  mA*-----*mB
	//   / \   / \
	//  /   \ /   \
	// *-----*-----*
	// A     mC     C
	
	//
	// Create the midpoints of each vertex
	//
	
	ZShadeSandboxMath::XMMath3 mA, mB, mC;
	
	// Middle of side AB
	mA.x = 0.5f * (A.x + B.x);
	mA.y = 0.5f * (A.y + B.y);
	mA.z = 0.5f * (A.z + B.z);
	
	// Middle of side BC
	mB.x = 0.5f * (B.x + C.x);
	mB.y = 0.5f * (B.y + C.y);
	mB.z = 0.5f * (B.z + C.z);
	
	// Middle of side AC
	mC.x = 0.5f * (A.x + C.x);
	mC.y = 0.5f * (A.y + C.y);
	mC.z = 0.5f * (A.z + C.z);
	
	if (bUseIndices)
	{
		//
		// Calculate the new indices based on the midpoint split
		//
		
		//           ind2
		//             *
		//            / \
		//           /   \
		//   midInd1*-----*midInd2
		//         / \   / \
		//        /   \ /   \
		//       *-----*-----*
		//    ind1  midInd3  ind3
		
		UINT midInd1, midInd2, midInd3;
		
		midInd1 = 0.5f * (ind1 + ind2);
		midInd2 = 0.5f * (ind2 + ind3);
		midInd3 = 0.5f * (ind1 + ind3);
		
		//
		// Create four new child triangles
		//
		
		vector<Triangle*> tris(4);
		
		tris[0] = new Triangle( B, mA, mB, ind2,    midInd1, midInd2);
		tris[1] = new Triangle(mA,  A, mC, midInd1, ind1,    midInd3);
		tris[2] = new Triangle(mA, mC, mB, midInd1, midInd3, midInd2);
		tris[3] = new Triangle(mB, mC,  C, midInd2, midInd3, ind3);
		
		return tris;
	}
	else
	{
		vector<Triangle*> tris(4);
		
		tris[0] = new Triangle( B, mA, mB);
		tris[1] = new Triangle(mA,  A, mC);
		tris[2] = new Triangle(mA, mC, mB);
		tris[3] = new Triangle(mB, mC,  C);
		
		return tris;
	}
}
//==================================================================================================================================
vector<Triangle*> Triangle::split2()
{
	ZShadeSandboxMath::XMMath3 A = mVertex[0];
	ZShadeSandboxMath::XMMath3 B = mVertex[1];
	ZShadeSandboxMath::XMMath3 C = mVertex[2];
	
	UINT ind1 = mIndices[0];
	UINT ind2 = mIndices[1];
	UINT ind3 = mIndices[2];
	
	//       B
	//       *
	//      /|\
	//     / | \
	//    *  |  *
	//   /   |   \
	//  /    |    \
	// *-----*-----*
	// A     mC     C
	
	//
	// Create the midpoint of the vertex
	//
	
	ZShadeSandboxMath::XMMath3 mC;
	
	// Middle of side AC
	mC.x = 0.5f * (A.x + C.x);
	mC.y = 0.5f * (A.y + C.y);
	mC.z = 0.5f * (A.z + C.z);
	
	if (bUseIndices)
	{
		//
		// Calculate the new indice based on the midpoint split
		//
		
		//           ind2
		//             *
		//            /|\
		//           / | \
		//          *  |  *
		//         /   |   \
		//        /    |    \
		//       *-----*-----*
		//    ind1  midInd3  ind3
		
		UINT midInd3;
		
		midInd3 = 0.5f * (ind1 + ind3);
		
		//
		// Create four new child triangles
		//
		
		vector<Triangle*> tris(2);
		
		tris[0] = new Triangle(B,  A, mC, ind2, ind1, midInd3);
		tris[1] = new Triangle(B, mC,  C, ind2, midInd3, ind3);
		
		return tris;
	}
	else
	{
		vector<Triangle*> tris(2);
		
		tris[0] = new Triangle(B,  A, mC);
		tris[1] = new Triangle(B, mC,  C);
		
		return tris;
	}
}
//==================================================================================================================================
void Triangle::FindCentroid()
{
	ZShadeSandboxMath::XMMath3 A = mVertex[0];
	ZShadeSandboxMath::XMMath3 B = mVertex[1];
	ZShadeSandboxMath::XMMath3 C = mVertex[2];
	
	// first construct the midpoints of each side.
	// Next, connect each midpoint to its opposite vertex, thus creating the medians of the triangle.
	// The point where all three medians intersect is called the CENTROID.
	
	//       B
	//       *
	//      / \
	//     /   \
	//  mA*-----*mB
	//   / \   / \
	//  /   \ /   \
	// *-----*-----*
	// A     mC     C
	
	//
	// Create the midpoints of each vertex
	//
	
	ZShadeSandboxMath::XMMath3 mA, mB, mC;
	
	// Middle of side AB
	mA.x = 0.5f * (A.x + B.x);
	mA.y = 0.5f * (A.y + B.y);
	mA.z = 0.5f * (A.z + B.z);
	
	// Middle of side BC
	mB.x = 0.5f * (B.x + C.x);
	mB.y = 0.5f * (B.y + C.y);
	mB.z = 0.5f * (B.z + C.z);
	
	// Middle of side AC
	mC.x = 0.5f * (A.x + C.x);
	mC.y = 0.5f * (A.y + C.y);
	mC.z = 0.5f * (A.z + C.z);
	
	//
	// Connect each midpoint to its opposite vertex
	//
	
	TLine line1, line2, line3;
	
	line1.v1 = mA;
	line1.v2 = C;
	
	line2.v1 = mB;
	line2.v2 = A;
	
	line3.v1 = mC;
	line3.v2 = B;
	
	// Find the point where all three line segments intersect
	if (!ThreeLinesIntersect(line1, line2, line3, mCentroid))
	{
		// Lines did not intersect and an error was found
		ZShadeMessageCenter::MsgBoxError(NULL, "Triangle: Lines did not intersect and the centroid not found !!!");
	}
}
//==================================================================================================================================
/*IntersectInfo Triangle::TwoLinesIntersect(TLine line1, TLine line2)
{
	ZShadeSandboxMath::XMMath3 a = line1.v1;
	ZShadeSandboxMath::XMMath3 b = line1.v2;
	ZShadeSandboxMath::XMMath3 c = line2.v1;
	ZShadeSandboxMath::XMMath3 d = line2.v2;
	
	// The dot product
	float den = ((d.y - c.y) * (b.x - a.x) - (d.x - c.x) * (b.y - a.y));
	
	float num1 = ((d.x - c.x) * (a.y - c.y) - (d.y - c.y) * (a.x - c.x));
	float num2 = ((b.x - a.x) * (a.y - c.y) - (b.y - a.y) * (a.x - c.x));
	float u1 = num1 / den;
	float u2 = num2 / den;
	
	if (den == 0 && num1  == 0 && num2 == 0)
	{
		return IntersectInfo::eCoincidents;
	}
	
	if (den == 0)
	{
		return IntersectInfo::eParallel;
	}
	
	if (u1 <0 || u1 > 1 || u2 < 0 || u2 > 1)
	{
		return IntersectInfo::eNoCollide;
	}
	
	return IntersectInfo::eCollides;
}
*/
double Dot(const ZShadeSandboxMath::XMMath3& a,const ZShadeSandboxMath::XMMath3& b)
{
	return (a.x * b.x) + (a.y * b.y);
}
double PerpDot(const ZShadeSandboxMath::XMMath3& a,const ZShadeSandboxMath::XMMath3& b)
{
	return (a.y * b.x) - (a.x * b.y);
}
bool LineCollision(const ZShadeSandboxMath::XMMath3& A1, const ZShadeSandboxMath::XMMath3& A2,
                   const ZShadeSandboxMath::XMMath3& B1, const ZShadeSandboxMath::XMMath3& B2,
                   ZShadeSandboxMath::XMMath3& out)
{
    ZShadeSandboxMath::XMMath3 a, b, c;
	
	a.x = A2.x - A1.x;
	a.y = A2.y - A1.y;
	a.z = A2.z - A1.z;
	
	b.x = B2.x - B1.x;
	b.y = B2.y - B1.y;
	b.z = B2.z - B1.z;
	
    double f = PerpDot(a, b);
	
	// lines are parallel
    if(!f)
	{
        return false;
    }
	
	c.x = B2.x - A2.x;
	c.y = B2.y - A2.y;
	c.z = B2.z - A2.z;
	
    double aa = PerpDot(a, c);
    double bb = PerpDot(b, c);
	
    if(f < 0)
    {
        if(aa > 0)     return false;
        if(bb > 0)     return false;
        if(aa < f)     return false;
        if(bb < f)     return false;
    }
    else
    {
        if(aa < 0)     return false;
        if(bb < 0)     return false;
        if(aa > f)     return false;
        if(bb > f)     return false;
    }
	
    double outD = 1.0 - (aa / f);
	
	out.x = ((B2.x - B1.x) * outD) + B1.x;
	out.y = ((B2.y - B1.y) * outD) + B1.y;
	out.z = ((B2.z - B1.z) * outD) + B1.z;
	
    return true;
}
//==================================================================================================================================
bool Triangle::ThreeLinesIntersect(TLine line1, TLine line2, TLine line3, ZShadeSandboxMath::XMMath3& center)
{
	// Check first two lines
	ZShadeSandboxMath::XMMath3 point1;
	if (!LineCollision(line1.v1, line1.v2, line2.v1, line2.v2, point1))
	{
		return false;
	}
	
	// Check last two lines
	ZShadeSandboxMath::XMMath3 point2;
	if (!LineCollision(line2.v1, line2.v2, line3.v1, line3.v2, point2))
	{
		return false;
	}
	
	// Check first and last lines
	ZShadeSandboxMath::XMMath3 point3;
	if (!LineCollision(line1.v1, line1.v2, line3.v1, line3.v2, point3))
	{
		return false;
	}
	
	// If all three points from each intersection are the same
	//  then all lines intersect and we found the center of the triangle
	
	bool b1 = (point1 == point2);
	bool b2 = (point2 == point3);
	bool b3 = (point1 == point3);
	
	if (b1 == b2 == b3)
	{
		center = point1;
		return true;
	}
	else
	{
		return false;
	}
}
//==================================================================================================================================
int Triangle::determinant()
{
	float x1, x2, x3, y1, y2, y3;
	float determ;
	
	x1 = mVertex[0].x;
	y1 = mVertex[0].z;
	x2 = mVertex[1].x;
	y2 = mVertex[1].z;
	x3 = mVertex[2].x;
	y3 = mVertex[2].z;
	
	determ = (x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1);
	
	if (determ >= 0.0)
		return COUNTER_CLOCKWISE;
	else
		return CLOCKWISE;
}
//==================================================================================================================================
void Swap(UINT& i1, UINT& i2)
{
	i1 ^= i2;
	i2 ^= i1;
	i1 ^= i2;
}
//==================================================================================================================================
void Swap(ZShadeSandboxMath::XMMath3& v1, ZShadeSandboxMath::XMMath3& v2)
{
	ZShadeSandboxMath::XMMath3 temp = v1;
	v1 = v2;
	v2 = temp;
}
//==================================================================================================================================
void Triangle::SetClockwise()
{
	int isClockwise = determinant();
	
	if (isClockwise != CLOCKWISE)
	{
		//Swap mIndices[1] and mIndices[2]
		if (bUseIndices) Swap(mIndices[1], mIndices[2]);
		Swap(mVertex[1], mVertex[2]);

		isClockwise = determinant();

		if (isClockwise != CLOCKWISE)
		{
			//Swap mIndices[1] and mIndices[2] back
			if (bUseIndices) Swap(mIndices[1], mIndices[2]);
			Swap(mVertex[1], mVertex[2]);

			//Swap mIndices[0] and mIndices[1] and keep mIndices[0] in middle
			if (bUseIndices) Swap(mIndices[0], mIndices[1]);
			Swap(mVertex[0], mVertex[1]);

			isClockwise = determinant();

			if (isClockwise != CLOCKWISE)
			{
				//Swap mIndices[0] and mIndices[2] for other check
				if (bUseIndices) Swap(mIndices[0], mIndices[2]);
				Swap(mVertex[0], mVertex[2]);

				isClockwise = determinant();

				if (isClockwise != CLOCKWISE)
				{
					//Swap mIndices[0] and mIndices[2] back
					if (bUseIndices) Swap(mIndices[0], mIndices[2]);
					Swap(mVertex[0], mVertex[2]);

					//Swap mIndices[1] and mIndices[2]
					if (bUseIndices) Swap(mIndices[1], mIndices[2]);
					Swap(mVertex[1], mVertex[2]);

					isClockwise = determinant();

					if (isClockwise != CLOCKWISE)
					{
						//Swap mIndices[0] and mIndices[1] for final check
						if (bUseIndices) Swap(mIndices[0], mIndices[1]);
						Swap(mVertex[0], mVertex[1]);

						isClockwise = determinant();

						if (isClockwise != CLOCKWISE)
						{
							//error
							ZShadeMessageCenter::MsgBoxError(NULL, "Triangle: Could not set triangle in clockwise order !!!");
						}
					}
				}
			}
		}
	}
}
//==================================================================================================================================