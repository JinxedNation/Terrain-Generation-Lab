#include "ProceduralTerrain.h"
#include <chrono>
#include <cmath>

const int erosionCount = 40;
const int BRUSH_RADIUS = 5;



ProceduralTerrain::ProceduralTerrain(unsigned tempSize) : Terrain(tempSize), m_TerrainData(m_TerrainSize* m_TerrainSize)
{
    simulateHeightmap(0, 0, m_TerrainSize - 1, m_TerrainSize - 1, m_TerrainRoughness);
    //scaleTerrain();
    getTerrainPoints();
}


bool ProceduralTerrain::loadTerrainData(const char* readFileName, unsigned tempSize)
{
    std::ifstream readFile;
    readFile.open(readFileName, std::ios::in || std::ios::binary);

    if (!readFile)
    {
        std::cerr << "Can not read the file " << readFileName << std::endl;
        
        return false;
    }

    unsigned int numBytes = tempSize * tempSize * sizeof(float);

    m_TerrainData.resize(tempSize * tempSize);

    readFile.read(reinterpret_cast<char*>(m_TerrainData.data()), numBytes);
    readFile.close();

    return true;
}


bool ProceduralTerrain::createTerrain(const char* readFileName)
{
    // Load terrain data from file
    if (!loadTerrainData(readFileName, m_TerrainSize))
    {
        return false;
    }

    // Generate terrain
    generateTerrain(width, height, roughness, numOctaves, persistence);

    // Generate texture
    generateTexture();

    return true;
}



void ProceduralTerrain::generateTerrain(int width, int height, float roughness, int numOctaves, float persistence)
{

    iterateDiamondSquare(20, m_TerrainScale);

    // Simulate erosion on the heightmap
    runErosion(m_MapWater, m_TerrainData, 100, m_ErodedMaterialCap, m_ErosionRate, 0.5f);

    // Scale the terrain to the desired size
    scaleTerrain(m_ScaleWidth, m_ScaleHeight, m_ScaleLength);
}



void ProceduralTerrain::simulateHeightmap(int xStart, int xEnd, int yStart, int yEnd, float tempRoughness)
{
    if (xEnd - xStart < 2 || yEnd - yStart < 2)
    {
        return;
    }

    int m_CentreX  = (xStart + xEnd) / 2;
    int m_CentreY  = (yStart + yEnd) / 2;
    float topLeft  = m_TerrainData[yStart * m_TerrainSize + xStart];
    float botLeft  = m_TerrainData[yStart * m_TerrainSize + xEnd];
    float topRight = m_TerrainData[yEnd   * m_TerrainSize + xStart];
    float botRight = m_TerrainData[yEnd   * m_TerrainSize + xEnd];

    float avg               = (topLeft + botLeft + topRight + botRight) / 4.0f;
    float displacement      = getRandomHeights(tempRoughness);
    m_TerrainData[m_CentreY * m_TerrainSize + m_CentreX] = avg + displacement;

    simulateHeightmap(xStart,    m_CentreX,  yStart,    m_CentreY,  tempRoughness / 2.0f);
    simulateHeightmap(m_CentreX, xEnd,       yStart,    m_CentreY,  tempRoughness / 2.0f);
    simulateHeightmap(xStart,    m_CentreX,  m_CentreY, yEnd,       tempRoughness / 2.0f);
    simulateHeightmap(m_CentreX, xEnd,       m_CentreY, yEnd,       tempRoughness / 2.0f);
}


