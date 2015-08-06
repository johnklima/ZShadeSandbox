#include "XMMath2.h"
using ZShadeSandboxMath::XMMath2;
//==============================================================================================================================
//==============================================================================================================================
XMMath2::XMMath2()
{
}
//==============================================================================================================================
XMMath2::XMMath2(const XMMath2& v)
{
	x = v.x;
	y = v.y;
}
//==============================================================================================================================
XMMath2::XMMath2(float x, float y)
{
	this->x = x;
	this->y = y;
}
//==============================================================================================================================
XMMath2::XMMath2(XMFLOAT2 v)
{
	x = v.x;
	y = v.y;
}
//==============================================================================================================================
XMMath2::XMMath2(XMVECTOR v)
{
	XMFLOAT2 vf;
	XMStoreFloat2(&vf, v);
	x = vf.x;
	y = vf.y;
}
//==============================================================================================================================
XMMath2::XMMath2(ZShadeSandboxMath::Vec2<XMFLOAT2> v)
{
	x = v.Vector().x;
	y = v.Vector().y;
}
//==============================================================================================================================
XMMath2 XMMath2::Clamp(XMMath2 value, XMMath2 pmin, XMMath2 pmax)
{
	float tx = value.x;
	tx = (tx > pmax.x) ? pmax.x : tx;
	tx = (tx < pmin.x) ? pmin.x : tx;
	
	float ty = value.y;
	ty = (ty > pmax.y) ? pmax.y : ty;
	ty = (ty < pmin.y) ? pmin.y : ty;
	
	return XMMath2(tx, ty);
}
//==============================================================================================================================
XMFLOAT2 XMMath2::Max(float x2, float y2)
{
	XMFLOAT2 ret;
	
	ret.x = (x > x2) ? x : x2;
	ret.y = (y > y2) ? y : y2;
	
	return ret;
}
//==============================================================================================================================
XMFLOAT2 XMMath2::Max(XMMath2 val)
{
	XMFLOAT2 ret;
	
	ret.x = (x > val.x) ? x : val.x;
	ret.y = (y > val.y) ? y : val.y;
	
	return ret;
}
//==============================================================================================================================
XMFLOAT2 XMMath2::Max(XMFLOAT2 val)
{
	XMFLOAT2 ret;
	
	ret.x = (x > val.x) ? x : val.x;
	ret.y = (y > val.y) ? y : val.y;
	
	return ret;
}
//==============================================================================================================================
XMFLOAT2 XMMath2::Max(ZShadeSandboxMath::Vec2<XMFLOAT2> val)
{
	XMFLOAT2 ret;
	
	ret.x = (x > val.Vector().x) ? x : val.Vector().x;
	ret.y = (y > val.Vector().y) ? y : val.Vector().y;
	
	return ret;
}
//==============================================================================================================================
XMFLOAT2 XMMath2::Max(XMVECTOR val)
{
	XMFLOAT2 vf;
	XMStoreFloat2(&vf, val);
	
	XMFLOAT2 ret;
	
	ret.x = (x > vf.x) ? x : vf.x;
	ret.y = (y > vf.y) ? y : vf.y;
	
	return ret;
}
//==============================================================================================================================
XMFLOAT2 XMMath2::Min(float x2, float y2)
{
	XMFLOAT2 ret;
	
	ret.x = (x < x2) ? x : x2;
	ret.y = (y < y2) ? y : y2;
	
	return ret;
}
//==============================================================================================================================
XMFLOAT2 XMMath2::Min(XMMath2 val)
{
	XMFLOAT2 ret;
	
	ret.x = (x < val.x) ? x : val.x;
	ret.y = (y < val.y) ? y : val.y;
	
	return ret;
}
//==============================================================================================================================
XMFLOAT2 XMMath2::Min(XMFLOAT2 val)
{
	XMFLOAT2 ret;
	
	ret.x = (x < val.x) ? x : val.x;
	ret.y = (y < val.y) ? y : val.y;
	
	return ret;
}
//==============================================================================================================================
XMFLOAT2 XMMath2::Min(ZShadeSandboxMath::Vec2<XMFLOAT2> val)
{
	XMFLOAT2 ret;
	
	ret.x = (x < val.Vector().x) ? x : val.Vector().x;
	ret.y = (y < val.Vector().y) ? y : val.Vector().y;
	
	return ret;
}
//==============================================================================================================================
XMFLOAT2 XMMath2::Min(XMVECTOR val)
{
	XMFLOAT2 vf;
	XMStoreFloat2(&vf, val);
	
	XMFLOAT2 ret;
	
	ret.x = (x < vf.x) ? x : vf.x;
	ret.y = (y < vf.y) ? y : vf.y;
	
	return ret;
}
//==============================================================================================================================
ZShadeSandboxMath::Vec2<XMFLOAT2> XMMath2::ToFloatVector(XMFLOAT2 val)
{
	return ZShadeSandboxMath::Vec2<XMFLOAT2>(val.x, val.y);
}
//==============================================================================================================================
ZShadeSandboxMath::Vec2<XMFLOAT2> XMMath2::ToFloatVector(XMVECTOR val)
{
	XMFLOAT2 ret;
	XMStoreFloat2(&ret, val);
	return ZShadeSandboxMath::Vec2<XMFLOAT2>(ret.x, ret.y);
}
//==============================================================================================================================
ZShadeSandboxMath::Vec2<XMFLOAT2> XMMath2::ToFloatVector() const
{
	return ZShadeSandboxMath::Vec2<XMFLOAT2>(x, y);
}
//==============================================================================================================================
XMVECTOR XMMath2::ToVector(XMFLOAT2 val)
{
	return XMLoadFloat2(&val);
}
//==============================================================================================================================
XMFLOAT2 XMMath2::ToFloat(XMVECTOR val)
{
	XMFLOAT2 ret;
	XMStoreFloat2(&ret, val);
	return ret;
}
//==============================================================================================================================
XMVECTOR XMMath2::ToVector() const
{
	ZShadeSandboxMath::Vec2<XMFLOAT2> data(x, y);
	return XMLoadFloat2(&data.Vector());
}
//==============================================================================================================================
XMFLOAT2 XMMath2::ToFloat()
{
	XMFLOAT2 ret;
	XMStoreFloat2(&ret, ToVector());
	return ret;
}
//==============================================================================================================================
float XMMath2::Distance(XMMath2 val)
{
	return sqrt(SQR(x - val.x) + SQR(y - val.y));
}
//==============================================================================================================================
float XMMath2::DistanceSquared(XMMath2 val)
{
	return SQR(x - val.x) + SQR(y - val.y);
}
//==============================================================================================================================
void XMMath2::Assign(const XMMath2& val)
{
	x = val.x;
	y = val.y;
}
//==============================================================================================================================
void XMMath2::Assign(XMFLOAT2 val)
{
	x = val.x;
	y = val.y;
}
//==============================================================================================================================
void XMMath2::Assign(float val)
{
	x = val;
	y = val;
}
//==============================================================================================================================
XMFLOAT2 XMMath2::Normalize()
{
	return ToFloat(XMVector2Normalize(ToVector()));
}
//==============================================================================================================================
void XMMath2::NormalizeMe()
{
	Assign(ToFloat(XMVector2Normalize(ToVector())));
}
//==============================================================================================================================
XMFLOAT2 XMMath2::TransformCoord(const XMMATRIX& mat)
{
	return ToFloat(XMVector2TransformCoord(ToVector(), mat));
}
//==============================================================================================================================
void XMMath2::TransformCoord(XMFLOAT2 val, const XMMATRIX& mat)
{
	XMFLOAT2 data = ToFloat(XMVector2TransformCoord(ToVector(val), mat));
	x = data.x;
	y = data.y;
}
//==============================================================================================================================
void XMMath2::TransformCoord(XMVECTOR val, const XMMATRIX& mat)
{
	XMFLOAT2 data = ToFloat(XMVector2TransformCoord(val, mat));
	x = data.x;
	y = data.y;
}
//==============================================================================================================================
XMFLOAT2 XMMath2::Transform(const XMMATRIX& mat)
{
	return ToFloat(XMVector2Transform(ToVector(), mat));
}
//==============================================================================================================================
float XMMath2::Dot(XMMath2 val)
{
	ZShadeSandboxMath::Vec2<XMFLOAT2> v(val.x, val.y);
	ZShadeSandboxMath::Vec2<XMFLOAT2> data(x, y);
	return data.Dot(v);
}
//==============================================================================================================================
float XMMath2::Dot(XMFLOAT2 val)
{
	ZShadeSandboxMath::Vec2<XMFLOAT2> v(val.x, val.y);
	ZShadeSandboxMath::Vec2<XMFLOAT2> data(x, y);
	return data.Dot(v);
}
//==============================================================================================================================
float XMMath2::Length()
{
	ZShadeSandboxMath::Vec2<XMFLOAT2> data(x, y);
	return data.Magnitude();
}
//==============================================================================================================================
float XMMath2::LengthSquared()
{
	return x*x + y*y;
}
//==============================================================================================================================
void XMMath2::Add(const XMMath2& val)
{
	XMFLOAT2 data = ToFloat(XMVectorAdd(ToVector(), val.ToVector()));
	x = data.x;
	y = data.y;
}
//==============================================================================================================================
void XMMath2::Subtract(const XMMath2& val)
{
	XMFLOAT2 data = ToFloat(XMVectorSubtract(ToVector(), val.ToVector()));
	x = data.x;
	y = data.y;
}
//==============================================================================================================================
void XMMath2::Multiply(const XMMath2& val)
{
	XMFLOAT2 data = ToFloat(XMVectorMultiply(ToVector(), val.ToVector()));
	x = data.x;
	y = data.y;
}
//==============================================================================================================================
void XMMath2::Divide(const XMMath2& val)
{
	XMFLOAT2 data = ToFloat(XMVectorDivide(ToVector(), val.ToVector()));
	x = data.x;
	y = data.y;
}
//==============================================================================================================================
XMMath2& XMMath2::operator += (const XMMath2& val)
{
	Add(val);
	return *this;
}
//==============================================================================================================================
XMMath2& XMMath2::operator -= (const XMMath2& val)
{
	Subtract(val);
	return *this;
}
//==============================================================================================================================
XMMath2& XMMath2::operator *= (const XMMath2& val)
{
	Multiply(val);
	return *this;
}
//==============================================================================================================================
XMMath2& XMMath2::operator /= (const XMMath2& val)
{
	Divide(val);
	return *this;
}
//==============================================================================================================================
XMMath2& XMMath2::operator + (const XMMath2& val)
{
	Add(val);
	return *this;
}
//==============================================================================================================================
XMMath2& XMMath2::operator - (const XMMath2& val)
{
	Subtract(val);
	return *this;
}
//==============================================================================================================================
XMMath2& XMMath2::operator * (const XMMath2& val)
{
	Multiply(val);
	return *this;
}
//==============================================================================================================================
XMMath2& XMMath2::operator / (const XMMath2& val)
{
	Divide(val);
	return *this;
}
//==============================================================================================================================
XMMath2& XMMath2::operator = (const XMMath2& val)
{
	Assign(val);
	return *this;
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath2::Equals(const ZShadeSandboxMath::XMMath2& val)
{
	return (x == val.x && y == val.y);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath2::LessThanEquals(const ZShadeSandboxMath::XMMath2& val)
{
	return (x <= val.x && y <= val.y);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath2::GreaterThanEquals(const ZShadeSandboxMath::XMMath2& val)
{
	return (x >= val.x && y >= val.y);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath2::LessThan(const ZShadeSandboxMath::XMMath2& val)
{
	return (x < val.x && y < val.y);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath2::GreaterThan(const ZShadeSandboxMath::XMMath2& val)
{
	return (x > val.x && y > val.y);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath2::operator <= (const ZShadeSandboxMath::XMMath2& val)
{
	return LessThanEquals(val);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath2::operator >= (const ZShadeSandboxMath::XMMath2& val)
{
	return GreaterThanEquals(val);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath2::operator < (const ZShadeSandboxMath::XMMath2& val)
{
	return LessThan(val);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath2::operator > (const ZShadeSandboxMath::XMMath2& val)
{
	return GreaterThan(val);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath2::operator == (const ZShadeSandboxMath::XMMath2& val)
{
	return Equals(val);
}
//==============================================================================================================================
void XMMath2::Add(const float& val)
{
	x += val;
	y += val;
}
//==============================================================================================================================
void XMMath2::Subtract(const float& val)
{
	x -= val;
	y -= val;
}
//==============================================================================================================================
void XMMath2::Multiply(const float& val)
{
	x *= val;
	y *= val;
}
//==============================================================================================================================
void XMMath2::Divide(const float& val)
{
	x /= val;
	y /= val;
}
//==============================================================================================================================
XMMath2& XMMath2::operator += (const float& val)
{
	Add(val);
	return *this;
}
//==============================================================================================================================
XMMath2& XMMath2::operator -= (const float& val)
{
	Subtract(val);
	return *this;
}
//==============================================================================================================================
XMMath2& XMMath2::operator *= (const float& val)
{
	Multiply(val);
	return *this;
}
//==============================================================================================================================
XMMath2& XMMath2::operator /= (const float& val)
{
	Divide(val);
	return *this;
}
//==============================================================================================================================
XMMath2& XMMath2::operator + (const float& val)
{
	Add(val);
	return *this;
}
//==============================================================================================================================
XMMath2& XMMath2::operator - (const float& val)
{
	Subtract(val);
	return *this;
}
//==============================================================================================================================
XMMath2& XMMath2::operator * (const float& val)
{
	Multiply(val);
	return *this;
}
//==============================================================================================================================
XMMath2& XMMath2::operator / (const float& val)
{
	Divide(val);
	return *this;
}
//==============================================================================================================================
XMMath2& XMMath2::operator = (const float& val)
{
	Assign(val);
	return *this;
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath2::Equals(const float& val)
{
	return (x == val && y == val);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath2::LessThanEquals(const float& val)
{
	return (x <= val && y <= val);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath2::GreaterThanEquals(const float& val)
{
	return (x >= val && y >= val);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath2::LessThan(const float& val)
{
	return (x < val && y < val);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath2::GreaterThan(const float& val)
{
	return (x > val && y > val);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath2::operator <= (const float& val)
{
	return LessThanEquals(val);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath2::operator >= (const float& val)
{
	return GreaterThanEquals(val);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath2::operator < (const float& val)
{
	return LessThan(val);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath2::operator > (const float& val)
{
	return GreaterThan(val);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath2::operator == (const float& val)
{
	return Equals(val);
}
//==============================================================================================================================