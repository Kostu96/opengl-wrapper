#include "glw/glw.hpp"

#include <cut/exception.hpp>

#include <iostream>
#include <print>

namespace {

using namespace glw;

template<typename T>
T load_gl_proc(const char* name, GLWLoadFunc func) {
    auto fn = reinterpret_cast<T>(func(name));
    cut::ensure(fn != nullptr, "Failed to load {}", name);
    return fn;
}

void APIENTRY gl_error_callback(GLenum source,
                                  GLenum type,
                                  GLuint id,
                                  GLenum severity,
                                  GLsizei /*length*/,
                                  const GLchar* message,
                                  const void* /*user_param*/) {
    switch (id) {
    case 131185: return; // Buffer detailed info
    }

    const char* source_str = "";
    switch (source) {
    case GL_DEBUG_SOURCE_API: source_str = "OGL API Call"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM: source_str = "Window System API Call"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: source_str = "Shader Compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY: source_str = "Third Party"; break;
    case GL_DEBUG_SOURCE_APPLICATION: source_str = "This Application"; break;
    case GL_DEBUG_SOURCE_OTHER: source_str = "Unknown"; break;
    }

    const char* type_str = "";
    switch (type) {
    case GL_DEBUG_TYPE_ERROR: type_str = "Error"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: type_str = "Deprecated behavior"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: type_str = "Undefined behavior"; break;
    case GL_DEBUG_TYPE_PORTABILITY: type_str = "Portability"; break;
    case GL_DEBUG_TYPE_PERFORMANCE: type_str = "Performance"; break;
    case GL_DEBUG_TYPE_MARKER: type_str = "Command stream annotation"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
    case GL_DEBUG_TYPE_POP_GROUP: type_str = "User defined"; break;
    case GL_DEBUG_TYPE_OTHER: type_str = "Unknown"; break;
    }

    const char* severity_str = "";
    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH: severity_str = "High"; break;
    case GL_DEBUG_SEVERITY_MEDIUM: severity_str = "Medium"; break;
    case GL_DEBUG_SEVERITY_LOW: severity_str = "Low"; break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: severity_str = "Notification"; break;
    }

    std::println(std::cerr, "GLWDebug: {} - {} {} ({}):\n{}", source_str, type_str, severity_str, id, message);
}

} // namespace


namespace glw {

void init(GLWLoadFunc func)
{
#define LOAD_OPENGL_FUNCTION(TYPE, NAME) NAME = load_gl_proc<TYPE>(#NAME, func);
FOR_OPENGL_FUNCTIONS(LOAD_OPENGL_FUNCTION)
#undef LOAD_OPENGL_FUNCTION

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(gl_error_callback, nullptr);
}

} // namespace glw
