#include "OBJMesh.h"
#include "TextureManager.h"
#include <memory>
#include "ShapeContact.h"
using namespace ZShadeSandboxMesh;
//===============================================================================================================================
//===============================================================================================================================
GameDirectory3D* OBJMesh::mGD3D = 0;
D3D*  OBJMesh::mD3DSystem = 0;
//===============================================================================================================================
OBJMesh::OBJMesh(D3D* d3d, GameDirectory3D* g3D)
{
	OBJMesh::mD3DSystem = d3d;
	OBJMesh::mGD3D = g3D;
}
//===============================================================================================================================
OBJMesh::~OBJMesh()
{
}
//===============================================================================================================================
void OBJMesh::Scale(XMFLOAT3 v)
{
	m_pMesh->Scale(v);
}
//===============================================================================================================================
void OBJMesh::Rotate(XMFLOAT3 v)
{
	m_pMesh->Rotate(v);
}
//===============================================================================================================================
void OBJMesh::Position(XMFLOAT3 v)
{
	m_pMesh->Position(v);
}
//===============================================================================================================================
void OBJMesh::SetFarPlane(float farPlane)
{
	m_pMesh->SetFarPlane(farPlane);
}
//===============================================================================================================================
ZShadeSandboxMath::AABB* OBJMesh::GetAABB()
{
	return m_pMesh->GetAABB();
}
//===============================================================================================================================
bool OBJMesh::IntersectsAABB(XMFLOAT3 point)
{
	return m_pMesh->IntersectsAABB(point);
}
//===============================================================================================================================
float OBJMesh::DistanceAABBToPoint(XMFLOAT3 point)
{
	return m_pMesh->DistanceAABBToPoint(point);
}
//===============================================================================================================================
void OBJMesh::UpdateAABB(XMFLOAT3 center, XMFLOAT3 scale)
{
	m_pMesh->UpdateAABB(center, scale);
}
//===============================================================================================================================
ZShadeSandboxMath::SpherePrimitive* OBJMesh::GetSphere()
{
	return m_pMesh->GetSphere();
}
//===============================================================================================================================
bool OBJMesh::IntersectsSphere(XMFLOAT3 point)
{
	return m_pMesh->IntersectsSphere(point);
}
//===============================================================================================================================
float OBJMesh::DistanceSphereToPoint(XMFLOAT3 point)
{
	return m_pMesh->DistanceSphereToPoint(point);
}
//===============================================================================================================================
void OBJMesh::Load(string filename, bool bRebuildNormals, bool isRHCoordSys)
{
	m_pMesh = new Mesh(mD3DSystem);
	m_pMesh->Load(filename, bRebuildNormals, isRHCoordSys);
}
//===============================================================================================================================
void OBJMesh::Render(MeshRenderParameters render_param)
{
	m_pMesh->Render(render_param);
}
//===============================================================================================================================
void OBJMesh::SetWireframe(bool wire)
{
	m_pMesh->SetWireframe(wire);
}
//===============================================================================================================================
void OBJMesh::Mesh::Scale(XMFLOAT3 v)
{
	mAttributes->mScale = v;

	// Rescale the AABB and Sphere
	

	//float r = mSphere->Radius();
	//mSphere->Radius() = r * v.x;
}
//===============================================================================================================================
void OBJMesh::Mesh::Rotate(XMFLOAT3 v)
{
	mAttributes->mRotation = v;

	// Rotate the AABB

}
//===============================================================================================================================
void OBJMesh::Mesh::Position(XMFLOAT3 v)
{
	mAttributes->mPosition = v;

	// Reposition the AABB and Sphere
	//UpdateAABB(v, mAABB->vSize);
	//mAABB->vCenter.x += v.x;
	//mAABB->vCenter.y += v.y;
	//mAABB->vCenter.z += v.z;

	mAABB->vMin.x += v.x;
	mAABB->vMin.y += v.y;
	mAABB->vMin.z += v.z;

	mAABB->vMax.x += v.x;
	mAABB->vMax.y += v.y;
	mAABB->vMax.z += v.z;
}
//===============================================================================================================================
void OBJMesh::Mesh::SetWireframe(bool wire)
{
	mShader->Wireframe() = wire;
	mTessellationShader->Wireframe() = wire;
	mOBJGBufferShader->Wireframe() = wire;
}
//===============================================================================================================================
void OBJMesh::Mesh::SetFarPlane(float farPlane)
{
	for (auto& group : mGroups)
	{
		auto* material = &group.Material;

		material->fFarPlane = farPlane;
	}
}
//===============================================================================================================================
ZShadeSandboxMath::AABB* OBJMesh::Mesh::GetAABB()
{
	return mAABB;
}
//===============================================================================================================================
bool OBJMesh::Mesh::IntersectsAABB(XMFLOAT3 point)
{
	if (mAABB == 0) return false;
	
	// Calculate the dimensions of this node.
	float xMin = mAABB->vMin.x;
	float xMax = mAABB->vMax.x;
	
	float zMin = mAABB->vMin.z;
	float zMax = mAABB->vMax.z;
	
	// See if the x and z coordinate are in this node, if not then stop traversing this part of the tree.
	if ((point.x < xMin) || (point.x > xMax) || (point.z < zMin) || (point.z > zMax))
	{
		return false;
	}
	
	return mAABB->ContainsPoint3D(point);
}
//===============================================================================================================================
float OBJMesh::Mesh::DistanceAABBToPoint(XMFLOAT3 point)
{
	return mAABB->DistanceToPoint3D(point);
}
//===============================================================================================================================
void OBJMesh::Mesh::UpdateAABB(XMFLOAT3 center, XMFLOAT3 scale)
{
	mAABB->vMin = XMFLOAT3(center.x - scale.x, center.y - scale.y, center.z - scale.z);
	mAABB->vMax = XMFLOAT3(center.x + scale.x, center.y + scale.y, center.z + scale.z);
}
//===============================================================================================================================
ZShadeSandboxMath::SpherePrimitive* OBJMesh::Mesh::GetSphere()
{
	return mSphere;
}
//===============================================================================================================================
bool OBJMesh::Mesh::IntersectsSphere(XMFLOAT3 point)
{
	return ShapeContact::SphereIntersectsPoint(*mSphere, point);
}
//===============================================================================================================================
float OBJMesh::Mesh::DistanceSphereToPoint(XMFLOAT3 point)
{
	return ShapeContact::DistanceSphereToPoint(*mSphere, point);
}
//===============================================================================================================================
bool OBJMesh::LoadObj(string filename, D3D* d3d, vector<VertexNormalTex>& vertices,
	vector<IndexType>& indices, vector<Group>& groups, bool& hadNormals, bool isRHCoordSys)
{
	// coordinate system handedness is handled similarly as in MeshFromOBJ10 sample (do nothing)

	vector<XMFLOAT3> positions;
	vector<XMFLOAT3> normals;
	vector<XMFLOAT2> texCoords;
	map<string, ZShadeSandboxLighting::ShaderMaterial> materials;

	ZShadeSandboxLighting::ShaderMaterial* currentMaterial = new ZShadeSandboxLighting::ShaderMaterial(d3d);
	
	// File Input
	ifstream file(filename.c_str());
	if (!file) return false;

	// Read the file line by line.
	for (string line; getline(file, line);)
	{
		string key;
		istringstream ss(line);
		ss >> key;

		if (key == "v") // vertex position
		{
			float x, y, z;
			ss >> x >> y >> z;
			if (isRHCoordSys)
			{
				z = z * -1.0f;
			}
			positions.push_back(XMFLOAT3(x*10, y*10, z*10));
		}
		else if (key == "vt") // texture coordinate
		{
			float u, v;
			ss >> u >> v;
			if (isRHCoordSys)
			{
				v = 1.0f - v;
			}
			texCoords.push_back(XMFLOAT2(u, v));
		}
		else if (key == "vn") // vertex normal
		{
			float  x, y, z;
			ss >> x >> y >> z;
			if (isRHCoordSys)
			{
				z = z * -1.0f;
			}
			normals.push_back(XMFLOAT3(x, y, z));
		}
		else if (key == "f") // face
		{
			// Other than triangular faces are not supported.
			for (size_t i = 0; i < 3; i++)
			{
				string vertexString;
				ss >> vertexString;
				istringstream vertexss(vertexString);

				string indexString;
				int index;
				VertexNormalTex vertex;

				// Each vertex must have position. Texture coordinates and normal are optional.
				getline(vertexss, indexString, '/');
				istringstream(indexString) >> index;
				vertex.position = positions[index - 1];

				if (getline(vertexss, indexString, '/') && !indexString.empty())
				{
					istringstream(indexString) >> index;
					vertex.texture = texCoords[index - 1];
				}
				else
					vertex.texture = XMFLOAT2(0, 0);

				if (getline(vertexss, indexString, '/') && !indexString.empty())
				{
					istringstream(indexString) >> index;
					vertex.normal = normals[index - 1];
				}
				else
					vertex.normal = XMFLOAT3(0, 1, 0);

				// If a similar vertex was added before, there is no need to add a new one.
				bool found = false;
				for (int i = 0; i < vertices.size(); i++)
				{
					if (memcmp(&vertices[i], &vertex, sizeof(VertexNormalTex)) == 0)
					{
						// The vertices area similar, use previously stored vertex.
						indices.push_back(i);
						found = true;
						break;
						// goto next_line;
					}					
				}
				if (!found)
				{
					indices.push_back(vertices.size());
					vertices.push_back(vertex);
				}
			}
		}
		else if (key == "g") // group
		{
			if (!groups.empty())
			{
				// finalize current group
				groups.back().Material = *currentMaterial;
				groups.back().IndexCount = indices.size() - groups.back().IndexStart;
			}

			// Start new group
			Group group;
			ss >> group.Name;
			group.IndexStart = indices.size();
			groups.push_back(group);
		}
		else if (key == "mtllib")
		{
			string mtllibFileName;
			ss >> mtllibFileName;
			if (!LoadMtl("Models//" + mtllibFileName, d3d, materials))
				return false;
		}
		else if (key == "usemtl") // material
		{
			string materialName;
			ss >> materialName;
			currentMaterial = &materials[materialName];
		}
	}
	hadNormals = !normals.empty();

	// Last group doesn't get finalized elsewhere
	if (!groups.empty())
	{
		groups.back().Material = *currentMaterial;
		groups.back().Material.SetD3D(mD3DSystem);
		groups.back().IndexCount = indices.size() - groups.back().IndexStart;
	}

	file.close();
	materials.clear();
	return true;
}
//===============================================================================================================================
bool OBJMesh::LoadMtl(string filename, D3D* d3d, map<string, ZShadeSandboxLighting::ShaderMaterial>& materials)
{
	// File Input
	ifstream file(filename.c_str());
	if (!file) return false;

	ZShadeSandboxLighting::ShaderMaterial* mat = NULL;
	for (string line; getline(file, line);)
	{
		string key;
		istringstream ss(line);
		ss >> key;

		if (key == "newmtl")
		{
			// Switching active materials
			string matName;
			ss >> matName;
			mat = new ZShadeSandboxLighting::ShaderMaterial(d3d, matName);
			materials[mat->sMaterialName] = *mat;
			mat = &materials[mat->sMaterialName];
		}

		// The rest of the commands need an active material
		if (mat == NULL) continue;

		if (key == "Ka")
		{
			// Ambient color
			float r, g, b;
			ss >> r >> g >> b;
			mat->vAmbientColor = XMFLOAT4(r, g, b, 1);
		}
		else if (key == "Kd")
		{
			// Diffuse color
			float r, g, b;
			ss >> r >> g >> b;
			mat->vDiffuseColor = XMFLOAT4(r, g, b, 1);
		}
		else if (key == "Ks")
		{
			// Specular color
			float r, g, b;
			ss >> r >> g >> b;
			mat->vSpecularColor = XMFLOAT4(r, g, b, 1);
		}
		else if (key == "Ke")
		{
			// Emissive color
			float r, g, b;
			ss >> r >> g >> b;
			mat->vEmissiveColor = XMFLOAT4(r, g, b, 1);
		}
		else if (key == "Tf")
		{
			// Transmission Filter
			float r, g, b;
			ss >> r >> g >> b;
			mat->vTransmissionFilter = XMFLOAT3(r, g, b);
		}
		else if (key == "d" || key == "Tr")
		{
			// Material Dissolve (Alpha)
			ss >> mat->fAlpha;
			if (mat->fAlpha > 0.0f)
			{
				mat->bHasTransparency = true;
			}
		}
		else if (key == "Ns")
		{
			// Specular Shininess
			int nShininess;
			ss >> nShininess;
			mat->fSpecularPower = nShininess;
		}
		else if (key == "Db")
		{
			ss >> mat->fDetailBrightness;
		}
		else if (key == "Ni")
		{
			ss >> mat->fRefractionIndex;
		}
		else if (key == "illum")
		{
			// Specular on/off
			int illumination;
			ss >> illumination;
			mat->iIlluminationModel = illumination;
			mat->bSpecularToggle = (illumination == 2);
		}
		else if (key == "map_Km")
		{
			// Diffuse Texture
			ss >> mat->sDetailMapTextureName;
			mat->bHasDetailMapTexture = true;
		}
		else if (key == "map_Kd")
		{
			// Diffuse Texture
			ss >> mat->sDiffuseTextureName;
			mat->bHasDiffuseTexture = true;
		}
		else if (key == "map_Ka")
		{
			// Ambient Texture
			ss >> mat->sAmbientTextureName;
			mat->bHasAmbientTexture = true;
		}
		else if (key == "map_Ks")
		{
			// Specular Texture
			ss >> mat->sSpecularTextureName;
			mat->bHasSpecularTexture = true;
		}
		else if (key == "map_Ke")
		{
			// Specular Texture
			ss >> mat->sEmissiveTextureName;
			mat->bHasEmissiveTexture = true;
		}
		else if (key == "map_d")
		{
			// Alpha Texture
			ss >> mat->sAlphaTextureName;
			mat->bHasTransparency = true;
		}
		else if (key == "map_bump")
		{
			// Normal Map (Bump Map) Texture
			ss >> mat->sNormalMapTextureName;
			mat->bHasNormalMapTexture = true;
		}
	}

	// Load material textures
	for (auto& iter : materials)
	{
		auto& mat = iter.second;

		if (mat.bHasDiffuseTexture) mat.AddDiffuseTexture(mGD3D->m_textures_path, mat.sDiffuseTextureName);
		if (mat.bHasAmbientTexture) mat.AddAmbientTexture(mGD3D->m_textures_path, mat.sAmbientTextureName);
		if (mat.bHasSpecularTexture) mat.AddSpecularTexture(mGD3D->m_textures_path, mat.sSpecularTextureName);
		if (mat.bHasEmissiveTexture) mat.AddEmissiveTexture(mGD3D->m_textures_path, mat.sEmissiveTextureName);
		if (mat.bHasNormalMapTexture) mat.AddNormalMapTexture(mGD3D->m_textures_path, mat.sNormalMapTextureName);
		if (mat.bHasTransparency) mat.AddAlphaMapTexture(mGD3D->m_textures_path, mat.sAlphaTextureName);
		if (mat.bHasDetailMapTexture) mat.AddDetailMapTexture(mGD3D->m_textures_path, mat.sDetailMapTextureName);

		mat.vDiffuseColor.w = mat.fAlpha;
		mat.vAmbientColor.w = mat.fAlpha;
		mat.vSpecularColor.w = mat.fAlpha;
		mat.vEmissiveColor.w = mat.fAlpha;
	}

	file.close();
	return true;
}
//===============================================================================================================================
OBJMesh::Mesh::Mesh(D3D* d3d)
	: mD3DSystem(d3d)
{
	mAttributes = new MeshAttributes();
	mAttributes->mD3D = d3d;

	mAttributes->mPosition = XMFLOAT3(0, 0, 0);
	mAttributes->mScale = XMFLOAT3(1, 1, 1);
	mAttributes->mRotation = XMFLOAT3(0, 0, 0);

	// Set the world matrix to the identity matrix
	//mWorld = XMMatrixIdentity();

	mShader = new OBJMeshShader(mD3DSystem);
	mTessellationShader = new OBJMeshTessellationShader(mD3DSystem);
	mOBJGBufferShader = new OBJGBufferShader(mD3DSystem);
}
//===============================================================================================================================
OBJMesh::Mesh::~Mesh()
{
	// Need to release all subsets
	mAttributes->Release();
	mGroups.clear();
	delete mShader;
	delete mTessellationShader;
	delete mOBJGBufferShader;
}
//===============================================================================================================================
bool OBJMesh::Mesh::Load(string filename, bool bRebuildNormals, bool isRHCoordSys)
{
	//vector<VertexNormalTex> vertices;
	//vector<IndexType> indices;
	bool objHadNormals;

	if (!LoadObj(filename, mD3DSystem, mAttributes->mVerticesNormalTex, mAttributes->mIndices, mGroups, objHadNormals, isRHCoordSys))
		return false;

	mAttributes->mVertexCount = mAttributes->mVerticesNormalTex.size();
	mAttributes->mIndexCount = mAttributes->mIndices.size();
	mAttributes->mTriangleCount = mAttributes->mIndexCount / 3;

	if (!objHadNormals || bRebuildNormals)
	{
		for (auto& vertex : mAttributes->mVerticesNormalTex)
		{
			vertex.normal = XMFLOAT3(0, 0, 0);
		}
		for (int i = 0; i < mAttributes->mIndices.size(); i += 3)
		{
			// Calculate triangle face normal

			int i0 = mAttributes->mIndices[i];
			int i1 = mAttributes->mIndices[i + 1];
			int i2 = mAttributes->mIndices[i + 2];

			XMFLOAT3 p0 = mAttributes->mVerticesNormalTex[i0].position;
			XMFLOAT3 p1 = mAttributes->mVerticesNormalTex[i1].position;
			XMFLOAT3 p2 = mAttributes->mVerticesNormalTex[i2].position;

			float edge1x = p1.x - p0.x;
			float edge1y = p1.y - p0.y;
			float edge1z = p1.z - p0.z;

			float edge2x = p2.x - p0.x;
			float edge2y = p2.y - p0.y;
			float edge2z = p2.z - p0.z;

			float normalx = edge1y * edge2z - edge1z * edge2y;
			float normaly = edge1z * edge2x - edge1x * edge2z;
			float normalz = edge1x * edge2y - edge1y * edge2x;

			mAttributes->mVerticesNormalTex[i0].normal.x += normalx;
			mAttributes->mVerticesNormalTex[i0].normal.y += normaly;
			mAttributes->mVerticesNormalTex[i0].normal.z += normalz;
			mAttributes->mVerticesNormalTex[i1].normal.x += normalx;
			mAttributes->mVerticesNormalTex[i1].normal.y += normaly;
			mAttributes->mVerticesNormalTex[i1].normal.z += normalz;
			mAttributes->mVerticesNormalTex[i2].normal.x += normalx;
			mAttributes->mVerticesNormalTex[i2].normal.y += normaly;
			mAttributes->mVerticesNormalTex[i2].normal.z += normalz;
		}
		for (auto& vertex : mAttributes->mVerticesNormalTex)
		{
			float f = 1.0f / (sqrtf(vertex.normal.x * vertex.normal.x + vertex.normal.y * vertex.normal.y + vertex.normal.z * vertex.normal.z));

			vertex.normal.x *= f;
			vertex.normal.y *= f;
			vertex.normal.z *= f;
		}
	}
	
	//
	// Create the Axis Aligned Bounding box for the mesh
	//

	XMFLOAT3 minVertex(FLT_MAX, FLT_MAX, FLT_MAX);
	XMFLOAT3 maxVertex(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	
	for (int i = 0; i < mAttributes->mVerticesNormalTex.size(); i++)
	{
		// The minVertex and maxVertex will most likely not be actual vertices in the model, but vertices
        // that use the smallest and largest x, y, and z values from the model to be sure ALL vertices are
        // covered by the bounding volume
		
        //Get the smallest vertex 
		minVertex.x = min(minVertex.x, mAttributes->mVerticesNormalTex[i].position.x);    // Find smallest x value in model
		minVertex.y = min(minVertex.y, mAttributes->mVerticesNormalTex[i].position.y);    // Find smallest y value in model
		minVertex.z = min(minVertex.z, mAttributes->mVerticesNormalTex[i].position.z);    // Find smallest z value in model
		
        //Get the largest vertex 
		maxVertex.x = max(maxVertex.x, mAttributes->mVerticesNormalTex[i].position.x);    // Find largest x value in model
		maxVertex.y = max(maxVertex.y, mAttributes->mVerticesNormalTex[i].position.y);    // Find largest y value in model
		maxVertex.z = max(maxVertex.z, mAttributes->mVerticesNormalTex[i].position.z);    // Find largest z value in model
	}
	
	mAABB = new ZShadeSandboxMath::AABB();
	mAABB->vMin = minVertex;
	mAABB->vMax = maxVertex;
	
	// Find the true center of the model
	//mAABB->vCenter = ZShadeSandboxMath::XMMath3(maxVertex) - ZShadeSandboxMath::XMMath3(minVertex) / 2.0f;
	
	float sphereRadius = 0.0f;
	
	//XMFLOAT3 center;

	// Get the bounding sphere
	/*for (int i = 0; i < mAttributes->mVerticesNormalTex.size(); i++)
	{
		float x = (mAABB->vCenter.x - mAttributes->mVerticesNormalTex[i].position.x) * (mAABB->vCenter.x - mAttributes->mVerticesNormalTex[i].position.x);
		float y = (mAABB->vCenter.y - mAttributes->mVerticesNormalTex[i].position.y) * (mAABB->vCenter.y - mAttributes->mVerticesNormalTex[i].position.y);
		float z = (mAABB->vCenter.z - mAttributes->mVerticesNormalTex[i].position.z) * (mAABB->vCenter.z - mAttributes->mVerticesNormalTex[i].position.z);
		
		sphereRadius = max(sphereRadius, x + y + z);
	}*/
	
	// We didn't use the square root when finding the largest distance since it slows things down.
	// We can square root the answer from above to get the actual bounding sphere now
	sphereRadius = sqrt(sphereRadius);
	
	mSphere = new ZShadeSandboxMath::SpherePrimitive();
	mSphere->Center() = mAABB->Center();
	mSphere->Radius() = sphereRadius;
	
	mAttributes->BuildVertexBuffer(ZShadeSandboxMesh::EVertexType::VT_NormalTex);
	mAttributes->BuildIndexBuffer();

	/*D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(ZShadeSandboxMesh::VertexNormalTex) * mVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = &vertices[0];
	mD3DSystem->GetDevice11()->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &mVB);

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(IndexType) * mIndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	mD3DSystem->GetDevice11()->CreateBuffer(&indexBufferDesc, &iinitData, &mIB);*/

	return true;
}
//===============================================================================================================================
//void OBJMesh::Mesh::CalculateWorldMatrix()
//{
//	XMMATRIX scale, rx, ry, rz, translation;
//
//	scale = XMMatrixScaling(mAttributes->mScale.x, mAttributes->mScale.y, mAttributes->mScale.z);
//	rx = XMMatrixRotationX(mAttributes->mRotation.x);
//	ry = XMMatrixRotationY(mAttributes->mRotation.y);
//	rz = XMMatrixRotationZ(mAttributes->mRotation.z);
//	translation = XMMatrixTranslation(mAttributes->mPosition.x, mAttributes->mPosition.y, mAttributes->mPosition.z);
//
//	XMMATRIX world = XMMatrixIdentity();
//
//	mWorld = XMMatrixMultiply(world, scale);
//	mWorld = XMMatrixMultiply(mWorld, rx);
//	mWorld = XMMatrixMultiply(mWorld, ry);
//	mWorld = XMMatrixMultiply(mWorld, rz);
//	mWorld = XMMatrixMultiply(mWorld, translation);
//}
//===============================================================================================================================
void OBJMesh::Mesh::Render(MeshRenderParameters render_param)
{
	if (render_param.camera == NULL) return;

	render_param.world = mAttributes->WorldXM();

	mAttributes->SetBuffers(render_param.renderType);

	for (auto& group : mGroups)
	{
		auto* material = &group.Material;

		if (material->bHasTransparency && material->fAlpha > 0)
		{
			continue;
		//	//mD3DSystem->TurnOnAlphaBlending();
		//	//mD3DSystem->TurnOnAdditiveBlending();
		}

		if (render_param.renderDeferred)
		{
			mOBJGBufferShader->Render11(
				group.IndexStart,
				group.IndexCount,
				render_param,
				material
			);
		}
		else
		{
			if (render_param.tessellate)
			{
				mTessellationShader->Render(
					group.IndexStart,
					group.IndexCount,
					render_param,
					material
				);
			}
			else
			{
				mShader->Render(
					group.IndexStart,
					group.IndexCount,
					render_param,
					material
				);
			}
		}

		//if (material->bHasTransparency && material->fAlpha > 0)
		//{
		//	//mD3DSystem->TurnOffAdditiveBlending();
		//	//mD3DSystem->TurnOffAlphaBlending();
		//}
	}
}
//===============================================================================================================================