//==============================================================================================================================
// Vec.h
//==============================================================================================================================
#ifndef VEC_H
#define VEC_H
//==============================================================================================================================
//==============================================================================================================================

#include "ZMath.h"
#include "XMMath.h"
#include <iostream>
#include <math.h>
using namespace std;

//==============================================================================================================================
//==============================================================================================================================
#define SQR(x)			(x * x)
#define INVALID		   -91403
#define INVALID_SIZE   -91404
//==============================================================================================================================
namespace ZShadeSandboxMath {
// These are provided so the the vector class can have vector data access if need be.
struct VecData2
{
	float x;
	float y;
	VecData2() : x(0), y(0) {}
	VecData2(float x, float y) : x(x), y(y) {}
};
//==============================================================================================================================
struct VecData3
{
	float x;
	float y;
	float z;
	VecData3() : x(0), y(0), z(0) {}
	VecData3(float x, float y, float z) : x(x), y(y), z(z) {}
};
//==============================================================================================================================
struct VecData4
{
	float x;
	float y;
	float z;
	float w;
	VecData4() : x(0), y(0), z(0), w(0) {}
	VecData4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
};
//==============================================================================================================================
/** Must have x, y or z or w in the data type T for the vector */
template <typename T, size_t S>
class Vec
{
protected:

	T __data;
	
	size_t __size;
	
public:
	
	// Constructors that initialize the vector
	Vec() : __size(S) {}
	Vec(float x, float y) : __size(S) { AddVectorData(x, y); }
	Vec(float x, float y, float z) : __size(S) { AddVectorData(x, y, z); }
	Vec(float x, float y, float z, float w) : __size(S) { AddVectorData(x, y, z, w); }
	Vec(T vec) : __size(S), __data(vec) {}
	/*
	Vec(const Vec<T, S>& vec) : __size(vec.Size())
	{
		switch (__size)
		{
			case 2: AddVectorData2D(vec); break;
			case 3: AddVectorData3D(vec); break;
			case 4: AddVectorData4D(vec); break;
		}
	}
	*/
	
	operator T () { return __data; }

	T Vector() const { return __data; }
	
	size_t Size() const { return __size; }
	
	// Adds vector data for a Vec2
	void AddVectorData(float x, float y)
	{
		__data.x = x;
		__data.y = y;
	}
	
	// Adds vector data for a Vec3
	void AddVectorData(float x, float y, float z)
	{
		__data.x = x;
		__data.y = y;
		__data.z = z;
	}
	
	// Adds vector data for a Vec4
	void AddVectorData(float x, float y, float z, float w)
	{
		__data.x = x;
		__data.y = y;
		__data.z = z;
		__data.w = w;
	}
	
	T Max(float x2, float y2)
	{
		T ret;
		
		ret.x = (__data.x > x2) ? __data.x : x2;
		ret.y = (__data.y > y2) ? __data.y : y2;
		
		return ret;
	}
	T Max(float x2, float y2, float z2)
	{
		T ret;
		
		ret.x = (__data.x > x2) ? __data.x : x2;
		ret.y = (__data.y > y2) ? __data.y : y2;
		ret.z = (__data.z > z2) ? __data.z : z2;
		
		return ret;
	}
	T Max(float x2, float y2, float z2, float w2)
	{
		T ret;
		
		ret.x = (__data.x > x2) ? __data.x : x2;
		ret.y = (__data.y > y2) ? __data.y : y2;
		ret.z = (__data.z > z2) ? __data.z : z2;
		ret.w = (__data.w > w2) ? __data.w : w2;
		
		return ret;
	}
	
	T Min(float x2, float y2)
	{
		T ret;
		
		ret.x = (__data.x < x2) ? __data.x : x2;
		ret.y = (__data.y < y2) ? __data.y : y2;
		
		return ret;
	}
	T Min(float x2, float y2, float z2)
	{
		T ret;
		
		ret.x = (__data.x < x2) ? __data.x : x2;
		ret.y = (__data.y < y2) ? __data.y : y2;
		ret.z = (__data.z < z2) ? __data.z : z2;
		
		return ret;
	}
	T Min(float x2, float y2, float z2, float w2)
	{
		T ret;
		
		ret.x = (__data.x < x2) ? __data.x : x2;
		ret.y = (__data.y < y2) ? __data.y : y2;
		ret.z = (__data.z < z2) ? __data.z : z2;
		ret.w = (__data.w < w2) ? __data.w : w2;
		
		return ret;
	}
	
	// Swaps the sign of each component
	float NegateX(bool negateMe = true)
	{
		if (negateMe)
		{
			__data.x = -__data.x;
			return __data.x;
		}
		else
			return -__data.x;
	}
	float NegateY(bool negateMe = true)
	{
		if (negateMe)
		{
			__data.y = -__data.y;
			return __data.y;
		}
		else
			return -__data.y;
	}
	float NegateZ(bool negateMe = true)
	{
		if (negateMe)
		{
			__data.z = -__data.z;
			return __data.z;
		}
		else
			return -__data.z;
	}
	float NegateW(bool negateMe = true)
	{
		if (negateMe)
		{
			__data.w = -__data.w;
			return __data.w;
		}
		else
			return -__data.w;
	}
	
