#include <stdio.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/freeglut.h>

#include "File.h"
#include "Vertex.h"

//CONSTANTES
#define FPS 60 //FPS máximos

//GLOBALES
int frameCount = 0;
double initialTime, finalTime, actual_frame_duration; // tiempo inicial, tiempo final, contador de frames
double frame_duration = (1 / (float)FPS);
clock_t frame_actual_duration;

using namespace std;

GLFWwindow* InitWindow(const int resX, const int resY);
void display(GLFWwindow* window);
void drawCube();
void controls(GLFWwindow* window, int key, int scancode, int action, int mods);


int main()
{
	File archivo("four_figs.obj");
	archivo.loadFile();
	//archivo.show_text_data();

	GLFWwindow* window = InitWindow(1024, 620);
	if (window) display( window );


	//****************************************************************->Este es el código del profe para hacer la pantalla
	////Los vertices tienen propiedades
	////1.- Su posicion
	//float vertices[] = {
	//	-0.5f, -0.5f, 0.0f,
	//	 0.5f, -0.5f, 0.0f,
	//	 0.0f,  0.5f, 0.0f
	//};

	//unsigned int VBO; //Guarda la informacion
	//unsigned int VAO; //Guarda la configuracion de las propiedades de los vertices

	//glGenVertexArrays(1, &VAO);
	//glGenBuffers(1, &VBO);

	//glBindVertexArray(VAO);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);

	///*GLuint programID =
	//	LoadShaders("C:\\Users\\Alberto\\Desktop\\PVJ\\OpenGL 3\\Basic shader - empty\\vs1.glsl",
	//		"C:\\Users\\Alberto\\Desktop\\PVJ\\OpenGL 3\\Basic shader - empty\\fs1.glsl");*/

	//do
	//{
	//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//	// Use our shader
	//	//glUseProgram(programID);
	//	glBindVertexArray(VAO);

	//	glDrawArrays(GL_TRIANGLES, 0, 3);//Numero de vertices


	//	glfwSwapBuffers(window);
	//	glfwPollEvents();


	//	//FPS

	//} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	//glDeleteBuffers(1, &VBO);
	//glDeleteVertexArrays(1, &VAO);
	////glDeleteProgram(programID);

	//glfwTerminate();
	//*****************************************************************************************************->Termina el código del profe 

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
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing

	// Open a window and create its OpenGL context
	GLFWwindow* window = glfwCreateWindow(resX, resY, "TEST", NULL, NULL);

	if (window == NULL)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return NULL;
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, controls);

	// Get info of GPU and supported OpenGL version
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("OpenGL version supported %s\n", glGetString(GL_VERSION));

	glEnable(GL_DEPTH_TEST); // Depth Testing
	glDepthFunc(GL_LEQUAL);
	glDisable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	return window;
}

void controls(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
		if (key == GLFW_KEY_ESCAPE)
			glfwSetWindowShouldClose(window, GL_TRUE);
}


//int InitWindow() ****************************************>>>>>>>>>>>>>>>>>Código del profe
//{
//	//GLFW
//	if (!glfwInit())
//	{
//		fprintf(stderr, "Failed to initialize GLFW\n");
//		return -1;
//	}
//
//	glfwWindowHint(GLFW_SAMPLES, 4);
//
//	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/
//
//	window = glfwCreateWindow(800, 600, "Lector OBJ Gráfico", NULL, NULL);
//	if (!window)
//	{
//		fprintf(stderr, "Failed to open GLFW window\n");
//		glfwTerminate();
//		return -1;
//	}
//
//	glfwMakeContextCurrent(window);
//	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
//	//glfwSetMouseButtonCallback(window, mouse_button_callback);
//
//	//Glew
//	glewExperimental = true;
//	if (glewInit() != GLEW_OK)
//	{
//		fprintf(stderr, "Failed to initialize GLEW\n");
//		getchar();
//		glfwTerminate();
//		return -1;
//	}
//
//	//OpenGl
//	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
//	//glViewport(0, 0, 1000, 1000);
//	glEnable(GL_DEPTH_TEST);
//
//	return 1;
//}  ****************************************>>>>>>>>>>>>>>>>>Código del profe

void display(GLFWwindow* window)
{

	double endtime = 0, crntTime = 0, timeDiff;
	unsigned int counter = 0;
	double local_current_time = 0, local_timeDiff = 0;
	
	crntTime = glfwGetTime();
	while (!glfwWindowShouldClose(window))
	{
		
		initialTime = glfwGetTime();
		

		// INICIO DEL FRAME
		// Scale to window size
		GLint windowWidth, windowHeight;
		glfwGetWindowSize(window, &windowWidth, &windowHeight);
		glViewport(0, 0, windowWidth, windowHeight);

		// Draw stuff
		glClearColor(0.0, 0.8, 0.3, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION_MATRIX);
		glLoadIdentity();
		gluPerspective(60, (double)windowWidth / (double)windowHeight, 0.1, 100);

		glMatrixMode(GL_MODELVIEW_MATRIX);
		glTranslatef(0, 0, -5);

		drawCube();

		// Update Screen
		glfwSwapBuffers(window);


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

		// Check for any input, or window movement
		glfwPollEvents();
	}
}

void drawCube()
{
	GLfloat vertices[] =
	{
		-1, -1, -1,   -1, -1,  1,   -1,  1,  1,   -1,  1, -1,
		1, -1, -1,    1, -1,  1,    1,  1,  1,    1,  1, -1,
		-1, -1, -1,   -1, -1,  1,    1, -1,  1,    1, -1, -1,
		-1,  1, -1,   -1,  1,  1,    1,  1,  1,    1,  1, -1,
		-1, -1, -1,   -1,  1, -1,    1,  1, -1,    1, -1, -1,
		-1, -1,  1,   -1,  1,  1,    1,  1,  1,    1, -1,  1
	};

	GLfloat colors[] =
	{
		0, 0, 0,   0, 0, 1,   0, 1, 1,   0, 1, 0,
		1, 0, 0,   1, 0, 1,   1, 1, 1,   1, 1, 0,
		0, 0, 0,   0, 0, 1,   1, 0, 1,   1, 0, 0,
		0, 1, 0,   0, 1, 1,   1, 1, 1,   1, 1, 0,
		0, 0, 0,   0, 1, 0,   1, 1, 0,   1, 0, 0,
		0, 0, 1,   0, 1, 1,   1, 1, 1,   1, 0, 1
	};

	static float alpha = 0;
	//attempt to rotate cube
	glRotatef(alpha, 0, 1, 0);

	/* We have a color array and a vertex array */
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glColorPointer(3, GL_FLOAT, 0, colors);

	/* Send data : 24 vertices */
	glDrawArrays(GL_QUADS, 0, 24);

	/* Cleanup states */
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	alpha += 1;
}