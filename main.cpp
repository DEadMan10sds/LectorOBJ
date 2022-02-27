#include <stdio.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <vector>
#include <iterator>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/freeglut.h>


#include <GLM/vec3.hpp>
#include <GLM/vec4.hpp>
#include <GLM/mat4x4.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include "File.h"
#include "Vertex.h"
#include "shader.hpp"

//CONSTANTES
#define FPS 60 //FPS máximos

//GLOBALES
int frameCount = 0;
double initialTime, finalTime, actual_frame_duration; // tiempo inicial, tiempo final, contador de frames
double frame_duration = (1 / (float)FPS);
File archivo("cube.obj");
glm::mat4 model, view, projection;


using namespace std;

GLFWwindow* InitWindow(const int resX, const int resY);
void display(GLFWwindow* window);
void genMatrices();


int main()
{
	
	//archivo.show_text_data();

	GLFWwindow* window = InitWindow(1024, 620);
	if (archivo.loadFile())
	{
		if (window)
			display(window);
	}
	glfwDestroyWindow(window);
	
	return 0;
}

GLFWwindow* InitWindow(const int resX, const int resY)
{
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	GLFWwindow* window = glfwCreateWindow(resX, resY, "LectorOBJ", NULL, NULL);

	if (window == NULL)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return NULL;
	}

	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	//Glew
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		glfwTerminate();
	}

	//OpenGl
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//glViewport(0, 0, 1000, 1000);
	glEnable(GL_DEPTH_TEST);

	// Get info of GPU and supported OpenGL version
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("OpenGL version supported %s\n", glGetString(GL_VERSION));

	//glEnable(GL_DEPTH_TEST); // Depth Testing
	//glDepthFunc(GL_LEQUAL);
	//glDisable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	return window;
}

void display(GLFWwindow* window)
{

	double endtime = 0, crntTime = 0, timeDiff;
	unsigned int counter = 0;
	double local_current_time = 0, local_timeDiff = 0;
	
	crntTime = glfwGetTime();

	float vertices[] = {
		1.000000, - 1.000000, - 1.000000,
		1.000000, - 1.000000, 1.000000,
		- 1.000000, - 1.000000, 1.000000,
		- 1.000000, - 1.000000, - 1.000000,
		1.000000, 1.000000,- 0.999999,
		0.999999, 1.000000, 1.000001,
		- 1.000000, 1.000000, 1.000000,
		- 1.000000 , 1.000000, - 1.000000,
	};

	float vertices1[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	archivo.createBuffer();
	const GLfloat* buffer = archivo.getBuffer();


	GLuint VBO;
	GLuint VAO;

	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * (archivo.returnLenght() * 3), buffer, GL_STATIC_DRAW);


	//Ubicación inicial, ubicación final, datatype, normalización, tamaño del stride (bloque de propiedades), valor inicial
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);



	//GLuint programID =
	//	LoadShaders(
	//	"D:/OneDrive - Universidad Autonoma de San Luis Potosi - UASLP/Tu/UASLP/8sem/Programacion de Videojuegos/LectorOBJ"
	//	,"D:/OneDrive - Universidad Autonoma de San Luis Potosi - UASLP/Tuf/UASLP/8sem/Programacion de Videojuegos/LectorOBJ");


	do
	{
		initialTime = glfwGetTime();
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//glUseProgram(programID);
		glBindVertexArray(VAO);


		glDrawArrays(GL_TRIANGLES, 0, 8);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawArrays(GL_TRIANGLES, 0, archivo.returnLenght());

		

		glfwSwapBuffers(window);
		glfwPollEvents();


		while (true)
		{
			finalTime = glfwGetTime();
			if ((finalTime - initialTime) >= frame_duration) break;
		}
		counter++;
		
		endtime = glfwGetTime();
		timeDiff = endtime - crntTime;
		if (timeDiff >= 1.0)
		{
			system("cls");
			cout << "FPS: " << counter << endl;
			cout << "Vertices a cargar: " << archivo.returnLenght() << endl;
			crntTime = glfwGetTime();
			counter = 0;
		}
	}while(!glfwWindowShouldClose(window));

	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	//glDeleteProgram(programID);

	glfwTerminate();
}

void genMatrices() {
	//Modelo
	model = glm::mat4(1);
	model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//angle += 0.1f;

	//Vista
	glm::vec3 eye(0.0f, 0.0f, 5.0f);
	glm::vec3 center(0.0f, 0.0f, 0.0f);
	glm::vec3 up(0.0f, 1.0f, 0.0f);
	view = glm::lookAt(eye, center, up);

	//Proyeccion
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
}