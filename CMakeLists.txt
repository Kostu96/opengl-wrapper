cmake_minimum_required(VERSION 3.24)

project(glw-proj LANGUAGES CXX)
set(GLW_TARGET_NAME glw)

add_subdirectory(cpp-utils)
add_subdirectory(third_party)

find_package(OpenGL REQUIRED)

add_library(${GLW_TARGET_NAME} STATIC
    include/glw/buffer.hpp
    include/glw/framebuffer.hpp
    include/glw/glw.hpp
    #include/glw/renderer.hpp
    include/glw/shader.hpp
    include/glw/texture.hpp
    include/glw/vertex_array.hpp
    source/buffer.cpp
    source/framebuffer.cpp
    source/glw.cpp
    #source/renderer.cpp
    source/shader.cpp
    source/texture.cpp
    source/vertex_array.cpp
)

target_compile_features(glw PRIVATE cxx_std_23)

target_include_directories(${GLW_TARGET_NAME} SYSTEM PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(${GLW_TARGET_NAME} PUBLIC
    cut
    glad
    OpenGL::GL
)

target_link_libraries(${GLW_TARGET_NAME} PRIVATE
    stb
)
