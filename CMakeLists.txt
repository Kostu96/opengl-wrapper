cmake_minimum_required(VERSION 3.24)

project(glw-proj LANGUAGES CXX)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(cpp-utils)

find_package(OpenGL REQUIRED)

set(BUILD_SHARED_LIBS OFF)
set(GLM_ENABLE_CXX_20 ON)
add_subdirectory(third_party/glm)
set_target_properties(glm PROPERTIES FOLDER glw/third_party)

add_library(glw STATIC
    src/buffer.cpp
    src/framebuffer.cpp
    src/glw.cpp
    src/shader.cpp
    src/texture.cpp
    src/vertex_array.cpp
    src/include/glw/buffer.hpp
    src/include/glw/framebuffer.hpp
    src/include/glw/glw.hpp
    src/include/glw/shader.hpp
    src/include/glw/texture.hpp
    src/include/glw/vertex_array.hpp
)

target_compile_features(glw PRIVATE cxx_std_23)

target_include_directories(glw PUBLIC
    src/include
    third_party/glm
    third_party/opengl_headers/include
)

target_link_libraries(glw PUBLIC
    cut::cut
    glm::glm
    OpenGL::GL
)

set_target_properties(glw PROPERTIES FOLDER glw)

add_library(glw::glw ALIAS glw)