	// Calculate the distance from this vector to specified vector
	float Distance(float x, float y)
	{
		return sqrt( SQR(x - this->__data.x) + SQR(y - this->__data.y) );
	}
	float Distance(float x, float y, float z)
	{
		return sqrt( SQR(x - this->__data.x) + SQR(y - this->__data.y) + SQR(z - this->__data.z) );
	}
	float Distance(float x, float y, float z, float w)
	{
		return sqrt( SQR(x - this->__data.x) + SQR( - this->__data.y) + SQR(z - this->__data.z) + SQR(w - this->__data.w) );
	}
	
	// Calculate the distance squared from this vector to specified vector
	float DistanceSquared(float x, float y)
	{
		return SQR(x - this->__data.x) + SQR(y - this->__data.y);
	}
	float DistanceSquared(float x, float y, float z)
	{
		return SQR(x - this->__data.x) + SQR(y - this->__data.y) + SQR(z - this->__data.z);
	}
	float DistanceSquared(float x, float y, float z, float w)
	{
		return SQR(x - this->__data.x) + SQR( - this->__data.y) + SQR(z - this->__data.z) + SQR(w - this->__data.w);
	}

	// Calculate the dot product of this vector and another vector
	float Dot(float x, float y)
	{
		return this->__data.x * x + this->__data.y * y;
	}
	float Dot(float x, float y, float z)
	{
		return this->__data.x * x + this->__data.y * y + this->__data.z * z;
	}
	float Dot(float x, float y, float z, float w)
	{
		return this->__data.x * x + this->__data.y * y + this->__data.z * z + this->__data.w * w;
	}

	// Calculates the cross product of this vector and another vector, (Cross Product is not well defined for 2D vectors)
	//   Create a plane perpendicular to the vector
	T Cross(float x, float y)
	{
		T ret;
		
		ret.x = this->__data.x * y - this->__data.y * x;
		ret.y = this->__data.x * y - this->__data.y * x;
		
		return ret;
	}
	T Cross(float x, float y, float z)
	{
		T ret;

		ret.x = (this->__data.x * z) - (this->__data.z * y);
		ret.y = (this->__data.z * x) - (this->__data.x * z);
		ret.z = (this->__data.x * y) - (this->__data.y * x);

		return ret;
	}
	T Cross(float x, float y, float z, float w)
	{
		// The result is the same as a 3D Cross Product and the fourth component is Zero

		T f(0, 0, 0, 1);

		T ret;

		ret.x =  this->__data.y * (z * f.w - f.z * w) - this->__data.z * (y * f.w - f.y * w) + this->__data.w * (y * f.z - f.y * z);
		ret.y = -this->__data.x * (z * f.w - f.z * w) + this->__data.z * (x * f.w - f.x * w) - this->__data.w * (x * f.z - f.x * z);
		ret.z =  this->__data.x * (y * f.w - f.y * w) - this->__data.y * (x * f.w - f.x * w) + this->__data.w * (x * f.y - f.x * y);
		ret.w = -this->__data.x * (y * f.z - f.y * z) + this->__data.y * (x * f.z - f.x * z) - this->__data.z * (x * f.y - f.x * y);

		return ret;
	}
	
	// Calculates the magnitude of a given vector
	float Magnitude(float x, float y)
	{
		return sqrt( x*x + y*y );
	}
	float Magnitude(float x, float y, float z)
	{
		return sqrt( x*x + y*y + z*z );
	}
	float Magnitude(float x, float y, float z, float w)
	{
		return sqrt(x*x + y*y + z*z + w*w );
	}
	
	float MagnitudeSquared(float x, float y)
	{
		return x*x + y*y;
	}
	float MagnitudeSquared(float x, float y, float z)
	{
		return x*x + y*y + z*z;
	}
	float MagnitudeSquared(float x, float y, float z, float w)
	{
		return x*x + y*y + z*z + w*w;
	}
	
	// Add a vector to this vector
	void Add(float x, float y)
	{
		this->__data.x += x;
		this->__data.y += y;
	}
	void Add(float x, float y, float z)
	{
		this->__data.x += x;
		this->__data.y += y;
		this->__data.z += z;
	}
	void Add(float x, float y, float z, float w)
	{
		this->__data.x += x;
		this->__data.y += y;
		this->__data.z += z;
		this->__data.w += w;
	}

	// Subtracts a vector from this vector
	void Subtract(float x, float y)
	{
		this->__data.x -= x;
		this->__data.y -= y;
	}
	void Subtract(float x, float y, float z)
	{
		this->__data.x -= x;
		this->__data.y -= y;
		this->__data.z -= z;
	}
	void Subtract(float x, float y, float z, float w)
	{
		this->__data.x -= x;
		this->__data.y -= y;
		this->__data.z -= z;
		this->__data.w -= w;
	}
	
	// Multiplies a vector to this vector
	void Multiply(float x, float y)
	{
		this->__data.x *= x;
		this->__data.y *= y;
	}
	void Multiply(float x, float y, float z)
	{
		this->__data.x *= x;
		this->__data.y *= y;
		this->__data.z *= z;
	}
	void Multiply(float x, float y, float z, float w)
	{
		this->__data.x *= x;
		this->__data.y *= y;
		this->__data.z *= z;
		this->__data.w *= w;
	}
	
	// Divides a vector from this vector
	void Divide(float x, float y)
	{
		this->__data.x /= x;
		this->__data.y /= y;
	}
	void Divide(float x, float y, float z)
	{
		this->__data.x /= x;
		this->__data.y /= y;
		this->__data.z /= z;
	}
	void Divide(float x, float y, float z, float w)
	{
		this->__data.x /= x;
		this->__data.y /= y;
		this->__data.z /= z;
		this->__data.w /= w;
	}
	
