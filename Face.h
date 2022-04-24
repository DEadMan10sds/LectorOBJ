#pragma once
#include <iostream>
#include <vector>
#include "Vertex.h"

using namespace std;

class Face
{
	private:
		int face_index;
		vector<int> list_vertices_face;
		vector<int>::iterator itr_vertices_face;
		vector<int> list_normals;
		vector<int>::iterator itr_normals;
		vector<int>list_texture;
		vector<int>::iterator itr_texture;
	public:
		Face(int _index_face);
		void addVertex(int _vertex);
		int showFaceIndex();
		void showFaceVertices();
		vector<int> returnVertexList();
		vector<int> returnNormalList();
		void addNormal(int _normal);
		void showNormals(vector<Vertex> global_list_normals);
		void addTexture(int _texture);
		vector<int> returnTextureList();
};

