#include "GameWindow.h"


GameWindow::GameWindow(unsigned tempWidth, unsigned tempHeight, const char* tempTitle) : m_GameWindow(nullptr), m_PlayerCamera(new PlayerCamera()), m_InputManager(nullptr)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_PlayerCamera = new PlayerCamera();
	m_InputManager = new InputManager(m_GameWindow, m_PlayerCamera);


	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	m_GameWindow = glfwCreateWindow(tempWidth, tempHeight, tempTitle, nullptr, nullptr);

	if (!m_GameWindow)
	{
		glfwTerminate();

		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(m_GameWindow);
	glfwSwapInterval(1);
}


GameWindow::GameWindow(GLFWwindow* tempGameWindow) : m_GameWindow(tempGameWindow), m_PlayerCamera(new PlayerCamera()), m_InputManager(nullptr)
{
	if (!m_GameWindow)
	{
		std::cerr << "Erorr there was a null ptr to GLFW " << std::endl;

		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(m_GameWindow);
	glfwSwapInterval(1);

	m_InputManager = new InputManager(m_GameWindow, m_PlayerCamera);
}



GameWindow::~GameWindow()
{

}


unsigned GameWindow::getGameWindowWidth()  const  { return m_WindowWidth;  }
unsigned GameWindow::getGameWindowHeight() const  { return m_WindowHeight; }


void GameWindow::setGameWindowWidth( unsigned  tempWidth)  { m_WindowWidth  = tempWidth; }
void GameWindow::setGameWindowHeight(unsigned  tempHeight) { m_WindowHeight = tempHeight; }



double GameWindow::getGameTimer() const
{
	static double previousTime = glfwGetTime();
	double currentTime		   = glfwGetTime();
	double deltaTime		   = currentTime - previousTime;
	previousTime			   = currentTime;

	return deltaTime;
}


bool GameWindow::ConfigureWindow(unsigned tempWidth, unsigned tempHeight, const char* tempTitle)
{
	m_GameWindow = glfwCreateWindow(tempWidth, tempHeight, tempTitle, nullptr, nullptr);

	if (!m_GameWindow)
	{
		std::cerr << "Error can not make a GLFW game window " << std::endl;

		return false;
	}

	glfwMakeContextCurrent(m_GameWindow);
	glfwSwapInterval(1);

	return true;
}


void GameWindow::launchGame()
{
	double latestFrame = glfwGetTime();

	InputManager m_UserInput(m_GameWindow, m_PlayerCamera);


	while (!glfwWindowShouldClose(m_GameWindow))
	{
		double deltaT = getGameTimer();
		glfwPollEvents();

		m_PlayerCamera->updateCameraValues();
		m_InputManager->readInput(m_GameWindow, deltaT);

		
		glfwSwapBuffers(m_GameWindow);
		latestFrame = glfwGetTime();
	}
}



void GameWindow::EndWindow()
{
	delete m_InputManager;
	delete m_PlayerCamera;
	glfwDestroyWindow(m_GameWindow);
	glfwTerminate();
}