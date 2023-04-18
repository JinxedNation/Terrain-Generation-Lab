#pragma once

#include <memory>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <glad/glad.h>

#include "OpenGL_API.h"
#include "Graphics_API.h"


	/*************************************************************************************  
	 * @class	: API_Factory
	 * @brief	: A Factory method pattern for varying types of graphics APIS
	 * 
	 * @date    : April 2023
	 * @author  : William Halling
	 * @version : 1.0
	 * @bug		:
	 **********************************************************************************/

class API_Factory
{
	public:

		std::unique_ptr <Graphics_API> RunAPI(std::string GraphicsAPI);

};