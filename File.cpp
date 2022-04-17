#include "File.h"

File::File(string _name)//Constructor
{
	file_name = _name;
	loaded = false;
	ModelMatrix = mat4(1.0f);
	loadFile();
}

//Cargar achivos
bool File::loadFile()
{
	int number_Vertices = 0, number_face = 0, number_Objects = 0;
	int normals = 0;
	float x, y, z;
	bool flag = false;
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
				int aux = 1;
				int new_face_vertex_index;
				number_face++;
				Face new_face(number_face);
				line.erase(0, 2);
				for (int i = 0; i < line.length(); i++)
					if (line[i] == '/') 
						line[i] = ' ';
				stringstream face_vertices(line);
				while (face_vertices >> new_face_vertex_index)
				{
					if (!flag)
					{
						new_face.addVertex(new_face_vertex_index);
						flag = !flag;
					}
					else
					{
						new_face.addNormal(new_face_vertex_index);
						flag = !flag;
					}
					/*if (aux % 2)new_face.addVertex(new_face_vertex_index);
					else new_face.addNormal(new_face_vertex_index);
					aux++;*/
				}
				itr_object = --list_object.end();
				itr_object->addFace(new_face);
				//itr_object = list_object.end();
			}
			if (line[0] == 'v' && line[1] != 'n')
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
				//itr_object = list_object.end();
			}
			if (line[0] == 'v' && line[1] == 'n')
			{
				line.erase(0, 3);
				normals++;
				Vertex aux_normal(normals);
				stringstream normal_aux;
				normal_aux << line;
				normal_aux >> x >> y >> z;
				aux_normal.load_Vertex_info(x, y, z);
				list_normal.push_back(aux_normal);
				itr_object = --list_object.end();
				itr_object->addNormal(normals);
			}
		}
		loaded = true;
	}
	else
	{
		cout << "No se pudo abrir el archivo " << endl;
		loaded = false;
		NEWFILE.close();
		return false;
	}
	NEWFILE.close();
	faces_amount = number_face;
	return true;
}

//Muestra el TXT del archivo
void File::show_text_data()
{
	for (vector<Object>::iterator for_itr = list_object.begin(); for_itr != list_object.end(); (++for_itr))
	{
		cout << for_itr->getObjectName() << endl;
		for_itr->showVertexList(list_vertices);
		for_itr->showNormalList(list_normal);
		for_itr->showFacesList();
	}
}

void File::show_vertex_data()//Muestra la información de cada vértice
{
	for (vector<Vertex>::iterator itr_vertex_list = list_vertices.begin(); itr_vertex_list != list_vertices.end(); (++itr_vertex_list))
		itr_vertex_list->showVertex_info();
}

void File::show_normal_data()//Muestra la información de cada vértice
{
	for (vector<Vertex>::iterator itr_normal_list = list_normal.begin(); itr_normal_list != list_normal.end(); (++itr_normal_list))
		itr_normal_list->showVertex_info();
}

void File::show_specific_vertex_data(int _index)//Info de un vértice en específico
{
	list_vertices[--_index].showVertex_info();
}

int File::returnLenght()//Longitud de la lista de vértices
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

//GENERA EL ARRAY CON LOS DATOS EN ORDEN
void File::createBuffer()
{
	buffer_size = faces_amount * 3 * 9;
	int color = 0;
	list_Buffer_data.clear();
	vector<Object>::iterator object_itr;
	for (object_itr = list_object.begin(); object_itr != list_object.end(); (++object_itr))//Recorre cada objeto
	{
		vector<Face> objectFaces = object_itr->returnFaceList();
		for (vector<Face>::iterator face_itr = objectFaces.begin(); face_itr != objectFaces.end(); (++face_itr))//Recorre las caras del objeto
		{
			vector<int> vertices_face = face_itr->returnVertexList();
			vector<int> normal_face = face_itr->returnNormalList();
			for (vector<int>::iterator index_face_list = vertices_face.begin(); index_face_list != vertices_face.end(); (++index_face_list))//Recorre los vértices de la cara del objeto
			{
				//ORDEN DE BUFFER: POSICIÓN - NORMAL - COLOR
				vector<Vertex>::iterator index_vertices = list_vertices.begin();//Asigna un iterador al inicio de la lista de vértices
				advance(index_vertices, (*index_face_list) - 1);//Mueve el iterador hasta el vértice
				Vertex vertice_aux = *index_vertices;
				list_Buffer_data.push_back(vertice_aux.returnX());//Guarda todos los valores de coordenadas
				list_Buffer_data.push_back(vertice_aux.returnY());
				list_Buffer_data.push_back(vertice_aux.returnZ());
				
				//Almacenamiento de las normales
				vector<Vertex>::iterator index_normal = list_normal.begin();
				vector<int>::iterator iterador_lista_normales = normal_face.begin();
				advance(index_normal, (*iterador_lista_normales) - 1);
				Vertex normal_aux = *index_normal;
				list_Buffer_data.push_back(normal_aux.returnX());
				list_Buffer_data.push_back(normal_aux.returnY());
				list_Buffer_data.push_back(normal_aux.returnZ());
				//Define el color 1 x 1, 

				//EDITAR PARA QUE CARGUE LAS NORMALES
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
	}
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


bool File::getLoadedStatus()
{
	return loaded;
}

GLint File::getVAO_VertexArrayid()
{
	return VAO;
}

GLint File::getVBO_VertexBufferid()
{
	return VBO;
}

void File::generate_VAOVBO()
{
	buffer = new GLfloat[returnBufferSize()];
	for (int i = 0; i < returnBufferSize(); i++)//Copia el buffer de la clase File al arreglo dinamico
	{
		buffer[i] = getBufferData(i);
	}

	const GLfloat* vertices = buffer;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	int tam = returnBufferSize() * 4;

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, tam, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 9, (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(sizeof(GLfloat) * 6));
	glEnableVertexAttribArray(2);

	//ModelMatrix = mat4(1.0f);
	//ModelMatrix = translate(ModelMatrix, vec3(0.0, 0.0, 0.0));
}

mat4 File::getModelMatrix()
{
	return ModelMatrix;
}

void File::freeBufferShaders()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	/*glDeleteProgram(programID);*/
}

void File::translate_model(vec3 position, bool camera_mode)
{
	vec3 firstPerson(position.x - 1.4f, position.y + 1.0f, position.z + 4.0f);
	vec3 thirdPerson(position.x, position.y + 2.7f, position.z + 9.0f);
	if (camera_mode) ModelMatrix = translate(ModelMatrix, -firstPerson);
	else ModelMatrix = translate(ModelMatrix, -thirdPerson);
}

void File::rotate_modelTP(vec3 new_position, float yrot)
{
	ModelMatrix = rotate(ModelMatrix, yrot, vec3(0.0f, 1.0f, 0.0f));
}

void File::rotate_modelFP(vec3 new_position, float xrot, float yrot)
{
	ModelMatrix = translate(ModelMatrix, -new_position);
	ModelMatrix = rotate(ModelMatrix, yrot, vec3(0.0f, -1.0f, 0.0f));
	ModelMatrix = rotate(ModelMatrix, xrot, vec3(-1.0f, 0.0f, 0.0f));
	ModelMatrix = translate(ModelMatrix, new_position);
}

void File::translation_model(vec3 position)
{
	ModelMatrix = translate(ModelMatrix, position);
}