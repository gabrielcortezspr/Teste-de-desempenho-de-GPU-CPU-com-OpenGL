cmake_minimum_required(VERSION 3.10)
project(OpenGL_Performance_Test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

# Include directories
include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${GLM_INCLUDE_DIRS})

# Source files for basic triangle test
set(BASIC_SOURCES
    src/main.cpp
    src/Renderer.cpp
    src/PerformanceMonitor.cpp
)

# Source files for performance test
set(PERF_SOURCES
    src/performance_test.cpp
    src/MultiTriangleRenderer.cpp
    src/PerformanceMonitor.cpp
)

# Source files for advanced test (with lighting and textures)
set(ADVANCED_SOURCES
    src/advanced_test.cpp
    src/AdvancedRenderer.cpp
    src/Lighting.cpp
    src/Texture.cpp
    src/PerformanceMonitor.cpp
)

# Source files for interactive demo
set(DEMO_SOURCES
    src/demo_interativo.cpp
    src/AdvancedRenderer.cpp
    src/Lighting.cpp
    src/Texture.cpp
    src/PerformanceMonitor.cpp
)

# Source files for extreme test (10k steps)
set(EXTREME_SOURCES
    src/extreme_test.cpp
    src/AdvancedRenderer.cpp
    src/Lighting.cpp
    src/Texture.cpp
    src/PerformanceMonitor.cpp
)

# Create executables
add_executable(${PROJECT_NAME} ${BASIC_SOURCES})
add_executable(PerformanceTest ${PERF_SOURCES})
add_executable(AdvancedTest ${ADVANCED_SOURCES})
add_executable(DemoInterativo ${DEMO_SOURCES})
add_executable(ExtremeTest ${EXTREME_SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME} 
    ${OPENGL_LIBRARIES}
    glfw
    GLEW::GLEW
    pthread
)

target_link_libraries(PerformanceTest 
    ${OPENGL_LIBRARIES}
    glfw
    GLEW::GLEW
    pthread
)

target_link_libraries(AdvancedTest 
    ${OPENGL_LIBRARIES}
    glfw
    GLEW::GLEW
    pthread
)

target_link_libraries(DemoInterativo 
    ${OPENGL_LIBRARIES}
    glfw
    GLEW::GLEW
    pthread
)

target_link_libraries(ExtremeTest 
    ${OPENGL_LIBRARIES}
    glfw
    GLEW::GLEW
    pthread
)

# Copy shaders to build directory
file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR})
file(COPY textures DESTINATION ${CMAKE_BINARY_DIR})
