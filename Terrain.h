#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/freeglut.h>

#include <GLM/vec3.hpp>
#include <GLM/vec4.hpp>
#include <GLM/mat4x4.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>


using namespace std;
using namespace glm;

class Terrain
{
	private:
		int width, height, nrChannels;
		const char* noise;
		unsigned char* data;
		vector<float> vertices;
		float yScale = 64.0f / 256.0f, yShift = 30.0f;
		int rez = 1;
		unsigned bytePerPixel = nrChannels;
		vector<unsigned> indices;
		const int* numStrips;
		const int* numTrisPerStrip;

		unsigned int terrainVAO, terrainVBO, terrainIBO;

		mat4 terrainMatrix;
		GLuint texture;
	public:
		Terrain();
		Terrain(const char* noise_path);
		//void LoadNoiseImage();
		//void CreateTerrainMesh();
		void IndexGeneration();
		void TerrainVAOVBO();
		unsigned int getTerrainVAO();
		const int getNumStrips();
		const int getNumTrisPerStrip();
		void setWidth(int _width);
		void setHeight(int _height);
		void setNrChannels(int _nrChannels);
		int getWidth();
		int getHeight();
		int getNrChannels();

		void setTexture(GLuint new_texture);
		GLuint returnTexture();
};


