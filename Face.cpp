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
	for (int i = 0; i < 3; i++)
	{
		cout << list_vertices_face[i] << "/" << list_texture[i] << "/" << list_normals[i] << endl;
	}
	cout << endl;
}

vector<int> Face::returnVertexList()
{
	return list_vertices_face;
}

void Face::addNormal(int _normal)
{
	list_normals.push_back(_normal);
}

void Face::showNormals(vector<Vertex> global_list_normals)
{
	cout << "Muestra normales" << endl;
	for (vector<int>::iterator itr_normal_list = list_normals.begin(); itr_normal_list != list_normals.end(); (++itr_normal_list))
	{
		cout << "Normal: " << *itr_normal_list << endl;
		global_list_normals[--(*itr_normal_list)].showVertex_info();
	}
}

vector<int> Face::returnNormalList()
{
	return list_normals;
}

void Face::addTexture(int _texture)
{
	list_texture.push_back(_texture);
}

vector<int> Face::returnTextureList()
{
	return list_texture;
}
