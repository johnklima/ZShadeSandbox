#include "BTT.h"
//==================================================================================================================================
//==================================================================================================================================
BTT::BTT()
:   mTriangles(0)
,   mNumVertices(0)
,   mRecursionLimit(0)
,   bUseIndices(false)
{
}
//==================================================================================================================================
BTT::BTT(int recursionLimit)
:   mTriangles(0)
,   mNumVertices(0)
,   mRecursionLimit(recursionLimit)
,   bUseIndices(false)
{
}
//==================================================================================================================================
BTT::BTT(ZShadeSandboxMath::XMMath3 v[3], int recursionLimit, bool split, bool splitOnce)
:   mTriangles(0)
,   mNumVertices(0)
,   mRecursionLimit(recursionLimit)
,   bUseIndices(false)
{
	Triangle* tri = new Triangle(v);

	AddRoot(tri, split, splitOnce);
}
//==================================================================================================================================
BTT::BTT(ZShadeSandboxMath::XMMath3 v1, ZShadeSandboxMath::XMMath3 v2, ZShadeSandboxMath::XMMath3 v3, int recursionLimit, bool split, bool splitOnce)
:   mTriangles(0)
,   mNumVertices(0)
,   mRecursionLimit(recursionLimit)
,   bUseIndices(false)
{
	Triangle* tri = new Triangle(v1, v2, v3);

	AddRoot(tri, split, splitOnce);
}
//==================================================================================================================================
BTT::BTT(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, int recursionLimit, bool split, bool splitOnce)
:   mTriangles(0)
,   mNumVertices(0)
,   mRecursionLimit(recursionLimit)
,   bUseIndices(false)
{
	Triangle* tri = new Triangle(x1, y1, z1, x2, y2, z2, x3, y3, z3);

	AddRoot(tri, split, splitOnce);
}
//==================================================================================================================================
BTT::BTT(ZShadeSandboxMath::XMMath3 v[3], UINT ind[3], int recursionLimit, bool split, bool splitOnce)
:   mTriangles(0)
,   mNumVertices(0)
,   mRecursionLimit(recursionLimit)
,   bUseIndices(true)
{
	Triangle* tri = new Triangle(v, ind);
	
	AddRoot(tri, split, splitOnce);
}
//==================================================================================================================================
BTT::BTT(ZShadeSandboxMath::XMMath3 v1, ZShadeSandboxMath::XMMath3 v2, ZShadeSandboxMath::XMMath3 v3, UINT ind1, UINT ind2, UINT ind3, int recursionLimit, bool split, bool splitOnce)
:   mTriangles(0)
,   mNumVertices(0)
,   mRecursionLimit(recursionLimit)
,   bUseIndices(true)
{
	Triangle* tri = new Triangle(v1, v2, v3, ind1, ind2, ind3);
	
	AddRoot(tri, split, splitOnce);
}
//==================================================================================================================================
BTT::BTT(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, UINT ind1, UINT ind2, UINT ind3, int recursionLimit, bool split, bool splitOnce)
:   mTriangles(0)
,   mNumVertices(0)
,   mRecursionLimit(recursionLimit)
,   bUseIndices(true)
{
	Triangle* tri = new Triangle(x1, y1, z1, x2, y2, z2, x3, y3, z3, ind1, ind2, ind3);
	
	AddRoot(tri, split, splitOnce);
}
//==================================================================================================================================
BTT::BTT(Triangle* tri, int recursionLimit, bool split, bool splitOnce)
:   mTriangles(0)
,   mNumVertices(0)
,   mRecursionLimit(recursionLimit)
{
	bUseIndices = tri->UseIndices();
	AddRoot(tri, split, splitOnce);
}
//==================================================================================================================================
BTT::BTT(BinaryTriangle* tri, int recursionLimit, bool split, bool splitOnce)
:   mTriangles(0)
,   mNumVertices(0)
,   mRecursionLimit(recursionLimit)
{
	bUseIndices = tri->triangle->UseIndices();
	AddRoot(tri, split, splitOnce);
}
//==================================================================================================================================
BTT::~BTT()
{
	
}
//==================================================================================================================================
void BTT::AddRoot(Triangle* tri, bool split, bool splitOnce)
{
	BinaryTriangle* bT = new BinaryTriangle();
	
	bT->triangle = tri;
	
	AddRoot(bT, split, splitOnce);
}
//==================================================================================================================================
void BTT::AddRoot(BinaryTriangle* tri, bool split, bool splitOnce)
{
	tri->recursionDepth = 0;
	tri->visited = false;
	
	root = tri;
	
	root->leftChild = NULL;
	root->rightChild = NULL;
	
	root->bottomNeighbor = NULL;
	root->leftNeighbor = NULL;
	root->rightNeighbor = NULL;
	
	observerList.push_back( root );
	
	mNumVertices = 3;
	mTriangles += 1;
	
	if (split)
	{
		if (splitOnce)
			SplitOnce();
		else
			Split();
	}
}
//==================================================================================================================================
void BTT::SplitOnce()
{
	for (int i = 0; i < observerList.size(); i++)
	{
		SplitOnce(observerList[i]);
	}
	
	//Clear the observerList
	observerList.clear();
	observerList.resize(observerListTemp.size());
	
	//Add contents of observerListTemp to observerList
	for (int i = 0; i < observerListTemp.size(); i++)
	{
		observerList[i] = observerListTemp[i];
	}
	
	//Clear observerListTemp
	observerListTemp.clear();
}
//==================================================================================================================================
void BTT::Split()
{
	SplitRec( root );
}
//==================================================================================================================================
void BTT::Split(BinaryTriangle* tri)
{
	//We are already split, no need to do it again
	if (tri->leftChild || tri->recursionDepth >= mRecursionLimit) return;
	
	//If this triangle is not in a proper diamond, force split our bottom neighbor
	if (tri->bottomNeighbor && (tri->bottomNeighbor->bottomNeighbor != tri))
		Split(tri->bottomNeighbor);
	
	//Create children
	SplitTriangle(tri);

	//If creation failed, exit
	if (!tri->leftChild) return;
	
	//Increment the render mode by one for this level of triangles
	tri->leftChild->recursionDepth = tri->rightChild->recursionDepth = tri->recursionDepth + 1;
	tri->leftChild->visited = tri->rightChild->visited = false;
	
	//Connect the parent pointers
	tri->leftChild->parent = tri;
	tri->rightChild->parent = tri;
	
	//Fill in the information we can get from the parent (neighbour pointers)
	tri->leftChild->bottomNeighbor = tri->leftNeighbor;
	tri->leftChild->leftNeighbor = tri->rightChild;
	
	tri->rightChild->bottomNeighbor = tri->rightNeighbor;
	tri->rightChild->rightNeighbor = tri->rightChild;
	
	//Link our left neighbour to the new children
	if (tri->leftNeighbor != NULL)
	{
		if (tri->leftNeighbor->bottomNeighbor == tri)
			tri->leftNeighbor->bottomNeighbor = tri->leftChild;
		else if (tri->leftNeighbor->leftNeighbor == tri)
			tri->leftNeighbor->leftNeighbor = tri->leftChild;
		else if (tri->leftNeighbor->rightNeighbor == tri)
			tri->leftNeighbor->rightNeighbor = tri->leftChild;
		else
			;//Illegal left neighbour
	}

	//Link our right neighbour to the new children
	if (tri->rightNeighbor != NULL)
	{
		if (tri->rightNeighbor->bottomNeighbor == tri)
			tri->rightNeighbor->bottomNeighbor = tri->rightChild;
		else if (tri->rightNeighbor->rightNeighbor == tri)
			tri->rightNeighbor->rightNeighbor = tri->rightChild;
		else if (tri->rightNeighbor->leftNeighbor == tri)
			tri->rightNeighbor->leftNeighbor = tri->rightChild;
		else
			;//Illegal right neighbour
	}

	//Link our base neighbour to the new children
	if (tri->bottomNeighbor != NULL)
	{
		if (tri->bottomNeighbor->leftChild)
		{
			tri->bottomNeighbor->leftChild->rightNeighbor = tri->rightChild;
			tri->bottomNeighbor->rightChild->leftNeighbor = tri->leftChild;
			tri->leftChild->rightNeighbor = tri->bottomNeighbor->rightChild;
			tri->rightChild->leftNeighbor = tri->bottomNeighbor->leftChild;
		}
		//else//Bottom neighbour in diamond not split yet
		//	Split(tri->bottomNeighbor);
	}
	else
	{
		//An edge triangle
		tri->leftChild->rightNeighbor = NULL;
		tri->rightChild->leftNeighbor = NULL;
	}
}
//==================================================================================================================================
void BTT::SplitOnce(BinaryTriangle* tri)
{
	if (!tri->visited && tri->recursionDepth < mRecursionLimit)
		Split(tri);
	
	tri->visited = true;

	if (tri->leftChild != NULL && !tri->leftChild->visited)
		observerListTemp.push_back( tri->leftChild );
	if (tri->rightChild != NULL && !tri->rightChild->visited)
		observerListTemp.push_back( tri->rightChild );
}
//==================================================================================================================================
void BTT::SplitRec(BinaryTriangle* tri)
{
	if (!tri->visited && tri->recursionDepth < mRecursionLimit)
		Split(tri);
	
	tri->visited = true;

	if (tri->leftChild)
	{
		SplitRec(tri->leftChild);
		SplitRec(tri->rightChild);
	}
}
//==================================================================================================================================
void BTT::SplitTriangle(BinaryTriangle*& tri)
{
	// triangle 0 -> leftChild
	// triangle 1 -> rightChild
	
	vector<Triangle*> tris = tri->triangle->split2();
	
	//If creation failed, exit
	if (tris[0] == NULL) return;
	
	BinaryTriangle* triangleLeft = new BinaryTriangle();
	BinaryTriangle* triangleRight = new BinaryTriangle();
	
	triangleLeft->triangle = tris[0];
	triangleLeft->bottomNeighbor = NULL;
	triangleLeft->leftNeighbor = NULL;
	triangleLeft->rightNeighbor = NULL;
	triangleLeft->leftChild = NULL;
	triangleLeft->rightChild = NULL;
	triangleLeft->recursionDepth = 0;
	triangleLeft->visited = false;
	
	triangleRight->triangle = tris[1];
	triangleRight->bottomNeighbor = NULL;
	triangleRight->leftNeighbor = NULL;
	triangleRight->rightNeighbor = NULL;
	triangleRight->leftChild = NULL;
	triangleRight->rightChild = NULL;
	triangleRight->recursionDepth = 0;
	triangleRight->visited = false;
	
	tri->leftChild = triangleLeft;
	tri->rightChild = triangleRight;
	
	// Add a new triangle
	mTriangles++;
	
	// Adds three new vertices
	mNumVertices += 3;
}
//==================================================================================================================================
vector<Triangle*> BTT::GetTriangles()
{
	vector<Triangle*> tris;

	GetTriangles(root, tris);

	return tris;
}
//==================================================================================================================================
void BTT::GetTriangles(BinaryTriangle* tri, vector<Triangle*>& tris)
{
	// Triangle does not exist
	if (tri == NULL) return;

	tris.push_back(tri->triangle);

	GetTriangles(tri->leftChild, tris);
	GetTriangles(tri->rightChild, tris);
}
//==================================================================================================================================
void BTT::CheckVariance(float x, float z, float& height, bool& foundHeight)
{
	CheckVariance(root, x, z, height, foundHeight);
}
//==================================================================================================================================
void BTT::CheckVariance(BinaryTriangle* tri, float x, float z, float& height, bool& foundHeight)
{
	// Triangle does not exist
	if (tri == NULL) return;
	
	float v0[3], v1[3], v2[3];
	
	Triangle* currTriangle = tri->triangle;
	
	v0[0] = currTriangle->GetVertex()[0].x;
	v0[1] = currTriangle->GetVertex()[0].y;
	v0[2] = currTriangle->GetVertex()[0].z;
	
	v1[0] = currTriangle->GetVertex()[1].x;
	v1[1] = currTriangle->GetVertex()[1].y;
	v1[2] = currTriangle->GetVertex()[1].z;
	
	v2[0] = currTriangle->GetVertex()[2].x;
	v2[1] = currTriangle->GetVertex()[2].y;
	v2[2] = currTriangle->GetVertex()[2].z;
	
	foundHeight = CalculateVariance(x, z, height, v0, v1, v2);
	
	// Continue searching if the height was not found at this position
	// Search the left child and right child
	if (!foundHeight)
	{
		CheckVariance(tri->leftChild, x, z, height, foundHeight);
		CheckVariance(tri->rightChild, x, z, height, foundHeight);
	}
}
//==================================================================================================================================
bool BTT::CalculateVariance(float x, float z, float& height, float v0[3], float v1[3], float v2[3])
{
	float startVector[3], directionVector[3], edge1[3], edge2[3], normal[3];
	float Q[3], e1[3], e2[3], e3[3], edgeNormal[3], temp[3];
	float magnitude, D, denominator, numerator, t, determinant;
	
	// Starting position of the ray that is being cast.
	startVector[0] = x;
	startVector[1] = 0.0f;
	startVector[2] = z;
	
	// The direction the ray is being cast.
	directionVector[0] =  0.0f;
	directionVector[1] = -1.0f;
	directionVector[2] =  0.0f;
	
	// Calculate the two edges from the three points given.
	edge1[0] = v1[0] - v0[0];
	edge1[1] = v1[1] - v0[1];
	edge1[2] = v1[2] - v0[2];
	
	edge2[0] = v2[0] - v0[0];
	edge2[1] = v2[1] - v0[1];
	edge2[2] = v2[2] - v0[2];
	
	// Calculate the normal of the triangle from the two edges.
	normal[0] = (edge1[1] * edge2[2]) - (edge1[2] * edge2[1]);
	normal[1] = (edge1[2] * edge2[0]) - (edge1[0] * edge2[2]);
	normal[2] = (edge1[0] * edge2[1]) - (edge1[1] * edge2[0]);
	
	magnitude = (float)sqrt((normal[0] * normal[0]) + (normal[1] * normal[1]) + (normal[2] * normal[2]));
	normal[0] = normal[0] / magnitude;
	normal[1] = normal[1] / magnitude;
	normal[2] = normal[2] / magnitude;
	
	// Find the distance from the origin to the plane.
	D = ((-normal[0] * v0[0]) + (-normal[1] * v0[1]) + (-normal[2] * v0[2]));
	
	// Get the denominator of the equation.
	denominator = ((normal[0] * directionVector[0]) + (normal[1] * directionVector[1]) + (normal[2] * directionVector[2]));
	
	// Make sure the result doesn't get too close to zero to prevent divide by zero.
	if(fabs(denominator) < 0.0001f)
	{
		return false;
	}
	
	// Get the numerator of the equation.
	numerator = -1.0f * (((normal[0] * startVector[0]) + (normal[1] * startVector[1]) + (normal[2] * startVector[2])) + D);
	
	// Calculate where we intersect the triangle.
	t = numerator / denominator;
	
	// Find the intersection vector.
	Q[0] = startVector[0] + (directionVector[0] * t);
	Q[1] = startVector[1] + (directionVector[1] * t);
	Q[2] = startVector[2] + (directionVector[2] * t);
	
	// Find the three edges of the triangle.
	e1[0] = v1[0] - v0[0];
	e1[1] = v1[1] - v0[1];
	e1[2] = v1[2] - v0[2];
	
	e2[0] = v2[0] - v1[0];
	e2[1] = v2[1] - v1[1];
	e2[2] = v2[2] - v1[2];
	
	e3[0] = v0[0] - v2[0];
	e3[1] = v0[1] - v2[1];
	e3[2] = v0[2] - v2[2];
	
	// Calculate the normal for the first edge.
	edgeNormal[0] = (e1[1] * normal[2]) - (e1[2] * normal[1]);
	edgeNormal[1] = (e1[2] * normal[0]) - (e1[0] * normal[2]);
	edgeNormal[2] = (e1[0] * normal[1]) - (e1[1] * normal[0]);
	
	// Calculate the determinant to see if it is on the inside, outside, or directly on the edge.
	temp[0] = Q[0] - v0[0];
	temp[1] = Q[1] - v0[1];
	temp[2] = Q[2] - v0[2];
	
	determinant = ((edgeNormal[0] * temp[0]) + (edgeNormal[1] * temp[1]) + (edgeNormal[2] * temp[2]));
	
	// Check if it is outside.
	if(determinant > 0.001f)
	{
		return false;
	}
	
	// Calculate the normal for the second edge.
	edgeNormal[0] = (e2[1] * normal[2]) - (e2[2] * normal[1]);
	edgeNormal[1] = (e2[2] * normal[0]) - (e2[0] * normal[2]);
	edgeNormal[2] = (e2[0] * normal[1]) - (e2[1] * normal[0]);
	
	// Calculate the determinant to see if it is on the inside, outside, or directly on the edge.
	temp[0] = Q[0] - v1[0];
	temp[1] = Q[1] - v1[1];
	temp[2] = Q[2] - v1[2];
	
	determinant = ((edgeNormal[0] * temp[0]) + (edgeNormal[1] * temp[1]) + (edgeNormal[2] * temp[2]));
	
	// Check if it is outside.
	if(determinant > 0.001f)
	{
		return false;
	}
	
	// Calculate the normal for the third edge.
	edgeNormal[0] = (e3[1] * normal[2]) - (e3[2] * normal[1]);
	edgeNormal[1] = (e3[2] * normal[0]) - (e3[0] * normal[2]);
	edgeNormal[2] = (e3[0] * normal[1]) - (e3[1] * normal[0]);
	
	// Calculate the determinant to see if it is on the inside, outside, or directly on the edge.
	temp[0] = Q[0] - v2[0];
	temp[1] = Q[1] - v2[1];
	temp[2] = Q[2] - v2[2];
	
	determinant = ((edgeNormal[0] * temp[0]) + (edgeNormal[1] * temp[1]) + (edgeNormal[2] * temp[2]));
	
	// Check if it is outside.
	if(determinant > 0.001f)
	{
		return false;
	}
	
	// Now we have our height.
	height = Q[1];
	
	return true;
}
//==================================================================================================================================