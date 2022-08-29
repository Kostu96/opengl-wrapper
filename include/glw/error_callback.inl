static void GLAPIENTRY OGLErrorCallback(
    GLenum source,
    GLenum type,
    GLuint /*id*/,
    GLenum severity,
    GLsizei /*length*/,
    const GLchar* message,
    const void* /*userParam*/)
{
    const char* sourceStr = "";
    switch (source) {
    case GL_DEBUG_SOURCE_API: sourceStr = "OGL API Call"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM: sourceStr = "Window System API Call"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: sourceStr = "Shader Compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY: sourceStr = "Third Party"; break;
    case GL_DEBUG_SOURCE_APPLICATION: sourceStr = "This Application"; break;
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

    const char* severityStr = "";
    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH: severityStr = "High"; break;
    case GL_DEBUG_SEVERITY_MEDIUM: severityStr = "Medium"; break;
    case GL_DEBUG_SEVERITY_LOW: severityStr = "Low"; break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: severityStr = "Notification"; break;
    }

    std::cerr << "OGLdebug: " << sourceStr << " | Type: " << typeStr << severityStr << '\n' << message << '\n';
}
