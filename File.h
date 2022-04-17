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

#include "Object.h"
#include "Vertex.h"

#include <GLM/vec3.hpp>
#include <GLM/vec4.hpp>
#include <GLM/mat4x4.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>


using namespace std;
using namespace glm;

class File
{
	private:
		string file_name;
		GLfloat* buffer;
		int buffer_size;
		int faces_amount;
		int angulo;
		vector<Object> list_object;
		vector<Object>::iterator itr_object;

		vector<Vertex> list_vertices;
		vector<Vertex>::iterator itr_vertices;

		vector<GLfloat> list_Buffer_data;
		vector<GLfloat>::iterator itr_Buffer_data;

		vector<Vertex> list_normal;
		vector<Vertex>::iterator itr_normal;
		bool loaded;

		//Datos únicos por objeto de Opengl
		GLint ShaderProgramid;
		GLuint VAO, VBO;
		
		mat4 ModelMatrix;
		vec3 position;


 
	public: 
		File(string _name);
		bool loadFile();
		void show_text_data();
		void show_vertex_data();
		void show_normal_data();
		void show_specific_vertex_data(int _index);
		int returnLenght();
		GLfloat return_XVertexList(int index);
		GLfloat return_YVertexList(int index);
		GLfloat return_ZVertexList(int index);
		void createBuffer();
		GLfloat* getBuffer();
		int returnBufferSize();
		int returnFaceaAmount();
		GLfloat getBufferData(int index);
		bool getLoadedStatus();

		GLint getVAO_VertexArrayid();
		GLint getVBO_VertexBufferid();

		void generate_VAOVBO();

		mat4 getModelMatrix();

		void freeBufferShaders();
		void translate_model(vec3 position, bool camera_mode);
		void rotate_modelTP(vec3 new_position, float yrot);
		void rotate_modelFP(vec3 new_position, float xrot, float yrot);
};

