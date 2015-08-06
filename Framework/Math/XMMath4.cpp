#include "XMMath4.h"
using ZShadeSandboxMath::XMMath4;
//==============================================================================================================================
//==============================================================================================================================
XMMath4::XMMath4()
{
}
//==============================================================================================================================
XMMath4::XMMath4(const XMMath4& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
}
//==============================================================================================================================
XMMath4::XMMath4(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}
//==============================================================================================================================
XMMath4::XMMath4(XMFLOAT4 v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
}
//==============================================================================================================================
XMMath4::XMMath4(XMVECTOR v)
{
	XMFLOAT4 vf;
	XMStoreFloat4(&vf, v);
	x = vf.x;
	y = vf.y;
	z = vf.z;
	w = vf.w;
}
//==============================================================================================================================
XMMath4::XMMath4(ZShadeSandboxMath::Vec4<XMFLOAT4> v)
{
	x = v.Vector().x;
	y = v.Vector().y;
	z = v.Vector().z;
	w = v.Vector().w;
}
//==============================================================================================================================
XMMath4 XMMath4::Clamp(XMMath4 value, XMMath4 pmin, XMMath4 pmax)
{
	float tx = value.x;
	tx = (tx > pmax.x) ? pmax.x : tx;
	tx = (tx < pmin.x) ? pmin.x : tx;
	
	float ty = value.y;
	ty = (ty > pmax.y) ? pmax.y : ty;
	ty = (ty < pmin.y) ? pmin.y : ty;
	
	float tz = value.z;
	tz = (tz > pmax.z) ? pmax.z : tz;
	tz = (tz < pmin.z) ? pmin.z : tz;
	
	float tw = value.w;
	tw = (tw > pmax.w) ? pmax.w : tw;
	tw = (tw < pmin.w) ? pmin.w : tw;
	
	return XMMath4(tx, ty, tz, tw);
}
//==============================================================================================================================
XMFLOAT4 XMMath4::Max(float x2, float y2, float z2, float w2)
{
	XMFLOAT4 ret;
	
	ret.x = (x > x2) ? x : x2;
	ret.y = (y > y2) ? y : y2;
	ret.z = (z > z2) ? z : z2;
	ret.w = (w > w2) ? w : w2;
	
	return ret;
}
//==============================================================================================================================
XMFLOAT4 XMMath4::Max(XMMath4 val)
{
	XMFLOAT4 ret;
	
	ret.x = (x > val.x) ? x : val.x;
	ret.y = (y > val.y) ? y : val.y;
	ret.z = (z > val.z) ? z : val.z;
	ret.w = (w > val.w) ? w : val.w;
	
	return ret;
}
//==============================================================================================================================
XMFLOAT4 XMMath4::Max(XMFLOAT4 val)
{
	XMFLOAT4 ret;
	
	ret.x = (x > val.x) ? x : val.x;
	ret.y = (y > val.y) ? y : val.y;
	ret.z = (z > val.z) ? z : val.z;
	ret.w = (w > val.w) ? w : val.w;
	
	return ret;
}
//==============================================================================================================================
XMFLOAT4 XMMath4::Max(ZShadeSandboxMath::Vec4<XMFLOAT4> val)
{
	XMFLOAT4 ret;
	
	ret.x = (x > val.Vector().x) ? x : val.Vector().x;
	ret.y = (y > val.Vector().y) ? y : val.Vector().y;
	ret.z = (z > val.Vector().z) ? z : val.Vector().z;
	ret.w = (w > val.Vector().w) ? w : val.Vector().w;
	
	return ret;
}
//==============================================================================================================================
XMFLOAT4 XMMath4::Max(XMVECTOR val)
{
	XMFLOAT4 vf;
	XMStoreFloat4(&vf, val);
	
	XMFLOAT4 ret;
	
	ret.x = (x > vf.x) ? x : vf.x;
	ret.y = (y > vf.y) ? y : vf.y;
	ret.z = (z > vf.z) ? z : vf.z;
	ret.w = (w > vf.w) ? w : vf.w;
	
	return ret;
}
//==============================================================================================================================
XMFLOAT4 XMMath4::Min(float x2, float y2, float z2, float w2)
{
	XMFLOAT4 ret;
	
	ret.x = (x < x2) ? x : x2;
	ret.y = (y < y2) ? y : y2;
	ret.z = (z < z2) ? z : z2;
	ret.w = (w < w2) ? w : w2;
	
	return ret;
}
//==============================================================================================================================
XMFLOAT4 XMMath4::Min(XMMath4 val)
{
	XMFLOAT4 ret;
	
	ret.x = (x < val.x) ? x : val.x;
	ret.y = (y < val.y) ? y : val.y;
	ret.z = (z < val.z) ? z : val.z;
	ret.w = (w < val.w) ? w : val.w;
	
	return ret;
}
//==============================================================================================================================
XMFLOAT4 XMMath4::Min(XMFLOAT4 val)
{
	XMFLOAT4 ret;
	
	ret.x = (x < val.x) ? x : val.x;
	ret.y = (y < val.y) ? y : val.y;
	ret.z = (z < val.z) ? z : val.z;
	ret.w = (w < val.w) ? w : val.w;
	
	return ret;
}
//==============================================================================================================================
XMFLOAT4 XMMath4::Min(ZShadeSandboxMath::Vec4<XMFLOAT4> val)
{
	XMFLOAT4 ret;
	
	ret.x = (x < val.Vector().x) ? x : val.Vector().x;
	ret.y = (y < val.Vector().y) ? y : val.Vector().y;
	ret.z = (z < val.Vector().z) ? z : val.Vector().z;
	ret.w = (w < val.Vector().w) ? w : val.Vector().w;
	
	return ret;
}
//==============================================================================================================================
XMFLOAT4 XMMath4::Min(XMVECTOR val)
{
	XMFLOAT4 vf;
	XMStoreFloat4(&vf, val);
	
	XMFLOAT4 ret;
	
	ret.x = (x < vf.x) ? x : vf.x;
	ret.y = (y < vf.y) ? y : vf.y;
	ret.z = (z < vf.z) ? z : vf.z;
	ret.w = (w < vf.w) ? w : vf.w;
	
	return ret;
}
//==============================================================================================================================
ZShadeSandboxMath::Vec4<XMFLOAT4> XMMath4::ToFloatVector(XMFLOAT4 val)
{
	return ZShadeSandboxMath::Vec4<XMFLOAT4>(val.x, val.y, val.z, val.w);
}
//==============================================================================================================================
ZShadeSandboxMath::Vec4<XMFLOAT4> XMMath4::ToFloatVector(XMVECTOR val)
{
	XMFLOAT4 ret;
	XMStoreFloat4(&ret, val);
	return ZShadeSandboxMath::Vec4<XMFLOAT4>(ret.x, ret.y, ret.z, ret.w);
}
//==============================================================================================================================
ZShadeSandboxMath::Vec4<XMFLOAT4> XMMath4::ToFloatVector() const
{
	return ZShadeSandboxMath::Vec4<XMFLOAT4>(x, y, z, w);
}
//==============================================================================================================================
XMVECTOR XMMath4::ToVector(XMFLOAT4 val)
{
	return XMLoadFloat4(&val);
}
//==============================================================================================================================
XMFLOAT4 XMMath4::ToFloat(XMVECTOR val)
{
	XMFLOAT4 ret;
	XMStoreFloat4(&ret, val);
	return ret;
}
//==============================================================================================================================
XMVECTOR XMMath4::ToVector() const
{
	XMFLOAT4 data(x, y, z, w);
	return XMLoadFloat4(&data);
}
//==============================================================================================================================
XMFLOAT4 XMMath4::ToFloat()
{
	XMFLOAT4 ret;
	XMStoreFloat4(&ret, ToVector());
	return ret;
}
//==============================================================================================================================
float XMMath4::Distance(XMMath4 val)
{
	return sqrt(SQR(x - val.x) + SQR(y - val.y) + SQR(z - val.z) + SQR(w - val.w));
}
//==============================================================================================================================
float XMMath4::DistanceSquared(XMMath4 val)
{
	return SQR(x - val.x) + SQR(y - val.y) + SQR(z - val.z) + SQR(w - val.w);
}
//==============================================================================================================================
void XMMath4::Assign(const XMMath4& val)
{
	x = val.x;
	y = val.y;
	z = val.z;
	w = val.w;
}
//==============================================================================================================================
void XMMath4::Assign(XMFLOAT4 val)
{
	x = val.x;
	y = val.y;
	z = val.z;
	w = val.w;
}
//==============================================================================================================================
void XMMath4::Assign(float val)
{
	x = val;
	y = val;
	z = val;
	w = val;
}
//==============================================================================================================================
XMFLOAT4 XMMath4::Normalize()
{
	return ToFloat(XMVector4Normalize(ToVector()));
}
//==============================================================================================================================
void XMMath4::NormalizeMe()
{
	Assign(ToFloat(XMVector4Normalize(ToVector())));
}
//==============================================================================================================================
//XMFLOAT4 XMMath4::TransformCoord(XMMATRIX mat)
//{
//	return ToFloat(XMVector4TransformCoord(ToVector(), mat));
//}
//==============================================================================================================================
XMFLOAT4 XMMath4::Transform(const XMMATRIX& mat)
{
	return ToFloat(XMVector4Transform(ToVector(), mat));
}
//==============================================================================================================================
float XMMath4::Dot(XMMath4 val)
{
	ZShadeSandboxMath::Vec4<XMFLOAT4> v(val.x, val.y, val.z, val.w);
	ZShadeSandboxMath::Vec4<XMFLOAT4> data(x, y, z, w);
	return data.Dot(v);
}
//==============================================================================================================================
float XMMath4::Dot(XMFLOAT4 val)
{
	ZShadeSandboxMath::Vec4<XMFLOAT4> v(val.x, val.y, val.z, val.w);
	ZShadeSandboxMath::Vec4<XMFLOAT4> data(x, y, z, w);
	return data.Dot(v);
}
//==============================================================================================================================
XMMath4 XMMath4::Cross(XMMath4 val)
{
	ZShadeSandboxMath::Vec4<XMFLOAT4> v2(val.x, val.y, val.z, val.w);
	ZShadeSandboxMath::Vec4<XMFLOAT4> v(x, y, z, w);
	XMFLOAT4 data = v.Cross(v2);
	x = data.x;
	y = data.y;
	z = data.z;
	w = data.w;
	return XMMath4(data);
}
//==============================================================================================================================
XMMath4 XMMath4::Cross(XMMath4 val1, XMMath4 val2)
{
	ZShadeSandboxMath::Vec4<XMFLOAT4> v1(val1.x, val1.y, val1.z, val1.w);
	ZShadeSandboxMath::Vec4<XMFLOAT4> v2(val2.x, val2.y, val2.z, val2.w);
	ZShadeSandboxMath::Vec4<XMFLOAT4> v(x, y, z, w);
	XMFLOAT4 data = v.Cross(v1, v2);
	x = data.x;
	y = data.y;
	z = data.z;
	w = data.w;
	return XMMath4(data);
}
//==============================================================================================================================
XMMath4 XMMath4::Cross(XMFLOAT4 val)
{
	ZShadeSandboxMath::Vec4<XMFLOAT4> v2(val.x, val.y, val.z, val.w);
	ZShadeSandboxMath::Vec4<XMFLOAT4> v(x, y, z, w);
	XMFLOAT4 data = v.Cross( v2 );
	x = data.x;
	y = data.y;
	z = data.z;
	w = data.w;
	return XMMath4(data);
}
//==============================================================================================================================
XMMath4 XMMath4::Cross(XMFLOAT4 val1, XMFLOAT4 val2)
{
	ZShadeSandboxMath::Vec4<XMFLOAT4> v1(val1.x, val1.y, val1.z, val1.w);
	ZShadeSandboxMath::Vec4<XMFLOAT4> v2(val2.x, val2.y, val2.z, val2.w);
	ZShadeSandboxMath::Vec4<XMFLOAT4> v(x, y, z, w);
	XMFLOAT4 data = v.Cross( v1, v2 );
	x = data.x;
	y = data.y;
	z = data.z;
	w = data.w;
	return XMMath4(data);
}
//==============================================================================================================================
XMMath4 XMMath4::Cross(XMVECTOR val1, XMVECTOR val2)
{
	XMFLOAT4 fval1 = ToFloat(val1);
	XMFLOAT4 fval2 = ToFloat(val2);
	ZShadeSandboxMath::Vec4<XMFLOAT4> v1(fval1.x, fval1.y, fval1.z, fval1.w);
	ZShadeSandboxMath::Vec4<XMFLOAT4> v2(fval2.x, fval2.y, fval2.z, fval2.w);
	ZShadeSandboxMath::Vec4<XMFLOAT4> v(x, y, z, w);
	XMFLOAT4 data = v.Cross( v1, v2 );
	x = data.x;
	y = data.y;
	z = data.z;
	w = data.w;
	return XMMath4(data);
}
//==============================================================================================================================
float XMMath4::Length()
{
	ZShadeSandboxMath::Vec4<XMFLOAT4> data(x, y, z, w);
	return data.Magnitude();
}
//==============================================================================================================================
float XMMath4::LengthSquared()
{
	return x*x + y*y + z*z + w*w;
}
//==============================================================================================================================
void XMMath4::Add(const XMMath4& val)
{
	XMFLOAT4 data = ToFloat(XMVectorAdd(ToVector(), val.ToVector()));
	x = data.x;
	y = data.y;
	z = data.z;
	w = data.w;
}
//==============================================================================================================================
void XMMath4::Subtract(const XMMath4& val)
{
	XMFLOAT4 data = ToFloat(XMVectorSubtract(ToVector(), val.ToVector()));
	x = data.x;
	y = data.y;
	z = data.z;
	w = data.w;
}
//==============================================================================================================================
void XMMath4::Multiply(const XMMath4& val)
{
	XMFLOAT4 data = ToFloat(XMVectorMultiply(ToVector(), val.ToVector()));
	x = data.x;
	y = data.y;
	z = data.z;
	w = data.w;
}
//==============================================================================================================================
void XMMath4::Divide(const XMMath4& val)
{
	XMFLOAT4 data = ToFloat(XMVectorDivide(ToVector(), val.ToVector()));
	x = data.x;
	y = data.y;
	z = data.z;
	w = data.w;
}
//==============================================================================================================================
XMMath4& XMMath4::operator += (const XMMath4& val)
{
	Add(val);
	return *this;
}
//==============================================================================================================================
XMMath4& XMMath4::operator -= (const XMMath4& val)
{
	Subtract(val);
	return *this;
}
//==============================================================================================================================
XMMath4& XMMath4::operator *= (const XMMath4& val)
{
	Multiply(val);
	return *this;
}
//==============================================================================================================================
XMMath4& XMMath4::operator /= (const XMMath4& val)
{
	Divide(val);
	return *this;
}
//==============================================================================================================================
XMMath4& XMMath4::operator + (const XMMath4& val)
{
	Add(val);
	return *this;
}
//==============================================================================================================================
XMMath4& XMMath4::operator - (const XMMath4& val)
{
	Subtract(val);
	return *this;
}
//==============================================================================================================================
XMMath4& XMMath4::operator * (const XMMath4& val)
{
	Multiply(val);
	return *this;
}
//==============================================================================================================================
XMMath4& XMMath4::operator / (const XMMath4& val)
{
	Divide(val);
	return *this;
}
//==============================================================================================================================
XMMath4& XMMath4::operator = (const XMMath4& val)
{
	Assign(val);
	return *this;
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath4::Equals(const ZShadeSandboxMath::XMMath4& val)
{
	return (x == val.x && y == val.y && z == val.z && w == val.w);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath4::LessThanEquals(const ZShadeSandboxMath::XMMath4& val)
{
	return (x <= val.x && y <= val.y && z <= val.z && w <= val.w);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath4::GreaterThanEquals(const ZShadeSandboxMath::XMMath4& val)
{
	return (x >= val.x && y >= val.y && z >= val.z && w >= val.w);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath4::LessThan(const ZShadeSandboxMath::XMMath4& val)
{
	return (x < val.x && y < val.y && z < val.z && w < val.w);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath4::GreaterThan(const ZShadeSandboxMath::XMMath4& val)
{
	return (x > val.x && y > val.y && z > val.z);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath4::operator <= (const ZShadeSandboxMath::XMMath4& val)
{
	return LessThanEquals(val);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath4::operator >= (const ZShadeSandboxMath::XMMath4& val)
{
	return GreaterThanEquals(val);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath4::operator < (const ZShadeSandboxMath::XMMath4& val)
{
	return LessThan(val);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath4::operator > (const ZShadeSandboxMath::XMMath4& val)
{
	return GreaterThan(val);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath4::operator == (const ZShadeSandboxMath::XMMath4& val)
{
	return Equals(val);
}
//==============================================================================================================================
void XMMath4::Add(const float& val)
{
	x += val;
	y += val;
	z += val;
	w += val;
}
//==============================================================================================================================
void XMMath4::Subtract(const float& val)
{
	x -= val;
	y -= val;
	z -= val;
	w -= val;
}
//==============================================================================================================================
void XMMath4::Multiply(const float& val)
{
	x *= val;
	y *= val;
	z *= val;
	w *= val;
}
//==============================================================================================================================
void XMMath4::Divide(const float& val)
{
	x /= val;
	y /= val;
	z /= val;
	w /= val;
}
//==============================================================================================================================
XMMath4& XMMath4::operator += (const float& val)
{
	Add(val);
	return *this;
}
//==============================================================================================================================
XMMath4& XMMath4::operator -= (const float& val)
{
	Subtract(val);
	return *this;
}
//==============================================================================================================================
XMMath4& XMMath4::operator *= (const float& val)
{
	Multiply(val);
	return *this;
}
//==============================================================================================================================
XMMath4& XMMath4::operator /= (const float& val)
{
	Divide(val);
	return *this;
}
//==============================================================================================================================
XMMath4& XMMath4::operator + (const float& val)
{
	Add(val);
	return *this;
}
//==============================================================================================================================
XMMath4& XMMath4::operator - (const float& val)
{
	Subtract(val);
	return *this;
}
//==============================================================================================================================
XMMath4& XMMath4::operator * (const float& val)
{
	Multiply(val);
	return *this;
}
//==============================================================================================================================
XMMath4& XMMath4::operator / (const float& val)
{
	Divide(val);
	return *this;
}
//==============================================================================================================================
XMMath4& XMMath4::operator = (const float& val)
{
	Assign(val);
	return *this;
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath4::Equals(const float& val)
{
	return (x == val && y == val && z == val && w == val);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath4::LessThanEquals(const float& val)
{
	return (x <= val && y <= val && z <= val && w <= val);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath4::GreaterThanEquals(const float& val)
{
	return (x >= val && y >= val && z >= val && w >= val);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath4::LessThan(const float& val)
{
	return (x < val && y < val && z < val && w < val);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath4::GreaterThan(const float& val)
{
	return (x > val && y > val && z > val && w > val);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath4::operator <= (const float& val)
{
	return LessThanEquals(val);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath4::operator >= (const float& val)
{
	return GreaterThanEquals(val);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath4::operator < (const float& val)
{
	return LessThan(val);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath4::operator > (const float& val)
{
	return GreaterThan(val);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath4::operator == (const float& val)
{
	return Equals(val);
}
//==============================================================================================================================