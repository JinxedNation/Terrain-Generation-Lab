#pragma once

#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <algorithm>

#include "AssetShader.h"
#include "Texture.h"

	/*************************************************************************************************************************************************
	 * @class : Terrain  
	 * @brief :  
	 * 
	 * 
	 * @author  : William Halling
	 * @date    : April 2023
	 * @version : 1.0
	 * @bug		:
	 **************************************************************************************************************************************************/

class Terrain 
{
	public:
		
			/********************************************************************************************************
			 * @brief : Terrain()
			 * @brief : Terrain Constructor takes the size of the terrain as a parameter
			 *******************************************************************************************************/

		Terrain(unsigned terrainSize);
		

			/********************************************************************************************************
			 * @brief : Terrain() Destructor
			 * @brief : Terrain Destructor virtual as we want to be able to delete the terrain object
			 *******************************************************************************************************/

		virtual ~Terrain() = default;


			/********************************************************************************************************
			 * @brief : createTerrain()
			 * @brief : Used to load the terrain data from the file that is read.
			 * @oaram : readFileName - const char*
			 *******************************************************************************************************/


		float findMinimum(const std::vector<float>& m_TerrainData) const;


		float findMaximum(const std::vector<float>& m_TerrainData) const;


		void normalizeTerrain(std::vector<float>& m_TerrainData);



		void scaleTerrain(std::vector<float>& m_TerrainData, float tempMinHeight, float tempHeightRange);


			/********************************************************************************************************
			 * @brief : draw()
			 * @brief : Used to draw the terrain 
			 * @param : tempProjection - glm::mat4
			 * @param : tempView	   - glm::mat4
			 *******************************************************************************************************/

		void draw() const;


		int m_TerrainSize;


	protected:
		
		virtual bool createTerrain(const char* readFileName) = 0;
		virtual void getTerrainPoints() = 0;

		virtual void generateTexture()  = 0;
		bool  m_isTexturedTerrain;
		float m_MinTerrainHeight;
		float m_MaxTerrainHeight;
		float m_HeightLevels;
		float m_TerrainHeight;

		

		GLuint m_TerrainVAO;
		GLuint m_TerrainVBO;
		GLuint m_TerrainEBO;

		std::vector<float>			 m_TerrainData;
		std::vector<unsigned int>	 m_TerrainIndices;
		std::unique_ptr<Texture>	 m_TerrainTexture;
		std::unique_ptr<AssetShader> m_AssetShader;
};