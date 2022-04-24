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
		itr_faces_list->showFaceVertices();
	}
}

vector<Face> Object::returnFaceList()
{
	return list_faces;
}

void Object::addNormal(int normal)
{
	list_normals.push_back(normal);
}

void Object::showNormalList(vector<Vertex> global_list_normals)
{
	cout << "LISTA GLOBAL DE NORMALES" << endl;
	for (vector<int>::iterator itr_object_face_list = list_normals.begin(); itr_object_face_list != list_normals.end(); (++itr_object_face_list))
	{
		//cout << "Vertice: " << *itr_text_vertices << endl;
		//file_vertex_list[--(*itr_text_vertices)].showVertex_info();
		cout << "Normal: " << *itr_object_face_list << endl;
		global_list_normals[--(*itr_object_face_list)].showVertex_info();
	}
}

void Object::addTexture(int texture)
{
	list_textures.push_back(texture);
}

void Object::showTextureList(vector<TextureVertex> texture_list)
{
	for (vector<TextureVertex>::iterator itr_texture = texture_list.begin(); itr_texture != texture_list.end(); (++itr_texture))
	{
		itr_texture->showData();
	}
}