//==============================================================================================================================
// XMMath2.h
//==============================================================================================================================
#ifndef XMMATH2_H
#define XMMATH2_H
//==============================================================================================================================
//==============================================================================================================================

#include "ZMath.h"
#include "Vec.h"

namespace ZShadeSandboxMath {
	template <typename T> class Vec2;
}

//==============================================================================================================================
//==============================================================================================================================
namespace ZShadeSandboxMath {
class XMMath2
{
public:
	
	float x;
	float y;

	XMMath2();
	XMMath2(const XMMath2& v);
	XMMath2(float x, float y);
	XMMath2(XMFLOAT2 v);
	XMMath2(XMVECTOR v);
	XMMath2(ZShadeSandboxMath::Vec2<XMFLOAT2> v);
	
	operator XMVECTOR() { return XMVectorSet(x, y, 0.0f, 0.0f); }
	operator XMFLOAT2() { return XMFLOAT2(x,y); }
	
	/*
	static XMFLOAT2 Max(float x1, float y1, float x2, float y2);
	static XMFLOAT2 Max(XMMath2 val1, XMMath2 val2);
	static XMFLOAT2 Max(XMFLOAT2 val1, XMFLOAT2 val1);
	static XMFLOAT2 Max(ZShadeSandboxMath::Vec2<XMFLOAT2> val1, ZShadeSandboxMath::Vec2<XMFLOAT2> val1);
	static XMFLOAT2 Max(XMVECTOR val1, XMVECTOR val1);
	static XMFLOAT2 Min(float x1, float y1, float x2, float y2);
	static XMFLOAT2 Min(XMMath2 val1, XMMath2 val2);
	static XMFLOAT2 Min(XMFLOAT2 val1, XMFLOAT2 val1);
	static XMFLOAT2 Min(ZShadeSandboxMath::Vec2<XMFLOAT2> val1, ZShadeSandboxMath::Vec2<XMFLOAT2> val1);
	static XMFLOAT2 Min(XMVECTOR val1, XMVECTOR val1);
	*/
	
	static XMMath2 Clamp(XMMath2 value, XMMath2 pmin, XMMath2 pmax);
	
	XMFLOAT2 Max(float x2, float y2);
	XMFLOAT2 Max(XMMath2 val);
	XMFLOAT2 Max(XMFLOAT2 val);
	XMFLOAT2 Max(ZShadeSandboxMath::Vec2<XMFLOAT2> val);
	XMFLOAT2 Max(XMVECTOR val);
	XMFLOAT2 Min(float x2, float y2);
	XMFLOAT2 Min(XMMath2 val);
	XMFLOAT2 Min(XMFLOAT2 val);
	XMFLOAT2 Min(ZShadeSandboxMath::Vec2<XMFLOAT2> val);
	XMFLOAT2 Min(XMVECTOR val);
	
	ZShadeSandboxMath::Vec2<XMFLOAT2> ToFloatVector(XMFLOAT2 val);
	ZShadeSandboxMath::Vec2<XMFLOAT2> ToFloatVector(XMVECTOR val);
	ZShadeSandboxMath::Vec2<XMFLOAT2> ToFloatVector() const;
	
	XMVECTOR ToVector(XMFLOAT2 val);
	XMFLOAT2 ToFloat(XMVECTOR val);
	
	XMVECTOR ToVector() const;
	XMFLOAT2 ToFloat();
	
	float Distance(XMMath2 val);
	float DistanceSquared(XMMath2 val);
	
	void Assign(const XMMath2& val);
	void Assign(XMFLOAT2 val);
	void Assign(float val);
	
	XMFLOAT2 Normalize();
	void NormalizeMe();
	
	XMFLOAT2 TransformCoord(const XMMATRIX& mat);
	void TransformCoord(XMFLOAT2 val, const XMMATRIX& mat);
	void TransformCoord(XMVECTOR val, const XMMATRIX& mat);
	
	XMFLOAT2 Transform(const XMMATRIX& mat);
	
	float Dot(XMMath2 val);
	float Dot(XMFLOAT2 val);
	
	float Length();
	float LengthSquared();
	
	void Add(const XMMath2& val);
	
	void Subtract(const XMMath2& val);
	
	void Multiply(const XMMath2& val);
	
	void Divide(const XMMath2& val);
	
	XMMath2& operator += (const XMMath2& val);
	
	XMMath2& operator -= (const XMMath2& val);
	
	XMMath2& operator *= (const XMMath2& val);
	
	XMMath2& operator /= (const XMMath2& val);
	
	XMMath2& operator + (const XMMath2& val);
	
	XMMath2& operator - (const XMMath2& val);
	
	XMMath2& operator * (const XMMath2& val);
	
	XMMath2& operator / (const XMMath2& val);
	
	XMMath2& operator = (const XMMath2& val);

	bool Equals(const XMMath2& val);
	
	bool LessThanEquals(const XMMath2& val);
	
	bool GreaterThanEquals(const XMMath2& val);
	
	bool LessThan(const XMMath2& val);
	
	bool GreaterThan(const XMMath2& val);

	bool operator <= (const XMMath2& val);
	
	bool operator >= (const XMMath2& val);

	bool operator < (const XMMath2& val);
	
	bool operator > (const XMMath2& val);

	bool operator == (const XMMath2& val);
	
	void Add(const float& val);

	void Subtract(const float& val);

	void Multiply(const float& val);

	void Divide(const float& val);

	XMMath2& operator += (const float& val);
	
	XMMath2& operator -= (const float& val);
	
	XMMath2& operator *= (const float& val);
	
	XMMath2& operator /= (const float& val);
	
	XMMath2& operator + (const float& val);
	
	XMMath2& operator - (const float& val);
	
	XMMath2& operator * (const float& val);
	
	XMMath2& operator / (const float& val);
	
	XMMath2& operator = (const float& val);

	bool Equals(const float& val);
	
	bool LessThanEquals(const float& val);
	
	bool GreaterThanEquals(const float& val);
	
	bool LessThan(const float& val);
	
	bool GreaterThan(const float& val);

	bool operator <= (const float& val);
	
	bool operator >= (const float& val);

	bool operator < (const float& val);
	
	bool operator > (const float& val);

	bool operator == (const float& val);
};
}
//==============================================================================================================================
//==============================================================================================================================
#endif//XMMATH2_H