#include "Vertex.h"

Vertex::Vertex(int _index)
{
	index = _index;
	x = 0;
	y = 0;
	z = 0;
}

void Vertex::load_Vertex_info(GLfloat _x, GLfloat _y, GLfloat _z)
{
	x = _x;
	y = _y;
	z = _z;
}

void Vertex::showVertex_info()
{
	cout << "Id: " << index << " X: " << x << " || Y: " << y << " || Z: " << z << endl;
}

GLfloat Vertex::returnX()
{
	return x;
}

GLfloat Vertex::returnY()
{
	return y;
}

GLfloat Vertex::returnZ()
{
	return z;
}

void Vertex::setY(GLfloat _y)
{
	y = _y;
}