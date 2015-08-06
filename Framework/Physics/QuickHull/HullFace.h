//===============================================================================================================================
// HullFace
//
//===============================================================================================================================
// History
//
// -Created on 4/2/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __HULLFACE_H
#define __HULLFACE_H
//===============================================================================================================================
//===============================================================================================================================

#include <vector>
#include <string>
#include "ZMath.h"
#include "LNode.h"
#include "HalfEdge.h"
using namespace std;

namespace ZShadeSandboxMath {
	class FaceList;
}

//===============================================================================================================================
namespace ZShadeSandboxMath {
struct HullFace
{
	ZShadeSandboxMath::HalfEdge* mHalfEdge;
	ZShadeSandboxMath::XMMath3 mNormal;
	float mArea;
	ZShadeSandboxMath::XMMath3 mCentroid;
	float mPlaneOffset;
	int mIndex;
	int mNumVerts;
	int mMark;
	HullFace* next;
	HullFace* prev;

	static int VISIBLE;
	static int NON_CONVEX;
	static int DELETED;

	ZShadeSandboxMath::HullVertex* outside;
	
	HullFace();
	
	void computeCentroid(ZShadeSandboxMath::XMMath3& c);
	void computeNormal(ZShadeSandboxMath::XMMath3& n, float minArea);
	void computeNormal(ZShadeSandboxMath::XMMath3& n);
	void computeNormalAndCentroid();
	void computeNormalAndCentroid(float minArea);
	void getVertexIndices(vector<int>& inds);
	void triangulate(ZShadeSandboxMath::FaceList& newFaces, float minArea);
	
	static ZShadeSandboxMath::HullFace* createTriangle(ZShadeSandboxMath::HullVertex* v0, ZShadeSandboxMath::HullVertex* v1, ZShadeSandboxMath::HullVertex* v2);
	static ZShadeSandboxMath::HullFace* createTriangle(ZShadeSandboxMath::HullVertex* v0, ZShadeSandboxMath::HullVertex* v1, ZShadeSandboxMath::HullVertex* v2, float minArea);
	static ZShadeSandboxMath::HullFace* create(vector<ZShadeSandboxMath::HullVertex*> vArr, vector<int> inds);
	
	ZShadeSandboxMath::HalfEdge* getFirstEdge() { return mHalfEdge; }
	ZShadeSandboxMath::HalfEdge* getEdge(int i);
	ZShadeSandboxMath::HalfEdge* findEdge(HullVertex* vt, HullVertex* vh);
	float distanceToPlane(XMMath3 p);
	string getVertexString();
	ZShadeSandboxMath::HullFace* connectHalfEdges(ZShadeSandboxMath::HalfEdge* halfEdgePrev, ZShadeSandboxMath::HalfEdge* halfEdge);
	int mergeAdjacentFace(ZShadeSandboxMath::HalfEdge* halfEdgeAdj, vector<ZShadeSandboxMath::HullFace*>& discarded);
	float areaSquared(ZShadeSandboxMath::HalfEdge* halfEdge0, ZShadeSandboxMath::HalfEdge* halfEdge1);
	// Returns a string of errors
	string checkConsistency();
};
}
//===============================================================================================================================
//===============================================================================================================================
#endif//__HULLFACE_H