	//
	// Print Capability
	//
	
	void Print2()
	{
		cout << "(" << this->__data.x << ", " << this->__data.y << ")" << endl;
	}
	void Print3()
	{
		cout << "(" << this->__data.x << ", " << this->__data.y << ", " << this->__data.z << ")" << endl;
	}
	void Print4()
	{
		cout << "(" << this->__data.x << ", " << this->__data.y << ", " << this->__data.z << ", " << this->__data.w << ")" << endl;
	}
};
//==============================================================================================================================
/** Defines a vector with x and y */
template <typename T>
class Vec2 : public Vec<T, 2>
{
public:

	// Constructors that initialize the vector
	Vec2() : Vec<T, 2>() {}
	Vec2(float x, float y) : Vec<T, 2>(x, y) {}
	Vec2(T vec) : Vec<T, 2>(vec) {}
	Vec2(const Vec<T, 2>& vec) : Vec<T, 2>(vec) {}

	// Adds generic vector
	void AddVectorData(float x, float y)
	{
		Vec<T, 2>::AddVectorData(x, y);
	}
	void AddVectorData(T data)
	{
		Vec<T, 2>::AddVectorData(data.x, data.y);
	}
	void AddVectorData(const Vec2<T>& vec)
	{
		Vec<T, 2>::AddVectorData(vec.Vector().x, vec.Vector().y);
	}
	
	Vec2<T>& Max(float x, float y)
	{
		return Vec<T, 2>::Max(x, y);
	}
	Vec2<T>& Max(const Vec2<T>& vec)
	{
		return Vec<T, 2>::Max(vec.x, vec.y);
	}
	
	Vec2<T>& Min(float x, float y)
	{
		return Vec<T, 2>::Min(x, y);
	}
	Vec2<T>& Min(const Vec2<T>& vec)
	{
		return Vec<T, 2>::Min(vec.x, vec.y);
	}
	
	// Swaps the sign of each component
	// true:  Will negate the actual vector
	// false: Will just return a negated version of the vector
	Vec2<T>& Negate(bool negateMe = true)
	{
		float x = Vec<T, 2>::NegateX(negateMe);
		float y = Vec<T, 2>::NegateY(negateMe);
		
		Vec2<T> v(x, y);
		
		return v;
	}
	
	// Calculate the distance from this vector to specified vector
	float Distance(const T& vec)
	{
		return Vec<T, 2>::Distance(vec.x, vec.y);
	}
	float Distance(const Vec2<T>& vec)
	{
		return Vec<T, 2>::Distance(vec.Vector().x, vec.Vector().y);
	}

	// Calculate the distance squared from this vector to specified vector
	float DistanceSquared(const T& vec)
	{
		return Vec<T, 2>::DistanceSquared(vec.x, vec.y);
	}
	float DistanceSquared(const Vec2<T>& vec)
	{
		return Vec<T, 2>::DistanceSquared(vec.Vector().x, vec.Vector().y);
	}

	// Calculate the dot product of this vector and another vector
	float Dot(const T& vec)
	{
		return Vec<T,2>::Dot(vec.x, vec.y);
	}
	float Dot(const Vec2<T>& vec)
	{
		return Vec<T, 2>::Dot(vec.Vector().x, vec.Vector().y);
	}

	// Calculates the magnitude of a given vector
	float Magnitude(const T& vec)
	{
		return Vec<T, 2>::Magnitude(vec.x, vec.y);
	}
	float Magnitude(const Vec2<T>& vec)
	{
		return Vec<T, 2>::Magnitude(vec.Vector().x, vec.Vector().y);
	}
	// Calculate the magnitude of this vector
	float Magnitude()
	{
		return Vec<T, 2>::Magnitude(__data.x, __data.y);
	}
	
	// Calculates the squared magnitude of a given vector
	float MagnitudeSquared(const T& vec)
	{
		return Vec<T, 2>::MagnitudeSquared(vec.x, vec.y);
	}
	float MagnitudeSquared(const Vec2<T>& vec)
	{
		return Vec<T, 2>::MagnitudeSquared(vec.Vector().x, vec.Vector().y);
	}
	// Calculate the squared magnitude of this vector
	float MagnitudeSquared()
	{
		return Vec<T, 2>::MagnitudeSquared(__data.x, __data.y);
	}

	// Normalize this vector
	void Normalize()
	{
		// Divide each component by the vectors magnitude
		Divide( Magnitude() );
	}

	// Add a vector to this vector
	void Add(const T& vec)
	{
		Vec<T, 2>::Add(vec.x, vec.y);
	}
	void Add(const Vec2<T>& vec)
	{
		Vec<T, 2>::Add(vec.Vector().x, vec.Vector().y);
	}
	void Add(float val)
	{
		Vec<T, 2>::Add(val, val);
	}

	// Subtracts a vector from this vector
	void Subtract(const T& vec)
	{
		Vec<T, 2>::Subtract(vec.x, vec.y);
	}
	void Subtract(const Vec2<T>& vec)
	{
		Vec<T, 2>::Subtract(vec.Vector().x, vec.Vector().y);
	}
	void Subtract(float val)
	{
		Vec<T, 2>::Subtract(val, val);
	}

