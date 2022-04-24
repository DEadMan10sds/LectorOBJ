#include "TextureVertex.h"

TextureVertex::TextureVertex(int _index)
{
	index = _index;
}

TextureVertex::TextureVertex(GLfloat _u, GLfloat _v, int _index)
{
	U = _u;
	V = _v;
	index = _index;
}

void TextureVertex::setUV(GLfloat _u, GLfloat _v)
{
	U = _u;
	V = _v;
}

GLfloat TextureVertex::getU()
{
	return U;
}

GLfloat TextureVertex::getV()
{
	return V;
}

GLfloat TextureVertex::getindex()
{
	return index;
}

void TextureVertex::showData()
{
	cout << "-----------------------------Vertice de textura: " << index << endl;
	cout << "U: " << U << " V: " << V << endl;
}