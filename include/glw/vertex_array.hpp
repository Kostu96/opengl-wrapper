#pragma once
#include <cstdint>
#include <vector>

namespace glw {

    class IndexBuffer;
    class VertexBuffer;
    class Shader;

    struct LayoutElement
    {
        enum class DataType
        {
            F32, F32_2, F32_3, F32_4,
            S32, S32_2, S32_3, S32_4,
            U8_4,
            U16_2,
            U32, U32_2, U32_3, U32_4,
        };

        LayoutElement(DataType inType, bool inPassAsFloat = false, bool inNormalized = false);

        static size_t dataTypeToSize(DataType type);

        DataType type;
        size_t size;
        uintptr_t offset;
        bool passAsFloat;
        bool normalized;
    };

    class VertexLayout final
    {
    public:
        using ContainerType = std::vector<LayoutElement>;
        using ContainerConstInterator = ContainerType::const_iterator;

        VertexLayout() = default;
        VertexLayout(const std::initializer_list<LayoutElement>& list);
        VertexLayout(const VertexLayout& other);

        const ContainerType& getElements() const { return m_elements; }
        uint32_t getStride() const { return m_stride; }

        ContainerConstInterator begin() const { return m_elements.begin(); }
        ContainerConstInterator end() const { return m_elements.end(); }
    private:
        ContainerType m_elements;
        uint32_t m_stride;
    };

    class VertexArray final
    {
    public:
        VertexArray(const VertexBuffer& vbo, const VertexLayout& layout);
        ~VertexArray();

        void bind() const;
        void unbind() const;

        const VertexBuffer& getVertexBuffer() const { return m_VBO; }
        void setIndexBuffer(const IndexBuffer& ibo);
        const IndexBuffer& getIndexBuffer() const { return *m_IBO; }
    private:
        uint32_t m_id;
        const VertexBuffer& m_VBO;
        const IndexBuffer* m_IBO;
    };

} // namespace glw
