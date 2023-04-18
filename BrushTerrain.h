#pragma once
#include "Terrain.h"
#include "TerrainFile.h"
#include "ProceduralTerrain.h"
#include <vector>


class BrushTerrain
{
    public:
        BrushTerrain();
        BrushTerrain(Terrain* terrain, int tempSize);
        float brushTerrain(int x_Coord, int y_Coord, float weight, int m_TerrainSize, std::vector<float>& m_TerrainData);
  
    private:
        Terrain& m_Terrain;
        float    m_BrushRadius;
        float    m_BrushWeight;

};