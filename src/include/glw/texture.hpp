#pragma once
#include <cut/auto_release.hpp>
#include <cut/non_copyable.hpp>
#include <cut/types.hpp>

#include <span>

namespace glw {

using cut::u16;
using cut::u32;

enum class TextureFilter {
    Linear,
    Nearest
};

enum class TextureWrapMode {
    Repeat,
    ClampToEdge
};

struct SamplerDescription {
    TextureFilter filter = TextureFilter::Linear;
    TextureWrapMode wrap_mode = TextureWrapMode::Repeat;
};

class Sampler final :
    cut::NonCopyable {
public:
    explicit Sampler(const SamplerDescription& desc);

    void bind(u32 unit) const;
private:
    cut::AutoRelease<u32> handle_;
};

enum class TextureType {
    Texture2D,
    CubeMap
};

enum class TextureFormat {
    None,
    
    RGBA8, // normalized integer
    R32U,   // unsigned integer

    Depth24Stencil8
};

struct TextureDescription {
    TextureType type = TextureType::Texture2D;
    TextureFormat format = TextureFormat::RGBA8;
    u16 width = 1;
    u16 height = 1;
};

class Texture final :
    cut::NonCopyable {
public:
    explicit Texture(const TextureDescription& desc);

    void set_pixels(std::span<const std::byte> pixels, u16 x_offset = 0, u16 y_offset = 0, u16 width = 0, u16 height = 0) const;
    
    void bind(u32 unit) const;
private:
    cut::AutoRelease<u32> handle_;
    TextureDescription desc_;
};

} // namespace glw
