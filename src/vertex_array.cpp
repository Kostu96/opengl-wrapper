#include "glw/vertex_array.hpp"
#include "glw/buffer.hpp"
#include "glw/glw.hpp"

#include <cut/exception.hpp>

namespace {

using namespace glw;
using cut::u8;
using cut::u16;
using cut::s32;
using cut::f32;

u32 to_size(VertexArray::DataType type) {
    switch (type) {
    case VertexArray::DataType::U8_4:
    case VertexArray::DataType::U8_4_Norm:  return sizeof(u8) * 4;
    case VertexArray::DataType::U16_2:
    case VertexArray::DataType::U16_2_Norm: return sizeof(u16) * 2;
    case VertexArray::DataType::U32:
    case VertexArray::DataType::U32_Norm:   return sizeof(u32);
    case VertexArray::DataType::U32_2:
    case VertexArray::DataType::U32_2_Norm: return sizeof(u32) * 2;
    case VertexArray::DataType::U32_3:
    case VertexArray::DataType::U32_3_Norm: return sizeof(u32) * 3;
    case VertexArray::DataType::U32_4:
    case VertexArray::DataType::U32_4_Norm: return sizeof(u32) * 4;
    case VertexArray::DataType::S32:
    case VertexArray::DataType::S32_Norm:   return sizeof(s32);
    case VertexArray::DataType::S32_2:
    case VertexArray::DataType::S32_2_Norm: return sizeof(s32) * 2;
    case VertexArray::DataType::S32_3:
    case VertexArray::DataType::S32_3_Norm: return sizeof(s32) * 3;
    case VertexArray::DataType::S32_4:
    case VertexArray::DataType::S32_4_Norm: return sizeof(s32) * 4;
    case VertexArray::DataType::F32:   return sizeof(f32);
    case VertexArray::DataType::F32_2: return sizeof(f32) * 2;
    case VertexArray::DataType::F32_3: return sizeof(f32) * 3;
    case VertexArray::DataType::F32_4: return sizeof(f32) * 4;
    }

    throw cut::Exception("Unhandled data type!");
    return {};
}

GLint to_count(VertexArray::DataType type) {
    switch (type) {
    case VertexArray::DataType::U32:
    case VertexArray::DataType::S32:
    case VertexArray::DataType::U32_Norm:
    case VertexArray::DataType::S32_Norm:
    case VertexArray::DataType::F32: return 1;
    case VertexArray::DataType::U16_2:
    case VertexArray::DataType::U32_2:
    case VertexArray::DataType::S32_2:
    case VertexArray::DataType::U16_2_Norm:
    case VertexArray::DataType::U32_2_Norm:
    case VertexArray::DataType::S32_2_Norm:
    case VertexArray::DataType::F32_2: return 2;
    case VertexArray::DataType::U32_3:
    case VertexArray::DataType::S32_3:
    case VertexArray::DataType::U32_3_Norm:
    case VertexArray::DataType::S32_3_Norm:
    case VertexArray::DataType::F32_3: return 3;
    case VertexArray::DataType::U8_4:
    case VertexArray::DataType::U32_4:
    case VertexArray::DataType::S32_4:
    case VertexArray::DataType::U8_4_Norm:
    case VertexArray::DataType::U32_4_Norm:
    case VertexArray::DataType::S32_4_Norm:
    case VertexArray::DataType::F32_4: return 4;
    }

    throw cut::Exception("Unhandled data type!");
    return {};
}

GLenum to_gl_enum(VertexArray::DataType type) {
    switch (type) {
    case VertexArray::DataType::U8_4:
    case VertexArray::DataType::U8_4_Norm:
        return GL_UNSIGNED_BYTE;
    case VertexArray::DataType::U16_2:
    case VertexArray::DataType::U16_2_Norm:
        return GL_UNSIGNED_SHORT;
    case VertexArray::DataType::U32:
    case VertexArray::DataType::U32_2:
    case VertexArray::DataType::U32_3:
    case VertexArray::DataType::U32_4:
    case VertexArray::DataType::U32_Norm:
    case VertexArray::DataType::U32_2_Norm:
    case VertexArray::DataType::U32_3_Norm:
    case VertexArray::DataType::U32_4_Norm:
        return GL_UNSIGNED_INT;
    case VertexArray::DataType::S32:
    case VertexArray::DataType::S32_2:
    case VertexArray::DataType::S32_3:
    case VertexArray::DataType::S32_4:
    case VertexArray::DataType::S32_Norm:
    case VertexArray::DataType::S32_2_Norm:
    case VertexArray::DataType::S32_3_Norm:
    case VertexArray::DataType::S32_4_Norm:
        return GL_INT;
    case VertexArray::DataType::F32:
    case VertexArray::DataType::F32_2:
    case VertexArray::DataType::F32_3:
    case VertexArray::DataType::F32_4:
        return GL_FLOAT;
    }

    throw cut::Exception("Unhandled data type!");
    return {};
}

bool is_integer(VertexArray::DataType type) {
    switch (type) {
    case VertexArray::DataType::F32:
    case VertexArray::DataType::F32_2:
    case VertexArray::DataType::F32_3:
    case VertexArray::DataType::F32_4:
    case VertexArray::DataType::S32_Norm:
    case VertexArray::DataType::S32_2_Norm:
    case VertexArray::DataType::S32_3_Norm:
    case VertexArray::DataType::S32_4_Norm:
    case VertexArray::DataType::U8_4_Norm:
    case VertexArray::DataType::U16_2_Norm:
    case VertexArray::DataType::U32_Norm:
    case VertexArray::DataType::U32_2_Norm:
    case VertexArray::DataType::U32_3_Norm:
    case VertexArray::DataType::U32_4_Norm: return false;
    case VertexArray::DataType::S32:
    case VertexArray::DataType::S32_2:
    case VertexArray::DataType::S32_3:
    case VertexArray::DataType::S32_4:
    case VertexArray::DataType::U8_4:
    case VertexArray::DataType::U16_2:
    case VertexArray::DataType::U32:
    case VertexArray::DataType::U32_2:
    case VertexArray::DataType::U32_3:
    case VertexArray::DataType::U32_4: return true;
    }

    throw cut::Exception("Unhandled data type!");
    return {};
}

bool should_normalize(VertexArray::DataType type) {
    switch (type) {
    case VertexArray::DataType::U8_4:
    case VertexArray::DataType::U16_2:
    case VertexArray::DataType::U32:
    case VertexArray::DataType::U32_2:
    case VertexArray::DataType::U32_3:
    case VertexArray::DataType::U32_4:
    case VertexArray::DataType::S32:
    case VertexArray::DataType::S32_2:
    case VertexArray::DataType::S32_3:
    case VertexArray::DataType::S32_4:
    case VertexArray::DataType::F32:
    case VertexArray::DataType::F32_2:
    case VertexArray::DataType::F32_3:
    case VertexArray::DataType::F32_4: return false;
    case VertexArray::DataType::U8_4_Norm:
    case VertexArray::DataType::U16_2_Norm:
    case VertexArray::DataType::U32_Norm:
    case VertexArray::DataType::U32_2_Norm:
    case VertexArray::DataType::U32_3_Norm:
    case VertexArray::DataType::U32_4_Norm:
    case VertexArray::DataType::S32_Norm:
    case VertexArray::DataType::S32_2_Norm:
    case VertexArray::DataType::S32_3_Norm:
    case VertexArray::DataType::S32_4_Norm: return true;
    }

    throw cut::Exception("Unhandled data type!");
    return {};
}

} // namespace

