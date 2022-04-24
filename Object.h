#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <vector>

#include "Vertex.h"
#include "Face.h"
#include "TextureVertex.h"

using namespace std;

class Object
{
	private:
		string object_name;
		vector<int> list_vertices;
		vector<int>::iterator itr_vertices;
		vector<int> list_normals;
		vector<int>::iterator itr_normals;
		vector<int> list_textures;
		vector<int>::iterator itr_textures;
		vector<Face> list_faces;
		vector<Face>::iterator itr_faces;
	public:
		Object(string _name);
		string getObjectName();
		void addVertex(int _index);
		void showVertexList(vector<Vertex> file_vertex_list);
		void addFace(Face _face);
		void showFacesList();
		vector<Face> returnFaceList();
		void addNormal(int normal);
		void showNormalList(vector<Vertex> global_list_normals);
		void addTexture(int texture);
		void showTextureList(vector<TextureVertex> texture_list);
};

