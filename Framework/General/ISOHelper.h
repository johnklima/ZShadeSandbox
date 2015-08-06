//======================================================================================================
// ISOHelper.h
//
// Helper functions for Isometric tiles
//======================================================================================================
// History
//
// -Created 9/15/2014 by Dustin Watson
//======================================================================================================
#ifndef __ISOHELPER_H
#define __ISOHELPER_H
//======================================================================================================
//======================================================================================================
#include "D3DIncludes.h"
#include <math.h>
//======================================================================================================
inline XMFLOAT2 IsoTo2D(float x, float y)
{
	XMFLOAT2 point;
	point.x = (2 * y + x) / 2;
	point.y = (2 * y - x) / 2;
	return point;
}
//======================================================================================================
inline XMFLOAT2 TwoDToIso(float x, float y)
{
	XMFLOAT2 point;
	point.x = x - y;
	point.y = (x + y) / 2;
	return point;
}
//======================================================================================================
inline XMFLOAT2 IsoTo2D(XMFLOAT2 p)
{
	XMFLOAT2 point;
	point.x = (2 * p.y + p.x) / 2;
	point.y = (2 * p.y - p.x) / 2;
	return point;
}
//======================================================================================================
inline XMFLOAT2 TwoDToIso(XMFLOAT2 p)
{
	XMFLOAT2 point;
	point.x = p.x - p.y;
	point.y = (p.x + p.y) / 2;
	return point;
}
//======================================================================================================
inline XMFLOAT2 IsoTileCoordinates(float x, float y, float tileHeight)
{
	XMFLOAT2 point;
	point.x = floor(x / tileHeight);
	point.y = floor(y / tileHeight);
	return point;
}
//======================================================================================================
inline XMFLOAT2 TwoDFromIsoTileCoordinates(float x, float y, float tileHeight)
{
	XMFLOAT2 point;
	point.x = x * tileHeight;
	point.y = y * tileHeight;
	return point;
}
//======================================================================================================
inline XMFLOAT2 IsoTileCoordinates(XMFLOAT2 p, float tileHeight)
{
	XMFLOAT2 point;
	point.x = floor(p.x / tileHeight);
	point.y = floor(p.y / tileHeight);
	return point;
}
//======================================================================================================
inline XMFLOAT2 TwoDFromIsoTileCoordinates(XMFLOAT2 p, float tileHeight)
{
	XMFLOAT2 point;
	point.x = p.x * tileHeight;
	point.y = p.y * tileHeight;
	return point;
}
//======================================================================================================
//======================================================================================================
#endif//__ISOHELPER_H