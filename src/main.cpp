// Include standard headers
#include <cstdio>
#include <iostream>

// Include GLAD
#include <glad/glad.h>

// Include GLFW
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Camera.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

void processUserInput(GLFWwindow *window, Camera *camera) {
    float cameraSpeed = 0.05f;

    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        camera->rotateAroundYAxis(cameraSpeed);
    else if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        camera->rotateAroundYAxis(-cameraSpeed);
    else if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        camera->rotateAroundXAxis(cameraSpeed);
    else if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera->rotateAroundXAxis(-cameraSpeed);
    else if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera->zoomIn(1.0f);
    else if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera->zoomOut(1.0f);
    else if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->moveForward(cameraSpeed);
    else if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->moveBackward(cameraSpeed);
}

struct WindowData {
    Camera* camera{};
    bool wireframeMode = false;
    bool rotationPaused = false;
    float pausedTime = 0.0f;
};

void key_callback(GLFWwindow* window, int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods) {
    auto *windowData = static_cast<WindowData *>(glfwGetWindowUserPointer(window));
    auto camera = windowData->camera;

    if (key == GLFW_KEY_HOME && action == GLFW_PRESS) {
        camera->reset();
    } else if (key == GLFW_KEY_GRAVE_ACCENT && action == GLFW_PRESS) {
        windowData->wireframeMode = !windowData->wireframeMode;
        if(windowData->wireframeMode)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    } else if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        windowData->rotationPaused = !windowData->rotationPaused;
        if (windowData->rotationPaused) {
            windowData->pausedTime = (float)glfwGetTime();
        }
    } else if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

[[maybe_unused]] static GLuint createSingleDotObject()
{
    // Create a single dot object
    float vertices[] = {
            0.0f, 0.0f, 0.0f
    };

    unsigned int VAO;
    unsigned int VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Set the vertex attributes pointers
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    // Unbind the VAO
    glBindVertexArray(0);

    return VAO;
}

static GLuint loadCubeObject() {
    // Create a cube object
    float vertices[] = {
            // positions                        // colors
            -0.5f, -0.5f, 0.5f,  0.0f, 1.0f, 0.0f,  // front face is green
            0.5f, -0.5f, 0.5f,   0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f,    0.0f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.5f,   0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,  // back face is red
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
            0.5f, 0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, -0.5f,  1.0f, 0.0f, 0.0f
    };

    unsigned int indices[] = {
            // front
            0, 1, 2,
            2, 3, 0,
            // right
            1, 5, 6,
            6, 2, 1,
            // back
            7, 6, 5,
            5, 4, 7,
            // left
            4, 0, 3,
            3, 7, 4,
            // bottom
            4, 5, 1,
            1, 0, 4,
            // top
            3, 2, 6,
            6, 7, 3
    };

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind the VAO
    glBindVertexArray(0);

    return VAO;
}

int main()
{
    // Initialise GLFW
    if(!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on macOS
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    const int WINDOW_WIDTH = 1024;
    const int WINDOW_HEIGHT = 768;

    // Open a window and create its OpenGL context
    GLFWwindow* window = glfwCreateWindow( WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Cube Render", nullptr, nullptr);

    if( window == nullptr ){
        std::cerr << "Failed to open GLFW window. Check your drivers opengl support." << std::endl;
        getchar();
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        getchar();
        glfwTerminate();
        return -1;
    }

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Set background color
    glClearColor(0.0f, 0.0f, 0.6f, 0.0f);

    // Load a 3D cube object
    GLuint VAO = loadCubeObject();

    // Load shaders
    Shader shader("shaders/vertex/vertex.glsl", "shaders/fragment/fragment.glsl");

    // Create a camera instance
    Camera camera;

    bool wireframeMode = false;

    auto *windowData = new WindowData{&camera, wireframeMode, false, 0.0f};
    glfwSetWindowUserPointer(window, windowData);

    // Check if the ESC key was pressed or the window was closed
    while(!glfwWindowShouldClose(window))
    {
        processUserInput(window, &camera);

        // Render here
        glClear(GL_COLOR_BUFFER_BIT);

        // Use the shader. It calls glUseProgram()
        shader.use();

        // Create the model identity matrix
        auto model = glm::mat4(1.0f);

        // Apply transformations to the model matrix to rotate it around the y-axis at a rate of 50 degrees per second
        float rotationTime = windowData->rotationPaused ? windowData->pausedTime : (float)glfwGetTime();
        model = glm::rotate(model, glm::radians(rotationTime * 50.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        // Set the view and projection matrices in the shader
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        // update the uniform color
        double timeValue = glfwGetTime();
        // Dividing by 2.0f scales the result of the sine function to range between -0.5 and 0.5.
        // Adding 0.5f shifts the range of values to be between 0 and 1.
        float redValue = ((float)sin(timeValue) / 2.0f) + 0.5f;
        float greenValue = ((float)sin(timeValue + 10) / 2.0f) + 0.5f;
        float blueValue = 0;
        int vertexColorLocation = glGetUniformLocation(shader.getProgramID(), "cubeColor");
        glUniform4f(vertexColorLocation, redValue, greenValue, blueValue, 1.0f);

        glBindVertexArray(VAO); // Bind the VAO
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0); // Unbind the VAO

        // check and call events and swap the buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    // Close OpenGL window and terminate GLFW
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
