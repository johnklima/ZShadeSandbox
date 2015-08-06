#include "GDIFont.h"
#define ULONG_PTR ULONG
#include <Windows.h>
#include <gdiplus.h>
using namespace std;
using namespace Gdiplus;
#pragma comment (lib, "gdiplus.lib")
#include "D3D.h"
//===============================================================================================================================
//===============================================================================================================================
GDIFont::GDIFont(D3D* d3d)
:   m_D3DSystem(d3d)
{
}
//===============================================================================================================================
GDIFont::~GDIFont()
{
}
//===============================================================================================================================
void GDIFont::Initialize(LPCWSTR fontName, float fontSize, UINT fontStyle, bool antiAliased)
{
	size = fontSize;

    TextRenderingHint hint = antiAliased ? TextRenderingHintAntiAliasGridFit : TextRenderingHintSystemDefault;
	
	// Create the font
	Gdiplus::Font font(fontName, fontSize, fontStyle, UnitPixel, NULL);

	// Check for error during construction
	font.GetLastStatus();
	
	// Create a temporary Bitmap and Graphics for figuring out the rough size required
	// for drawing all of the characters
	int size = static_cast<int>(fontSize * NumChars * 2) + 1;
	Bitmap sizeBitmap(size, size, PixelFormat32bppARGB);
	sizeBitmap.GetLastStatus();
	
	Graphics sizeGraphics(&sizeBitmap);
	sizeGraphics.GetLastStatus();
	sizeGraphics.SetTextRenderingHint(hint);
	
	charHeight = font.GetHeight(&sizeGraphics) * 1.5f;

	WCHAR allChars[NumChars + 1];
	for(WCHAR i = 0; i < NumChars; ++i)
		allChars[i] = i + StartChar;
	allChars[NumChars] = 0;

	RectF sizeRect;
	sizeGraphics.MeasureString(allChars, NumChars, &font, PointF(0, 0), &sizeRect);
	int numRows = static_cast<int>(sizeRect.Width / TexWidth) + 1;
	int texHeight = static_cast<int>(numRows * charHeight) + 1;

	// Create a temporary Bitmap and Graphics for drawing the characters one by one
	int tempSize = static_cast<int>(fontSize * 2);
	Bitmap drawBitmap(tempSize, tempSize, PixelFormat32bppARGB);
	drawBitmap.GetLastStatus();

	Graphics drawGraphics(&drawBitmap);
	drawGraphics.GetLastStatus();
	drawGraphics.SetTextRenderingHint(hint);

	// Create a temporary Bitmap + Graphics for creating a full character set
	Bitmap textBitmap (TexWidth, texHeight, PixelFormat32bppARGB);
	textBitmap.GetLastStatus();

	Graphics textGraphics (&textBitmap);
	textGraphics.GetLastStatus();
	textGraphics.Clear(Color(0, 255, 255, 255));
	textGraphics.SetCompositingMode(CompositingModeSourceCopy);

	// Solid brush for text rendering
	SolidBrush brush (Color(255, 255, 255, 255));
	brush.GetLastStatus();
	
	// Draw all of the characters, and copy them to the full character set
	WCHAR charString [2];
	charString[1] = 0;
	int currentX = 0;
	int currentY = 0;
	for(UINT64 i = 0; i < NumChars; ++i)
	{
		charString[0] = static_cast<WCHAR>(i + StartChar);

		// Draw the character
		drawGraphics.Clear(Color(0, 255, 255, 255));
		drawGraphics.DrawString(charString, 1, &font, PointF(0, 0), &brush);

		// Figure out the amount of blank space before the character
		int minX = 0;
		for(int x = 0; x < tempSize; ++x)
		{
			for(int y = 0; y < tempSize; ++y)
			{
				Color color;
				drawBitmap.GetPixel(x, y, &color);
				if(color.GetAlpha() > 0)
				{
					minX = x;
					x = tempSize;
					break;
				}
			}
		}

		// Figure out the amount of blank space after the character
		int maxX = tempSize - 1;
		for(int x = tempSize - 1; x >= 0; --x)
		{
			for(int y = 0; y < tempSize; ++y)
			{
				Color color;
				drawBitmap.GetPixel(x, y, &color);
				if(color.GetAlpha() > 0)
				{
					maxX = x;
					x = -1;
					break;
				}
			}
		}

		int charWidth = maxX - minX + 1;

		// Figure out if we need to move to the next row
		if (currentX + charWidth >= TexWidth)
		{
			currentX = 0;
			currentY += static_cast<int>(charHeight) + 1;
		}

		// Fill out the structure describing the character position
		charDescs[i].X = static_cast<float>(currentX);
		charDescs[i].Y = static_cast<float>(currentY);
		charDescs[i].Width = static_cast<float>(charWidth);
		charDescs[i].Height = static_cast<float>(charHeight);

		// Copy the character over
		int height = static_cast<int>(charHeight + 1);
		textGraphics.DrawImage(&drawBitmap, currentX, currentY, minX, 0, charWidth, height, UnitPixel);

		currentX += charWidth + 1;
	}
	
	// Figure out the width of a space character
	charString[0] = ' ';
	charString[1] = 0;
	drawGraphics.MeasureString(charString, 1, &font, PointF(0, 0), &sizeRect);
	spaceWidth = sizeRect.Width;

	// Lock the bitmap for direct memory access
	BitmapData bmData;
	textBitmap.LockBits(&Rect(0, 0, TexWidth, texHeight), ImageLockModeRead, PixelFormat32bppARGB, &bmData);

	// Create a D3D texture, initalized with the bitmap data
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = TexWidth;
	texDesc.Height = texHeight;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_IMMUTABLE;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = bmData.Scan0;
	data.SysMemPitch = TexWidth * 4;
	data.SysMemSlicePitch = 0;
	
	m_D3DSystem->GetDevice11()->CreateTexture2D(&texDesc, &data, &texture);
	
	textBitmap.UnlockBits(&bmData);
	
	// Create the shader resource view
	D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	srDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srDesc.Texture2D.MipLevels = 1;
	srDesc.Texture2D.MostDetailedMip = 0;
	
	m_D3DSystem->GetDevice11()->CreateShaderResourceView(texture, &srDesc, &srView);
}
//===============================================================================================================================
ID3D11ShaderResourceView* GDIFont::SRView() const
{
	return srView;
}
//===============================================================================================================================
const GDIFont::CharDesc* GDIFont::CharDescriptors() const
{
	return charDescs;
}
//===============================================================================================================================
const GDIFont::CharDesc& GDIFont::GetCharDescriptor(WCHAR character) const
{
	_ASSERT(character >= StartChar && character <= EndChar);
    return charDescs[character - StartChar];
}
//===============================================================================================================================
float GDIFont::Size() const
{
	return size;
}
//===============================================================================================================================
ID3D11Texture2D* GDIFont::Texture() const
{
	return texture;
}
//===============================================================================================================================
UINT GDIFont::TextureWidth() const
{
	return TexWidth;
}
//===============================================================================================================================
UINT GDIFont::TextureHeight() const
{
	return texHeight;
}
//===============================================================================================================================
float GDIFont::SpaceWidth() const
{
	return spaceWidth;
}
//===============================================================================================================================
float GDIFont::CharHeight() const
{
	 return charHeight;
}
//===============================================================================================================================