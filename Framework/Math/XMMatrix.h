//==============================================================================================================================
// XMMatrix.h
//==============================================================================================================================
#ifndef XMMATRIX_H
#define XMMATRIX_H
//==============================================================================================================================
//==============================================================================================================================

#include "ZMath.h"

namespace ZShadeSandboxMath {
	class XMMath3;
}

//==============================================================================================================================
//==============================================================================================================================
namespace ZShadeSandboxMath {
class XMMatrix
{
	XMMATRIX __matrix;
	
public:
	
	float _11;
	float _12;
	float _13;
	float _14;
	float _21;
	float _22;
	float _23;
	float _24;
	float _31;
	float _32;
	float _33;
	float _34;
	float _41;
	float _42;
	float _43;
	float _44;
	
	XMMatrix();
	XMMatrix(float _11, float _12, float _13, float _14, float _21, float _22, float _23, float _24, float _31, float _32, float _33, float _34, float _41, float _42, float _43, float _44);
	XMMatrix(const XMMATRIX& mat);
	
	void Update(float _11, float _12, float _13, float _14, float _21, float _22, float _23, float _24, float _31, float _32, float _33, float _34, float _41, float _42, float _43, float _44);
	void Update();
	void UpdateFields();
	void ZeroOut();
	
	void Assign(const XMMatrix& mat);
	
	// Scale the current matrix
	XMMatrix Scale(float x, float y, float z);
	
	// Translate the current matrix
	XMMatrix Translate(float x, float y, float z);
	
	// Transpose the current matrix
	XMMatrix Transpose();
	
	// Invert the current matrix
	XMMatrix Inverse();
	
	// Multiply another matrix my this matrix
	void Multiply(const XMMatrix& mat);
	void Multiply(const XMMATRIX& mat);

	void LookAtLH(ZShadeSandboxMath::XMMath3 pos, ZShadeSandboxMath::XMMath3 target, ZShadeSandboxMath::XMMath3 up);
	
	XMMatrix AffineTransform(ZShadeSandboxMath::XMMath3 scaling, ZShadeSandboxMath::XMMath3 rotation, ZShadeSandboxMath::XMMath3 rotQuat, ZShadeSandboxMath::XMMath3 translation);
	
	XMFLOAT4X4 ToFloat4x4();

	// Converts this to a XMMATRIX
	operator XMMATRIX () { return __matrix; }

	XMMATRIX Get() const { return __matrix; }

	// Does a matrix multiply
	XMMatrix& operator * (const XMMatrix& mat);

	// Assigns another matrix to this matrix
	XMMatrix& operator = (const XMMatrix& mat);

	bool operator == (const XMMatrix& mat);

	// Generic matrix helper functions
public:

	// Scale another matrix
	static void Scale(float x, float y, float z, XMMATRIX& mat)
	{
		mat = XMMatrixScaling(x, y, z);
	}

	// Translate another matrix
	static void Translate(float x, float y, float z, XMMATRIX& mat)
	{
		mat = XMMatrixTranslation(x, y, z);
	}

	// Transpose another matrix
	static void Transpose(XMMATRIX& mat)
	{
		mat = XMMatrixTranspose(mat);
	}

	// Invert another matrix
	static void Inverse(XMMATRIX& mat)
	{
		mat = XMMatrixInverse(NULL, mat);
	}

	// Multiply another matrix my another matrix
	static XMMATRIX Multiply(const XMMATRIX& mat1, const XMMATRIX& mat2)
	{
		return XMMatrixMultiply(mat1, mat2);
	}
};
}
//==============================================================================================================================
//==============================================================================================================================
#endif//XMMATH_H