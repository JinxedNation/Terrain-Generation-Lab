#include "JPG_Loader.h"

JPG_Loader::JPG_Loader(std::string tempFile)
{
	m_TextureFile = tempFile;
}


void JPG_Loader::loadNewTexture()
{
	glGenTextures(1, &m_TextID);
	glBindTexture(GL_TEXTURE_2D, m_TextID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,	  m_WrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,	  m_WrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_MinimumFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_MagFilter);

	stbi_set_flip_vertically_on_load(m_FlipTexture);
	m_TextureData = stbi_load(m_TextureFile.c_str(), &m_TextureWidth, &m_TextureHeight, &m_ChannelCount, 0);


	if (m_TextureData)
	{
		GLenum internalFormat = m_ChannelCount == 4 ? GL_SRGB_ALPHA : GL_SRGB;
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_TextureWidth, m_TextureHeight, 0, m_ChannelCount == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, m_TextureData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Unable to load the terrain texture file!" << std::endl;
	}

	stbi_image_free(m_TextureData);
}


void JPG_Loader::bindNewTexture(int tempTexture)
{

	glActiveTexture(GL_TEXTURE0 + tempTexture);   // modify this function in the future to specify which texture unit? eg 0, 1 ..
	glBindTexture(GL_TEXTURE_2D, tempTexture);
}


void JPG_Loader::unbindOldTexture()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}