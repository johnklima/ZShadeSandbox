#ifndef __IMAGE_H
#define __IMAGE_H
//-----------------------------------------------------------------------------------------
#include "D3DIncludes.h"
#include "D3D.h"
#include <string>
#include "Sprite.h"
#include "CGlobal.h"
#include "QuadMesh.h"
#include "Light.h"
using std::string;
//-----------------------------------------------------------------------------------------
namespace ZShadeSandboxGraphics {
	class Image
	{
	public:
		
		Image(D3D* d3d
		,	string texture_name
		,	string base_path
		,	float x
		,	float y
		,	int width
		,	int height
		,	string base_path_optional = ""
		,	string optionalColorName = ""
		);
		~Image();

		void Draw(Camera* camera, int blendAmount);

		string GetName()              { return mName; }
		string GetOptionalColorName() { return mOptionalColorName; }
		void UpdateMeshTexture(string base_path, string textureFilename);
		void SetOptionalBitmap(ZShadeSandboxMesh::QuadMesh* tex) { mOptionalBitmap = tex; }

		float& XPos()    	{ return mXPos; }
		float XPos() const  { return mXPos; }
		float& YPos()    	{ return mYPos; }
		float YPos() const 	{ return mYPos; }
		int& Width()     	{ return mWantedWidth; }
		int Width() const   { return mWantedWidth; }
		int& Height()    	{ return mWantedHeight; }
		int Height() const  { return mWantedHeight; }
		int& ID()			{ return mID; }
		int ID() const		{ return mID; }
		
		RECT GetRect();
		
		Image* Clone(string base_path, string base_path_optional = "");
		Image* Clone(string base_path, int x, int y, string base_path_optional = "");
		Sprite* ToSprite(string base_path);
		
	private:
		
		ZShadeSandboxMesh::QuadMesh* mBitmap;
		ZShadeSandboxMesh::QuadMesh* mOptionalBitmap;
		D3D*               mD3D;
		string             mName;
		string             mOptionalColorName;
		int				   mID;
		float              mXPos;
		float              mYPos;
		int                mWantedWidth;
		int                mWantedHeight;
	};
}
//-----------------------------------------------------------------------------------------
#endif//__IMAGE_H