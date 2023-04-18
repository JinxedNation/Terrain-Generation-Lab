#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>


#include "PlayerCamera.h"

    /**********************************************************************************************************
     * @class   : InputManager
     * @brief   : Class is responsible for reading and performing user inputs
     * @brief   : 
     * 
     * @author  : William Halling
     * @date    : April 2023
     * @version : 1.0
     * @bug     : 
     **********************************************************************************************************/

class InputManager
{
    public:
            /**********************************************************************************************************
             * @brief : InputManager
             * @brief : InputManager Constructor
             **********************************************************************************************************/

        InputManager(GLFWwindow* m_GameWindow, PlayerCamera* tempCamera);


            /**********************************************************************************************************
             * @brief : InputManager
             * @brief : InputManager Destructor
             **********************************************************************************************************/

        ~InputManager();


            /**********************************************************************************************************
             * @brief : readInput
             * @brief : Used to read user input
             * 
             * @param : tempDeltaT   - float
             * @param : m_GameWindow - GLFWwindow* 
             **********************************************************************************************************/

        void readInput(GLFWwindow* m_GameWindow, float tempDeltaT);


            /**********************************************************************************************************
             * @brief : setupKeyboard
             * @brief : used to set up a users keyboard so the gamewindow can register if the user enters any keys
             **********************************************************************************************************/

        void setupKeyboard();


            /**********************************************************************************************************
             * @brief : setupMouse
             * @brief : used to set up a users mouse so the gamewindow can register if the users mouse was moved
             **********************************************************************************************************/

        void setupMouse();


            /**********************************************************************************************************
             * @brief : setupMouseScroll
             * @brief : used to set up a users mouse so the gamewindow can register if the user scrolled mouse wheel
             **********************************************************************************************************/

        void setupMouseWheel();


            /**********************************************************************************************************
             * @brief : CallMouseScroll
             * @brief : Used to call the users mouse scroll if they opperate it
             * 
             * @param : m_GameWindow - GLFWwindow* 
             * @param : tempYOffSet  - double
             **********************************************************************************************************/

        void CallMouseScroll(GLFWwindow* m_GameWindow, double tempYOffSet);


            /**********************************************************************************************************
             * @brief : CallMouse
             * @brief : used to register if a player moves the mouse
             * 
             * @param : tempYCoord   - double
             * @param : tempXCoord   - double
             * @param : m_GameWindow - GLFWwindow* 
             **********************************************************************************************************/

        void CallMouse(GLFWwindow* m_GameWindow, double tempXCoord, double tempYCoord);


            /**********************************************************************************************************
             * @brief : isKeyEntered
             * @brief : used to check if a key is entered
             * 
             * @param  : tempKeyEntry - int
             * @return : true if a key is entered else false 
             **********************************************************************************************************/

        bool isKeyEntered(int tempKeyEntry);


            /**********************************************************************************************************
             * @brief : keyCallback
             * @brief : used to perform an action based on what key is enterd by the user
             * 
             * @param : tempKeyEntry - int the action expected
             * @param : enteredKey   - int 
             * @param : m_GameWindow - GLFWwindow* 
             *******************************************************************************s***************************/

        void keyCallback(GLFWwindow* m_GameWindow, int enteredKey, int tempKeyEntry, int task);


    private:
        bool          isWireframeMode;
        PlayerCamera* m_PlayerCamera;
        GLFWwindow*   m_GameWindow;
};