#include "glw/buffer.hpp"
#include "glw/glw.hpp"

namespace glw {

Buffer::Buffer(std::span<const std::byte> bytes) :
    handle_(0u, [](u32 handle){ glDeleteBuffers(1, &handle); })
{    
    GLuint handle;
    glCreateBuffers(1, &handle);
    handle_.reset(handle);
    
    glNamedBufferStorage(handle, bytes.size(), bytes.data(), 0);
}

Buffer::Buffer(size_t size) :
    handle_(0u, [](u32 handle) { glDeleteBuffers(1, &handle); })
{
    GLuint handle;
    glCreateBuffers(1, &handle);
    handle_.reset(handle);

    glNamedBufferStorage(handle, size, nullptr, GL_DYNAMIC_STORAGE_BIT);
}

void Buffer::write(std::span<const std::byte> bytes) const {
    glNamedBufferSubData(handle_.get(), 0, bytes.size(), bytes.data());
}

} // namespace glw
