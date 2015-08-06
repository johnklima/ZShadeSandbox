#include "QuickHull.h"
#include "HullException.h"
#include "Convert.h"
#include "ZShadeMessageCenter.h"
using ZShadeSandboxMath::QuickHull;
//===============================================================================================================================
//===============================================================================================================================
QuickHull::QuickHull(bool param_debug)
:   debug(param_debug)
,   findIndex(-1)
{
}
//===============================================================================================================================
QuickHull::QuickHull(vector<float> coords, bool param_debug)
:   debug(param_debug)
,   findIndex(-1)
{
	build(coords, coords.size() / 3);
}
//===============================================================================================================================
QuickHull::QuickHull(vector<ZShadeSandboxMath::XMMath3> coords, bool param_debug)
:   debug(param_debug)
,   findIndex(-1)
{
	build(coords, coords.size());
}
//===============================================================================================================================
vector<ZShadeSandboxMath::XMMath3> QuickHull::getVertices()
{
	vector<ZShadeSandboxMath::XMMath3> verts;
	verts.resize(mNumVertices);
	
	for (int i = 0; i < mNumVertices; i++)
	{
		verts[i] = mPointBuffer[mPointIndices[i]]->point;
	}
	
	return verts;
}
//===============================================================================================================================
int QuickHull::getVertices(vector<float>& coords)
{
	coords.resize(mNumVertices * 3 + 2);
	
	for (int i = 0; i < mNumVertices; i++)
	{
		ZShadeSandboxMath::XMMath3 p = mPointBuffer[mPointIndices[i]]->point;
		
		coords[i * 3 + 0] = p.x;
		coords[i * 3 + 1] = p.y;
		coords[i * 3 + 2] = p.z;
	}
	
	return mNumVertices;
}
//===============================================================================================================================
vector<int> QuickHull::getVertexPointIndices()
{
	vector<int> inds;
	inds.resize(mNumVertices);
	
	for (int i = 0; i < mNumVertices; i++)
	{
		inds[i] = mPointIndices[i];
	}
	
	return inds;
}
//===============================================================================================================================
vector<vector<int>> QuickHull::getFaces(int indexFlags)
{
	vector<vector<int>> allFaces;
	allFaces.resize(mFaces.size());
	
	int k = 0;
	
	vector<ZShadeSandboxMath::HullFace*>::iterator it = mFaces.begin();
	
	for (; it != mFaces.end(); it++)
	{
		ZShadeSandboxMath::HullFace* face = (ZShadeSandboxMath::HullFace*)((*it)->next);
		
		allFaces[k].resize(face->mNumVerts);
		
		getFaceIndices(allFaces[k], face, indexFlags);
		
		k++;
	}
	
	return allFaces;
}
//===============================================================================================================================
void QuickHull::build(vector<float> coords)
{
	build(coords, coords.size() / 3);
}
//===============================================================================================================================
void QuickHull::build(vector<float> coords, int nump)
{
	if (nump < 4)
	{
		throw HullException("Less than four input points specified");
	}
	
	if (coords.size() / 3 < nump)
	{
		throw HullException("Coordinate array too small for specified number of points");
	}
	
	outDebugFile.open("QuickHullDebug.txt", ios::out);
	outFaceConvexity.open("QuickHullFaceConvexity.txt", ios::out);
	
	initBuffers(nump);
	setPoints(coords, nump);
	buildHull();
	
	outDebugFile.close();
	outFaceConvexity.close();
}
//===============================================================================================================================
void QuickHull::build(vector<ZShadeSandboxMath::XMMath3> coords)
{
	build(coords, coords.size());
}
//===============================================================================================================================
void QuickHull::build(vector<ZShadeSandboxMath::XMMath3> coords, int nump)
{
	if (nump < 4)
	{
		throw HullException("Less than four input points specified");
	}
	
	if (coords.size() < nump)
	{
		throw HullException("Point array too small for specified number of points");
	}
	
	outDebugFile.open("QuickHullDebug.txt", ios::out);
	outFaceConvexity.open("QuickHullFaceConvexity.txt", ios::out);
	
	initBuffers(nump);
	setPoints(coords, nump);
	buildHull();
	
	outDebugFile.close();
	outFaceConvexity.close();
}
//===============================================================================================================================
void QuickHull::triangulate()
{
	float minArea = 1000 * charLength * DOUBLE_PREC;
	
	mNewFaces.clear();
	
	vector<ZShadeSandboxMath::HullFace*>::iterator it = mFaces.begin();
	for (; it != mFaces.end(); it++)
	{
		ZShadeSandboxMath::HullFace* face = (ZShadeSandboxMath::HullFace*)(*it)->next;
		
		if (face->mMark == HullFace::VISIBLE)
		{
			face->triangulate(mNewFaces, minArea);
		}
	}
	
	for (ZShadeSandboxMath::HullFace* face = mNewFaces.first(); face != nullptr; face = face->next)
	{
		mFaces.push_back(face);
	}
}
//===============================================================================================================================
void QuickHull::print()
{
	print(0);
}
//===============================================================================================================================
void QuickHull::print(int indexFlags)
{
	outPrint.open("QuickHullData.txt", ios::out);
	
	if ((indexFlags & INDEXED_FROM_ZERO) == 0)
	{
		indexFlags |= INDEXED_FROM_ONE;
	}
	
	for (int i=0; i < mNumVertices; i++)
	{
		ZShadeSandboxMath::XMMath3 pnt = mPointBuffer[mPointIndices[i]]->point;
		
		string px = ZShadeSandboxGlobal::Convert::ConvertFloatToString(pnt.x);
		string py = ZShadeSandboxGlobal::Convert::ConvertFloatToString(pnt.y);
		string pz = ZShadeSandboxGlobal::Convert::ConvertFloatToString(pnt.z);
		
		outPrint << "vertex(" << px << " " << py << " " << pz << ")\n";
	}
	
	vector<ZShadeSandboxMath::HullFace*>::iterator it = mFaces.begin();
	for (; it != mFaces.end(); it++)
	{
		ZShadeSandboxMath::HullFace* face = (ZShadeSandboxMath::HullFace*)(*it)->next;
		//int[] indices = new int[face->numVertices()];
		vector<int> indices(face->mNumVerts);
		getFaceIndices(indices, face, indexFlags);

		outPrint << "face(";
		
		for (int k = 0; k < indices.size(); k++)
		{
			outPrint << " " << ZShadeSandboxGlobal::Convert::ConvertIntToString(indices[k]);
		}

		outPrint << ")\n";
	}
	
	outPrint.close();
}
//===============================================================================================================================
bool QuickHull::check()
{
	return check(getDistanceTolerance());
}
//===============================================================================================================================
bool QuickHull::check(float tol)
{
	outCheck.open("QuickHullToleranceCheck.txt", ios::out);
	
	float dist;
	float pointTol = 10 * tol;
	
	if (!checkFaces(tolerance))
		return false;
	
	//Inclusion
	for (int i = 0; i < mNumPoints; i++)
	{
		ZShadeSandboxMath::XMMath3 p = mPointBuffer[i]->point;
		
		vector<ZShadeSandboxMath::HullFace*>::iterator it = mFaces.begin();
		for (; it != mFaces.end(); it++)
		{
			ZShadeSandboxMath::HullFace* face = (ZShadeSandboxMath::HullFace*)(*it)->next;
			
			if (face->mMark == HullFace::VISIBLE)
			{
				dist = face->distanceToPlane(p);
				
				if (dist > pointTol)
				{
					string str_i = ZShadeSandboxGlobal::Convert::ConvertIntToString(i);
					string str_dist = ZShadeSandboxGlobal::Convert::ConvertFloatToString(dist);
					
					outCheck << "Point " << str_i << " " << str_dist << " above face" << face->getVertexString() << "\n";
					
					return false;
				}
			}
		}
	}
	
	outCheck.close();
	
	return true;
}
//===============================================================================================================================
void QuickHull::setFromQhull(vector<float> coords, int nump, bool triangulate)
{
	// Wait on this function and do it last
	
	BetterString commandStr = "./qhull i";
	
	if (triangulate)
		commandStr += " -Qt";
	
	//try
	//{
		// Execute commandStr through environment
		
	//} catch(HullException e)
	//{
		
	//}
}
//===============================================================================================================================
void QuickHull::setHull(vector<float> coords, int nump, vector<vector<int>> faceIndices, int numf)
{
	initBuffers(nump);
	setPoints(coords, nump);
	computeMaxAndMin();
	
	for (int i = 0; i < numf; i++)
	{
		ZShadeSandboxMath::HullFace* face = ZShadeSandboxMath::HullFace::create(mPointBuffer, faceIndices[i]);
		
		ZShadeSandboxMath::HalfEdge* he = face->mHalfEdge;
		
		do {
			ZShadeSandboxMath::HalfEdge* heOpp = findHalfEdge(he->head(), he->tail());
			
			if (heOpp != nullptr)
				he->opposite = heOpp;
			
			he = he->next;
		} while(he != face->mHalfEdge);
		
		mFaces.push_back(face);
	}
}
//===============================================================================================================================
void QuickHull::initBuffers(int nump)
{
	if (mPointBuffer.size() < nump)
	{
		vector<ZShadeSandboxMath::HullVertex*> newBuffer;
		
		newBuffer.resize(nump);
		mPointIndices.resize(nump);
		
		for (int i = 0; i < mPointBuffer.size(); i++)
			newBuffer[i] = mPointBuffer[i];
		
		for (int i = mPointBuffer.size(); i < nump; i++)
			newBuffer[i] = new HullVertex();
		
		for (int i = 0; i < newBuffer.size(); i++)
			mPointBuffer[i] = newBuffer[i];
	}
	
	mFaces.clear();
	mClaimed.clear();
	mNumFaces = 0;
	mNumPoints = nump;
}
//===============================================================================================================================
void QuickHull::setPoints(vector<float> coords, int nump)
{
	for (int i = 0; i < nump; i++)
	{
		ZShadeSandboxMath::HullVertex* v = mPointBuffer[i];
		
		v->point.x = coords[i * 3 + 0];
		v->point.y = coords[i * 3 + 1];
		v->point.z = coords[i * 3 + 2];
		
		v->index = i;
	}
}
//===============================================================================================================================
void QuickHull::setPoints(vector<ZShadeSandboxMath::XMMath3> coords, int nump)
{
	for (int i = 0; i < nump; i++)
	{
		ZShadeSandboxMath::HullVertex* v = mPointBuffer[i];
		
		v->point.x = coords[i].x;
		v->point.y = coords[i].y;
		v->point.z = coords[i].z;
		
		v->index = i;
	}
}
//===============================================================================================================================
void QuickHull::computeMaxAndMin()
{
	ZShadeSandboxMath::XMMath3 mmax;
	ZShadeSandboxMath::XMMath3 mmin;
	
	for (int i = 0; i < 3; i++)
		mMaxVertexes[i] = mMinVertexes[i] = mPointBuffer[0];
	
	mmax = mPointBuffer[0]->point;
	mmin = mPointBuffer[0]->point;
	
	for (int i = 1; i < mNumPoints; i++)
	{
		ZShadeSandboxMath::XMMath3 p = mPointBuffer[i]->point;
		
		if (p.x > mmax.x)
		{
			mmax.x = p.x;
			mMaxVertexes[0] = mPointBuffer[i];
		}
		else if (p.x < mmin.x)
		{
			mmin.x = p.x;
			mMinVertexes[0] = mPointBuffer[i];
		}
		
		if (p.y > mmax.y)
		{
			mmax.y = p.y;
			mMaxVertexes[1] = mPointBuffer[i];
		}
		else if (p.y < mmin.y)
		{
			mmin.y = p.y;
			mMinVertexes[1] = mPointBuffer[i];
		}
		
		if (p.z > mmax.z)
		{
			mmax.z = p.z;
			mMaxVertexes[2] = mPointBuffer[i];
		}
		else if (p.z < mmin.z)
		{
			mmin.z = p.z;
			mMinVertexes[2] = mPointBuffer[i];
		}
	}
	
	//
	// this epsilon formula comes from QuickHull.
	//
	
	charLength = max(mmax.x - mmin.x, mmax.y - mmin.y);
	charLength = max(mmax.z - mmin.z, charLength);
	
	if (explicitTolerance == AUTOMATIC_TOLERANCE)
	{
		tolerance = 3 * DOUBLE_PREC * (max(abs(mmax.x), abs(mmin.x)) + max(abs(mmax.y), abs(mmin.y)) + max(abs(mmax.z), abs(mmin.z)));
	}
	else
	{
		tolerance = explicitTolerance;
	}
}
//===============================================================================================================================
void QuickHull::createInitialSimplex()
{
	float fMax = 0;
	int iMax = 0;
	
	float diffX = mMaxVertexes[0]->point.x - mMinVertexes[0]->point.x;
	if (diffX > fMax)
	{
		fMax = diffX;
		iMax = 0;
	}
	
	float diffY = mMaxVertexes[1]->point.y - mMinVertexes[1]->point.y;
	if (diffY > fMax)
	{
		fMax = diffY;
		iMax = 1;
	}
	
	float diffZ = mMaxVertexes[2]->point.z - mMinVertexes[2]->point.z;
	if (diffZ > fMax)
	{
		fMax = diffZ;
		iMax = 2;
	}
	
	if (fMax <= tolerance)
		throw ZShadeSandboxMath::HullException("createInitialSimplex: Input points appear to be coincident");
	
	ZShadeSandboxMath::HullVertex** vtx = new ZShadeSandboxMath::HullVertex*[4];
	
	// set first two vertices to be those with the greatest one dimensional separation
	vtx[0] = mMaxVertexes[iMax];
	vtx[1] = mMinVertexes[iMax];
	
	// set third vertex to be the vertex farthest from the line between vtx0 and vtx1
	ZShadeSandboxMath::XMMath3 u01, diff02, nrml, xprod;
	
	float maxSqr = 0;
	
	u01 = vtx[1]->point;
	u01 -= vtx[0]->point;
	u01.NormalizeMe();
	
	for (int i = 0; i < mNumPoints; i++)
	{
		diff02 = mPointBuffer[i]->point;
		diff02 -= vtx[0]->point;
		
		xprod = u01;
		xprod.Cross(diff02);
		
		float lengthSqr = xprod.x * xprod.x + xprod.y * xprod.y + xprod.z * xprod.z;
		
		if (lengthSqr > maxSqr && mPointBuffer[i] != vtx[0] && mPointBuffer[i] != vtx[1])
		{
			maxSqr = lengthSqr;
			vtx[2] = mPointBuffer[i];
			nrml = xprod;
		}
	}
	
	if (sqrt(maxSqr) <= 100 * tolerance)
		throw ZShadeSandboxMath::HullException("createInitialSimplex: Input points appear to be colinear");
	
	nrml.NormalizeMe();
	
	float maxDist = 0;
	float d0 = vtx[2]->point.Dot(nrml);
	
	for (int i = 0; i < mNumPoints; i++)
	{
		float dist = abs(mPointBuffer[i]->point.Dot(nrml) - d0);
		
		if (dist > maxDist && mPointBuffer[i] != vtx[0] && mPointBuffer[i] != vtx[1] && mPointBuffer[i] != vtx[2])
		{
			maxDist = dist;
			vtx[3] = mPointBuffer[i];
		}
	}
	
	if (abs(maxDist) <= 100 * tolerance)
		throw ZShadeSandboxMath::HullException("createInitialSimplex: Input points appear to be coplanar");
	
	if (debug)
	{
		for (int i = 0; i < 4; i++)
		{
			string px = ZShadeSandboxGlobal::Convert::ConvertFloatToString(vtx[i]->point.x);
			string py = ZShadeSandboxGlobal::Convert::ConvertFloatToString(vtx[i]->point.y);
			string pz = ZShadeSandboxGlobal::Convert::ConvertFloatToString(vtx[i]->point.z);

			outDebugFile << "initial vertices:\n";
			outDebugFile << ZShadeSandboxGlobal::Convert::ConvertIntToString(vtx[i]->index);
			outDebugFile << ": (";
			outDebugFile << px;
			outDebugFile << ", ";
			outDebugFile << py;
			outDebugFile << ", ";
			outDebugFile << pz;
			outDebugFile << ")\n";
		}
	}
	
	ZShadeSandboxMath::HullFace** tris = new ZShadeSandboxMath::HullFace*[4];
	
	if (vtx[3]->point.Dot(nrml) - d0 < 0)
	{
		tris[0] = ZShadeSandboxMath::HullFace::createTriangle(vtx[0], vtx[1], vtx[2]);
		tris[1] = ZShadeSandboxMath::HullFace::createTriangle(vtx[3], vtx[1], vtx[0]);
		tris[2] = ZShadeSandboxMath::HullFace::createTriangle(vtx[3], vtx[2], vtx[1]);
		tris[3] = ZShadeSandboxMath::HullFace::createTriangle(vtx[3], vtx[0], vtx[2]);
		
		for (int i = 0; i < 3; i++)
		{
			int k = (i + 1) % 3;
			
			tris[i + 1]->getEdge(1)->opposite = tris[k + 1]->getEdge(0);
			tris[i + 1]->getEdge(2)->opposite = tris[0]->getEdge(k);
		}
	}
	else
	{
		tris[0] = ZShadeSandboxMath::HullFace::createTriangle(vtx[0], vtx[2], vtx[1]);
		tris[1] = ZShadeSandboxMath::HullFace::createTriangle(vtx[3], vtx[0], vtx[1]);
		tris[2] = ZShadeSandboxMath::HullFace::createTriangle(vtx[3], vtx[1], vtx[2]);
		tris[3] = ZShadeSandboxMath::HullFace::createTriangle(vtx[3], vtx[2], vtx[0]);
		
		for (int i = 0; i < 3; i++)
		{
			int k = (i + 1) % 3;
			
			tris[i + 1]->getEdge(0)->opposite = tris[k + 1]->getEdge(1);
			tris[i + 1]->getEdge(2)->opposite = tris[0]->getEdge((3 - i) % 3);
		}
	}
	
	for (int i = 0; i < 4; i++)
	{
		mFaces.push_back(tris[i]);
	}
	
	for (int i = 0; i < mNumPoints; i++)
	{
		ZShadeSandboxMath::HullVertex* v = mPointBuffer[i];

		if (v == vtx[0] || v == vtx[1] || v == vtx[2] || v == vtx[3])
			continue;
		
		maxDist = tolerance;
		
		HullFace* maxFace = nullptr;
		
		for (int k = 0; k < 4; k++)
		{
			float dist = tris[k]->distanceToPlane(v->point);
			
			if (dist > maxDist)
			{
				maxFace = tris[k];
				maxDist = dist;
			}
		}
		
		if (maxFace != nullptr)
			addPointToFace(v, maxFace);
	}
}
//===============================================================================================================================
void QuickHull::resolveUnclaimedPoints(ZShadeSandboxMath::FaceList newFaces)
{
	ZShadeSandboxMath::HullVertex* vNext = mUnclaimed.first();
	
	for (ZShadeSandboxMath::HullVertex* v = vNext; v != nullptr; v = vNext)
	{
		vNext = v->next;
		
		float maxDist = tolerance;
		
		ZShadeSandboxMath::HullFace* maxFace = nullptr;
		
		for (ZShadeSandboxMath::HullFace* newFace = mNewFaces.first(); newFace != nullptr; newFace = newFace->next)
		{
			if (newFace->mMark == ZShadeSandboxMath::HullFace::VISIBLE)
			{
				float dist = newFace->distanceToPlane(v->point);
				
				if (dist > maxDist)
					maxFace = newFace;
				
				if (maxDist > 1000 * tolerance)
					break;
			}
		}
		
		if (maxFace != nullptr)
		{
			addPointToFace(v, maxFace);
			
			if (debug && v->index == findIndex)
			{
				outDebugFile << ZShadeSandboxGlobal::Convert::ConvertIntToString(findIndex) << " CLAIMED BY " << maxFace->getVertexString() << "\n";
			}
		}
		else
		{
			if (debug && v->index == findIndex)
			{
				outDebugFile << ZShadeSandboxGlobal::Convert::ConvertIntToString(findIndex) << " DISCARDED" << "\n";
			}
		}
	}
}
//===============================================================================================================================
void QuickHull::deleteFacePoints(ZShadeSandboxMath::HullFace* face, ZShadeSandboxMath::HullFace* absorbingFace)
{
	ZShadeSandboxMath::HullVertex* vFace = removeAllPointsFromFace(face);
	
	if (vFace != nullptr)
	{
		if (absorbingFace == nullptr)
			mUnclaimed.addAll(vFace);
		else
		{
			ZShadeSandboxMath::HullVertex* vNext = vFace;
			
			for (ZShadeSandboxMath::HullVertex* v = vNext; v != nullptr; v = vNext)
			{
				vNext = v->next;
				
				float dist = absorbingFace->distanceToPlane(v->point);
				
				if (dist > tolerance)
					addPointToFace(v, absorbingFace);
				else
					mUnclaimed.add(v);
			}
		}
	}
}
//===============================================================================================================================
double QuickHull::oppFaceDistance(ZShadeSandboxMath::HalfEdge* he)
{
	return he->face->distanceToPlane(he->opposite->face->mCentroid);
}
//===============================================================================================================================
void QuickHull::calculateHorizon(ZShadeSandboxMath::XMMath3 eyePnt, ZShadeSandboxMath::HalfEdge* edge0, ZShadeSandboxMath::HullFace* face, vector<ZShadeSandboxMath::HalfEdge*>& horizon)
{
	deleteFacePoints(face, nullptr);
	
	face->mMark = ZShadeSandboxMath::HullFace::DELETED;
	
	if (debug)
	{
		outDebugFile << "  visiting face " << face->getVertexString() << "\n";
	}
	
	ZShadeSandboxMath::HalfEdge* edge;
	
	if (edge0 == nullptr)
	{
		edge0 = face->getEdge(0);
		edge = edge0;
	}
	else
	{
		edge = edge0->next;
	}
	
	do {
		ZShadeSandboxMath::HullFace* oppFace = edge->oppositeFace();
		
		if (oppFace->mMark == HullFace::VISIBLE)
		{
			if (oppFace->distanceToPlane(eyePnt) > tolerance)
			{
				calculateHorizon(eyePnt, edge->opposite, oppFace, horizon);
			}
		}
		else
		{
			horizon.push_back(edge);
			
			if (debug)
			{
				outDebugFile << "  adding horizon edge " << edge->getVertexString() << "\n";
			}
		}
		edge = edge->next;
	} while (edge != edge0);
}
//===============================================================================================================================
void QuickHull::addNewFaces(ZShadeSandboxMath::FaceList& newFaces, ZShadeSandboxMath::HullVertex* eyeVtx, vector<ZShadeSandboxMath::HalfEdge*> horizon)
{
	newFaces.clear();
	
	ZShadeSandboxMath::HalfEdge* heSidePrev = nullptr;
	ZShadeSandboxMath::HalfEdge* heSideBegin = nullptr;
	
	vector<ZShadeSandboxMath::HalfEdge*>::iterator it = horizon.begin();
	for (; it != horizon.end(); it++)
	{
		ZShadeSandboxMath::HalfEdge* horizonHe = (ZShadeSandboxMath::HalfEdge*)(*it)->next;
		
		ZShadeSandboxMath::HalfEdge* heSide = addJoiningFace(eyeVtx, horizonHe);
		
		if (debug)
		{
			outDebugFile << "new face: " << heSide->face->getVertexString() << "\n";
		}
		
		if (heSidePrev != nullptr)
			heSide->next->opposite = heSidePrev;
		else
			heSideBegin = heSide;
		
		newFaces.add(heSide->face);
		
		heSidePrev = heSide;
	}
	
	heSideBegin->next->opposite = heSidePrev;
}
//===============================================================================================================================
ZShadeSandboxMath::HullVertex* QuickHull::nextPointToAdd()
{
	if (!mClaimed.isEmpty())
	{
		ZShadeSandboxMath::HullFace* eyeFace = mClaimed.first()->face;
		
		ZShadeSandboxMath::HullVertex* eyeVtx = nullptr;
		
		float maxDist = 0;
		
		for (ZShadeSandboxMath::HullVertex* v = eyeFace->outside; v != nullptr && v->face == eyeFace; v = v->next)
		{
			float dist = eyeFace->distanceToPlane(v->point);
			
			if (dist > maxDist)
			{
				maxDist = dist;
				eyeVtx = v;
			}
		}
		
		return eyeVtx;
	}
	
	return nullptr;
}
//===============================================================================================================================
void QuickHull::addPointToHull(ZShadeSandboxMath::HullVertex* eyeVtx)
{
	mHorizen.clear();
	mUnclaimed.clear();
	
	if (debug)
	{
		outDebugFile << "Adding point: " << ZShadeSandboxGlobal::Convert::ConvertIntToString(eyeVtx->index) << "\n";
		
		string dist = ZShadeSandboxGlobal::Convert::ConvertFloatToString(eyeVtx->face->distanceToPlane(eyeVtx->point));
		
		outDebugFile << " which is " << dist << " above face " << eyeVtx->face->getVertexString() << "\n";
	}
	
	removePointFromFace(eyeVtx, eyeVtx->face);
	
	calculateHorizon(eyeVtx->point, nullptr, eyeVtx->face, mHorizen);
	
	mNewFaces.clear();
	
	addNewFaces(mNewFaces, eyeVtx, mHorizen);
	
	// first merge pass ... merge faces which are non-convex
	// as determined by the larger face
	
	for (ZShadeSandboxMath::HullFace* face = mNewFaces.first(); face != nullptr; face = face->next)
	{
		if (face->mMark == ZShadeSandboxMath::HullFace::VISIBLE)
		{
			while (doAdjacentMerge(face, NONCONVEX_WRT_LARGER_FACE))
				;
		}
	}
	
	// second merge pass ... merge faces which are non-convex
	// wrt either face
	
	for (ZShadeSandboxMath::HullFace* face = mNewFaces.first(); face != nullptr; face = face->next)
	{
		if (face->mMark == ZShadeSandboxMath::HullFace::NON_CONVEX)
		{
			face->mMark = ZShadeSandboxMath::HullFace::VISIBLE;
			
			while (doAdjacentMerge(face, NONCONVEX))
				;
		}
	}
	
	resolveUnclaimedPoints(mNewFaces);
}
//===============================================================================================================================
void QuickHull::buildHull()
{
	int count = 0;
	
	ZShadeSandboxMath::HullVertex* eyeVtx;
	
	computeMaxAndMin();
	
	createInitialSimplex();
	
	while ((eyeVtx = nextPointToAdd()) != nullptr)
	{
		addPointToHull(eyeVtx);
		
		count++;
		
		if (debug)
		{
			outDebugFile << "iteration " << ZShadeSandboxGlobal::Convert::ConvertIntToString(count) << " done\n";
		}
	}
	
	reindexFacesAndVertices();
	
	if (debug)
	{
		outDebugFile << "hull done\n";
	}
}
//===============================================================================================================================
void QuickHull::reindexFacesAndVertices()
{
	for (int i = 0; i < mNumPoints; i++)
	{
		mPointBuffer[i]->index = -1;
	}
	
	// remove inactive faces and mark active vertices
	mNumFaces = 0;
	
	vector<vector<ZShadeSandboxMath::HullFace*>::iterator> removeFaces;
	vector<ZShadeSandboxMath::HullFace*>::iterator it = mFaces.begin();
	
	for (; it != mFaces.end(); it++)
	{
		ZShadeSandboxMath::HullFace* face = (ZShadeSandboxMath::HullFace*)(*it)->next;
		
		if (face->mMark != ZShadeSandboxMath::HullFace::VISIBLE)
		{
			removeFaces.push_back(it);
		}
		else
		{
			markFaceVertices(face, 0);
			
			mNumFaces++;
		}
	}
	
	// Remove the inactive faces
	for (int i = 0; i < removeFaces.size(); i++)
	{
		mFaces.erase(removeFaces[i]);
	}
	
	// reindex vertices
	mNumVertices = 0;
	
	for (int i = 0; i < mNumPoints; i++)
	{
		ZShadeSandboxMath::HullVertex* v = mPointBuffer[i];
		
		if (v->index == 0)
		{
			mPointIndices[mNumVertices] = i;
			v->index = mNumVertices++;
		}
	}
}
//===============================================================================================================================
bool QuickHull::checkFaceConvexity(ZShadeSandboxMath::HullFace* face, float tol, string& out)
{
	float dist;
	
	ZShadeSandboxMath::HalfEdge* he = face->mHalfEdge;
	
	do {
		string error = face->checkConsistency();
		
		if (BetterString(error) != "")
		{
			// Throw exception for the error
			throw ZShadeSandboxMath::HullException(error);
		}
		
		// make sure edge is convex
		dist = oppFaceDistance(he);
		
		if (dist > tol)
		{
			out += "Edge ";
			out += he->getVertexString().toString();
			out += " non-convex by ";
			out += ZShadeSandboxGlobal::Convert::ConvertFloatToString(dist);
			
			return false;
		}
		
		dist = oppFaceDistance(he->opposite);
		
		if (dist > tol)
		{
			out += "Opposite edge ";
			out += he->opposite->getVertexString().toString();
			out += " non-convex by ";
			out += ZShadeSandboxGlobal::Convert::ConvertFloatToString(dist);
			
			return false;
		}
		
		if (he->next->oppositeFace() == he->oppositeFace())
		{
			out += "Redundant vertex";
			out += ZShadeSandboxGlobal::Convert::ConvertIntToString(he->head()->index);
			out += " in face ";
			out += face->getVertexString();
			
			return false;
		}
		
		he = he->next;
	} while (he != face->mHalfEdge);
	
	return true;
}
//===============================================================================================================================
bool QuickHull::checkFaces(float tol)
{
	// check edge convexity
	bool convex = true;
	
	vector<ZShadeSandboxMath::HullFace*>::iterator it = mFaces.begin();
	
	for (; it != mFaces.end(); it++)
	{
		string out = "";
		
		ZShadeSandboxMath::HullFace* face = (ZShadeSandboxMath::HullFace*)(*it)->next;
		
		if (face->mMark != ZShadeSandboxMath::HullFace::VISIBLE)
		{
			if (!checkFaceConvexity(face, tol, out))
			{
				convex = false;
				
				// Print the out string to a file
				outFaceConvexity << out << "\n";
			}
		}
	}
	
	return convex;
}
//===============================================================================================================================
void QuickHull::printPoints()
{
	outPointsFile.open("QuickHullPoints.txt", ios::out);
	
	// Print the points to a file
	for (int i = 0; i < mNumPoints; i++)
	{
		XMMath3 p = mPointBuffer[i]->point;
		
		string px = ZShadeSandboxGlobal::Convert::ConvertFloatToString(p.x);
		string py = ZShadeSandboxGlobal::Convert::ConvertFloatToString(p.y);
		string pz = ZShadeSandboxGlobal::Convert::ConvertFloatToString(p.z);
		
		outPointsFile << "(" + px + "," + py + "," + pz + ") ";
	}
	
	outPointsFile.close();
}
//===============================================================================================================================
void QuickHull::addPointToFace(ZShadeSandboxMath::HullVertex* vtx, ZShadeSandboxMath::HullFace* face)
{
	vtx->face = face;
	
	if (face->outside == nullptr)
		mClaimed.add(vtx);
	else
		mClaimed.insertBefore(vtx, face->outside);
	
	face->outside = vtx;
}
//===============================================================================================================================
void QuickHull::removePointFromFace(ZShadeSandboxMath::HullVertex* vtx, ZShadeSandboxMath::HullFace* face)
{
	if (vtx == face->outside)
	{
		if (vtx->next != nullptr && vtx->next->face == face)
			face->outside = vtx->next;
		else
			face->outside = nullptr;
	}
	
	mClaimed.erase(vtx);
}
//===============================================================================================================================
ZShadeSandboxMath::HullVertex* QuickHull::removeAllPointsFromFace(ZShadeSandboxMath::HullFace* face)
{
	if (face->outside != nullptr)
	{
		ZShadeSandboxMath::HullVertex* end = face->outside;
		
		while (end->next != nullptr && end->next->face == face)
			end = end->next;
		
		mClaimed.erase(face->outside, end);
		
		end->next = nullptr;
		
		return face->outside;
	}
	
	return nullptr;
}
//===============================================================================================================================
ZShadeSandboxMath::HalfEdge* QuickHull::findHalfEdge(ZShadeSandboxMath::HullVertex* tail, ZShadeSandboxMath::HullVertex* head)
{
	vector<ZShadeSandboxMath::HullFace*>::iterator it = mFaces.begin();
	
	for (; it != mFaces.end(); it++)
	{
		ZShadeSandboxMath::HullFace* face = (ZShadeSandboxMath::HullFace*)(*it)->next;
		
		ZShadeSandboxMath::HalfEdge* he = face->findEdge(tail, head);
		
		if (he != nullptr)
			return he;
	}
	
	return nullptr;
}
//===============================================================================================================================
void QuickHull::getFaceIndices(vector<int>& indices, ZShadeSandboxMath::HullFace* face, int flags)
{
	bool ccw = ((flags & CLOCKWISE) == 0);
	bool indexedFromOne = ((flags & INDEXED_FROM_ONE) != 0);
	bool pointRelative = ((flags & POINT_RELATIVE) != 0);
	
	ZShadeSandboxMath::HalfEdge* he = face->mHalfEdge;
	
	int k = 0;
	
	do {
		int idx = he->head()->index;
		
		if (pointRelative)
			idx = mPointIndices[idx];
		
		if (indexedFromOne)
			idx++;
		
		indices[k++] = idx;
		
		he = (ccw ? he->next : he->prev);
	} while (he != face->mHalfEdge);
}
//===============================================================================================================================
bool QuickHull::doAdjacentMerge(ZShadeSandboxMath::HullFace* face, int mergeType)
{
	ZShadeSandboxMath::HalfEdge* he = face->mHalfEdge;
	
	bool convex = true;
	
	do {
		ZShadeSandboxMath::HullFace* oppFace = he->oppositeFace();
		
		bool merge = false;
		float dist1, dist2;
		
		// merge faces if they are definitively non-convex
		if (mergeType == NONCONVEX)
		{
			if (oppFaceDistance(he) > -tolerance || oppFaceDistance(he->opposite) > -tolerance)
				merge = true;
		}
		else
		{
			// merge faces if they are parallel or non-convex
			// wrt to the larger face; otherwise, just mark
			// the face non-convex for the second pass.
			if (face->mArea > oppFace->mArea)
			{
				if ((dist1 = oppFaceDistance(he)) > -tolerance)
					merge = true;
				else if (oppFaceDistance(he->opposite) > -tolerance)
					convex = false;
			}
			else
			{
				if (oppFaceDistance(he->opposite) > -tolerance)
					merge = true;
				else if (oppFaceDistance(he) > -tolerance)
					convex = false;
			}
		}
		
		if (merge)
		{
			if (debug)
			{
				outDebugFile << "  merging " << face->getVertexString() << "  and  " << oppFace->getVertexString() << "\n";
			}
			
			int numd = face->mergeAdjacentFace(he, mDiscardedFaces);
			
			for (int i = 0; i < numd; i++)
				deleteFacePoints(mDiscardedFaces[i], face);
			
			if (debug)
			{
				outDebugFile << "  result: " << face->getVertexString() << "\n";
			}
			
			return true;
		}
		
		he = he->next;
	} while (he != face->mHalfEdge);
	
	if (!convex)
		face->mMark = ZShadeSandboxMath::HullFace::NON_CONVEX;
	
	return false;
}
//===============================================================================================================================
ZShadeSandboxMath::HalfEdge* QuickHull::addJoiningFace(ZShadeSandboxMath::HullVertex* eyeVtx, ZShadeSandboxMath::HalfEdge* he)
{
	ZShadeSandboxMath::HullFace* face = ZShadeSandboxMath::HullFace::createTriangle(eyeVtx, he->tail(), he->head());
	
	mFaces.push_back(face);
	
	face->getEdge(-1)->opposite = he->opposite;
	
	return face->getEdge(0);
}
//===============================================================================================================================
void QuickHull::markFaceVertices(ZShadeSandboxMath::HullFace* face, int mark)
{
	ZShadeSandboxMath::HalfEdge* he0 = face->getFirstEdge();
	ZShadeSandboxMath::HalfEdge* he = he0;
	
	do {
		he->head()->index = mark;
		he = he->next;
	} while (he != he0);
}
//===============================================================================================================================