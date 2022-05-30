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
	int normals = 0, texture_vertex = 0;
	float x, y, z, u, v;
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
			if (line[0] == 'v' && ((line[1] != 'n') && (line[1] != 't')))
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
			if (line[0] == 'v' && line[1] == 't')
			{
				line.erase(0, 3);
				texture_vertex++;
				TextureVertex aux_TextVertex(texture_vertex);
				stringstream texture_aux;
				texture_aux << line;
				texture_aux >> u >> v;
				aux_TextVertex.setUV(u, v);
				list_texture_coord.push_back(aux_TextVertex);
				itr_object = --list_object.end();
				itr_object->addTexture(texture_vertex);
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
			if (line[0] == 'f')
			{
				int aux = 0;
				int data_index;
				number_face++;
				Face new_face(number_face);
				line.erase(0, 2);
				for (int i = 0; i < line.length(); i++)
					if (line[i] == '/')
						line[i] = ' ';
				stringstream face_vertices(line);
				//Vertice - textura - normal
				while (face_vertices >> data_index)
				{
					aux++;
					if (aux == 1) new_face.addVertex(data_index);
					else if (aux == 2) new_face.addTexture(data_index);
					else if (aux == 3)
					{
						new_face.addNormal(data_index);
						aux = 0;
					}
				}
				itr_object = --list_object.end();
				itr_object->addFace(new_face);
				//itr_object = list_object.end();
			}
		}
		loaded = true;
		faces_amount = number_face;
	}
	else
	{
		cout << "No se pudo abrir el archivo " << endl;
		loaded = false;
	}
	NEWFILE.close();
	return true;
}

//Muestra el TXT del archivo
void File::show_text_data()
{
	for (vector<Object>::iterator for_itr = list_object.begin(); for_itr != list_object.end(); (++for_itr))
	{
		cout << for_itr->getObjectName() << endl;
		for_itr->showVertexList(list_vertices);
		for_itr->showTextureList(list_texture_coord);
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
	cout << "Asignar info buffer" << endl;
	buffer_size = (faces_amount * 3 * 11);
	cout << buffer_size << endl;
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
			vector<int> texture_face = face_itr->returnTextureList();
			int i = 0;
			for (vector<int>::iterator index_face_list = vertices_face.begin(); index_face_list != vertices_face.end(); (++index_face_list))//Recorre los vértices de la cara del objeto
			{
				//ORDEN DE BUFFER: POSICIÓN 3 - NORMAL 3 - COLOR 3 - TEXTURA 2
				vector<Vertex>::iterator index_vertices = list_vertices.begin();//Asigna un iterador al inicio de la lista de vértices
				advance(index_vertices, (*index_face_list) - 1);//Mueve el iterador hasta el vértice
				Vertex vertice_aux = *index_vertices;
				list_Buffer_data.push_back(vertice_aux.returnX());//Guarda todos los valores de coordenadas
				list_Buffer_data.push_back(vertice_aux.returnY());
				list_Buffer_data.push_back(vertice_aux.returnZ());

				
				//Almacenamiento de las normales
				vector<Vertex>::iterator index_normal = list_normal.begin();
				vector<int>::iterator iterador_lista_normales = (normal_face.begin() + i);
				advance(index_normal, (*iterador_lista_normales) - 1);
				Vertex normal_aux = *index_normal;
				list_Buffer_data.push_back(normal_aux.returnX());
				list_Buffer_data.push_back(normal_aux.returnY());
				list_Buffer_data.push_back(normal_aux.returnZ());

				/*cout << "Vertice: " << endl;
				vertice_aux.showVertex_info();
				cout << "Normal: " << endl;
				normal_aux.showVertex_info();
				cout << "Color: ";*/
				
				if (color == 0)
				{
					list_Buffer_data.push_back(1);
					//cout << " 1 ";
				}
				else

				{
					list_Buffer_data.push_back(0);
					//cout << " 0 ";
				}
				if (color == 1)
				{
					list_Buffer_data.push_back(1);
					//cout << " 1 ";
				}
				else
				{
					list_Buffer_data.push_back(0);
					//cout << " 0 ";
				}
				if (color == 2)
				{
					list_Buffer_data.push_back(1);
					//cout << " 1 " << endl;
				}
				else
				{
					list_Buffer_data.push_back(0);
					//cout << " 0 " << endl;
				}
				color++;
				if (color == 3)
					color = 0;

				
				//Textura - Similar a las normales
				vector<TextureVertex>::iterator texture_index = list_texture_coord.begin();
				vector<int>::iterator iterador_lista_texturas = (texture_face.begin()+i);
				advance(texture_index, (*iterador_lista_texturas) - 1);
				TextureVertex texture_aux = *texture_index;
				list_Buffer_data.push_back(texture_aux.getU());
				list_Buffer_data.push_back(texture_aux.getV());
				//texture_aux.showData();
				i++;
			}
		}
	}
}

