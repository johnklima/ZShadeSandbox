#include "HullFace.h"
#include "HullVertex.h"
#include "BetterString.h"
#include "Convert.h"
#include "HullException.h"
#include "FaceList.h"
using ZShadeSandboxMath::HullFace;
//===============================================================================================================================
//===============================================================================================================================
int HullFace::VISIBLE = 1;
int HullFace::NON_CONVEX = 2;
int HullFace::DELETED = 3;
//===============================================================================================================================
HullFace::HullFace()
{
	mMark = VISIBLE;
}
//===============================================================================================================================
void HullFace::computeCentroid(ZShadeSandboxMath::XMMath3& c)
{
	ZShadeSandboxMath::HalfEdge* he = mHalfEdge;
	
	do {
		c += he->head()->point;
		he = he->next;
	} while (he != mHalfEdge);
	
	c *= (1 / (float)mNumVerts);
}
//===============================================================================================================================
void HullFace::computeNormal(ZShadeSandboxMath::XMMath3& n, float minArea)
{
	computeNormal(n);

	if (mArea < minArea)
	{
		// make the normal more robust by removing
	    // components parallel to the longest edge
		
		ZShadeSandboxMath::HalfEdge* heMax = nullptr;
		
		float lenSqrMax = 0;
		
		ZShadeSandboxMath::HalfEdge* he = mHalfEdge;
		
		do {
			double lenSqr = he->lengthSquared();
			
			if (lenSqr > lenSqrMax)
			{
				heMax = he;
				lenSqrMax = lenSqr;
			}
			
			he = he->next;
		} while (he != mHalfEdge);
		
		ZShadeSandboxMath::XMMath3 p2 = heMax->head()->point;
		ZShadeSandboxMath::XMMath3 p1 = heMax->tail()->point;
		
		float lenMax = (float)sqrt(lenSqrMax);
		float ux = (p2.x - p1.x) / lenMax;
		float uy = (p2.y - p1.y) / lenMax;
		float uz = (p2.z - p1.z) / lenMax;
		float dot = n.x * ux + n.y * uy + n.z * uz;
		
		n.x -= dot * ux;
		n.y -= dot * uy;
		n.z -= dot * uz;
		
		n.NormalizeMe();
	}
}
//===============================================================================================================================
void HullFace::computeNormal(ZShadeSandboxMath::XMMath3& n)
{
	ZShadeSandboxMath::HalfEdge* he1 = mHalfEdge->next;
	ZShadeSandboxMath::HalfEdge* he2 = he1->next;
	
	ZShadeSandboxMath::XMMath3 p0 = mHalfEdge->head()->point;
	ZShadeSandboxMath::XMMath3 p2 = he1->head()->point;
	
	float d2x = p2.x - p0.x;
	float d2y = p2.y - p0.y;
	float d2z = p2.z - p0.z;
	
	mNumVerts = 2;
	
	while (he2 != mHalfEdge)
	{
		float d1x = d2x;
		float d1y = d2y;
		float d1z = d2z;
		
		p2 = he2->head()->point;
		
		d2x = p2.x - p0.x;
		d2y = p2.y - p0.y;
		d2z = p2.z - p0.z;
		
		n.x += d1y * d2z - d1z * d2y;
		n.y += d1z * d2x - d1x * d2z;
		n.z += d1x * d2y - d1y * d2x;
		
		he1 = he2;
		he2 = he2->next;
		
		mNumVerts++;
	}
	
	mArea = sqrt(n.x * n.x + n.y * n.y + n.z * n.z);
	n *= (1 / mArea);
}
//===============================================================================================================================
void HullFace::computeNormalAndCentroid()
{
	computeNormal(mNormal);
	computeCentroid(mCentroid);
	
	mPlaneOffset = mNormal.Dot(mCentroid);
	
	int numv = 0;
	
	ZShadeSandboxMath::HalfEdge* he = mHalfEdge;
	
	do {
		numv++;
		he = he->next;
	} while (he != mHalfEdge);
	
	if (numv != mNumVerts)
	{
		BetterString s = "";
		s += "face ";
		s += getVertexString();
		s += " numVerts=";
		s += ZShadeSandboxGlobal::Convert::ConvertIntToString(mNumVerts);
		s += " should be ";
		s += ZShadeSandboxGlobal::Convert::ConvertIntToString(numv);
		throw HullException(s.toString());
	}
}
//===============================================================================================================================
void HullFace::computeNormalAndCentroid(float minArea)
{
	computeNormal(mNormal, minArea);
	computeCentroid(mCentroid);
	
	mPlaneOffset = mNormal.Dot(mCentroid);
}
//===============================================================================================================================
void HullFace::getVertexIndices(vector<int>& inds)
{
	if (inds.size() > 0) inds.clear();
	
	ZShadeSandboxMath::HalfEdge* he = mHalfEdge;
	
	do {
		inds.push_back(he->head()->index);
		he = he->next;
	} while (he != mHalfEdge);
}
//===============================================================================================================================
string HullFace::checkConsistency()
{
	BetterString s = "";
	
	// do a sanity check on the face
	ZShadeSandboxMath::HalfEdge* he = mHalfEdge;
	float maxd = 0;
	int numv = 0;
	
	if (mNumVerts < 3)
	{
		s += "degenerate face: ";
		s += getVertexString();
		return s.toString();
	}
	
	do {
		ZShadeSandboxMath::HalfEdge* heOpp = he->opposite;
		
		if (heOpp == nullptr)
		{
			s += "face ";
			s += getVertexString();
			s += ": unreflected half edge ";
			s += he->getVertexString();
			return s.toString();
		}
		else if (heOpp->opposite != he)
		{
			s += "face ";
			s += getVertexString();
			s += ": opposite half edge ";
			s += heOpp->getVertexString();
			s += " has opposite ";
			s += heOpp->opposite->getVertexString();
			return s.toString();
		}
		
		if (heOpp->head() != he->tail() ||
			he->head() != heOpp->tail())
		{
			s += "face ";
			s += getVertexString();
			s += ": half edge ";
			s += he->getVertexString();
			s += " reflected by ";
			s += heOpp->getVertexString();
			return s.toString();
		}
		
		ZShadeSandboxMath::HullFace* oppFace = heOpp->face;
		
		if (oppFace == nullptr)
		{
			s += "face ";
			s += getVertexString();
			s += ": no face on half edge ";
			s += heOpp->getVertexString();
			return s.toString();
		}
		else if (oppFace->mMark == DELETED)
		{
			s += "face ";
			s += getVertexString();
			s += ": opposite face ";
			s += oppFace->getVertexString();
			s += " not on hull";
			return s.toString();
		}
		
		double d = abs(distanceToPlane(he->head()->point));
		
		if (d > maxd)
			maxd = d;
		
		numv++;
		
		he = he->next;
	} while (he != mHalfEdge);
	
	if (numv != mNumVerts)
	{
		s += "face ";
		s += getVertexString();
		s += " numVerts=";
		s += ZShadeSandboxGlobal::Convert::ConvertIntToString(mNumVerts);
		s += " should be ";
		s += ZShadeSandboxGlobal::Convert::ConvertIntToString(numv);
	}
	
	return s.toString();
}
//===============================================================================================================================
void HullFace::triangulate(ZShadeSandboxMath::FaceList& newFaces, float minArea)
{
	if (mNumVerts < 4) return;
	
	ZShadeSandboxMath::HullVertex* v0 = mHalfEdge->head();
	ZShadeSandboxMath::HullFace* prevFace = nullptr;
	ZShadeSandboxMath::HalfEdge* he = mHalfEdge->next;
	ZShadeSandboxMath::HalfEdge* oppPrev = he->opposite;
	ZShadeSandboxMath::HullFace* face0 = nullptr;
	
	for (he = he->next; he != mHalfEdge->prev; he = he->next)
	{
		HullFace* face = createTriangle(v0, he->prev->head(), he->head(), minArea);
		
		face->mHalfEdge->next->opposite = oppPrev;
		face->mHalfEdge->prev->opposite = he->opposite;
		
		oppPrev = face->mHalfEdge;
		
		newFaces.add(face);
		
		if (face0 == nullptr)
			face0 = face;
	}
	
	he = new ZShadeSandboxMath::HalfEdge(mHalfEdge->prev->prev->head(), this);
	
	he->opposite = oppPrev;
	he->prev = mHalfEdge;
	he->prev->next = he;
	he->next = mHalfEdge->prev;
	he->next->prev = he;
	
	computeNormalAndCentroid(minArea);
	
	string error = checkConsistency();
	
	if (BetterString(error) != "")
	{
		// Throw exception for the error
		throw HullException(error);
	}
	
	for (HullFace* face = face0; face != nullptr; face = face->next)
	{
		string faceError = face->checkConsistency();
		
		if (BetterString(faceError) != "")
		{
			// Throw exception for the error
			throw HullException(faceError);
		}
	}
}
//===============================================================================================================================
HullFace* HullFace::createTriangle(ZShadeSandboxMath::HullVertex* v0, ZShadeSandboxMath::HullVertex* v1, ZShadeSandboxMath::HullVertex* v2)
{
	return createTriangle(v0, v1, v2, 0);
}
//===============================================================================================================================
HullFace* HullFace::createTriangle(ZShadeSandboxMath::HullVertex* v0, ZShadeSandboxMath::HullVertex* v1, ZShadeSandboxMath::HullVertex* v2, float minArea)
{
	HullFace* face = new HullFace();
	
	HalfEdge* he0 = new HalfEdge(v0, face);
	HalfEdge* he1 = new HalfEdge(v1, face);
	HalfEdge* he2 = new HalfEdge(v2, face);
	
	he0->prev = he2;
	he0->next = he1;
	he1->prev = he0;
	he1->next = he2;
	he2->prev = he1;
	he2->next = he0;
	
	face->mHalfEdge = he0;
	
	face->computeNormalAndCentroid(minArea);
	
	return face;
}
//===============================================================================================================================
HullFace* HullFace::create(vector<ZShadeSandboxMath::HullVertex*> vArr, vector<int> inds)
{
	HullFace* face = new HullFace();
	
	ZShadeSandboxMath::HalfEdge* hePrev = nullptr;
	
	for (int i = 0; i < inds.size(); i++)
	{
		HalfEdge* he = new ZShadeSandboxMath::HalfEdge(vArr[inds[i]], face);
		
		if (hePrev != nullptr)
		{
			he->prev = hePrev;
			hePrev->next = he;
		}
		else
		{
			face->mHalfEdge = he;
		}
		
		hePrev = he;
	}
	
	face->mHalfEdge->prev = hePrev;
	hePrev->next = face->mHalfEdge;
	
	face->computeNormalAndCentroid();
	
	return face;
}
//===============================================================================================================================
ZShadeSandboxMath::HalfEdge* HullFace::getEdge(int i)
{
	ZShadeSandboxMath::HalfEdge* he = mHalfEdge;
	
	while (i > 0)
	{
		he = he->next;
		
		i--;
	}
	
	while (i < 0)
	{
		he = he->prev;
		
		i++;
	}
	
	return he;
}
//===============================================================================================================================
ZShadeSandboxMath::HalfEdge* HullFace::findEdge(ZShadeSandboxMath::HullVertex* vt, ZShadeSandboxMath::HullVertex* vh)
{
	ZShadeSandboxMath::HalfEdge* he = mHalfEdge;
	
	do {
		if (he->head() == vh && he->tail() == vt)
			return he;
		he = he->next;
	} while (he != mHalfEdge);
	
	return nullptr;
}
//===============================================================================================================================
float HullFace::distanceToPlane(ZShadeSandboxMath::XMMath3 p)
{
	return mNormal.x * p.x + mNormal.y * p.y + mNormal.z * p.z - mPlaneOffset;
}
//===============================================================================================================================
string HullFace::getVertexString()
{
	BetterString s = "";
	
	ZShadeSandboxMath::HalfEdge* he = mHalfEdge;
	
	do {
		if (s == "")
		{
			s += ZShadeSandboxGlobal::Convert::ConvertIntToString(he->head()->index);
		}
		else
		{
			s += " ";
			s += ZShadeSandboxGlobal::Convert::ConvertIntToString(he->head()->index);
		}
		he = he->next;
	} while (he != mHalfEdge);
	
	return s.toString();
}
//===============================================================================================================================
HullFace* HullFace::connectHalfEdges(ZShadeSandboxMath::HalfEdge* halfEdgePrev, ZShadeSandboxMath::HalfEdge* halfEdge)
{
	HullFace* discardedFace = nullptr;
	
	// There is a redundant edge that we can get rid off
	if (halfEdgePrev->oppositeFace() == halfEdge->oppositeFace())
	{
		HullFace* oppFace = halfEdge->oppositeFace();
		
		ZShadeSandboxMath::HalfEdge* heOpp;
		
		if (halfEdgePrev == mHalfEdge)
		{
			mHalfEdge = halfEdge;
		}
		
		// We can get rid of the opposite face altogether
		if (oppFace->mNumVerts == 3)
		{
			heOpp = halfEdge->opposite->prev->opposite;
			
			oppFace->mMark = DELETED;
			
			discardedFace = oppFace;
		}
		else
		{
			heOpp = halfEdge->opposite->next;
			
			if (oppFace->mHalfEdge == halfEdgePrev->prev)
				oppFace->mHalfEdge = heOpp;
			
			heOpp->prev = heOpp->prev->prev;
			heOpp->prev->next = heOpp;
		}
		
		halfEdge->prev = halfEdgePrev->prev;
		halfEdge->prev->next = halfEdge;
		
		halfEdge->opposite = heOpp;
		heOpp->opposite = halfEdge;
		
		// oppFace was modified, so need to recompute
	    oppFace->computeNormalAndCentroid();
	}
	else
	{
		halfEdgePrev->next = halfEdge;
		halfEdge->prev = halfEdgePrev;
	}
	
	return discardedFace;
}
//===============================================================================================================================
int HullFace::mergeAdjacentFace(ZShadeSandboxMath::HalfEdge* halfEdgeAdj, vector<ZShadeSandboxMath::HullFace*>& discarded)
{
	HullFace* oppFace = halfEdgeAdj->oppositeFace();
	
	int numDiscarded = 0;
	
	discarded.push_back(oppFace);
	numDiscarded++;
	
	oppFace->mMark = DELETED;
	
	ZShadeSandboxMath::HalfEdge* heOpp = halfEdgeAdj->opposite;
	
	ZShadeSandboxMath::HalfEdge* heAdjPrev = halfEdgeAdj->prev;
	ZShadeSandboxMath::HalfEdge* heAdjNext = halfEdgeAdj->next;
	ZShadeSandboxMath::HalfEdge* heOppPrev = heOpp->prev;
	ZShadeSandboxMath::HalfEdge* heOppNext = heOpp->next;
	
	while (heAdjPrev->oppositeFace() == oppFace)
	{
		heAdjPrev = heOppPrev->prev;
		heOppNext = heOppNext->next;
	}
	
	while (heAdjNext->oppositeFace() == oppFace)
	{
		heOppPrev = heOppPrev->prev;
		heAdjNext = heAdjNext->next;
	}
	
	ZShadeSandboxMath::HalfEdge* he;
	
	for (he = heOppNext; he != heOppPrev->next; he = he->next)
	{
		he->face = this;
	}
	
	if (halfEdgeAdj == mHalfEdge)
	{
		mHalfEdge = heAdjNext;
	}
	
	// handle the half edges at the head
	HullFace* discardedFace = connectHalfEdges(heOppPrev, heAdjNext);
	
	if (discardedFace != nullptr)
	{
		discarded.push_back(discardedFace);
		numDiscarded++;
	}
	
	// handle the half edges at the tail
	discardedFace = connectHalfEdges(heAdjPrev, heOppNext);
	
	if (discardedFace != nullptr)
	{
		discarded.push_back(discardedFace);
		numDiscarded++;
	}
	
	computeNormalAndCentroid();
	
	string error = checkConsistency();
	
	if (BetterString(error) != "")
	{
		// Throw exception for the error
		throw HullException(error);
	}
	
	return numDiscarded;
}
//===============================================================================================================================
float HullFace::areaSquared(ZShadeSandboxMath::HalfEdge* halfEdge0, ZShadeSandboxMath::HalfEdge* halfEdge1)
{
	// return the squared area of the triangle defined
	// by the half edge hedge0 and the point at the
	// head of halfEdge1.
	
	ZShadeSandboxMath::XMMath3 p0 = halfEdge0->tail()->point;
	ZShadeSandboxMath::XMMath3 p1 = halfEdge0->head()->point;
	ZShadeSandboxMath::XMMath3 p2 = halfEdge1->head()->point;
	
	float dx1 = p1.x - p0.x;
	float dy1 = p1.y - p0.y;
	float dz1 = p1.z - p0.z;
	
	float dx2 = p2.x - p0.x;
	float dy2 = p2.y - p0.y;
	float dz2 = p2.z - p0.z;
	
	float x = dy1 * dz2 - dz1 * dy2;
	float y = dz1 * dx2 - dx1 * dz2;
	float z = dx1 * dy2 - dy1 * dx2;
	
	return x * x + y * y + z * z;
}
//===============================================================================================================================