#pragma once
#include <cstddef>
#include <cstdint>

namespace glw {

    class VertexBuffer final
    {
    public:
        VertexBuffer(const void* vertices, size_t size);
        explicit VertexBuffer(size_t size);
        ~VertexBuffer();

        void bind() const;
        void unbind() const;
        void setData(const void* data, size_t size) const;

        VertexBuffer(const VertexBuffer&) = delete;
        VertexBuffer& operator=(const VertexBuffer&) = delete;
    private:
        uint32_t m_id;
    };

    class IndexBuffer final
    {
    public:
        enum class IndexType {
            U8,
            U16,
            U32
        };

        IndexBuffer(const void* indices, uint32_t count, IndexType type);
        ~IndexBuffer();

        void bind() const;
        void unbind() const;
        uint32_t getCount() const { return m_count; }
        IndexType getIndexType() const { return m_indexType; }

        IndexBuffer(const IndexBuffer&) = delete;
        IndexBuffer& operator=(const IndexBuffer&) = delete;
    private:
        uint32_t m_id;
        uint32_t m_count;
        IndexType m_indexType;
    };

    class UniformBuffer final
    {
    public:
        explicit UniformBuffer(size_t size);
        ~UniformBuffer();

        void bind(uint32_t slot) const;
        void unbind(uint32_t slot) const;
        void setData(const void* data, size_t size, size_t offset = 0) const;

        UniformBuffer(const UniformBuffer&) = delete;
        UniformBuffer& operator=(const UniformBuffer&) = delete;
    private:
        uint32_t m_id;
    };

} // namespace glw
