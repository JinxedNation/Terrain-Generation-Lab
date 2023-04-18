#pragma once

#include <memory>
#include <string>
#include "JPG_Loader.h"
#include "PNG_Loader.h"
#include "TextureLoader.h"



class TextureLoader_Factory
{
	public:
		std::unique_ptr<TextureLoader> generateTexture(std::string textureFile);
};