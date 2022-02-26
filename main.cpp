#include <stdio.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/freeglut.h>

#include "File.h"

using namespace std;

int InitWindow();
void display();

GLFWwindow* window;

int main()
{
	File archivo("four_figs.obj");
	archivo.loadFile();
	archivo.show_text_data();

	if (InitWindow() < 0) return -1;

	//Los vertices tienen propiedades
	//1.- Su posicion
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	unsigned int VBO; //Guarda la informacion
	unsigned int VAO; //Guarda la configuracion de las propiedades de los vertices

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	/*GLuint programID =
		LoadShaders("C:\\Users\\Alberto\\Desktop\\PVJ\\OpenGL 3\\Basic shader - empty\\vs1.glsl",
			"C:\\Users\\Alberto\\Desktop\\PVJ\\OpenGL 3\\Basic shader - empty\\fs1.glsl");*/

	do
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Use our shader
		//glUseProgram(programID);
		glBindVertexArray(VAO);

		glDrawArrays(GL_TRIANGLES, 0, 3);//Numero de vertices


		glfwSwapBuffers(window);
		glfwPollEvents();


		//FPS

	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	//glDeleteProgram(programID);

	glfwTerminate();

	return 0;
}

int InitWindow()
{
	//GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(800, 600, "Prueba", NULL, NULL);
	if (window == NULL)
	{
		fprintf(stderr, "Failed to open GLFW window\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	//glfwSetMouseButtonCallback(window, mouse_button_callback);

	//Glew
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	//OpenGl
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//glViewport(0, 0, 1000, 1000);
	glEnable(GL_DEPTH_TEST);

	return 1;
}