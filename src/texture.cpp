#include "glw/texture.hpp"
#include "glw/glw.hpp"

#include <cut/exception.hpp>

namespace {

using namespace glw;

GLenum to_gl_enum(TextureFilter filter) {
    switch (filter) {
    case TextureFilter::Linear:  return GL_LINEAR;
    case TextureFilter::Nearest: return GL_NEAREST;
    }

    throw cut::Exception("Unhandled texture filter!");
    return {};
}

GLenum to_gl_enum(TextureWrapMode wrap_mode) {
    switch (wrap_mode) {
    case TextureWrapMode::Repeat:      return GL_REPEAT;
    case TextureWrapMode::ClampToEdge: return GL_CLAMP_TO_EDGE;
    }

    throw cut::Exception("Unhandled texture wrap mode!");
    return {};
}

GLenum to_gl_enum(TextureType type) {
    switch (type) {
    case TextureType::Texture2D: return GL_TEXTURE_2D;
    case TextureType::Cubemap:   return GL_TEXTURE_CUBE_MAP;
    }

    throw cut::Exception("Unhandled texture format!");
    return {};
}

GLenum to_gl_enum(TextureFormat format) {
    switch (format) {
    using enum TextureFormat;
    case RGB8:            return GL_RGB8;
    case RGBA8:           return GL_RGBA8;
    case SRGB8:           return GL_SRGB8;
    case SRGB8Alpha8:     return GL_SRGB8_ALPHA8;
    case R32U:            return GL_R32UI;
    case Depth24Stencil8: return GL_DEPTH24_STENCIL8;
    case Depth32F:        return GL_DEPTH_COMPONENT32F;
    }

    throw cut::Exception("Unhandled texture format!");
    return {};
}

} // namespace

namespace glw {

Sampler::Sampler(const SamplerDescription& desc) :
    handle_(0u, [](u32 handle){ glDeleteSamplers(1, &handle); })
{
    GLuint handle;
    glCreateSamplers(1, &handle);
    handle_.reset(handle);

    glSamplerParameteri(handle, GL_TEXTURE_MIN_FILTER, to_gl_enum(desc.filter));
    glSamplerParameteri(handle, GL_TEXTURE_MAG_FILTER, to_gl_enum(desc.filter));
    glSamplerParameteri(handle, GL_TEXTURE_WRAP_S, to_gl_enum(desc.wrap_mode));
    glSamplerParameteri(handle, GL_TEXTURE_WRAP_T, to_gl_enum(desc.wrap_mode));
    glSamplerParameteri(handle, GL_TEXTURE_WRAP_R, to_gl_enum(desc.wrap_mode));
}

void Sampler::bind(u32 unit) const {
    glBindSampler(unit, handle_.get());
}

Texture::Texture(const TextureDescription &desc) :
    handle_(0u, [](u32 handle){ glDeleteTextures(1, &handle); }),
    desc_(desc)
{
    GLuint handle;
    glCreateTextures(to_gl_enum(desc.type), 1, &handle);
    handle_.reset(handle);

    glTextureStorage2D(handle, 1, to_gl_enum(desc.format), desc.width, desc.height);
}

void Texture::set_pixels_2d(std::span<const std::byte> pixels, u16 channels,
    u16 x_offset, u16 y_offset, u16 width, u16 height) const
{
    if (width == 0) width = desc_.width;
    if (height == 0) height = desc_.height;

    cut::ensure(width * height * channels == pixels.size(), "Setting pixels partially is unsupported!");

    glTextureSubImage2D(handle_.get(), 0,
                        x_offset, y_offset,
                        width, height,
                        channels == 4 ? GL_RGBA : GL_RGB,
                        GL_UNSIGNED_BYTE, pixels.data());
}

void Texture::set_pixels_3d(std::span<const std::byte> pixels, u16 channels,
    u16 x_offset, u16 y_offset, u16 z_offset, u16 width, u16 height) const
{
    if (width == 0) width = desc_.width;
    if (height == 0) height = desc_.height;

    cut::ensure(width * height * channels == pixels.size(), "Setting pixels partially is unsupported!");

    glTextureSubImage3D(handle_.get(), 0,
                        x_offset, y_offset, z_offset,
                        width, height, 1,
                        channels == 4 ? GL_RGBA : GL_RGB,
                        GL_UNSIGNED_BYTE, pixels.data());
}

void Texture::bind(u32 unit) const {
    glBindTextureUnit(unit, handle_.get());
}

} // namespace glw
