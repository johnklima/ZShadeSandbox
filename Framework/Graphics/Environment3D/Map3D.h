//==================================================================================================================================
// Map3D.h
//
// This will represent a 3D map that contains polygons, terrain and/or BSP and entities
// An entity can be a static entity that represents a tree or rock
// Really what I need to do is store entities in the leaf nodes of the terrain quadtree so they can be rendered with the
// Quadtree's LOD scheme.
// Maybe what I can do is add a flag into the terrain Quadtree node that corresponds to all the entities that need to be
// rendered when the node is rendered, which will be a unique ID for each entity on the map.
//==================================================================================================================================
// History
//
// -Created on 3/11/2015 by Dustin Watson
//==================================================================================================================================
#ifndef __MAP3D_H
#define __MAP3D_H
//==================================================================================================================================
//==================================================================================================================================

//
// Includes
//

#include "D3D.h"
#include "ZMath.h"
#include "Camera.h"
#include "PolygonMesh.h"
#include "QuadTree.h"
#include "QuadTreeMesh.h"
#include "QuadTreeRenderer.h"
#include "ShaderMaterial.h"

//==================================================================================================================================
//==================================================================================================================================
/*
	Need to:
	
	1) Load a polygon from xml file
	2) Save a polygon to xml file
	3) Add a point to currently editing polygon
	4) Remove a point from currently editing polygon
	5) Set rendering primitive type for currently editing polygon
	6) Function to rebuild the currently editing polygon
	7) Toggle culling of the polygon to make it visible or not
	8) Function to move the selected vertex from the currently editing polygon
	9) Function to select a polygon based on x,y mouse points (This is done in the editor helper)
	10) Function to select a vertex in currently editing polygon based on x,y mouse points
	11) Scale (resize) the currently editing polygon
	12) Rotate the currently editing polygon
	13) Transform (move) the entire currently editing polygon to a new position
	14) Create a Convex Hull of the currently editing polygon
	15) Assign material to currently editing polygon based on name of material
*/
class Map3D
{
public:
	
	Map3D(D3D* d3d);
	
	// Renders everything in the map
	void Render(ZShadeSandboxMesh::MeshRenderParameters mrp);
	
	//
	// Polygon Functions
	//
	
	// This will begin creating a polygon that starts out with a single point and creates a mesh for it and a NULL material
	void InsertPolygon(ZShadeSandboxMath::XMMath3 point);
	
	// Creates a polygon mesh, locates the material and adds the polygon mesh from the polygon
	void InsertPolygon(ZShadeSandboxMath::Polygon* poly);
	
	// Adds a single point to an existing polygon to the points that it already contains
	void AddPolygonPoint(int id, ZShadeSandboxMath::XMMath3 point);
	
	// Removes a single point from an existing polygon from the points that it already contains
	void RemovePolygonPoint(int id, ZShadeSandboxMath::XMMath3 point);
	
	// Builds a polygon using the points that contains so that it is ready for rendering
	void RebuildPolygon(int id);
	
	// Sets the rendering primitive for a polygon just in case the polygon has < 3 points and cannot be a triangle.
	// Also can be handy for debugging if you wanted to toggle between different primitive types for the polygon.
	void SetPolygonRenderingPrimitiveType(int id, ZShadeSandboxMesh::ERenderType::Type type);
	
	// Self explanatory
	void TogglePolygonVisibility(int id, bool visible);
	
	// Moves a point in the polygon to the moveTo position
	// moveTo is the 3D position that is moving the point which could the Mouse position converted to 3D
	void MovePolygonVertex(int id, ZShadeSandboxMath::XMMath3 point, ZShadeSandboxMath::XMMath3 moveTo);
	
	// Selects a point in the polygon and returns it.
	// moveTo is the 3D position that we are selecting from
	void SelectPolygonVertex(int id, ZShadeSandboxMath::XMMath3 moveTo, ZShadeSandboxMath::XMMath3& point);
	
	// Transformation capability for the polygon
	void ScalePolygon(int id, ZShadeSandboxMath::XMMath3 scaleVector);
	void RotatePolygon(int id, ZShadeSandboxMath::XMMath3 rotationVector);
	void TranslatePolygon(int id, ZShadeSandboxMath::XMMath3 translationVector);
	
	// Generates a convex hull for the polygon
	void CreatePolygonHull(int id);
	
	// Self explanatory
	void AssignPolygonMaterial(int id);
	void AssignPolygonMaterial(int id, string materialName);
	void AssignPolygonMaterial(int id, ZShadeSandboxLighting::ShaderMaterial* material);
	
	ZShadeSandboxMesh::PolygonMesh* GetPolygonMesh(int id);
	
	//
	// Terrain Functions
	//
	
	
	//
	// Entity Functions
	//
	
	
	
private:
	
	void RenderPolygons(ZShadeSandboxMesh::MeshRenderParameters mrp);
	
	
	std::vector<ZShadeSandboxMesh::PolygonMesh*> mPolygons;
	
	// Terrain in the map
	ZShadeSandboxTerrain::QuadTree* mQuadTree;
	ZShadeSandboxTerrain::QuadTreeMesh* mQuadTreeMesh;
	ZShadeSandboxTerrain::QuadTreeRenderer* mQuadTreeRenderer;
	
	D3D* m_D3DSystem;
	EngineOptions* m_EngineOptions;
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__MAP3D_H