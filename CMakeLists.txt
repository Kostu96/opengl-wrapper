cmake_minimum_required(VERSION 3.20)

project(glw-proj LANGUAGES CXX)
set(GLW_TARGET_NAME glw)

add_subdirectory(third_party)

add_library(${GLW_TARGET_NAME} STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/glw/buffers.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/glw/error_callback.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/include/glw/framebuffer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/glw/glw.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/glw/shader.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/glw/texture.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/glw/vertex_array.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/buffers.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/framebuffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/glw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/shader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/texture.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/vertex_array.cpp
)

target_include_directories(${GLW_TARGET_NAME} SYSTEM PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(${GLW_TARGET_NAME} PUBLIC
    glad
)

target_link_libraries(${GLW_TARGET_NAME} PRIVATE
    stb
)
