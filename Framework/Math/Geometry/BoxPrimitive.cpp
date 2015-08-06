#include "BoxPrimitive.h"
using ZShadeSandboxMath::BoxPrimitive;
//==============================================================================================================================
//==============================================================================================================================
BoxPrimitive::BoxPrimitive()
{
}
//==============================================================================================================================
BoxPrimitive::BoxPrimitive(XMFLOAT3 v[8])
{
	for (int i = 0; i < 8; i++)
		this->vertices[i] = v[i];
}
//==============================================================================================================================
BoxPrimitive::BoxPrimitive(XMFLOAT4 v[8])
{
	for (int i = 0; i < 8; i++)
		this->vertices4[i] = v[i];
}
//==============================================================================================================================
void BoxPrimitive::GetVertices(XMFLOAT3 v[8])
{
	for (int i = 0; i < 8; i++)
		v[0] = this->vertices[i];
}
//==============================================================================================================================
void BoxPrimitive::GetVertices(XMFLOAT4 v[8])
{
	for (int i = 0; i < 8; i++)
		v[0] = this->vertices4[i];
}
//==============================================================================================================================
XMFLOAT3 BoxPrimitive::Center()
{
	// Find two diagonal points
	ZShadeSandboxMath::Vec3<XMFLOAT3> diag1 = this->vertices[0];
	ZShadeSandboxMath::Vec3<XMFLOAT3> diag2 = this->vertices[6];

	ZShadeSandboxMath::Vec3<XMFLOAT3> c;

	// Find the midpoint of the diagonal points
	c.AddVectorData(
		(diag1.Vector().x + diag2.Vector().x) / 2,
		(diag1.Vector().y + diag2.Vector().y) / 2,
		(diag1.Vector().z + diag2.Vector().z) / 2);

	return c;
}
//==============================================================================================================================
XMFLOAT4 BoxPrimitive::Center4()
{
	// Find two diagonal points
	ZShadeSandboxMath::Vec4<XMFLOAT4> diag1 = this->vertices4[0];
	ZShadeSandboxMath::Vec4<XMFLOAT4> diag2 = this->vertices4[6];

	ZShadeSandboxMath::Vec4<XMFLOAT4> c;

	// Find the midpoint of the diagonal points
	c.AddVectorData(
		(diag1.Vector().x + diag2.Vector().x) / 2,
		(diag1.Vector().y + diag2.Vector().y) / 2,
		(diag1.Vector().z + diag2.Vector().z) / 2,
		(diag1.Vector().w + diag2.Vector().w) / 2);

	return c;
}
//==============================================================================================================================
bool BoxPrimitive::ContainsPoint(XMFLOAT3 point)
{
	// Find two diagonal points
	ZShadeSandboxMath::Vec3<XMFLOAT3> vdiag1 = this->vertices[0];
	ZShadeSandboxMath::Vec3<XMFLOAT3> vdiag2 = this->vertices[6];
	XMFLOAT3 diag1(vdiag1.Vector().x, vdiag1.Vector().y, vdiag1.Vector().z);// , vdiag1.Vector().w);
	XMFLOAT3 diag2(vdiag2.Vector().x, vdiag2.Vector().y, vdiag2.Vector().z);// , vdiag2.Vector().w);

	// See if the point lies within the diagonal of the box
	bool inX = point.x > diag1.x && point.x < diag2.x;
	bool inY = point.y > diag1.y && point.y < diag2.y;
	bool inZ = point.z > diag1.z && point.z < diag2.z;

	return inX && inY && inZ;
}
//==============================================================================================================================
bool BoxPrimitive::ContainsPoint(XMFLOAT4 point)
{
	// Find two diagonal points
	ZShadeSandboxMath::Vec4<XMFLOAT4> vdiag1 = this->vertices4[0];
	ZShadeSandboxMath::Vec4<XMFLOAT4> vdiag2 = this->vertices4[6];
	XMFLOAT4 diag1(vdiag1.Vector().x, vdiag1.Vector().y, vdiag1.Vector().z, vdiag1.Vector().w);
	XMFLOAT4 diag2(vdiag2.Vector().x, vdiag2.Vector().y, vdiag2.Vector().z, vdiag2.Vector().w);

	// See if the point lies within the diagonal of the box
	bool inX = point.x > diag1.x && point.x < diag2.x;
	bool inY = point.y > diag1.y && point.y < diag2.y;
	bool inZ = point.z > diag1.z && point.z < diag2.z;
	bool inW = point.w > diag1.w && point.w < diag2.w;

	return inX && inY && inZ && inW;
}
//==============================================================================================================================
/*void BoxPrimitive::CreateFrom4Points(XMFLOAT3 p1, XMFLOAT3 p2, XMFLOAT3 p3, XMFLOAT3 p4, float max, float min, ZShadeSandboxMath::EAxis::Type axis)
{
	vector<XMFLOAT3> point_check;
	point_check.push_back(p1);
	point_check.push_back(p2);
	point_check.push_back(p3);
	point_check.push_back(p4);

	this->vertices[0] = p1;
	this->vertices[1] = p2;
	this->vertices[2] = p3;
	this->vertices[3] = p4;

	if (axis == 0)
	{
		this->vertices[0].AddVectorData(p1.x - min, this->vertices[0].Vector().y, this->vertices[0].Vector().z);
		this->vertices[1].AddVectorData(p2.x - min, this->vertices[1].Vector().y, this->vertices[1].Vector().z);
		this->vertices[2].AddVectorData(p3.x - min, this->vertices[2].Vector().y, this->vertices[2].Vector().z);
		this->vertices[3].AddVectorData(p4.x - min, this->vertices[3].Vector().y, this->vertices[3].Vector().z);
	}
	else if (axis == 1)
	{
		this->vertices[0].AddVectorData(this->vertices[0].Vector().x, p1.y - min, this->vertices[0].Vector().z);
		this->vertices[1].AddVectorData(this->vertices[1].Vector().x, p2.y - min, this->vertices[1].Vector().z);
		this->vertices[2].AddVectorData(this->vertices[2].Vector().x, p3.y - min, this->vertices[2].Vector().z);
		this->vertices[3].AddVectorData(this->vertices[3].Vector().x, p4.y - min, this->vertices[3].Vector().z);
	}
	else if (axis == 2)
	{
		this->vertices[0].AddVectorData(this->vertices[0].Vector().x, this->vertices[0].Vector().y, p1.z - min);
		this->vertices[1].AddVectorData(this->vertices[1].Vector().x, this->vertices[1].Vector().y, p1.z - min);
		this->vertices[2].AddVectorData(this->vertices[2].Vector().x, this->vertices[2].Vector().y, p1.z - min);
		this->vertices[3].AddVectorData(this->vertices[3].Vector().x, this->vertices[3].Vector().y, p1.z - min);
	}

	unsigned int cnt = 0;

	// Extend the other four points along a distance in a given direction
	for (int i = 4; i < 8; i++)
	{
		XMFLOAT3 vector = point_check[cnt];

		cnt++;

		if (axis == 0)
		{
			this->vertices[i].AddVectorData(vector.x + max, vector.y, vector.z);
		}
		else if (axis == 1)
		{
			this->vertices[i].AddVectorData(vector.x, vector.y + max, vector.z);
		}
		else if (axis == 2)
		{
			this->vertices[i].AddVectorData(vector.x, vector.y, vector.z + max);
		}
	}
}
//==============================================================================================================================
void BoxPrimitive::CreateFrom4Points(XMFLOAT4 p1, XMFLOAT4 p2, XMFLOAT4 p3, XMFLOAT4 p4, float max, float min, ZShadeSandboxMath::EAxis::Type axis)
{
	vector<XMFLOAT4> point_check;
	point_check.push_back(p1);
	point_check.push_back(p2);
	point_check.push_back(p3);
	point_check.push_back(p4);

	this->vertices4[0] = p1;
	this->vertices4[1] = p2;
	this->vertices4[2] = p3;
	this->vertices4[3] = p4;

	if (axis == 0)
	{
		this->vertices4[0].AddVectorData(p1.x - min, this->vertices4[0].Vector().y, this->vertices4[0].Vector().z, 0);
		this->vertices4[1].AddVectorData(p2.x - min, this->vertices4[1].Vector().y, this->vertices4[1].Vector().z, 0);
		this->vertices4[2].AddVectorData(p3.x - min, this->vertices4[2].Vector().y, this->vertices4[2].Vector().z, 0);
		this->vertices4[3].AddVectorData(p4.x - min, this->vertices4[3].Vector().y, this->vertices4[3].Vector().z, 0);
	}
	else if (axis == 1)
	{
		this->vertices4[0].AddVectorData(this->vertices4[0].Vector().x, p1.y - min, this->vertices4[0].Vector().z, 0);
		this->vertices4[1].AddVectorData(this->vertices4[1].Vector().x, p2.y - min, this->vertices4[1].Vector().z, 0);
		this->vertices4[2].AddVectorData(this->vertices4[2].Vector().x, p3.y - min, this->vertices4[2].Vector().z, 0);
		this->vertices4[3].AddVectorData(this->vertices4[3].Vector().x, p4.y - min, this->vertices4[3].Vector().z, 0);
	}
	else if (axis == 2)
	{
		this->vertices4[0].AddVectorData(this->vertices4[0].Vector().x, this->vertices4[0].Vector().y, p1.z - min, 0);
		this->vertices4[1].AddVectorData(this->vertices4[1].Vector().x, this->vertices4[1].Vector().y, p1.z - min, 0);
		this->vertices4[2].AddVectorData(this->vertices4[2].Vector().x, this->vertices4[2].Vector().y, p1.z - min, 0);
		this->vertices4[3].AddVectorData(this->vertices4[3].Vector().x, this->vertices4[3].Vector().y, p1.z - min, 0);
	}

	unsigned int cnt = 0;

	// Extend the other four points along a distance in a given direction
	for (int i = 4; i < 8; i++)
	{
		XMFLOAT4 vector = point_check[cnt];

		cnt++;

		if (axis == 0)
		{
			this->vertices4[i].AddVectorData(vector.x + max, vector.y, vector.z, 0);
		}
		else if (axis == 1)
		{
			this->vertices4[i].AddVectorData(vector.x, vector.y + max, vector.z, 0);
		}
		else if (axis == 2)
		{
			this->vertices4[i].AddVectorData(vector.x, vector.y, vector.z + max, 0);
		}
	}
}*/
//==============================================================================================================================