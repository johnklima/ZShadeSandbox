//******************************************************************************************************
// AnimationFrame2D.h
//******************************************************************************************************
#ifndef __ANIMATIONFRAME_H
#define __ANIMATIONFRAME_H
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
//Includes
#include "QuadMesh.h"
#include "D3D.h"
#include <string>
using namespace std;
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
class AnimationFrame2D
{
public:
	AnimationFrame2D(D3D* d3d);
	AnimationFrame2D(const AnimationFrame2D& other);
	~AnimationFrame2D();

	void SetFrameBasePath(string frameBasePath);

	void SetFrameName(string name);

	void SetFramePath(string path);

	void SetFrameBox(float x, float y, float w, float h);

	void SetDirection(int dir);

	string GetFrameName();

	float GetX();
	float GetY();
	float GetWidth();
	float GetHeight();

	int GetDirection();

	ZShadeSandboxMesh::QuadMesh* GetFrameTexture();

	void LoadFrameTexture();

private:
	D3D* m_D3DSystem;

	ZShadeSandboxMesh::QuadMesh* m_FrameTexture;

	string mFrameBasePath;
	string mFrameName;

	string mFramePath;

	float x;
	float y;
	float width;
	float height;

	int direction;
};
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
#endif//__ANIMATIONFRAME_H