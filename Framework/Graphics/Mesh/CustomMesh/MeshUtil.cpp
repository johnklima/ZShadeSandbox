#include "MeshUtil.h"
//===============================================================================================================================
//===============================================================================================================================
MeshData* MeshUtil::LoadMesh(char* filename, int vertexType)
{
	ifstream fin;
	char input;
	int i;

	// Open the model file.
	fin.open(filename);

	// If it could not open the file then exit.
	if (fin.fail())
	{
		return 0;
	}

	// Read up to the value of vertex count.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	MeshData* md = new MeshData();

	// Read in the vertex count
	fin >> md->vertexCount;

	// Set the number of indices to be the same as the vertex count.
	md->indexCount = md->vertexCount;
	
	switch (vertexType)
	{
		case ZShadeSandboxMesh::EVertexType::VT_NormalTex:
		{
			// Create the model using the vertex count that was read in.
			md->dataVNT = new ZShadeSandboxMesh::VertexNormalTex[md->vertexCount];

			if (!md->dataVNT) return 0;

			// Read up to the beginning of the data.
			fin.get(input);
			while (input != ':')
			{
				fin.get(input);
			}
			fin.get(input);
			fin.get(input);

			// Read in the vertex data.
			for (i = 0; i<md->vertexCount; i++)
			{
				fin >> md->dataVNT[i].position.x >> md->dataVNT[i].position.y >> md->dataVNT[i].position.z;
				fin >> md->dataVNT[i].texture.x >> md->dataVNT[i].texture.y;
				fin >> md->dataVNT[i].normal.x >> md->dataVNT[i].normal.y >> md->dataVNT[i].normal.z;
			}
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_NormalTexTan:
		{
			// Create the model using the vertex count that was read in.
			md->dataVNTT = new ZShadeSandboxMesh::VertexNormalTexTan[md->vertexCount];

			if (!md->dataVNTT) return 0;

			// Read up to the beginning of the data.
			fin.get(input);
			while (input != ':')
			{
				fin.get(input);
			}
			fin.get(input);
			fin.get(input);

			// Read in the vertex data.
			for (i = 0; i<md->vertexCount; i++)
			{
				fin >> md->dataVNTT[i].position.x >> md->dataVNTT[i].position.y >> md->dataVNTT[i].position.z;
				fin >> md->dataVNTT[i].texture.x >> md->dataVNTT[i].texture.y;
				fin >> md->dataVNTT[i].normal.x >> md->dataVNTT[i].normal.y >> md->dataVNTT[i].normal.z;
				fin >> md->dataVNTT[i].tangentU.x >> md->dataVNTT[i].tangentU.y >> md->dataVNTT[i].tangentU.z;
			}
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Pos:
		{
			// Create the model using the vertex count that was read in.
			md->dataPos = new ZShadeSandboxMesh::VertexPos[md->vertexCount];

			if (!md->dataPos) return 0;

			// Read up to the beginning of the data.
			fin.get(input);
			while (input != ':')
			{
				fin.get(input);
			}
			fin.get(input);
			fin.get(input);

			// Read in the vertex data.
			for (i = 0; i<md->vertexCount; i++)
			{
				fin >> md->dataPos[i].position.x >> md->dataPos[i].position.y >> md->dataPos[i].position.z;
			}
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Tex:
		{
			// Create the model using the vertex count that was read in.
			md->dataTex = new ZShadeSandboxMesh::VertexTex[md->vertexCount];

			if (!md->dataTex) return 0;

			// Read up to the beginning of the data.
			fin.get(input);
			while (input != ':')
			{
				fin.get(input);
			}
			fin.get(input);
			fin.get(input);

			// Read in the vertex data.
			for (i = 0; i<md->vertexCount; i++)
			{
				fin >> md->dataTex[i].position.x >> md->dataTex[i].position.y >> md->dataTex[i].position.z;
				fin >> md->dataTex[i].texture.x >> md->dataTex[i].texture.y;
			}
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Color:
		{
			// Create the model using the vertex count that was read in.
			md->dataColor = new ZShadeSandboxMesh::VertexColor[md->vertexCount];

			if (!md->dataColor) return 0;

			// Read up to the beginning of the data.
			fin.get(input);
			while (input != ':')
			{
				fin.get(input);
			}
			fin.get(input);
			fin.get(input);

			// Read in the vertex data.
			for (i = 0; i<md->vertexCount; i++)
			{
				fin >> md->dataColor[i].position.x >> md->dataColor[i].position.y >> md->dataColor[i].position.z;
				fin >> md->dataColor[i].color.x >> md->dataColor[i].color.y >> md->dataColor[i].color.z >> md->dataColor[i].color.w;
			}
		}
		break;
	}

	// Close the model file.
	fin.close();

	return md;
}
//===============================================================================================================================