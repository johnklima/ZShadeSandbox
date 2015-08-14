#include "Map3D.h"
#include "MaterialManager.h"
#include "Vertex.h"
//==================================================================================================================================
//==================================================================================================================================
Map3D::Map3D(D3D* d3d)
{
	
}
//==================================================================================================================================
void Map3D::Render(ZShadeSandboxMesh::MeshRenderParameters mrp)
{
	RenderPolygons(mrp);
	
	// ...
}
//==================================================================================================================================
ZShadeSandboxMesh::PolygonMesh* Map3D::GetPolygonMesh(int id)
{
	return mPolygons[id];
}
//==================================================================================================================================
void Map3D::InsertPolygon(ZShadeSandboxMath::XMMath3 point)
{
	ZShadeSandboxMath::Polygon* poly = new ZShadeSandboxMath::Polygon();
	
	vector<ZShadeSandboxMesh::VertexTex> vertices(1);
	vector<UINT> indices(1);
	
	vertices[0] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(point.x, point.y, point.z, 0, 0);
	indices[0] = 0;
	
	poly->Name() = "";
	poly->Create(vertices);
	poly->CreateIndices(indices);
	poly->Position() = XMFLOAT3(point.x, point.y, point.z);
	poly->Rotation() = XMFLOAT3(0, 0, 0);
	poly->Scale() = XMFLOAT3(1, 1, 1);
	poly->MaterialName() = "";
	
	ZShadeSandboxMesh::PolygonMesh* polyMesh = new ZShadeSandboxMesh::PolygonMesh(poly, 0);
	
	mPolygons.push_back(polyMesh);
}
//==================================================================================================================================
// Creates a polygon mesh, locates the material and adds the polygon mesh from the polygon
void Map3D::InsertPolygon(ZShadeSandboxMath::Polygon* poly)
{
	ZShadeSandboxMesh::PolygonMesh* polyMesh = new ZShadeSandboxMesh::PolygonMesh(
		poly,
		MaterialManager::Instance()->GetMaterial(poly->MaterialName())
	);
	
	mPolygons.push_back(polyMesh);
}
//==================================================================================================================================
// Adds a single point to an existing polygon to the points that it already contains
void Map3D::AddPolygonPoint(int id, ZShadeSandboxMath::XMMath3 point)
{
	ZShadeSandboxMesh::PolygonMesh* polyMesh = GetPolygonMesh(id);
	
	polyMesh->Poly()->AddVertex(ZShadeSandboxMesh::VertexUtil::LoadVertexTex(point.x, point.y, point.z, 0, 0));
}
//==================================================================================================================================
// Removes a single point from an existing polygon from the points that it already contains
void Map3D::RemovePolygonPoint(int id, ZShadeSandboxMath::XMMath3 point)
{
	
}
//==================================================================================================================================
// Builds a polygon using the points that contains so that it is ready for rendering
void Map3D::RebuildPolygon(int id)
{
	
}
//==================================================================================================================================
// Sets the rendering primitive for a polygon just in case the polygon has < 3 points and cannot be a triangle.
// Also can be handy for debugging if you wanted to toggle between different primitive types for the polygon.
void Map3D::SetPolygonRenderingPrimitiveType(int id, ZShadeSandboxMesh::ERenderType::Type type)
{
	
}
//==================================================================================================================================
void Map3D::TogglePolygonVisibility(int id, bool visible)
{
	
}
//==================================================================================================================================
// Moves a point in the polygon to the moveTo position
// moveTo is the 3D position that is moving the point which could the Mouse position converted to 3D
void Map3D::MovePolygonVertex(int id, ZShadeSandboxMath::XMMath3 point, ZShadeSandboxMath::XMMath3 moveTo)
{
	
}
//==================================================================================================================================
// Selects a point in the polygon and returns it.
// moveTo is the 3D position that we are selecting from
void Map3D::SelectPolygonVertex(int id, ZShadeSandboxMath::XMMath3 moveTo, ZShadeSandboxMath::XMMath3& point)
{
	
}
//==================================================================================================================================
// Transformation capability for the polygon
void Map3D::ScalePolygon(int id, ZShadeSandboxMath::XMMath3 scaleVector)
{
	
}
//==================================================================================================================================
void Map3D::RotatePolygon(int id, ZShadeSandboxMath::XMMath3 rotationVector)
{
	
}
//==================================================================================================================================
void Map3D::TranslatePolygon(int id, ZShadeSandboxMath::XMMath3 translationVector)
{
	
}
//==================================================================================================================================
void Map3D::CreatePolygonHull(int id)
{
	
}
//==================================================================================================================================
void Map3D::AssignPolygonMaterial(int id)
{
	
}
//==================================================================================================================================
void Map3D::AssignPolygonMaterial(int id, string materialName)
{
	
}
//==================================================================================================================================
void Map3D::AssignPolygonMaterial(int id, ZShadeSandboxLighting::ShaderMaterial* material)
{
	
}
//==================================================================================================================================
void Map3D::RenderPolygons(ZShadeSandboxMesh::MeshRenderParameters mrp)
{
	
}
//==================================================================================================================================
