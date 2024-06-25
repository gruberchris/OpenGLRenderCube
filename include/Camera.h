//
// Created by Christopher Gruber on 6/23/24.
//

#ifndef OPENGLRENDERCUBE_CAMERA_H
#define OPENGLRENDERCUBE_CAMERA_H


#include <glm/vec3.hpp>
#include <glm/fwd.hpp>

class Camera {
public:
    Camera() = default;

    [[nodiscard]] glm::mat4 getViewMatrix() const;
    void moveLeft(float delta);
    void moveRight(float delta);
    void moveUp(float delta);
    void moveDown(float delta);
    void zoomIn(float delta);
    void zoomOut(float delta);
    void reset();
    void rotateAroundYAxis(float angle);
    void rotateAroundXAxis(float angle);

private:
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
    float zoom = 45.0f;
public:
    [[nodiscard]] float getZoom() const;

    [[nodiscard]] const glm::vec3 &getPosition() const;
};

#endif //OPENGLRENDERCUBE_CAMERA_H
