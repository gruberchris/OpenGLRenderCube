//
// Created by Christopher Gruber on 6/23/24.
//

#include "Camera.h"
#include <glm/ext/matrix_transform.hpp>

Camera::Camera() : position(0.0f, 0.0f, 3.0f), zoom(45.0f) {}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::moveLeft(float delta) {
    position.x -= delta;
}

void Camera::moveRight(float delta) {
    position.x += delta;
}

void Camera::moveUp(float delta) {
    position.y += delta;
}

void Camera::moveDown(float delta) {
    position.y -= delta;
}

void Camera::zoomIn(float delta) {
    if(zoom >= 1.0f && zoom <= 45.0f)
        zoom -= delta;
    if(zoom <= 1.0f)
        zoom = 1.0f;
}

void Camera::zoomOut(float delta) {
    if(zoom <= 45.0f)
        zoom += delta;
    if(zoom >= 45.0f)
        zoom = 45.0f;
}

void Camera::reset() {
    position = glm::vec3(0.0f, 0.0f, 3.0f);
    zoom = 45.0f;
}