#pragma once
#include <cut/auto_release.hpp>
#include <cut/non_copyable.hpp>
#include <cut/types.hpp>

#include <vector>

namespace glw {

using cut::u32;

class Buffer;

class VertexArray final :
    cut::NonCopyable {
public:
    enum class DataType {
        U8_4,
        U16_2,
        U32, U32_2, U32_3, U32_4,
        S32, S32_2, S32_3, S32_4,
        U8_4_Norm,
        U16_2_Norm,
        U32_Norm, U32_2_Norm, U32_3_Norm, U32_4_Norm,
        S32_Norm, S32_2_Norm, S32_3_Norm, S32_4_Norm,
        F32, F32_2, F32_3, F32_4
    };

    VertexArray(const Buffer& vertex_buffer, const std::initializer_list<DataType>& layout);

    void set_index_buffer(const Buffer& index_buffer);

    void bind() const;
private:
    cut::AutoRelease<u32> handle_;
};

} // namespace glw
