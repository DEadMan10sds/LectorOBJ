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
	GLfloat* aux;
	int size = (list_vertices.size() * 3);
	aux = (GLfloat*)malloc((sizeof(GLfloat)));
	int j = 0;

	for (int i = 0; i != list_vertices.size(); ++i)
	{
		aux[j++] = list_vertices[i].returnX();
		aux[j++] = list_vertices[i].returnY();
		aux[j++] = list_vertices[i].returnZ();
	}

	buffer = aux;
}

GLfloat* File::getBuffer()
{
	return buffer;
}