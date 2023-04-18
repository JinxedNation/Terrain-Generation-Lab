#include "AssetShader.h"


AssetShader::AssetShader(const char* vertexFile, const char* fragmentFile)
{
    runAndLink(vertexFile, fragmentFile);

}

std::string readShaderFromFile(const std::string& mShadeFile)
{
    std::ifstream shaderFile;


    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    
    
    try 
    {
        shaderFile.open(mShadeFile);
    
        std::stringstream shaderStream;
        
        shaderStream << shaderFile.rdbuf();
        
        shaderFile.close();


        return shaderStream.str();
    }

    catch (std::ifstream::failure eShader) 
    {
        throw std::runtime_error("Failed to read shader file: " + mShadeFile);
    }
}



void AssetShader::runAndLink(const char* vertexFile, const char* fragmentFile)
{
    std::string m_VertexData, m_FragmentData;

    try 
    {
        m_VertexData   = readShaderFromFile(vertexFile);
        m_FragmentData = readShaderFromFile(fragmentFile);
    }
    catch (const std::exception& eShader) 
    {
        std::cerr << "Unable to read shaders from file :: " << eShader.what() << std::endl;

        return;
    }

    const char* vertexSData   = m_VertexData.c_str();
    const char* fragmentSData = m_FragmentData.c_str();

        // Compile shaders
    unsigned int m_Vert, m_Frag;
    int          success;
    char         errorLog[512];
    
        // Vertex Shader
    m_Vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(m_Vert, 1, &vertexSData, NULL);
    glCompileShader(m_Vert);
    isRTErrors(m_Vert, "VERTEX");


        // Fragment Shader  
    m_Frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(m_Frag, 1, &fragmentSData, NULL);
    glCompileShader(m_Frag);
    isRTErrors(m_Frag, "FRAGMENT");


        // Shader Program
    m_Shaders = glCreateProgram();
    glAttachShader(m_Shaders, m_Vert);
    glAttachShader(m_Shaders, m_Frag);
    glLinkProgram(m_Shaders);
    isRTErrors(m_Shaders, "PROGRAM");


        // Delete shaders after linked
    glDeleteShader(m_Vert);
    glDeleteShader(m_Frag);
}


void AssetShader::activeUse()
{
    glUseProgram(m_Shaders);
}



void AssetShader::setBoolean(const std::string& tempName, bool tempVal) const
{
    setUniform(tempName, tempVal);
}


void AssetShader::setInteger(const std::string& tempName, int tempVal) const
{
    setUniform(tempName, tempVal);
}


void AssetShader::setFloat(const std::string& tempName, float tempVal) const
{
    setUniform(tempName, tempVal);
}


void AssetShader::setMat4(const std::string& tempName, const glm::mat4& tempVal) const
{
    setUniform(tempName, tempVal);
}




void AssetShader::setVec3(const std::string& tempName, const glm::vec3& tempVal) const
{
    setUniform(tempName, tempVal);
}


template<typename T>
void AssetShader::setUniform(const std::string& tempName, const T& tempVal) const
{
    auto location = uniforms.find(tempName);

    if (location != uniforms.end() && location->second.shaderType != tempVal)
    {

        location->second.shaderType = tempVal;
        

        if constexpr (std::is_same_v<T, bool>) 
        {
            glUniform1i(location->second.storeLocation, (int)tempVal);
        }
        
        else if constexpr (std::is_same_v<T, int>) 
        {
            glUniform1i(location->second.storeLocation, tempVal);
        }
        
        else if constexpr (std::is_same_v<T, float>)
        {
            glUniform1f(location->second.storeLocation, tempVal);
        }
        
        else if constexpr (std::is_same_v<T, glm::mat4>) 
        {
            glUniformMatrix4fv(location->second.storeLocation, 1, GL_FALSE, glm::value_ptr(tempVal));
        }
        
        else if constexpr (std::is_same_v<T, glm::vec3>) 
        {
            glUniform3fv(location->second.storeLocation, 1, glm::value_ptr(tempVal));
        }
    }
}


void AssetShader::isRTErrors(unsigned int tempShader, std::string tempType)
{
    int  itRuns;
    char infoLog[1024];
    if (tempType != "PROGRAM")
    {
        glGetShaderiv(tempShader, GL_COMPILE_STATUS, &itRuns);
       
        if (!itRuns)
        {
            glGetShaderInfoLog(tempShader, 1024, NULL, infoLog);

            std::cerr << "Compile error with the shader (" << tempType << "): " << infoLog << std::endl;
        }
    }
    else
    {
        glGetProgramiv(tempShader, GL_LINK_STATUS, &itRuns);
        
        if (!itRuns) 
        {
            glGetProgramInfoLog(tempShader, 1024, NULL, infoLog);

            std::cerr << "Linking error with shader : " << infoLog << std::endl;
        }
    }
}