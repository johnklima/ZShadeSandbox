#include "ConePrimitive.h"
#include "Vec.h"
using ZShadeSandboxMath::ConePrimitive;
//==============================================================================================================================
//==============================================================================================================================
ConePrimitive::ConePrimitive(XMFLOAT3 point, float height, int a)
: vertex(point),
  height(height)
{
	Set(point, height, a);
}
//==============================================================================================================================
ConePrimitive::ConePrimitive(XMFLOAT4 point, float height, int a)
:   vertex4(point),
	height(height)
{
	Set(point, height, a);
}
//==============================================================================================================================
ConePrimitive::ConePrimitive(XMFLOAT3 point, XMFLOAT3 axis, float angle, float height)
	: vertex(point),
	height(height),
	angle(angle),
	vaxis(axis)
{
	// Compute the sin and cos of the cone
	ssin = sin(angle);
	ccos = cos(angle);
}
//==============================================================================================================================
ConePrimitive::ConePrimitive(XMFLOAT4 point, XMFLOAT4 axis, float angle, float height)
	: vertex4(point),
	height(height),
	angle(angle),
	vaxis4(axis)
{
	// Compute the sin and cos of the cone
	ssin = sin(angle);
	ccos = cos(angle);
}
//==============================================================================================================================
void ConePrimitive::Set(XMFLOAT3 point, float height, int a)
{
	vertex = point;
	this->height = height;

	ZShadeSandboxMath::Vec3<XMFLOAT3> p1(vertex);
	ZShadeSandboxMath::Vec3<XMFLOAT3> va(vaxis);

	// Create the axis
	if (a == 0)		 va.AddVectorData(p1.Vector().x + height, p1.Vector().y, p1.Vector().z);
	else if (a == 1) va.AddVectorData(p1.Vector().x, p1.Vector().y + height, p1.Vector().z);
	else if (a == 2) va.AddVectorData(p1.Vector().x, p1.Vector().y, p1.Vector().z + height);

	// Perpendicular line shooting from the height of the cone
	ZShadeSandboxMath::Vec3<XMFLOAT3> vcross = p1.Cross(va);

	// Make the Axis ray a unit vector
	vcross.Normalize();

	// Angle of the cone is 45 degrees
	angle = PI / 2;

	// Calculate the distance for the radius of the cones circle
	//radius = vcross.Dot( vertex );
	radius = height * tan(angle);

	// Compute the sin and cos of the cone
	ssin = sin(angle);
	ccos = cos(angle);
}
//==============================================================================================================================
void ConePrimitive::Set(XMFLOAT4 point, float height, int a)
{
	vertex4 = point;
	this->height = height;

	ZShadeSandboxMath::Vec4<XMFLOAT4> p1(vertex4);
	ZShadeSandboxMath::Vec4<XMFLOAT4> va(vaxis4);

	// Create the axis
	if (a == 0)		 va.AddVectorData(p1.Vector().x + height, p1.Vector().y, p1.Vector().z, p1.Vector().w);
	else if (a == 1) va.AddVectorData(p1.Vector().x, p1.Vector().y + height, p1.Vector().z, p1.Vector().w);
	else if (a == 2) va.AddVectorData(p1.Vector().x, p1.Vector().y, p1.Vector().z + height, p1.Vector().w);

	// Perpendicular line shooting from the height of the cone
	ZShadeSandboxMath::Vec4<XMFLOAT4> vcross = p1.Cross(va);

	// Make the Axis ray a unit vector
	vcross.Normalize();

	// Angle of the cone is 45 degrees
	angle = PI / 2;

	// Calculate the distance for the radius of the cones circle
	//radius = vcross.Dot( vertex );
	radius = height * tan( angle );

	// Compute the sin and cos of the cone
	ssin = sin( angle );
	ccos = cos( angle );
}
//==============================================================================================================================
void ConePrimitive::Set(XMFLOAT3 point, XMFLOAT3 axis, float angle, float height)
{
	vertex = point;
	this->vaxis = axis;
	this->height = height;
	this->angle = angle;

	// Compute the sin and cos of the cone
	ssin = sin(angle);
	ccos = cos(angle);
}
//==============================================================================================================================
void ConePrimitive::Set(XMFLOAT4 point, XMFLOAT4 axis, float angle, float height)
{
	vertex4 = point;
	this->vaxis4 = axis;
	this->height = height;
	this->angle = angle;

	// Compute the sin and cos of the cone
	ssin = sin(angle);
	ccos = cos(angle);
}
//==============================================================================================================================
XMFLOAT3 ConePrimitive::Vertex() { return vertex; }
//==============================================================================================================================
XMFLOAT3 ConePrimitive::GetAxis() { return vaxis; }
//==============================================================================================================================
XMFLOAT4 ConePrimitive::Vertex4() { return vertex4; }
//==============================================================================================================================
XMFLOAT4 ConePrimitive::GetAxis4() { return vaxis4; }
//==============================================================================================================================
float ConePrimitive::Sin() { return ssin; }
//==============================================================================================================================
float ConePrimitive::Cos() { return ccos; }
//==============================================================================================================================
float ConePrimitive::Radius() { return radius; }
//==============================================================================================================================
float ConePrimitive::Height() { return height; }
//==============================================================================================================================
float ConePrimitive::Angle() { return angle; }
//==============================================================================================================================