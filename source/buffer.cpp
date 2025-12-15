#include "glw/buffer.hpp"
#include "glw/glw.hpp"

namespace glw {

Buffer::Buffer(std::span<const std::byte> bytes) :
    handle_(0u, [](cut::u32 handle){ glDeleteBuffers(1, &handle); }) {
    
    GLuint handle;
    glCreateBuffers(1, &handle);
    glNamedBufferStorage(handle, bytes.size(), bytes.data(), 0);

    handle_.reset(handle);
}

#pragma region VertexBuffer
    VertexBuffer::VertexBuffer(const void* vertices, size_t size)
    {
        glCreateBuffers(1, &m_id);
        //glNamedBufferData(m_id, static_cast<GLsizeiptr>(size), vertices, GL_STATIC_DRAW);
    }

    VertexBuffer::VertexBuffer(size_t size)
    {
        glCreateBuffers(1, &m_id);
        //glNamedBufferData(m_id, static_cast<GLsizeiptr>(size), nullptr, GL_DYNAMIC_DRAW);
    }

    VertexBuffer::~VertexBuffer()
    {
        glDeleteBuffers(1, &m_id);
    }

    void VertexBuffer::bind() const
    {
        //glBindBuffer(GL_ARRAY_BUFFER, m_id);
    }

    void VertexBuffer::unbind() const
    {
        //glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void VertexBuffer::setData(const void* data, size_t size) const
    {
        //glNamedBufferSubData(m_id, 0, static_cast<GLsizeiptr>(size), data);
    }
#pragma endregion

#pragma region IndexBuffer
    static size_t indexTypeToSize(IndexBuffer::IndexType type)
    {
        switch (type)
        {
        case IndexBuffer::IndexType::U8:  return sizeof(uint8_t);
        case IndexBuffer::IndexType::U16: return sizeof(uint16_t);
        case IndexBuffer::IndexType::U32: return sizeof(uint32_t);
        }

        return 0;
    }

    IndexBuffer::IndexBuffer(const void* indices, uint32_t count, IndexType type) :
        m_count{ count },
        m_indexType{ type }
    {
        glCreateBuffers(1, &m_id);
        //glNamedBufferData(m_id, static_cast<GLsizeiptr>(count * indexTypeToSize(type)), indices, GL_STATIC_DRAW);
    }

    IndexBuffer::~IndexBuffer()
    {
        glDeleteBuffers(1, &m_id);
    }

    void IndexBuffer::bind() const
    {
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    }

    void IndexBuffer::unbind() const
    {
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
#pragma endregion

#pragma region UniformBuffer
    UniformBuffer::UniformBuffer(size_t size)
    {
        glCreateBuffers(1, &m_id);
        //glNamedBufferData(m_id, static_cast<GLsizeiptr>(size), nullptr, GL_DYNAMIC_DRAW);
    }

    UniformBuffer::~UniformBuffer()
    {
        glDeleteBuffers(1, &m_id);
    }

    void UniformBuffer::bind(uint32_t slot) const
    {
        //glBindBufferBase(GL_UNIFORM_BUFFER, slot, m_id);
    }

    void UniformBuffer::unbind(uint32_t slot) const
    {
        //glBindBufferBase(GL_UNIFORM_BUFFER, slot, 0);
    }

    void UniformBuffer::setData(const void* data, size_t size, size_t offset) const
    {
        //glNamedBufferSubData(m_id, static_cast<GLintptr>(offset), static_cast<GLsizeiptr>(size), data);
    }
#pragma endregion

} // namespace glw
