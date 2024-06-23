//
// Created by Christopher Gruber on 6/22/24.
//

#ifndef OPENGLRENDERCUBE_SHADER_H
#define OPENGLRENDERCUBE_SHADER_H

#include <string>
#include <glad/glad.h>

class Shader {
public:
    // Constructor that builds the shader from a vertex and fragment shader file
    Shader(const std::string& vertexPath, const std::string& fragmentPath);

    // Use the current shader
    void use() const;

    // Utility functions
    [[nodiscard]] GLuint getProgramID() const;

    [[nodiscard]] bool isCompiled() const;

private:
    // Program ID
    GLuint programID;

    GLuint vertexID;
    GLuint fragmentID;

    // Utility function for checking shader compilation/linking errors.
    static void checkCompileErrors(GLuint shader, const std::string& type);
};

#endif //OPENGLRENDERCUBE_SHADER_H
