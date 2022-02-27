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

// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";


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

	GLfloat vertices1[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f, 0.5f, 0.0f,
		 -0.5f,  0.5f, 0.0f
	};

	GLuint indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	archivo.createBuffer();
	const GLfloat* buffer = archivo.getBuffer();


	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	GLuint VAO, VBO, EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	//GLuint VBO;
	//GLuint VAO;

	//
	//glGenVertexArrays(1, &VAO);
	//glGenBuffers(1, &VBO);

	//glBindVertexArray(VAO);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * (archivo.returnLenght() * 3), buffer, GL_STATIC_DRAW);

	////Ubicación inicial, ubicación final, datatype, normalización, tamaño del stride (bloque de propiedades), valor inicial
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	//glEnableVertexAttribArray(0);



	//GLuint programID =
	//	LoadShaders(
	//	"D:/OneDrive - Universidad Autonoma de San Luis Potosi - UASLP/Tu/UASLP/8sem/Programacion de Videojuegos/LectorOBJ"
	//	,"D:/OneDrive - Universidad Autonoma de San Luis Potosi - UASLP/Tuf/UASLP/8sem/Programacion de Videojuegos/LectorOBJ");


	do
	{
		initialTime = glfwGetTime();
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shaderProgram);
		//glUseProgram(programID);
		glBindVertexArray(VAO);


		//glDrawArrays(GL_TRIANGLES, 0, 8);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);
	//glDeleteProgram(programID);

	
}