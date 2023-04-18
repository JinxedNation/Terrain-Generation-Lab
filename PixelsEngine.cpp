#include "PixelsEngine.h"
#include "PlayerCamera.h"   
#include "API_Factory.h"

PixelsEngine::PixelsEngine() : m_PixelsWindow(nullptr), m_PlayerCamera(nullptr), m_GraphicsAPI(nullptr), m_FactoryAPI() {}



void PixelsEngine::runEngine() 
{
    loadGameWindow(m_WindowWidth, m_WindowHeight, m_ProjectTitle);
    loadGraphicsAPI();
    loadPlayerCamera(m_SpawnLocation);
    setWindowCallbacks();
    loadTerrain();
    configureTerrain();
    m_PixelsWindow->launchGame([this] { drawFrame(); });
}


void PixelsEngine::loadGameWindow(unsigned tempWindowWidth, unsigned tempWindowHeight, const char* GameName)
{
    m_WindowWidth  = tempWindowWidth;
    m_WindowHeight = tempWindowHeight;
    m_ProjectTitle = GameName;

    m_PixelsWindow = new GameWindow(m_WindowWidth, m_WindowHeight, m_ProjectTitle);
    

    if (!m_PixelsWindow->ConfigureWindow(m_WindowWidth, m_WindowHeight, m_ProjectTitle))
    {
        return;
    }
}


void PixelsEngine::loadPlayerCamera(glm::vec3& m_SpawnLocation)
{
    m_SpawnLocation = glm::vec3(0.0f, 0.0f, 2.0f);
    m_PlayerCamera  = new PlayerCamera(m_SpawnLocation);

    prevXCoord = m_WindowWidth  / 1.5f;
    prevYCoord = m_WindowHeight / 1.5f;

    mouseMoved = true;
}


void PixelsEngine::loadGraphicsAPI()
{
    m_GraphicsAPI = m_FactoryAPI.RunAPI("OPEN_GL");
    
    if (!m_GraphicsAPI)
    {
        //std::cout << "Error can not find the Open_GL API" << std::endl;
        return;
    }
    
    m_GraphicsAPI->runAPI();
    m_GraphicsAPI->setCameraView(1, 0.0, m_PixelsWindow->getGameWindowWidth(), m_PixelsWindow->getGameWindowHeight());
    m_GraphicsAPI->enableDepthTest();

    m_Shader = new AssetShader("shaders/dark_vs.shader", "shaders/dark_fs.shader");
}


void PixelsEngine::loadWorldTerrain()
{
    m_WorldEnvironment = new ProceduralTerrain();
    int terrainSize = 128;

    m_WorldEnvironment->generateTerrain(terrainSize, terrainSize, 0.03f, 4, 0.5f);
    m_WorldEnvironment->scaleTerrain(0.8, 0.3, 1.5);
    m_WorldEnvironment->setTerrainMaterial("Earth.jpg");

    m_WorldEnvironment->setWaterLevel(0.2f);
    m_WorldEnvironment->setErosionParameters(100, 0.5f, 0.5f);
    m_WorldEnvironment->setRainLevel(0.1f);
    m_WorldEnvironment->setErodedMaterialCap(0.02f);
    m_WorldEnvironment->setErosionRate(0.1f);
    m_WorldEnvironment->setSize(terrainSize);
    m_WorldEnvironment->readTerrainData(true);
    m_WorldEnvironment->loadTexturedTerrain("earth.jpg");

    runTerrainShader();
}



void PixelsEngine::loadWorldTerrain()
{
    m_WorldEnvironment = new ProceduralTerrain();
    int m_TerrainSize = 128;

    m_WorldEnvironment->generateTerrain(m_TerrainSize, m_TerrainSize, 0.5f, 8, 0.5f);
    m_WorldEnvironment->scaleTerrain(0.8, 0.3, 1.5);
    m_WorldEnvironment->setTerrainMaterial(0.5f, 0.5f, 0.5f);
    m_WorldEnvironment->
    m_Terrain->setSize
    //m_WorldEnvironment->read
    runTerrainShader();
}


void PixelsEngine::runTerrainShader()
{
    auto& engine = PixelsEngine::getSession();
    Shader ourShader("shaders/light_vs.shader", "shaders/light_fs.shader");
}


void PixelsEngine::loadTerrain()
{
    m_WorldEnvironment = new ProceduralTerrain();


    std::string tempReadFile = "height128.raw";
    m_WorldEnvironment->loadTerrainData(tempReadFile.c_str(), 128);
}


void PixelsEngine::configureTerrain()
{
    m_WorldEnvironment->setTerrainScale(0.5, 0.1, 0.50);
    m_WorldEnvironment->readTerrainData(true);
    m_WorldEnvironment->loadTexturedTerrain("earth.jpg");
    m_WorldEnvironment->terrainShader = &ourShader;
    m_WorldEnvironment->startPos = glm::vec3(0.0, 0.0, 0.0);
}


void PixelsEngine::setUpViewProjection()
{
    glm::mat4 view = m_PlayerCamera->GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    m_ShaderProgram->setMat4("view", view);
    m_ShaderProgram->setMat4("projection", projection);
}

void PixelsEngine::clearBuffers()
{
    graphics->clearColor(0.38f, 0.47f, 0.65f, 1.0f);
    graphics->clearBuffers();
}

void PixelsEngine::activateShaderProgram()
{
    m_ShaderProgram->use();
}


void PixelsEngine::drawFrame()
{
    clearBuffers();
    activateShaderProgram();
    setUpViewProjection();
    m_WorldEnvironment->draw();
    glfwSwapBuffers(m_GLFWWindow);
    glfwPollEvents();



    m_Terrain = new Terrain();


    void PixelsEngine::loadWorldTerrain()
    {
        // Create a new instance of Terrain
        

        // Set the size of the terrain and the height scale
        m_Terrain->setSize(128, 128);
        m_Terrain->setHeightScale(0.5f);

        // Generate the terrain using a procedural algorithm
        BrushTerrain terrainGenerator;
        terrainGenerator.applyTo(m_Terrain);

        // Set the texture for the terrain
        m_Terrain->setTexture("path/to/terrain_texture.jpg");

        // Load the terrain into the graphics API
        m_Terrain->load(m_GraphicsAPI);

        // Set the camera position for the terrain
        m_Terrain->setCameraPosition(m_PlayerCamera->GetPosition());
    }
    void PixelsEngine::drawFrame()
    {
        clearBuffers();
        activateShaderProgram();
        setUpViewProjection();

        // Update the camera position for the terrain
        m_Terrain->setCameraPosition(m_PlayerCamera->GetPosition());

        // Draw the terrain
        m_Terrain->draw();

        glfwSwapBuffers(m_GLFWWindow);
        glfwPollEvents();
    }