void ProceduralTerrain::getTerrainPoints()
{
    m_TerrainVertices.reserve(m_TerrainSize      * m_TerrainSize * 3);
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
                unsigned topLeft  = ( z_Coord      * m_TerrainSize) + x_Coord;
                unsigned botLeft  = ((z_Coord + 1) * m_TerrainSize) + x_Coord;
                unsigned topRight = topLeft   + 1;
                unsigned botRight = botLeft   + 1;

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


void ProceduralTerrain::setTerrainMaterial(const char* tempTerrainTexture)
{
    std::unique_ptr<JPG_Loader> terrainTexture = std::make_unique<JPG_Loader>(tempTerrainTexture);
    terrainTexture->loadNewTexture();

    // Apply texture to the terrain
    for (int i = 0; i < m_TerrainSize * m_TerrainSize; ++i)
    {
        m_TerrainMaterial.push_back(terrainTexture);
    }
}

void ProceduralTerrain::generateTexture()
{
    if (!m_TerrainTexture)
    {
        m_TerrainTexture = std::make_unique<Texture>("Earth.png");
    }
}


void ProceduralTerrain::scaleTerrain(float scale_X, float scale_Y, float scale_Z)
{
    if (m_TerrainData.empty())
    {
        return;
    }
    m_ScaleHeight = scale_Y;
    m_ScaleWidth  = scale_Z;
    m_ScaleLength = scale_X;
}


void ProceduralTerrain::runErosion(std::vector<float>& mapWater, std::vector<float>& terrainData, int numIterations, float sedimentCapacityFactor, float erodeSpeed, float depositSpeed)
{
   std::vector<float> erodedMap(m_TerrainSize * m_TerrainSize);

    for (int i = 0; i < numIterations; ++i)
    {
        for (unsigned y_Coord = 0; y_Coord < m_TerrainSize; ++y_Coord)
        {
            for (unsigned x_Coord = 0; x_Coord < m_TerrainSize; ++x_Coord)
            {
                int index = y_Coord * m_TerrainSize + x_Coord;

                // Apply brush effect to waterMap
                float brushValue = m_BrushTerrain.brushTerrain(x_Coord, y_Coord, m_BrushWeight, m_TerrainSize, m_TerrainData);

                if (brushValue > 0.0f)
                {
                    brushTerrain(x_Coord, y_Coord, brushValue, m_MapWater);
                }

                // Calculate slope and sediment capacity
                float slope      = calculateTerrainAngles(x_Coord, y_Coord);
                float water      = mapWater[index];
                float depositCap = calculateMovingMaterial(slope, water, m_ErodedMaterialCap);

                // Calculate erosion and deposition
                float sediment = m_ErodedTerrain[index];
                float erodeAmount = std::min(erodeSpeed * depositCap, sediment);
                float depositAmount = std::min(depositSpeed * (sediment - depositCap), -sediment);
                sediment -= erodeAmount + depositAmount;

                // Update heightmap and sedimentmap
                erodeMaterial(x_Coord, y_Coord, erodeAmount, m_TerrainData, m_ErodedTerrain);
                moveErodedMaterial(x_Coord, y_Coord, depositAmount, m_TerrainData, m_ErodedTerrain);
                // Update water level
                
                m_MapWater[index] += m_Rain - erodeAmount;

                if (m_MapWater[index] < 0.0f)
                { 
                   m_MapWater[index] = 0.0f;
                }
            }
        }
        // Update sediment level
        updateSedimentLevel();
    }
}


void ProceduralTerrain::simulateErosion(std::vector<float>& mapWater, std::vector<float>& terrainData)
{
    int   iterateErosion = 18;
    float erosionRate    = 4.5f;
    float depositRate    = 0.2f;
    float groupSize      = 4.5f;

    std::vector<float> erodedMap(m_TerrainSize * m_TerrainSize);

    for (int erosionItr = 0; erosionItr < iterateErosion; ++erosionItr)
    {
        for (unsigned y_Group = 0; y_Group < m_TerrainSize / groupSize; ++y_Group)
        {
            for (unsigned x_Group = 0; x_Group < m_TerrainSize / groupSize; ++x_Group)
            {
                int sourceY = y_Group * groupSize;
                int sourceX = x_Group * groupSize;
                int destY   = sourceY + groupSize;
                int destX   = x_Group + groupSize;


                for (int y_Coord = sourceY; y_Coord < destY; ++y_Coord)
                {
                    for (int x_Coord = sourceX; x_Coord < destX; ++x_Coord)
                    {
                        erodeTerrain(x_Coord, y_Coord, erosionRate, m_TerrainData, m_ErodedTerrain);
                    }
                }
            }
        }
    }
}


float getRandomHeights(float tempRoughness)
{
    static std::default_random_engine generateRandomPoints(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<float> distribution(-tempRoughness, tempRoughness);


    return distribution(generateRandomPoints);
}


void ProceduralTerrain::brushTerrain(int x_Coord, int y_Coord, float erosionStrength, std::vector<float>& m_TerrainData)
{
    const int startRow    = -BRUSH_THICKNESS;
    const int endRow      =  BRUSH_THICKNESS;
    const int startCol    = -BRUSH_THICKNESS;
    const int endCol      =  BRUSH_THICKNESS;
    const int terrainSize =  m_TerrainData.size();

    for (int row = startRow; row <= endRow; ++row)
    {
        int rowOffset = row * m_TerrainSize;

        for (int col = startCol; col <= endCol; ++col)
        {
            int index = (y_Coord + row) * m_TerrainSize + (x_Coord + col);


            if (index < 0 || index >= terrainSize)
            {
                continue;
            }

            float dx = col;
            float dy = row;
            float distance = std::sqrt(dx * dx + dy * dy);

            if (distance > BRUSH_THICKNESS)
            {
                continue;
            }

            float influence = (1.0f - distance / BRUSH_THICKNESS) * erosionStrength;
            m_TerrainData[index] *= (1.0f - influence);
        }
    }
}


void ProceduralTerrain::erodeMaterial(int x_Coord, int y_Coord, float amount, std::vector<float>&m_TerrainData, std::vector<float>& m_ErodedTerrain)
{
    int index              = x_Coord + y_Coord * m_TerrainSize;
    float totalMaterial    = m_TerrainData[index]   + m_ErodedTerrain[index];
    float newMaterial      = std::max(totalMaterial - amount, 0.0f);
    m_ErodedTerrain[index] = newMaterial            - m_TerrainData[index];
    m_TerrainData[index]   = std::max(newMaterial, 0.0f);
}


void ProceduralTerrain::erodeTerrain(int tempX_Coord, int tempY_Coord, float amount, std::vector<float>& m_TerrainData, std::vector<float>& m_ErodedTerrain)
{
    const float maxWater = 1.0f;

    if (tempX_Coord < 0 || tempX_Coord >= m_TerrainSize || tempY_Coord < 0 || tempY_Coord >= m_TerrainSize)
    {
        return;
    }

    int index     = tempY_Coord * m_TerrainSize + tempX_Coord;
    float slope   = calculateTerrainAngles(tempX_Coord, tempY_Coord);
    float water   = (m_ErodedTerrain[index] < maxWater) ? m_ErodedTerrain[index] : maxWater;
    float deposit = calculateErodedMaterial(slope, water, amount);

    moveErodedMaterial(tempX_Coord, tempY_Coord, deposit, m_TerrainData, m_ErodedTerrain);

    m_ErodedTerrain[index] = std::max(m_ErodedTerrain[index] - deposit + amount, 0.0f);
    m_TerrainData[index]  -= amount;
}


void ProceduralTerrain::moveErodedMaterial(int x_Coord, int y_Coord, float total, std::vector<float>& m_TerrainData, std::vector<float>& m_ErodedTerrain)
{
    int index         = x_Coord + y_Coord    * m_TerrainSize;
    float totalHeight = m_TerrainData[index] + m_ErodedTerrain[index];

    if (totalHeight <= m_TerrainData[index + 1])
    {
        float movedTerrain = (m_TerrainData[index + 1] - totalHeight) / 2.0f;

        m_ErodedTerrain[index]     -= total * movedTerrain;
        m_ErodedTerrain[index + 1] += total * movedTerrain;
    }
}


void ProceduralTerrain::dumpWater(int x_Coord, int y_Coord, float amount)
{
    m_MapWater[x_Coord + y_Coord * m_TerrainSize] += amount;
}


void ProceduralTerrain::evaporateWater(float evaporationSpeed)
{
    const float tolerance = 1e-6f;
    for (float& m_Water : m_MapWater)
    {
        m_Water -= m_Water * evaporationSpeed;

        if (m_Water < tolerance)
        {
            m_Water = 0.0f;
        }
    }
}


void ProceduralTerrain::checkNextNode(int x_Coord, int y_Coord, void(*callback)(int, int))
{
    const int terrainSize = m_TerrainSize;

    for (int next_Y = y_Coord - 1; next_Y <= y_Coord + 1; ++next_Y)
    {
        for (int next_X = x_Coord - 1; next_X <= x_Coord + 1; ++next_X)
        {
            // Skip cells outside the terrain or outside the lake bounds
            if (next_X < 0 || next_X >= terrainSize || next_X < m_LakeLeftSide || next_X > m_LakeRightSide || next_Y < 0 || next_Y >= terrainSize || next_Y < m_LakeSurface || next_Y > m_LakeBed)
            {
                continue;
            }
            callback(next_X, next_Y);
        }
    }
}


float ProceduralTerrain::getDebrisLevel(float debrisLevel)
{
    return debrisLevel;
}


float ProceduralTerrain::calculateMinimum(const std::vector<float>& m_TerrainData) const
{
    if (m_TerrainData.empty())
    {
        return 0.0f;
    }

    float minValue = m_TerrainData.front();

    for (unsigned i = 0; i < m_TerrainData.size() - 1; i += 2)
    {
        float val1 = m_TerrainData[i];
        float val2 = m_TerrainData[i + 1];

        if (val1 < val2)
        {
            minValue = std::min(minValue, val1);
        }
        else
        {
            minValue = std::min(minValue, val2);
        }
    }

    // If the size of the vector is odd, we need to compare the last value separately
    if (m_TerrainData.size() % 2 == 1)
    {
        minValue = std::min(minValue, m_TerrainData.back());
    }
    return minValue;
}


float ProceduralTerrain::calculateMaximum(const std::vector<float>& m_TerrainData) const
{
    if (m_TerrainData.empty())
    {
        return 0.0f;
    }

    float maxValue = m_TerrainData.front();

    for (unsigned i = 0; i < m_TerrainData.size() - 1; i += 2)
    {
        float val1 = m_TerrainData[i];
        float val2 = m_TerrainData[i + 1];

        if (val1 > val2)
        {
            maxValue = std::max(maxValue, val1);
        }
        else
        {
            maxValue = std::max(maxValue, val2);
        }
    }
    // If the size of the vector is odd, we need to compare the last value separately
    if (m_TerrainData.size() % 2 == 1)
    {
        maxValue = std::max(maxValue, m_TerrainData.back());
    }

    return maxValue;
}


float ProceduralTerrain::calculateTerrainAngles(int x_Coord, int y_Coord)
{
    float height = m_TerrainData[y_Coord * m_TerrainSize + x_Coord];

    // Calculate differences in height with neighbouring terrain
    float dx = (m_TerrainData[y_Coord * m_TerrainSize + (x_Coord + 1)] - height) / m_DistanceToPoint;
    float dy = (m_TerrainData[(y_Coord + 1) * m_TerrainSize + x_Coord] - height) / m_DistanceToPoint;

    // Calculate slope using Pythagorean theorem
    return sqrt(dx * dx + dy * dy);
}

float ProceduralTerrain::calculateErosion(float tempSlope, float tempWater, float tempSediment)
{
    float totalMaterial = std::max((tempWater - 0.01f) * (tempSlope + 0.01f), 0.0f);

    return std::min(totalMaterial - tempSediment, tempSediment);
}


float ProceduralTerrain::calculateFolliage(float slope, float water, float folliageCap)
{
    return slope * water * folliageCap;
}


float ProceduralTerrain::calculateWaterLevel(float waterLevel, float flowRate)
{
    return waterLevel - flowRate;
}


/// calculate currentErrodingMaterial
float ProceduralTerrain::calculateMovingMaterial(float angle, float water, float errodedMaterialCap)
{
    float sedimentCapacity = pow(angle, errodedMaterialCap) * water;

    return sedimentCapacity;
}


LakeTypeData ProceduralTerrain::calculateFlowRate(int x_Start, int y_Start, int x_End, int y_End, const std::vector<float>& m_TerrainData, const std::vector<float>& m_MapWater, float flowRate)
{
    int   m_StartLocation = x_Start + y_Start * m_TerrainSize;
    int   m_Destination   = x_End   + y_End   * m_TerrainSize;
    float heightStart     = m_TerrainData[m_StartLocation] + m_MapWater[m_StartLocation];
    float heightEnd       = m_TerrainData[m_Destination]   + m_MapWater[m_Destination];
    float heightDiff      = heightStart - heightEnd;
    float waterFlow       = 0.0f;
    float sedimentRunOff  = 0.0f;

    if (heightDiff > 0.0f)
    {
        waterFlow = heightDiff * flowRate;

        sedimentRunOff = waterFlow * m_ErodedTerrain[m_StartLocation];
        m_WaterLevel  += waterFlow;
        m_DebrisLevel += sedimentRunOff;
    }
    return { waterFlow, sedimentRunOff };
}


float ProceduralTerrain::calculateErodedMaterial(float fallAngle, float water, float amount)
{
    // Calculate sediment capacity based on the current water level and slope
    float erodedMaterialCap = (fallAngle * water * m_Gravity) / m_Cohesion;
    float dumpMaterial      = std::min(amount, erodedMaterialCap - water);

    float erosionAngle      = std::max(0.0f, static_cast<float>(std::cos(m_Gravity * std::acos(-1.0) / 180.0)) - fallAngle);
    float cohesionFactor    = m_Cohesion / (water + dumpMaterial);
    float dumpedMaterial    = dumpMaterial * erosionAngle * cohesionFactor;

    return dumpedMaterial;
}


void ProceduralTerrain::updateSedimentLevel()
{
    const float depositionRate = 0.05f;
    const float dissolvingRate = 0.01f;

    for (unsigned i = 0; i < m_TerrainSize * m_TerrainSize; ++i)
    {
        if (m_ErodedTerrain[i] > 0.0f)
        {
            float foliage = m_ErodedTerrain[i];
            float height = m_TerrainData[i];

            // Deposit sediment
            float depositionAmount = depositionRate * foliage;
            m_ErodedTerrain[i]    -= depositionAmount;
            m_TerrainData[i]      += depositionAmount;

            // Dissolve sediment
            float dissolvingAmount = dissolvingRate * (height - foliage);
            m_ErodedTerrain[i]    += dissolvingAmount;
            m_TerrainData[i]      -= dissolvingAmount;

            // Ensure that terrain height is non-negative
            m_TerrainData[i] = std::max(0.0f, m_TerrainData[i]);
        }
    }
}

void ProceduralTerrain::updateCurrentWaterLevel(float waterChange)
{
    // Update lake water
    m_Water += waterChange;

    // Constrain lake water to non-negative value
    m_Water = std::max(m_Water, 0.0f);
}


// force lake sediment to non-negative value
void ProceduralTerrain::updateCurrentDebrisLevel(float debrisChange)
{
    m_Debris += debrisChange;
    m_Debris = std::max(m_Debris, 0.0f);
}



void ProceduralTerrain::updateWaterLevel(float deltaT)
{
    // Calculate outflow from the lake
    float outflow = m_Water * m_FlowRate * deltaT;

    // Iterate over the lake cells
    for (int y_Coord = m_LakeSurface; y_Coord <= m_LakeBed; ++y_Coord)
    {
        for (int x_Coord = m_LakeLeftSide; x_Coord <= m_LakeRightSide; ++x_Coord)
        {

            // Skip land cells
            if (m_ErodedTerrain[x_Coord + y_Coord * m_TerrainSize] > 0.0f)
            {
                continue;
            }

            // Calculate flow rate from neighboring cells
            float waterIn    = 0.0f;
            float sedimentIn = 0.0f;

            // Calculate lake inflow
            calculateFlowRate(x_Coord, y_Coord, x_Coord + 1, y_Coord, m_TerrainData, m_MapWater, m_FlowRate);

            // Calculate change in lake water and sediment
            float new_WaterLevel = calculateWaterLevel(waterIn, outflow);
            float foliageLevel   = getDebrisLevel(sedimentIn);

            // Update lake water and sediment
            updateCurrentWaterLevel(new_WaterLevel);
            updateCurrentDebrisLevel(foliageLevel);
        }
    }
    // Reset cells considered for next iteration
    resetTerrainNodes();
}



void ProceduralTerrain::resetTerrainNodes()
{
    std::fill(m_CheckedNode.begin(), m_CheckedNode.end(), false);
}


float ProceduralTerrain::calculateTerrainHeights(int x_Coord, int y_Coord) const
{
    int points = y_Coord * m_TerrainSize + x_Coord;
    return m_TerrainData[points] + m_MapWater[points];
}


float ProceduralTerrain::calculateDropoff(int temp_XCoord, int temp_YCoord)
{
    float topLeft     = calculateTerrainHeights(temp_XCoord - 1, temp_YCoord - 1);
    float top         = calculateTerrainHeights(temp_XCoord,     temp_YCoord - 1);
    float topRight    = calculateTerrainHeights(temp_XCoord + 1, temp_YCoord - 1);
    float left        = calculateTerrainHeights(temp_XCoord - 1, temp_YCoord);
    float right       = calculateTerrainHeights(temp_XCoord + 1, temp_YCoord);
    float bottomLeft  = calculateTerrainHeights(temp_XCoord - 1, temp_YCoord + 1);
    float bottom      = calculateTerrainHeights(temp_XCoord,     temp_YCoord + 1);
    float bottomRight = calculateTerrainHeights(temp_XCoord + 1, temp_YCoord + 1);

    float dX = ((topRight   + 2 * right + bottomRight)  - (topLeft + 2 * left + bottomLeft)) / (8.0f * m_TerrainScale);
    float dY = ((bottomLeft + 2 * bottom + bottomRight) - (topLeft + 2 * top  + topRight))   / (8.0f * m_TerrainScale);

    float slope = std::sqrt(dX * dX + dY * dY);
    return slope;
}




void ProceduralTerrain::iterateDiamondSquare(int numItrations, float tempScale)
{
    int terrainSize = m_TerrainSize - 1;
    int destination = terrainSize;

    for (int runCount = 0; runCount < numItrations; runCount++)
    {
        int halfwayToDestination = destination / 2;
        bool offset = false;


        for (int y_Coord = 0; y_Coord < terrainSize; y_Coord += halfwayToDestination)
        {
            int originX = offset;
            int x_Coord = (y_Coord + halfwayToDestination) % destination;

            for (x_Coord; x_Coord < terrainSize; x_Coord += destination)
            {
                int centralPoint  = x_Coord  + y_Coord * m_TerrainSize;
                int topLeftPoint  = (x_Coord - halfwayToDestination) + (y_Coord - halfwayToDestination) * m_TerrainSize;
                int topRightPoint = (x_Coord + halfwayToDestination) + (y_Coord - halfwayToDestination) * m_TerrainSize;
                int botLeftPoint  = (x_Coord - halfwayToDestination) + (y_Coord + halfwayToDestination) * m_TerrainSize;
                int botRightPoint = (x_Coord + halfwayToDestination) + (y_Coord + halfwayToDestination) * m_TerrainSize;

                float diamondCentre = (m_TerrainData[topLeftPoint] + m_TerrainData[topRightPoint] + m_TerrainData[botLeftPoint] + m_TerrainData[botRightPoint]) * 0.25f;
                diamondCentre      += getRandomHeights(tempScale);

                m_TerrainData[centralPoint] = diamondCentre;


                if (y_Coord == halfwayToDestination)
                {
                    m_TerrainData[topRightPoint] = (m_TerrainData[topLeftPoint] + m_TerrainData[centralPoint] + m_TerrainData[botRightPoint]) * 3.0f + getRandomHeights(tempScale);
                }

                if (x_Coord == 0)
                {
                    m_TerrainData[botLeftPoint] = (m_TerrainData[topLeftPoint] + m_TerrainData[centralPoint] + m_TerrainData[botRightPoint]) * 3.0f + getRandomHeights(tempScale);
                }

                if (y_Coord == terrainSize - halfwayToDestination)
                {
                    m_TerrainData[botRightPoint] = (m_TerrainData[topRightPoint] + m_TerrainData[centralPoint] + m_TerrainData[botLeftPoint]) * 3.0f + getRandomHeights(tempScale);
                }

                if (x_Coord == terrainSize - halfwayToDestination)
                {
                    m_TerrainData[topLeftPoint] = (m_TerrainData[topRightPoint] + m_TerrainData[centralPoint] + m_TerrainData[botLeftPoint]) * 3.0f + getRandomHeights(tempScale);
                }
            }
            offset = !offset;
        }


        for (int y_Coord = 0; y_Coord < terrainSize; y_Coord += halfwayToDestination)
        {


            int botRightPoint = (y_Coord + destination) * terrainSize - 1;


            for (int x_Coord = 0; x_Coord < terrainSize; x_Coord += halfwayToDestination)
            {
                int centralPoint = x_Coord + halfwayToDestination + (y_Coord + halfwayToDestination) * m_TerrainSize;
                int topLeftPoint = x_Coord + y_Coord * m_TerrainSize;
                int botLeftPoint = x_Coord + (y_Coord + destination) * m_TerrainSize;
                int topRightPoint = (x_Coord + destination) + y_Coord * m_TerrainSize;

                float squareCentre = (m_TerrainData[topLeftPoint] + m_TerrainData[topRightPoint] + m_TerrainData[botLeftPoint] + m_TerrainData[botRightPoint]) * 0.25f;
                squareCentre += getRandomHeights(tempScale);

                m_TerrainData[centralPoint] = squareCentre;

                if (x_Coord == 0)
                {
                    m_TerrainData[topLeftPoint] = (m_TerrainData[topRightPoint] + m_TerrainData[botLeftPoint] + squareCentre) / 3.0f + getRandomHeights(tempScale);
                }

                if (y_Coord == 0)
                {
                    m_TerrainData[topRightPoint] = (m_TerrainData[topLeftPoint] + m_TerrainData[botRightPoint] + squareCentre) / 3.0f + getRandomHeights(tempScale);
                }

                if (x_Coord == terrainSize - halfwayToDestination)
                {
                    m_TerrainData[botRightPoint] = (m_TerrainData[botLeftPoint] + m_TerrainData[topRightPoint] + squareCentre) / 3.0f + getRandomHeights(tempScale);
                }

                if (y_Coord == terrainSize - halfwayToDestination)
                {
                    m_TerrainData[botLeftPoint] = (m_TerrainData[botRightPoint] + m_TerrainData[topLeftPoint] + squareCentre) / 3.0f + getRandomHeights(tempScale);
                }
            }
        }
    }
}
      