namespace glw {

VertexArray::VertexArray(const Buffer& vertex_buffer, const std::initializer_list<DataType>& layout) :
    handle_(0u, [](u32 handle){ glDeleteVertexArrays(1, &handle); }) {   
    
    GLuint handle;
    glCreateVertexArrays(1, &handle);
    handle_.reset(handle);

    constexpr GLuint vertex_buffer_binding = 0;
    u32 offset = 0;
    GLuint index = 0;
    for (const auto& element_type : layout) {
        glEnableVertexArrayAttrib(handle, index);
        glVertexArrayAttribBinding(handle, index, vertex_buffer_binding);
        
        if (is_integer(element_type)) {
            glVertexArrayAttribIFormat(handle, index,
                to_count(element_type), to_gl_enum(element_type),
                offset);
        }
        else {
            glVertexArrayAttribFormat(handle, index,
                to_count(element_type), to_gl_enum(element_type),
                should_normalize(element_type) ? GL_TRUE : GL_FALSE,
                offset);
        }
        index++;
        offset += to_size(element_type);
    }

    glVertexArrayVertexBuffer(handle, vertex_buffer_binding, vertex_buffer.get_native_handle(), 0, offset);
}

void VertexArray::set_index_buffer(const Buffer& index_buffer) {
    glVertexArrayElementBuffer(handle_.get(), index_buffer.get_native_handle());
}

void VertexArray::bind() const {
    glBindVertexArray(handle_.get());
}

} // namespace glw
