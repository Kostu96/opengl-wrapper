#include "glw/framebuffer.hpp"

#include <glad/gl.h>

#include <cassert>

namespace glw {

    static uint32_t textureFormatToGLEnum(TextureFormat format)
    {
        switch (format)
        {
        case TextureFormat::RGBA8: return GL_RGBA8;
        case TextureFormat::R32:   return GL_RED_INTEGER;
        }

        return 0;
    }

    static bool isDepthAttachment(TextureFormat format)
    {
        switch (format)
        {
        case TextureFormat::RGBA8:
        case TextureFormat::R32:
            return false;
        case TextureFormat::Depth24Stencil8:
            return true;
        }

        return false;
    }

    Framebuffer::Framebuffer(const Properties& properties) :
        m_properties{ properties }
    {
        recreate();
    }

    Framebuffer::~Framebuffer()
    {
        glDeleteFramebuffers(1, &m_ID);
    }

    void Framebuffer::bind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
        glViewport(0, 0, m_properties.width, m_properties.height);
        glClipControl(GL_UPPER_LEFT, GL_ZERO_TO_ONE);
    }

    void Framebuffer::unbind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);
    }

    void Framebuffer::resize(uint32_t width, uint32_t height)
    {
        m_properties.width = width;
        m_properties.height = height;

        recreate();
    }

    uint32_t Framebuffer::readPixel(uint32_t colorAttachmentIndex, uint32_t x, uint32_t y) const
    {
        glReadBuffer(GL_COLOR_ATTACHMENT0 + colorAttachmentIndex);

        uint32_t pixelData;
        glReadPixels(x, y, 1, 1, textureFormatToGLEnum(m_attachments[colorAttachmentIndex].getProperties().specification.format), GL_INT, &pixelData);

        return pixelData;
    }

    void Framebuffer::clearAttachment(uint32_t attachmentIndex, int value) const
    {
        const Texture& attachment = m_attachments[attachmentIndex];
        int data[]{ value };
        glClearTexImage(
            static_cast<GLuint>(reinterpret_cast<uintptr_t>(attachment.getRendererID())),
            0,
            textureFormatToGLEnum(attachment.getProperties().specification.format),
            GL_INT,
            data
        );
    }

    void Framebuffer::clearAttachment(uint32_t attachmentIndex, float value) const
    {
        const Texture& attachment = m_attachments[attachmentIndex];
        float data[]{ value };
        glClearTexImage(
            static_cast<GLuint>(reinterpret_cast<uintptr_t>(attachment.getRendererID())),
            1,
            textureFormatToGLEnum(attachment.getProperties().specification.format),
            GL_FLOAT,
            data
        );
    }

    void Framebuffer::recreate()
    {
        if (m_ID) {
            m_attachments.clear();
            glDeleteFramebuffers(1, &m_ID);
        }

        glCreateFramebuffers(1, &m_ID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

        m_attachments.reserve(m_properties.attachmentsSpecifications.size());
        uint32_t numColorAtachments = 0;
        for (uint32_t i = 0; i < m_properties.attachmentsSpecifications.size(); ++i)
        {
            bool isDepth = isDepthAttachment(m_properties.attachmentsSpecifications[i].format);
            if (!isDepth) numColorAtachments++;
            m_attachments.emplace_back(Texture::Properties{ m_properties.attachmentsSpecifications[i], m_properties.width, m_properties.height });
            GLenum attachmentTarget =  isDepth ? GL_DEPTH_STENCIL_ATTACHMENT : GL_COLOR_ATTACHMENT0 + i;
            glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentTarget, GL_TEXTURE_2D, m_attachments[i].getID(), 0);
        }

        assert(glCheckNamedFramebufferStatus(m_ID, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
        glNamedFramebufferDrawBuffers(m_ID, numColorAtachments, buffers);
    }

} // namespace jng
