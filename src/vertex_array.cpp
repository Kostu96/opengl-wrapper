#include "glw/vertex_array.hpp"
#include "glw/buffer.hpp"
#include "glw/glw.hpp"

#include <cut/exception.hpp>

namespace {

using namespace glw;

u32 to_size(glw::LayoutElement::DataType type) {
    switch (type) {
    case LayoutElement::DataType::U8_4:
    case LayoutElement::DataType::U8_4_Norm:  return sizeof(u8) * 4;
    case LayoutElement::DataType::U16_2:
    case LayoutElement::DataType::U16_2_Norm: return sizeof(u16) * 2;
    case LayoutElement::DataType::U32:
    case LayoutElement::DataType::U32_Norm:   return sizeof(u32);
    case LayoutElement::DataType::U32_2:
    case LayoutElement::DataType::U32_2_Norm: return sizeof(u32) * 2;
    case LayoutElement::DataType::U32_3:
    case LayoutElement::DataType::U32_3_Norm: return sizeof(u32) * 3;
    case LayoutElement::DataType::U32_4:
    case LayoutElement::DataType::U32_4_Norm: return sizeof(u32) * 4;
    case LayoutElement::DataType::S32:
    case LayoutElement::DataType::S32_Norm:   return sizeof(s32);
    case LayoutElement::DataType::S32_2:
    case LayoutElement::DataType::S32_2_Norm: return sizeof(s32) * 2;
    case LayoutElement::DataType::S32_3:
    case LayoutElement::DataType::S32_3_Norm: return sizeof(s32) * 3;
    case LayoutElement::DataType::S32_4:
    case LayoutElement::DataType::S32_4_Norm: return sizeof(s32) * 4;
    case LayoutElement::DataType::F32:   return sizeof(f32);
    case LayoutElement::DataType::F32_2: return sizeof(f32) * 2;
    case LayoutElement::DataType::F32_3: return sizeof(f32) * 3;
    case LayoutElement::DataType::F32_4: return sizeof(f32) * 4;
    }

    throw cut::Exception("Unhandled data type!");
    return {};
}

GLint to_count(LayoutElement::DataType type) {
    switch (type) {
    case LayoutElement::DataType::U32:
    case LayoutElement::DataType::S32:
    case LayoutElement::DataType::U32_Norm:
    case LayoutElement::DataType::S32_Norm:
    case LayoutElement::DataType::F32: return 1;
    case LayoutElement::DataType::U16_2:
    case LayoutElement::DataType::U32_2:
    case LayoutElement::DataType::S32_2:
    case LayoutElement::DataType::U16_2_Norm:
    case LayoutElement::DataType::U32_2_Norm:
    case LayoutElement::DataType::S32_2_Norm:
    case LayoutElement::DataType::F32_2: return 2;
    case LayoutElement::DataType::U32_3:
    case LayoutElement::DataType::S32_3:
    case LayoutElement::DataType::U32_3_Norm:
    case LayoutElement::DataType::S32_3_Norm:
    case LayoutElement::DataType::F32_3: return 3;
    case LayoutElement::DataType::U8_4:
    case LayoutElement::DataType::U32_4:
    case LayoutElement::DataType::S32_4:
    case LayoutElement::DataType::U8_4_Norm:
    case LayoutElement::DataType::U32_4_Norm:
    case LayoutElement::DataType::S32_4_Norm:
    case LayoutElement::DataType::F32_4: return 4;
    }

    throw cut::Exception("Unhandled data type!");
    return {};
}

GLenum to_gl_enum(LayoutElement::DataType type) {
    switch (type) {
    case LayoutElement::DataType::U8_4:
    case LayoutElement::DataType::U8_4_Norm:
        return GL_UNSIGNED_BYTE;
    case LayoutElement::DataType::U16_2:
    case LayoutElement::DataType::U16_2_Norm:
        return GL_UNSIGNED_SHORT;
    case LayoutElement::DataType::U32:
    case LayoutElement::DataType::U32_2:
    case LayoutElement::DataType::U32_3:
    case LayoutElement::DataType::U32_4:
    case LayoutElement::DataType::U32_Norm:
    case LayoutElement::DataType::U32_2_Norm:
    case LayoutElement::DataType::U32_3_Norm:
    case LayoutElement::DataType::U32_4_Norm:
        return GL_UNSIGNED_INT;
    case LayoutElement::DataType::S32:
    case LayoutElement::DataType::S32_2:
    case LayoutElement::DataType::S32_3:
    case LayoutElement::DataType::S32_4:
    case LayoutElement::DataType::S32_Norm:
    case LayoutElement::DataType::S32_2_Norm:
    case LayoutElement::DataType::S32_3_Norm:
    case LayoutElement::DataType::S32_4_Norm:
        return GL_INT;
    case LayoutElement::DataType::F32:
    case LayoutElement::DataType::F32_2:
    case LayoutElement::DataType::F32_3:
    case LayoutElement::DataType::F32_4:
        return GL_FLOAT;
    }

    throw cut::Exception("Unhandled data type!");
    return {};
}

bool is_integer(glw::LayoutElement::DataType type) {
    switch (type) {
    case LayoutElement::DataType::F32:
    case LayoutElement::DataType::F32_2:
    case LayoutElement::DataType::F32_3:
    case LayoutElement::DataType::F32_4:
    case LayoutElement::DataType::S32_Norm:
    case LayoutElement::DataType::S32_2_Norm:
    case LayoutElement::DataType::S32_3_Norm:
    case LayoutElement::DataType::S32_4_Norm:
    case LayoutElement::DataType::U8_4_Norm:
    case LayoutElement::DataType::U16_2_Norm:
    case LayoutElement::DataType::U32_Norm:
    case LayoutElement::DataType::U32_2_Norm:
    case LayoutElement::DataType::U32_3_Norm:
    case LayoutElement::DataType::U32_4_Norm: return false;
    case LayoutElement::DataType::S32:
    case LayoutElement::DataType::S32_2:
    case LayoutElement::DataType::S32_3:
    case LayoutElement::DataType::S32_4:
    case LayoutElement::DataType::U8_4:
    case LayoutElement::DataType::U16_2:
    case LayoutElement::DataType::U32:
    case LayoutElement::DataType::U32_2:
    case LayoutElement::DataType::U32_3:
    case LayoutElement::DataType::U32_4: return true;
    }

    throw cut::Exception("Unhandled data type!");
    return {};
}

bool should_normalize(glw::LayoutElement::DataType type) {
    switch (type) {
    case LayoutElement::DataType::U8_4:
    case LayoutElement::DataType::U16_2:
    case LayoutElement::DataType::U32:
    case LayoutElement::DataType::U32_2:
    case LayoutElement::DataType::U32_3:
    case LayoutElement::DataType::U32_4:
    case LayoutElement::DataType::S32:
    case LayoutElement::DataType::S32_2:
    case LayoutElement::DataType::S32_3:
    case LayoutElement::DataType::S32_4:
    case LayoutElement::DataType::F32:
    case LayoutElement::DataType::F32_2:
    case LayoutElement::DataType::F32_3:
    case LayoutElement::DataType::F32_4: return false;
    case LayoutElement::DataType::U8_4_Norm:
    case LayoutElement::DataType::U16_2_Norm:
    case LayoutElement::DataType::U32_Norm:
    case LayoutElement::DataType::U32_2_Norm:
    case LayoutElement::DataType::U32_3_Norm:
    case LayoutElement::DataType::U32_4_Norm:
    case LayoutElement::DataType::S32_Norm:
    case LayoutElement::DataType::S32_2_Norm:
    case LayoutElement::DataType::S32_3_Norm:
    case LayoutElement::DataType::S32_4_Norm: return true;
    }

    throw cut::Exception("Unhandled data type!");
    return {};
}

} // namespace

