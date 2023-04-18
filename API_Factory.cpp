#include "API_Factory.h"

	/***********************************************************
	 *
	 * @brief : at a later date add vulkan, directX
	 *
	 ***********************************************************/

std::unique_ptr <Graphics_API> API_Factory::RunAPI(std::string GraphicsAPI)
{
	if (GraphicsAPI == "OPEN_GL")
	{
		return std::make_unique<OpenGL_API>();
	}
	//GraphicsAPI == VULKAN
	//GraphicsAPI == DIRECTX_12

}