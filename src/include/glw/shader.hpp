#pragma once
#include <cut/auto_release.hpp>
#include <cut/non_copyable.hpp>
#include <cut/types.hpp>

#include <glm/glm.hpp>

#include <string>
#include <string_view>
#include <unordered_map>

namespace glw {

using cut::u32;
using cut::s32;

class Shader final :
    cut::NonCopyable {
public:
    enum class Type { Vertex, Fragment };

    Shader(std::string_view source, Type type);

    u32 get_native_handle() const { return handle_.get(); }
private:
    cut::AutoRelease<u32> handle_;
};

class Program final :
    cut::NonCopyable {
public:
    Program(const std::initializer_list<Shader>& shaders);

    void set_uniform_mat4f(std::string_view name, const glm::mat4& value) const;

    void bind() const;
private:
    struct TransparentHash {
        using is_transparent = void;

        template<typename T>
        std::size_t operator()(const T& v) const noexcept {
            return std::hash<std::string_view>{}(v);
        }
    };

    struct TransparentEqual {
        using is_transparent = void;

        template<typename A, typename B>
        bool operator()(const A& lhs, const B& rhs) const noexcept {
            return std::string_view(lhs) == std::string_view(rhs);
        }
    };

    s32 get_uniform_location(std::string_view name) const;

    cut::AutoRelease<u32> handle_;
    std::unordered_map<std::string, s32, TransparentHash, TransparentEqual> uniforms_;
};

} // namespace glw
