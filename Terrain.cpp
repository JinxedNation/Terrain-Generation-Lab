#include "Terrain.h"

Terrain::Terrain(unsigned tempSize) : m_TerrainSize(tempSize), m_TerrainVAO(0), m_TerrainEBO(0), m_isTexturedTerrain(false)
{
}


float Terrain::findMinimum(const std::vector<float>& m_TerrainData) const
{
	float minPoint = std::numeric_limits<float>::max();


	for (auto height : m_TerrainData)
	{
		if (height < minPoint)
		{
			minPoint = height;
		}
	}

	return minPoint;
}


float Terrain::findMaximum(const std::vector<float>& m_TerrainData) const
{
	float maxPoint = std::numeric_limits<float>::min();
	
	for (auto height : m_TerrainData) 
	{
		if (height > maxPoint)
		{
			maxPoint = height;
		}
	}
	return maxPoint;
}


void Terrain::normalizeTerrain(std::vector<float>& m_TerrainData)
{
	float minValue = *std::min_element(m_TerrainData.begin(), m_TerrainData.end());
	float maxValue = *std::max_element(m_TerrainData.begin(), m_TerrainData.end());

	float scaleFactor = 1.0f / (maxValue - minValue);

	std::transform(m_TerrainData.begin(), m_TerrainData.end(), m_TerrainData.begin(), [&](float tempValue) -> float 
	{
		return (tempValue - minValue) * scaleFactor;
	});
}


void Terrain::scaleTerrain(std::vector<float>& m_TerrainData, float tempMinHeight, float tempHeightRange)
{
	std::transform(m_TerrainData.begin(), m_TerrainData.end(), m_TerrainData.begin(), [&](float tempData) -> float
	{
		return tempMinHeight + tempData * tempHeightRange;
	});
}

void Terrain::draw()
{
	glBindVertexArray(m_TerrainVAO);
	glDrawElements(GL_TRIANGLES, m_TerrainIndices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}