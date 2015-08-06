#include "XMMatrix.h"
#include "XMMath3.h"
using ZShadeSandboxMath::XMMatrix;
//==============================================================================================================================
//==============================================================================================================================
XMMatrix::XMMatrix()
{
	ZeroOut();
}
//==============================================================================================================================
XMMatrix::XMMatrix(float _11, float _12, float _13, float _14, float _21, float _22, float _23, float _24, float _31, float _32, float _33, float _34, float _41, float _42, float _43, float _44)
{
	Update
	(
		_11, _12, _13, _14,
		_21, _22, _23, _24,
		_31, _32, _33, _34,
		_41, _42, _43, _44
	);
}
//==============================================================================================================================
XMMatrix::XMMatrix(const XMMATRIX& mat)
{
	__matrix = mat;
}
//==============================================================================================================================
void XMMatrix::Update(float _11, float _12, float _13, float _14, float _21, float _22, float _23, float _24, float _31, float _32, float _33, float _34, float _41, float _42, float _43, float _44)
{
	XMVECTOR r0 = XMVectorSet(_11, _12, _13, _14);
	XMVECTOR r1 = XMVectorSet(_21, _22, _23, _24);
	XMVECTOR r2 = XMVectorSet(_31, _32, _33, _34);
	XMVECTOR r3 = XMVectorSet(_41, _42, _43, _44);
	
	__matrix.r[0] = r0;
	__matrix.r[1] = r1;
	__matrix.r[2] = r2;
	__matrix.r[3] = r3;
}
//==============================================================================================================================
void XMMatrix::Assign(const XMMatrix& mat)
{
	__matrix = mat.Get();
	UpdateFields();
}
//==============================================================================================================================
void XMMatrix::Update()
{
	XMVECTOR r0 = XMVectorSet(_11, _12, _13, _14);
	XMVECTOR r1 = XMVectorSet(_21, _22, _23, _24);
	XMVECTOR r2 = XMVectorSet(_31, _32, _33, _34);
	XMVECTOR r3 = XMVectorSet(_41, _42, _43, _44);
	
	__matrix.r[0] = r0;
	__matrix.r[1] = r1;
	__matrix.r[2] = r2;
	__matrix.r[3] = r3;
}
//==============================================================================================================================
void XMMatrix::ZeroOut()
{
	_11 = 0.0f;
	_12 = 0.0f;
	_13 = 0.0f;
	_14 = 0.0f;
	
	_21 = 0.0f;
	_22 = 0.0f;
	_23 = 0.0f;
	_24 = 0.0f;
	
	_31 = 0.0f;
	_32 = 0.0f;
	_33 = 0.0f;
	_34 = 0.0f;
	
	_41 = 0.0f;
	_42 = 0.0f;
	_43 = 0.0f;
	_44 = 0.0f;
	
	XMVECTOR r0 = XMVectorSet(_11, _12, _13, _14);
	XMVECTOR r1 = XMVectorSet(_21, _22, _23, _24);
	XMVECTOR r2 = XMVectorSet(_31, _32, _33, _34);
	XMVECTOR r3 = XMVectorSet(_41, _42, _43, _44);
	
	__matrix.r[0] = r0;
	__matrix.r[1] = r1;
	__matrix.r[2] = r2;
	__matrix.r[3] = r3;
}
//==============================================================================================================================
void XMMatrix::UpdateFields()
{
	XMFLOAT4 r0;
	XMFLOAT4 r1;
	XMFLOAT4 r2;
	XMFLOAT4 r3;
	
	XMStoreFloat4(&r0, __matrix.r[0]);
	XMStoreFloat4(&r1, __matrix.r[1]);
	XMStoreFloat4(&r2, __matrix.r[2]);
	XMStoreFloat4(&r3, __matrix.r[3]);
	
	_11 = r0.x;
	_12 = r0.y;
	_13 = r0.z;
	_14 = r0.w;
	
	_21 = r1.x;
	_22 = r1.y;
	_23 = r1.z;
	_24 = r1.w;
	
	_31 = r2.x;
	_32 = r2.y;
	_33 = r2.z;
	_34 = r2.w;
	
	_41 = r3.x;
	_42 = r3.y;
	_43 = r3.z;
	_44 = r3.w;
}
//==============================================================================================================================
XMMatrix XMMatrix::Scale(float x, float y, float z)
{
	__matrix = XMMatrixScaling(x, y, z);
	UpdateFields();
	return __matrix;
}
//==============================================================================================================================
XMMatrix XMMatrix::Translate(float x, float y, float z)
{
	__matrix = XMMatrixTranslation(x, y, z);
	UpdateFields();
	return __matrix;
}
//==============================================================================================================================
XMMatrix XMMatrix::Transpose()
{
	__matrix = XMMatrixTranspose( __matrix );
	UpdateFields();
	return __matrix;
}
//==============================================================================================================================
XMMatrix XMMatrix::Inverse()
{
	__matrix = XMMatrixInverse( NULL, __matrix );
	UpdateFields();
	return __matrix;
}
//==============================================================================================================================
void XMMatrix::Multiply(const XMMatrix& mat)
{
	__matrix = XMMatrixMultiply(__matrix, (XMMATRIX)mat.Get());
	UpdateFields();
}
//==============================================================================================================================
void XMMatrix::Multiply(const XMMATRIX& mat)
{
	__matrix = XMMatrixMultiply(__matrix, (XMMATRIX)mat);
	UpdateFields();
}
//==============================================================================================================================
XMFLOAT4X4 XMMatrix::ToFloat4x4()
{
	XMFLOAT4X4 mat;
	XMStoreFloat4x4(&mat, __matrix);
	return mat;
}
//==============================================================================================================================
void XMMatrix::LookAtLH(ZShadeSandboxMath::XMMath3 pos, ZShadeSandboxMath::XMMath3 target, ZShadeSandboxMath::XMMath3 up)
{
	XMVECTOR posV = pos.ToVector();
	XMVECTOR targetV = target.ToVector();
	XMVECTOR upV = up.ToVector();
	__matrix = XMMatrixLookAtLH(posV, targetV, upV);
	UpdateFields();
}
//==============================================================================================================================
XMMatrix XMMatrix::AffineTransform(ZShadeSandboxMath::XMMath3 scaling, ZShadeSandboxMath::XMMath3 rotation, ZShadeSandboxMath::XMMath3 rotQuat, ZShadeSandboxMath::XMMath3 translation)
{
	XMVECTOR scalingV = scaling.ToVector();
	XMVECTOR rotationV = rotation.ToVector();
	XMVECTOR rotQuatV = rotQuat.ToVector();
	XMVECTOR translationV = translation.ToVector();
	__matrix = XMMatrixAffineTransformation(scalingV, rotationV, rotQuatV, translationV);
	UpdateFields();
	return __matrix;
}
//==============================================================================================================================
XMMatrix& XMMatrix::operator * (const XMMatrix& mat)
{
	Multiply(mat);
	return *this;
}
//==============================================================================================================================
XMMatrix& XMMatrix::operator = (const XMMatrix& mat)
{
	Assign(mat);
	return *this;
}
//==============================================================================================================================
bool XMMatrix::operator == (const XMMatrix& mat)
{
	bool ret = true;
	
	for (uint32 i = 0; i < 4; i++)
	{
		for (uint32 j = 0; j < 4; j++)
		{
			if (__matrix.m[i][j] != mat.Get().m[i][j])
				ret = false;
		}
	}

	return ret;
}
//==============================================================================================================================