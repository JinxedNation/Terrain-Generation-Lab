#include "TerrainFile.h"

TerrainFile::TerrainFile(unsigned tempSize) : Terrain(tempSize), m_TerrainData(tempSize* tempSize) {}


bool TerrainFile::createTerrain(const char* tempReadFile)
{
    std::ifstream readFile(tempReadFile, std::ios::in | std::ios::binary);
    
    
    if (!readFile.is_open())
    {
        return false;
    }

    // read the terrain data
    readFile.read(reinterpret_cast<char*>(m_TerrainData.data()), m_TerrainData.size() * sizeof(float));

    // normalize and scale the terrain data
    normalizeTerrain(m_TerrainData);
    float minHeight   = findMinimum(m_TerrainData);
    float maxHeight   = findMaximum(m_TerrainData);
    float heightRange = maxHeight - minHeight;
    scaleTerrain(m_TerrainData, minHeight, heightRange);

    getTerrainPoints();

    return true;
}


void TerrainFile::getTerrainPoints()
{
    m_TerrainVertices.reserve(m_TerrainSize * m_TerrainSize * 3);
    m_TerrainIndices.reserve((m_TerrainSize - 1) * (m_TerrainSize - 1) * 6);

    for (unsigned z_Coord = 0; z_Coord < m_TerrainSize; z_Coord++)
    {
        for (unsigned x_Coord = 0; x_Coord < m_TerrainSize; x_Coord++)
        {
            m_TerrainHeight = m_TerrainData[z_Coord * m_TerrainSize + x_Coord];

            m_TerrainVertices.push_back(static_cast<float>(x_Coord));
            m_TerrainVertices.push_back(m_TerrainHeight);
            m_TerrainVertices.push_back(static_cast<float>(z_Coord));


            if (x_Coord < m_TerrainSize - 1 && z_Coord < m_TerrainSize - 1)
            {
                unsigned topLeft = (z_Coord * m_TerrainSize) + x_Coord;
                unsigned botLeft = ((z_Coord + 1) * m_TerrainSize) + x_Coord;
                unsigned topRight = topLeft + 1;
                unsigned botRight = botLeft + 1;

                m_TerrainIndices.push_back(topLeft);
                m_TerrainIndices.push_back(botLeft);
                m_TerrainIndices.push_back(topRight);

                m_TerrainIndices.push_back(topRight);
                m_TerrainIndices.push_back(botLeft);
                m_TerrainIndices.push_back(botRight);
            }
        }
    }
    generateTexture();
}


void TerrainFile::generateTexture()
{
    m_TerrainTexture = std::make_unique<Texture>("Earth.png");
}