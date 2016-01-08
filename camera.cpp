#include "camera.hpp"
#include "matrix.hpp"
#include "macros.h"
#include "chunk.hpp"
#include "chunkmanager.hpp"

#include <cmath>
#include <iostream>

Camera::Camera(const Vector3f& pos, const Vector3f& target, const Vector3f& up) :
    position(pos), target(target), up(up), speedY(0), onGround(false)
{
    this->target.Normalize();
    this->up.Normalize();

    init();
}

void Camera::init()
{
    Vector3f HTarget(target.x, 0.0, target.z);
    HTarget.Normalize();

    if (HTarget.z >= 0.0f)
    {
        if (HTarget.x >= 0.0f)
            angleH = 360.0f - ToDegree(asin(HTarget.z));
        else
            angleH = 180.0f + ToDegree(asin(HTarget.z));
    } else
    {
        if (HTarget.x >= 0.0f)
            angleH = ToDegree(asin(-HTarget.z));
        else
            angleH = 90.0f + ToDegree(asin(-HTarget.z));
    }

    angleV = -ToDegree(asin(target.y));
}

void Camera::Update(GLFWwindow* window, float deltaTime, bool flying)
{
    const float gravity = -32;

    keyboard(window, deltaTime, flying);
    mouse(window, deltaTime, flying);

    Chunk* currentChunk = NULL;
    if (position.x >= 0 && position.z >= 0 && position.y > 0 &&
            position.x < ChunkManager::WORLD_SIZE_X * 16 &&
            position.z < ChunkManager::WORLD_SIZE_Z * 16)
        currentChunk = ChunkManager::getChunk((int)position.x / 16, (int)position.z / 16);

    if (flying)
        speedY = 0;
    else
    {
        float feetY = int(position.y - 1.62f);
        float headY = int(position.y + 0.18f);
        int chunkX = int(position.x) % 16;
        int chunkZ = int(position.z) % 16;
        if (currentChunk != NULL && currentChunk->IsBlockSolid(chunkX, feetY, chunkZ))
        {
            feetY++; // Current feetY is one block into the ground
            position.y = feetY + 1.62f;
            speedY = 0;
            onGround = true;
        }
        else if (currentChunk != NULL && currentChunk->IsBlockSolid(chunkX, headY, chunkZ) && speedY > 0)
        {
            headY = int(headY);
            position.y = headY - 0.18f;
            speedY = 0;
        }
        else
        {
            speedY += gravity * deltaTime;
            if (speedY < -80)
                speedY = -80;
            position.y += speedY * deltaTime;
            onGround = false;
        }
    }

    if (angleV > 89.0f) angleV = 89.0f;
    if (angleV < -89.0f) angleV = -89.0f;

    const Vector3f axisV(0.0f, 1.0f, 0.0f);
    Vector3f view(1.0f, 0.0f, 0.0f);
    view.Rotate(angleH, axisV);

    Vector3f axisH = axisV.Cross(view);
    axisH.Normalize();
    view.Rotate(angleV, axisH);
    view.Normalize();

    target = view;
    up = target.Cross(axisH);
    up.Normalize();
}

bool Camera::checkSideCollision(Chunk* chunk, int chunkX, float feetY, int chunkZ)
{
    int feet = int(feetY + 0.1f); // Offset to avoid sticking to the ground
    int head = int(feetY + PLAYER_HEIGHT - 0.1f);
    int body = feet + 1;
    if (chunk->IsBlockSolid(chunkX, feet, chunkZ) ||
        chunk->IsBlockSolid(chunkX, body, chunkZ) ||
        chunk->IsBlockSolid(chunkX, head, chunkZ))

        return true;
    return false;
}

float Camera::calculateXoverlap(const Vector3f& newposition)
{
    if (newposition.x > position.x)
    {
        float newRight = newposition.x + PLAYER_WIDTH / 2;
        int chunkX = int(newRight) % 16;
        int chunkZ = int(position.z) % 16;

        Chunk* chunk = ChunkManager::getChunkFromBlock(newRight, position.y, position.z);
        float feetY = position.y - 1.62f;
        if (checkSideCollision(chunk, chunkX, feetY, chunkZ))
            return -newRight + (int)newRight;
    } else if (newposition.x < position.x)
    {
        float newLeft = newposition.x - PLAYER_WIDTH / 2;
        int chunkX = int(newLeft) % 16;
        int chunkZ = int(position.z) % 16;

        Chunk* chunk = ChunkManager::getChunkFromBlock(newLeft, position.y, position.z);
        float feetY = position.y - 1.62f;
        if (checkSideCollision(chunk, chunkX, feetY, chunkZ)) {
            return (int)newLeft + 1 - newLeft;
        }
    }

    return 0.0f;
}

