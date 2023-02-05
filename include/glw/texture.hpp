#pragma once
#include <cstddef>
#include <cstdint>

namespace glw {

    enum class TextureFormat {
        None,
        
        RGBA8, // normalized integer
        R32,   // unsigned integer

        Depth24Stencil8
    };

    enum class TextureFilter {
        Linear,
        Nearest
    };

    enum class TextureWrapMode {
        Clamp,
        Repeat
    };

    struct TextureSpecification {
        TextureFormat format;
        TextureFilter minificationFilter = TextureFilter::Linear;
        TextureFilter magnificationFilter = TextureFilter::Linear;
        TextureWrapMode wrapMode = TextureWrapMode::Clamp;

        TextureSpecification(TextureFormat inFormat) : format{ inFormat } {}
        TextureSpecification(TextureFormat inFormat, TextureFilter inMinFilter, TextureFilter inMagFilter, TextureWrapMode inWrapMode) :
            format{ inFormat }, minificationFilter{ inMinFilter }, magnificationFilter{ inMagFilter }, wrapMode{ inWrapMode } {}
    };

    class Texture final
    {
    public:
        struct Properties {
            TextureSpecification specification = TextureFormat::None;
            uint32_t width;
            uint32_t height;
        };

        explicit Texture(const char* path, bool flip = false);
        explicit Texture(const Properties& properties);
        Texture(Texture&& other) noexcept;
        ~Texture();

        void bind(uint32_t slot) const;
        void unbind(uint32_t slot) const;
        void setData(const void* data, size_t size, uint32_t xoffset = 0, uint32_t yoffset = 0, uint32_t width = 0, uint32_t height = 0) const;

        uint32_t getID() const { return m_id; }
        const Properties& getProperties() const { return m_properties; }
        const void* getRendererID() const { return reinterpret_cast<const void*>(static_cast<uintptr_t>(m_id)); }

        Texture(const Texture&) = delete;
        Texture& operator=(const Texture&) = delete;
    private:
        void createTexture();

        Properties m_properties;
        uint32_t m_id;
    };

} // namespace glw
