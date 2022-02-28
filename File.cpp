#include "File.h"

File::File(string _name)
{
	file_name = _name;
}

bool File::loadFile()
{
	int number_Vertices = 0, number_face = 0, number_Objects = 0;
	float x, y, z;
	ifstream NEWFILE;
	string line;
	NEWFILE.open(file_name.c_str(), ios::in);
	if (NEWFILE)
	{
		while (getline(NEWFILE, line))
		{
			if (line[0] == 'o')
			{
				number_Objects++;
				line.erase(0, 2);
				Object new_object(line);
				list_object.push_back(new_object);
			}
			if (line[0] == 'f')
			{
				int new_face_vertex_index;
				number_face++;
				line.erase(0, 2);
				Face new_face(number_face);
				stringstream face_vertices(line);
				while (face_vertices >> new_face_vertex_index)
				{
					new_face.addVertex(new_face_vertex_index);
				}
				itr_object = --list_object.end();
				itr_object->addFace(new_face);
			}
			if (line[0] == 'v')
			{
				number_Vertices++;
				line.erase(0, 2);
				Vertex new_vertex(number_Vertices);
				stringstream vertices;
				vertices << line;
				vertices >> x >> y >> z;
				new_vertex.load_Vertex_info(x, y, z);
				list_vertices.push_back(new_vertex);
				itr_object = --list_object.end();
				itr_object->addVertex(number_Vertices);
			}
		}
	}
	else
	{
		cout << "No se pudo abrir el archivo " << endl;
		NEWFILE.close();
		return false;
	}
	NEWFILE.close();
	faces_amount = number_face;
	return true;
}

void File::show_text_data()
{
	for (vector<Object>::iterator for_itr = list_object.begin(); for_itr != list_object.end(); (++for_itr))
	{
		cout << for_itr->getObjectName() << endl;
		for_itr->showVertexList(list_vertices);
		for_itr->showFacesList();
	}
}

void File::show_vertex_data()
{
	for (vector<Vertex>::iterator itr_vertex_list = list_vertices.begin(); itr_vertex_list != list_vertices.end(); (++itr_vertex_list))
		itr_vertex_list->showVertex_info();
}

void File::show_specific_vertex_data(int _index)
{
	list_vertices[--_index].showVertex_info();
}

int File::returnLenght()
{
	return list_vertices.size();
}

GLfloat File::return_XVertexList(int index)
{
	return list_vertices[index].returnX();
}

GLfloat File::return_YVertexList(int index)
{
	return list_vertices[index].returnY();
}

GLfloat File::return_ZVertexList(int index)
{
	return list_vertices[index].returnZ();
}

void File::createBuffer()
{
	buffer_size = faces_amount * 3 * 6;
	int color = 0;
	list_Buffer_data.clear();
	vector<Object>::iterator object_itr;
	for (object_itr = list_object.begin(); object_itr != list_object.end(); (++object_itr))
	{
		vector<Face> objectFaces = object_itr->returnFaceList();
		for (vector<Face>::iterator face_itr = objectFaces.begin(); face_itr != objectFaces.end(); (++face_itr))
		{
			vector<int> vertices_face = face_itr->returnVertexList();
			for (vector<int>::iterator index_face_list = vertices_face.begin(); index_face_list != vertices_face.end(); (++index_face_list))
			{
				vector<Vertex>::iterator index_vertices = list_vertices.begin();
				advance(index_vertices, (*index_face_list) - 1);
				Vertex vertice_aux = *index_vertices;
				list_Buffer_data.push_back(vertice_aux.returnX());
				list_Buffer_data.push_back(vertice_aux.returnY());
				list_Buffer_data.push_back(vertice_aux.returnZ());
				if (color == 0)
					list_Buffer_data.push_back(1);
				else
					list_Buffer_data.push_back(0);
				if (color == 1)
					list_Buffer_data.push_back(1);
				else
					list_Buffer_data.push_back(0);
				if (color == 2)
					list_Buffer_data.push_back(1);
				else
					list_Buffer_data.push_back(0);
				color++;
				if (color == 3)
					color = 0;
			}

		}

		/*int k = 0;
		for (int i = 0; i < list_Buffer_data.size(); i++)
		{
			if (k < 6)
			{
				cout << " - " << list_Buffer_data[i];
				k++;
			}
			if (k >= 6)
			{
				k = 0;
				cout << endl;
			}
 	}*/

		/*GLfloat* aux;
		buffer_size = list_Buffer_data.size();
		int j = 0;
		aux = new GLfloat[buffer_size];
		for (int i = 0; i < list_Buffer_data.size(); i++)
		{
			if (aux)
			{
				aux[i] = list_Buffer_data[i];
			}
		}
		buffer = aux;*/
	}

	//int j = 0;
	//GLfloat* aux;
	//int size = list_vertices.size() * 3 * 2;
	//aux = new GLfloat[size];

	//for (int i = 0; i < list_vertices.size(); i++)
	//{
	//	if (aux)
	//	{
	//		aux[j] = list_vertices[i].returnX() / 2;
	//		j++;
	//		aux[j] = list_vertices[i].returnY() / 2;
	//		j++;
	//		aux[j] = list_vertices[i].returnZ() / 2;
	//		j++;
	//		aux[j] = 1.0f;
	//		j++;
	//		aux[j] = 1.0f;
	//		j++;
	//		aux[j] = 1.0f;
	//		j++;
	//		//cout << "X: " << list_vertices[i].returnX() << "Y: " << list_vertices[i].returnY() << "Z: " << list_vertices[i].returnZ() << endl;
	//	}
	//}

	//buffer = aux;

	//for (int i = 0; i < size; i++)
	//{
	//	cout << "V: " << aux[i] << endl;
	//}
}

GLfloat* File::getBuffer()
{
	return buffer;
}

int File::returnBufferSize()
{
	return buffer_size;
}

int File::returnFaceaAmount()
{
	return faces_amount;
}

GLfloat File::getBufferData(int index)
{
	return list_Buffer_data[index];
}
