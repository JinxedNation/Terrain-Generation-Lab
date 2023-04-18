#include "OpenGL_API.h"
#include <GLFW/glfw3.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/glut.h>
#include <GL/gl.h>


bool OpenGL_API::runAPI()
{
    // Initialize OpenGL context and resources
    if (!glfwInit()) 
    {
        return false;
    }

    const char* vertexShaderCode = "#version 330 core\nlayout(location = 0) in vec3 aPosition;\nvoid main() {gl_Position = vec4(aPosition, 1.0);}";
    const char* fragmentShaderCode = "#version 330 core\nout vec4 fragColor;\nvoid main() {fragColor = vec4(1.0, 0.0, 0.0, 1.0);}";
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderCode, nullptr);
    glCompileShader(vertexShader);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderCode, nullptr);
    glCompileShader(fragmentShader);
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    return true;
}


bool OpenGL_API::setGameWindow(void* windowHandle)
{
    GLFWwindow* m_GLFWWindow = static_cast<GLFWwindow*>(windowHandle);
    glfwMakeContextCurrent(m_GLFWWindow);

    return true;
}


void OpenGL_API::setCameraView(int viewID, double tempXmin, double tempXMax, double tempYMin, double tempYMax, double tempNearZ, double tempFarZ)
{
    // Get the current viewport dimensions
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    // Calculate the aspect ratio
    double aspectRatio = (double)viewport[2] / (double)viewport[3];

    // Set the projection matrix
    glm::mat4 cameraProjection = glm::ortho(tempXmin, tempXMax, tempYMin, tempYMax, tempNearZ, tempFarZ);
    GLfloat cameraProjectionData[16];
    memcpy(cameraProjectionData, &cameraProjection[0][0], sizeof(cameraProjectionData));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "cameraProjection"), 1, GL_FALSE, cameraProjectionData);

    // Set the view matrix
    glm::mat4 playerView = glm::mat4(1.0f); // Identity matrix
    GLfloat playerViewData[16];
    memcpy(playerViewData, &playerView[0][0], sizeof(playerViewData));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "playerView"), 1, GL_FALSE, playerViewData);
}



void OpenGL_API::enableDepthTest() 
{
    glEnable(GL_DEPTH_TEST);
}


void OpenGL_API::clearBuffers()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void OpenGL_API::setView(int tempX, int tempY, int tempWidth, int tempHeight)
{
	glViewport(tempX, tempY, tempWidth, tempHeight);
}


void OpenGL_API::bindVAO(int vao)
{

	glBindVertexArray(vao);
}


void OpenGL_API::unbindVAO()
{
	glBindVertexArray(0);
}