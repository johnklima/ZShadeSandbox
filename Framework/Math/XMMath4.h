//==============================================================================================================================
// XMMath4.h
//==============================================================================================================================
#ifndef XMMATH4_H
#define XMMATH4_H
//==============================================================================================================================
//==============================================================================================================================

#include "ZMath.h"
#include "Vec.h"

namespace ZShadeSandboxMath {
	template <typename T> class Vec4;
}

//==============================================================================================================================
//==============================================================================================================================
namespace ZShadeSandboxMath {
class XMMath4
{
public:
	
	float x;
	float y;
	float z;
	float w;

	XMMath4();
	XMMath4(const XMMath4& v);
	XMMath4(float x, float y, float z, float w);
	XMMath4(XMFLOAT4 v);
	XMMath4(XMVECTOR v);
	XMMath4(ZShadeSandboxMath::Vec4<XMFLOAT4> v);
	
	operator XMVECTOR() { return XMVectorSet(x, y, z, w); }
	operator XMFLOAT4() { return XMFLOAT4(x, y, z, w); }
	
	static XMMath4 Clamp(XMMath4 value, XMMath4 pmin, XMMath4 pmax);
	
	XMFLOAT4 Max(float x2, float y2, float z2, float w2);
	XMFLOAT4 Max(XMMath4 val);
	XMFLOAT4 Max(XMFLOAT4 val);
	XMFLOAT4 Max(ZShadeSandboxMath::Vec4<XMFLOAT4> val);
	XMFLOAT4 Max(XMVECTOR val);
	
	XMFLOAT4 Min(float x2, float y2, float z2, float w2);
	XMFLOAT4 Min(XMMath4 val);
	XMFLOAT4 Min(XMFLOAT4 val);
	XMFLOAT4 Min(ZShadeSandboxMath::Vec4<XMFLOAT4> val);
	XMFLOAT4 Min(XMVECTOR val);
	
	ZShadeSandboxMath::Vec4<XMFLOAT4> ToFloatVector(XMFLOAT4 val);
	ZShadeSandboxMath::Vec4<XMFLOAT4> ToFloatVector(XMVECTOR val);
	ZShadeSandboxMath::Vec4<XMFLOAT4> ToFloatVector() const;

	XMVECTOR ToVector(XMFLOAT4 val);
	XMFLOAT4 ToFloat(XMVECTOR val);
	
	XMVECTOR ToVector() const;
	XMFLOAT4 ToFloat();
	
	float Distance(XMMath4 val);
	float DistanceSquared(XMMath4 val);
	
	void Assign(const XMMath4& val);
	void Assign(XMFLOAT4 val);
	void Assign(float val);
	
	XMFLOAT4 Normalize();
	void NormalizeMe();
	
	//XMFLOAT4 TransformCoord(XMMATRIX mat);
	
	XMFLOAT4 Transform(const XMMATRIX& mat);
	
	float Dot(XMMath4 val);
	float Dot(XMFLOAT4 val);
	
	XMMath4 Cross(XMMath4 val);
	XMMath4 Cross(XMMath4 val1, XMMath4 val2);
	XMMath4 Cross(XMFLOAT4 val);
	XMMath4 Cross(XMFLOAT4 val1, XMFLOAT4 val2);
	XMMath4 Cross(XMVECTOR val1, XMVECTOR val2);
	
	float Length();
	float LengthSquared();
	
	void Add(const XMMath4& val);
	
	void Subtract(const XMMath4& val);
	
	void Multiply(const XMMath4& val);
	
	void Divide(const XMMath4& val);
	
	XMMath4& operator += (const XMMath4& val);
	
	XMMath4& operator -= (const XMMath4& val);
	
	XMMath4& operator *= (const XMMath4& val);
	
	XMMath4& operator /= (const XMMath4& val);
	
	XMMath4& operator + (const XMMath4& val);
	
	XMMath4& operator - (const XMMath4& val);
	
	XMMath4& operator * (const XMMath4& val);
	
	XMMath4& operator / (const XMMath4& val);
	
	XMMath4& operator = (const XMMath4& val);
	
	bool Equals(const XMMath4& val);

	bool LessThanEquals(const XMMath4& val);

	bool GreaterThanEquals(const XMMath4& val);

	bool LessThan(const XMMath4& val);

	bool GreaterThan(const XMMath4& val);

	bool operator <= (const XMMath4& val);

	bool operator >= (const XMMath4& val);

	bool operator < (const XMMath4& val);

	bool operator > (const XMMath4& val);

	bool operator == (const XMMath4& val);

	void Add(const float& val);

	void Subtract(const float& val);

	void Multiply(const float& val);

	void Divide(const float& val);

	XMMath4& operator += (const float& val);
	
	XMMath4& operator -= (const float& val);
	
	XMMath4& operator *= (const float& val);
	
	XMMath4& operator /= (const float& val);
	
	XMMath4& operator + (const float& val);
	
	XMMath4& operator - (const float& val);
	
	XMMath4& operator * (const float& val);
	
	XMMath4& operator / (const float& val);
	
	XMMath4& operator = (const float& val);

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
#endif//XMMATH4_H