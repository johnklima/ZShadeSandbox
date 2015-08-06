//==============================================================================================================================
// ZMath.h Helper Math class
//
// Part of the code credited to Frank Luna (C) 2011 All Rights Reserved.
//==============================================================================================================================
#ifndef ZMATH_H
#define ZMATH_H
//==============================================================================================================================
//==============================================================================================================================
#define _XM_NO_INTRINSICS_
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
using namespace DirectX;
using namespace DirectX::PackedVector;

#include <Windows.h>
#include <ctime>
#include <cstdlib>
#include "CGlobal.h"
#include "LinePrimitive.h"
#include "TrianglePrimitive.h"
#include "SpherePrimitive.h"
#include "PlanePrimitive.h"
#include "BoxPrimitive.h"
#include "ConePrimitive.h"
#include "CirclePrimitive.h"
#include "Vec.h"

//==============================================================================================================================
// Collision intersection attributes for Math primitives
#define INTERSECT 10045
#define OUT 10046
#define IN 10047

#define EPSILON	0.00001;

#define SQR(x) ( x * x )

#define HALF_SQRT_2	0.7071068f
#define GRAV_ACCEL	981.0f	// The acceleration of gravity, cm/s^2

#define BLOCK_SIZE_X 16
#define BLOCK_SIZE_Y 16

#define TWO_PI 6.283185307179586476925286766559
#define PI     3.141592653589793

#define FFT_DIMENSIONS 3U
#define FFT_PLAN_SIZE_LIMIT (1U << 27)

#define FFT_FORWARD -1
#define FFT_INVERSE 1

namespace ZShadeSandboxMath {
	class XMMath3;
}

namespace ZShadeSandboxMath {

const float Infinity = FLT_MAX;

namespace EAxis
{
	enum Type
	{
		Axis_X,
		Axis_Y,
		Axis_Z
	};
}

// add support for logical XOR 
#define log_xor || log_xor_helper() ||
struct log_xor_helper {
	bool value;
};
template<typename LEFT>
log_xor_helper &operator ||(const LEFT &left, log_xor_helper &xor) {
	xor.value = (bool)left;
	return xor;
}
template<typename RIGHT>
bool operator ||(const log_xor_helper &xor, const RIGHT &right) {
	return xor.value ^ (bool)right;
}

class ZMath
{
public:
	
	// This is used with the BSP
	static bool GetIntersect
	(	ZShadeSandboxMath::XMMath3 linestart
	,	ZShadeSandboxMath::XMMath3 lineend
	,	ZShadeSandboxMath::XMMath3 vertex
	,	ZShadeSandboxMath::XMMath3 normal
	,	ZShadeSandboxMath::XMMath3& intersection
	,	float& percentage
	);

	static const XMFLOAT3 vec3Epsilon;
	static const XMFLOAT2 vec2Epsilon;

	static const XMFLOAT3 vec3True;
	static const XMFLOAT2 vec2True;

	static bool CompareVector2WithEpsilon(const XMFLOAT2& lhs, const XMFLOAT2& rhs);
	static bool CompareVector3WithEpsilon(const XMFLOAT3& lhs, const XMFLOAT3& rhs);

	static bool IsPowerOf2(int x)
	{
		return x > 0 && (x << 1 == (x | (x - 1)) + 1);
	}
	
	// Grabs a random seed value based on current time
	static void RandomSeed()
	{
		unsigned rseed = unsigned(time(NULL));
		srand(rseed);
	}
	
	static float Abs(float x)
	{
		return (float)abs(x);
	}

	static float Fabs(float x)
	{
		return (float)fabs(x);
	}

	static float lerp(float a, float b, float t=0.05f)
	{
		return (1 - t) * a + t * b;
	}

	static double DoRound(double d)
	{
		return floor(d + 0.5);
	}

	static XMFLOAT4X4 GMathMF(XMMATRIX& m)
	{
		XMFLOAT4X4 mat;
		XMStoreFloat4x4(&mat, m);
		return mat;
	}

	static XMMATRIX GMathFM(XMFLOAT4X4& m)
	{
		return XMLoadFloat4x4(&m);
	}

	static XMVECTOR GMathFV(XMFLOAT3& val)
	{
		return XMLoadFloat3(&val);
	}

	static XMFLOAT3 GMathVF(XMVECTOR& vec)
	{
		XMFLOAT3 val;
		XMStoreFloat3(&val, vec);
		return val;
	}

	static float ToRadians(float degrees)
	{
		return (degrees * PI) / 180;
	}

	static float ToDegrees(float radians)
	{
		return (radians * 180) / PI;
	}
	
	// http://stackoverflow.com/questions/17333/most-effective-way-for-float-and-double-comparison
	
	static bool AreSame(float a, float b)
	{
		return fabs(a - b) < EPSILON;
	}
	
	static bool AreSame(double a, double b)
	{
		return fabs(a - b) < EPSILON;
	}
	
	// Usable AlmostEqual function    
	static bool AlmostEqual2sComplement(float A, float B)    
	{
		int maxUlps = 4;
		// Make sure maxUlps is non-negative and small enough that the    
		// default NAN won't compare as equal to anything.    
		assert(maxUlps > 0 && maxUlps < 4 * 1024 * 1024);    
		int aInt = *(int*)&A;    
		// Make aInt lexicographically ordered as a twos-complement int    
		if (aInt < 0)    
			aInt = 0x80000000 - aInt;    
		// Make bInt lexicographically ordered as a twos-complement int    
		int bInt = *(int*)&B;    
		if (bInt < 0)    
			bInt = 0x80000000 - bInt;    
		int intDiff = abs(aInt - bInt);    
		if (intDiff <= maxUlps)    
			return true;    
		return false;    
	}
	
