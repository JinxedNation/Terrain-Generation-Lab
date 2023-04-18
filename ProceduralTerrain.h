#pragma once

#include <cmath>

#include <vector>
#include <random>
#include <algorithm>
#include "TextureLoader.h"
#include "JPG_Loader.h"
#include "PNG_Loader.h"
#include "Terrain.h"
#include "BrushTerrain.h"


typedef struct
{
    float waterFlow;
    float sedimentRunOff;
}
LakeTypeData;


class ProceduralTerrain : public Terrain
{
    public:

        ProceduralTerrain(unsigned tempSize);
        void  generateTexture();
        void  setTerrainMaterial(const char* tempTerrainTexture);
        void  setTerrainWaterLevels(float tempLevels);

        void  getTerrainPoints();
        bool  loadTerrainData(const char* readFileName, unsigned tempSize);
        bool  createTerrain(const char* readFileName) override;
        void  generateTerrain(int width, int height, float roughnes, int numOctaves, float persistence);
        void  simulateHeightmap(int xStart, int xEnd, int yStart, int yEnd, float tempRoughness);
        void  simulateErosion(std::vector<float>& m_MapWater, std::vector<float>& m_TerrainData);
        void  scaleTerrain(float scale_X, float scale_Y, float scale_Z);
        void  brushTerrain(int x_Coord, int y_Coord, float brushWeight, std::vector<float>& m_TerrainData);
        void  erodeMaterial(int x_Coord, int y_Coord, float amount, std::vector<float>& m_TerrainData, std::vector<float>& m_ErodedTerrain);
        void  erodeTerrain(int x_Coord, int y_Coord, float amount, std::vector<float>& m_TerrainData, std::vector<float>& m_ErodedTerrain);
        void  moveErodedMaterial(int x_Coord, int y_Coord, float total, std::vector<float>& m_TerrainData, std::vector<float>& m_ErodedTerrain);
        void  runErosion(std::vector<float>& m_MapWater, std::vector<float>& m_TerrainData, int numIterations, float errodedMaterialCap, float erosionRate, float depositSpeed);
        void  checkNextNode(int x_Coord, int y_Coord, void(*callback)(int, int));
        
        void  dumpWater(int x_Coord, int y_Coord, float amount);
        void  evaporateWater(float evaporationSpeed);
        void  updateWaterLevel(float deltaT);
        void  updateCurrentWaterLevel(float waterChange);
        void  updateCurrentDebrisLevel(float debrisChange);
        void  updateSedimentLevel();
        float calculateMinimum(const std::vector<float>& m_TerrainData) const;
        float calculateMaximum(const std::vector<float>& m_TerrainData) const;
        LakeTypeData calculateFlowRate(int x_Start, int y_Start, int x_End, int y_End, const std::vector<float>& m_TerrainData, const std::vector<float>& m_MapWater, float flowRate);
        float calculateTerrainAngles(int x_Coord, int y_Coord);
        float calculateMovingMaterial(float slope, float water, float errodedMaterialCap);
        float calculateErosion(float tempSlope, float tempWater, float tempSediment);
        float calculateFolliage(float slope, float water, float errodedMaterialCap);
        float calculateWaterLevel(float waterLevel, float flowRate);
        float calculateErodedMaterial(float slope, float water, float amount);
        float calculateTerrainHeights(int x_Coord, int y_Coord) const;
        float calculateDropoff(int x_Coord, int y_Coord);
        float getDebrisLevel(float debrisLevel);
        
        void  resetTerrainNodes();


        void iterateDiamondSquare(int itrCounter, float m_TerrainRoughness);
        
        void  updateTerrain();

        float m_ScaleHeight;
        float m_ScaleWidth;
        float m_ScaleLength;

    private:
       
        
        std::vector<bool>  m_CheckedNode;
        std::vector<float> m_MapWater;
        std::vector<float> m_TerrainData;
        std::vector<float> m_ErodedTerrain;
        
        
        float m_Water;
        float m_Debris;
        float m_FlowRate;
        float m_MaxDebris;
        float m_MaxHeight;
        float m_DumpRate;
        float m_DistanceToPoint;
        float m_TerrainRoughness;
        int   m_BrushWeight;
        int   m_LakeSurface;
        int   m_LakeBed;
        int   m_LakeLeftSide;
        int   m_LakeRightSide;
        float m_ErosionRate;
        float m_WaterLevel;
        float m_DebrisLevel;
        float m_ErodedMaterialCap;
        float m_TerrainScale;
        static const int BRUSH_THICKNESS = 3.5;
        
        const float m_Rain        = 0.03f;
        const float m_Gravity     = 9.8f;
        const float m_Cohesion    = 0.1f;  
        const float m_TravelAngle = 25.0f;
        
        BrushTerrain m_BrushTerrain;
};