	// Multiplies a vector to this vector
	void Multiply(const T& vec)
	{
		Vec<T, 2>::Multiply(vec.x, vec.y);
	}
	void Multiply(const Vec2<T>& vec)
	{
		Vec<T, 2>::Multiply(vec.Vector().x, vec.Vector().y);
	}
	void Multiply(float val)
	{
		Vec<T, 2>::Multiply(val, val);
	}

	// Divides a vector from this vector
	void Divide(const T& vec)
	{
		Vec<T, 2>::Divide(vec.x, vec.y);
	}
	void Divide(const Vec2<T>& vec)
	{
		Vec<T, 2>::Divide(vec.Vector().x, vec.Vector().y);
	}
	void Divide(float val)
	{
		Vec<T, 2>::Divide(val, val);
	}

	//
	// Overloaded operators
	//
	
	Vec2<T>& operator + (const T& vec)
	{
		Add( vec );
		return *this;
	}

	Vec2<T>& operator - (const T& vec)
	{
		Subtract( vec );
		return *this;
	}

	Vec2<T>& operator * (const T& vec)
	{
		Multiply( vec );
		return *this;
	}

	Vec2<T>& operator / (const T& vec)
	{
		Divide( vec );
		return *this;
	}

	Vec2<T>& operator + (const Vec2<T>& vec)
	{
		Add( vec );
		return *this;
	}

	Vec2<T>& operator - (const Vec2<T>& vec)
	{
		Subtract( vec );
		return *this;
	}

	Vec2<T>& operator * (const Vec2<T>& vec)
	{
		Multiply( vec );
		return *this;
	}

	Vec2<T>& operator / (const Vec2<T>& vec)
	{
		Divide( vec );
		return *this;
	}

	Vec2<T>& operator += (const T& vec)
	{
		Add( vec );
		return *this;
	}

	Vec2<T>& operator -= (const T& vec)
	{
		Subtract( vec );
		return *this;
	}

	Vec2<T>& operator *= (const T& vec)
	{
		Multiply( vec );
		return *this;
	}

	Vec2<T>& operator /= (const T& vec)
	{
		Divide( vec );
		return *this;
	}

	Vec2<T>& operator = (const T& vec)
	{
		AddVectorData( vec );
		return *this;
	}

	Vec2<T>& operator += (const Vec2<T>& vec)
	{
		Add( vec );
		return *this;
	}

	Vec2<T>& operator -= (const Vec2<T>& vec)
	{
		Subtract( vec );
		return *this;
	}

	Vec2<T>& operator *= (const Vec2<T>& vec)
	{
		Multiply( vec );
		return *this;
	}

	Vec2<T>& operator /= (const Vec2<T>& vec)
	{
		Divide( vec );
		return *this;
	}

	Vec2<T>& operator = (const Vec2<T>& vec)
	{
		AddVectorData(vec);
		return *this;
	}

	Vec2<T>& operator + (const float val)
	{
		Add(val);
		return *this;
	}

	Vec2<T>& operator - (const float val)
	{
		Subtract(val);
		return *this;
	}

	Vec2<T>& operator * (const float val)
	{
		Multiply(val);
		return *this;
	}

	Vec2<T>& operator / (const float val)
	{
		Divide(val);
		return *this;
	}

	Vec2<T>& operator += (const float val)
	{
		Add(val);
		return *this;
	}

	Vec2<T>& operator -= (const float val)
	{
		Subtract(val);
		return *this;
	}

	Vec2<T>& operator *= (const float val)
	{
		Multiply(val);
		return *this;
	}

	Vec2<T>& operator /= (const float val)
	{
		Divide(val);
		return *this;
	}

	bool operator == (const Vec2<T>& val)
	{
		return (this->Vector().x == val.Vector().x &&
			    this->Vector().y == val.Vector().y);
	}

	friend std::ostream& operator << (std::ostream& out, const Vec2<T>& vec)
	{
		return out << "(" << vec.Vector().x << ", " << vec.Vector().y << ")" << endl;
	}

	//
	// Print Capability
	//

	void Print()
	{
		Vec<T, 2>::Print2();
	}
};
//==============================================================================================================================
/** Defines a vector with x, y and z */
template <typename T>
class Vec3 : public Vec<T, 3>
{
public:

	// Constructors that initialize the vector
	Vec3() : Vec<T, 3>() {}
	Vec3(float x, float y, float z) : Vec<T, 3>(x, y, z) {}
	Vec3(T vec) : Vec<T, 3>(vec) {}
	Vec3(const Vec<T, 3>& vec) : Vec<T, 3>(vec) {}

	// Adds generic vector
	void AddVectorData(float x, float y, float z)
	{
		Vec<T, 3>::AddVectorData(x, y, z);
	}
	void AddVectorData(T data)
	{
		Vec<T, 3>::AddVectorData(data.x, data.y, data.z);
	}

	void AddVectorData(const Vec3<T>& vec)
	{
		Vec<T, 3>::AddVectorData(vec.Vector().x, vec.Vector().y, vec.Vector().z);
	}
	
	Vec3<T>& Max(float x, float y, float z)
	{
		return Vec<T, 3>::Max(x, y, z);
	}
	Vec3<T>& Max(const Vec3<T>& vec)
	{
		return Vec<T, 3>::Max(vec.x, vec.y, vec.z);
	}
	
