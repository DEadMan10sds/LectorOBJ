#include "Terrain.h"


Terrain::Terrain()
{

}

Terrain::Terrain(const char* noise_path) 
{
	noise = noise_path;
}

/*void Terrain::LoadNoiseImage()
{
	data = stbi_load(noise, &width, &height, &nrChannels, 0);
	if (data) cout << "Ruido de terreno cargado" << endl;
	else cout << "La imagen del terreno no se logro cargar" << endl;
}

void Terrain::CreateTerrainMesh()
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            unsigned char* pixelOffset = data + (j + width * i) * bytePerPixel;
            unsigned char y = pixelOffset[0];

            // vertex
            vertices.push_back(-height / 2.0f + height * i / (float)height);   // vx
            vertices.push_back((int)y * yScale - yShift);   // vy
            vertices.push_back(-width / 2.0f + width * j / (float)width);   // vz
        }
    }
    cout << "Loaded " << vertices.size() / 3 << " vertices" << endl;
    stbi_image_free(data);
}*/

void Terrain::IndexGeneration()
{
    const int NumStrips = (height - 1) / rez;
    const int NumTrisPerStrip = (width / rez) * 2 - 2;
    for (unsigned i = 0; i < height - 1; i += rez)
    {
        for (unsigned j = 0; j < width; j += rez)
        {
            for (unsigned k = 0; k < 2; k++)
            {
                indices.push_back(j + width * (i + k * rez));
            }
        }
    }
    cout << "Loaded " << indices.size() << " indices" << endl;
    numStrips = &NumStrips;
    numTrisPerStrip = &NumTrisPerStrip;
    cout << "Created lattice of " << numStrips << " strips with " << numTrisPerStrip << " triangles each" << endl;
    cout << "Created " << (*numStrips) * (*numTrisPerStrip) << " triangles total" << endl;
}


void Terrain::TerrainVAOVBO()
{
    glGenVertexArrays(1, &terrainVAO);
    glBindVertexArray(terrainVAO);

    glGenBuffers(1, &terrainVBO);
    glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &terrainIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), &indices[0], GL_STATIC_DRAW);
}

unsigned int Terrain::getTerrainVAO()
{
    return terrainVAO;
}

const int Terrain::getNumStrips()
{
    return (*numStrips);
}

const int Terrain::getNumTrisPerStrip()
{
    return (*numTrisPerStrip);
}

void Terrain::setWidth(int _width)
{
    width = _width;
}

void Terrain::setHeight(int _height)
{
    height = _height;
}

void Terrain::setNrChannels(int _nrChannels)
{
    nrChannels = _nrChannels;
}

int Terrain::getWidth()
{
    return width;
}

int Terrain::getHeight()
{
    return height;
}

int Terrain::getNrChannels()
{
    return nrChannels;
}

void Terrain::setTexture(GLuint new_Texture)
{
    texture = new_Texture;
}

GLuint Terrain::returnTexture()
{
    return texture;
}
