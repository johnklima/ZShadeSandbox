#include "Image.h"
#include "MaterialManager.h"
using ZShadeSandboxGraphics::Image;
//=================================================================================================================
//=================================================================================================================
Image::Image(D3D* d3d
,	string texture_name
,	string base_sprite_path
,	float x
,	float y
,	int width
,	int height
,	string base_path_optional
,	string optionalColorName
)
:	mD3D(d3d)
,	mName(texture_name)
,	mXPos(x)
,	mYPos(y)
,	mWantedWidth(width)
,	mWantedHeight(height)
,	mBitmap(0)
,	mOptionalBitmap(0)
{
	if (texture_name.compare("") != 0)
	{
		MaterialManager::Instance()->AddTransparentDiffuse(d3d, base_sprite_path,
			texture_name, texture_name, texture_name, XMFLOAT3(255, 0, 255));

		ZShadeSandboxMesh::MeshParameters mp;
		mp.pos = XMFLOAT3(0, 0, 0);
		mp.rot = XMFLOAT3(0, 0, 0);
		mp.scale = XMFLOAT3(1, 1, 1);
		mp.rotationAxisX = false;
		mp.rotationAxisY = false;
		mp.rotationAxisZ = false;
		mp.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
		mp.useCustomShader = false;
		mp.material = MaterialManager::Instance()->GetMaterial(texture_name);
		mp.in2D = true;
		mBitmap = new ZShadeSandboxMesh::QuadMesh(d3d, mp, mWantedWidth, mWantedHeight, true);
	}
	else
	{
		mBitmap = 0;
	}

	if (optionalColorName.compare("") != 0)
	{
		mOptionalColorName = optionalColorName;
		
		MaterialManager::Instance()->AddTransparentDiffuse(d3d, base_path_optional,
			optionalColorName, optionalColorName, optionalColorName, XMFLOAT3(255, 0, 255));
		
		ZShadeSandboxMesh::MeshParameters mp;
		mp.pos = XMFLOAT3(0, 0, 0);
		mp.rot = XMFLOAT3(0, 0, 0);
		mp.scale = XMFLOAT3(1, 1, 1);
		mp.rotationAxisX = false;
		mp.rotationAxisY = false;
		mp.rotationAxisZ = false;
		mp.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
		mp.useCustomShader = false;
		mp.material = MaterialManager::Instance()->GetMaterial(optionalColorName);
		mp.in2D = true;
		mOptionalBitmap = new ZShadeSandboxMesh::QuadMesh(d3d, mp, mWantedWidth, mWantedHeight, true);
	}
}
//=================================================================================================================
Image::~Image()
{
}
//=================================================================================================================
RECT Image::GetRect()
{
	RECT P;
	P.top = mXPos;
	P.left = mYPos;
	P.right = P.left + mWantedWidth;
	P.bottom = P.top + mWantedHeight;
	return P;
}
//=================================================================================================================
void Image::UpdateMeshTexture(string base_path, string textureFilename)
{
	// Creates new material if one does not exist, if it does exist then do nothing
	MaterialManager::Instance()->AddTransparentDiffuse(mD3D, base_path,
		textureFilename, textureFilename, textureFilename, XMFLOAT3(255, 0, 255));
	
	if (mBitmap)
	{
		// Grab the material whether it existed or not
		mBitmap->SetMaterial(MaterialManager::Instance()->GetMaterial(textureFilename));
	}
	else
	{
		// Grab the material whether it existed or not
		mOptionalBitmap->SetMaterial(MaterialManager::Instance()->GetMaterial(textureFilename));
	}
}
//=================================================================================================================
Image* Image::Clone(string base_path, string base_path_optional)
{
	return new Image(mD3D, mName, base_path, mXPos, mYPos, mWantedWidth, mWantedHeight, base_path_optional, mOptionalColorName);
}
//=================================================================================================================
Image* Image::Clone(string base_path, int x, int y, string base_path_optional)
{
	return new Image(mD3D, mName, base_path, x, y, mWantedWidth, mWantedHeight, base_path_optional, mOptionalColorName);
}
//=================================================================================================================
Sprite* Image::ToSprite(string base_path)
{
	Sprite* spr = new Sprite(mD3D);
	spr->Initialize(base_path, mName, XMFLOAT3(mXPos, mYPos, 0), mWantedWidth, mWantedHeight, STATIC);
	return spr;
}
//=================================================================================================================
void Image::Draw(Camera* camera, int blendAmount)
{
	if (mBitmap)
	{
		mBitmap->UpdateBuffers(mXPos, mYPos, mWantedWidth, mWantedHeight);
		ZShadeSandboxMesh::MeshRenderParameters mrp;
		mrp.camera = camera;
		mrp.blendAmount = blendAmount;
		mrp.specifyWorld = true;
		mrp.world = XMMatrixIdentity();
		mBitmap->Render(mrp);
	}
	else
	{
		mOptionalBitmap->UpdateBuffers(mXPos, mYPos, mWantedWidth, mWantedHeight);
		ZShadeSandboxMesh::MeshRenderParameters mrp;
		mrp.camera = camera;
		mrp.blendAmount = blendAmount;
		mrp.specifyWorld = true;
		mrp.world = XMMatrixIdentity();
		mOptionalBitmap->Render(mrp);
	}
}
//=================================================================================================================