	Vec3<T>& Min(float x, float y, float z)
	{
		return Vec<T, 3>::Min(x, y, z);
	}
	Vec3<T>& Min(const Vec3<T>& vec)
	{
		return Vec<T, 3>::Min(vec.x, vec.y, vec.z);
	}
	
	// Swaps the sign of each component
	// true:  Will negate the actual vector
	// false: Will just return a negated version of the vector
	Vec3<T>& Negate(bool negateMe = true)
	{
		float x = Vec<T, 3>::NegateX(negateMe);
		float y = Vec<T, 3>::NegateY(negateMe);
		float z = Vec<T, 3>::NegateZ(negateMe);
		
		Vec3<T> v(x, y, z);
		
		return v;
	}
	
	// Calculate the distance from this vector to specified vector
	float Distance(const T& vec)
	{
		return Vec<T, 3>::Distance(vec.x, vec.y, vec.z);
	}
	float Distance(const Vec3<T>& vec)
	{
		return Vec<T, 3>::Distance(vec.Vector().x, vec.Vector().y, vec.Vector().z);
	}

	// Calculate the distance squared from this vector to specified vector
	float DistanceSquared(const T& vec)
	{
		return Vec<T, 3>::DistanceSquared(vec.x, vec.y, vec.z);
	}
	float DistanceSquared(const Vec3<T>& vec)
	{
		return Vec<T, 3>::DistanceSquared(vec.Vector().x, vec.Vector().y, vec.Vector().z);
	}

	// Calculate the dot product of this vector and another vector
	float Dot(const T& vec)
	{
		return Vec<T, 3>::Dot(vec.x, vec.y, vec.z);
	}
	float Dot(const Vec3<T>& vec)
	{
		return Vec<T, 3>::Dot(vec.Vector().x, vec.Vector().y, vec.Vector().z);
	}

	// Calculates the cross product of this vector and another vector, (Cross Product does not exist for 2D vectors)
	//   Create a plane perpendicular to the vector
	T Cross(const T& vec)
	{
		return Vec<T, 3>::Cross(vec.x, vec.y, vec.z);
	}
	T Cross(const Vec3<T>& vec)
	{
		return Vec<T, 3>::Cross(vec.Vector().x, vec.Vector().y, vec.Vector().z);
	}
	// Triple Cross Product
	T Cross(Vec3<T> v2, Vec3<T> v3)
	{
		//v1 x (v2 x v3) = (v1.x*v3.x + v1.y*v3.y + v1.z*v3.z)(v1.x*i + v1.y*j + v1.z*k) -
		//                 (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z)(v3.x*i + v3.y*j + v3.z*k) = (v1*v3) v1 - (v1 * v2) v3;
		
		T ret;
		
		T c = v2.Cross( v3 );
		
		Vec3<T> nc(c.x, c.y, c.z);
		
		ret = this->Cross( nc );
		
		return ret;
	}
	
	//Compute the dot product AB * BC
	float IDot(Vec3<T> v2, Vec3<T> v3)
	{
		Vec3<T> AB(v2.Vector().x - Vec<T, 3>::Vector().x, v2.Vector().y - Vec<T, 3>::Vector().y, v2.Vector().z - Vec<T, 3>::Vector().z);
		Vec3<T> BC(v3.Vector().x - v2.Vector().x, v3.Vector().y - v2.Vector().y, v3.Vector().z - v2.Vector().z);
		
		return AB.Dot( BC );
	}
	
	//Compute the cross product AB x AC
	int ICross(Vec3<T> v2, Vec3<T> v3)
	{
		Vec3<T> AB(v2.Vector().x - Vec<T, 3>::Vector().x, v2.Vector().y - Vec<T, 3>::Vector().y, v2.Vector().z - Vec<T, 3>::Vector().z);
		Vec3<T> AC(v3.Vector().x - Vec<T, 3>::Vector().x, v3.Vector().y - Vec<T, 3>::Vector().y, v3.Vector().z - Vec<T, 3>::Vector().z);
		
		return AB.Vector().x * AC.Vector().y - AB.Vector().y * AC.Vector().x;
	}
	
	// Calculates the magnitude of a given vector
	float Magnitude(const T& vec)
	{
		return Vec<T, 3>::Magnitude(vec.x, vec.y, vec.z);
	}
	float Magnitude(const Vec3<T>& vec)
	{
		return Vec<T, 3>::Magnitude(vec.Vector().x, vec.Vector().y, vec.Vector().z);
	}
	// Calculate the magnitude of this vector
	float Magnitude()
	{
		return Vec<T, 3>::Magnitude(__data.x, __data.y, __data.z);
	}
	
	// Calculates the squared magnitude of a given vector
	float MagnitudeSquared(const T& vec)
	{
		return Vec<T, 3>::MagnitudeSquared(vec.x, vec.y, vec.z);
	}
	float MagnitudeSquared(const Vec3<T>& vec)
	{
		return Vec<T, 3>::MagnitudeSquared(vec.Vector().x, vec.Vector().y, vec.Vector().z);
	}
	// Calculate the squared magnitude of this vector
	float MagnitudeSquared()
	{
		return Vec<T, 3>::MagnitudeSquared(__data.x, __data.y, __data.z);
	}

	// Normalize this vector
	void Normalize()
	{
		// Divide each component by the vectors magnitude
		Divide( Magnitude() );
	}

