#include "InputManager.h"


InputManager::InputManager(GLFWwindow* gameWindow, PlayerCamera* tempCamera) : m_GameWindow(gameWindow), m_PlayerCamera(tempCamera)
{
    glfwSetWindowUserPointer(m_GameWindow, this);
    setupKeyboard();
    setupMouse();
    setupMouseWheel();
    glfwSetInputMode(m_GameWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}


InputManager::~InputManager() {}


void InputManager::setupKeyboard()
{
    glfwSetKeyCallback(m_GameWindow, [](GLFWwindow* m_GameWindow, int enteredKey, int tempCode, int tempAction, int tempMods)
    {
            InputManager* inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(m_GameWindow));
            inputManager->keyCallback(m_GameWindow, enteredKey, tempAction);
    });
}


void InputManager::setupMouse()
{
    glfwSetCursorPosCallback(m_GameWindow, [](GLFWwindow* m_GameWindow, double tempXCoord, double tempYCoord)
    {
        InputManager* inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(m_GameWindow));

        inputManager->CallMouse(m_GameWindow, tempXCoord, tempYCoord);
    });
}


void InputManager::setupMouseWheel()
{
    glfwSetScrollCallback(m_GameWindow, [](GLFWwindow* m_GameWindow, double tempXOffSet, double tempYOffSet)
    {
        InputManager* inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(m_GameWindow));

        inputManager->CallMouseScroll(m_GameWindow, tempYOffSet);
    });
}



void InputManager::CallMouseScroll(GLFWwindow* m_GameWindow, double tempYOffSet)
{
    m_PlayerCamera->readMouseScroll(tempYOffSet);
}



void InputManager::CallMouse(GLFWwindow* m_GameWindow, double tempXCoord, double tempYCoord)
{
    static float lastMouseX = static_cast<float>(tempXCoord);
    static float lastMouseY = static_cast<float>(tempYCoord);


    float xoffset = static_cast<float>(tempXCoord) - lastMouseX;
    float yoffset = lastMouseY - static_cast<float>(tempYCoord);


    lastMouseX = static_cast<float>(tempXCoord);
    lastMouseY = static_cast<float>(tempYCoord);


    m_PlayerCamera->readMouseMovement(xoffset, yoffset);
}



bool InputManager::isKeyEntered(int keyToCheck)
{
    return keyToCheck == GLFW_PRESS;
}



void InputManager::keyCallback(GLFWwindow* m_GameWindow, int keyPressed, int keyAction, int action)
{
    if (!isKeyEntered(keyPressed))
    {
        return;
    }

    switch (keyPressed)
    {
        case GLFW_KEY_W:
            m_PlayerCamera->movePlayerForward();
        break;
    
        case GLFW_KEY_S:
            m_PlayerCamera->movePlayerBack();
        break;
    
        
        case GLFW_KEY_A:
            m_PlayerCamera->movePlayerLeft();
        break;
    
        
        case GLFW_KEY_D:
            m_PlayerCamera->movePlayerRight();
        break;
    
        
        case GLFW_KEY_UP:
            m_PlayerCamera->lookUp();
        break;
    
        
        case GLFW_KEY_DOWN:
            m_PlayerCamera->lookDown();
        break;
    
        
        case GLFW_KEY_LEFT:
            m_PlayerCamera->lookLeft();
        break;
    
        
        case GLFW_KEY_RIGHT:
            m_PlayerCamera->lookRight();
        break;

        case GLFW_KEY_C:
            
            if (action == GLFW_PRESS)
            {
                isWireframeMode = !isWireframeMode;

                if (isWireframeMode)
                {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                }
                else
                {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }
            }
        break;


        case GLFW_KEY_F:
            
            if (action == GLFW_PRESS)
            {
                if (isWireframeMode)
                {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                }
                else
                {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }
            }
        break;
    }
}










/*********

/// move to camera class
// Key callback function
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // If the "C" key is pressed and the action is key press, toggle the light state
    if (key == GLFW_KEY_C && action == GLFW_PRESS) {
        colour = !colour;

        if (colour)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }


        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }

    auto& engRef = LabEngine::getInstance();
    // If the "F" key is pressed then swap between can FLY and can't FLY
    if (key == GLFW_KEY_F && action == GLFW_PRESS)
    {
        engRef.canFly = !engRef.canFly;


        if (colour)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }

        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }

**********/
