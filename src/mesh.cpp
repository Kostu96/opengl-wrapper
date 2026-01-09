#include "glw/mesh.hpp"
#include "glw/glw.hpp"

#include <cut/exception.hpp>

namespace {

using namespace glw;

using cut::u8;
using cut::u16;

u32 to_size(Mesh::IndexType type) {
    switch (type) {
    using enum Mesh::IndexType;
    case U8:  return sizeof(u8);
    case U16: return sizeof(u16);
    case U32: return sizeof(u32);
    }

    throw cut::Exception("Unhandled index type!");
    return {};
}

} // namespace

namespace glw {

Mesh::Mesh(ByteView vertices, ByteView indices, IndexType index_type,
           std::initializer_list<VertexArray::DataType> vertex_layout) :
    vbo_{ vertices },
    ibo_{ indices },
    vao_{ vbo_, vertex_layout },
    index_count_{ cut::to_u32(indices.size()) / to_size(index_type) },
    index_type_{ index_type }
{
    vao_.set_index_buffer(ibo_);
}

u32 Mesh::to_gl_enum(IndexType type) {
    switch (type) {
    using enum IndexType;
    case U8:  return GL_UNSIGNED_BYTE;
    case U16: return GL_UNSIGNED_SHORT;
    case U32: return GL_UNSIGNED_INT;
    }

    throw cut::Exception("Unhandled index type!");
    return {};
}

} // namespace glw
