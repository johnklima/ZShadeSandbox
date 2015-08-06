#include "Heightmap.h"
#include "BetterString.h"
#include "CGlobal.h"
#include "HeightErosion.h"
#include "WaterErosion.h"
#include <fstream>
#include "ProceduralAdditionHelper.h"
using namespace std;
using ZShadeSandboxTerrain::Heightmap;
//===================================================================================================================
Heightmap::Heightmap(string heightmap, ZShadeSandboxTerrain::ProceduralParameters pp, float heightScale)
:	mProceduralParameters(pp)
,	fHeightScale(heightScale)
{
	if (!LoadElevation(heightmap))
	{
		// Nothing loaded so procedurally load a map
		LoadProceduralElevation();
	}
}
//===================================================================================================================
Heightmap::Heightmap(ZShadeSandboxTerrain::ProceduralParameters pp, float heightScale)
:	mProceduralParameters(pp)
,	fHeightScale(heightScale)
{
	LoadProceduralElevation();
}
//===================================================================================================================
Heightmap::Heightmap(const Heightmap& o)
{
}
//===================================================================================================================
Heightmap::~Heightmap()
{
}
//===================================================================================================================
void Heightmap::LoadProceduralElevation()
{
	// Heightmap Erosion
	// Google: simple heightmap erosion
	// http://ranmantaru.com/blog/2011/10/08/water-erosion-on-heightmap-terrain/
	// http://www.dreamincode.net/forums/blog/2250/entry-4550-terrain-erosion/
	// http://www.gamedev.net/topic/334604-erosion-on-a-heightmap/
	// Procedurally load a height map
	
	m_heightmap.HeightmapSize() = mProceduralParameters.terrainSize;
	m_heightmap.SeaLevel() = mProceduralParameters.seaLevel;
	m_heightmap.Clear(false);
	m_heightmap.Init();
	
	switch (mProceduralParameters.proceduralType)
	{
		case EProceduralType::Type::eRandom:
		{
			BuildRandomHeightmap();
		}
		break;
		case EProceduralType::Type::ePerlinNoise:
		{
			BuildPerlinNoiseHeightmap();
		}
		break;
		case EProceduralType::Type::eFieldNoise:
		{
			BuildFieldNoiseHeightmap();
		}
		break;
		case EProceduralType::Type::eDiamondSquare:
		{
			BuildDiamondSquare();
		}
		break;
	}
	
	if (mProceduralParameters.useErosion)
	{
		switch (mProceduralParameters.erosionType)
		{
			case EErosionType::Type::eHeight:
			{
				ErodeHeight(mProceduralParameters.erosionValue);
			}
			break;
			case EErosionType::Type::eWater:
			{
				ErodeWater(mProceduralParameters.waterErosionParameters);
			}
			break;
		}
	}
	
	if (mProceduralParameters.useThermalWeathering)
	{
		ThermalWeathering(mProceduralParameters.waterErosionParameters);
	}
	
	if (mProceduralParameters.useSmoothing)
	{
		Smooth(mProceduralParameters.smoothingPassCount);
	}
	
	if (mProceduralParameters.normalize)
	{
		Normalize(mProceduralParameters.normalizeFactor);
	}

	CopyToHeightMapData();
}
//===================================================================================================================
bool Heightmap::LoadElevation(string heightmap)
{
	// Auto detect the height extension
	string fileExt = CGlobal::GetFileExt(heightmap);
	
	if (fileExt == "bmp")
	{
		m_ext = EHeightExtension::Type::eBmp;
	}
	else if (fileExt == "raw")
	{
		m_ext = EHeightExtension::Type::eRaw;
	}
	else
	{
		m_ext = EHeightExtension::Type::eNone;		
		return false;
	}
	
	switch (m_ext)
	{
		case EHeightExtension::Type::eBmp:
		{
			#pragma region "Load BMP"
			FILE* file_ptr;
			int error;
			unsigned int count;
			BITMAPFILEHEADER bitmapFileHeader;
			BITMAPINFOHEADER bitmapInfoHeader;
			int imageSize, x, z;
			unsigned char* bitmapImage;
			unsigned char height;

			//Open the heightmap file in binary
			error = fopen_s(&file_ptr, heightmap.c_str(), "rb");

			if (error != 0) return false;

			//Read in the file header
			count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, file_ptr);

			if (count != 1) return false;

			//Read in the bitmap info header
			count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, file_ptr);

			if (count != 1) return false;

			// Store the size of the heightmap
			int heightmapWidth = bitmapInfoHeader.biWidth;
			int heightmapHeight = bitmapInfoHeader.biHeight;
			
			if (!ZShadeSandboxMath::ZMath::IsPowerOf2(heightmapWidth) && !ZShadeSandboxMath::ZMath::IsPowerOf2(heightmapHeight))
			{
				return false;
			}
			
			//Calculate the size of the bitmap image data
			// What if it's not 3-channel 24-bit image?...
			imageSize = heightmapWidth * heightmapHeight * 3;

			//Allocate memory for the bitmap image data
			bitmapImage = new unsigned char[imageSize];

			if (!bitmapImage) return false;

			//Move to the beginning of the bitmap data
			fseek(file_ptr, bitmapFileHeader.bfOffBits, SEEK_SET);

			//Read in the bitmap image data
			count = fread(bitmapImage, 1, imageSize, file_ptr);

			if (count != imageSize) return false;

			//Close the file
			error = fclose(file_ptr);
			if (error != 0) return false;

			//m_heightmap.resize(m_heightmap_width * m_heightmap_height);
			
			// The heightmap is square
			mProceduralParameters.terrainSize = heightmapWidth;
			
			m_heightmap.HeightmapSize() = mProceduralParameters.terrainSize;
			m_heightmap.SeaLevel() = mProceduralParameters.seaLevel;
			m_heightmap.Init();
			
			//Read the image data into the heightmap
			for (z = 0; z < mProceduralParameters.terrainSize; z++)
			{
				for (x = 0; x < mProceduralParameters.terrainSize; x++)
				{
					height = bitmapImage[3 * (x + z * mProceduralParameters.terrainSize)] * fHeightScale;

					HeightData _data;
					_data.x = x;
					_data.y = height;
					_data.z = z;

					m_heightmap.UpdateHeight(x, z, _data);
				}
			}

			//Release the bitmap image data
			delete[] bitmapImage;

			CopyToHeightMapData();

			return true;
			#pragma endregion
		}
		break;
		case EHeightExtension::eRaw:
		{
			#pragma region "Load RAW"
			vector<unsigned char> in (mProceduralParameters.terrainSize * mProceduralParameters.terrainSize);

			//Open the file
			ifstream inFile(heightmap, ios_base::binary);
			
			if (inFile)
			{
				//Read the RAW bytes
				inFile.read((char*)&in[0], (streamsize)in.size());

				//Done with file
				inFile.close();
			}

			int k = 0, index = 0;
			float height;

			HeightData _data;

			m_heightmap.HeightmapSize() = mProceduralParameters.terrainSize;
			m_heightmap.SeaLevel() = mProceduralParameters.seaLevel;
			m_heightmap.Init();

			//Copy the array data into a float array and scale it.
			for (int z = 0; z < mProceduralParameters.terrainSize; z++)
			{
				for (int x = 0; x < mProceduralParameters.terrainSize; x++)
				{
					// Indexing is probably incorrect here too

					height = (in[k++] / 255.0f) * fHeightScale;

					_data.x = x;
					_data.y = height;
					_data.z = z;

					m_heightmap.UpdateHeight(x, z, _data);
				}
			}

			CopyToHeightMapData();
			#pragma endregion
		}
		break;
		default: break;//Do not support anything else currently
	}
}
//===================================================================================================================
void Heightmap::BuildRandomHeightmap()
{
	ZShadeSandboxTerrain::ProceduralGenerator pg(mProceduralParameters);
	pg.BuildRandomHeightmap();
	m_heightmap.CopyHeight(pg.ProceduralMap());
}
//===================================================================================================================
void Heightmap::BuildPerlinNoiseHeightmap()
{
	ZShadeSandboxTerrain::ProceduralGenerator pg(mProceduralParameters);
	pg.BuildPerlinNoiseHeightmap();
	m_heightmap.CopyHeight(pg.ProceduralMap());
}
//===================================================================================================================
void Heightmap::BuildFieldNoiseHeightmap()
{
	ZShadeSandboxTerrain::ProceduralGenerator pg(mProceduralParameters);
	pg.BuildFieldNoiseHeightmap();
	m_heightmap.CopyHeight(pg.ProceduralMap());
}
//===================================================================================================================
void Heightmap::BuildDiamondSquare()
{
	ZShadeSandboxTerrain::ProceduralGenerator pg(mProceduralParameters);
	pg.BuildDiamondSquare();
	m_heightmap.CopyHeight(pg.ProceduralMap());
}
//===================================================================================================================
void Heightmap::AddRandomHeightmap()
{
	ZShadeSandboxTerrain::ProceduralAdditionHelper pah(m_heightmap, mProceduralParameters);
	pah.AddRandomHeightmap();
	m_heightmap.CopyHeight(pah.ProceduralMap());
}
//===================================================================================================================
void Heightmap::AddPerlinNoiseHeightmap()
{
	ZShadeSandboxTerrain::ProceduralAdditionHelper pah(m_heightmap, mProceduralParameters);
	pah.AddPerlinNoiseHeightmap();
	m_heightmap.CopyHeight(pah.ProceduralMap());
}
//===================================================================================================================
void Heightmap::AddFieldNoiseHeightmap()
{
	ZShadeSandboxTerrain::ProceduralAdditionHelper pah(m_heightmap, mProceduralParameters);
	pah.AddFieldNoiseHeightmap();
	m_heightmap.CopyHeight(pah.ProceduralMap());
}
//===================================================================================================================
void Heightmap::Smooth(int smoothingPassCount)
{
	for (int i = 0; i < smoothingPassCount; i++)
	{
		Smooth();
	}
}
//===================================================================================================================
void Heightmap::Smooth()
{
	for (int z = 0; z < mProceduralParameters.terrainSize; z++)
	{
		for (int x = 0; x < mProceduralParameters.terrainSize; x++)
		{
			float averageHeight = 0;
			float count = 0;

			for (int m = x - 1; m <= x + 1; m++)
			{
				for (int n = z - 1; n <= z + 1; n++)
				{
					if (m >= 0 && m < mProceduralParameters.terrainSize && n >= 0 && n < mProceduralParameters.terrainSize)
					{
						averageHeight += m_heightmap.ReadHeight(m, n);
						count += 1;
					}
				}
			}

			m_heightmap.UpdateHeight(x, z, averageHeight / count);
		}
	}
}
//===================================================================================================================
void Heightmap::Normalize(float normalizeFactor)
{
	m_heightmap.NormalizeHeight(normalizeFactor);
}
//===================================================================================================================
void Heightmap::ErodeHeight(float erosionValue)
{
	ZShadeSandboxTerrain::HeightErosion he(m_heightmap, mProceduralParameters.terrainSize, erosionValue);
	m_heightmap.CopyHeight(he.ErosionMap());
}
//===================================================================================================================
void Heightmap::ErodeWater(ZShadeSandboxTerrain::WaterErosionParameters wep)
{
	ZShadeSandboxTerrain::WaterErosion we(m_heightmap, wep, false);
	m_heightmap.CopyHeight(we.GetErosionMap());
}
//===================================================================================================================
void Heightmap::ThermalWeathering(ZShadeSandboxTerrain::WaterErosionParameters wep)
{
	ZShadeSandboxTerrain::WaterErosion we(m_heightmap, wep, true);
	m_heightmap.CopyHeight(we.GetErosionMap());
}
//===================================================================================================================
void Heightmap::Flatten(float flatHeight)
{
	m_heightmap.FillHeight(flatHeight);
}
//===================================================================================================================
void Heightmap::CopyToHeightMapData()
{
	if (mHeightData.size() > 0)
	{
		mHeightData.clear();
	}

	mHeightData.resize(mProceduralParameters.terrainSize * mProceduralParameters.terrainSize);

	for (int z = 0; z < mProceduralParameters.terrainSize; z++)
	{
		for (int x = 0; x < mProceduralParameters.terrainSize; x++)
		{
			int currentIndex = (z * mProceduralParameters.terrainSize) + x;

			mHeightData[currentIndex].x = x;
			mHeightData[currentIndex].y = m_heightmap.ReadHeight(currentIndex);
			mHeightData[currentIndex].z = z;
		}
	}
}
//===================================================================================================================
bool Heightmap::InBounds(int x, int z)
{
	return (x < mProceduralParameters.terrainSize && z < mProceduralParameters.terrainSize && x >= 0 && z >= 0);
}
//===================================================================================================================
bool Heightmap::InBounds(int index)
{
	return (index < mProceduralParameters.terrainSize * mProceduralParameters.terrainSize && index >= 0);
}
//===================================================================================================================
float Heightmap::ReadHeight(int x, int z)
{
	if (InBounds(x, z))
	{
		return mHeightData[(z * mProceduralParameters.terrainSize) + x].y;
	}
	
	return mProceduralParameters.seaLevel;
}
//===================================================================================================================
float Heightmap::ReadHeight(int index)
{
	if (InBounds(index))
	{
		return mHeightData[index].y;
	}
	
	return mProceduralParameters.seaLevel;
}
//===================================================================================================================
float Heightmap::ReadX(int x, int z)
{
	if (InBounds(x, z))
	{
		return mHeightData[(z * mProceduralParameters.terrainSize) + x].x;
	}

	return 0;
}
//===================================================================================================================
float Heightmap::ReadX(int index)
{
	if (InBounds(index))
	{
		return mHeightData[index].x;
	}

	return 0;
}
//===================================================================================================================
float Heightmap::ReadZ(int x, int z)
{
	if (InBounds(x, z))
	{
		return mHeightData[(z * mProceduralParameters.terrainSize) + x].z;
	}

	return 0;
}
//===================================================================================================================
float Heightmap::ReadZ(int index)
{
	if (InBounds(index))
	{
		return mHeightData[index].z;
	}

	return 0;
}
//===================================================================================================================