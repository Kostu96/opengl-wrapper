#include "glw/vertex_array.hpp"

#include "glw/buffers.hpp"

#include <glad/gl.h>

#include <cassert>

namespace glw {

    LayoutElement::LayoutElement(DataType inType, bool inPassAsFloat, bool inNormalized) :
        type{ inType },
        size{ dataTypeToSize(type) },
        offset{ 0 },
        passAsFloat{ inPassAsFloat },
        normalized{ inNormalized } {}

    size_t LayoutElement::dataTypeToSize(DataType type)
    {
        switch (type)
        {
        case DataType::F32:   return sizeof(float);
        case DataType::F32_2: return sizeof(float) * 2;
        case DataType::F32_3: return sizeof(float) * 3;
        case DataType::F32_4: return sizeof(float) * 4;
        case DataType::S32:   return sizeof(int32_t);
        case DataType::S32_2: return sizeof(int32_t) * 2;
        case DataType::S32_3: return sizeof(int32_t) * 3;
        case DataType::S32_4: return sizeof(int32_t) * 4;
        case DataType::U8_4:  return sizeof(uint8_t) * 4;
        case DataType::U16_2: return sizeof(uint16_t) * 2;
        case DataType::U32:   return sizeof(uint32_t);
        case DataType::U32_2: return sizeof(uint32_t) * 2;
        case DataType::U32_3: return sizeof(uint32_t) * 3;
        case DataType::U32_4: return sizeof(uint32_t) * 4;
        }

        assert(false);
        return 0;
    }

    VertexLayout::VertexLayout(const std::initializer_list<LayoutElement>& list) :
        m_elements(list)
    {
        uint32_t offset = 0;
        m_stride = 0;
        for (auto& element : m_elements)
        {
            element.offset = offset;
            offset += static_cast<uint32_t>(element.size);
            m_stride += static_cast<uint32_t>(element.size);
        }
    }

    VertexLayout::VertexLayout(const VertexLayout& other) :
        m_elements{ other.m_elements },
        m_stride{ other.m_stride } {}

    static uint32_t dataTypeToGLEnum(LayoutElement::DataType type)
    {
        switch (type)
        {
        case LayoutElement::DataType::F32:
        case LayoutElement::DataType::F32_2:
        case LayoutElement::DataType::F32_3:
        case LayoutElement::DataType::F32_4:
            return GL_FLOAT;
        case LayoutElement::DataType::S32:
        case LayoutElement::DataType::S32_2:
        case LayoutElement::DataType::S32_3:
        case LayoutElement::DataType::S32_4:
            return GL_INT;
        case LayoutElement::DataType::U8_4:
            return GL_UNSIGNED_BYTE;
        case LayoutElement::DataType::U16_2:
            return GL_UNSIGNED_SHORT;
        case LayoutElement::DataType::U32:
        case LayoutElement::DataType::U32_2:
        case LayoutElement::DataType::U32_3:
        case LayoutElement::DataType::U32_4:
            return GL_UNSIGNED_INT;
        }

        assert(false);
        return 0;
    }

    static int32_t dataTypeToCount(LayoutElement::DataType type)
    {
        switch (type)
        {
        case LayoutElement::DataType::F32:
        case LayoutElement::DataType::S32:
        case LayoutElement::DataType::U32:
            return 1;
        case LayoutElement::DataType::F32_2:
        case LayoutElement::DataType::S32_2:
        case LayoutElement::DataType::U16_2:
        case LayoutElement::DataType::U32_2:
            return 2;
        case LayoutElement::DataType::F32_3:
        case LayoutElement::DataType::S32_3:
        case LayoutElement::DataType::U32_3:
            return 3;
        case LayoutElement::DataType::F32_4:
        case LayoutElement::DataType::S32_4:
        case LayoutElement::DataType::U8_4:
        case LayoutElement::DataType::U32_4:
            return 4;
        }

        assert(false);
        return 0;
    }

    VertexArray::VertexArray(const VertexBuffer& vbo, const VertexLayout& layout) :
        m_VBO{ vbo },
        m_IBO{ nullptr }
    {   
        glCreateVertexArrays(1, &m_id);
        glBindVertexArray(m_id);
        vbo.bind();
        uint32_t index = 0;
        for (const auto& element : layout)
        {
            glEnableVertexAttribArray(index);
            
            if (dataTypeToGLEnum(element.type) == GL_FLOAT || element.passAsFloat)
                glVertexAttribPointer(index,
                    dataTypeToCount(element.type),
                    dataTypeToGLEnum(element.type),
                    element.normalized ? GL_TRUE : GL_FALSE,
                    static_cast<int>(layout.getStride()),
                    reinterpret_cast<const void*>(element.offset)
                );
            else    
                glVertexAttribIPointer(index,
                    dataTypeToCount(element.type),
                    dataTypeToGLEnum(element.type),
                    static_cast<int>(layout.getStride()),
                    reinterpret_cast<const void*>(element.offset)
                );

            ++index;
        }
    }

    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &m_id);
    }

    void VertexArray::bind() const
    {
        glBindVertexArray(m_id);
    }

    void VertexArray::unbind() const
    {
        glBindVertexArray(0);
    }

    void VertexArray::setIndexBuffer(const IndexBuffer& ibo)
    {
        m_IBO = &ibo;
        glBindVertexArray(m_id);
        ibo.bind();
    }

} // namespace jng
