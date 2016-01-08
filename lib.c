#include "lib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <png.h>

int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;

int getScreenWidth()
{
    return SCREEN_WIDTH;
}

int getScreenHeight()
{
    return SCREEN_HEIGHT;
}

void errorCallback(int error, const char* description)
{
    fputs(description, stderr);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    SCREEN_WIDTH = width;
    SCREEN_HEIGHT = height;
}

GLuint loadShader(GLenum shaderType, const char* filename)
{
    char* buffer = NULL;
    int length;
    FILE* file = fopen(filename, "rb");

    if (!file)
    {
        fprintf(stderr, "Could not open file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    length = ftell(file);
    fseek(file, 0, SEEK_SET);
    buffer = (char*)malloc(length);
    if (!buffer)
    {
        fprintf(stderr, "Problem with buffer allocation.\n");
        exit(EXIT_FAILURE);
    }
    fread(buffer, 1, length, file);
    fclose(file);

    GLuint shader = glCreateShader(shaderType);
    const char* shaderText = buffer;
    glShaderSource(shader, 1, &shaderText, &length);
    glCompileShader(shader);
    free(buffer);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        GLchar info[1024];
        glGetShaderInfoLog(shader, sizeof(info), NULL, info);
        fprintf(stderr, "Error compiling shader type %d: '%s'\n", shaderType, info);
    }

    return shader;
}

GLuint initShaders(const char* vertexShaderFile, const char* fragmentShaderFile)
{
    GLuint vertShdr = loadShader(GL_VERTEX_SHADER, vertexShaderFile);
    GLuint fragShdr = loadShader(GL_FRAGMENT_SHADER, fragmentShaderFile);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertShdr);
    glAttachShader(program, fragShdr);
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        GLchar info[1024];
        glGetProgramInfoLog(program, sizeof(info), NULL, info);
        fprintf(stderr, "Error linking shader program: %s", info);
    }

    glDeleteShader(vertShdr);
    glDeleteShader(fragShdr);
    return program;
}

GLuint loadBMP(const char* filename)
{
    // Data read from the header of the BMP file
    unsigned char header[54]; // Each BMP file begins by a 54-bytes header
    unsigned int dataPos;     // Position in the file where the actual data begins
    unsigned int width, height;
    unsigned int imageSize;   // = width*height*3
    // Actual RGB data
    unsigned char * data;

    // Open the file
    FILE* file = fopen(filename,"rb");
    if (!file)
    {
        printf("Image %s could not be opened\n", filename);
        return 0;
    }

    if (fread(header, 1, 54, file) != 54)
    {
        // If not 54 bytes read : problem
        printf("%s is not a correct BMP file\n", filename);
        return 0;
    }

    if (header[0]!='B' || header[1]!='M')
    {
        printf("%s is not a correct BMP file\n", filename);
        return 0;
    }

    // Read ints from the byte array
    dataPos    = *(int*)&(header[0x0A]);
    imageSize  = *(int*)&(header[0x22]);
    width      = *(int*)&(header[0x12]);
    height     = *(int*)&(header[0x16]);

    // Some BMP files are misformatted, guess missing information
    if (imageSize==0) imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
    if (dataPos==0)   dataPos=54; // The BMP header is done that way

    data = (unsigned char*)malloc(sizeof(unsigned char)*imageSize);

    // Read the actual data from the file into the buffer
    fread(data, 1, imageSize, file);

    //Everything is in memory now, the file can be closed
    fclose(file);

    // Create one OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    return textureID;
}

