#pragma once

#include <glw/buffers.hpp>
#include <glw/framebuffer.hpp>
#include <glw/renderer.hpp>
#include <glw/shader.hpp>
#include <glw/texture.hpp>
#include <glw/vertex_array.hpp>

namespace glw {
    
    typedef void (*GLWApiProc)(void);
    typedef GLWApiProc(*GLWLoadFunc)(const char* name);

    bool init(GLWLoadFunc func);

} // namespace glw
