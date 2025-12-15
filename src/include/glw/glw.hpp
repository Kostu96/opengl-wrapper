#pragma once
#include <GL/gl.h>
#include <GL/glcorearb.h>

#define FOR_OPENGL_FUNCTIONS(DO)                                       \
    DO(PFNGLDEBUGMESSAGECALLBACKPROC,      glDebugMessageCallback)     \
    DO(PFNGLCREATEBUFFERSPROC,             glCreateBuffers)            \
    DO(PFNGLDELETEBUFFERSPROC,             glDeleteBuffers)            \
    DO(PFNGLNAMEDBUFFERSTORAGEPROC,        glNamedBufferStorage)       \
    DO(PFNGLCREATEVERTEXARRAYSPROC,        glCreateVertexArrays)       \
    DO(PFNGLDELETEVERTEXARRAYSPROC,        glDeleteVertexArrays)       \
    DO(PFNGLVERTEXARRAYVERTEXBUFFERPROC,   glVertexArrayVertexBuffer)  \
    DO(PFNGLVERTEXARRAYELEMENTBUFFERPROC,  glVertexArrayElementBuffer) \
    DO(PFNGLENABLEVERTEXARRAYATTRIBPROC,   glEnableVertexArrayAttrib)  \
    DO(PFNGLVERTEXARRAYATTRIBBINDINGPROC,  glVertexArrayAttribBinding) \
    DO(PFNGLVERTEXARRAYATTRIBFORMATPROC,   glVertexArrayAttribFormat)  \
    DO(PFNGLVERTEXARRAYATTRIBIFORMATPROC,  glVertexArrayAttribIFormat) \
    DO(PFNGLBINDVERTEXARRAYPROC,           glBindVertexArray)          \
    DO(PFNGLCREATESHADERPROC,              glCreateShader)             \
    DO(PFNGLDELETESHADERPROC,              glDeleteShader)             \
    DO(PFNGLSHADERSOURCEPROC,              glShaderSource)             \
    DO(PFNGLCOMPILESHADERPROC,             glCompileShader)            \
    DO(PFNGLGETSHADERIVPROC,               glGetShaderiv)              \
    DO(PFNGLGETSHADERINFOLOGPROC,          glGetShaderInfoLog)         \
    DO(PFNGLCREATEPROGRAMPROC,             glCreateProgram)            \
    DO(PFNGLDELETEPROGRAMPROC,             glDeleteProgram)            \
    DO(PFNGLATTACHSHADERPROC,              glAttachShader)             \
    DO(PFNGLDETACHSHADERPROC,              glDetachShader)             \
    DO(PFNGLLINKPROGRAMPROC,               glLinkProgram)              \
    DO(PFNGLGETPROGRAMIVPROC,              glGetProgramiv)             \
    DO(PFNGLGETPROGRAMINFOLOGPROC,         glGetProgramInfoLog)        \
    DO(PFNGLUSEPROGRAMPROC,                glUseProgram)

#define DECLARE_OPENGL_FUNCTION(TYPE, NAME) inline TYPE NAME = nullptr;
FOR_OPENGL_FUNCTIONS(DECLARE_OPENGL_FUNCTION)
#undef DECLARE_OPENGL_FUNCTION

namespace glw {
    
    typedef void (*GLWApiProc)(void);
    typedef GLWApiProc(*GLWLoadFunc)(const char* name);

    bool init(GLWLoadFunc func);

} // namespace glw
