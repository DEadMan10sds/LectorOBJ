#pragma once
#include <iostream>

using namespace std;

class Vertex
{
	private:
		float x, y, z;
		int index;
	public:
		Vertex(int _index);
		void load_Vertex_info(float _x, float _y, float _z);
		void showVertex_info();

};

