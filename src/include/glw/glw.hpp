#pragma once

#ifdef _WIN32

#ifndef APIENTRY
#define APIENTRY __stdcall
#endif

#endif // _WIN32

#include <GL/glcorearb.h>

#define FOR_OPENGL_FUNCTIONS(DO)                                            \
    DO(PFNGLATTACHSHADERPROC,                glAttachShader)                \
    DO(PFNGLBINDBUFFERBASEPROC,              glBindBufferBase)              \
    DO(PFNGLBINDFRAMEBUFFERPROC,             glBindFramebuffer)             \
    DO(PFNGLBINDSAMPLERPROC,                 glBindSampler)                 \
    DO(PFNGLBINDTEXTUREUNITPROC,             glBindTextureUnit)             \
    DO(PFNGLBINDVERTEXARRAYPROC,             glBindVertexArray)             \
    DO(PFNGLBLITNAMEDFRAMEBUFFERPROC,        glBlitNamedFramebuffer)        \
    DO(PFNGLCHECKNAMEDFRAMEBUFFERSTATUSPROC, glCheckNamedFramebufferStatus) \
    DO(PFNGLCLEARPROC,                       glClear)                       \
    DO(PFNGLCLEARCOLORPROC,                  glClearColor)                  \
    DO(PFNGLCLEARNAMEDFRAMEBUFFERFVPROC,     glClearNamedFramebufferfv)     \
    DO(PFNGLCLEARNAMEDFRAMEBUFFERIVPROC,     glClearNamedFramebufferiv)     \
    DO(PFNGLCOMPILESHADERPROC,               glCompileShader)               \
    DO(PFNGLCREATEBUFFERSPROC,               glCreateBuffers)               \
    DO(PFNGLCREATEFRAMEBUFFERSPROC,          glCreateFramebuffers)          \
    DO(PFNGLCREATEPROGRAMPROC,               glCreateProgram)               \
    DO(PFNGLCREATESAMPLERSPROC,              glCreateSamplers)              \
    DO(PFNGLCREATESHADERPROC,                glCreateShader)                \
    DO(PFNGLCREATETEXTURESPROC,              glCreateTextures)              \
    DO(PFNGLCREATEVERTEXARRAYSPROC,          glCreateVertexArrays)          \
    DO(PFNGLDEBUGMESSAGECALLBACKPROC,        glDebugMessageCallback)        \
    DO(PFNGLDEPTHFUNCPROC,                   glDepthFunc)                   \
    DO(PFNGLDEPTHMASKPROC,                   glDepthMask)                   \
    DO(PFNGLDELETEBUFFERSPROC,               glDeleteBuffers)               \
    DO(PFNGLDELETEFRAMEBUFFERSPROC,          glDeleteFramebuffers)          \
    DO(PFNGLDELETEPROGRAMPROC,               glDeleteProgram)               \
    DO(PFNGLDELETESAMPLERSPROC,              glDeleteSamplers)              \
    DO(PFNGLDELETESHADERPROC,                glDeleteShader)                \
    DO(PFNGLDELETETEXTURESPROC,              glDeleteTextures)              \
    DO(PFNGLDELETEVERTEXARRAYSPROC,          glDeleteVertexArrays)          \
    DO(PFNGLDETACHSHADERPROC,                glDetachShader)                \
    DO(PFNGLDISABLEPROC,                     glDisable)                     \
    DO(PFNGLDRAWARRAYSPROC,                  glDrawArrays)                  \
    DO(PFNGLDRAWELEMENTSPROC,                glDrawElements)                \
    DO(PFNGLENABLEPROC,                      glEnable)                      \
    DO(PFNGLENABLEVERTEXARRAYATTRIBPROC,     glEnableVertexArrayAttrib)     \
    DO(PFNGLGENERATETEXTUREMIPMAPPROC,       glGenerateTextureMipmap)       \
    DO(PFNGLGETACTIVEUNIFORMPROC,            glGetActiveUniform)            \
    DO(PFNGLGETFLOATVPROC,                   glGetFloatv)                   \
    DO(PFNGLGETINTEGERVPROC,                 glGetIntegerv)                 \
    DO(PFNGLGETPROGRAMINFOLOGPROC,           glGetProgramInfoLog)           \
    DO(PFNGLGETPROGRAMIVPROC,                glGetProgramiv)                \
    DO(PFNGLGETSHADERINFOLOGPROC,            glGetShaderInfoLog)            \
    DO(PFNGLGETSHADERIVPROC,                 glGetShaderiv)                 \
    DO(PFNGLGETSTRINGIPROC,                  glGetStringi)                  \
    DO(PFNGLGETUNIFORMLOCATIONPROC,          glGetUniformLocation)          \
    DO(PFNGLLINEWIDTHPROC,                   glLineWidth)                   \
    DO(PFNGLLINKPROGRAMPROC,                 glLinkProgram)                 \
    DO(PFNGLNAMEDBUFFERSUBDATAPROC,          glNamedBufferSubData)          \
    DO(PFNGLNAMEDBUFFERSTORAGEPROC,          glNamedBufferStorage)          \
    DO(PFNGLNAMEDFRAMEBUFFERDRAWBUFFERSPROC, glNamedFramebufferDrawBuffers) \
    DO(PFNGLNAMEDFRAMEBUFFERTEXTUREPROC,     glNamedFramebufferTexture)     \
    DO(PFNGLPROGRAMUNIFORM1FPROC,            glProgramUniform1f)            \
    DO(PFNGLPROGRAMUNIFORM1IPROC,            glProgramUniform1i)            \
    DO(PFNGLPROGRAMUNIFORM3FPROC,            glProgramUniform3f)            \
    DO(PFNGLPROGRAMUNIFORM3FVPROC,           glProgramUniform3fv)           \
    DO(PFNGLPROGRAMUNIFORMMATRIX3FVPROC,     glProgramUniformMatrix3fv)     \
    DO(PFNGLPROGRAMUNIFORMMATRIX4FVPROC,     glProgramUniformMatrix4fv)     \
    DO(PFNGLSAMPLERPARAMETERFPROC,           glSamplerParameterf)           \
    DO(PFNGLSAMPLERPARAMETERIPROC,           glSamplerParameteri)           \
    DO(PFNGLSHADERSOURCEPROC,                glShaderSource)                \
    DO(PFNGLTEXTURESTORAGE2DPROC,            glTextureStorage2D)            \
    DO(PFNGLTEXTURESUBIMAGE2DPROC,           glTextureSubImage2D)           \
    DO(PFNGLTEXTURESUBIMAGE3DPROC,           glTextureSubImage3D)           \
    DO(PFNGLUSEPROGRAMPROC,                  glUseProgram)                  \
    DO(PFNGLVERTEXARRAYATTRIBBINDINGPROC,    glVertexArrayAttribBinding)    \
    DO(PFNGLVERTEXARRAYATTRIBFORMATPROC,     glVertexArrayAttribFormat)     \
    DO(PFNGLVERTEXARRAYATTRIBIFORMATPROC,    glVertexArrayAttribIFormat)    \
    DO(PFNGLVERTEXARRAYELEMENTBUFFERPROC,    glVertexArrayElementBuffer)    \
    DO(PFNGLVERTEXARRAYVERTEXBUFFERPROC,     glVertexArrayVertexBuffer)     \
    DO(PFNGLVIEWPORTPROC,                    glViewport)

#define DECLARE_OPENGL_FUNCTION(TYPE, NAME) inline TYPE NAME = nullptr;
FOR_OPENGL_FUNCTIONS(DECLARE_OPENGL_FUNCTION)
#undef DECLARE_OPENGL_FUNCTION

namespace glw {
    
typedef void (*GLWApiProc)(void);
typedef GLWApiProc(*GLWLoadFunc)(const char* name);

void init(GLWLoadFunc func);

} // namespace glw
