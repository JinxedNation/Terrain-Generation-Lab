#include "PlayerCamera.h"


// constructor with vectors
PlayerCamera::PlayerCamera(glm::vec3 tempPosition = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 tempUp = glm::vec3(0.0f, 1.0f, 0.0f), float tempYaw = CameraYaw, float tempPitch = CameraPitch) : m_ForwardDir(glm::vec3(0.0f, 0.0f, -1.0f)), m_Speed(CameraSpeed), m_RotationSpeed(CameraSensitivity), m_CameraZoom(CamearZoom)
{
    m_CameraPosition  = tempPosition;
    m_UpDown       = tempUp;
    m_CameraYaw    = tempYaw;
    m_CameraPitch  = tempPitch;

    updateCameraValues();
}


// constructor with scalar values
PlayerCamera::PlayerCamera(float tempX, float tempY, float tempZ, float tempXUp, float tempYUp, float tempZUp, float tempYaw, float tempPitch) : m_ForwardDir(glm::vec3(0.0f, 0.0f, -1.0f)), m_Speed(CameraSpeed), m_RotationSpeed(CameraSensitivity), m_CameraZoom(CamearZoom)
{
    m_CameraPosition = glm::vec3(tempX, tempY, tempZ);
    m_UpDown         = glm::vec3(tempXUp, tempYUp, tempZUp);
    m_CameraYaw      = tempYaw;
    m_CameraPitch    = tempPitch;

    updateCameraValues();
}



glm::mat4 PlayerCamera::getViewMatrix()
{
    return glm::lookAt(m_CameraPosition, m_CameraPosition + m_ForwardDir, m_UpDir);
}



void PlayerCamera::readKeyboard(MovementDirection tempDirection, float tempDeltaT)
{
    float cameraVelocity = m_Speed * tempDeltaT;


    if (tempDirection == LEFT)
    {
        m_CameraPosition -= m_LeftRight * cameraVelocity;
    }


    if (tempDirection == RIGHT)
    {
        m_CameraPosition += m_LeftRight * cameraVelocity;
    }


    if (tempDirection == UP)
    {
        m_CameraPosition += m_ForwardDir * cameraVelocity;
    }


    if (tempDirection == BACK)
    {
        m_CameraPosition -= m_ForwardDir * cameraVelocity;
    }
}



void PlayerCamera::readMouseMovement(float tempXOffSet, float tempYOffSet, GLboolean constrainPitch = true)
{
    tempXOffSet *= m_RotationSpeed;
    tempYOffSet *= m_RotationSpeed;

    m_CameraYaw   += tempXOffSet;
    m_CameraPitch += tempYOffSet;


    if (constrainPitch)
    {
        if (m_CameraPitch > 89.0f)
        {
            m_CameraPitch = 89.0f;
        }

        if (m_CameraPitch < -89.0f)
        {
            m_CameraPitch = -89.0f;
        }
    }

    updateCameraValues();
}



void PlayerCamera::readMouseScroll(float tempYOffSet)
{
    m_CameraZoom -= (float) tempYOffSet;

    if (m_CameraZoom < 1.0f)
    {
        m_CameraZoom = 1.0f;
    }

    if (m_CameraZoom > 45.0f)
    {
        m_CameraZoom = 45.0f;
    }
}


void PlayerCamera::updateCameraValues()
{
    glm::vec3 m_Forward;

    m_Forward.x = cos(glm::radians(m_CameraYaw)) * cos(glm::radians(m_CameraPitch));

    m_Forward.y = sin(glm::radians(m_CameraPitch));

    m_Forward.z = sin(glm::radians(m_CameraYaw)) * cos(glm::radians(m_CameraPitch));


    m_ForwardDir    = glm::normalize(m_Forward);
    m_LeftRight     = glm::normalize(glm::cross(m_ForwardDir, m_UpDown));
    m_UpDir         = glm::normalize(glm::cross(m_LeftRight, m_ForwardDir));
}



void PlayerCamera::movePlayerLeft()
{
    m_CameraPosition -= m_LeftRight * m_Speed;
}



void PlayerCamera::movePlayerRight()
{
    m_CameraPosition += m_LeftRight * m_Speed;
}



void PlayerCamera::movePlayerForward()
{
    m_CameraPosition += m_ForwardDir * m_Speed;
}



void PlayerCamera::movePlayerBack()
{
    m_CameraPosition -= m_ForwardDir * m_Speed;
}



void PlayerCamera::lookLeft()
{
    m_CameraYaw -= m_Speed;
    updateCameraValues();
}



void PlayerCamera::lookRight()
{
    m_CameraYaw += m_Speed;
    updateCameraValues();
}



void PlayerCamera::lookUp()
{
    m_CameraPitch += m_Speed;
    updateCameraValues();
}



void PlayerCamera::lookDown()
{
    m_CameraPitch -= m_Speed;
    updateCameraValues();
}


glm::vec3 PlayerCamera::getStartLocation(const SimpleTerrain& terrain) const
{
    const float terrainSize     = static_cast<float>(terrain.getSize());
    const float terrainScaleX   = terrain.getScaleX();
    const float terrainScaleZ   = terrain.getScaleZ();
    const float terrainHalfSize = terrainSize / 2.0f;

    const float startX = terrainHalfSize;
    const float startZ = terrainHalfSize;
    const float startY = terrain.getHeight(startX * terrainScaleX, startZ * terrainScaleZ);

    const glm::vec3 initialPosition(startX * terrainScaleX, startY + 1.0f, startZ * terrainScaleZ);

    return initialPosition;
}