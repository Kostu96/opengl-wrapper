#include "glw/buffer.hpp"
#include "glw/glw.hpp"

namespace glw {

Buffer::Buffer(std::span<const std::byte> bytes) :
    handle_(0u, [](u32 handle){ glDeleteBuffers(1, &handle); }) {
    
    GLuint handle;
    glCreateBuffers(1, &handle);
    handle_.reset(handle);
    
    glNamedBufferStorage(handle, bytes.size(), bytes.data(), 0);
}

} // namespace glw
