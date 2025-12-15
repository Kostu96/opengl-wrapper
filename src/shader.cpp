#include "glw/shader.hpp"
#include "glw/glw.hpp"

#include <cut/exception.hpp>

namespace {

cut::u32 shaderTypeToOGLShaderType(glw::Shader::Type type) {
    switch (type) {
    using glw::Shader;
    case Shader::Type::Vertex:   return GL_VERTEX_SHADER;
    case Shader::Type::Fragment: return GL_FRAGMENT_SHADER;
    }

    throw cut::Exception("Unhandled Shader type!");
    return cut::to_u32(-1);
}

} // namespace

namespace glw {

Shader::Shader(std::string_view source, Type type) : 
    handle_(glCreateShader(shaderTypeToOGLShaderType(type)), glDeleteShader) {
    
    const GLchar* sources[]{ source.data() };
    const GLint lengths[]{ static_cast<GLint>(source.size()) };
    glShaderSource(handle_.get(), 1, sources, lengths);
    glCompileShader(handle_.get());

    int ret;
    glGetShaderiv(handle_.get(), GL_COMPILE_STATUS, &ret);
    if (!ret) {
        int length = 0;
        glGetShaderiv(handle_.get(), GL_INFO_LOG_LENGTH, &length);
        std::string infoLog;
        infoLog.resize(length);
        glGetShaderInfoLog(handle_.get(), length, nullptr, infoLog.data());
        throw cut::Exception(std::format("Could not compile shader! {}", infoLog));
    };
}

Program::Program(std::initializer_list<Shader> shaders) :
    handle_(glCreateProgram(), glDeleteProgram) {

    for (auto& shader : shaders) {
        glAttachShader(handle_.get(), shader.get_native_handle());
    }

    glLinkProgram(handle_.get());
    
    for (auto& shader : shaders) {
        glDetachShader(handle_.get(), shader.get_native_handle());
    }

    int success;
    glGetProgramiv(handle_.get(), GL_LINK_STATUS, &success);
    if (!success) {
        int length = 0;
        glGetProgramiv(handle_.get(), GL_INFO_LOG_LENGTH, &length);
        std::string infoLog;
        infoLog.resize(length);
        glGetProgramInfoLog(handle_.get(), length, nullptr, infoLog.data());
        throw cut::Exception(std::format("Could not link program! {}", infoLog));
    }
}

void Program::bind() const {
    glUseProgram(handle_.get());
}

} // namespace glw
