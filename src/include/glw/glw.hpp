#pragma once

#ifdef _WIN32

#ifndef APIENTRY
#define APIENTRY __stdcall
#endif

#endif // _WIN32

#include <GL/glcorearb.h>

#define FOR_OPENGL_FUNCTIONS(DO)                                       \
    DO(PFNGLENABLEPROC,                    glEnable)                   \
    DO(PFNGLDEBUGMESSAGECALLBACKPROC,      glDebugMessageCallback)     \
    DO(PFNGLGETINTEGERVPROC,               glGetIntegerv)              \
    DO(PFNGLVIEWPORTPROC,                  glViewport)                 \
    DO(PFNGLCLEARCOLORPROC,                glClearColor)               \
    DO(PFNGLCLEARPROC,                     glClear)                    \
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
    DO(PFNGLUSEPROGRAMPROC,                glUseProgram)               \
    DO(PFNGLGETACTIVEUNIFORMPROC,          glGetActiveUniform)         \
    DO(PFNGLGETUNIFORMLOCATIONPROC,        glGetUniformLocation)       \
    DO(PFNGLPROGRAMUNIFORM1FPROC,          glProgramUniform1f)         \
    DO(PFNGLPROGRAMUNIFORM3FPROC,          glProgramUniform3f)         \
    DO(PFNGLPROGRAMUNIFORM3FVPROC,         glProgramUniform3fv)        \
    DO(PFNGLPROGRAMUNIFORMMATRIX3FVPROC,   glProgramUniformMatrix3fv)  \
    DO(PFNGLPROGRAMUNIFORMMATRIX4FVPROC,   glProgramUniformMatrix4fv)  \
    DO(PFNGLCREATESAMPLERSPROC,            glCreateSamplers)           \
    DO(PFNGLDELETESAMPLERSPROC,            glDeleteSamplers)           \
    DO(PFNGLSAMPLERPARAMETERIPROC,         glSamplerParameteri)        \
    DO(PFNGLBINDSAMPLERPROC,               glBindSampler)              \
    DO(PFNGLCREATETEXTURESPROC,            glCreateTextures)           \
    DO(PFNGLDELETETEXTURESPROC,            glDeleteTextures)           \
    DO(PFNGLTEXTURESTORAGE2DPROC,          glTextureStorage2D)         \
    DO(PFNGLTEXTURESUBIMAGE2DPROC,         glTextureSubImage2D)        \
    DO(PFNGLBINDTEXTUREUNITPROC,           glBindTextureUnit)          \
    DO(PFNGLDRAWELEMENTSPROC,              glDrawElements)

#define DECLARE_OPENGL_FUNCTION(TYPE, NAME) inline TYPE NAME = nullptr;
FOR_OPENGL_FUNCTIONS(DECLARE_OPENGL_FUNCTION)
#undef DECLARE_OPENGL_FUNCTION

namespace glw {
    
typedef void (*GLWApiProc)(void);
typedef GLWApiProc(*GLWLoadFunc)(const char* name);

void init(GLWLoadFunc func);

} // namespace glw
