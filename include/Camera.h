//
// Created by Christopher Gruber on 6/23/24.
//

#ifndef OPENGLRENDERCUBE_CAMERA_H
#define OPENGLRENDERCUBE_CAMERA_H


#include <glm/vec3.hpp>
#include <glm/fwd.hpp>

class Camera {
public:
    glm::vec3 position;
    float zoom;

    Camera();
    [[nodiscard]] glm::mat4 getViewMatrix() const;
    void moveLeft(float delta);
    void moveRight(float delta);
    void moveUp(float delta);
    void moveDown(float delta);
    void zoomIn(float delta);
    void zoomOut(float delta);
    void reset();
};

#endif //OPENGLRENDERCUBE_CAMERA_H
