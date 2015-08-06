//==================================================================================================================================
// BTT.h
//
// Binary Triangle Tree
//
//Notes on the recursion of the BTT
//break into 2*(n-1)*(n-1) triangles with height map width = n
//2 + 4 + 8 + 16 + 32
//recursion_limit = ZxZ -> n
//2^n+1 = ZxZ -> n
//1 = 2x2 -> 0    3
//3 = 3x3 -> 1   15
//5 = 5x5 -> 2   63
//9 = 9x9 -> 3   255
//17 = 17x17 -> 4   1023
//33 = 33x33 -> 5
//==================================================================================================================================
// History
//
// -Created on 4/16/2015 by Dustin Watson
//==================================================================================================================================
#ifndef __BTT_H
#define __BTT_H
//==================================================================================================================================
//==================================================================================================================================
#include "D3D.h"
#include "ZMath.h"
#include "Triangle.h"
//==================================================================================================================================
struct BinaryTriangle
{
	int 			recursionDepth;
	bool 			visited;
	Triangle* 		triangle;
	BinaryTriangle* parent;
	BinaryTriangle* leftChild;
	BinaryTriangle* rightChild;
	BinaryTriangle* leftNeighbor;
	BinaryTriangle* rightNeighbor;
	BinaryTriangle* bottomNeighbor;
};
//==================================================================================================================================
/*
	Setting up the BTT
	
	// Need 2 BTTs for a quad
	
	BinaryTriangleTree* tree1 = new BinaryTriangleTree(detailLevel-1);
	BinaryTriangle* bTT1 = new BinaryTriangle();
	
	ZShadeSandboxMath::XMMath3 v1[3];
	v1[0] = vv1;
	v1[1] = vv2;
	v1[2] = vv3;
	
	//Create the indices
	UINT ind1[3];
	ind1[0] = TopRight;
	ind1[1] = TopLeft;
	ind1[2] = BottomLeft;
	
	bTT1->triangle = new Triangle(v1, ind1);
	
	tree1->AddRoot( bTT1 );
	
	// 2nd triangle
	UINT indice2[3];
	indice2[0] = BottomLeft;
	indice2[1] = BottomRight;
	indice2[2] = TopRight;
	
	// Once it is created call Split or SplitOnce
*/
class BTT
{
public:
	
	BTT();
	BTT(int recursionLimit);
	BTT(ZShadeSandboxMath::XMMath3 v[3], int recursionLimit, bool split = false, bool splitOnce = false);
	BTT(ZShadeSandboxMath::XMMath3 v1, ZShadeSandboxMath::XMMath3 v2, ZShadeSandboxMath::XMMath3 v3, int recursionLimit, bool split = false, bool splitOnce = false);
	BTT(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, int recursionLimit, bool split = false, bool splitOnce = false);
	BTT(ZShadeSandboxMath::XMMath3 v[3], UINT ind[3], int recursionLimit, bool split = false, bool splitOnce = false);
	BTT(ZShadeSandboxMath::XMMath3 v1, ZShadeSandboxMath::XMMath3 v2, ZShadeSandboxMath::XMMath3 v3, UINT ind1, UINT ind2, UINT ind3, int recursionLimit, bool split = false, bool splitOnce = false);
	BTT(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, UINT ind1, UINT ind2, UINT ind3, int recursionLimit, bool split = false, bool splitOnce = false);
	BTT(Triangle* tri, int recursionLimit, bool split = false, bool splitOnce = false);
	BTT(BinaryTriangle* tri, int recursionLimit, bool split = false, bool splitOnce = false);
	~BTT();
	
	// Need to add recursion function to free memory in BTT and copy BTT in copy constructor
	
	
	void AddRoot(Triangle* tri, bool split = false, bool splitOnce = false);
	void AddRoot(BinaryTriangle* tri, bool split = false, bool splitOnce = false);
	void SplitOnce();
	void Split();
	
	// Variance check to get height at a specified position
	void CheckVariance(float x, float z, float& height, bool& foundHeight);
	
	vector<Triangle*> GetTriangles();

	static bool CalculateVariance(float x, float z, float& height, float v0[3], float v1[3], float v2[3]);

public:
	
	bool& UseIndices()						{ return bUseIndices; }
	int& RecursionLimit() 					{ return mRecursionLimit; }
	int RecursionLimit() const 				{ return mRecursionLimit; }
	int GetNumTriangles() const 			{ return mTriangles; }
	int GetNumVertices() const 				{ return mNumVertices; }
	const BinaryTriangle* GetRoot() const 	{ return root; }
	
private:
	
	void GetTriangles(BinaryTriangle* tri, vector<Triangle*>& tris);

	void CheckVariance(BinaryTriangle* tri, float x, float z, float& height, bool& foundHeight);
	
	void Split(BinaryTriangle* tri);
	void SplitOnce(BinaryTriangle* tri);
	void SplitRec(BinaryTriangle* tri);
	void SplitTriangle(BinaryTriangle*& tri);
	
	vector<BinaryTriangle*> observerList;
	vector<BinaryTriangle*> observerListTemp;
	
	BinaryTriangle* root;
	
	bool bUseIndices;
	int mRecursionLimit;
	int mTriangles;
	int mNumVertices;
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__BTT_H