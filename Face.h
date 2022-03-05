#pragma once
#include <iostream>
#include <vector>

using namespace std;

class Face
{
	private:
		int face_index;
		vector<int> list_vertices_face;
		vector<int>::iterator itr_vertices_face;
		vector<int> list_normals;
		vector<int>::iterator itr_normals;
	public:
		Face(int _index_face);
		void addVertex(int _vertex);
		int showFaceIndex();
		void showFaceVertices();
		vector<int> returnVertexList();
		void addNormal(int _normal);
};

