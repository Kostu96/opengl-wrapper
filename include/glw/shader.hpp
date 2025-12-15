#pragma once
#include <cut/auto_release.hpp>
#include <cut/non_copyable.hpp>
#include <cut/types.hpp>

#include <string_view>

namespace glw {

using cut::u32;

class Shader final :
    cut::NonCopyable {
public:
    enum class Type { Vertex, Fragment };

    Shader(std::string_view source, Type type);

    u32 get_native_handle() const { return handle_.get(); }
private:
    cut::AutoRelease<u32, void(*)(u32)> handle_;
};

class Program final :
    cut::NonCopyable {
public:
    Program(std::initializer_list<Shader> shaders);

    void bind() const;
private:
    cut::AutoRelease<u32, void(*)(u32)> handle_;
};

} // namespace glw
