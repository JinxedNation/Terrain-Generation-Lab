#pragma once
#include "Terrain.h"

class TerrainFile : public Terrain
{
    public:
        TerrainFile(unsigned tempSize);
        virtual ~TerrainFile() = default;

        virtual bool createTerrain(const char* tempReadFile) override;


    protected:
        
        virtual void generateTexture()  override;
        virtual void getTerrainPoints() override;

    private:
        std::vector<float> m_TerrainData;
        float m_MinTerrainHeight;
        float m_MaxTerrainHeight;
        float m_HeightLevels;
        float m_TerrainHeight;
};