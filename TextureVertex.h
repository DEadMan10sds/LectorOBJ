#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/freeglut.h>

using namespace std;

class TextureVertex
{
	private:
		GLfloat U,  V;
		int index;
	public:
		TextureVertex(int _index);
		TextureVertex(GLfloat _u, GLfloat _v, int _index);
		void setUV(GLfloat _u, GLfloat _v);
		GLfloat getU();
		GLfloat getV();
		GLfloat getindex();
		void showData();
};

