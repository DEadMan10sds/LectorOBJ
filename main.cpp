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

//Cargador de imágenes
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"//Sólo se agrega en este archivo para que no tener duplicidad de código

#include "File.h"
#include "Vertex.h"
#include "shader.hpp"


//CONSTANTES
#define FPS 60 //FPS máximos

//Namespaces
using namespace std;
using namespace glm;

//GLOBALES
int resX = 1024, resY = 620;
int frameCount = 0;
double initialTime, finalTime, actual_frame_duration; //Tiempo inicial, tiempo final, contador de frames
double frame_duration = (1 / (float)FPS);
File archivo("cube.obj");
File arma("Pelican.obj");
File plano("PlaneTexture.obj");

float sensitivity = .5f;

//Variables locales de matrices
mat4 view, projection;

//Variables globales para rotaciones de cámara
GLfloat RotX = 0.0f, RotY = 0.0f;
double finY, finX;
double yaw_, yaw2, pitch_;
float mouseSpeed = 1.0f;
vec3 mov(0.0f, -2.0f, -5.0f);
vec3 pivot(0.0f, -1.5f, -5.0f);
vec3 pivot_fixed(0.0f, -1.5f, -5.0f);
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
unsigned int LoadTexture(/*char* texture_route*/);

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


    crntTime = glfwGetTime();//Obtiene el tiempo actual segun opengl

    /*string textureRoute = "woodFloor.jpg";
    GLuint texture = LoadTexture(textureRoute);*/

    GLuint texture = LoadTexture();

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

    cout << "Todos los modelos han creado su buffer" << endl;

    //Cargan los shaders
    GLuint programIDP = LoadShaders( "vs1Texture.glsl", "fs1Texture.glsl"); //phong
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
                
                if (camera_mode)
                {
                    Current_model.rotate_modelFP(mov, radians(pitch_), radians(yaw_));
                    Current_model.translate_model(mov, camera_mode);
                }
                else
                {
                    Current_model.rotate_modelTP(pivot, radians(yaw_));
                    Current_model.translate_model(pivot, camera_mode);
                }
                
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

            glBindTexture(GL_TEXTURE_2D, texture);

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
    glDeleteProgram(programID);//Si se usan más shaders o shaders distintos por cada prograna se tiene que incluir en el método freeBufferShaders()

    
}


void processInput(GLFWwindow* window)
{
    //Los controles se invierten, el sumar es abajo en lugar de arriba
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        mov += forward_ * speed;
        pivot += forward_ * speed;
        pivot_fixed += forward_ * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        mov -= forward_ * speed;
        pivot -= forward_ * speed;
        pivot_fixed -= forward_ * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        mov += sides_ * speed;
        pivot += sides_ * speed;
        pivot_fixed += sides_ * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        mov -= sides_ * speed;
        pivot -= sides_ * speed;
        pivot_fixed -= sides_ * speed;
    }

    if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) camera_mode = !camera_mode;
}

void createMatrices(GLFWwindow* window)
{
    //Vista
    view = FPView(window, pitch_, yaw_);

    //Proyección
    projection = perspective(radians(45.0f), (float)(resX / resY), 0.1f, 100.0f);//Ángulo de visión

}


//Ángulos en RADIANES
mat4 FPView(GLFWwindow* window, float rotX, float rotY)
{
    mat4 view;
    float rotXr = radians(rotX);
    float rotYr = radians(rotY);


    if (camera_mode)
    {
        //**************************************APARTADO DE LA CÁMARA DE 1ERA PERSONA**************************************
        //Rotacion en X
        view = rotate(mat4(1.0f), rotXr, vec3(1.0f, 0.0f, 0.0f));
        //Rotacion en Y
        view = rotate(view, rotYr, vec3(0.0f, 1.0f, 0.0f));
        forward_ = vec3(-view[2].x, 0.0f, view[2].z);
        sides_ = vec3(view[0].x, 0.0f, -view[0].z);
        //Traslacion
        view = translate(view, mov);
    }
    else
    {
        //**************************************APARTADO DE LA CÁMARA DE 3ERA PERSONA**************************************
        //Rotacion en Y
        view = translate(mat4(1.0f), pivot);
        view = rotate(view, rotYr, vec3(0.0f, -1.0f, 0.0f));
        view = translate(mat4(1.0f), -pivot);
        /*mat4 view_rotated = rotate(view, rotYr, vec3(0.0f, 1.0f, 0.0f));
        forward_ = vec3(-view_rotated[2].x, 0.0f, view_rotated[2].z);
        sides_ = vec3(view_rotated[0].x, 0.0f, -view_rotated[0].z);*/

        forward_ = vec3(-view[2].x, 0.0f, view[2].z);
        sides_ = vec3(view[0].x, 0.0f, -view[0].z);

        //Traslacion
        view = translate(view, pivot);
    }

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

//Función para cargar imágenes
unsigned int LoadTexture(/*char* texture_route*/)
{

    //Definición del búfer y configuraciones del mismo buffer de textura para opengl
    unsigned int texture;
    glGenTextures(1, &texture); //Genera el espacio y asigna el número de identificador a la variable identificador de textura
    glBindTexture(GL_TEXTURE_2D, texture);//Enlaza la textura al buffer de textura, es decir, de ahora en adelante todas las 
    //operaciones de textura se realizarán con este buffer

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //La textura en la que se está trabajando sobre el eje X se va a repetir
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//Ahora en el eje Y se va a repetir

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //Elije el filtro nearest para la textura cuando se reduce en tamaño
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//Igual que el anterior pero para cuando aumenta el tamaño

    //Apartado de carga de archivos usando la función stbi_load de la librería stbimage
    //width, height, nrChannels se pasan por referencia ya que son valores que regresa la función
    //ancho - alto - número de canales de color (rgb)

    int width, height, nrChannels;
    unsigned char* data = stbi_load("metal.jpg", &width, &height, &nrChannels, 0);
    if (data)//Si se logró abrir la imagen
    {
        cout << "Textura creada" << endl;
        //**************************************SE CARGA EN OPENGL**************************************
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); //Es textura 2d, nivel, formato en el que se va a guardar la imagen, ancho, alto, borde, formato de la imagen, ubicación de la imagen (dirección de memoria)
        glGenerateMipmap(GL_TEXTURE_2D);//Crea un conjunto de la misma imagen pero en tamaños diferentes
    }
    else cout << "No se pudo cargar la textura" << endl;

    stbi_image_free(data);//Libera la información para no sobrecargar la memoria y evitar duplicidad de datos

    return texture;
}