#include "BrushTerrain.h"


BrushTerrain::BrushTerrain(int tempSize) : m_TerrainSize(tempSize)
{
}

float BrushTerrain::brushTerrain(int x_Coord, int y_Coord, float weight, int m_TerrainSize, std::vector<float>& m_TerrainData)
{
    const int BRUSH_RADIUS = 4;
    
    for (int row = -BRUSH_RADIUS; row <= BRUSH_RADIUS; ++row)
    {
        for (int col = -BRUSH_RADIUS; col <= BRUSH_RADIUS; ++col)
        {

            int index = (y_Coord + row) * m_TerrainSize + (x_Coord + col);


            if (index < 0 || index >= m_TerrainData.size())
            {
                continue;
            }

            float dx = col;
            float dy = row;
            float distance = std::sqrt(dx * dx + dy * dy);

            if (distance > BRUSH_RADIUS)
            {
                continue;
            }

            float influence = (1.0f - distance / BRUSH_RADIUS) * weight;
            m_TerrainData[index] *= (1.0f - influence);
        }

    }
}