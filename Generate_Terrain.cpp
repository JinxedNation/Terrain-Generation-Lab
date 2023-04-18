bool Terrain::setUpTerrainData(bool hasTexture)
{
    this->hasTexture = hasTexture;

    if (hasTexture == false) {
        ourShader = new Shader("shaders/vertexShader_terrain.shader", "shaders/fragmentShader_terrain.shader");

        // Set up the vertex and index data
        setUpNoTextureData();

        // Set up the VAO and VBO with vertex attributes
        setUpVAO();

    }
    else 
    {
        ourShader = new Shader("shaders/vertexShader.shader", "shaders/fragmentShader.shader");

        // Set up the vertex and index data
        setUpTextureData();

        // Set up the VAO and VBO with vertex attributes
        setUpVAO();

        // Set the texture unit for the shader
        ourShader->use();
        ourShader->setInt("texture1", 0);
    }

    return true;
}



Terrain::createVerts()
{


    for (int z = 0; z < size; z++) {
        for (int x = 0; x < size; x++) {
            heightColor = getHeightColor(x, z);
            vertices.push_back((float)x * scaleX);
            vertices.push_back(getHeight(x, z));
            vertices.push_back((float)z * scaleZ);
        }
    }
}




void Terrain::setUpNoTexturePoints()
{
    unsigned char hcolor;

    for (int z = 0; z < size; z++) {
        for (int x = 0; x < size; x++) {
            hcolor = getHeightColor(x, z);

            vertices.push_back((float)x * scaleX);
            vertices.push_back(getHeight(x, z));
            vertices.push_back((float)z * scaleZ);
            vertices.push_back((float)hcolor);

            if (x < size - 1 && z < size - 1) {
                unsigned int topLeft = z * size + x;
                unsigned int topRight = topLeft + 1;
                unsigned int bottomLeft = topLeft + size;
                unsigned int bottomRight = bottomLeft + 1;

                indices.push_back(topLeft);
                indices.push_back(bottomLeft);
                indices.push_back(topRight);
                indices.push_back(topRight);
                indices.push_back(bottomLeft);
                indices.push_back(bottomRight);
            }
        }
    }
}


void GenerateTerrain::setUpTerrainTextures()
{
    createVerts();
    createNorms();
    createTextCoords();
    setUpVAO();
    setUpTexture();
}


void GenerateTerrain::createVerts()
{
    unsigned char 
}