GLuint loadPNG(const char * file_name, int * width, int * height)
{
    // This function was originally written by David Grayson for
    // https://github.com/DavidEGrayson/ahrs-visualizer
    png_byte header[8];
    FILE *fp = fopen(file_name, "rb");
    if (fp == 0)
    {
        perror(file_name);
        return 0;
    }
    // read the header
    fread(header, 1, 8, fp);
    if (png_sig_cmp(header, 0, 8))
    {
        fprintf(stderr, "error: %s is not a PNG.\n", file_name);
        fclose(fp);
        return 0;
    }
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
    {
        fprintf(stderr, "error: png_create_read_struct returned 0.\n");
        fclose(fp);
        return 0;
    }
    // create png info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        fprintf(stderr, "error: png_create_info_struct returned 0.\n");
        png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
        fclose(fp);
        return 0;
    }
    // create png info struct
    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info)
    {
        fprintf(stderr, "error: png_create_info_struct returned 0.\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
        fclose(fp);
        return 0;
    }
    // the code in this if statement gets called if libpng encounters an error
    if (setjmp(png_jmpbuf(png_ptr))) {
        fprintf(stderr, "error from libpng\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        return 0;
    }
    // init png reading
    png_init_io(png_ptr, fp);
    // let libpng know you already read the first 8 bytes
    png_set_sig_bytes(png_ptr, 8);
    // read all the info up to the image data
    png_read_info(png_ptr, info_ptr);
    // variables to pass to get info
    int bit_depth, color_type;
    png_uint_32 temp_width, temp_height;
    // get info about png
    png_get_IHDR(png_ptr, info_ptr, &temp_width, &temp_height, &bit_depth, &color_type,
    NULL, NULL, NULL);
    if (width){ *width = temp_width; }
    if (height){ *height = temp_height; }
    //printf("%s: %lux%lu %d\n", file_name, temp_width, temp_height, color_type);
    if (bit_depth != 8)
    {
        fprintf(stderr, "%s: Unsupported bit depth %d. Must be 8.\n", file_name, bit_depth);
        return 0;
    }
    GLint format;
    switch(color_type)
    {
        case PNG_COLOR_TYPE_RGB:
        format = GL_RGB;
        break;
        case PNG_COLOR_TYPE_RGB_ALPHA:
        format = GL_RGBA;
        break;
        default:
        fprintf(stderr, "%s: Unknown libpng color type %d.\n", file_name, color_type);
        return 0;
    }
    // Update the png info struct.
    png_read_update_info(png_ptr, info_ptr);
    // Row size in bytes.
    int rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    // glTexImage2d requires rows to be 4-byte aligned
    rowbytes += 3 - ((rowbytes-1) % 4);
    // Allocate the image_data as a big block, to be given to opengl
    png_byte * image_data = (png_byte *)malloc(rowbytes * temp_height * sizeof(png_byte)+15);
    if (image_data == NULL)
    {
        fprintf(stderr, "error: could not allocate memory for PNG image data\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        return 0;
    }
    // row_pointers is for pointing to image_data for reading the png with libpng
    png_byte ** row_pointers = (png_byte **)malloc(temp_height * sizeof(png_byte *));
    if (row_pointers == NULL)
    {
        fprintf(stderr, "error: could not allocate memory for PNG row pointers\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        free(image_data);
        fclose(fp);
        return 0;
    }
    // set the individual row_pointers to point at the correct offsets of image_data
    for (unsigned int i = 0; i < temp_height; i++)
    {
        row_pointers[temp_height - 1 - i] = image_data + i * rowbytes;
    }
    // read the png into image_data through row_pointers
    png_read_image(png_ptr, row_pointers);
    // Generate the OpenGL texture object
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, format, temp_width, temp_height, 0, format, GL_UNSIGNED_BYTE, image_data);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    free(image_data);
    free(row_pointers);
    fclose(fp);
    return texture;
}

GLFWwindow* initLibraries(const char* title)
{
    if (!glfwInit()) exit(EXIT_FAILURE);

    glfwSetErrorCallback(errorCallback);

    GLFWwindow* window = glfwCreateWindow(640, 480, title, NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    GLenum res = glewInit();
    if (res != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW: '%s'\n", glewGetErrorString(res));
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    return window;
}

void checkGLErrors(const char* loc)
{
    GLenum error;
    while((error = glGetError()))
        fprintf(stderr, "Error at %s: %s\n", loc, gluErrorString(error));
}

