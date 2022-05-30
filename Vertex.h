#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/freeglut.h>

using namespace std;

class Vertex
{
	private:
		GLfloat x, y, z;
		int index;
	public:
		Vertex(int _index);
		void load_Vertex_info(GLfloat _x, GLfloat _y, GLfloat _z);
		void showVertex_info();
		GLfloat returnX();
		GLfloat returnY();
		GLfloat returnZ();
		void setY(GLfloat _y);

};

