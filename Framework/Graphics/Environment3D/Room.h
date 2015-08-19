//==================================================================================================================================
// Room.h
//
// http://codentronix.com/2011/06/18/a-simple-3d-room-made-with-directx-and-c/
// This class will represent a 3D room with 4 walls so that an object or entity can be inside it.
//==================================================================================================================================
// History
//
// -Created on 8/18/2015 by Dustin Watson
//==================================================================================================================================
#ifndef __ROOM_H
#define __ROOM_H
//==================================================================================================================================
//==================================================================================================================================

//
// Includes
//

#include "D3D.h"
#include "ZMath.h"
#include "Camera.h"
#include "MemoryMgr.h"
#include "PolygonMesh.h"
#include "ShaderMaterial.h"

/*
Example:

#define WORLD_Y 9
#define WORLD_X 10

static int iWallMap[WORLD_Y][WORLD_X] =
{
	1,1,1,1,1,1,1,1,1,1,
	1,0,0,0,0,0,0,0,0,1,
	1,0,1,0,0,0,0,1,0,1,
	1,0,1,0,0,0,0,1,0,1,
	1,0,0,0,0,0,0,1,0,1,
	1,0,1,0,0,0,0,1,0,1,
	1,0,1,0,1,1,1,1,0,1,
	1,0,0,0,0,0,0,0,0,1,
	1,1,1,1,1,1,1,1,1,1
};
*/

//==================================================================================================================================
//==================================================================================================================================
class Room
{
public:
	
	Room(D3D* d3d);
	
	// Renders everything
	void Render(ZShadeSandboxMesh::MeshRenderParameters mrp);
	
	// This will build the vertex data for the wall, ceiling and floor polygons
	void Build();
	
	void SetWallMapSize(int w, int h);
	void AddWallMapValue(int x, int y, int value);
	
	// Sets the rendering primitive for a polygon just in case the polygon has < 3 points and cannot be a triangle.
	// Also can be handy for debugging if you wanted to toggle between different primitive types for the polygon.
	//void SetPolygonRenderingPrimitiveType(int id, ZShadeSandboxMesh::ERenderType::Type type);
	
	// Self explanatory
	//void TogglePolygonVisibility(int id, bool visible);
	
	// Selects a point in the polygon and returns it.
	// moveTo is the 3D position that we are selecting from
	//void SelectPolygonVertex(int id, ZShadeSandboxMath::XMMath3 moveTo, ZShadeSandboxMath::XMMath3& point);
	
	// Transformation capability for the polygon
	//void ScalePolygon(int id, ZShadeSandboxMath::XMMath3 scaleVector);
	//void RotatePolygon(int id, ZShadeSandboxMath::XMMath3 rotationVector);
	//void TranslatePolygon(int id, ZShadeSandboxMath::XMMath3 translationVector);
	
	// Generates a convex hull for the polygon
	//void CreatePolygonHull(int id);
	
	// Self explanatory
	//void AssignPolygonMaterial(int id);
	//void AssignPolygonMaterial(int id, string materialName);
	//void AssignPolygonMaterial(int id, ZShadeSandboxLighting::ShaderMaterial* material);
	
	//ZShadeSandboxMesh::PolygonMesh* GetPolygonMesh(int id);
	
private:
	
	void ConstructPolygon(vector<ZShadeSandboxMesh::VertexTex> verts, ZShadeSandboxMesh::PolygonMesh*& poly);
	
	void RenderPolygons(ZShadeSandboxMesh::MeshRenderParameters mrp);
	
	SmartArray2D<int> mWallMap;
	
	// This will hold the walls, ceiling and floor
	ZShadeSandboxMesh::PolygonMesh* mFrontWall;
	ZShadeSandboxMesh::PolygonMesh* mLeftWall;
	ZShadeSandboxMesh::PolygonMesh* mRightWall;
	ZShadeSandboxMesh::PolygonMesh* mBackWall;
	ZShadeSandboxMesh::PolygonMesh* mCeiling;
	ZShadeSandboxMesh::PolygonMesh* mFloor;
	
	D3D* m_D3DSystem;
	EngineOptions* m_EngineOptions;
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__ROOM_H