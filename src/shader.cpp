#include "glw/shader.hpp"
#include "glw/glw.hpp"

#include <cut/exception.hpp>

#include <glm/gtc/type_ptr.hpp>

#include <print>

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

Shader::Shader(std::string_view source, Type type) : 
    handle_(glCreateShader(to_gl_enum(type)), glDeleteShader) {
    
    const GLchar* sources[]{ source.data() };
    const GLint lengths[]{ static_cast<GLint>(source.size()) };
    glShaderSource(handle_.get(), 1, sources, lengths);
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
