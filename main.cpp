#include <stdio.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <vector>
#include <iterator>
#include <iostream>
#include <math.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <GLM/vec3.hpp>
#include <GLM/vec4.hpp>
#include <GLM/mat4x4.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtx/string_cast.hpp>

#include "File.h"
#include "Vertex.h"
#include "shader.hpp"


//CONSTANTES
#define FPS 60 //FPS m�ximos

//Namespaces
using namespace std;
using namespace glm;

//GLOBALES
int resX = 1024, resY = 620;
int frameCount = 0;
double initialTime, finalTime, actual_frame_duration; //Tiempo inicial, tiempo final, contador de frames
double frame_duration = (1 / (float)FPS);
File archivo("mono.obj");
File arma("gun.obj");
File plano("plane.obj");

float sensitivity = .5f;

//Variables locales de matrices
mat4 view, projection;

//Variables globales para rotaciones de c�mara
GLfloat RotX = 0.0f, RotY = 0.0f;
double finY, finX;
double yaw_, yaw2, pitch_;
float mouseSpeed = 1.0f;
vec3 mov(0.0f, -0.5f, -7.0f);
vec3 forward_, sides_;
float speed = .1f;
bool camera_mode = true; //True es 1era persona, false es 3era personas


//Funciones
GLFWwindow* InitWindow(const int resX, const int resY);
void display(GLFWwindow* window);
void processInput(GLFWwindow* window);
void createMatrices(GLFWwindow* window);
mat4 FPView(GLFWwindow* window, float rotX, float rotY);
void Mouse(GLFWwindow* window, double initialTime);


int main()
{

    //Crea ventana
    GLFWwindow* window = InitWindow(resX, resY);
    if (window)
        display(window);
    //if (archivo.getLoadedStatus())//Verifica que se cargue el archivo
    //{
    //	//archivo.show_text_data();//Muestra txt del archivo
    //	
    //}

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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    //glfwSetCursorPosCallback(window, cursorPositionCallback);

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
    return window;
}