//GENERA EL ARRAY CON LOS DATOS EN ORDEN EDITANDO LAS ALTURAS
void File::createTerrainBuffer(vector<GLbyte> heights, int width, int height)
{
	cout << "Asignar info buffer Terreno" << endl;
	buffer_size = (faces_amount * 3 * 11);
	cout << buffer_size << endl;
	vector<GLbyte>::iterator heightIterator = heights.begin();
	int color = 0;
	float U, V;
	list_Buffer_data.clear();
	vector<Object>::iterator object_itr;
	for (object_itr = list_object.begin(); object_itr != list_object.end(); (++object_itr))//Recorre cada objeto
	{
		vector<Face> objectFaces = object_itr->returnFaceList();
		for (vector<Face>::iterator face_itr = objectFaces.begin(); face_itr != objectFaces.end(); (++face_itr))//Recorre las caras del objeto
		{
			vector<int> vertices_face = face_itr->returnVertexList();
			vector<int> normal_face = face_itr->returnNormalList();
			vector<int> texture_face = face_itr->returnTextureList();
			int i = 0;
			for (vector<int>::iterator index_face_list = vertices_face.begin(); index_face_list != vertices_face.end(); (++index_face_list))//Recorre los vértices de la cara del objeto
			{
				
				//Textura - Similar a las normales
				vector<TextureVertex>::iterator texture_index = list_texture_coord.begin();
				vector<int>::iterator iterador_lista_texturas = (texture_face.begin() + i);
				advance(texture_index, (*iterador_lista_texturas) - 1);
				TextureVertex texture_aux = *texture_index;

				
				
				

				U = texture_aux.getU();
				V = texture_aux.getV();

				

				//ORDEN DE BUFFER: POSICIÓN 3 - NORMAL 3 - COLOR 3 - TEXTURA 2
				vector<Vertex>::iterator index_vertices = list_vertices.begin();//Asigna un iterador al inicio de la lista de vértices
				advance(index_vertices, (*index_face_list) - 1);//Mueve el iterador hasta el vértice
				Vertex vertice_aux = *index_vertices;
				list_Buffer_data.push_back(vertice_aux.returnX());//Guarda todos los valores de coordenadas
				

				GLfloat newY;
				int col = round(U * width);
				int row = round(V * height);
				int pos = col + row * height;
				
				
				newY = (GLfloat)heightIterator[pos] / 25.0f;
				//cout << i << endl;
				
				vertice_aux.setY(newY);
				list_Buffer_data.push_back(vertice_aux.returnY());
				list_Buffer_data.push_back(vertice_aux.returnZ());


				//Almacenamiento de las normales
				vector<Vertex>::iterator index_normal = list_normal.begin();
				vector<int>::iterator iterador_lista_normales = (normal_face.begin() + i);
				advance(index_normal, (*iterador_lista_normales) - 1);
				Vertex normal_aux = *index_normal;
				list_Buffer_data.push_back(normal_aux.returnX());
				list_Buffer_data.push_back(normal_aux.returnY());
				list_Buffer_data.push_back(normal_aux.returnZ());

				/*cout << "Vertice: " << endl;
				vertice_aux.showVertex_info();
				cout << "Normal: " << endl;
				normal_aux.showVertex_info();
				cout << "Color: ";*/

				if (color == 0)
				{
					list_Buffer_data.push_back(1);
					//cout << " 1 ";
				}
				else

				{
					list_Buffer_data.push_back(0);
					//cout << " 0 ";
				}
				if (color == 1)
				{
					list_Buffer_data.push_back(1);
					//cout << " 1 ";
				}
				else
				{
					list_Buffer_data.push_back(0);
					//cout << " 0 ";
				}
				if (color == 2)
				{
					list_Buffer_data.push_back(1);
					//cout << " 1 " << endl;
				}
				else
				{
					list_Buffer_data.push_back(0);
					//cout << " 0 " << endl;
				}
				color++;
				if (color == 3)
					color = 0;


				
				list_Buffer_data.push_back(texture_aux.getU());
				list_Buffer_data.push_back(texture_aux.getV());
				//texture_aux.showData();
				i++;
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
	buffer = new GLfloat[buffer_size];
	for (int i = 0; i < buffer_size; i++)//Copia el buffer de la clase File al arreglo dinamico
	{
		//cout << i << endl;
		buffer[i] = list_Buffer_data[i];
		//cout << "/" << buffer[i];
	}
	//cout << endl;

	const GLfloat* vertices = buffer;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);


	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, (sizeof(GLfloat) * buffer_size), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (void*)(sizeof(GLfloat) * 0));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (void*)(sizeof(GLfloat) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (void*)(sizeof(GLfloat) * 6));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (void*)(sizeof(GLfloat) * 9));
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	//ModelMatrix = mat4(1.0f);
	//ModelMatrix = translate(ModelMatrix, vec3(0.0, 0.0, 0.0));
}

mat4 File::getModelMatrix()
{
	return ModelMatrix;
}

void File::setModelMatrix(mat4 matrix)
{
	ModelMatrix = matrix;
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
	ModelMatrix = rotate(ModelMatrix, yrot, vec3(0.0f, -1.0f, 0.0f));
	//ModelMatrix = translate(ModelMatrix, new_position);
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

void File::setTexture(GLuint new_Texture)
{
	texture = new_Texture;
}

GLuint File::returnTexture()
{
	return texture;
}
