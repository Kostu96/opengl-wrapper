#pragma once
#include <cut/auto_release.hpp>
#include <cut/non_copyable.hpp>
#include <cut/string_map.hpp>
#include <cut/types.hpp>

#include <glm/fwd.hpp>

#include <span>
#include <string_view>

namespace glw {

using cut::u32;
using cut::s32;
using cut::f32;

class Shader final :
    cut::NonCopyable {
public:
    enum class Type { Vertex, Fragment };

    Shader(Type type, std::span<const std::string_view> sources);

    u32 get_native_handle() const { return handle_.get(); }
private:
    cut::AutoRelease<u32> handle_;
};

class Program final :
    cut::NonCopyable {
public:
    Program(std::span<const Shader* const> shaders);

    void set_uniform_1i(std::string_view name, s32 value) const;
    void set_uniform_1f(std::string_view name, f32 value) const;
    void set_uniform_vec3f(std::string_view name, glm::vec3 value) const;
    void set_uniform_mat3f(std::string_view name, const glm::mat3& value) const;
    void set_uniform_mat4f(std::string_view name, const glm::mat4& value) const;

    void bind() const;
private:
    s32 get_uniform_location(std::string_view name) const;

    cut::AutoRelease<u32> handle_;
    cut::StringMap<s32> uniforms_;
};

} // namespace glw
