#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include "Object.h"
#include "Vertex.h"


using namespace std;

class File
{
	private:
		string file_name;
		vector<Object> list_object;
		vector<Object>::iterator itr_object;
		vector<Vertex> list_vertices;
		vector<Vertex>::iterator itr_vertices;
	public: 
		File(string _name);
		void loadFile();
		void show_text_data();
		void show_vertex_data();
		void show_specific_vertex_data(int _index);
};

