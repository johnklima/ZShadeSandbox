//===============================================================================================================================
// QuadMesh
//
//===============================================================================================================================
// History
//
// -Created on 7/8/2014 by Dustin Watson
//===============================================================================================================================
#ifndef __QUADMESH_H
#define __QUADMESH_H
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//

#include "ZMath.h"
#include "CustomMesh.h"

//===============================================================================================================================
//===============================================================================================================================
namespace ZShadeSandboxMesh {
class QuadMesh : public ZShadeSandboxMesh::CustomMesh
{
	int m_screenWidth, m_screenHeight;
	int m_bitmapWidth, m_bitmapHeight;
	float m_previousPosX, m_previousPosY;
	bool bUpdatable;
	bool bFullScreen;
	ZShadeSandboxMath::EAxis::Type mAxis;
	
	void CreateQuadVertex6Point(XMFLOAT3 positions[6]);
	
public:
	
	// updatable means means that this quad mesh can have its position updated
	// fullscreen means that this quad mesh is a fullscreen quad
	QuadMesh
	(	D3D* d3d
	,	ZShadeSandboxMesh::MeshParameters mp
	,	int textureWidth
	,	int textureHeight
	,	bool updatable = false
	,	bool fullScreen = false
	,	ZShadeSandboxMath::EAxis::Type axis = ZShadeSandboxMath::EAxis::Type::Axis_Z
	);
	
	~QuadMesh();
	
	//Re-build the dynamic vertex buffer for rendering to a different location on the screen
	void UpdateBuffers(XMFLOAT2 position);
	void UpdateBuffers(XMFLOAT3 position);
	void UpdateBuffers(float positionX, float positionY);
	void UpdateBuffers(XMFLOAT2 position, int width, int height);
	void UpdateBuffers(XMFLOAT3 position, int width, int height);
	void UpdateBuffers(float positionX, float positionY, int width, int height);
	
	void Initialize();
	
	int& ScreenWidth() { return m_screenWidth; }
	int ScreenWidth() const { return m_screenWidth; }
	int& ScreenHeight() { return m_screenHeight; }
	int ScreenHeight() const { return m_screenHeight; }
	int& BitmapWidth() { return m_bitmapWidth; }
	int BitmapWidth() const { return m_bitmapWidth; }
	int& BitmapHeight() { return m_bitmapHeight; }
	int BitmapHeight() const { return m_bitmapHeight; }

	// Loads 4 Quad Points for tessellation
	// If the mesh parameters is set to use tessellation then this will be used
	void InitTessellation();
	
	void InitFullscreenQuad();
};
}
//===============================================================================================================================
//===============================================================================================================================
#endif//__QUADMESH_H