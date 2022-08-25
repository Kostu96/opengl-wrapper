#include "glw/texture.hpp"

#include <glad/gl.h>
//#include <stb/stb_image.h>

#include <cassert>

namespace glw {

    static uint32_t textureFormatToGLEnum(TextureFormat format)
    {
        switch (format)
        {
        case TextureFormat::RGBA8:           return GL_RGBA8;
        case TextureFormat::R32:             return GL_R32I;
        case TextureFormat::Depth24Stencil8: return GL_DEPTH24_STENCIL8;
        }

        return 0;
    }

    /*Texture::Texture(const char* path)
    {
        int width, height, channels;
        stbi_uc* data = stbi_load(path, &width, &height, &channels, 4);
        JNG_CORE_ASSERT(data, std::string{ "Failed to load image: " } + path);

        m_properties.specification.format = TextureFormat::RGBA8;
        m_properties.width = static_cast<u32>(width);
        m_properties.height = static_cast<u32>(height);

        createTexture();
        glTextureSubImage2D(m_id, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }*/

    Texture::Texture(const Properties& properties) :
        m_properties{ properties }
    {
        createTexture();
    }

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

        assert(width * height == size);

        glTextureSubImage2D(
            m_id, 0,
            xoffset, yoffset,
            width, height,
            GL_RGBA, GL_UNSIGNED_BYTE, data
        );
    }

    void Texture::createTexture()
    {
        glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
        glTextureStorage2D(m_id, 1,
            textureFormatToGLEnum(m_properties.specification.format),
            static_cast<int>(m_properties.width),
            static_cast<int>(m_properties.height)
        );

        glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(m_id, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

} // namespace jng
