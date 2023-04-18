#pragma once
#include "TextureLoader.h"


/****************************************************
 *
 *
 *
 *
 ****************************************************/



class JPG_Loader : public TextureLoader
{
	public:

		JPG_Loader(std::string tempFile);

		virtual void loadNewTexture() override;

		virtual void bindNewTexture(int textureUnit) override;

		virtual void unbindOldTexture() override;
};

