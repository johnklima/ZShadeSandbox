//******************************************************************************************************
// Texture.h
//******************************************************************************************************
#ifndef __TEXTURE_H
#define __TEXTURE_H
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
//Includes
#include "D3DIncludes.h"
#include "D3D.h"
#include "CGlobal.h"
#include "TextureManager.h"
#include <string>
using namespace std;
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
class Texture
{
public:
	Texture(D3D* d3d);
	Texture(const Texture& other);
	~Texture();

	bool Initialize(string base_image_path, string filename);
	bool InitializeLayerMapArray(vector<string> textureFileNames);
	void Shutdown();
	
	ID3D11ShaderResourceView*& setTexture11() { return m_texture11; }
	ID3D11ShaderResourceView* getTexture11() const { return m_texture11; }
	ID3D11Texture2D* getTexture2D11() const { return m_renderTargetTexture11; }

	ID3D10ShaderResourceView*& setTexture10() { return m_texture10; }
	ID3D10ShaderResourceView* getTexture10() const { return m_texture10; }
	ID3D10Texture2D* getTexture2D10() const { return m_renderTargetTexture10; }

	string GetWindowName() { return m_windowName; }
	string GetTextureName();
	
	int Width() { return m_textureWidth; }
	int Height() { return m_textureHeight; }
	
	void GetImageSize(int& width, int& height);

	void SetSRV(ID3D11ShaderResourceView* srv) { m_texture11 = srv; }

private:
	D3D* m_D3DSystem;
	EngineOptions* m_engineOptions;

	//What window does this texture belong to
	string m_windowName;
	
	string m_textureName;
	int m_textureWidth;
	int m_textureHeight;
	
	ID3D11Texture2D* m_renderTargetTexture11;
	ID3D11RenderTargetView* m_renderTargetView11;
	ID3D11ShaderResourceView* m_texture11;

	ID3D10Texture2D* m_renderTargetTexture10;
	ID3D10RenderTargetView* m_renderTargetView10;
	ID3D10ShaderResourceView* m_texture10;
};
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
#endif//__TEXTURE_H