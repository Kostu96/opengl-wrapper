#include "glw/texture.hpp"

namespace glw {

glw::Sampler::Sampler() {
}

static uint32_t textureFormatToGLEnum(TextureFormat format)
{
    switch (format)
    {
    case TextureFormat::RGBA8:           return GL_RGBA8;
    case TextureFormat::R32:             return GL_R32I;
    case TextureFormat::Depth24Stencil8: return GL_DEPTH24_STENCIL8;
    default:
        assert(false);
        return 0;
    }
}

static uint32_t textureFilterToGLEnum(TextureFilter filter)
{
    switch (filter)
    {
    case TextureFilter::Linear:  return GL_LINEAR;
    case TextureFilter::Nearest: return GL_NEAREST;
    default:
        assert(false);
        return 0;
    }
}

static uint32_t textureWrapModeToGLEnum(TextureWrapMode wrapMode)
{
    switch (wrapMode)
    {
    case TextureWrapMode::Clamp: return GL_CLAMP_TO_EDGE;
    case TextureWrapMode::Repeat: return GL_REPEAT;
    default:
        assert(false);
        return 0;
    }
}

Texture::Texture(const char* path, bool flip)
{
    stbi_set_flip_vertically_on_load(flip);

    int width, height, channels;
    stbi_uc* data = stbi_load(path, &width, &height, &channels, 4);
    assert(data && "Failed to load texture!");

    m_properties.specification.format = TextureFormat::RGBA8;
    m_properties.width = static_cast<uint32_t>(width);
    m_properties.height = static_cast<uint32_t>(height);

    createTexture();
    glTextureSubImage2D(m_id, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
}

Texture::Texture(const Properties& properties) :
    m_properties{ properties }
{
    createTexture();
}

Texture::Texture(Texture&& other) noexcept :
    m_properties{ other.m_properties },
    m_id{ other.m_id } {}

Texture::~Texture()
{
    glDeleteTextures(1, &m_id);
}

void Texture::bind(uint32_t slot) const
{
    glBindTextureUnit(slot, m_id);
}

void Texture::unbind(uint32_t slot) const
{
    glBindTextureUnit(slot, 0);
}

void Texture::setData(const void* data, size_t size, uint32_t xoffset, uint32_t yoffset, uint32_t width, uint32_t height) const
{
    if (width == 0) width = m_properties.width;
    if (height == 0) height = m_properties.height;

    assert(width * height * 4 == size);

    glTextureSubImage2D(
        m_id, 0,
        xoffset, yoffset,
        width, height,
        GL_RGBA, GL_UNSIGNED_BYTE, data
    );
}

void Texture::createTexture()
{
    const auto& spec = m_properties.specification;

    glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
    glTextureStorage2D(m_id, 1,
        textureFormatToGLEnum(spec.format),
        static_cast<int>(m_properties.width),
        static_cast<int>(m_properties.height)
    );

    glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, textureFilterToGLEnum(spec.minificationFilter));
    glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, textureFilterToGLEnum(spec.magnificationFilter));
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_R, textureWrapModeToGLEnum(spec.wrapMode));
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, textureWrapModeToGLEnum(spec.wrapMode));
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, textureWrapModeToGLEnum(spec.wrapMode));
}

} // namespace glw
