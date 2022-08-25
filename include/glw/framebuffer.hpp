/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/core/base.hpp"
#include "jng/renderer/texture.hpp"

namespace jng {

    class Framebuffer final
    {
    public:
        struct Properties {
            u32 width;
            u32 height;
            u32 samples = 1;
            std::vector<TextureSpecification> attachmentsSpecifications;
            
            bool swapChainTarget = false;
        };

        explicit Framebuffer(const Properties& properties);
        ~Framebuffer();

        void bind() const;
        void unbind() const;

        void resize(u32 width, u32 height);
        u32 readPixel(u32 colorAttachmentIndex, u32 x, u32 y) const;
        void clearAttachment(u32 attachmentIndex, int value) const;
        void clearAttachment(u32 attachmentIndex, float value) const;
        
        const std::vector<Ref<Texture>>& getAttachments() const { return m_attachments; }
        const Properties& getProperties() const { return m_properties; }
    private:
        void recreate();

        Properties m_properties;
        u32 m_ID;
        std::vector<Ref<Texture>> m_attachments;
        u32 m_colorAttachmentID;
        u32 m_depthAttachmentID;
    };

} // namespace jng
