#pragma once



#include <iostream>
#include <fstream>
#include <string>

#include <GLFW/glfw3.h>


#include "PlayerCamera.h"
#include "InputManager.h"


	/****************************************************************************************************************************************
	 * @class   : GameWindow
	 * @brief   : A game window class this class is utilized to generate a game window
	 * @brief   :
	 *
	 * @author  : William Halling
	 * @date    : 2023
	 * @version : 1.0
	 ****************************************************************************************************************************************/

class GameWindow
{
	public:

			/************************************************************************************************************************************
			 * @brief : GameWindow()
			 * @brief : Constructor that takes the windows width, height and title
			 * 
			 * @param : tempWidth  - unsigned
			 * @param : tempHeight - unsigned 
			 * @param : tempTitle  - const char*
			 ************************************************************************************************************************************/

		GameWindow(unsigned tempWidth, unsigned tempHeight, const char* tempTitle);


			/************************************************************************************************************************************
			 * @brief : GameWindow()
			 * @brief : Used to get an Instance of a game window
			 * 
			 * @param : tempGameWindow - GLFWwindow*
			 ************************************************************************************************************************************/

		GameWindow(GLFWwindow* tempGameWindow);


			/************************************************************************************************************************************
			 * @brief : GameWindow()
			 * @brief : Used to get an Instance of a game window
			 ************************************************************************************************************************************/

		~GameWindow();


			/************************************************************************************************************************************
			 * @brief : GameWindow()
			 * @brief : Used to get an Instance of a game window
			 ************************************************************************************************************************************/

		unsigned getGameWindowWidth() const;


			/************************************************************************************************************************************
			 * @brief : GameWindow()
			 * @brief : Used to get an Instance of a game window
			 ************************************************************************************************************************************/

		unsigned getGameWindowHeight() const;


			/************************************************************************************************************************************
			 * @brief : GameWindow()
			 * @brief : Used to get an Instance of a game window
			 ************************************************************************************************************************************/

		void setGameWindowWidth(unsigned  tempWidth);


			/************************************************************************************************************************************
			 * @brief : GameWindow()
			 * @brief : Used to get an Instance of a game window
			 ************************************************************************************************************************************/

		void setGameWindowHeight(unsigned tempHeight);


			/************************************************************************************************************************************
			 * @brief  : ConfigureWindow()
			 * @brief  : Used to set the paramaters for a GameWindow
			 * 
			 * @param  : tempWidth	- unsigned
			 * @param  : tempHeight - unsigned  
			 * @param  : tempTitle  - const char*
			 * @return : true or flase depending on if the window was configured or not
			 ************************************************************************************************************************************/

		bool ConfigureWindow(unsigned tempWidth, unsigned tempHeight, const char* tempTitle);


			/************************************************************************************************************************************
			 * @brief : launchGame()
			 * @brief : runs the game and keeps the window open as long as the player doesnt opt to close the window
			 ************************************************************************************************************************************/

		void launchGame();


			/************************************************************************************************************************************
			 * @brief : getGameTimer()
			 * @brief : Is used to track the time between in frame within the game
			 * @brief : m_DeltaTime - float
			 ************************************************************************************************************************************/

		double getGameTimer() const;


			/************************************************************************************************************************************
			 * @brief : EndWindow()
			 * @brief : used to end the game window
			 ************************************************************************************************************************************/

		void EndWindow();


	private:

		GLFWwindow*   m_GameWindow;
		PlayerCamera* m_PlayerCamera;
		InputManager* m_InputManager;
		double		  m_DeltaTime;
		unsigned	  m_WindowWidth, m_WindowHeight;
};

