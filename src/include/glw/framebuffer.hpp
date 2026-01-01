#pragma once
#include "glw/texture.hpp"

#include <cut/auto_release.hpp>
#include <cut/non_copyable.hpp>
#include <cut/types.hpp>

#include <span>
#include <vector>

namespace glw {

struct FramebufferDescription {
    u16 width;
    u16 height;
    std::vector<TextureFormat> attachments_formats;
};

class Framebuffer final :
    cut::NonCopyable {
public:
    explicit Framebuffer(const FramebufferDescription& desc);

    void bind() const;

    void resize(u16 width, u16 height);

    //uint32_t readPixel(uint32_t colorAttachmentIndex, uint32_t x, uint32_t y) const;
    //void clearAttachment(uint32_t attachmentIndex, int value) const;
    //void clearAttachment(uint32_t attachmentIndex, float value) const;
        
    const std::span<const Texture> get_attachments() const { return attachments_; }
    const FramebufferDescription& get_description() const { return desc_; }

    u32 get_native_handle() const { return handle_.get(); }

    static void bind_default();
private:
    void recreate();

    cut::AutoRelease<u32> handle_;
    FramebufferDescription desc_;
    std::vector<Texture> attachments_;
};

} // namespace glw
