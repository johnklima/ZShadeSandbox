//===============================================================================================================================
// GDIFont
//
//===============================================================================================================================
// History
//
// -Created on 1/14/2014 by Dustin Watson
//===============================================================================================================================
#ifndef __GDIFONT_H
#define __GDIFONT_H
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//

#include <Windows.h>
#include "BetterString.h"
#include "InterfacePointers.h"
using namespace std;

class D3D;

//===============================================================================================================================
//===============================================================================================================================

namespace FontStyle
{
	enum
	{
		Regular = 0,
		Bold = 1 << 0,
		Italic = 1 << 1,
		BoldItalic = Bold | Italic,
		Underline = 1 << 2,
		Strikeout = 1 << 3
	};
};
	
//===============================================================================================================================
//===============================================================================================================================
class GDIFont
{
public:

	struct CharDesc
    {
        float X;
        float Y;
        float Width;
        float Height;
    };

private:

    static const WCHAR StartChar = '!';
    static const WCHAR EndChar = 127;
    static const UINT64 NumChars = EndChar - StartChar;
    static const UINT TexWidth = 1024;
	
protected:
	
	D3D* m_D3DSystem;
	ID3D11Texture2DPtr texture;
    ID3D11ShaderResourceViewPtr srView;
    CharDesc charDescs [NumChars];
    float size;
    UINT texHeight;
    float spaceWidth;
    float charHeight;
	
public:
	
	GDIFont() {}
	GDIFont(D3D* d3d);
	~GDIFont();
	
	void SetD3D(D3D* d3d) { m_D3DSystem = d3d; }
	
	void Initialize(LPCWSTR fontName, float fontSize, UINT fontStyle, bool antiAliased);
	
	ID3D11ShaderResourceView* SRView() const;
    const CharDesc* CharDescriptors() const;
    const CharDesc& GetCharDescriptor(WCHAR character) const;
    float Size() const;
    ID3D11Texture2D* Texture() const;
    UINT TextureWidth() const;
    UINT TextureHeight() const;
    float SpaceWidth() const;
    float CharHeight() const;
};
//===============================================================================================================================
//===============================================================================================================================
#endif//__GDIFONT_H