	static XMMATRIX FromRowVectors(XMFLOAT3 m0, XMFLOAT3 m1, XMFLOAT3 m2, XMFLOAT3 m3);
	static XMMATRIX MatrixFromNormal(XMFLOAT3 normal);

	/// testing whether two doubles are almost equal. We consider two doubles
	/// equal if the difference is within the range [0, epsilon).
	///
	/// epsilon: a positive number (supposed to be small)
	///
	/// if either x or y is 0, then we are comparing the absolute difference to
	/// epsilon.
	/// if both x and y are non-zero, then we are comparing the relative difference
	/// to epsilon.
	static bool almost_equal(double x, double y, double epsilon)
	{
		double diff = x - y;
		if (x != 0 && y != 0){
			diff = diff/y; 
		}

		if (diff < epsilon && -1.0*diff < epsilon){
			return true;
		}
		return false;
	}
	
	// Returns random T in [0, 1).
	template<typename T>
	static T RandF()
	{
		return (T)(rand()) / (T)RAND_MAX;
	}

	// Returns random T in [a, b).
	template<typename T>
	static T RandF(T a, T b)
	{
		return a + ((rand()) / (T)RAND_MAX) * (b - a);
	}

	template<typename T>
	static T Min(const T& a, const T& b)
	{
		return a < b ? a : b;
	}

	template<typename T>
	static T Max(const T& a, const T& b)
	{
		return a > b ? a : b;
	}

	template<typename T>
	static T Lerp(const T& a, const T& b, float t)
	{
		return a + (b - a) * t;
	}

	template<typename T>
	static T Clamp(const T& x, const T& low, const T& high)
	{
		return x < low ? low : (x > high ? high : x);
	}

	static void Max(XMFLOAT3 left, XMFLOAT3 right, XMFLOAT3& result)
	{
		result.x = (left.x > right.x) ? left.x : right.x;
		result.y = (left.y > right.y) ? left.y : right.y;
		result.z = (left.z > right.z) ? left.z : right.z;
	}

	static void Min(XMFLOAT3 left, XMFLOAT3 right, XMFLOAT3& result)
	{
		result.x = (left.x < right.x) ? left.x : right.x;
		result.y = (left.y < right.y) ? left.y : right.y;
		result.z = (left.z < right.z) ? left.z : right.z;
	}

	static void Mid(XMFLOAT3 p1, XMFLOAT3 p2, XMFLOAT3& result)
	{
		result.x = (p1.x + p2.x) / 2;
		result.y = (p1.y + p2.y) / 2;
		result.z = (p1.z + p2.z) / 2;
	}

	static void Mid(XMFLOAT4 p1, XMFLOAT4 p2, XMFLOAT4& result)
	{
		result.x = (p1.x + p2.x) / 2;
		result.y = (p1.y + p2.y) / 2;
		result.z = (p1.z + p2.z) / 2;
		result.w = (p1.w + p2.w) / 2;
	}

	/*static void Mid(Vec3<XMFLOAT3> p1, Vec3<XMFLOAT3> p2, Vec3<XMFLOAT3>& result)
	{
		result.AddVectorData(
			(p1.Vector().x + p2.Vector().x) / 2,
			(p1.Vector().y + p2.Vector().y) / 2, 
			(p1.Vector().z + p2.Vector().z) / 2);
	}

	static void Mid(Vec4<XMFLOAT4> p1, Vec4<XMFLOAT4> p2, Vec4<XMFLOAT4>& result)
	{
		result.AddVectorData(
			(p1.Vector().x + p2.Vector().x) / 2,
			(p1.Vector().y + p2.Vector().y) / 2,
			(p1.Vector().z + p2.Vector().z) / 2,
			(p1.Vector().w + p2.Vector().w) / 2);
	}*/

	static float Distance(XMFLOAT3 value1, XMFLOAT3 value2)
	{
		float x = value1.x - value2.x;
		float y = value1.y - value2.y;
		float z = value1.z - value2.z;

		return (float)sqrt((x * x) + (y * y) + (z * z));
	}

	/*
	int TessFactor(XMFLOAT3 eye, BoundingBox bounds, float maxDist, float minDist, float maxTess, float minTess)
	{
	XMFLOAT3 c = (bounds._bmax - bounds._bmin) / 2 + bounds._bmin;

	float d = Distance(eye, c);

	float s = Clamp<float>(-(d - maxDist) / (maxDist - minDist), 0, 1);

	s = 1.0f - s;

	int p = (int)((minTess + (maxTess - 1 - minTess)) * s);
	int t = (int)pow(2, p);
	return t;
	//return 0;
	}
	*/

	static XMFLOAT3 SubFloat3(XMFLOAT3 one, XMFLOAT3 two)
	{
		XMFLOAT3 ret;

		ret.x = one.x - two.x;
		ret.y = one.y - two.y;
		ret.z = one.z - two.z;

		return ret;
	}

	// Returns the polar angle of the point (x,y) in [0, 2*PI).
	static float AngleFromXY(float x, float y);

	static XMMATRIX InverseTransVertexe(CXMMATRIX M)
	{
		// Inverse-transpose is just applied to normals.  So zero out 
		// translation row so that it doesn't get into our inverse-transVertexe
		// calculation--we don't want the inverse-transVertexe of the translation.
		XMMATRIX A = M;
		A.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

		XMVECTOR det = XMMatrixDeterminant(A);
		return XMMatrixTranspose(XMMatrixInverse(&det, A));
	}

	static XMVECTOR RandUnitVec3();
	static XMVECTOR RandHemisphereUnitVec3(XMVECTOR n);
};

}
//==============================================================================================================================
//==============================================================================================================================
#endif // ZMATH_H