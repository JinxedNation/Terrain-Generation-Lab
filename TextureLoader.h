#pragma once

#include "stb_image.h"
#include <glad/glad.h>

#include <string>
#include <iostream>
#include <memory>


class TextureLoader
{

	public:

		virtual void loadNewTexture() = 0;

		virtual void bindNewTexture(int textureUnit) = 0;

		virtual void unbindOldTexture() = 0;


protected:

	int	   m_TextureWidth;
	int	   m_TextureHeight;
	int	   m_ChannelCount;
	bool   m_FlipTexture = true;

	unsigned int   m_TextID;
	std::string	   m_TextureFile;
	unsigned char* m_TextureData;

	GLenum m_WrapMode = GL_REPEAT;
	GLenum m_MinimumFilter = GL_LINEAR_MIPMAP_LINEAR;
	GLenum m_MagFilter = GL_LINEAR;
};
