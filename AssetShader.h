#ifndef ASSETSHADER_H
#define ASSETSHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>

// Math
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


    /******************************************
     * @class : Asset Shader
     * @brief : Developed using https://learnopengl.com/Getting-started/shaders
     * 
     * @date   :
     * @author :
     * @bug    :
     ******************************************/




class AssetShader
{
    public:


            /***************************************************************************************************************
             * @brief : 
             * @brief :
             ***************************************************************************************************************/

        AssetShader(const char* vertexFile, const char* fragmentFile);

        
            /***************************************************************************************************************
             * @brief :
             * @brief :
             ***************************************************************************************************************/

        void activeUse();
    

        void runAndLink(const char* vertexFile, const char* fragmentFile);

            /***************************************************************************************************************
             * @brief :
             * @brief :
             ***************************************************************************************************************/

        template<typename T>
        void setUniform(const std::string& tempName, const T& tempVal) const;


        // utility uniform functions
        // ------------------------------------------------------------------------

            /***************************************************************************************************************
             * @brief :
             * @brief :
             ***************************************************************************************************************/
    
        void setBoolean(const std::string& tempName, bool tempVal) const;


            /***************************************************************************************************************
             * @brief :
             * @brief :
             ***************************************************************************************************************/

        void setInteger(const std::string& tempName, int tempVal) const;


            /***************************************************************************************************************
             * @brief :
             * @brief :
             ***************************************************************************************************************/

        void setFloat(const std::string& tempName, float tempVal) const;


            /***************************************************************************************************************
             * @brief :
             * @brief :
             ***************************************************************************************************************/

        void setMat4(const std::string& tempName, const glm::mat4& tempVal) const;


            /***************************************************************************************************************
             * @brief :
             * @brief :
             ***************************************************************************************************************/

        void setVec3(const std::string& tempName, const glm::vec3& tempVal) const;


    private:
 
            /***************************************************************************************************************
             * @brief : isRTErrors
             * @brief :
             ***************************************************************************************************************/

        void isRTErrors(unsigned int tempShader, std::string tempType);
        
    

        struct ShaderDataTypes 
        {
            GLenum dataType;
            GLint  storeLocation;
            union 
            {
                bool boolValue;
                int intValue;
                float floatValue;
                glm::vec3 vectorValue;
                glm::mat4 matrixValue;
            } 
            shaderType;
            
            
            ShaderDataTypes(GLenum type, GLint location) : dataType(type), storeLocation(location) {}
            
            ~ShaderDataTypes() {}
            
            ShaderDataTypes(const ShaderDataTypes& oldData)
            {
                dataType      = oldData.dataType;
                storeLocation = oldData.storeLocation;
                shaderType    = oldData.shaderType;
            }


            ShaderDataTypes& operator=(const ShaderDataTypes& cmpShader)
            {
                if (this == &cmpShader)
                {
                    return *this;
                }

                dataType      = cmpShader.dataType;
                storeLocation = cmpShader.storeLocation;
                shaderType    = cmpShader.shaderType;
                return *this;
            }

            bool operator==(const ShaderDataTypes& cmp) const
            {
                if (dataType != cmp.dataType)
                {
                    return false;
                }
                
                if (dataType == GL_BOOL) 
                {
                    return shaderType.boolValue == cmp.shaderType.boolValue;
                }
                
                
                if (dataType == GL_INT)
                {
                    return shaderType.intValue == cmp.shaderType.intValue;
                }


                if (dataType == GL_FLOAT)
                {
                    return shaderType.floatValue == cmp.shaderType.floatValue;
                }

                if (dataType == GL_FLOAT_VEC3) 
                {
                    return shaderType.vectorValue == cmp.shaderType.vectorValue;
                }

                if (dataType == GL_FLOAT_MAT4)
                {
                    return shaderType.matrixValue == cmp.shaderType.matrixValue;
                }
                return false;
            }
        };

        std::map<std::string, ShaderDataTypes> uniforms;

        unsigned int m_Shaders;

        std::string readShaderFile(const char* tempFile);
};
#endif