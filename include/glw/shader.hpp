/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/core/base.hpp"

namespace jng {

    class Shader final
    {
    public:
        enum class Type {
            Vertex,
            Fragment
        };

        Shader(const std::filesystem::path& vertexShaderFilename, const std::filesystem::path& fragmentShaderFilename);
        ~Shader();

        void bind() const;
        void unbind() const;
    private:
        std::filesystem::path getCacheDirectory() const;
        void createCacheDirectoryIfNeeded() const;
        std::vector<u32> compileToSPIRV(const std::filesystem::path& filename, Type type) const;
        static const char* shaderTypeToHashFileExtension(Type type);
        static const char* shaderTypeToCachedVlkFileExtension(Type type);
        static u32 shaderTypeToShaderCKind(Type type);

        u32 compileShader(const std::filesystem::path& filename, Type type) const;

        mutable bool m_isCacheDirty = true;
        u32 m_id;
    };

} // namespace jng
