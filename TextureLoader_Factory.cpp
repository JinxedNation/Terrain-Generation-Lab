#include "TextureLoader_Factory.h"


std::unique_ptr<TextureLoader> TextureLoader_Factory::generateTexture(std::string textureFile)
{
	std::string fileType = textureFile.substr(textureFile.find_last_of(".") + 1);


	if (fileType == "jpg" || fileType == "jpeg")
	{
		return std::make_unique<JPG_Loader>(textureFile); // unique pointer so when goes out of scope memory is auto deletd. ( I think )

	}
	else if (fileType == "png")
	{
		return std::make_unique<PNG_Loader>(textureFile);
	}
}

std::unique_ptr<> createTexture(std::string fileName) {
	// find out the file type
	
}