//
// Created by Christopher Gruber on 6/23/24.
//

#include "Camera.h"
#include <glm/ext/matrix_transform.hpp>

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, glm::vec3(0.0f, 0.0f, 0.0f), up);
}

void Camera::moveForward(float delta) {
    glm::mat4 viewMatrix = getViewMatrix();
    glm::vec3 forward = glm::normalize(glm::vec3(viewMatrix[2][0], viewMatrix[2][1], viewMatrix[2][2]));
    position -= forward * delta;
}

void Camera::moveBackward(float delta) {
    glm::mat4 viewMatrix = getViewMatrix();
    glm::vec3 forward = glm::normalize(glm::vec3(viewMatrix[2][0], viewMatrix[2][1], viewMatrix[2][2]));
    position += forward * delta;
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
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    zoom = 45.0f;
    radius = 3.0f;
}

void Camera::rotateAroundYAxis(float angle) {
    glm::vec3 center(0.0f, 0.0f, 0.0f);
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::vec3 offset = position - center;
    offset = glm::vec3(rotation * glm::vec4(offset, 1.0f));
    position = center + offset;
    up = glm::vec3(rotation * glm::vec4(up, 0.0f));
}

void Camera::rotateAroundXAxis(float angle) {
    glm::vec3 center(0.0f, 0.0f, 0.0f);
    glm::vec3 viewDir = glm::normalize(center - position);
    glm::vec3 right = glm::normalize(glm::cross(viewDir, up));
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, right);
    glm::vec3 offset = position - center;
    offset = glm::vec3(rotation * glm::vec4(offset, 1.0f));
    position = center + offset;
    up = glm::vec3(rotation * glm::vec4(up, 0.0f));
}

float Camera::getZoom() const {
    return zoom;
}

const glm::vec3 &Camera::getPosition() const {
    return position;
}
