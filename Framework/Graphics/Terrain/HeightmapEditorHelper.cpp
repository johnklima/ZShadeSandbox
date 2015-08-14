#include "HeightmapEditorHelper.h"
#include "ZShadeMessageCenter.h"
using ZShadeSandboxTerrain::HeightmapEditorHelper;
using ZShadeSandboxTerrain::Heightmap;
//===============================================================================================================================
//===============================================================================================================================
HeightmapEditorHelper::HeightmapEditorHelper()
{
	Init();
}
//===============================================================================================================================
HeightmapEditorHelper::HeightmapEditorHelper(ZShadeSandboxTerrain::Heightmap* heightmap)
:	mHeightmap(heightmap)
{
	Init();
}
//===============================================================================================================================
HeightmapEditorHelper::~HeightmapEditorHelper()
{
}
//===============================================================================================================================
void HeightmapEditorHelper::Init()
{
	fErosionFactor = 0.3f;
	fNormizingFactor = 1.0f;
	fSmoothingCount = 1.0f;
	fSeaLevel = 0;
	fFlatHeight = fSeaLevel;
	fMaxHeight = 70;
	fBrushSize = 40;
	fBrushStrength = 4;
	mBrushType = EBrushType::Type::eNone;
}
//===============================================================================================================================
void HeightmapEditorHelper::SaveToFile(const char* filename)
{
	FILE* file = fopen(filename, "wb");
	
	float height;
	
	for (int z = 0; z < mHeightmap->HeightmapSize(); z++)
	{
		for (int x = 0; x < mHeightmap->HeightmapSize(); x++)
		{
			height = mHeightmap->ReadHeight(x, z);
			
			height *= 255;
			
			fwrite(&height, sizeof(float), 1, file);
		}
	}
	
	fclose(file);
}
//===============================================================================================================================
void HeightmapEditorHelper::UpdateBrush(XMFLOAT3 point)
{
	switch (mBrushType)
	{
		case EBrushType::Type::eRaise:
		{
			Raise(point);
		}
		break;
		case EBrushType::Type::eLower:
		{
			Lower(point);
		}
		break;
		case EBrushType::Type::eFlatten:
		{
			Flatten(point);
		}
		break;
		case EBrushType::Type::eSmooth:
		{
			Smooth(point);
		}
		break;
		default:
		{
			ZShadeMessageCenter::MsgBoxError(NULL, "HeightmapEditorHelper: Brush Type not defined !!");
		}
		break;
	}
}
//===============================================================================================================================
void HeightmapEditorHelper::AddToHeightmapUpdateList(XMFLOAT3 point)
{
	if (point.x >= 0 && point.x < mHeightmap->HeightmapSize() && point.z >= 0 && point.z < mHeightmap->HeightmapSize())
	{
		// Since this is a set it does not allow duplicates
		heightmapUpdateList.insert(HeightData(point.x, point.y, point.z));
	}
	
	if (point.x + 1 >= 0 && point.x + 1 < mHeightmap->HeightmapSize() && point.z >= 0 && point.z < mHeightmap->HeightmapSize())
	{
		// Since this is a set it does not allow duplicates
		heightmapUpdateList.insert(HeightData(point.x + 1, point.y, point.z));
	}
	
	if (point.x - 1 >= 0 && point.x - 1 < mHeightmap->HeightmapSize() && point.z >= 0 && point.z < mHeightmap->HeightmapSize())
	{
		// Since this is a set it does not allow duplicates
		heightmapUpdateList.insert(HeightData(point.x - 1, point.y, point.z));
	}
	
	if (point.x >= 0 && point.x < mHeightmap->HeightmapSize() && point.z + 1 >= 0 && point.z + 1 < mHeightmap->HeightmapSize())
	{
		// Since this is a set it does not allow duplicates
		heightmapUpdateList.insert(HeightData(point.x, point.y, point.z + 1));
	}
	
	if (point.x >= 0 && point.x < mHeightmap->HeightmapSize() && point.z - 1 >= 0 && point.z - 1 < mHeightmap->HeightmapSize())
	{
		// Since this is a set it does not allow duplicates
		heightmapUpdateList.insert(HeightData(point.x, point.y, point.z - 1));
	}
}
//===============================================================================================================================
void HeightmapEditorHelper::UpdateHeightmapAfterMove()
{
	float x, y, z;
	
	for (auto i = heightmapUpdateList.begin(); i != heightmapUpdateList.end(); i++)
	{
		x = (*i).x;
		y = (*i).y;
		z = (*i).z;
		
		// Update the spot on the heightmap
		mHeightmap->UpdateHeight(x, z, y);
	}
}
//===============================================================================================================================
void HeightmapEditorHelper::MoveVertices(XMFLOAT3 point, float height)
{
	int x = (int)(point.x * mHeightmap->HeightmapSize());
	int z = (int)(point.z * mHeightmap->HeightmapSize());
	
	float dist;
	float h;
	
	for (int v = -fBrushSize; v < fBrushSize; v++)
	{
		for (int w = -fBrushSize; w < fBrushSize; w++)
		{
			ZShadeSandboxMath::XMMath2 vw(v, w);
			ZShadeSandboxMath::XMMath2 zero(0, 0);
			
			dist = zero.Distance(vw);
			
			if (dist < fBrushSize)
			{
				h = height * (fBrushSize - dist) / 2.0f;
				
				AddToHeightmapUpdateList(XMFLOAT3(x + v, h, z + w));
			}
		}
	}
	
	UpdateHeightmapAfterMove();
	
	if (heightmapUpdateList.size() > 0)
		heightmapUpdateList.clear();
}
//===============================================================================================================================
void HeightmapEditorHelper::Raise(XMFLOAT3 point)
{
	MoveVertices(point, fBrushStrength * 0.0001f);
}
//===============================================================================================================================
void HeightmapEditorHelper::Lower(XMFLOAT3 point)
{
	MoveVertices(point, -fBrushStrength * 0.0001f);
}
//===============================================================================================================================
void HeightmapEditorHelper::Flatten(XMFLOAT3 point)
{
	float heightValue = fFlatHeight / fMaxHeight;
	
	int x = (int)(point.x * mHeightmap->HeightmapSize());
	int z = (int)(point.z * mHeightmap->HeightmapSize());
	
	float dist;
	
	for (int v = -fBrushSize; v < fBrushSize; v++)
	{
		for (int w = -fBrushSize; w < fBrushSize; w++)
		{
			ZShadeSandboxMath::XMMath2 vw(v, w);
			ZShadeSandboxMath::XMMath2 zero(0, 0);
			
			dist = zero.Distance(vw);
			
			if (dist < fBrushSize)
			{
				AddToHeightmapUpdateList(XMFLOAT3(x + v, heightValue, z + w));
			}
		}
	}
	
	UpdateHeightmapAfterMove();
	
	if (heightmapUpdateList.size() > 0)
		heightmapUpdateList.clear();
}
//===============================================================================================================================
void HeightmapEditorHelper::Smooth(XMFLOAT3 point)
{
	int x = (int)(point.x * mHeightmap->HeightmapSize());
	int z = (int)(point.z * mHeightmap->HeightmapSize());
	
	float dist;
	float h;
	
	for (int v = -fBrushSize; v < fBrushSize; v++)
	{
		for (int w = -fBrushSize; w < fBrushSize; w++)
		{
			ZShadeSandboxMath::XMMath2 vw(v, w);
			ZShadeSandboxMath::XMMath2 zero(0, 0);
			
			dist = zero.Distance(vw);
			
			if (dist < fBrushSize)
			{
				float averageHeight = 0;
				float count = 0;

				for (int m = w - 1; m <= w + 1; m++)
				{
					for (int n = v - 1; n <= v + 1; n++)
					{
						if (m >= 0 && m < mHeightmap->HeightmapSize() && n >= 0 && n < mHeightmap->HeightmapSize())
						{
							averageHeight += mHeightmap->ReadHeight(m, n);
							count += 1;
						}
					}
				}
				
				h = averageHeight / count;
				
				heightmapUpdateList.insert(HeightData(x + v, h, z + w));
				heightmapUpdateList.insert(HeightData(x + v + 1, h, z + w));
				heightmapUpdateList.insert(HeightData(x + v - 1, h, z + w));
				heightmapUpdateList.insert(HeightData(x + v, h, z + w + 1));
				heightmapUpdateList.insert(HeightData(x + v, h, z + w - 1));
			}
		}
	}
	
	UpdateHeightmapAfterMove();
	
	if (heightmapUpdateList.size() > 0)
		heightmapUpdateList.clear();
}
//===============================================================================================================================
void HeightmapEditorHelper::FlattenHeightmap()
{
	mHeightmap->Flatten(fFlatHeight);
}
//===============================================================================================================================
void HeightmapEditorHelper::SmoothHeightmap()
{
	mHeightmap->Smooth(fSmoothingCount);
}
//===============================================================================================================================
void HeightmapEditorHelper::ErodeHeightmap()
{
	//mHeightmap->Erode(fErosionFactor);
}
//===============================================================================================================================
void HeightmapEditorHelper::NormalizeHeightmap()
{
	mHeightmap->Normalize(fNormizingFactor);
}
//===============================================================================================================================