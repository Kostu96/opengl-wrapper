#pragma once
#include <cut/auto_release.hpp>
#include <cut/non_copyable.hpp>
#include <cut/types.hpp>

#include <span>

namespace glw {

using cut::u32;

class Buffer final :
    cut::NonCopyable {
public:
    explicit Buffer(std::span<const std::byte> bytes);

    u32 get_native_handle() const { return handle_.get(); }
private:
    cut::AutoRelease<u32, void(*)(u32)> handle_;
};

} // namespace glw
