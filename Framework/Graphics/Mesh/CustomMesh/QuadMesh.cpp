#include "QuadMesh.h"
using ZShadeSandboxMesh::QuadMesh;
//===============================================================================================================================
//===============================================================================================================================
QuadMesh::QuadMesh
(	D3D* d3d
,	ZShadeSandboxMesh::MeshParameters mp
,	int textureWidth
,	int textureHeight
,	bool updatable
,	bool fullScreen
,	ZShadeSandboxMath::EAxis::Type axis
)
:   ZShadeSandboxMesh::CustomMesh( d3d, mp )
,   bUpdatable(updatable)
,   bFullScreen(fullScreen)
,	mAxis(axis)
{
	//Store the screen size
	if (updatable)
	{
		m_screenWidth = mEngineOptions->m_screenWidth;
		m_screenHeight = mEngineOptions->m_screenHeight;
	}
	else
	{
		m_screenWidth = textureWidth;
		m_screenHeight = textureHeight;
	}
	
	//Store the size in pixels that this bitmap should be rendered at
	m_bitmapWidth = textureWidth;
	m_bitmapHeight = textureHeight;
	
	//Initialize the previous rendering position to negative one
	m_previousPosX = -1;
	m_previousPosY = -1;
	
	mMeshType = ZShadeSandboxMesh::EMeshType::QUAD;

	InitializeMesh();
	CreateBuffers();
}
//===============================================================================================================================
QuadMesh::~QuadMesh()
{
}
//===============================================================================================================================
void QuadMesh::UpdateBuffers(XMFLOAT2 position)
{
	UpdateBuffers(position.x, position.y);
}
//===============================================================================================================================
void QuadMesh::UpdateBuffers(XMFLOAT3 position)
{
	UpdateBuffers(position.x, position.y);
}
//===============================================================================================================================
void QuadMesh::UpdateBuffers(XMFLOAT2 position, int width, int height)
{
	m_bitmapWidth = width;
	m_bitmapHeight = height;
	UpdateBuffers(position.x, position.y);
}
//===============================================================================================================================
void QuadMesh::UpdateBuffers(XMFLOAT3 position, int width, int height)
{
	m_bitmapWidth = width;
	m_bitmapHeight = height;
	UpdateBuffers(position.x, position.y);
}
//===============================================================================================================================
void QuadMesh::UpdateBuffers(float positionX, float positionY, int width, int height)
{
	m_bitmapWidth = width;
	m_bitmapHeight = height;
	UpdateBuffers(positionX, positionY);
}
//===============================================================================================================================
void QuadMesh::UpdateBuffers(float positionX, float positionY)
{
	if (bUpdatable)
	{
		mAttributes->mPosition.x = positionX;
		mAttributes->mPosition.y = positionY;

		float left, right, top, bottom;

		//If the position we are rendering this bitmap to has not changed then don't update the vertex buffer
		//since it currently has the correct parameters
		if ((positionX == m_previousPosX) && (positionY == m_previousPosY))
		{
			return;
		}

		//If it has changed then update the position it is being rendered to
		m_previousPosX = positionX;
		m_previousPosY = positionY;

		//Calculate the screen coordinates of the left side of the bitmap
		left = (float)((m_screenWidth / 2) * -1) + (float)positionX;

		//Calculate the screen coordinates of the right side of the bitmap
		right = left + (float)m_bitmapWidth;

		//Calculate the screen coordinates of the top of the bitmap
		top = (float)(m_screenHeight / 2) - (float)positionY;

		//Calculate the screen coordinates of the bottom of the bitmap
		bottom = top - (float)m_bitmapHeight;
		
		
		XMFLOAT3 positions[6];
		
		switch (mAxis)
		{
			case ZShadeSandboxMath::EAxis::Type::Axis_X:
			{
				positions[0] = XMFLOAT3(0.0f, left, top);
				positions[1] = XMFLOAT3(0.0f, right, bottom);
				positions[2] = XMFLOAT3(0.0f, left, bottom);
				positions[3] = XMFLOAT3(0.0f, left, top);
				positions[4] = XMFLOAT3(0.0f, right, top);
				positions[5] = XMFLOAT3(0.0f, right, bottom);
			}
			break;
			case ZShadeSandboxMath::EAxis::Type::Axis_Y:
			{
				positions[0] = XMFLOAT3(left, 0.0f, top);
				positions[1] = XMFLOAT3(right, 0.0f, bottom);
				positions[2] = XMFLOAT3(left, 0.0f, bottom);
				positions[3] = XMFLOAT3(left, 0.0f, top);
				positions[4] = XMFLOAT3(right, 0.0f, top);
				positions[5] = XMFLOAT3(right, 0.0f, bottom);
			}
			break;
			case ZShadeSandboxMath::EAxis::Type::Axis_Z:
			{
				positions[0] = XMFLOAT3(left, top, 0.0f);
				positions[1] = XMFLOAT3(right, bottom, 0.0f);
				positions[2] = XMFLOAT3(left, bottom, 0.0f);
				positions[3] = XMFLOAT3(left, top, 0.0f);
				positions[4] = XMFLOAT3(right, top, 0.0f);
				positions[5] = XMFLOAT3(right, bottom, 0.0f);
			}
			break;
		}
		
		CreateQuadVertex6Point(positions);
		
		// Regenerate the vertex buffer
		RegenVertexBuffer();
	}
}
//===============================================================================================================================
void QuadMesh::Initialize()
{
	if (bFullScreen)
	{
		InitFullscreenQuad();
	}
	else
	{
		//
		// Load Vertices
		//
		
		XMFLOAT3 positions[6];
		
		switch (mAxis)
		{
			case ZShadeSandboxMath::EAxis::Type::Axis_X:
			{
				positions[0] = XMFLOAT3(0, 0, 0);
				positions[1] = XMFLOAT3(0, 1, 1);
				positions[2] = XMFLOAT3(0, 0, 1);
				positions[3] = XMFLOAT3(0, 0, 0);
				positions[4] = XMFLOAT3(0, 1, 0);
				positions[5] = XMFLOAT3(0, 1, 1);
			}
			break;
			case ZShadeSandboxMath::EAxis::Type::Axis_Y:
			{
				positions[0] = XMFLOAT3(0, 0, 0);
				positions[1] = XMFLOAT3(1, 0, 1);
				positions[2] = XMFLOAT3(0, 0, 1);
				positions[3] = XMFLOAT3(0, 0, 0);
				positions[4] = XMFLOAT3(1, 0, 0);
				positions[5] = XMFLOAT3(1, 0, 1);
			}
			break;
			case ZShadeSandboxMath::EAxis::Type::Axis_Z:
			{
				positions[0] = XMFLOAT3(0, 0, 0);
				positions[1] = XMFLOAT3(1, 1, 0);
				positions[2] = XMFLOAT3(0, 1, 0);
				positions[3] = XMFLOAT3(0, 0, 0);
				positions[4] = XMFLOAT3(1, 0, 0);
				positions[5] = XMFLOAT3(1, 1, 0);
			}
			break;
		}
		
		CreateQuadVertex6Point(positions);
		
		mAttributes->mTriangleCount = 2;
		
		//
		// Load Indices
		//
		
		mAttributes->mIndexCount = 6;
		
		UINT indices[6] =
		{
			0, 1, 2, 3, 4, 5
		};

		mAttributes->mIndices.resize(6);

		for (int i = 0; i < 6; i++)
		{
			mAttributes->mIndices[i] = indices[i];
		}
	}
}
//===============================================================================================================================
void QuadMesh::InitTessellation()
{
	// Just creates a fullscreen quad with tessellation
	InitFullscreenQuad();
}
//===============================================================================================================================
void QuadMesh::InitFullscreenQuad()
{
	//
	// Load Vertices
	//
	
	XMFLOAT3 v0(0, 0, 0);
	XMFLOAT3 v1(0, 0, 0);
	XMFLOAT3 v2(0, 0, 0);
	XMFLOAT3 v3(0, 0, 0);
	
	switch (mAxis)
	{
		case ZShadeSandboxMath::EAxis::Type::Axis_X:
		{
			v0 = XMFLOAT3(0, -1,  1);
			v1 = XMFLOAT3(0,  1,  1);
			v2 = XMFLOAT3(0, -1, -1);
			v3 = XMFLOAT3(0,  1, -1);
		}
		break;
		case ZShadeSandboxMath::EAxis::Type::Axis_Y:
		{
			v0 = XMFLOAT3(-1, 0,  1);
			v1 = XMFLOAT3( 1, 0,  1);
			v2 = XMFLOAT3(-1, 0, -1);
			v3 = XMFLOAT3( 1, 0, -1);
		}
		break;
		case ZShadeSandboxMath::EAxis::Type::Axis_Z:
		{
			v0 = XMFLOAT3(-1,  1, 0);
			v1 = XMFLOAT3( 1,  1, 0);
			v2 = XMFLOAT3(-1, -1, 0);
			v3 = XMFLOAT3( 1, -1, 0);
		}
		break;
	}
	
	switch (mMeshParameters.vertexType)
	{
		case ZShadeSandboxMesh::EVertexType::VT_NormalTex:
		{
			ZShadeSandboxMesh::VertexNormalTex c1 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(v0.x, v0.y, v0.z, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTex c2 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(v1.x, v1.y, v1.z, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTex c3 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(v2.x, v2.y, v2.z, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
			ZShadeSandboxMesh::VertexNormalTex c4 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(v3.x, v3.y, v3.z, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);
			
			ZShadeSandboxMesh::VertexNormalTex verts[4] =
			{
				c1, c2, c3, c4
			};
			
			mAttributes->mVerticesNormalTex.resize( 4 );

			for (int i = 0; i < 4; i++)
			{
				mAttributes->mVerticesNormalTex[i] = verts[i];
			}
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_NormalTexTan:
		{
			ZShadeSandboxMesh::VertexNormalTexTan c1 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(v0.x, v0.y, v0.z, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTexTan c2 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(v1.x, v1.y, v1.z, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTexTan c3 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(v2.x, v2.y, v2.z, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTexTan c4 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(v3.x, v3.y, v3.z, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
			
			ZShadeSandboxMesh::VertexNormalTexTan verts[4] =
			{
				c1, c2, c3, c4
			};
			
			mAttributes->mVerticesNormalTexTan.resize( 4 );

			for (int i = 0; i < 4; i++)
			{
				mAttributes->mVerticesNormalTexTan[i] = verts[i];
			}
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Pos:
		{
			ZShadeSandboxMesh::VertexPos c1 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos(v0.x, v0.y, v0.z);
			ZShadeSandboxMesh::VertexPos c2 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos(v1.x, v1.y, v1.z);
			ZShadeSandboxMesh::VertexPos c3 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos(v2.x, v2.y, v2.z);
			ZShadeSandboxMesh::VertexPos c4 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos(v3.x, v3.y, v3.z);
			
			ZShadeSandboxMesh::VertexPos verts[4] =
			{
				c1, c2, c3, c4
			};
			
			mAttributes->mVerticesPos.resize( 4 );

			for (int i = 0; i < 4; i++)
			{
				mAttributes->mVerticesPos[i] = verts[i];
			}
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Tex:
		{
			ZShadeSandboxMesh::VertexTex c1 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(v0.x, v0.y, v0.z, 0, 0);
			ZShadeSandboxMesh::VertexTex c2 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(v1.x, v1.y, v1.z, 1, 0);
			ZShadeSandboxMesh::VertexTex c3 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(v2.x, v2.y, v2.z, 0, 1);
			ZShadeSandboxMesh::VertexTex c4 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(v3.x, v3.y, v3.z, 1, 1);

			ZShadeSandboxMesh::VertexTex verts[4] =
			{
				c1, c2, c3, c4
			};
			
			mAttributes->mVerticesTex.resize( 4 );

			for (int i = 0; i < 4; i++)
			{
				mAttributes->mVerticesTex[i] = verts[i];
			}
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Color:
		{
			XMFLOAT4 diffuseColor = mMaterial->vDiffuseColor;

			ZShadeSandboxMesh::VertexColor c1 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(v0.x, v0.y, v0.z, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
			ZShadeSandboxMesh::VertexColor c2 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(v1.x, v1.y, v1.z, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
			ZShadeSandboxMesh::VertexColor c3 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(v2.x, v2.y, v2.z, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
			ZShadeSandboxMesh::VertexColor c4 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(v3.x, v3.y, v3.z, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
			
			ZShadeSandboxMesh::VertexColor verts[4] =
			{
				c1, c2, c3, c4
			};
			
			mAttributes->mVerticesColor.resize( 4 );

			for (int i = 0; i < 4; i++)
			{
				mAttributes->mVerticesColor[i] = verts[i];
			}
		}
		break;
	}
	
	mAttributes->mVertexCount = 4;
	
	mAttributes->mTriangleCount = 2;
	
	//
	// Load Indices
	//
	
	mAttributes->mIndexCount = 6;
	
	UINT indices[6] =
	{
		0, 1, 2, 3, 0, 2
	};
	
	mAttributes->mIndices.resize(6);
	
	for (int i = 0; i < 6; i++)
	{
		mAttributes->mIndices[i] = indices[i];
	}
}
//===============================================================================================================================
void QuadMesh::CreateQuadVertex6Point(XMFLOAT3 positions[6])
{
	//Load the vertex array with data
	switch (mMeshParameters.vertexType)
	{
		case ZShadeSandboxMesh::EVertexType::VT_NormalTex:
		{
#pragma region "NormalTex"
			//First Triangle
			ZShadeSandboxMesh::VertexNormalTex c1 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(positions[0].x, positions[0].y, positions[0].z, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTex c2 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(positions[1].x, positions[1].y, positions[1].z, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);
			ZShadeSandboxMesh::VertexNormalTex c3 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(positions[2].x, positions[2].y, positions[2].z, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
			
			//Second Triangle
			ZShadeSandboxMesh::VertexNormalTex c4 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(positions[3].x, positions[3].y, positions[3].z, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTex c5 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(positions[4].x, positions[4].y, positions[4].z, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTex c6 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(positions[5].x, positions[5].y, positions[5].z, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);
			
			ZShadeSandboxMesh::VertexNormalTex verts[6] =
			{
				c1, c2, c3, c4, c5, c6
			};
			
			mAttributes->mVerticesNormalTex.resize( 6 );

			for (int i = 0; i < 6; i++)
			{
				mAttributes->mVerticesNormalTex[i] = verts[i];
			}

			mAttributes->mVertexCount = 6;
#pragma endregion
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_NormalTexTan:
		{
#pragma region "NormalTexTan"
			//First Triangle
			ZShadeSandboxMesh::VertexNormalTexTan c1 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(positions[0].x, positions[0].y, positions[0].z, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTexTan c2 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(positions[1].x, positions[1].y, positions[1].z, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTexTan c3 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(positions[2].x, positions[2].y, positions[2].z, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
			
			//Second Triangle
			ZShadeSandboxMesh::VertexNormalTexTan c4 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(positions[3].x, positions[3].y, positions[3].z, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTexTan c5 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(positions[4].x, positions[4].y, positions[4].z, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTexTan c6 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(positions[5].x, positions[5].y, positions[5].z, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
			
			ZShadeSandboxMesh::VertexNormalTexTan verts[6] =
			{
				c1, c2, c3, c4, c5, c6
			};
			
			mAttributes->mVerticesNormalTexTan.resize( 6 );

			for (int i = 0; i < 6; i++)
			{
				mAttributes->mVerticesNormalTexTan[i] = verts[i];
			}

			mAttributes->mVertexCount = 6;
#pragma endregion
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Pos:
		{
#pragma region "Pos"
			//First Triangle
			ZShadeSandboxMesh::VertexPos c1 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos(positions[0].x, positions[0].y, positions[0].z);
			ZShadeSandboxMesh::VertexPos c2 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos(positions[1].x, positions[1].y, positions[1].z);
			ZShadeSandboxMesh::VertexPos c3 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos(positions[2].x, positions[2].y, positions[2].z);
			
			//Second Triangle
			ZShadeSandboxMesh::VertexPos c4 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos(positions[3].x, positions[3].y, positions[3].z);
			ZShadeSandboxMesh::VertexPos c5 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos(positions[4].x, positions[4].y, positions[4].z);
			ZShadeSandboxMesh::VertexPos c6 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos(positions[5].x, positions[5].y, positions[5].z);
			
			ZShadeSandboxMesh::VertexPos verts[6] =
			{
				c1, c2, c3, c4, c5, c6
			};
			
			mAttributes->mVerticesPos.resize( 6 );

			for (int i = 0; i < 6; i++)
			{
				mAttributes->mVerticesPos[i] = verts[i];
			}

			mAttributes->mVertexCount = 6;
#pragma endregion
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Tex:
		{
#pragma region "Tex"
			//First Triangle
			ZShadeSandboxMesh::VertexTex c1 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(positions[0].x, positions[0].y, positions[0].z, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexTex c2 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(positions[1].x, positions[1].y, positions[1].z, 1.0f, 1.0f);
			ZShadeSandboxMesh::VertexTex c3 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(positions[2].x, positions[2].y, positions[2].z, 0.0f, 1.0f);
			
			//Second Triangle
			ZShadeSandboxMesh::VertexTex c4 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(positions[3].x, positions[3].y, positions[3].z, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexTex c5 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(positions[4].x, positions[4].y, positions[4].z, 1.0f, 0.0f);
			ZShadeSandboxMesh::VertexTex c6 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(positions[5].x, positions[5].y, positions[5].z, 1.0f, 1.0f);
			
			ZShadeSandboxMesh::VertexTex verts[6] =
			{
				c1, c2, c3, c4, c5, c6
			};
			
			mAttributes->mVerticesTex.clear();
			mAttributes->mVerticesTex.resize( 6 );

			for (int i = 0; i < 6; i++)
			{
				mAttributes->mVerticesTex[i] = verts[i];
			}

			mAttributes->mVertexCount = 6;
#pragma endregion
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Color:
		{
#pragma region "Color"
			XMFLOAT4 diffuseColor = mMaterial->vDiffuseColor;

			//First Triangle
			ZShadeSandboxMesh::VertexColor c1 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(positions[0].x, positions[0].y, positions[0].z, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
			ZShadeSandboxMesh::VertexColor c2 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(positions[1].x, positions[1].y, positions[1].z, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
			ZShadeSandboxMesh::VertexColor c3 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(positions[2].x, positions[2].y, positions[2].z, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
			
			//Second Triangle
			ZShadeSandboxMesh::VertexColor c4 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(positions[3].x, positions[3].y, positions[3].z, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
			ZShadeSandboxMesh::VertexColor c5 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(positions[4].x, positions[4].y, positions[4].z, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
			ZShadeSandboxMesh::VertexColor c6 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(positions[5].x, positions[5].y, positions[5].z, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
			
			ZShadeSandboxMesh::VertexColor verts[6] =
			{
				c1, c2, c3, c4, c5, c6
			};
			
			mAttributes->mVerticesColor.resize( 6 );

			for (int i = 0; i < 6; i++)
			{
				mAttributes->mVerticesColor[i] = verts[i];
			}

			mAttributes->mVertexCount = 6;
#pragma endregion
		}
		break;
	}
}
//===============================================================================================================================