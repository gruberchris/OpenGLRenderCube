cmake_minimum_required(VERSION 3.28)

# Set the project name and specify C++ as the language
project(OpenGLRenderCube LANGUAGES CXX C)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Get environment variables
set(GLFW_INCLUDE_DIRS $ENV{GLFW_INCLUDE_DIRS})
set(GLAD_INCLUDE_DIRS $ENV{GLAD_INCLUDE_DIRS})
set(GLFW_LIBRARIES $ENV{GLFW_LIBRARIES})

# Print environment variables for debugging
message(STATUS "GLFW_INCLUDE_DIRS = ${GLFW_INCLUDE_DIRS}")
message(STATUS "GLAD_INCLUDE_DIRS = ${GLAD_INCLUDE_DIRS}")
message(STATUS "GLFW_LIBRARIES = ${GLFW_LIBRARIES}")

# Collect all source files in src directory
file(GLOB SOURCE_FILES "src/*.cpp" "src/glad.c")

# Include directories from environment variables
include_directories(
        $ENV{GLAD_INCLUDE_DIRS}
        $ENV{GLFW_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}/include
)

# Add the executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Link GLFW library from environment variable
target_link_libraries(${PROJECT_NAME}
        $ENV{GLFW_LIBRARIES}/libglfw.3.dylib
        "-framework OpenGL"
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
)

# Set compiler flags for macOS ARM64
if(APPLE AND CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -arch arm64")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -arch arm64")
endif()

# Copy the shaders directory to the output directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders)

# Define shader files
file(GLOB SHADERS
        "${PROJECT_SOURCE_DIR}/shaders/vertex/*.glsl"
        "${PROJECT_SOURCE_DIR}/shaders/fragment/*.glsl"
        "${PROJECT_SOURCE_DIR}/shaders/geometry/*.glsl"
)

# Add an install target to copy shaders
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(FILES ${SHADERS} DESTINATION shaders)