	// Add a vector to this vector
	void Add(const T& vec)
	{
		Vec<T, 3>::Add(vec.x, vec.y, vec.z);
	}
	void Add(const Vec3<T>& vec)
	{
		Vec<T, 3>::Add(vec.Vector().x, vec.Vector().y, vec.Vector().z);
	}
	void Add(float val)
	{
		Vec<T, 3>::Add(val, val, val);
	}

	// Subtracts a vector from this vector
	void Subtract(const T& vec)
	{
		Vec<T, 3>::Subtract(vec.x, vec.y, vec.z);
	}
	void Subtract(const Vec3<T>& vec)
	{
		Vec<T, 3>::Subtract(vec.Vector().x, vec.Vector().y, vec.Vector().z);
	}
	void Subtract(float val)
	{
		Vec<T, 3>::Subtract(val, val, val);
	}

	// Multiplies a vector to this vector
	void Multiply(const T& vec)
	{
		Vec<T, 3>::Multiply(vec.x, vec.y, vec.z);
	}
	void Multiply(const Vec3<T>& vec)
	{
		Vec<T, 3>::Multiply(vec.Vector().x, vec.Vector().y, vec.Vector().z);
	}
	void Multiply(float val)
	{
		Vec<T, 3>::Multiply(val, val, val);
	}

	// Divides a vector from this vector
	void Divide(const T& vec)
	{
		Vec<T, 3>::Divide(vec.x, vec.y, vec.z);
	}
	void Divide(const Vec3<T>& vec)
	{
		Vec<T, 3>::Divide(vec.Vector().x, vec.Vector().y, vec.Vector().z);
	}
	void Divide(float val)
	{
		Vec<T, 3>::Divide(val, val, val);
	}

	//
	// Overloaded operators
	//

	Vec3<T>& operator + (const T& vec)
	{
		Add(vec);
		return *this;
	}

	Vec3<T>& operator - (const T& vec)
	{
		Subtract(vec);
		return *this;
	}

	Vec3<T>& operator * (const T& vec)
	{
		Multiply(vec);
		return *this;
	}

	Vec3<T>& operator / (const T& vec)
	{
		Divide(vec);
		return *this;
	}

	Vec3<T>& operator + (const Vec3<T>& vec)
	{
		Add(vec);
		return *this;
	}

	Vec3<T>& operator - (const Vec3<T>& vec)
	{
		Subtract(vec);
		return *this;
	}

	Vec3<T>& operator * (const Vec3<T>& vec)
	{
		Multiply(vec);
		return *this;
	}

	Vec3<T>& operator / (const Vec3<T>& vec)
	{
		Divide(vec);
		return *this;
	}

	Vec3<T>& operator += (const T& vec)
	{
		Add(vec);
		return *this;
	}

	Vec3<T>& operator -= (const T& vec)
	{
		Subtract(vec);
		return *this;
	}

	Vec3<T>& operator *= (const T& vec)
	{
		Multiply(vec);
		return *this;
	}

	Vec3<T>& operator /= (const T& vec)
	{
		Divide(vec);
		return *this;
	}

	Vec3<T>& operator = (const T& vec)
	{
		AddVectorData(vec);
		return *this;
	}

	Vec3<T>& operator += (const Vec3<T>& vec)
	{
		Add(vec);
		return *this;
	}

	Vec3<T>& operator -= (const Vec3<T>& vec)
	{
		Subtract(vec);
		return *this;
	}

	Vec3<T>& operator *= (const Vec3<T>& vec)
	{
		Multiply(vec);
		return *this;
	}

	Vec3<T>& operator /= (const Vec3<T>& vec)
	{
		Divide(vec);
		return *this;
	}

	Vec3<T>& operator = (const Vec3<T>& vec)
	{
		AddVectorData(vec);
		return *this;
	}

	bool operator == (const Vec3<T>& val)
	{
		return (this->Vector().x == val.Vector().x &&
			    this->Vector().y == val.Vector().y &&
			    this->Vector().z == val.Vector().z);
	}

	Vec3<T>& operator + (const float val)
	{
		Add(val);
		return *this;
	}

	Vec3<T>& operator - (const float val)
	{
		Subtract(val);
		return *this;
	}

	Vec3<T>& operator * (const float val)
	{
		Multiply(val);
		return *this;
	}

	Vec3<T>& operator / (const float val)
	{
		Divide(val);
		return *this;
	}

	Vec3<T>& operator += (const float val)
	{
		Add(val);
		return *this;
	}

	Vec3<T>& operator -= (const float val)
	{
		Subtract(val);
		return *this;
	}

	Vec3<T>& operator *= (const float val)
	{
		Multiply(val);
		return *this;
	}

	Vec3<T>& operator /= (const float val)
	{
		Divide(val);
		return *this;
	}

	friend std::ostream& operator << (std::ostream& out, const Vec3<T>& vec)
	{
		return out << "(" << vec.Vector().x << ", " << vec.Vector().y << ", " << vec.Vector().z << ")" << endl;
	}

	//
	// Print Capability
	//

	void Print()
	{
		Vec<T, 3>::Print3();
	}
};
//==============================================================================================================================
/** Defines a vector with x, y, z and w */
template <typename T>
class Vec4 : public Vec<T, 4>
{
public:

	// Constructors that initialize the vector
	Vec4() : Vec<T, 4>() {}
	Vec4(float x, float y, float z, float w) : Vec<T, 4>(x, y, z, w) {}
	Vec4(T vec) : Vec<T, 4>(vec) {}
	Vec4(const Vec<T, 4>& vec) : Vec<T, 4>(vec) {}

