//===============================================================================================================================
// QuickHull
//
// Code from QuickHull algorithm in C++ described in paper from Barber, Dobkin, and Huhdanpaa: http://citeseer.ist.psu.edu/barber96quickhull.html
//===============================================================================================================================
// History
//
// -Created on 4/6/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __QUICKHULL_H
#define __QUICKHULL_H
//===============================================================================================================================
//===============================================================================================================================
#include <vector>
#include <fstream>
#include "HullVertex.h"
#include "HullFace.h"
#include "FaceList.h"
#include "VertexList.h"
#include "HalfEdge.h"
using namespace std;
//===============================================================================================================================
#define		CLOCKWISE						0x1
#define		INDEXED_FROM_ONE				0x2
#define		INDEXED_FROM_ZERO				0x4
#define		POINT_RELATIVE					0x8
#define		AUTOMATIC_TOLERANCE				-1
#define		DOUBLE_PREC						2.2204460492503131e-16
#define		NONCONVEX_WRT_LARGER_FACE		1
#define		NONCONVEX						2
//===============================================================================================================================
//===============================================================================================================================
namespace ZShadeSandboxMath {
class QuickHull
{
// Constructors
public:
	
	QuickHull(bool param_debug = false);
	QuickHull(vector<float> coords, bool param_debug = false);
	QuickHull(vector<ZShadeSandboxMath::XMMath3> coords, bool param_debug = false);
	
// Getters and Setters
public:
	
	// Returns the distance tolerance that was used for the most recently computed hull.
	float getDistanceTolerance() { return tolerance; }
	
	// Sets an explicit distance tolerance for convexity tests.
	void setExplicitDistanceTolerance(float tol) { explicitTolerance = tol; }
	
	// Returns the explicit distance tolerance.
	float getExplicitDistanceTolerance() { return explicitTolerance; }
	
	bool& Debugging() { return debug; }
	int getNumVertices() { return mNumVertices; }
	vector<ZShadeSandboxMath::XMMath3> getVertices();
	int getVertices(vector<float>& coords);
	vector<int> getVertexPointIndices();
	int getNumFaces() { return mFaces.size(); }
	vector<vector<int>> getFaces() { return getFaces(0); }
	vector<vector<int>> getFaces(int indexFlags);
	
// Accessor methods
public:
	
	void build(vector<float> coords);
	void build(vector<float> coords, int nump);
	void build(vector<ZShadeSandboxMath::XMMath3> coords);
	void build(vector<ZShadeSandboxMath::XMMath3> coords, int nump);
	void triangulate();
	void print(); // Print to a file
	void print(int indexFlags); // Print to a file
	bool check(); // Print to a file
	bool check(float tol); // Print to a file
	
// Protected methods
protected:
	
	void setFromQhull(vector<float> coords, int nump, bool triangulate);
	void setHull(vector<float> coords, int nump, vector<vector<int>> faceIndices, int numf);
	void initBuffers(int nump);
	void setPoints(vector<float> coords, int nump);
	void setPoints(vector<ZShadeSandboxMath::XMMath3> coords, int nump);
	void computeMaxAndMin();
	void createInitialSimplex();
	void resolveUnclaimedPoints(ZShadeSandboxMath::FaceList newFaces);
	void deleteFacePoints(ZShadeSandboxMath::HullFace* face, ZShadeSandboxMath::HullFace* absorbingFace);
	double oppFaceDistance(ZShadeSandboxMath::HalfEdge* he);
	void calculateHorizon(ZShadeSandboxMath::XMMath3 eyePnt, ZShadeSandboxMath::HalfEdge* edge0, ZShadeSandboxMath::HullFace* face, vector<ZShadeSandboxMath::HalfEdge*>& horizon);
	void addNewFaces(ZShadeSandboxMath::FaceList& newFaces, ZShadeSandboxMath::HullVertex* eyeVtx, vector<ZShadeSandboxMath::HalfEdge*> horizon);
	ZShadeSandboxMath::HullVertex* nextPointToAdd();
	void addPointToHull(ZShadeSandboxMath::HullVertex* eyeVtx);
	void buildHull();
	void reindexFacesAndVertices();
	bool checkFaceConvexity(ZShadeSandboxMath::HullFace* face, float tol, string& out);
	bool checkFaces(float tol);
	
// Private methods
private:
	
	void printPoints(); // Print to a file
	void addPointToFace(ZShadeSandboxMath::HullVertex* vtx, ZShadeSandboxMath::HullFace* face);
	void removePointFromFace(ZShadeSandboxMath::HullVertex* vtx, ZShadeSandboxMath::HullFace* face);
	HullVertex* removeAllPointsFromFace(ZShadeSandboxMath::HullFace* face);
	HalfEdge* findHalfEdge(ZShadeSandboxMath::HullVertex* tail, ZShadeSandboxMath::HullVertex* head);
	void getFaceIndices(vector<int>& indices, ZShadeSandboxMath::HullFace* face, int flags);
	bool doAdjacentMerge(ZShadeSandboxMath::HullFace* face, int mergeType);
	HalfEdge* addJoiningFace(ZShadeSandboxMath::HullVertex* eyeVtx, ZShadeSandboxMath::HalfEdge* he);
	void markFaceVertices(ZShadeSandboxMath::HullFace* face, int mark);
	
// Private member variables
private:
	
	ofstream outPrint;
	ofstream outCheck;
	ofstream outPointsFile;
	ofstream outFaceConvexity;
	ofstream outDebugFile;
	
	vector<ZShadeSandboxMath::HullVertex*> mPointBuffer;
	vector<int> mPointIndices;
	vector<ZShadeSandboxMath::HullFace*> mDiscardedFaces;
	
	vector<ZShadeSandboxMath::HullVertex*> mMaxVertexes;
	vector<ZShadeSandboxMath::HullVertex*> mMinVertexes;
	
	vector<ZShadeSandboxMath::HullFace*> mFaces;
	vector<ZShadeSandboxMath::HalfEdge*> mHorizen;
	
	ZShadeSandboxMath::FaceList mNewFaces;
	ZShadeSandboxMath::VertexList mUnclaimed;
	ZShadeSandboxMath::VertexList mClaimed;
	
	int mNumVertices;
	int mNumFaces;
	int mNumPoints;
	
	int findIndex;
	float charLength;
	float explicitTolerance;
	float tolerance;
	bool debug;
};
}
//===============================================================================================================================
//===============================================================================================================================
#endif//__QUICKHULL_H