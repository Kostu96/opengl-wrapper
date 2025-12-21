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
    /*
    * Creates immutable Buffer and upload bytes into it
    */
    explicit Buffer(std::span<const std::byte> bytes);

    /*
    * Creates writable Buffer with a given size
    */
    explicit Buffer(size_t size);

    void write(std::span<const std::byte> bytes) const;

    u32 get_native_handle() const { return handle_.get(); }
private:
    cut::AutoRelease<u32> handle_;
};

} // namespace glw
