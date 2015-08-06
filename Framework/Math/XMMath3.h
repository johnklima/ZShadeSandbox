//==============================================================================================================================
// XMMath3.h
//==============================================================================================================================
#ifndef _XMMATH3_H
#define _XMMATH3_H
//==============================================================================================================================
//==============================================================================================================================

#include "ZMath.h"
#include "Vec.h"

namespace ZShadeSandboxMath {
	template <typename T> class Vec3;
}

//==============================================================================================================================
//==============================================================================================================================
namespace ZShadeSandboxMath {
class XMMath3
{
public:
	
	float x;
	float y;
	float z;

	XMMath3();
	XMMath3(const XMMath3& v);
	XMMath3(float x, float y, float z);
	XMMath3(XMFLOAT3 v);
	XMMath3(XMVECTOR v);
	XMMath3(ZShadeSandboxMath::Vec3<XMFLOAT3> v);
	
	operator XMVECTOR() { return XMVectorSet(x, y, z, 0.0f); }
	operator XMFLOAT3() { return XMFLOAT3(x, y, z); }
	
	static XMMath3 Clamp(XMMath3 value, XMMath3 pmin, XMMath3 pmax);
	
	XMFLOAT3 Max(float x2, float y2, float z2);
	XMFLOAT3 Max(XMMath3 val);
	XMFLOAT3 Max(XMFLOAT3 val);
	XMFLOAT3 Max(ZShadeSandboxMath::Vec3<XMFLOAT3> val);
	XMFLOAT3 Max(XMVECTOR val);
	XMFLOAT3 Min(float x2, float y2, float z2);
	XMFLOAT3 Min(XMMath3 val);
	XMFLOAT3 Min(XMFLOAT3 val);
	XMFLOAT3 Min(ZShadeSandboxMath::Vec3<XMFLOAT3> val);
	XMFLOAT3 Min(XMVECTOR val);
	
	ZShadeSandboxMath::Vec3<XMFLOAT3> ToFloatVector(XMFLOAT3 val);
	ZShadeSandboxMath::Vec3<XMFLOAT3> ToFloatVector(XMVECTOR val);
	ZShadeSandboxMath::Vec3<XMFLOAT3> ToFloatVector() const;
	
	XMVECTOR ToVector(XMFLOAT3 val);
	XMFLOAT3 ToFloat(XMVECTOR val);
	
	XMVECTOR ToVector() const;
	XMFLOAT3 ToFloat();
	
	float Distance(XMMath3 val);
	float DistanceSquared(XMMath3 val);
	
	void Assign(const XMMath3& val);
	void Assign(XMFLOAT3 val);
	void Assign(float val);
	
	XMFLOAT3 Normalize();
	void NormalizeMe();
	
	XMFLOAT3 TransformCoord(const XMMATRIX& mat);
	void TransformCoord(XMFLOAT3 val, const XMMATRIX& mat);
	void TransformCoord(XMVECTOR val, const XMMATRIX& mat);
	
	XMFLOAT3 Transform(const XMMATRIX& mat);
	
	float Dot(XMMath3 val);
	float Dot(XMFLOAT3 val);
	
	XMMath3 Cross(XMMath3 val);
	XMMath3 Cross(XMMath3 val1, XMMath3 val2);
	XMMath3 Cross(XMFLOAT3 val);
	XMMath3 Cross(XMFLOAT3 val1, XMFLOAT3 val2);
	XMMath3 Cross(XMVECTOR val1, XMVECTOR val2);
	
	float Length();
	float LengthSquared();
	
	void Add(const XMMath3& val);
	
	void Subtract(const XMMath3& val);
	
	void Multiply(const XMMath3& val);
	
	void Divide(const XMMath3& val);

	XMMath3& operator += (const XMMath3& val);
	
	XMMath3& operator -= (const XMMath3& val);
	
	XMMath3& operator *= (const XMMath3& val);
	
	XMMath3& operator /= (const XMMath3& val);
	
	XMMath3& operator + (const XMMath3& val);
	
	XMMath3& operator - (const XMMath3& val);
	
	XMMath3& operator * (const XMMath3& val);
	
	XMMath3& operator / (const XMMath3& val);
	
	XMMath3& operator = (const XMMath3& val);

	bool Equals(const XMMath3& val);

	bool LessThanEquals(const XMMath3& val);

	bool GreaterThanEquals(const XMMath3& val);

	bool LessThan(const XMMath3& val);

	bool GreaterThan(const XMMath3& val);

	bool operator <= (const XMMath3& val);
	
	bool operator >= (const XMMath3& val);

	bool operator < (const XMMath3& val);
	
	bool operator > (const XMMath3& val);

	bool operator == (const XMMath3& val);

	void Add(const float& val);
	
	void Subtract(const float& val);
	
	void Multiply(const float& val);
	
	void Divide(const float& val);

	XMMath3& operator += (const float& val);
	
	XMMath3& operator -= (const float& val);
	
	XMMath3& operator *= (const float& val);
	
	XMMath3& operator /= (const float& val);
	
	XMMath3& operator + (const float& val);
	
	XMMath3& operator - (const float& val);
	
	XMMath3& operator * (const float& val);
	
	XMMath3& operator / (const float& val);
	
	XMMath3& operator = (const float& val);

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
#endif//_XMMATH3_H