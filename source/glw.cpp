#include "glw/glw.hpp"

#include <glad/gl.h>

#include <iostream>

namespace glw {

#include "glw/error_callback.inl"

    bool init(GLWLoadFunc func)
    {
        if (!gladLoadGL(func)) return false;

        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(OGLErrorCallback, nullptr);

        return true;
    }

} // namespace glw
