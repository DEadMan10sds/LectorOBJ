#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/freeglut.h>

#include "Object.h"
#include "Vertex.h"


using namespace std;

class File
{
	private:
		string file_name;
		vector<Object> list_object;
		vector<Object>::iterator itr_object;
		vector<Vertex> list_vertices;
		vector<Vertex>::iterator itr_vertices;
		GLfloat* buffer;
	public: 
		File(string _name);
		bool loadFile();
		void show_text_data();
		void show_vertex_data();
		void show_specific_vertex_data(int _index);
		int returnLenght();
		GLfloat return_XVertexList(int index);
		GLfloat return_YVertexList(int index);
		GLfloat return_ZVertexList(int index);
		void createBuffer();
		GLfloat* getBuffer();
};

