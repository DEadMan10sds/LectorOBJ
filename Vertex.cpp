#include "Vertex.h"

Vertex::Vertex(int _index)
{
	index = _index;
	x = 0;
	y = 0;
	z = 0;
}

void Vertex::load_Vertex_info(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
}

void Vertex::showVertex_info()
{
	cout << "X: " << x << " || Y: " << y << " || Z: " << z << endl;
}