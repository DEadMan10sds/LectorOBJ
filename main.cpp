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
int resX = 1024, resY = 620;
int frameCount = 0;
double initialTime, finalTime, actual_frame_duration; // tiempo inicial, tiempo final, contador de frames
double frame_duration = (1 / (float)FPS);
File archivo("sphere.obj");

using namespace std;
using namespace glm;

mat4 model, view, projection;

GLFWwindow* InitWindow(const int resX, const int resY);
void display(GLFWwindow* window);
void createMatrices();

int main()
{
	

	GLFWwindow* window = InitWindow(resX, resY);
	if (archivo.loadFile())
	{
		//archivo.show_text_data();
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
	glClearColor(0.1f, 0.0f, 0.2f, 0.0f);
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


	GLfloat cube[] = {
		1.000000, -1.000000, -1.000000, 1.0f, 0.0f, 0.0f,
		1.000000, -1.000000,  1.000000, 1.0f, 0.0f, 1.0f,
	   -1.000000, -1.000000,  1.000000, 1.0f, 0.0f, 0.0f,
	   -1.000000, -1.000000, -1.000000, 1.0f, 1.0f, 0.0f,
		1.000000,  1.000000, -0.999999, 1.0f, 0.0f, 1.0f,
		0.999999,  1.000000,  1.000001, 1.0f, 0.0f, 0.0f,
	   -1.000000,  1.000000,  1.000000, 1.0f, 1.0f, 0.0f,
	   -1.000000,  1.000000, -1.000000, 1.0f, 0.0f, 1.0f
	};

	float triangle2d[] = {
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
	};

	archivo.createBuffer();
	GLfloat* buffer = new GLfloat[archivo.returnBufferSize()];
	for (int i = 0; i < archivo.returnBufferSize(); i++)
	{
		buffer[i] = archivo.getBufferData(i);
	}

	const GLfloat* vertices = buffer;
	

	GLuint VAO, VBO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	int tam = archivo.returnBufferSize() * 4;
	GLint vertexbuffer;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, tam, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (void*)(sizeof(GLfloat)*3));
	glEnableVertexAttribArray(1);

	GLuint programID =
		LoadShaders(
		"D:/OneDrive - Universidad Autonoma de San Luis Potosi - UASLP/Tuf/UASLP/8sem/Programacion de Videojuegos/LectorOBJ/vs_basico.glsl",
		"D:/OneDrive - Universidad Autonoma de San Luis Potosi - UASLP/Tuf/UASLP/8sem/Programacion de Videojuegos/LectorOBJ/fs_basico.glsl");


	do
	{
		initialTime = glfwGetTime();
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(programID);

		//VARIABLES UNIFORMES


		/*int idUniform = glGetUniformLocation(programID, "colorUniform");
		glUniform3f(idUniform, 1.0, 1.0, 1.0);*/

		/*int idFactorAmb = glGetUniformLocation(programID, "factorAmbiental");
		glUniform1f(idFactorAmb, 1.0f);*/

		createMatrices();
		int modelLoc = glGetUniformLocation(programID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
		modelLoc = glGetUniformLocation(programID, "view");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(view));
		modelLoc = glGetUniformLocation(programID, "projection");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(projection));


		glBindVertexArray(VAO);

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//Directiva de dibujo, cantidad de indices, tipo de dato de indices, inicio de indices
		glDrawArrays(GL_TRIANGLES, 0, (archivo.returnFaceaAmount() * 3));

		glfwSwapBuffers(window);
		glfwPollEvents();



		//FPS
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
			crntTime = glfwGetTime();
			counter = 0;
		}
	}while(!glfwWindowShouldClose(window));

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(programID);

	
}

void createMatrices()
{
	//Modelo
	model = mat4(1);
	translate(model, vec3(0.0f, 0.0f, 0.0f));

	//Vista
	vec3 eye(-2.0f, 1.0f, 5.0f);
	vec3 center(0.0f, 0.0f, 0.0f);
	vec3 up(0.0f, 1.0f, 0.0f);
	view = lookAt(eye, center, up);

	//Proyección
	projection = perspective(radians(45.0f), (float)(resX / resY), 0.1f, 100.0f);
}