void display(GLFWwindow* window)
{
    //Vriables locales del FPS
    double endtime = 0, crntTime = 0, timeDiff;
    unsigned int counter = 0;
    double local_current_time = 0, local_timeDiff = 0;

    //arma.setPosition();

    crntTime = glfwGetTime();//Obtiene el tiempo actual segun opengl

    //Crear lista de objetos del programa
    vector<File> lista_objetos_programa;
    lista_objetos_programa.push_back(archivo);
    lista_objetos_programa.push_back(arma);
    lista_objetos_programa.push_back(plano);
    for (int i = 0; i < lista_objetos_programa.size(); i++)
    {
        lista_objetos_programa[i].createBuffer();
        cout << "Buffer: " << i << " Creado" << endl;
    }

    //Cargan los shaders
    GLuint programIDP = LoadShaders( "vs1.glsl", "fs1.glsl"); //phong
    GLuint programIDG = LoadShaders("Gouraud_vs.glsl", "Gouraud_fs.glsl"); //Goraud
    GLuint programIDF = LoadShaders("flat_vs.glsl", "flat_fs.glsl"); //flat


    GLuint programID = programIDP;
    do
    {
        initialTime = glfwGetTime();//Tiempo inicial del frame
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) programID = programIDP;
        if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) programID = programIDG;
        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) programID = programIDF;
        if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) camera_mode = true;
        //glfwSetMouseButtonCallback(window, mouse_button_callback);
        glfwSetCursorPos(window, resX / 2, resY / 2);
        processInput(window);
        

        for (int i = 0; i < lista_objetos_programa.size(); i++)
        {
            lista_objetos_programa[i].generate_VAOVBO();
            File Current_model = lista_objetos_programa[i];
            glUseProgram(programID);//Cargan los shaders

            if (i == 1)
            {
                //Current_model.keepView(mov, pitch_, yaw_);
                Current_model.translate_model(mov, pitch_, yaw_);
            }

            //VARIABLES UNIFORMES
            //Solo quedan dentro del ciclo for (al igual que los shaders) si se van a cambiar dependiendo del objeto

            int idUniform = glGetUniformLocation(programID, "colorUniform");
            glUniform3f(idUniform, 1.0, 1.0, 1.0);

            int idFactorAmb = glGetUniformLocation(programID, "factorAmbiental");
            glUniform1f(idFactorAmb, 1.0f);

            createMatrices(window);//Crea las matrices
            //CARGA LAS MATRICES EN LOS SHADERS
            int modelLoc = glGetUniformLocation(programID, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(Current_model.getModelMatrix()));
            modelLoc = glGetUniformLocation(programID, "view");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(view));
            modelLoc = glGetUniformLocation(programID, "projection");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(projection));


            glBindVertexArray(Current_model.getVAO_VertexArrayid());

            //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//Directiva de dibujo, cantidad de indices, tipo de dato de indices, inicio de indices
            glDrawArrays(GL_TRIANGLES, 0, (Current_model.returnFaceaAmount() * 3));
        }
        glfwSwapBuffers(window);

        glfwPollEvents();

        Mouse(window, initialTime);
        //FPS
        while (true)//Si duran mas que lo indicado
        {
            finalTime = glfwGetTime();
            if ((finalTime - initialTime) >= frame_duration) break;
        }
        counter++;
        
        Mouse(window, initialTime);

        endtime = glfwGetTime();
        timeDiff = endtime - crntTime;
        if (timeDiff >= 1.0)
        {
            system("cls");
            printf("Renderer: %s\n", glGetString(GL_RENDERER));
            printf("OpenGL version supported %s\n", glGetString(GL_VERSION));
            cout << "FPS: " << counter << endl;
            
            crntTime = glfwGetTime();
            counter = 0;
        }
    }while(!glfwWindowShouldClose(window));

    //Ciclo para liberar la memoria de los objetos
    for (int i = 0; i < lista_objetos_programa.size(); i++)
        lista_objetos_programa[i].freeBufferShaders();
    glDeleteProgram(programID);//Si se usan m�s shaders o shaders distintos por cada prograna se tiene que incluir en el m�todo freeBufferShaders()

    
}


void processInput(GLFWwindow* window)
{

    //Los controles se invierten, el sumar es abajo en lugar de arriba
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        mov += forward_ * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) mov -= forward_ * speed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) mov += sides_ * speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) mov -= sides_ * speed;
}

void createMatrices(GLFWwindow* window)
{
    //Vista
    view = FPView(window, pitch_, yaw_);

    //Proyecci�n
    projection = perspective(radians(45.0f), (float)(resX / resY), 0.1f, 100.0f);//�ngulo de visi�n

}


//�ngulos en RADIANES
mat4 FPView(GLFWwindow* window, float rotX, float rotY)
{
    mat4 view;
    rotX = radians(rotX);
    rotY = radians(rotY);


    //Rotacion en X
    view = rotate(mat4(1.0f), rotX, vec3(1.0f, 0.0f, 0.0f));

    //Rotacion en Y
    view = rotate(view, rotY, vec3(0.0f, 1.0f, 0.0f));


    forward_ = vec3(-view[2].x, 0.0f, view[2].z);
    sides_ = vec3(view[0].x, 0.0f, -view[0].z);
    

    //Traslacion
    view = translate(view, mov);

    return view;
}

void Mouse(GLFWwindow* window , double initialTime)
{
    double posx, posy;
    glfwGetCursorPos(window, &posx, &posy);
    double finalTime = glfwGetTime();
    double deltaTime = finalTime - initialTime;
    yaw_ -= mouseSpeed * deltaTime * float(resX / 2 - posx);
    //yaw2 += mouseSpeed * deltaTime * float(resX / 2 - posx);
    pitch_ -= mouseSpeed * deltaTime * float(resY / 2 - posy);
    if (pitch_ >= 45) pitch_ = 45;
    if (pitch_ <= -25) pitch_ = -25;
    
}