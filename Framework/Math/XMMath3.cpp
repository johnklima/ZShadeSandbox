#include "XMMath3.h"
//using ZShadeSandboxMath::XMMath3;
//==============================================================================================================================
//==============================================================================================================================
ZShadeSandboxMath::XMMath3::XMMath3()
{
}
//==============================================================================================================================
ZShadeSandboxMath::XMMath3::XMMath3(const ZShadeSandboxMath::XMMath3& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
}
//==============================================================================================================================
ZShadeSandboxMath::XMMath3::XMMath3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}
//==============================================================================================================================
ZShadeSandboxMath::XMMath3::XMMath3(XMFLOAT3 v)
{
	x = v.x;
	y = v.y;
	z = v.z;
}
//==============================================================================================================================
ZShadeSandboxMath::XMMath3::XMMath3(XMVECTOR v)
{
	XMFLOAT3 vf;
	XMStoreFloat3(&vf, v);
	x = vf.x;
	y = vf.y;
	z = vf.z;
}
//==============================================================================================================================
ZShadeSandboxMath::XMMath3::XMMath3(ZShadeSandboxMath::Vec3<XMFLOAT3> v)
{
	x = v.Vector().x;
	y = v.Vector().y;
	z = v.Vector().z;
}
//==============================================================================================================================
ZShadeSandboxMath::XMMath3 ZShadeSandboxMath::XMMath3::Clamp(ZShadeSandboxMath::XMMath3 value, ZShadeSandboxMath::XMMath3 pmin, ZShadeSandboxMath::XMMath3 pmax)
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
	
	return ZShadeSandboxMath::XMMath3(tx, ty, tz);
}
//==============================================================================================================================
XMFLOAT3 ZShadeSandboxMath::XMMath3::Max(float x2, float y2, float z2)
{
	XMFLOAT3 ret;
	
	ret.x = (x > x2) ? x : x2;
	ret.y = (y > y2) ? y : y2;
	ret.z = (z > z2) ? z : z2;
	
	return ret;
}
//==============================================================================================================================
XMFLOAT3 ZShadeSandboxMath::XMMath3::Max(ZShadeSandboxMath::XMMath3 val)
{
	XMFLOAT3 ret;
	
	ret.x = (x > val.x) ? x : val.x;
	ret.y = (y > val.y) ? y : val.y;
	ret.z = (z > val.z) ? z : val.z;
	
	return ret;
}
//==============================================================================================================================
XMFLOAT3 ZShadeSandboxMath::XMMath3::Max(XMFLOAT3 val)
{
	XMFLOAT3 ret;
	
	ret.x = (x > val.x) ? x : val.x;
	ret.y = (y > val.y) ? y : val.y;
	ret.z = (z > val.z) ? z : val.z;
	
	return ret;
}
//==============================================================================================================================
XMFLOAT3 ZShadeSandboxMath::XMMath3::Max(ZShadeSandboxMath::Vec3<XMFLOAT3> val)
{
	XMFLOAT3 ret;
	
	ret.x = (x > val.Vector().x) ? x : val.Vector().x;
	ret.y = (y > val.Vector().y) ? y : val.Vector().y;
	ret.z = (z > val.Vector().z) ? z : val.Vector().z;
	
	return ret;
}
//==============================================================================================================================
XMFLOAT3 ZShadeSandboxMath::XMMath3::Max(XMVECTOR val)
{
	XMFLOAT3 vf;
	XMStoreFloat3(&vf, val);
	
	XMFLOAT3 ret;
	
	ret.x = (x > vf.x) ? x : vf.x;
	ret.y = (y > vf.y) ? y : vf.y;
	ret.z = (z > vf.z) ? z : vf.z;
	
	return ret;
}
//==============================================================================================================================
XMFLOAT3 ZShadeSandboxMath::XMMath3::Min(float x2, float y2, float z2)
{
	XMFLOAT3 ret;
	
	ret.x = (x < x2) ? x : x2;
	ret.y = (y < y2) ? y : y2;
	ret.z = (z < z2) ? z : z2;
	
	return ret;
}
//==============================================================================================================================
XMFLOAT3 ZShadeSandboxMath::XMMath3::Min(ZShadeSandboxMath::XMMath3 val)
{
	XMFLOAT3 ret;
	
	ret.x = (x < val.x) ? x : val.x;
	ret.y = (y < val.y) ? y : val.y;
	ret.z = (z < val.z) ? z : val.z;
	
	return ret;
}
//==============================================================================================================================
XMFLOAT3 ZShadeSandboxMath::XMMath3::Min(XMFLOAT3 val)
{
	XMFLOAT3 ret;
	
	ret.x = (x < val.x) ? x : val.x;
	ret.y = (y < val.y) ? y : val.y;
	ret.z = (z < val.z) ? z : val.z;
	
	return ret;
}
//==============================================================================================================================
XMFLOAT3 ZShadeSandboxMath::XMMath3::Min(ZShadeSandboxMath::Vec3<XMFLOAT3> val)
{
	XMFLOAT3 ret;
	
	ret.x = (x < val.Vector().x) ? x : val.Vector().x;
	ret.y = (y < val.Vector().y) ? y : val.Vector().y;
	ret.z = (z < val.Vector().z) ? z : val.Vector().z;
	
	return ret;
}
//==============================================================================================================================
XMFLOAT3 ZShadeSandboxMath::XMMath3::Min(XMVECTOR val)
{
	XMFLOAT3 vf;
	XMStoreFloat3(&vf, val);
	
	XMFLOAT3 ret;
	
	ret.x = (x < vf.x) ? x : vf.x;
	ret.y = (y < vf.y) ? y : vf.y;
	ret.z = (z < vf.z) ? z : vf.z;
	
	return ret;
}
//==============================================================================================================================
ZShadeSandboxMath::Vec3<XMFLOAT3> ZShadeSandboxMath::XMMath3::ToFloatVector(XMFLOAT3 val)
{
	return ZShadeSandboxMath::Vec3<XMFLOAT3>(val.x, val.y, val.z);
}
//==============================================================================================================================
ZShadeSandboxMath::Vec3<XMFLOAT3> ZShadeSandboxMath::XMMath3::ToFloatVector(XMVECTOR val)
{
	XMFLOAT3 ret;
	XMStoreFloat3(&ret, val);
	return ZShadeSandboxMath::Vec3<XMFLOAT3>(ret.x, ret.y, ret.z);
}
//==============================================================================================================================
ZShadeSandboxMath::Vec3<XMFLOAT3> ZShadeSandboxMath::XMMath3::ToFloatVector() const
{
	return ZShadeSandboxMath::Vec3<XMFLOAT3>(x, y, z);
}
//==============================================================================================================================
XMVECTOR ZShadeSandboxMath::XMMath3::ToVector(XMFLOAT3 val)
{
	return XMLoadFloat3(&val);
}
//==============================================================================================================================
XMFLOAT3 ZShadeSandboxMath::XMMath3::ToFloat(XMVECTOR val)
{
	XMFLOAT3 ret;
	XMStoreFloat3(&ret, val);
	return ret;
}
//==============================================================================================================================
XMVECTOR ZShadeSandboxMath::XMMath3::ToVector() const
{
	XMFLOAT3 data(x, y, z);
	return XMLoadFloat3(&data);
}
//==============================================================================================================================
XMFLOAT3 ZShadeSandboxMath::XMMath3::ToFloat()
{
	XMFLOAT3 ret;
	XMStoreFloat3(&ret, ToVector());
	return ret;
}
//==============================================================================================================================
float ZShadeSandboxMath::XMMath3::Distance(ZShadeSandboxMath::XMMath3 val)
{
	return sqrt(SQR(x - val.x) + SQR(y - val.y) + SQR(z - val.z));
}
//==============================================================================================================================
float ZShadeSandboxMath::XMMath3::DistanceSquared(ZShadeSandboxMath::XMMath3 val)
{
	return SQR(x - val.x) + SQR(y - val.y) + SQR(z - val.z);
}
//==============================================================================================================================
void ZShadeSandboxMath::XMMath3::Assign(const ZShadeSandboxMath::XMMath3& val)
{
	x = val.x;
	y = val.y;
	z = val.z;
}
//==============================================================================================================================
void ZShadeSandboxMath::XMMath3::Assign(XMFLOAT3 val)
{
	x = val.x;
	y = val.y;
	z = val.z;
}
//==============================================================================================================================
void ZShadeSandboxMath::XMMath3::Assign(float val)
{
	x = val;
	y = val;
	z = val;
}
//==============================================================================================================================
XMFLOAT3 ZShadeSandboxMath::XMMath3::Normalize()
{
	return ToFloat(XMVector3Normalize(ToVector()));
}
//==============================================================================================================================
void ZShadeSandboxMath::XMMath3::NormalizeMe()
{
	Assign(ToFloat(XMVector3Normalize(ToVector())));
}
//==============================================================================================================================
XMFLOAT3 ZShadeSandboxMath::XMMath3::TransformCoord(const XMMATRIX& mat)
{
	return ToFloat(XMVector3TransformCoord(ToVector(), mat));
}
//==============================================================================================================================
void ZShadeSandboxMath::XMMath3::TransformCoord(XMFLOAT3 val, const XMMATRIX& mat)
{
	XMFLOAT3 data = ToFloat(XMVector3TransformCoord(ToVector(val), mat));
	x = data.x;
	y = data.y;
	z = data.z;
}
//==============================================================================================================================
void ZShadeSandboxMath::XMMath3::TransformCoord(XMVECTOR val, const XMMATRIX& mat)
{
	XMFLOAT3 data = ToFloat(XMVector3TransformCoord(val, mat));
	x = data.x;
	y = data.y;
	z = data.z;
}
//==============================================================================================================================
XMFLOAT3 ZShadeSandboxMath::XMMath3::Transform(const XMMATRIX& mat)
{
	return ToFloat(XMVector3Transform(ToVector(), mat));
}
//==============================================================================================================================
float ZShadeSandboxMath::XMMath3::Dot(ZShadeSandboxMath::XMMath3 val)
{
	ZShadeSandboxMath::Vec3<XMFLOAT3> v(val.x, val.y, val.z);
	ZShadeSandboxMath::Vec3<XMFLOAT3> data(x, y, z);
	return data.Dot(v);
}
//==============================================================================================================================
float ZShadeSandboxMath::XMMath3::Dot(XMFLOAT3 val)
{
	ZShadeSandboxMath::Vec3<XMFLOAT3> v(val.x, val.y, val.z);
	ZShadeSandboxMath::Vec3<XMFLOAT3> data(x, y, z);
	return data.Dot(v);
}
//==============================================================================================================================
ZShadeSandboxMath::XMMath3 ZShadeSandboxMath::XMMath3::Cross(ZShadeSandboxMath::XMMath3 val)
{
	XMFLOAT3 data = ToFloat(XMVector3Cross(ToVector(), val.ToVector()));
	x = data.x;
	y = data.y;
	z = data.z;
	return ZShadeSandboxMath::XMMath3(data);
}
//==============================================================================================================================
ZShadeSandboxMath::XMMath3 ZShadeSandboxMath::XMMath3::Cross(ZShadeSandboxMath::XMMath3 val1, ZShadeSandboxMath::XMMath3 val2)
{
	XMFLOAT3 data = ToFloat(XMVector3Cross(ToVector(), XMVector3Cross(val1.ToVector(), val2.ToVector())));
	x = data.x;
	y = data.y;
	z = data.z;
	return ZShadeSandboxMath::XMMath3(data);
}
//==============================================================================================================================
ZShadeSandboxMath::XMMath3 ZShadeSandboxMath::XMMath3::Cross(XMFLOAT3 val)
{
	XMFLOAT3 data = ToFloat(XMVector3Cross(ToVector(), ToVector(val)));
	x = data.x;
	y = data.y;
	z = data.z;
	return ZShadeSandboxMath::XMMath3(data);
}
//==============================================================================================================================
ZShadeSandboxMath::XMMath3 ZShadeSandboxMath::XMMath3::Cross(XMFLOAT3 val1, XMFLOAT3 val2)
{
	XMFLOAT3 data = ToFloat(XMVector3Cross(ToVector(), XMVector3Cross(ToVector(val1), ToVector(val2))));
	x = data.x;
	y = data.y;
	z = data.z;
	return ZShadeSandboxMath::XMMath3(data);
}
//==============================================================================================================================
ZShadeSandboxMath::XMMath3 ZShadeSandboxMath::XMMath3::Cross(XMVECTOR val1, XMVECTOR val2)
{
	XMFLOAT3 data = ToFloat(XMVector3Cross(ToVector(), XMVector3Cross(val1, val2)));
	x = data.x;
	y = data.y;
	z = data.z;
	return ZShadeSandboxMath::XMMath3(data);
}
//==============================================================================================================================
float ZShadeSandboxMath::XMMath3::Length()
{
	ZShadeSandboxMath::Vec3<XMFLOAT3> data(x, y, z);
	return data.Magnitude();
}
//==============================================================================================================================
float ZShadeSandboxMath::XMMath3::LengthSquared()
{
	return x*x + y*y + z*z;
}
//==============================================================================================================================
void ZShadeSandboxMath::XMMath3::Add(const ZShadeSandboxMath::XMMath3& val)
{
	XMFLOAT3 data = ToFloat(XMVectorAdd(ToVector(), val.ToVector()));
	x = data.x;
	y = data.y;
	z = data.z;
}
//==============================================================================================================================
void ZShadeSandboxMath::XMMath3::Subtract(const ZShadeSandboxMath::XMMath3& val)
{
	XMFLOAT3 data = ToFloat(XMVectorSubtract(ToVector(), val.ToVector()));
	x = data.x;
	y = data.y;
	z = data.z;
}
//==============================================================================================================================
void ZShadeSandboxMath::XMMath3::Multiply(const ZShadeSandboxMath::XMMath3& val)
{
	XMFLOAT3 data = ToFloat(XMVectorMultiply(ToVector(), val.ToVector()));
	x = data.x;
	y = data.y;
	z = data.z;
}
//==============================================================================================================================
void ZShadeSandboxMath::XMMath3::Divide(const ZShadeSandboxMath::XMMath3& val)
{
	XMFLOAT3 data = ToFloat(XMVectorDivide(ToVector(), val.ToVector()));
	x = data.x;
	y = data.y;
	z = data.z;
}
//==============================================================================================================================
ZShadeSandboxMath::XMMath3& ZShadeSandboxMath::XMMath3::operator += (const ZShadeSandboxMath::XMMath3& val)
{
	Add(val);
	return *this;
}
//==============================================================================================================================
ZShadeSandboxMath::XMMath3& ZShadeSandboxMath::XMMath3::operator -= (const ZShadeSandboxMath::XMMath3& val)
{
	Subtract(val);
	return *this;
}
//==============================================================================================================================
ZShadeSandboxMath::XMMath3& ZShadeSandboxMath::XMMath3::operator *= (const ZShadeSandboxMath::XMMath3& val)
{
	Multiply(val);
	return *this;
}
//==============================================================================================================================
ZShadeSandboxMath::XMMath3& ZShadeSandboxMath::XMMath3::operator /= (const ZShadeSandboxMath::XMMath3& val)
{
	Divide(val);
	return *this;
}
//==============================================================================================================================
ZShadeSandboxMath::XMMath3& ZShadeSandboxMath::XMMath3::operator + (const ZShadeSandboxMath::XMMath3& val)
{
	Add(val);
	return *this;
}
//==============================================================================================================================
ZShadeSandboxMath::XMMath3& ZShadeSandboxMath::XMMath3::operator - (const ZShadeSandboxMath::XMMath3& val)
{
	Subtract(val);
	return *this;
}
//==============================================================================================================================
ZShadeSandboxMath::XMMath3& ZShadeSandboxMath::XMMath3::operator * (const ZShadeSandboxMath::XMMath3& val)
{
	Multiply(val);
	return *this;
}
//==============================================================================================================================
ZShadeSandboxMath::XMMath3& ZShadeSandboxMath::XMMath3::operator / (const ZShadeSandboxMath::XMMath3& val)
{
	Divide(val);
	return *this;
}
//==============================================================================================================================
ZShadeSandboxMath::XMMath3& ZShadeSandboxMath::XMMath3::operator = (const ZShadeSandboxMath::XMMath3& val)
{
	Assign(val);
	return *this;
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath3::Equals(const ZShadeSandboxMath::XMMath3& val)
{
	return (x == val.x && y == val.y && z == val.z);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath3::LessThanEquals(const ZShadeSandboxMath::XMMath3& val)
{
	return (x <= val.x && y <= val.y && z <= val.z);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath3::GreaterThanEquals(const ZShadeSandboxMath::XMMath3& val)
{
	return (x >= val.x && y >= val.y && z >= val.z);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath3::LessThan(const ZShadeSandboxMath::XMMath3& val)
{
	return (x < val.x && y < val.y && z < val.z);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath3::GreaterThan(const ZShadeSandboxMath::XMMath3& val)
{
	return (x > val.x && y > val.y && z > val.z);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath3::operator <= (const ZShadeSandboxMath::XMMath3& val)
{
	return LessThanEquals(val);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath3::operator >= (const ZShadeSandboxMath::XMMath3& val)
{
	return GreaterThanEquals(val);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath3::operator < (const ZShadeSandboxMath::XMMath3& val)
{
	return LessThan(val);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath3::operator > (const ZShadeSandboxMath::XMMath3& val)
{
	return GreaterThan(val);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath3::operator == (const ZShadeSandboxMath::XMMath3& val)
{
	return Equals(val);
}
//==============================================================================================================================
void ZShadeSandboxMath::XMMath3::Add(const float& val)
{
	x += val;
	y += val;
	z += val;
}
//==============================================================================================================================
void ZShadeSandboxMath::XMMath3::Subtract(const float& val)
{
	x -= val;
	y -= val;
	z -= val;
}
//==============================================================================================================================
void ZShadeSandboxMath::XMMath3::Multiply(const float& val)
{
	x *= val;
	y *= val;
	z *= val;
}
//==============================================================================================================================
void ZShadeSandboxMath::XMMath3::Divide(const float& val)
{
	x /= val;
	y /= val;
	z /= val;
}
//==============================================================================================================================
ZShadeSandboxMath::XMMath3& ZShadeSandboxMath::XMMath3::operator += (const float& val)
{
	Add(val);
	return *this;
}
//==============================================================================================================================
ZShadeSandboxMath::XMMath3& ZShadeSandboxMath::XMMath3::operator -= (const float& val)
{
	Subtract(val);
	return *this;
}
//==============================================================================================================================
ZShadeSandboxMath::XMMath3& ZShadeSandboxMath::XMMath3::operator *= (const float& val)
{
	Multiply(val);
	return *this;
}
//==============================================================================================================================
ZShadeSandboxMath::XMMath3& ZShadeSandboxMath::XMMath3::operator /= (const float& val)
{
	Divide(val);
	return *this;
}
//==============================================================================================================================
ZShadeSandboxMath::XMMath3& ZShadeSandboxMath::XMMath3::operator + (const float& val)
{
	Add(val);
	return *this;
}
//==============================================================================================================================
ZShadeSandboxMath::XMMath3& ZShadeSandboxMath::XMMath3::operator - (const float& val)
{
	Subtract(val);
	return *this;
}
//==============================================================================================================================
ZShadeSandboxMath::XMMath3& ZShadeSandboxMath::XMMath3::operator * (const float& val)
{
	Multiply(val);
	return *this;
}
//==============================================================================================================================
ZShadeSandboxMath::XMMath3& ZShadeSandboxMath::XMMath3::operator / (const float& val)
{
	Divide(val);
	return *this;
}
//==============================================================================================================================
ZShadeSandboxMath::XMMath3& ZShadeSandboxMath::XMMath3::operator = (const float& val)
{
	Assign(val);
	return *this;
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath3::Equals(const float& val)
{
	return (x == val && y == val && z == val);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath3::LessThanEquals(const float& val)
{
	return (x <= val && y <= val && z <= val);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath3::GreaterThanEquals(const float& val)
{
	return (x >= val && y >= val && z >= val);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath3::LessThan(const float& val)
{
	return (x < val && y < val && z < val);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath3::GreaterThan(const float& val)
{
	return (x > val && y > val && z > val);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath3::operator <= (const float& val)
{
	return LessThanEquals(val);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath3::operator >= (const float& val)
{
	return GreaterThanEquals(val);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath3::operator < (const float& val)
{
	return LessThan(val);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath3::operator > (const float& val)
{
	return GreaterThan(val);
}
//==============================================================================================================================
bool ZShadeSandboxMath::XMMath3::operator == (const float& val)
{
	return Equals(val);
}
//==============================================================================================================================