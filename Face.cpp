#include "Face.h"

Face::Face(int _index_face)
{
	face_index = _index_face;
}

void Face::addVertex(int _vertex)
{
	list_vertices_face.push_back(_vertex);
}

int Face::showFaceIndex()
{
	return face_index;
}

void Face::showFaceVertices()
{
	for (vector<int>::iterator itr_vertices = list_vertices_face.begin(); itr_vertices != list_vertices_face.end(); (++itr_vertices))
	{
		cout << "||" << *itr_vertices;
	}
	cout << endl;
}