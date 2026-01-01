#include "glw/framebuffer.hpp"
#include "glw/glw.hpp"

#include <cut/exception.hpp>

namespace {

using namespace glw;

/*GLenum to_gl_enum(TextureFormat format) {
    switch (format) {
    case TextureFormat::RGBA8: return GL_RGBA8;
    case TextureFormat::R32U:  return GL_RED_INTEGER;
    default:
        return 0;
    }
}*/

bool is_depth_attachment(TextureFormat format) {
    switch (format) {
    using enum TextureFormat;
    case RGBA8:
    case R32U:
        return false;
    case Depth24Stencil8:
        return true;
    }

    throw cut::Exception("Unhandled texture format!");
    return {};
}

} // namespace

namespace glw {

Framebuffer::Framebuffer(const FramebufferDescription& desc) :
    handle_(0u, [](u32 handle) { glDeleteFramebuffers(1, &handle); }),
    desc_(desc)
{
    recreate();
}

void Framebuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, handle_.get());
}

void Framebuffer::resize(u16 width, u16 height) {
    desc_.width = width;
    desc_.height = height;

    recreate();
}

/*uint32_t Framebuffer::readPixel(uint32_t colorAttachmentIndex, uint32_t x, uint32_t y) const
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
}*/

void Framebuffer::bind_default() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::recreate() {
    GLuint handle;
    glCreateFramebuffers(1, &handle);
    handle_.reset(handle);
    attachments_.clear();

    attachments_.reserve(desc_.attachments_formats.size());
    int num_color_atachments = 0;
    for (int i = 0; i < desc_.attachments_formats.size(); ++i)
    {
        bool is_depth = is_depth_attachment(desc_.attachments_formats[i]);
        if (!is_depth)
            num_color_atachments++;

        attachments_.emplace_back(TextureDescription{
            .type = TextureType::Texture2D,
            .format = desc_.attachments_formats[i],
            .width = desc_.width,
            .height = desc_.height
        });
        GLenum attachment_target =  is_depth ? GL_DEPTH_STENCIL_ATTACHMENT : GL_COLOR_ATTACHMENT0 + i;
        glNamedFramebufferTexture(handle_.get(), attachment_target, attachments_[i].get_native_handle(), 0);
    }

    cut::ensure(glCheckNamedFramebufferStatus(handle_.get(), GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
        "Framebuffer is not complete!");

    constexpr size_t max_color_attachments = 8;
    cut::ensure(num_color_atachments <= max_color_attachments, "Too much color attachments!");

    GLenum buffers[max_color_attachments] = {
        GL_COLOR_ATTACHMENT0,
        GL_COLOR_ATTACHMENT1,
        GL_COLOR_ATTACHMENT2,
        GL_COLOR_ATTACHMENT3,
        GL_COLOR_ATTACHMENT4,
        GL_COLOR_ATTACHMENT5,
        GL_COLOR_ATTACHMENT6,
        GL_COLOR_ATTACHMENT7 };
    glNamedFramebufferDrawBuffers(handle_.get(), num_color_atachments, buffers);
}

} // namespace glw
