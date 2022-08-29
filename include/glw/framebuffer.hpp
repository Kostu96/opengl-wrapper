#pragma once
#include "glw/texture.hpp"

#include <cstdint>
#include <vector>

namespace glw {

    class Framebuffer final
    {
    public:
        struct Properties {
            uint32_t width;
            uint32_t height;
            uint32_t samples = 1;
            std::vector<TextureSpecification> attachmentsSpecifications;
            
            bool swapChainTarget = false;
        };

        explicit Framebuffer(const Properties& properties);
        ~Framebuffer();

        void bind() const;
        void unbind() const;

        void resize(uint32_t width, uint32_t height);
        uint32_t readPixel(uint32_t colorAttachmentIndex, uint32_t x, uint32_t y) const;
        void clearAttachment(uint32_t attachmentIndex, int value) const;
        void clearAttachment(uint32_t attachmentIndex, float value) const;
        
        const std::vector<Texture>& getAttachments() const { return m_attachments; }
        const Properties& getProperties() const { return m_properties; }

        Framebuffer(const Framebuffer&) = delete;
        Framebuffer& operator=(const Framebuffer&) = delete;
    private:
        void recreate();

        Properties m_properties;
        uint32_t m_ID;
        std::vector<Texture> m_attachments;
    };

} // namespace jng
