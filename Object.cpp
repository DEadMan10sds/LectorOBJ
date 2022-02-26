#include "Object.h"

Object::Object(string _name)
{
	object_name = _name;
}


string Object::getObjectName()
{
	return object_name;
}

void Object::addVertex(int _index)
{
	list_vertices.push_back(_index);
}

void Object::showVertexList(vector<Vertex> file_vertex_list)
{
	for (vector<int>::iterator itr_text_vertices = list_vertices.begin(); itr_text_vertices != list_vertices.end(); (++itr_text_vertices))
	{
		cout << "Vertice: " << *itr_text_vertices << endl;
		file_vertex_list[-- ( * itr_text_vertices)].showVertex_info();
	}
}

void Object::addFace(Face _face)
{
	list_faces.push_back(_face);
}

void Object::showFacesList()
{
	for (vector<Face>::iterator itr_faces_list = list_faces.begin(); itr_faces_list != list_faces.end(); (++itr_faces_list))
	{
		cout << "Cara: " << itr_faces_list->showFaceIndex() << endl;
		cout << "Vertices: " << endl;
		itr_faces_list->showFaceVertices();
	}
}
