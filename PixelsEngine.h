#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <functional>
#include <iostream>
#include <string>


#include "ProceduralTerrain.h"
#include "Graphics_API.h"
#include "PlayerCamera.h"
#include "AssetShader.h"
#include "GameWindow.h"

#include "stb_image.h"



class PixelsEngine
{
public:

    PixelsEngine();

    void runEngine();


    void loadGraphicsAPI();


    void loadGameWindow(unsigned tempWindowWidth, unsigned tempWindowHeight, const char* GameName);


    void loadPlayerCamera(glm::vec3& playerLocation);
    //void loadPlayerCamera(glm::vec3 spawnLocation);




    void setWindowCallbacks();

    void loadWorldTerrain();
    void cleanup();


private:
    void setUpViewProjection();
    void clearBuffers();
    void drawFrame();


    GameWindow*         m_PixelsWindow;
    PlayerCamera*       m_PlayerCamera;
    ProceduralTerrain*  m_WorldEnvironment;
    API_Factory         m_FactoryAPI;
    GLFWwindow*         m_GLFWWindow;
    AssetShader*        m_Shader;

    float prevXCoord;
    float prevYCoord;
    bool  mouseMoved;

    unsigned m_WindowWidth;
    unsigned m_WindowHeight;


    const char*        m_ProjectTitle;
    glm::vec3          m_SpawnLocation;
    std::unique_ptr<Graphics_API> m_GraphicsAPI;
};

