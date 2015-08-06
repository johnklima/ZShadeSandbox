#include "Vertex.h"
//===============================================================================================================================
D3D11_INPUT_ELEMENT_DESC ZShadeSandboxMesh::VertexLayout::mesh_layout_pos[1] =
{
	{ "position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
//===============================================================================================================================
D3D11_INPUT_ELEMENT_DESC ZShadeSandboxMesh::VertexLayout::mesh_layout_pos_instance[2] =
{
	{ "position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "INSTANCEPOS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
};
//===============================================================================================================================
//===============================================================================================================================
D3D11_INPUT_ELEMENT_DESC ZShadeSandboxMesh::VertexLayout::mesh_layout_pos_color[2] =
{
	{ "position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};
//===============================================================================================================================
D3D11_INPUT_ELEMENT_DESC ZShadeSandboxMesh::VertexLayout::mesh_layout_pos_color_instance[3] =
{
	{ "position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "INSTANCEPOS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
};
//===============================================================================================================================
//===============================================================================================================================
D3D11_INPUT_ELEMENT_DESC ZShadeSandboxMesh::VertexLayout::mesh_layout_pos_tex[2] =
{
	{ "position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};
//===============================================================================================================================
D3D11_INPUT_ELEMENT_DESC ZShadeSandboxMesh::VertexLayout::mesh_layout_pos_tex_instance[3] =
{
	{ "position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "INSTANCEPOS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
};
//===============================================================================================================================
//===============================================================================================================================
D3D11_INPUT_ELEMENT_DESC ZShadeSandboxMesh::VertexLayout::mesh_layout_pos_normal_tex[3] =
{
	{ "position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};
//===============================================================================================================================
D3D11_INPUT_ELEMENT_DESC ZShadeSandboxMesh::VertexLayout::mesh_layout_pos_normal_tex_instance[4] =
{
	{ "position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "INSTANCEPOS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
};
//===============================================================================================================================
//===============================================================================================================================
D3D11_INPUT_ELEMENT_DESC ZShadeSandboxMesh::VertexLayout::mesh_layout_pos_normal_tex_tan[4] =
{
	{ "position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};
//===============================================================================================================================
D3D11_INPUT_ELEMENT_DESC ZShadeSandboxMesh::VertexLayout::mesh_layout_pos_normal_tex_tan_instance[5] =
{
	{ "position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "INSTANCEPOS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
};
//===============================================================================================================================
//===============================================================================================================================
D3D11_INPUT_ELEMENT_DESC ZShadeSandboxMesh::VertexLayout::mesh_layout_pos_normal_tex_tan_bitan[5] =
{
	{ "position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};
//===============================================================================================================================
D3D11_INPUT_ELEMENT_DESC ZShadeSandboxMesh::VertexLayout::mesh_layout_pos_normal_tex_tan_bitan_instance[6] =
{
	{ "position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "INSTANCEPOS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
};
//===============================================================================================================================
//===============================================================================================================================
D3D11_INPUT_ELEMENT_DESC ZShadeSandboxMesh::VertexLayout::mesh_layout_sprite_pos_tex[2] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};
//===============================================================================================================================
//===============================================================================================================================
D3D11_INPUT_ELEMENT_DESC ZShadeSandboxMesh::VertexLayout::mesh_layout_sprite_instanced[8] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TRANSFORM", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	{ "TRANSFORM", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	{ "TRANSFORM", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	{ "TRANSFORM", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 64, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	{ "SOURCERECT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 80, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
};
//===============================================================================================================================
//===============================================================================================================================
ZShadeSandboxMesh::InstancePos ZShadeSandboxMesh::VertexUtil::LoadInstancePos(float x, float y, float z)
{
	InstancePos vert;
	vert.position = XMFLOAT3(x, y, z);
	return vert;
}
//===============================================================================================================================
ZShadeSandboxMesh::VertexPos ZShadeSandboxMesh::VertexUtil::LoadVertexPos(float x, float y, float z)
{
	VertexPos vert;
	vert.position = XMFLOAT3(x, y, z);
	return vert;
}
//===============================================================================================================================
ZShadeSandboxMesh::VertexColor ZShadeSandboxMesh::VertexUtil::LoadVertexColor(float x, float y, float z, float r, float g, float b, float a)
{
	VertexColor vert;
	vert.position = XMFLOAT3(x, y, z);
	vert.color = XMFLOAT4(r, g, b, a);
	return vert;
}
//===============================================================================================================================
ZShadeSandboxMesh::VertexTex ZShadeSandboxMesh::VertexUtil::LoadVertexTex(float x, float y, float z, float u, float v)
{
	VertexTex vert;
	vert.position = XMFLOAT3(x, y, z);
	vert.texture = XMFLOAT2(u, v);
	return vert;
}
//===============================================================================================================================
ZShadeSandboxMesh::VertexNormalTex ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(float x, float y, float z, float nx, float ny, float nz, float u, float v)
{
	VertexNormalTex vert;
	vert.position = XMFLOAT3(x, y, z);
	vert.normal = XMFLOAT3(nx, ny, nz);
	vert.texture = XMFLOAT2(u, v);
	return vert;
}
//===============================================================================================================================
ZShadeSandboxMesh::VertexNormalTexTan ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(float x, float y, float z, float nx, float ny, float nz, float u, float v, float tanx, float tany, float tanz)
{
	VertexNormalTexTan vert;
	vert.position = XMFLOAT3(x, y, z);
	vert.normal = XMFLOAT3(nx, ny, nz);
	vert.texture = XMFLOAT2(u, v);
	vert.tangentU = XMFLOAT3(tanx, tany, tanz);
	return vert;
}
//===============================================================================================================================
ZShadeSandboxMesh::VertexNormalTexTanBiTan ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(float x, float y, float z, float nx, float ny, float nz, float u, float v, float tanx, float tany, float tanz, float tanw, float btanx, float btany, float btanz)
{
	VertexNormalTexTanBiTan vert;
	vert.position = XMFLOAT3(x, y, z);
	vert.normal = XMFLOAT3(nx, ny, nz);
	vert.texture = XMFLOAT2(u, v);
	vert.tangent = XMFLOAT4(tanx, tany, tanz, tanw);
	vert.bitangent = XMFLOAT3(btanx, btany, btanz);
	return vert;
}
//===============================================================================================================================
void ZShadeSandboxMesh::VertexUtil::InitVertex(ZShadeSandboxMesh::InstancePos& v)
{
	v.position = XMFLOAT3(0.0f, 0.0f, 0.0f);
}
//===============================================================================================================================
void ZShadeSandboxMesh::VertexUtil::InitVertex(ZShadeSandboxMesh::VertexPos& v)
{
	v.position = XMFLOAT3(0.0f, 0.0f, 0.0f);
}
//===============================================================================================================================
void ZShadeSandboxMesh::VertexUtil::InitVertex(ZShadeSandboxMesh::VertexColor& v)
{
	v.position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	v.color = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
}
//===============================================================================================================================
void ZShadeSandboxMesh::VertexUtil::InitVertex(ZShadeSandboxMesh::VertexTex& v)
{
	v.position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	v.texture = XMFLOAT2(0.0f, 0.0f);
}
//===============================================================================================================================
void ZShadeSandboxMesh::VertexUtil::InitVertex(ZShadeSandboxMesh::VertexNormalTex& v)
{
	v.position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	v.normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	v.texture = XMFLOAT2(0.0f, 0.0f);
}
//===============================================================================================================================
void ZShadeSandboxMesh::VertexUtil::InitVertex(ZShadeSandboxMesh::VertexNormalTexTan& v)
{
	v.position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	v.normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	v.texture = XMFLOAT2(0.0f, 0.0f);
	v.tangentU = XMFLOAT3(0.0f, 0.0f, 0.0f);
}
//===============================================================================================================================
void ZShadeSandboxMesh::VertexUtil::InitVertex(ZShadeSandboxMesh::VertexNormalTexTanBiTan& v)
{
	v.position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	v.normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	v.texture = XMFLOAT2(0.0f, 0.0f);
	v.tangent = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	v.bitangent = XMFLOAT3(0.0f, 0.0f, 0.0f);
}
//===============================================================================================================================