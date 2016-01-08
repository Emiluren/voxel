#ifndef CAMERA_HPP
#define CAMERA_HPP

class Matrix4f;
class Chunk;

#include "vector.hpp"
#include "lib.h"

class Camera
{
public:
    Camera(const Vector3f& pos, const Vector3f& target, const Vector3f& up);

    void Update(GLFWwindow* window, float deltaTime, bool flying);
    Matrix4f GetTransformation() const;

    void SetPosition(const Vector3f& pos) { position = pos; }
    Vector3f GetPosition() { return position; }
    Vector3f GetTarget() { return target; }

private:
    float PLAYER_WIDTH = 0.6f;
    float PLAYER_HEIGHT = 1.8f;

    Vector3f position;
    Vector3f target;
    Vector3f up;

    float angleH;
    float angleV;

    // Should be moved to player class
    float speedY;
    bool onGround;

    void init();
    void keyboard(GLFWwindow* window, float deltaTime, bool flying);
    void mouse(GLFWwindow* window, float deltaTime, bool flying);

    Matrix4f cameraRotation(const Vector3f& target, const Vector3f& up) const;

    float calculateXoverlap(const Vector3f& newposition);
    float calculateZoverlap(const Vector3f& newposition);
    bool checkSideCollision(Chunk* chunk, int chunkX, float feetY, int chunkZ);
};

#endif // CAMERA_HPP
