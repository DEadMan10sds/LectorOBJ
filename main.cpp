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
void display(GLFWwindow* window, int aux);
void controls(GLFWwindow* window, int key, int scancode, int action, int mods);
void loadVertices(vector<GLfloat> vertices);


int main()
{
	
	//archivo.show_text_data();

	GLFWwindow* window = InitWindow(1024, 620);
	if (archivo.loadFile())
	{
		const int aux = archivo.returnLenght();
		if (window)
			display(window, aux);
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

	// Open a window and create its OpenGL context
	GLFWwindow* window = glfwCreateWindow(resX, resY, "LectorOBJ", NULL, NULL);

	if (window == NULL)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return NULL;
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, controls);

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

void controls(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
		if (key == GLFW_KEY_ESCAPE)
			glfwSetWindowShouldClose(window, GL_TRUE);
}

void display(GLFWwindow* window, int aux)
{

	double endtime = 0, crntTime = 0, timeDiff;
	unsigned int counter = 0;
	double local_current_time = 0, local_timeDiff = 0;
	
	crntTime = glfwGetTime();


	unsigned int VBO;
	unsigned int VAO;

	GLfloat* buffer = archivo.getBuffer();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * (archivo.returnLenght() * 3), buffer, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);//Ubicación inicial, ubicación final, datatype, normalización, tamaño del stride (bloque de propiedades), valor inicial
	
	glEnableVertexAttribArray(0);



	/*GLuint programID =
		LoadShaders(
		"D:\\OneDrive - Universidad Autonoma de San Luis Potosi - UASLP\\Tuf\\UASLP\\8sem\\Programacion de Videojuegos\\LectorOBJ"
		,"D:\\OneDrive - Universidad Autonoma de San Luis Potosi - UASLP\\Tuf\\UASLP\\8sem\\Programacion de Videojuegos\\LectorOBJ");
*/

	do
	{
		initialTime = glfwGetTime();
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(VAO);
		
		//glUseProgram(programID);
		glBindVertexArray(VAO);
		
		glDrawArrays(GL_TRIANGLES, 0, archivo.returnLenght());

		

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

	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	//glDeleteProgram(programID);

}

void loadVertices(vector<GLfloat> vertices)
{
	

	/*int j = 0;

	for (int i = 0; i != vertices.size(); ++i)
	{
		if (j <= 2)
		{
			cout << "-" << vertices[i];
			j++;
		}
		else
		{
			cout << " " << endl;
			j = 0;
		}
	}

	Sleep(600000);*/
}