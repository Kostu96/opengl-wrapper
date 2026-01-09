#pragma once
#include "glw/buffer.hpp"
#include "glw/vertex_array.hpp"

#include <cut/types.hpp>

namespace glw {

using cut::u32;

class Mesh :
    cut::NonCopyable {
public:
    using ByteView = std::span<const std::byte>;
    enum class IndexType { U8, U16, U32 };

    Mesh(ByteView vertices, ByteView indices, IndexType index_type,
         std::initializer_list<VertexArray::DataType> vertex_layout);

    void bind() const { vao_.bind(); }

    u32 get_index_count() const { return index_count_; }
    IndexType get_index_type() const { return index_type_; }

    static u32 to_gl_enum(IndexType type);
private:
    glw::Buffer vbo_;
    glw::Buffer ibo_;
    glw::VertexArray vao_;
    u32 index_count_;
    IndexType index_type_;
};

} // namespace glw
