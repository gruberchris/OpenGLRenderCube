cmake_minimum_required(VERSION 3.28)

# Set the project name and specify C++ as the language
project(OpenGLRenderCube LANGUAGES CXX C)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add GLFW from source
add_subdirectory(${PROJECT_SOURCE_DIR}/libs/glfw)

# Collect all source files in src directory
file(GLOB SOURCE_FILES
    "src/*.cpp"
    "libs/glad/src/glad.c"
)

# Add the executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/libs
        ${PROJECT_SOURCE_DIR}/libs/glfw/include
        ${PROJECT_SOURCE_DIR}/libs/glad/include
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

# Set compiler flags for macOS ARM64
if(APPLE AND CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -arch arm64")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -arch arm64")
    # On modern macOS, system frameworks are found automatically when linking with glfw
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
