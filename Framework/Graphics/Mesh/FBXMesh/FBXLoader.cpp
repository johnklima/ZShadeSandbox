#include "FBXLoader.h"
#include <algorithm>
#include "ZShadeMessageCenter.h"
#include "FBXHelper.h"
#include "Convert.h"
using ZShadeSandboxMesh::FBXLoader;
// https://github.com/lang1991/FBXExporter/blob/master/FBXExporter/FBXExporter.cpp
//===============================================================================================================================
//===============================================================================================================================
FBXLoader::FBXLoader(D3D* d3d, GameDirectory3D* gd3d)
:	m_pD3DSystem(d3d)
,	m_pGD3D(gd3d)
,   m_pFBXManager(nullptr)
,	m_pFbxScene(nullptr)
,	mTriangleCount(0)
,	bHasAnimation(true)
{
	QueryPerformanceFrequency(&mCPUFreq);
}
//===============================================================================================================================
FBXLoader::~FBXLoader()
{
}
//===============================================================================================================================
void FBXLoader::CleanupFbxManager()
{
	m_pFbxScene->Destroy();
	m_pFBXManager->Destroy();

	mTriangles.clear();
	mVertices.clear();
	mSkeleton.joints.clear();

	for (auto iter = mMaterials.begin(); iter != mMaterials.end(); iter++)
	{
		delete iter->second;
	}

	mMaterials.clear();
}
//===============================================================================================================================
bool FBXLoader::Load(const char* filename)
{
	try
	{
		//
		// Initialize FBX
		//
		
		outFile.open("LoadingFBXMesh.txt", ios::out);
		
		mInputFilePath = string(filename);
		
		if (m_pFBXManager == nullptr)
		{
			m_pFBXManager = FbxManager::Create();
			
			FbxIOSettings* pIOSettings = FbxIOSettings::Create(m_pFBXManager, IOSROOT);
			
			m_pFBXManager->SetIOSettings(pIOSettings);
		}
		
		//
		// Load the scene for the model
		//
		
		LoadScene();
		
		//
		// Load the mesh and skeleton
		//
		
		// Need to write a profiler system that does time performance calculation
		
		LARGE_INTEGER start;
		LARGE_INTEGER end;
		
		QueryPerformanceCounter(&start);
		LoadSkeletonHierarchy(m_pFbxScene->GetRootNode());
		if(mSkeleton.joints.empty())
		{
			bHasAnimation = false;
		}
		
		outFile << "\n\n\n\nLoading FBX Model: " <<  filename << "\n";
		QueryPerformanceCounter(&end);
		float finalTime = ((end.QuadPart - start.QuadPart) / static_cast<float>(mCPUFreq.QuadPart));
		string finalTimeStr = ZShadeSandboxGlobal::Convert::ConvertToString<float>(finalTime);
		outFile << "Processing Skeleton Hierarchy: " << finalTimeStr << "s\n";
		
		QueryPerformanceCounter(&start);
		LoadGeometry(m_pFbxScene->GetRootNode());
		QueryPerformanceCounter(&end);
		finalTime = ((end.QuadPart - start.QuadPart) / static_cast<float>(mCPUFreq.QuadPart));
		finalTimeStr = ZShadeSandboxGlobal::Convert::ConvertToString<float>(finalTime);
		outFile << "Processing Geometry: " << finalTimeStr << "s\n";
		
		QueryPerformanceCounter(&start);
		Optimize();
		QueryPerformanceCounter(&end);
		finalTime = ((end.QuadPart - start.QuadPart) / static_cast<float>(mCPUFreq.QuadPart));
		finalTimeStr = ZShadeSandboxGlobal::Convert::ConvertToString<float>(finalTime);
		outFile << "Optimization: " << finalTimeStr << "s\n";
		
		CleanupFbxManager();
		outFile << "\n\nFBX Loading Done!\n";
		
		outFile.close();
	} catch (std::exception e)
	{
		outFile << "Error loading FBX Model" << e.what() << "!\n";
		outFile.close();
	}
	
	return true;
}
//===============================================================================================================================
bool FBXLoader::LoadScene()
{
	LARGE_INTEGER start;
	LARGE_INTEGER end;
	int fileMinor, fileRevision;
	int sdkMajor, sdkMinor, sdkRevision;
	int fileFormat;
	
	QueryPerformanceCounter(&start);
	{
		FbxString filePath = FbxGetApplicationDirectory();
		m_pFBXManager->LoadPluginsDirectory(filePath.Buffer());
		
		FbxManager::GetFileFormatVersion(sdkMajor, sdkMinor, sdkRevision);
		FbxImporter* pImporter = FbxImporter::Create(m_pFBXManager, "");
		m_pFbxScene = FbxScene::Create(m_pFBXManager, "");
		
		if(!m_pFBXManager->GetIOPluginRegistry()->DetectReaderFileFormat(mInputFilePath.c_str(), fileFormat))
		{
			//Unrecognizable file format. Try to fall back on FbxImorter::eFBX_BINARY
			fileFormat = m_pFBXManager->GetIOPluginRegistry()->FindReaderIDByDescription("FBX binary (*.fbx)");
		}
		
		bool bSuccess = pImporter->Initialize(mInputFilePath.c_str(), fileFormat, m_pFBXManager->GetIOSettings());
		
		pImporter->GetFileVersion(fileMinor, fileMinor, fileRevision);
		
		if(!bSuccess)
		{
			printf( "ERROR %s : %d FbxImporter Initialize failed!\n", __FILE__, __LINE__ );
			return false;
		}
		
		bSuccess = pImporter->Import(m_pFbxScene);
		
		if (!bSuccess)
		{
			return false;
		}
		
		pImporter->Destroy();
	}
	QueryPerformanceCounter(&end);
	
	float finalTime = ((end.QuadPart - start.QuadPart) / static_cast<float>(mCPUFreq.QuadPart));
	string finalTimeStr = ZShadeSandboxGlobal::Convert::ConvertToString<float>(finalTime);
	outFile << "Loading FBX File: " << finalTimeStr << "s\n";
}
//===============================================================================================================================
void FBXLoader::Optimize()
{
	std::vector<ZShadeSandboxMesh::VertexNormalTexBlend> uniqueVertices;
	
	for (uint32 i = 0; i < mTriangles.size(); ++i)
	{
		for (uint32 j = 0; j < 3; ++j)
		{
			// If the current vertex is not in the unique list then add it
			if (FindVertex(mVertices[i * 3 + j], uniqueVertices) == -1)
			{
				uniqueVertices.push_back(mVertices[i * 3 + j]);
			}
		}
	}
	
	// Regenerate the index buffer
	for (uint32 i = 0; i < mTriangles.size(); ++i)
	{
		for (uint32 j = 0; j < 3; ++j)
		{
			mTriangles[i].indices[j] = FindVertex(mVertices[i * 3 + j], uniqueVertices);
		}
	}
	
	mVertices.clear();
	mVertices = uniqueVertices;
	uniqueVertices.clear();
	
	// Sort the triangles by materials
	std::sort(mTriangles.begin(), mTriangles.end());
}
//===============================================================================================================================
int FBXLoader::FindVertex(const ZShadeSandboxMesh::VertexNormalTexBlend& inTargetVertex, const std::vector<ZShadeSandboxMesh::VertexNormalTexBlend>& uniqueVertices)
{
	for (uint32 i = 0; i < uniqueVertices.size(); ++i)
	{
		if (inTargetVertex == uniqueVertices[i])
		{
			return i;
		}
	}
	
	return -1;
}
//===============================================================================================================================
void FBXLoader::LoadMesh(FbxNode* node)
{
	FbxMesh* mesh = node->GetMesh();
	
	mTriangleCount = mesh->GetPolygonCount();
	int vertexCounter = 0;
	mTriangles.reserve(mTriangleCount);
	
	for (uint32 i = 0; i < mTriangleCount; ++i)
	{
		XMFLOAT3 normal[3];
		XMFLOAT3 tangent[3];
		XMFLOAT3 binormal[3];
		XMFLOAT2 uv[3][2];
		
		ZShadeSandboxMesh::FBXTriangle triangle;
		
		mTriangles.push_back(triangle);
		
		for (uint32 j = 0; j < 3; ++j)
		{
			int controlPointIndex = mesh->GetPolygonVertex(i, j);
			ZShadeSandboxMesh::PhysicalPoint* ctlPoint = mControlPoints[controlPointIndex];
			
			LoadVertexNormal(mesh, controlPointIndex, vertexCounter, normal[j]);
			
			// Only have diffuse texture
			for (int k = 0; k < 1; ++k)
			{
				LoadVertexTexture(mesh, controlPointIndex, mesh->GetTextureUVIndex(i, j), k, uv[j][k]);
			}
			
			ZShadeSandboxMesh::VertexNormalTexBlend temp;
			
			temp.position = ctlPoint->position;
			temp.normal = normal[j];
			temp.texture = uv[j][0];
			
			// Copy the blending from each control point
			for (uint32 i = 0; i < ctlPoint->blendingInfo.size(); ++i)
			{
				ZShadeSandboxMesh::VertexBlendingInfo blendingInfo;
				
				blendingInfo.blendingIndex = ctlPoint->blendingInfo[i].blendingIndex;
				blendingInfo.blendingWeight = ctlPoint->blendingInfo[i].blendingWeight;
				
				temp.vertexBlendingInfos.push_back(blendingInfo);
			}
			
			// Sort blending info to remove duplicate vertices
			temp.SortBlendingInfoByWeight();
			
			mVertices.push_back(temp);
			mTriangles.back().indices.push_back(vertexCounter);
			
			++vertexCounter;
		}
	}
	
	// Now mControlPoints has served its purpose so we can free its memory
	for(auto itr = mControlPoints.begin(); itr != mControlPoints.end(); ++itr)
	{
		delete itr->second;
	}
	
	mControlPoints.clear();
}
//===============================================================================================================================
void FBXLoader::LoadVertexTexture(FbxMesh* mesh, int inCtrlPointIndex, int inTextureIndex, int inTextureLayer, XMFLOAT2& outTexture)
{
	if (inTextureLayer >= 2 || mesh->GetElementUVCount() <= inTextureLayer)
	{
		throw std::exception("Invalid UV Layer Number");
	}
	
	FbxGeometryElementUV* vertexUV = mesh->GetElementUV(inTextureLayer);
	
	switch (vertexUV->GetMappingMode())
	{
		case FbxGeometryElement::eByControlPoint:
		{
			switch (vertexUV->GetReferenceMode())
			{
				case FbxGeometryElement::eDirect:
				{
					outTexture.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(inCtrlPointIndex).mData[0]);
					outTexture.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(inCtrlPointIndex).mData[1]);
				}
				break;
				case FbxGeometryElement::eIndexToDirect:
				{
					int index = vertexUV->GetIndexArray().GetAt(inCtrlPointIndex);
					outTexture.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[0]);
					outTexture.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[1]);
				}
				break;
				default: throw std::exception("Invalid Reference");
			}
		}
		break;
		case FbxGeometryElement::eByPolygonVertex:
		{
			switch (vertexUV->GetReferenceMode())
			{
				case FbxGeometryElement::eDirect:
				case FbxGeometryElement::eIndexToDirect:
				{
					outTexture.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(inTextureIndex).mData[0]);
					outTexture.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(inTextureIndex).mData[1]);
				}
				break;
				default: throw std::exception("Invalid Reference");
			}
		}
		break;
	}
}
//===============================================================================================================================
void FBXLoader::LoadVertexNormal(FbxMesh* mesh, int inCtrlPointIndex, int inVertexCounter, XMFLOAT3& outNormal)
{
	if (mesh->GetElementNormalCount() < 1)
	{
		throw std::exception("Invalid Normal Number");
	}
	
	FbxGeometryElementNormal* vertexNormal = mesh->GetElementNormal(0);
	
	switch (vertexNormal->GetMappingMode())
	{
		case FbxGeometryElement::eByControlPoint:
		{
			switch (vertexNormal->GetReferenceMode())
			{
				case FbxGeometryElement::eDirect:
				{
					outNormal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[0]);
					outNormal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[1]);
					outNormal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[2]);
				}
				break;
				case FbxGeometryElement::eIndexToDirect:
				{
					int index = vertexNormal->GetIndexArray().GetAt(inCtrlPointIndex);
					outNormal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
					outNormal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
					outNormal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
				}
				break;
				default: throw std::exception("Invalid Reference");
			}
			break;
		}
		break;
		case FbxGeometryElement::eByPolygonVertex:
		{
			switch (vertexNormal->GetReferenceMode())
			{
				case FbxGeometryElement::eDirect:
				{
					outNormal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inVertexCounter).mData[0]);
					outNormal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inVertexCounter).mData[1]);
					outNormal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inVertexCounter).mData[2]);
				}
				break;
				case FbxGeometryElement::eIndexToDirect:
				{
					int index = vertexNormal->GetIndexArray().GetAt(inVertexCounter);
					outNormal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
					outNormal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
					outNormal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
				}
				break;
				default: throw std::exception("Invalid Reference");
			}
		}
		break;
	}
}
//===============================================================================================================================
void FBXLoader::LoadVertexBinormal(FbxMesh* mesh, int inCtrlPointIndex, int inVertexCounter, XMFLOAT3& outBinormal)
{
	if (mesh->GetElementBinormalCount() < 1)
	{
		throw std::exception("Invalid Binormal Number");
	}

	FbxGeometryElementBinormal* vertexBinormal = mesh->GetElementBinormal(0);
	
	switch (vertexBinormal->GetMappingMode())
	{
		case FbxGeometryElement::eByControlPoint:
		{
			switch (vertexBinormal->GetReferenceMode())
			{
				case FbxGeometryElement::eDirect:
				{
					outBinormal.x = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[0]);
					outBinormal.y = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[1]);
					outBinormal.z = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[2]);
				}
				break;
				case FbxGeometryElement::eIndexToDirect:
				{
					int index = vertexBinormal->GetIndexArray().GetAt(inCtrlPointIndex);
					outBinormal.x = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[0]);
					outBinormal.y = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[1]);
					outBinormal.z = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[2]);
				}
				break;
				default: throw std::exception("Invalid Reference");
			}
		}
		break;
		case FbxGeometryElement::eByPolygonVertex:
		{
			switch (vertexBinormal->GetReferenceMode())
			{
				case FbxGeometryElement::eDirect:
				{
					outBinormal.x = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(inVertexCounter).mData[0]);
					outBinormal.y = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(inVertexCounter).mData[1]);
					outBinormal.z = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(inVertexCounter).mData[2]);
				}
				break;
				case FbxGeometryElement::eIndexToDirect:
				{
					int index = vertexBinormal->GetIndexArray().GetAt(inVertexCounter);
					outBinormal.x = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[0]);
					outBinormal.y = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[1]);
					outBinormal.z = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[2]);
				}
				break;
				default: throw std::exception("Invalid Reference");
			}
		}
		break;
	}
}
//===============================================================================================================================
void FBXLoader::LoadVertexTangent(FbxMesh* mesh, int inCtrlPointIndex, int inVertexCounter, XMFLOAT3& outTangent)
{
	if (mesh->GetElementTangentCount() < 1)
	{
		throw std::exception("Invalid Tangent Number");
	}

	FbxGeometryElementTangent* vertexTangent = mesh->GetElementTangent(0);
	
	switch (vertexTangent->GetMappingMode())
	{
		case FbxGeometryElement::eByControlPoint:
		{
			switch (vertexTangent->GetReferenceMode())
			{
				case FbxGeometryElement::eDirect:
				{
					outTangent.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(inCtrlPointIndex).mData[0]);
					outTangent.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(inCtrlPointIndex).mData[1]);
					outTangent.z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(inCtrlPointIndex).mData[2]);
				}
				break;
				case FbxGeometryElement::eIndexToDirect:
				{
					int index = vertexTangent->GetIndexArray().GetAt(inCtrlPointIndex);
					outTangent.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[0]);
					outTangent.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[1]);
					outTangent.z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[2]);
				}
				break;
				default: throw std::exception("Invalid Reference");
			}
		}
		break;
		case FbxGeometryElement::eByPolygonVertex:
		{
			switch (vertexTangent->GetReferenceMode())
			{
				case FbxGeometryElement::eDirect:
				{
					outTangent.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(inVertexCounter).mData[0]);
					outTangent.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(inVertexCounter).mData[1]);
					outTangent.z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(inVertexCounter).mData[2]);
				}
				break;
				case FbxGeometryElement::eIndexToDirect:
				{
					int index = vertexTangent->GetIndexArray().GetAt(inVertexCounter);
					outTangent.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[0]);
					outTangent.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[1]);
					outTangent.z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[2]);
				}
				break;
				default: throw std::exception("Invalid Reference");
			}
		}
		break;
	}
}
//===============================================================================================================================
uint32 FBXLoader::FindJointIndexUsingName(string& name)
{
	for (uint32 i = 0; i < mSkeleton.joints.size(); ++i)
	{
		if (mSkeleton.joints[i].name == name)
		{
			return i;
		}
	}
	
	throw std::exception("Skeleton information in FBX file is corrupted.");
}
//===============================================================================================================================
void FBXLoader::LoadGeometry(FbxNode* node)
{
	if (node->GetNodeAttribute())
	{
		switch (node->GetNodeAttribute()->GetAttributeType())
		{
			case FbxNodeAttribute::eMesh:
			{
				LoadControlPoints(node);
				
				if (bHasAnimation)
				{
					LoadJointsAndAnimation(node);
				}
				
				LoadMesh(node);
				AssociateMaterialToMesh(node);
				LoadMaterials(node);
			}
			break;
		}
	}
	
	for (int i = 0; i < node->GetChildCount(); ++i)
	{
		LoadGeometry(node->GetChild(i));
	}
}
//===============================================================================================================================
void FBXLoader::LoadControlPoints(FbxNode* node)
{
	FbxMesh* mesh = node->GetMesh();
	uint32 controlPointCount = mesh->GetControlPointsCount();
	
	for (uint32 i = 0; i < controlPointCount; ++i)
	{
		ZShadeSandboxMesh::PhysicalPoint* currCtrlPoint = new ZShadeSandboxMesh::PhysicalPoint();
		
		XMFLOAT3 position;
		position.x = static_cast<float>(mesh->GetControlPointAt(i).mData[0]);
		position.y = static_cast<float>(mesh->GetControlPointAt(i).mData[1]);
		position.z = static_cast<float>(mesh->GetControlPointAt(i).mData[2]);
		currCtrlPoint->position = position;
		
		mControlPoints[i] = currCtrlPoint;
	}
}
//===============================================================================================================================
void FBXLoader::LoadSkeletonHierarchy(FbxNode* inRootNode)
{
	for (int childIndex = 0; childIndex < inRootNode->GetChildCount(); ++childIndex)
	{
		FbxNode* node = inRootNode->GetChild(childIndex);
		
		LoadSkeletonHierarchy(node, 0, 0, -1);
	}
}
//===============================================================================================================================
void FBXLoader::LoadSkeletonHierarchy(FbxNode* inNode, int inDepth, int myIndex, int parentIndex)
{
	if (inNode->GetNodeAttribute() &&
	    inNode->GetNodeAttribute()->GetAttributeType() &&
		inNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		ZShadeSandboxMesh::FBXJoint currJoint;
		currJoint.parentIndex = parentIndex;
		currJoint.name = inNode->GetName();
		mSkeleton.joints.push_back(currJoint);
	}
	
	for (int i = 0; i < inNode->GetChildCount(); i++)
	{
		LoadSkeletonHierarchy(inNode->GetChild(i), inDepth + 1, mSkeleton.joints.size(), myIndex);
	}
}
//===============================================================================================================================
void FBXLoader::LoadJointsAndAnimation(FbxNode* inNode)
{
	FbxMesh* mesh = inNode->GetMesh();
	
	uint32 numOfDeformers = mesh->GetDeformerCount();
	
	FbxAMatrix geomTrans = ZShadeSandboxMesh::FBXHelper::GetGeometryTransformation(inNode);
	
	// A deformer contains clusters.
	// A cluster contains a link, which is a joint.
	// Normally, There is only one deformer in a mesh but Maya has many types.
	for (uint32 deformerIndex = 0; deformerIndex < numOfDeformers; ++deformerIndex)
	{
		// Lets see if this deformer is a skin
		FbxSkin* skin = reinterpret_cast<FbxSkin*>(mesh->GetDeformer(deformerIndex, FbxDeformer::eSkin));
		
		if (!skin) continue;
		
		uint32 numOfClusters = skin->GetClusterCount();
		
		for (uint32 clusterIndex = 0; clusterIndex < numOfClusters; ++clusterIndex)
		{
			FbxCluster* cluster = skin->GetCluster(clusterIndex);
			
			string jointName = cluster->GetLink()->GetName();
			uint32 jointIndex = FindJointIndexUsingName(jointName);
			
			FbxAMatrix transform;
			FbxAMatrix transformLink;
			FbxAMatrix globalBindposeInverse;
			
			// The transformation of the mesh at binding time
			cluster->GetTransformMatrix(transform);
			
			// The transformation of the cluster (joint) at binding time from joint space to world space
			cluster->GetTransformLinkMatrix(transformLink);
			
			globalBindposeInverse = transformLink.Inverse() * transform * geomTrans;
			
			
			// Update skeletal information
			mSkeleton.joints[jointIndex].globalBindposeInverse = globalBindposeInverse;
			mSkeleton.joints[jointIndex].node = cluster->GetLink();
			
			// Associate each joint with the control points it affects
			uint32 numOfIndices = cluster->GetControlPointIndicesCount();
			
			for (uint32 i = 0; i < numOfIndices; ++i)
			{
				ZShadeSandboxMesh::BlendingIndexWeightPair currBlendingIndexWeightPair;
				currBlendingIndexWeightPair.blendingIndex = jointIndex;
				currBlendingIndexWeightPair.blendingWeight = cluster->GetControlPointWeights()[i];
				mControlPoints[cluster->GetControlPointIndices()[i]]->blendingInfo.push_back(currBlendingIndexWeightPair);
			}
			
			// Animation information
			FbxAnimStack* animStack = m_pFbxScene->GetSrcObject<FbxAnimStack>(0);
			FbxString animStackName = animStack->GetName();
			mAnimationName = animStackName.Buffer();
			FbxTakeInfo* takeInfo = m_pFbxScene->GetTakeInfo(animStackName);
			FbxTime start = takeInfo->mLocalTimeSpan.GetStart();
			FbxTime end = takeInfo->mLocalTimeSpan.GetStop();
			mAnimationLength = end.GetFrameCount(FbxTime::eFrames24) - start.GetFrameCount(FbxTime::eFrames24) + 1;
			FBXKeyframe** anim = &mSkeleton.joints[jointIndex].animation;
			
			for (FbxLongLong i = start.GetFrameCount(FbxTime::eFrames24); i <= end.GetFrameCount(FbxTime::eFrames24); ++i)
			{
				FbxTime time;
				time.SetFrame(i, FbxTime::eFrames24);
				*anim = new FBXKeyframe();
				(*anim)->frameNum = i;
				FbxAMatrix currentTransformOffset = inNode->EvaluateGlobalTransform(time) * geomTrans;
				(*anim)->globalTransform = currentTransformOffset.Inverse() * cluster->GetLink()->EvaluateGlobalTransform(time);
				anim = &((*anim)->next);
			}
		}
	}
	
	// Some control points have less than 4 joints
	ZShadeSandboxMesh::BlendingIndexWeightPair currBlendingIndexWeightPair;
	currBlendingIndexWeightPair.blendingIndex = 0;
	currBlendingIndexWeightPair.blendingWeight = 0;
	for(auto itr = mControlPoints.begin(); itr != mControlPoints.end(); ++itr)
	{
		for(unsigned int i = itr->second->blendingInfo.size(); i <= 4; ++i)
		{
			itr->second->blendingInfo.push_back(currBlendingIndexWeightPair);
		}
	}
}
//===============================================================================================================================
void FBXLoader::AssociateMaterialToMesh(FbxNode* inNode)
{
	FbxLayerElementArrayTemplate<int>* materialIndices;
	FbxGeometryElement::EMappingMode materialMappingMode = FbxGeometryElement::eNone;
	FbxMesh* mesh = inNode->GetMesh();
	
	if (mesh->GetElementMaterial())
	{
		materialIndices = &(mesh->GetElementMaterial()->GetIndexArray());
		materialMappingMode = mesh->GetElementMaterial()->GetMappingMode();
		
		if (materialIndices)
		{
			switch (materialMappingMode)
			{
				case FbxGeometryElement::eByPolygon:
				{
					if (materialIndices->GetCount() == mTriangleCount)
					{
						for (uint32 i = 0; i < mTriangleCount; ++i)
						{
							uint32 materialIndex = materialIndices->GetAt(i);
							mTriangles[i].materialIndex = materialIndex;
						}
					}
				}
				break;
				case FbxGeometryElement::eAllSame:
				{
					uint32 materialIndex = materialIndices->GetAt(0);
					for (uint32 i = 0; i < mTriangleCount; ++i)
					{
						mTriangles[i].materialIndex = materialIndex;
					}
				}
				break;
				default: throw std::exception("Invalid mapping mode for material\n");
			}
		}
	}
}
//===============================================================================================================================
void FBXLoader::LoadMaterials(FbxNode* inNode)
{
	uint32 materialCount = inNode->GetMaterialCount();
	
	for (uint32 i = 0; i < materialCount; ++i)
	{
		FbxSurfaceMaterial* surfaceMaterial = inNode->GetMaterial(i);
		LoadMaterialAttribute(surfaceMaterial, i);
		LoadMaterialTexture(surfaceMaterial, mMaterials[i]);
	}
}
//===============================================================================================================================
void FBXLoader::LoadMaterialAttribute(FbxSurfaceMaterial* inMaterial, uint32 inMaterialIndex)
{
	FbxDouble3 double3;
	FbxDouble double1;
	
	if (inMaterial->GetClassId().Is(FbxSurfacePhong::ClassId))
	{
		ZShadeSandboxLighting::ShaderMaterial* material = new ZShadeSandboxLighting::ShaderMaterial();
		
		// Ambient Color
		double3 = reinterpret_cast<FbxSurfacePhong*>(inMaterial)->Ambient;
		material->vAmbientColor.x = static_cast<float>(double3.mData[0]);
		material->vAmbientColor.y = static_cast<float>(double3.mData[1]);
		material->vAmbientColor.z = static_cast<float>(double3.mData[2]);
		
		// Diffuse Color
		double3 = reinterpret_cast<FbxSurfacePhong*>(inMaterial)->Diffuse;
		material->vDiffuseColor.x = static_cast<float>(double3.mData[0]);
		material->vDiffuseColor.y = static_cast<float>(double3.mData[1]);
		material->vDiffuseColor.z = static_cast<float>(double3.mData[2]);
		
		// Specular Color
		double3 = reinterpret_cast<FbxSurfacePhong*>(inMaterial)->Specular;
		material->vSpecularColor.x = static_cast<float>(double3.mData[0]);
		material->vSpecularColor.y = static_cast<float>(double3.mData[1]);
		material->vSpecularColor.z = static_cast<float>(double3.mData[2]);
		
		// Emissive Color
		double3 = reinterpret_cast<FbxSurfacePhong*>(inMaterial)->Emissive;
		material->vEmissiveColor.x = static_cast<float>(double3.mData[0]);
		material->vEmissiveColor.y = static_cast<float>(double3.mData[1]);
		material->vEmissiveColor.z = static_cast<float>(double3.mData[2]);
		
		// Reflection
		double3 = reinterpret_cast<FbxSurfacePhong*>(inMaterial)->Reflection;
		material->vReflection.x = static_cast<float>(double3.mData[0]);
		material->vReflection.y = static_cast<float>(double3.mData[1]);
		material->vReflection.z = static_cast<float>(double3.mData[2]);
		
		// Alpha transparency factor
		double1 = reinterpret_cast<FbxSurfacePhong*>(inMaterial)->TransparencyFactor;
		material->fAlpha = double1;
		
		// Material Shininess
		double1 = reinterpret_cast<FbxSurfacePhong*>(inMaterial)->Shininess;
		material->fSpecularIntensity = double1;
		
		// Specular Power
		double1 = reinterpret_cast<FbxSurfacePhong*>(inMaterial)->SpecularFactor;
		material->fSpecularPower = double1;
		
		// Material reflectivity factor
		double1 = reinterpret_cast<FbxSurfacePhong*>(inMaterial)->ReflectionFactor;
		material->fReflectivity = double1;
		
		mMaterials[inMaterialIndex] = material;
	}
	else if (inMaterial->GetClassId().Is(FbxSurfaceLambert::ClassId))
	{
		ZShadeSandboxLighting::ShaderMaterial* material = new ZShadeSandboxLighting::ShaderMaterial();
		
		// Ambient Color
		double3 = reinterpret_cast<FbxSurfacePhong*>(inMaterial)->Ambient;
		material->vAmbientColor.x = static_cast<float>(double3.mData[0]);
		material->vAmbientColor.y = static_cast<float>(double3.mData[1]);
		material->vAmbientColor.z = static_cast<float>(double3.mData[2]);
		
		// Diffuse Color
		double3 = reinterpret_cast<FbxSurfacePhong*>(inMaterial)->Diffuse;
		material->vDiffuseColor.x = static_cast<float>(double3.mData[0]);
		material->vDiffuseColor.y = static_cast<float>(double3.mData[1]);
		material->vDiffuseColor.z = static_cast<float>(double3.mData[2]);
		
		// Emissive Color
		double3 = reinterpret_cast<FbxSurfacePhong*>(inMaterial)->Emissive;
		material->vEmissiveColor.x = static_cast<float>(double3.mData[0]);
		material->vEmissiveColor.y = static_cast<float>(double3.mData[1]);
		material->vEmissiveColor.z = static_cast<float>(double3.mData[2]);
		
		// Alpha transparency factor
		double1 = reinterpret_cast<FbxSurfacePhong*>(inMaterial)->TransparencyFactor;
		material->fAlpha = double1;
		
		mMaterials[inMaterialIndex] = material;
	}
}
//===============================================================================================================================
void FBXLoader::LoadMaterialTexture(FbxSurfaceMaterial* inMaterial, ZShadeSandboxLighting::ShaderMaterial* ioMaterial)
{
	uint32 textureIndex = 0;
	FbxProperty property;
	
	FBXSDK_FOR_EACH_TEXTURE(textureIndex)
	{
		property = inMaterial->FindProperty(FbxLayerElement::sTextureChannelNames[textureIndex]);
		if (property.IsValid())
		{
			uint32 textureCount = property.GetSrcObjectCount<FbxTexture>();
			for (uint32 i = 0; i < textureCount; ++i)
			{
				FbxLayeredTexture* layeredTexture = property.GetSrcObject<FbxLayeredTexture>(i);
				if(layeredTexture)
				{
					throw std::exception("Layered Texture is currently unsupported\n");
				}
				else
				{
					FbxTexture* texture = property.GetSrcObject<FbxTexture>(i);
					if (texture)
					{
						std::string textureType = property.GetNameAsCStr();
						FbxFileTexture* fileTexture = FbxCast<FbxFileTexture>(texture);

						if (fileTexture)
						{
							if (textureType == "DiffuseColor")
							{
								ioMaterial->sDiffuseTextureName = fileTexture->GetFileName();
								ioMaterial->bHasDiffuseTexture = true;
							}
							else if (textureType == "SpecularColor")
							{
								ioMaterial->sSpecularTextureName = fileTexture->GetFileName();
								ioMaterial->bHasSpecularTexture = true;
							}
							else if (textureType == "Bump")
							{
								ioMaterial->sNormalMapTextureName = fileTexture->GetFileName();
								ioMaterial->bHasNormalMapTexture = true;
							}
						}
					}
				}
			}
		}
	}
	
	ioMaterial->SetD3D(m_pD3DSystem);
	
	// Load the textures
	if (ioMaterial->bHasDiffuseTexture) ioMaterial->AddDiffuseTexture(m_pGD3D->m_textures_path, ioMaterial->sDiffuseTextureName);
	if (ioMaterial->bHasSpecularTexture) ioMaterial->AddSpecularTexture(m_pGD3D->m_textures_path, ioMaterial->sSpecularTextureName);
	if (ioMaterial->bHasNormalMapTexture) ioMaterial->AddNormalMapTexture(m_pGD3D->m_textures_path, ioMaterial->sNormalMapTextureName);
}
//===============================================================================================================================