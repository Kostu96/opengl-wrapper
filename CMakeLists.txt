cmake_minimum_required(VERSION 3.24)

project(glw-proj LANGUAGES CXX)

add_subdirectory(cpp-utils)
add_subdirectory(third_party)

find_package(OpenGL REQUIRED)

add_library(glw STATIC
    src/buffer.cpp
    src/framebuffer.cpp
    src/glw.cpp
    #src/renderer.cpp
    src/shader.cpp
    src/texture.cpp
    src/vertex_array.cpp
    src/include/glw/buffer.hpp
    src/include/glw/framebuffer.hpp
    src/include/glw/glw.hpp
    #src/include/glw/renderer.hpp
    src/include/glw/shader.hpp
    src/include/glw/texture.hpp
    src/include/glw/vertex_array.hpp
)

target_compile_features(glw PRIVATE cxx_std_23)

target_include_directories(glw PUBLIC src/include)

target_link_libraries(glw PUBLIC
    cut
    glad
    OpenGL::GL
)

target_link_libraries(glw PRIVATE
    stb
)