	// Adds generic vector
	void AddVectorData(float x, float y, float z, float w)
	{
		Vec<T, 4>::AddVectorData(x, y, z, w);
	}
	void AddVectorData(T data)
	{
		Vec<T, 4>::AddVectorData(data.x, data.y, data.z, data.w);
	}

	void AddVectorData(const Vec4<T>& vec)
	{
		Vec<T, 4>::AddVectorData(vec.Vector().x, vec.Vector().y, vec.Vector().z, vec.Vector().w);
	}
	
	Vec4<T>& Max(float x, float y, float z, float w)
	{
		return Vec<T, 4>::Max(x, y, z, w);
	}
	Vec4<T>& Max(const Vec4<T>& vec)
	{
		return Vec<T, 4>::Max(vec.x, vec.y, vec.z, vec.w);
	}
	
	Vec4<T>& Min(float x, float y, float z, float w)
	{
		return Vec<T, 4>::Min(x, y, z, w);
	}
	Vec4<T>& Min(const Vec4<T>& vec)
	{
		return Vec<T, 4>::Min(vec.x, vec.y, vec.z, vec.w);
	}
	
	// Swaps the sign of each component
	// true:  Will negate the actual vector
	// false: Will just return a negated version of the vector
	Vec4<T>& Negate(bool negateMe = true)
	{
		float x = Vec<T, 4>::NegateX(negateMe);
		float y = Vec<T, 4>::NegateY(negateMe);
		float z = Vec<T, 4>::NegateZ(negateMe);
		float w = Vec<T, 4>::NegateW(negateMe);
		
		Vec4<T> v(x, y, z, w);
		
		return v;
	}
	
	// Calculate the distance from this vector to specified vector
	float Distance(const T& vec)
	{
		return Vec<T, 4>::Distance(vec.x, vec.y, vec.z, vec.w);
	}
	float Distance(const Vec4<T>& vec)
	{
		return Vec<T, 4>::Distance(vec.Vector().x, vec.Vector().y, vec.Vector().z, vec.Vector().w);
	}

	// Calculate the distance squared from this vector to specified vector
	float DistanceSquared(const T& vec)
	{
		return Vec<T, 4>::DistanceSquared(vec.x, vec.y, vec.z, vec.w);
	}
	float DistanceSquared(const Vec4<T>& vec)
	{
		return Vec<T, 4>::DistanceSquared(vec.Vector().x, vec.Vector().y, vec.Vector().z, vec.Vector().w);
	}

	// Calculate the dot product of this vector and another vector
	float Dot(const T& vec)
	{
		return Vec<T, 4>::Dot(vec.x, vec.y, vec.z, vec.w);
	}
	float Dot(const Vec4<T>& vec)
	{
		return Vec<T, 4>::Dot(vec.Vector().x, vec.Vector().y, vec.Vector().z, vec.Vector().w);
	}

	// Calculates the cross product of this vector and another vector, (Cross Product does not exist for 2D vectors)
	//   Create a plane perpendicular to the vector
	T Cross(const T& vec)
	{
		return Vec<T, 4>::Cross(vec.x, vec.y, vec.z, vec.w);
	}
	T Cross(const Vec4<T>& vec)
	{
		return Vec<T, 4>::Cross(vec.Vector().x, vec.Vector().y, vec.Vector().z, vec.Vector().w);
	}
	// Triple Cross Product
	T Cross(Vec4<T> v2, Vec4<T> v3)
	{
		//v1 x (v2 x v3) = (v1.x*v3.x + v1.y*v3.y + v1.z*v3.z)(v1.x*i + v1.y*j + v1.z*k) -
		//                 (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z)(v3.x*i + v3.y*j + v3.z*k) = (v1*v3) v1 - (v1 * v2) v3;
		
		T ret;
		
		T c = v2.Cross( v3 );
		
		Vec4<T> nc(c.x, c.y, c.z, c.w);
		
		ret = this->Cross( nc );
		
		return ret;
	}

	// Calculates the magnitude of a given vector
	float Magnitude(const T& vec)
	{
		return Vec<T, 4>::Magnitude(vec.x, vec.y, vec.z, vec.w);
	}
	float Magnitude(const Vec4<T>& vec)
	{
		return Vec<T, 4>::Magnitude(vec.Vector().x, vec.Vector().y, vec.Vector().z, vec.Vector().w);
	}
	// Calculate the magnitude of this vector
	float Magnitude()
	{
		return Vec<T, 4>::Magnitude(__data.x, __data.y, __data.z, __data.w);
	}
	
	// Calculates the squared magnitude of a given vector
	float MagnitudeSquared(const T& vec)
	{
		return Vec<T, 4>::MagnitudeSquared(vec.x, vec.y, vec.z, vec.w);
	}
	float MagnitudeSquared(const Vec4<T>& vec)
	{
		return Vec<T, 4>::MagnitudeSquared(vec.Vector().x, vec.Vector().y, vec.Vector().z, vec.Vector().w);
	}
	// Calculate the squared magnitude of this vector
	float MagnitudeSquared()
	{
		return Vec<T, 4>::MagnitudeSquared(__data.x, __data.y, __data.z, __data.w);
	}

	// Normalize this vector
	void Normalize()
	{
		// Divide each component by the vectors magnitude
		Divide( Magnitude() );
	}

