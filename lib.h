#ifndef LIB_H
#define LIB_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

int getScreenWidth();
int getScreenHeight();

GLuint initShaders(const char* vertexShaderFile, const char* fragmentShaderFile);
GLFWwindow* initLibraries(const char* title);
GLuint loadBMP(const char* filename);
GLuint loadPNG(const char * file_name, int * width, int * height);
void checkGLErrors(const char* loc);

#endif // LIB_H
