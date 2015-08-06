#include "HeightmapContainer.h"
using ZShadeSandboxTerrain::HeightmapContainer;
//===============================================================================================================================
//===============================================================================================================================
HeightmapContainer::HeightmapContainer()
:	iHeightmapSize(0)
,	fSeaLevel(0)
{
}
//===============================================================================================================================
HeightmapContainer::HeightmapContainer(int heightmapSize, float seaLevel)
:	iHeightmapSize(heightmapSize)
,	fSeaLevel(seaLevel)
{
	Init();
}
//===============================================================================================================================
HeightmapContainer::~HeightmapContainer()
{
}
//===============================================================================================================================
void HeightmapContainer::Init()
{
	Resize();
	FlattenToSeaLevel();
}
//===============================================================================================================================
void HeightmapContainer::Clear(bool resize)
{
	if (mHeightMap.size() > 0)
	{
		mHeightMap.clear();
		
		if (resize)
		{
			Resize();
			FlattenToSeaLevel();
		}
	}
}
//===============================================================================================================================
void HeightmapContainer::Resize()
{
	mHeightMap.resize(iHeightmapSize * iHeightmapSize);
}
//===============================================================================================================================
bool HeightmapContainer::InBounds(int x, int z)
{
	return (x < iHeightmapSize && z < iHeightmapSize && x >= 0 && z >= 0);
}
//===============================================================================================================================
float HeightmapContainer::ReadHeight(int x, int z, ZShadeSandboxMath::EAxis::Type axis)
{
	if (InBounds(x, z))
	{
		switch (axis)
		{
			case ZShadeSandboxMath::EAxis::Type::Axis_X:
			{
				return mHeightMap[(z * iHeightmapSize) + x].x;
			}
			break;
			case ZShadeSandboxMath::EAxis::Type::Axis_Y:
			{
				return mHeightMap[(z * iHeightmapSize) + x].y;
			}
			break;
			case ZShadeSandboxMath::EAxis::Type::Axis_Z:
			{
				return mHeightMap[(z * iHeightmapSize) + x].z;
			}
			break;
		}
	}
	
	return 0;
}
//===============================================================================================================================
float HeightmapContainer::ReadHeight(int index, ZShadeSandboxMath::EAxis::Type axis)
{
	switch (axis)
	{
		case ZShadeSandboxMath::EAxis::Type::Axis_X:
		{
			return mHeightMap[index].x;
		}
		break;
		case ZShadeSandboxMath::EAxis::Type::Axis_Y:
		{
			return mHeightMap[index].y;
		}
		break;
		case ZShadeSandboxMath::EAxis::Type::Axis_Z:
		{
			return mHeightMap[index].z;
		}
		break;
	}
}
//===============================================================================================================================
void HeightmapContainer::UpdateHeight(int x, int z, float value, ZShadeSandboxMath::EAxis::Type axis)
{
	switch (axis)
	{
		case ZShadeSandboxMath::EAxis::Type::Axis_X:
		{
			mHeightMap[(z * iHeightmapSize) + x].x = value;
		}
		break;
		case ZShadeSandboxMath::EAxis::Type::Axis_Y:
		{
			mHeightMap[(z * iHeightmapSize) + x].y = value;
		}
		break;
		case ZShadeSandboxMath::EAxis::Type::Axis_Z:
		{
			mHeightMap[(z * iHeightmapSize) + x].z = value;
		}
		break;
	}
}
//===============================================================================================================================
void HeightmapContainer::UpdateHeight(int index, float value, ZShadeSandboxMath::EAxis::Type axis)
{
	switch (axis)
	{
		case ZShadeSandboxMath::EAxis::Type::Axis_X:
		{
			mHeightMap[index].x = value;
		}
		break;
		case ZShadeSandboxMath::EAxis::Type::Axis_Y:
		{
			mHeightMap[index].y = value;
		}
		break;
		case ZShadeSandboxMath::EAxis::Type::Axis_Z:
		{
			mHeightMap[index].z = value;
		}
		break;
	}
}
//===============================================================================================================================
void HeightmapContainer::UpdateHeight(int x, int z, ZShadeSandboxTerrain::HeightData hd)
{
	if (InBounds(x, z))
	{
		mHeightMap[(z * iHeightmapSize) + x] = hd;
	}
}
//===============================================================================================================================
void HeightmapContainer::UpdateHeight(int index, ZShadeSandboxTerrain::HeightData hd)
{
	mHeightMap[index] = hd;
}
//===============================================================================================================================
void HeightmapContainer::FlattenToSeaLevel()
{
	FillHeight(fSeaLevel);
}
//===============================================================================================================================
void HeightmapContainer::CopyHeight(HeightmapContainer hmap)
{
	for (int z = 0; z < iHeightmapSize; z++)
	{
		for (int x = 0; x < iHeightmapSize; x++)
		{
			UpdateHeight(x, z, hmap.ReadHeight(x, z, ZShadeSandboxMath::EAxis::Type::Axis_X), ZShadeSandboxMath::EAxis::Type::Axis_X);
			UpdateHeight(x, z, hmap.ReadHeight(x, z));
			UpdateHeight(x, z, hmap.ReadHeight(x, z, ZShadeSandboxMath::EAxis::Type::Axis_Z), ZShadeSandboxMath::EAxis::Type::Axis_Z);
		}
	}
}
//===============================================================================================================================
void HeightmapContainer::AddHeight(HeightmapContainer hmap)
{
	for (int z = 0; z < iHeightmapSize; z++)
	{
		for (int x = 0; x < iHeightmapSize; x++)
		{
			float currentHeight = ReadHeight(x, z);
			
			UpdateHeight(x, z, x, ZShadeSandboxMath::EAxis::Type::Axis_X);
			UpdateHeight(x, z, currentHeight + hmap.ReadHeight(x, z));
			UpdateHeight(x, z, z, ZShadeSandboxMath::EAxis::Type::Axis_Z);
		}
	}
}
//===============================================================================================================================
void HeightmapContainer::SubtractHeight(HeightmapContainer hmap)
{
	for (int z = 0; z < iHeightmapSize; z++)
	{
		for (int x = 0; x < iHeightmapSize; x++)
		{
			float currentHeight = ReadHeight(x, z);
			
			UpdateHeight(x, z, x, ZShadeSandboxMath::EAxis::Type::Axis_X);
			UpdateHeight(x, z, currentHeight - hmap.ReadHeight(x, z));
			UpdateHeight(x, z, z, ZShadeSandboxMath::EAxis::Type::Axis_Z);
		}
	}
}
//===============================================================================================================================
void HeightmapContainer::ScaleHeight(HeightmapContainer hmap)
{
	for (int z = 0; z < iHeightmapSize; z++)
	{
		for (int x = 0; x < iHeightmapSize; x++)
		{
			float currentHeight = ReadHeight(x, z);
			
			UpdateHeight(x, z, x, ZShadeSandboxMath::EAxis::Type::Axis_X);
			UpdateHeight(x, z, currentHeight * hmap.ReadHeight(x, z));
			UpdateHeight(x, z, z, ZShadeSandboxMath::EAxis::Type::Axis_Z);
		}
	}
}
//===============================================================================================================================
void HeightmapContainer::NormalizeHeight(HeightmapContainer hmap)
{
	for (int z = 0; z < iHeightmapSize; z++)
	{
		for (int x = 0; x < iHeightmapSize; x++)
		{
			float currentHeight = ReadHeight(x, z);
			
			UpdateHeight(x, z, x, ZShadeSandboxMath::EAxis::Type::Axis_X);
			
			// Avoid divide by zero error
			if (hmap.ReadHeight(x, z) == 0) continue;
			
			UpdateHeight(x, z, currentHeight / hmap.ReadHeight(x, z));
			UpdateHeight(x, z, z, ZShadeSandboxMath::EAxis::Type::Axis_Z);
		}
	}
}
//===============================================================================================================================
void HeightmapContainer::FillHeight(float height)
{
	for (int z = 0; z < iHeightmapSize; z++)
	{
		for (int x = 0; x < iHeightmapSize; x++)
		{
			UpdateHeight(x, z, x, ZShadeSandboxMath::EAxis::Type::Axis_X);
			UpdateHeight(x, z, height);
			UpdateHeight(x, z, z, ZShadeSandboxMath::EAxis::Type::Axis_Z);
		}
	}
}
//===============================================================================================================================
void HeightmapContainer::AddHeight(float height)
{
	for (int z = 0; z < iHeightmapSize; z++)
	{
		for (int x = 0; x < iHeightmapSize; x++)
		{
			float currentHeight = ReadHeight(x, z);
			
			UpdateHeight(x, z, x, ZShadeSandboxMath::EAxis::Type::Axis_X);
			UpdateHeight(x, z, currentHeight + height);
			UpdateHeight(x, z, z, ZShadeSandboxMath::EAxis::Type::Axis_Z);
		}
	}
}
//===============================================================================================================================
void HeightmapContainer::SubtractHeight(float height)
{
	for (int z = 0; z < iHeightmapSize; z++)
	{
		for (int x = 0; x < iHeightmapSize; x++)
		{
			float currentHeight = ReadHeight(x, z);
			
			UpdateHeight(x, z, x, ZShadeSandboxMath::EAxis::Type::Axis_X);
			UpdateHeight(x, z, currentHeight - height);
			UpdateHeight(x, z, z, ZShadeSandboxMath::EAxis::Type::Axis_Z);
		}
	}
}
//===============================================================================================================================
void HeightmapContainer::ScaleHeight(float height)
{
	for (int z = 0; z < iHeightmapSize; z++)
	{
		for (int x = 0; x < iHeightmapSize; x++)
		{
			float currentHeight = ReadHeight(x, z);
			
			UpdateHeight(x, z, x, ZShadeSandboxMath::EAxis::Type::Axis_X);
			UpdateHeight(x, z, currentHeight * height);
			UpdateHeight(x, z, z, ZShadeSandboxMath::EAxis::Type::Axis_Z);
		}
	}
}
//===============================================================================================================================
void HeightmapContainer::NormalizeHeight(float height)
{
	// Avoid divide by zero error
	if (height == 0) return;
	
	for (int z = 0; z < iHeightmapSize; z++)
	{
		for (int x = 0; x < iHeightmapSize; x++)
		{
			float currentHeight = ReadHeight(x, z);
			
			UpdateHeight(x, z, x, ZShadeSandboxMath::EAxis::Type::Axis_X);
			UpdateHeight(x, z, currentHeight / height);
			UpdateHeight(x, z, z, ZShadeSandboxMath::EAxis::Type::Axis_Z);
		}
	}
}
//===============================================================================================================================