namespace glw {

LayoutElement::LayoutElement(DataType type_) :
    offset(0),
    type(type_) {}

VertexLayout::VertexLayout(const std::initializer_list<LayoutElement>& list) :
    elements_(list) {
    
    u32 offset = 0;
    for (auto& e : elements_){
        e.offset = offset;
        offset += to_size(e.type);
    }
    stride_ = offset;
}

VertexArray::VertexArray(const Buffer& vertex_buffer, const VertexLayout& layout) :
    handle_(0u, [](u32 handle){ glDeleteVertexArrays(1, &handle); }) {   
    
    GLuint handle;
    glCreateVertexArrays(1, &handle);
    handle_.reset(handle);

    constexpr GLuint vertex_buffer_binding = 0;
    glVertexArrayVertexBuffer(handle, vertex_buffer_binding, vertex_buffer.get_native_handle(), 0, layout.get_stride());

    GLuint index = 0;
    for (const auto& element : layout) {
        glEnableVertexArrayAttrib(handle, index);
        glVertexArrayAttribBinding(handle, index, vertex_buffer_binding);
        
        if (is_integer(element.type)) {
            glVertexArrayAttribIFormat(handle, index,
                                       to_count(element.type), to_gl_enum(element.type),
                                       element.offset);
        }
        else {
            glVertexArrayAttribFormat(handle, index,
                                      to_count(element.type), to_gl_enum(element.type),
                                      should_normalize(element.type) ? GL_TRUE : GL_FALSE,
                                      element.offset);
        }
        index++;
    }
}

void VertexArray::set_index_buffer(const Buffer& index_buffer) {
    glVertexArrayElementBuffer(handle_.get(), index_buffer.get_native_handle());
}

void VertexArray::bind() const {
    glBindVertexArray(handle_.get());
}

} // namespace glw
