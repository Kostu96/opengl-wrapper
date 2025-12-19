#include "glw/shader.hpp"
#include "glw/glw.hpp"

#include <cut/exception.hpp>

#include <glm/gtc/type_ptr.hpp>

#include <ranges>

namespace {

using namespace glw;

GLenum to_gl_enum(Shader::Type type) {
    switch (type) {
    case Shader::Type::Vertex:   return GL_VERTEX_SHADER;
    case Shader::Type::Fragment: return GL_FRAGMENT_SHADER;
    }

    throw cut::Exception("Unhandled Shader type!");
    return {};
}

} // namespace

namespace glw {

Shader::Shader(Type type, std::span<const std::string_view> sources) :
    handle_(glCreateShader(to_gl_enum(type)), glDeleteShader) {

    std::vector<std::string> line_strs;
    std::vector<const GLchar*> gl_sources;
    std::vector<GLint> gl_lengths;

    line_strs.reserve(sources.size() - 1);
    gl_sources.reserve(sources.size() * 2 - 1);
    gl_lengths.reserve(sources.size() * 2 - 1);
    for (auto [i, sv] : std::views::enumerate(sources)) {
        if (i > 0) {
            line_strs.push_back(std::format("\n#line 0 {}\n", i));
            gl_sources.push_back(line_strs.back().data());
            gl_lengths.push_back(line_strs.back().size());
        }

        gl_sources.push_back(sv.data());
        gl_lengths.push_back(sv.size());
    }

    glShaderSource(handle_.get(), gl_sources.size(), gl_sources.data(), gl_lengths.data());
    glCompileShader(handle_.get());

    GLint success;
    glGetShaderiv(handle_.get(), GL_COMPILE_STATUS, &success);
    if (success != GL_TRUE) {
        GLint length = 0;
        glGetShaderiv(handle_.get(), GL_INFO_LOG_LENGTH, &length);
        std::string infoLog;
        infoLog.resize(length);
        glGetShaderInfoLog(handle_.get(), length, nullptr, infoLog.data());
        throw cut::Exception(std::format("Could not compile shader! {}", infoLog));
    };
}

Program::Program(const std::initializer_list<Shader>& shaders) :
    handle_(glCreateProgram(), glDeleteProgram) {

    for (auto& shader : shaders) {
        glAttachShader(handle_.get(), shader.get_native_handle());
    }

    glLinkProgram(handle_.get());
    
    for (auto& shader : shaders) {
        glDetachShader(handle_.get(), shader.get_native_handle());
    }

    GLint success;
    glGetProgramiv(handle_.get(), GL_LINK_STATUS, &success);
    if (success != GL_TRUE) {
        GLint length = 0;
        glGetProgramiv(handle_.get(), GL_INFO_LOG_LENGTH, &length);
        std::string infoLog(length, '\0');
        glGetProgramInfoLog(handle_.get(), length, nullptr, infoLog.data());
        throw cut::Exception(std::format("Could not link program! {}", infoLog));
    }

    GLint uniform_count;
    glGetProgramiv(handle_.get(), GL_ACTIVE_UNIFORMS, &uniform_count);
    if (uniform_count != 0) {
        GLint max_length;
        glGetProgramiv(handle_.get(), GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_length);
        std::string uniform_name;
        for (GLint i = 0; i < uniform_count; i++) {
            uniform_name.resize(max_length);
            GLint length;
            GLint size;
            GLenum type;
            glGetActiveUniform(handle_.get(), i, max_length, &length, &size, &type, uniform_name.data());
            uniform_name.resize(length);
            GLint location = glGetUniformLocation(handle_.get(), uniform_name.c_str());
            uniforms_[uniform_name] = location;
        }
    }
}

void Program::set_uniform_1i(std::string_view name, s32 value) const {
    glProgramUniform1i(handle_.get(), get_uniform_location(name), value);
}

void Program::set_uniform_1f(std::string_view name, f32 value) const {
    glProgramUniform1f(handle_.get(), get_uniform_location(name), value);
}

void Program::set_uniform_vec3f(std::string_view name, glm::vec3 value) const {
    glProgramUniform3fv(handle_.get(), get_uniform_location(name), 1, glm::value_ptr(value));
}

void Program::set_uniform_mat3f(std::string_view name, const glm::mat3& value) const {
    glProgramUniformMatrix3fv(handle_.get(), get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(value));
}

void Program::set_uniform_mat4f(std::string_view name, const glm::mat4& value) const {
    glProgramUniformMatrix4fv(handle_.get(), get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(value));
}

void Program::bind() const {
    glUseProgram(handle_.get());
}

s32 Program::get_uniform_location(std::string_view name) const {
    auto loc = uniforms_.find(name);
    cut::ensure(loc != uniforms_.end(), "Uniform {} not in the cache!", name);
    return loc->second;
}

} // namespace glw