float Camera::calculateZoverlap(const Vector3f& newposition)
{
    if (newposition.z > position.z)
    {
        float newBack = newposition.z + PLAYER_WIDTH / 2;
        int chunkX = int(position.x) % 16;
        int chunkZ = int(newBack) % 16;

        Chunk* chunk = ChunkManager::getChunkFromBlock(position.x, position.y, newBack);
        float feetY = position.y - 1.62f;
        if (checkSideCollision(chunk, chunkX, feetY, chunkZ))
            return -newBack + (int)newBack;
    } else if (newposition.z < position.z)
    {
        float newFront = newposition.z - PLAYER_WIDTH / 2;
        int chunkX = int(position.x) % 16;
        int chunkZ = int(newFront) % 16;

        Chunk* chunk = ChunkManager::getChunkFromBlock(position.x, position.y, newFront);
        float feetY = position.y - 1.62f;
        if (checkSideCollision(chunk, chunkX, feetY, chunkZ)) {
            return (int)newFront + 1 - newFront;
        }
    }

    return 0.0f;
}

void Camera::keyboard(GLFWwindow* window, float deltaTime, bool flying)
{
    float speed = 5.0f;
    if (flying)
        speed = 20.0f;

    Vector3f newposition = position;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        if (flying)
            position += target * speed * deltaTime;
        else
        {
            Vector3f forward = target;
            forward.y = 0;
            forward.Normalize();
            newposition += forward * speed * deltaTime;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        if (flying)
            position += target * -speed * deltaTime;
        else
        {
            Vector3f forward = target;
            forward.y = 0;
            forward.Normalize();
            newposition += forward * -speed * deltaTime;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        Vector3f left = target.Cross(up);
        left.Normalize();
        newposition += left * speed * deltaTime;
        if (flying)
            position = newposition;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        Vector3f right = up.Cross(target);
        right.Normalize();
        newposition += right * speed * deltaTime;
        if (flying)
            position = newposition;
    }

    float feetY = int(position.y - 1.62f);
    float headY = int(position.y + 0.18f);

    if (!flying)
    {
        float overlapX = calculateXoverlap(newposition);
        float overlapZ = calculateZoverlap(newposition);

        newposition.x += overlapX;
        newposition.z += overlapZ;

        position = newposition;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !flying && onGround)
    {
        speedY = 10;
    }
}

void Camera::mouse(GLFWwindow* window, float deltaTime, bool flying)
{
    double xpos, ypos;
    int width = getScreenWidth();
    int height = getScreenHeight();
    glfwGetCursorPos(window, &xpos, &ypos);
    if (xpos == 0)
        xpos = width / 2;
    if (ypos == 0)
        ypos = height / 2;
    glfwSetCursorPos(window, width / 2, height / 2);

    float speed = 20.0f;
    if (flying)
        speed = 40.0f;
    angleH += speed * deltaTime * float(xpos - width / 2);
    angleV += speed * deltaTime * float(ypos - height / 2);
}

Matrix4f Camera::GetTransformation() const
{
    return cameraRotation(target, up) *
        translate(-position.x, -position.y, -position.z);
}

Matrix4f Camera::cameraRotation(const Vector3f& target, const Vector3f& up) const
{
    Matrix4f ret;

    Vector3f N = target;
    Vector3f U = up;
    N.Normalize();
    U.Normalize();
    U = U.Cross(N);
    Vector3f V = N.Cross(U);

    ret[0][0] = U.x;  ret[0][1] = U.y;  ret[0][2] = U.z;  ret[0][3] = 0.0f;
    ret[1][0] = V.x;  ret[1][1] = V.y;  ret[1][2] = V.z;  ret[1][3] = 0.0f;
    ret[2][0] = N.x;  ret[2][1] = N.y;  ret[2][2] = N.z;  ret[2][3] = 0.0f;
    ret[3][0] = 0.0f; ret[3][1] = 0.0f; ret[3][2] = 0.0f; ret[3][3] = 1.0f;

    return ret;
}
