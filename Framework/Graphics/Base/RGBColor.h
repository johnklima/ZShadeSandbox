//===============================================================================================================================
// RGBColor
//
//===============================================================================================================================
// History
//
// -Created on 1/12/2014 by Dustin Watson
//===============================================================================================================================
#ifndef __RGBCOLOR_H
#define __RGBCOLOR_H
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//

//===============================================================================================================================
//===============================================================================================================================
namespace RGBColor
{
	typedef DWORD ARGB;
//===============================================================================================================================
class Color
{
public:

    Color()
    {
        Argb = (ARGB)Color::Black;
    }

    // Construct an opaque Color object with
    // the specified R, G, B values.

    Color(BYTE r,
          BYTE g,
          BYTE b)
    {
        Argb = MakeARGB(255, r, g, b);
    }

    // Construct a Color object with
    // the specified A, R, G, B values.
    //
    // NOTE: R, G, B color values are not premultiplied.

    Color(BYTE a,
          BYTE r,
          BYTE g,
          BYTE b)
    {
        Argb = MakeARGB(a, r, g, b);
    }

    // Construct a Color object with
    // the specified ARGB values.
    //
    // NOTE: R, G, B color components are not premultiplied.

    Color(ARGB argb)
    {
        Argb = argb;
    }

    // Extract A, R, G, B components

    BYTE GetAlpha() const
    {
        return (BYTE) (Argb >> AlphaShift);
    }

    BYTE GetA() const
    {
        return GetAlpha();
    }

    BYTE GetRed() const
    {
        return (BYTE) (Argb >> RedShift);
    }

    BYTE GetR() const
    {
        return GetRed();
    }

    BYTE GetGreen() const
    {
        return (BYTE) (Argb >> GreenShift);
    }

    BYTE GetG() const
    {
        return GetGreen();
    }

    BYTE GetBlue() const
    {
        return (BYTE) (Argb >> BlueShift);
    }

    BYTE GetB() const
    {
        return GetBlue();
    }

    // Retrieve ARGB values

    ARGB GetValue() const
    {
        return Argb;
    }

    VOID SetValue(ARGB argb)
    {
        Argb = argb;
    }

    VOID SetFromCOLORREF(COLORREF rgb)
    {
        Argb = MakeARGB(255, GetRValue(rgb), GetGValue(rgb), GetBValue(rgb));
    }

    COLORREF ToCOLORREF() const
    {
        return RGB(GetRed(), GetGreen(), GetBlue());
    }

	static RGBColor::Color SystemColor()
	{
		return RGBColor::Color(GetSysColor(COLOR_BACKGROUND));
	}

public:

    // Standard color constants
    enum
    {
       Black = 0xff000000,
       Silver = 0xffc0c0c0,
       Gray = 0xff808080,
       White = 0xffffffff,
       Maroon = 0xff800000,
       Red = 0xffff0000,
       Purple = 0xff800080,
       Fuchsia = 0xffff00ff,
       Green = 0xff008000,
       Lime = 0xff00ff00,
       Olive = 0xff808000,
       Yellow = 0xffffff00,
       Navy = 0xff000080,
       Blue = 0xff0000ff,
       Teal = 0xff008080,
       Aqua = 0xff00ffff
    };

    // Shift count and bit mask for A, R, G, B components
    enum
    {
        AlphaShift  = 24,
        RedShift    = 16,
        GreenShift  = 8,
        BlueShift   = 0
    };

    enum
    {
        AlphaMask   = 0xff000000,
        RedMask     = 0x00ff0000,
        GreenMask   = 0x0000ff00,
        BlueMask    = 0x000000ff
    };

    // Assemble A, R, G, B values into a 32-bit integer
    static ARGB MakeARGB(BYTE a,
                         BYTE r,
                         BYTE g,
                         BYTE b)
    {
        return (((ARGB) (b) <<  BlueShift) |
                ((ARGB) (g) << GreenShift) |
                ((ARGB) (r) <<   RedShift) |
                ((ARGB) (a) << AlphaShift));
    }
	
protected:
	
    ARGB Argb;
};
//===============================================================================================================================
}
//===============================================================================================================================
//===============================================================================================================================
#endif//__RGBCOLOR_H