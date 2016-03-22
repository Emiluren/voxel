#include "lib.h"
#include "matrix.hpp"
#include "vector.hpp"
#include "camera.hpp"
#include "vertex.hpp"
#include "texturedShader.hpp"

#include "chunk.hpp"
#include "chunkmanager.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <vector>

GLFWwindow* window = NULL;

TexturedShader* textured;

DirectionalLight dirLight;
Camera* camera = NULL;

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        Vector3f hit;
        Side side;
        if (ChunkManager::raycast(camera->GetPosition(), camera->GetTarget(), hit, side))
        {
            int x = hit.x, y = hit.y, z = hit.z;
            if (button == 1)
            {
                switch (side)
                {
                    case Top:
                        ChunkManager::addBlock(x, y+1, z);
                        break;
                    case Bottom:
                        ChunkManager::addBlock(x, y-1, z);
                        break;
                    case Left:
                        ChunkManager::addBlock(x-1, y, z);
                        break;
                    case Right:
                        ChunkManager::addBlock(x+1, y, z);
                        break;
                    case Front:
                        ChunkManager::addBlock(x, y, z-1);
                        break;
                    case Back:
                        ChunkManager::addBlock(x, y, z+1);
                        break;
                }
            }
            else
                ChunkManager::removeBlock(x, y, z);
        }
    }
}

void init()
{
    window = initLibraries("Voxel");
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    glClearColor(0.3f, 0.3f, 1.0f, 0.0f);
    textured = new TexturedShader(initShaders("textured.vs", "textured.fs"));

    // printf("test\n");
    // GLuint vao;
    // glGenVertexArrays(1, &vao);
    // glBindVertexArray(vao);
    // printf("slut\n");

    ChunkManager::bufferGraphicsData();

    dirLight.ambientIntensity = 0.2f;
    dirLight.color.r = 1.0f;
    dirLight.color.g = 1.0f;
    dirLight.color.b = 1.0f;

    dirLight.direction = Vector3f(-0.4f, -1.0f, 0.6f);
    dirLight.direction.Normalize();
    dirLight.diffuseIntensity = 0.8f;

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    camera = new Camera(
        Vector3f(1.0f, 62.0f, 1.0f),
        Vector3f(0.0f, 0.0f, 1.0f),
        Vector3f(0.0f, 1.0f, 0.0f)
    );

    glfwSetMouseButtonCallback(window, mouseButtonCallback);
}

bool fpressed = false;
bool flying = true;
void update(float deltaTime)
{
    Vector3f v = camera->GetPosition();
    int x = v.x;
    int y = v.y;
    int z = v.z;
    /*if (x >= 0 && z >= 0 && y > 0 && x < ChunkManager::WORLD_SIZE_X * 16 && z < ChunkManager::WORLD_SIZE_Z * 16)
    {
        //int chunkIndex = (x / 16) * ChunkManager::WORLD_SIZE_X + (z / 16);
        //camera->Update(window, deltaTime, flying, chunks[chunkIndex]);

        camera->Update(window, deltaTime, flying, ChunkManager::getChunk(x / 16, z / 16));
    }
    else
        camera->Update(window, deltaTime, flying, NULL);*/

    camera->Update(window, deltaTime, flying);

    // TODO: Change to the callback function
    if (!fpressed && glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        fpressed = true;
        flying = !flying;
    }
    if (glfwGetKey(window, GLFW_KEY_F) != GLFW_PRESS)
        fpressed = false;
}

// void setDirectionalLight()
// {
// }

void render()
{
    // Render floor
    float ar = float(getScreenWidth()) / float(getScreenHeight());
    Vector3f camPos = camera->GetPosition();

    textured->Use(dirLight);

    for (int x = 0; x < ChunkManager::WORLD_SIZE_X; x++)
    {
        for (int z = 0; z < ChunkManager::WORLD_SIZE_Z; z++)
        {
            Matrix4f world = translate(x * 16, 0, z * 16) * scale(1.0f, 1.0f, 1.0f);
            Matrix4f wvp = persProj(ar, 0.1f, 100.0f, 45.0f) * camera->GetTransformation() * world;
            textured->SetWVP(wvp);
            textured->SetWorld(world);
            ChunkManager::getChunk(x, z)->Render(textured);
        }
    }

    checkGLErrors("render chunk");

    textured->StopUsing();

    glUseProgram(0);
}

int main()
{
    //printf("test\n");
    init();

    double time = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
        double newTime = glfwGetTime();
        double deltaTime = newTime - time;
        time = newTime;

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        update(deltaTime);
        render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete camera;
    glfwDestroyWindow(window);
    glfwTerminate();
}
