static void GLAPIENTRY OGLErrorCallback(
    GLenum source,
    GLenum type,
    GLuint /*id*/,
    GLenum /*severity*/,
    GLsizei /*length*/,
    const GLchar* message,
    const void* /*userParam*/)
{
    const char* sourceStr = "";
    switch (source) {
    case GL_DEBUG_SOURCE_API: sourceStr = "Call to the OpenGL API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM: sourceStr = "Call to a window-system API"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: sourceStr = "Compiler for a shading language"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY: sourceStr = "Application associated with OpenGL"; break;
    case GL_DEBUG_SOURCE_APPLICATION: sourceStr = "User of this application"; break;
    case GL_DEBUG_SOURCE_OTHER: sourceStr = "Unknown"; break;
    }

    const char* typeStr = "";
    switch (type) {
    case GL_DEBUG_TYPE_ERROR: typeStr = "Error"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: typeStr = "Deprecated behavior"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: typeStr = "Undefined behavior"; break;
    case GL_DEBUG_TYPE_PORTABILITY: typeStr = "Portability"; break;
    case GL_DEBUG_TYPE_PERFORMANCE: typeStr = "Performance"; break;
    case GL_DEBUG_TYPE_MARKER: typeStr = "Command stream annotation"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
    case GL_DEBUG_TYPE_POP_GROUP: typeStr = "User defined"; break;
    case GL_DEBUG_TYPE_OTHER: typeStr = "Unknown"; break;
    }

    std::cerr << "OpenGL debug | " << sourceStr << " | Type: " << typeStr << '\n' << message;
}
