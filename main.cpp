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

using namespace std;

GLFWwindow* InitWindow(const int resX, const int resY);
void display(GLFWwindow* window);


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
	glfwTerminate();
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

	// Open a window and create its OpenGL context -> Primer null es fullscreen o no
	GLFWwindow* window = glfwCreateWindow(resX, resY, "LectorOBJ", NULL, NULL);

	if (!window)
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
	glClearColor(0.4f, 0.5f, 0.2f, 0.0f);
	//glViewport(0, 0, 1000, 1000);
	glEnable(GL_DEPTH_TEST);

	// Get info of GPU and supported OpenGL version
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("OpenGL version supported %s\n", glGetString(GL_VERSION));

	return window;
}

void display(GLFWwindow* window)
{

	double endtime = 0, crntTime = 0, timeDiff;
	unsigned int counter = 0;
	double local_current_time = 0, local_timeDiff = 0;
	
	crntTime = glfwGetTime();


	GLfloat vertices[] = {
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
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
		 0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f
		 //-0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f
	};

	archivo.createBuffer();
	const GLfloat* buffer = archivo.getBuffer();

	unsigned int VAO, VBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)12);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	GLuint programID =
		LoadShaders(
		"D:/OneDrive - Universidad Autonoma de San Luis Potosi - UASLP/Tu/UASLP/8sem/Programacion de Videojuegos/LectorOBJ"
		,"D:/OneDrive - Universidad Autonoma de San Luis Potosi - UASLP/Tuf/UASLP/8sem/Programacion de Videojuegos/LectorOBJ");


	do
	{
		initialTime = glfwGetTime();
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(programID);
		glBindVertexArray(VAO);


		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//Directiva de dibujo, cantidad de indices, tipo de dato de indices, inicio de indices
		glDrawArrays(GL_TRIANGLES, 0, 3);

		

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
			//system("cls");
			cout << "FPS: " << counter << endl;
			cout << "Vertices a cargar: " << archivo.returnLenght() << endl;
			crntTime = glfwGetTime();
			counter = 0;
		}
	}while(!glfwWindowShouldClose(window));

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(programID);

	
}