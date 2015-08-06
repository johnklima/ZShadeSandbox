//==============================================================================================================================
// ConePrimitive.h
//==============================================================================================================================
#ifndef CONEPRIMITIVE_H
#define CONEPRIMITIVE_H
//==============================================================================================================================
//==============================================================================================================================

//
// Includes
//

#include "ZMath.h"
#include "XMMath.h"

//==============================================================================================================================
//==============================================================================================================================
namespace ZShadeSandboxMath {
class ConePrimitive
{
	// Initial vertex V of the cone
	XMFLOAT3 vertex;
	XMFLOAT4 vertex4;

	// Axis ray with origin at vertex and unit-length direction A
	XMFLOAT3 vaxis;
	XMFLOAT4 vaxis4;

	float radius;
	float height;
	float angle;

	float ssin;
	float ccos;

public:
	
	ConePrimitive() {}
	ConePrimitive(XMFLOAT3 point, float height = 100, int a = 0);
	ConePrimitive(XMFLOAT4 point, float height = 100, int a = 0);
	ConePrimitive(XMFLOAT3 point, XMFLOAT3 axis, float angle, float height);
	ConePrimitive(XMFLOAT4 point, XMFLOAT4 axis, float angle, float height);
	
	void Set(XMFLOAT3 point = XMFLOAT3(0, 0, 0), float height = 100, int a = 0);
	void Set(XMFLOAT4 point = XMFLOAT4(0, 0, 0, 0), float height = 100, int a = 0);
	void Set(XMFLOAT3 point, XMFLOAT3 axis, float angle, float height);
	void Set(XMFLOAT4 point, XMFLOAT4 axis, float angle, float height);

	XMFLOAT3 Vertex();
	XMFLOAT3 GetAxis();

	XMFLOAT4 Vertex4();
	XMFLOAT4 GetAxis4();

	float Sin();
	float Cos();

	float Radius();
	float Height();
	float Angle();
};
}
//==============================================================================================================================
//==============================================================================================================================
#endif// CONEPRIMITIVE_H