	// Add a vector to this vector
	void Add(const T& vec)
	{
		Vec<T, 4>::Add(vec.x, vec.y, vec.z, vec.w);
	}
	void Add(const Vec4<T>& vec)
	{
		Vec<T, 4>::Add(vec.Vector().x, vec.Vector().y, vec.Vector().z, vec.Vector().w);
	}
	void Add(float val)
	{
		Vec<T, 4>::Add(val, val, val, val);
	}

	// Subtracts a vector from this vector
	void Subtract(const T& vec)
	{
		Vec<T, 4>::Subtract(vec.x, vec.y, vec.z, vec.w);
	}
	void Subtract(const Vec4<T>& vec)
	{
		Vec<T, 4>::Subtract(vec.Vector().x, vec.Vector().y, vec.Vector().z, vec.Vector().w);
	}
	void Subtract(float val)
	{
		Vec<T, 4>::Subtract(val, val, val, val);
	}

	// Multiplies a vector to this vector
	void Multiply(const T& vec)
	{
		Vec<T, 4>::Multiply(vec.x, vec.y, vec.z, vec.w);
	}
	void Multiply(const Vec4<T>& vec)
	{
		Vec<T, 4>::Multiply(vec.Vector().x, vec.Vector().y, vec.Vector().z, vec.Vector().w);
	}
	void Multiply(float val)
	{
		Vec<T, 4>::Multiply(val, val, val, val);
	}

	// Divides a vector from this vector
	void Divide(const T& vec)
	{
		Vec<T, 4>::Divide(vec.x, vec.y, vec.z, vec.w);
	}
	void Divide(const Vec4<T>& vec)
	{
		Vec<T, 4>::Divide(vec.Vector().x, vec.Vector().y, vec.Vector().z, vec.Vector().w);
	}
	void Divide(float val)
	{
		Vec<T, 4>::Divide(val, val, val, val);
	}

	//
	// Overloaded operators
	//

	Vec4<T>& operator + (const T& vec)
	{
		Add(vec);
		return *this;
	}

	Vec4<T>& operator - (const T& vec)
	{
		Subtract(vec);
		return *this;
	}

	Vec4<T>& operator * (const T& vec)
	{
		Multiply(vec);
		return *this;
	}

	Vec4<T>& operator / (const T& vec)
	{
		Divide(vec);
		return *this;
	}

	Vec4<T>& operator + (const Vec4<T>& vec)
	{
		Add(vec);
		return *this;
	}

	Vec4<T>& operator - (const Vec4<T>& vec)
	{
		Subtract(vec);
		return *this;
	}

	Vec4<T>& operator * (const Vec4<T>& vec)
	{
		Multiply(vec);
		return *this;
	}

	Vec4<T>& operator / (const Vec4<T>& vec)
	{
		Divide(vec);
		return *this;
	}

	Vec4<T>& operator += (const T& vec)
	{
		Add(vec);
		return *this;
	}

	Vec4<T>& operator -= (const T& vec)
	{
		Subtract(vec);
		return *this;
	}

	Vec4<T>& operator *= (const T& vec)
	{
		Multiply(vec);
		return *this;
	}

	Vec4<T>& operator /= (const T& vec)
	{
		Divide(vec);
		return *this;
	}

	Vec4<T>& operator = (const T& vec)
	{
		AddVectorData(vec);
		return *this;
	}

	Vec4<T>& operator += (const Vec4<T>& vec)
	{
		Add(vec);
		return *this;
	}

	Vec4<T>& operator -= (const Vec4<T>& vec)
	{
		Subtract(vec);
		return *this;
	}

	Vec4<T>& operator *= (const Vec4<T>& vec)
	{
		Multiply(vec);
		return *this;
	}

	Vec4<T>& operator /= (const Vec4<T>& vec)
	{
		Divide(vec);
		return *this;
	}

	Vec4<T>& operator = (const Vec4<T>& vec)
	{
		AddVectorData(vec);
		return *this;
	}

	Vec4<T>& operator + (const float val)
	{
		Add(val);
		return *this;
	}

	Vec4<T>& operator - (const float val)
	{
		Subtract(val);
		return *this;
	}

	Vec4<T>& operator * (const float val)
	{
		Multiply(val);
		return *this;
	}

	Vec4<T>& operator / (const float val)
	{
		Divide(val);
		return *this;
	}

	Vec4<T>& operator += (const float val)
	{
		Add(val);
		return *this;
	}

	Vec4<T>& operator -= (const float val)
	{
		Subtract(val);
		return *this;
	}

	Vec4<T>& operator *= (const float val)
	{
		Multiply(val);
		return *this;
	}

	Vec4<T>& operator /= (const float val)
	{
		Divide(val);
		return *this;
	}

	bool operator == (const Vec3<T>& val)
	{
		return (this->Vector().x == val.Vector().x &&
				this->Vector().y == val.Vector().y &&
				this->Vector().z == val.Vector().z &&
				this->Vector().w == val.Vector().w);
	}

	friend std::ostream& operator << (std::ostream& out, const Vec4<T>& vec)
	{
		return out << "(" << vec.Vector().x << ", " << vec.Vector().y << ", " << vec.Vector().z << ", " << vec.Vector().w << ")" << endl;
	}

	//
	// Print Capability
	//

	void Print()
	{
		Vec<T, 4>::Print4();
	}
};
}
//==============================================================================================================================
//==============================================================================================================================
#endif//VEC_H