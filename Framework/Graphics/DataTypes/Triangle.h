//==================================================================================================================================
// Triangle.h
//
//==================================================================================================================================
// History
//
// -Created on 4/16/2015 by Dustin Watson
//==================================================================================================================================
#ifndef __TRIANGLE_H
#define __TRIANGLE_H
//==================================================================================================================================
//==================================================================================================================================
#include "D3D.h"
#include "ZMath.h"
#include "Vertex.h"
#include "XMMath.h"
//==================================================================================================================================
#define CLOCKWISE 0
#define COUNTER_CLOCKWISE 1
//==================================================================================================================================
//==================================================================================================================================
struct TLine
{
	ZShadeSandboxMath::XMMath3 v1;
	ZShadeSandboxMath::XMMath3 v2;
};
//==================================================================================================================================
/*enum IntersectInfo
{
	eCoincidents,
	eParallel,
	eNoCollide,
	eCollides
};*/
//==================================================================================================================================
class Triangle
{
public:
	
	Triangle();
	Triangle(const Triangle& tri);
	Triangle(ZShadeSandboxMath::XMMath3 v[3]);
	Triangle(ZShadeSandboxMath::XMMath3 v1, ZShadeSandboxMath::XMMath3 v2, ZShadeSandboxMath::XMMath3 v3);
	Triangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
	Triangle(ZShadeSandboxMath::XMMath3 v[3], UINT ind[3]);
	Triangle(ZShadeSandboxMath::XMMath3 v1, ZShadeSandboxMath::XMMath3 v2, ZShadeSandboxMath::XMMath3 v3, UINT ind1, UINT ind2, UINT ind3);
	Triangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, UINT ind1, UINT ind2, UINT ind3);
	
	void deep_copy(const Triangle& tri);
	void copy_vertices(ZShadeSandboxMath::XMMath3 v[3]);
	void copy_vertices(ZShadeSandboxMath::XMMath3 v1, ZShadeSandboxMath::XMMath3 v2, ZShadeSandboxMath::XMMath3 v3);
	void copy_vertices(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
	void copy_indices(UINT ind[3]);
	void copy_indices(UINT ind1, UINT ind2, UINT ind3);
	
	// Ensures that this triangle is in clockwise order
	void SetClockwise();
	
	// Splits the triangle into four new triangles
	// triangle 0 -> upperChild
	// triangle 1 -> leftChild
	// triangle 2 -> middleChild
	// triangle 3 -> rightChild
	vector<Triangle*> split4();
	
	// Splits the triangle into two new triangles
	// triangle 0 -> leftChild
	// triangle 1 -> rightChild
	vector<Triangle*> split2();
	
	// Locates the center of the triangle
	void FindCentroid();
	
	// This is just a rough test to see if this works
	//IntersectInfo TwoLinesIntersect(TLine line1, TLine line2);
	bool ThreeLinesIntersect(TLine line1, TLine line2, TLine line3, ZShadeSandboxMath::XMMath3& center);
	
	bool& UseIndices()					{ return bUseIndices; }
	ZShadeSandboxMath::XMMath3 Centroid() const			{ return mCentroid; }
	const ZShadeSandboxMath::XMMath3* GetVertex() const 	{ return mVertex; }
	const UINT* Indices() const 		{ return mIndices; }
	
	ZShadeSandboxMath::XMMath3 GetVertex(int index) { return mVertex[index]; }
	
private:
	
	// Finds out if the triangle is clockwise or counter-clockwise
	int determinant();
	
	bool bUseIndices;

	ZShadeSandboxMath::XMMath3	mCentroid;
	ZShadeSandboxMath::XMMath3 	mVertex[3];
	UINT						mIndices[3];
};
//==================================================================================================================================
//==================================================================================================================================
#endif // __TRIANGLE_H